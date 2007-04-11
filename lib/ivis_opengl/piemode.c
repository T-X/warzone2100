/*
	This file is part of Warzone 2100.
	Copyright (C) 1999-2004  Eidos Interactive
	Copyright (C) 2005-2007  Warzone Resurrection Project

	Warzone 2100 is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	Warzone 2100 is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Warzone 2100; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/
/***************************************************************************/
/*
 * pieMode.h
 *
 * renderer control for pumpkin library functions.
 *
 */
/***************************************************************************/

#include "lib/framework/frame.h"

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include "lib/ivis_common/piedef.h"
#include "lib/ivis_common/piestate.h"
#include "lib/ivis_common/piemode.h"
#include "piematrix.h"
#include "lib/ivis_common/piefunc.h"
#include "lib/ivis_common/tex.h"
#include "lib/ivis_common/rendmode.h"
#include "lib/ivis_common/pieclip.h"
#include "screen.h"

/***************************************************************************/
/*
 *	Local Definitions
 */
/***************************************************************************/
#define DIVIDE_TABLE_SIZE		1024

/***************************************************************************/
/*
 *	Local Variables
 */
/***************************************************************************/

Sint32		_iVPRIM_DIVTABLE[DIVIDE_TABLE_SIZE];


/***************************************************************************/
/*
 *	Local ProtoTypes
 */
/***************************************************************************/
extern void screenDoDumpToDiskIfRequired(void);

/***************************************************************************/
/*
 *	Source
 */
/***************************************************************************/

BOOL pie_Initialise(void)
{
	int i;

	pie_InitMaths();
	pie_TexInit();

	rendSurface.usr = REND_UNDEFINED;
	rendSurface.flags = REND_SURFACE_UNDEFINED;
	rendSurface.buffer = NULL;
	rendSurface.size = 0;

	// divtable: first entry == unity to (ie n/0 == 1 !)
	_iVPRIM_DIVTABLE[0] = iV_DIVMULTP;

	for (i=1; i<DIVIDE_TABLE_SIZE; i++)
	{
		_iVPRIM_DIVTABLE[i-0] = MAKEINT ( FRACTdiv(MAKEFRACT(1),MAKEFRACT(i)) *  iV_DIVMULTP);
	}

	/* Find texture compression extension */
	if (check_extension("GL_ARB_texture_compression"))
	{
		debug(LOG_TEXTURE, "Texture compression: Yes");
		wz_texture_compression = GL_COMPRESSED_RGBA_ARB;
	} else {
		debug(LOG_TEXTURE, "Texture compression: No");
		wz_texture_compression = GL_RGBA;
	}

	pie_MatInit();
	_TEX_INDEX = 0;

	rendSurface.buffer	= 0;
	rendSurface.flags	= REND_SURFACE_SCREEN;
	rendSurface.width	= pie_GetVideoBufferWidth();
	rendSurface.height	= pie_GetVideoBufferHeight();
	rendSurface.xcentre	= pie_GetVideoBufferWidth()/2;
	rendSurface.ycentre	= pie_GetVideoBufferHeight()/2;
	rendSurface.clip.left	= 0;
	rendSurface.clip.top	= 0;
	rendSurface.clip.right	= pie_GetVideoBufferWidth();
	rendSurface.clip.bottom	= pie_GetVideoBufferHeight();
	rendSurface.xpshift	= 10;
	rendSurface.ypshift	= 10;

	pie_SetDefaultStates();
	iV_RenderAssign(&rendSurface);

	return TRUE;
}


void pie_ShutDown(void) {
	rendSurface.buffer = NULL;
	rendSurface.flags = REND_SURFACE_UNDEFINED;
	rendSurface.usr = REND_UNDEFINED;
	rendSurface.size = 0;

	pie_CleanUp();
}

/***************************************************************************/

void pie_ScreenFlip(int clearMode) {
	GLbitfield clearFlags = 0;

	screenDoDumpToDiskIfRequired();
	SDL_GL_SwapBuffers();
	switch (clearMode&CLEAR_MODE_MASK) {
		case CLEAR_OFF_AND_NO_BUFFER_DOWNLOAD:
			break;
		case CLEAR_BLACK:
			glDepthMask(GL_TRUE);
			glClearColor(0.0f,0.0f,0.0f,0.0f);
			clearFlags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
			break;
		default:
			glDepthMask(GL_TRUE);
			clearFlags = GL_DEPTH_BUFFER_BIT;
			break;
	}
	if(clearMode&CLEAR_SHADOW_MASK)
	{
		clearFlags |= GL_STENCIL_BUFFER_BIT;
	}
	if(clearFlags)
	{
		glClear(clearFlags);
	}
	if (screen_GetBackDrop()) {
		screen_Upload(NULL);
	}
}

/***************************************************************************/
UDWORD	pie_GetResScalingFactor( void ) {
//	UDWORD	resWidth;	//n.b. resolution width implies resolution height...!

	if (pie_GetVideoBufferWidth() * 4 > pie_GetVideoBufferHeight() * 5) {
		return pie_GetVideoBufferHeight()*5/4/6;
	} else {
		return pie_GetVideoBufferWidth()/6;
	}
}

