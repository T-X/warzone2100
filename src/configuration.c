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
/*
 *Config.c  saves your favourite options to the Registry.
 *
 */
#include <string.h>

#include "lib/framework/frame.h"
#include "lib/framework/strres.h"

#include "configuration.h"
#include "lib/framework/configfile.h"
#include "objmem.h"
#include "display.h"	// gammaValue
#include "lib/sound/sound.h"		// audio
#include "lib/ivis_common/piestate.h"	// setgamma.
#include "warzoneconfig.h"	// renderMode
#include "component.h"
#include "text.h"
#include "seqdisp.h"
#include "difficulty.h"
#include "lib/netplay/netplay.h"
#include "display3d.h"
#include "multiplay.h"
#include "ai.h"
#include "advvis.h"
#include "hci.h"
#include "fpath.h"
#include "radar.h"
// HACK bAllowDebugMode shouldn't be in clparse
#include "clparse.h"

// ////////////////////////////////////////////////////////////////////////////

#define DEFAULTFXVOL	80
#define DEFAULTCDVOL	60
#define DEFAULTGAMMA	20
#define DEFAULTSCROLL	800
#define DEFAULTMAPNAME	"Rush"

extern  char	sForceName[256];
extern	UBYTE	sPlayer[128];

extern void registry_clear(void); // from configfile.c


// ////////////////////////////////////////////////////////////////////////////
BOOL loadConfig(void)
{
	SDWORD	val;
	char	sBuf[255];

	openWarzoneKey();

	//  options screens.
	// //////////////////////////

	// //////////////////////////
	// subtitles
	if(getWarzoneKeyNumeric("allowsubtitles", &val))
	{
		war_SetAllowSubtitles(val);
	}

	// //////////////////////////
	// voice vol
	if(getWarzoneKeyNumeric("voicevol", &val))
	{
		mixer_SetWavVolume((SDWORD)val);//was val
	}

	// //////////////////////////
	// fx vol
	if(getWarzoneKeyNumeric("fxvol", &val))
	{
		mixer_Set3dWavVolume((SDWORD)val);//was val
	}

	// //////////////////////////
	// cdvol
	if(getWarzoneKeyNumeric("cdvol", &val))
	{
		mixer_SetCDVolume((SDWORD)val);
	}

	if (getWarzoneKeyNumeric("playaudiocds", &val)) {
		war_SetPlayAudioCDs(val);
	}

	if (getWarzoneKeyNumeric("debugmode", &val))
	{
		bAllowDebugMode = val;
	}
	else
	{
		bAllowDebugMode = FALSE;
		setWarzoneKeyNumeric("debugmode", FALSE);
	}

	if (getWarzoneKeyNumeric("framerate", &val))
	{
		setFramerateLimit(val);
	}
	else
	{
		setFramerateLimit(60);
		setWarzoneKeyNumeric("framerate", 60);
	}

	// //////////////////////////
	// gamma
	if (getWarzoneKeyNumeric("gamma", &val))
	{
		gammaValue = val;
		if (gammaValue < 10)
			gammaValue = 10;
// 		pie_SetGammaValue((float)gammaValue / 20.0f);
	}
	else
	{
		gammaValue = DEFAULTGAMMA;
		if (gammaValue < 10)
			gammaValue = 10;
// 		pie_SetGammaValue((float)gammaValue / 20.0f);
		setWarzoneKeyNumeric("gamma", gammaValue);
	}

	// //////////////////////////
	// scroll
	if(getWarzoneKeyNumeric("scroll", &val))
	{
		scroll_speed_accel = val;
	}
	else
	{
		scroll_speed_accel = DEFAULTSCROLL;
		setWarzoneKeyNumeric("scroll", DEFAULTSCROLL);
	}

	// //////////////////////////
	// screen shake
	if(getWarzoneKeyNumeric("shake", &val))
	{
		setShakeStatus(val);
	}
	else
	{
		setShakeStatus(TRUE);
		setWarzoneKeyNumeric("shake", TRUE);
	}

	// //////////////////////////
	// draw shadows
	if(getWarzoneKeyNumeric("shadows", &val))
	{
		setDrawShadows(val);
	}
	else
	{
		setDrawShadows(TRUE);
		setWarzoneKeyNumeric("shadows", TRUE);
	}

	// //////////////////////////
	// enable sound
	if(getWarzoneKeyNumeric("sound", &val))
	{
		war_setSoundEnabled( val );
	}
	else
	{
		war_setSoundEnabled( TRUE );
		setWarzoneKeyNumeric( "sound", TRUE );
	}

	// //////////////////////////
	// invert mouse
	if(getWarzoneKeyNumeric("mouseflip", &val))
	{
		setInvertMouseStatus(val);
	}
	else
	{
		setInvertMouseStatus(TRUE);
		setWarzoneKeyNumeric("mouseflip", TRUE);
	}

	if (getWarzoneKeyString("masterserver_name", sBuf))
	{
		NETsetMasterserverName(sBuf);
	}
	else
	{
		NETsetMasterserverName("lobby.wz2100.net");
		setWarzoneKeyString("masterserver_name", "lobby.wz2100.net");
	}

	if (getWarzoneKeyNumeric("masterserver_port", &val))
	{
		NETsetMasterserverPort(val);
	}
	else
	{
		NETsetMasterserverPort(9998);
		setWarzoneKeyNumeric("masterserver_port", 9998);
	}

	if (getWarzoneKeyNumeric("gameserver_port", &val))
	{
		NETsetGameserverPort(val);
	}
	else
	{
		NETsetGameserverPort(9999);
		setWarzoneKeyNumeric("gameserver_port", 9999);
	}

	// //////////////////////////
	// sequences
	if(getWarzoneKeyNumeric("sequences", &val))
	{
		war_SetSeqMode(val);
	}
	else
	{
		war_SetSeqMode(SEQ_FULL);
	}

	// //////////////////////////
	// subtitles
	if(getWarzoneKeyNumeric("subtitles", &val))
	{
		seq_SetSubtitles(val);
	}
	else
	{
		seq_SetSubtitles(TRUE);
	}

	// //////////////////////////
	// difficulty

	if(getWarzoneKeyNumeric("difficulty", &val))
	{
		setDifficultyLevel(val);
	}
	else
	{
		setDifficultyLevel(DL_NORMAL);
		setWarzoneKeyNumeric("difficulty", DL_NORMAL);
	}


	if(getWarzoneKeyNumeric("barmode", &val)&&(val<3)&&(val>=0))
	{
		barMode = val;
	}
	else
	{
		barMode = BAR_FULL;
//		setDifficultyLevel(DL_NORMAL);
		setWarzoneKeyNumeric("barmode", BAR_FULL);
	}


	// //////////////////////////
	// use vis fog
	if(getWarzoneKeyNumeric("visfog", &val))
	{
		if(val)
		{
			war_SetFog(FALSE);
			avSetStatus(TRUE);
		}
		else
		{
			avSetStatus(FALSE);
			war_SetFog(TRUE);
		}
	}
	else
	{
		avSetStatus(FALSE);
		war_SetFog(TRUE);
		setWarzoneKeyNumeric("visfog", 0);
	}

	// //////////////////////////
	// favourite colour
	if(!bMultiPlayer)
	{
		initPlayerColours();	// clear current maps.
		if(getWarzoneKeyNumeric("colour", &val))
		{
			setPlayerColour(0, val);
		}
		else
		{
			setPlayerColour(0, 0);
			setWarzoneKeyNumeric("colour", 0);
		}
	}


	// reopen the build menu after placing a structure
	if(getWarzoneKeyNumeric("reopenBuild", &val))
	{
		intReopenBuild(val);
	}
	else
	{
		intReopenBuild(FALSE);
		setWarzoneKeyNumeric("reopenBuild", FALSE);
	}

	// the maximum route processing per frame
/*	if(getWarzoneKeyNumeric("maxRoute", &val))
	{
		fpathSetMaxRoute(val);
	}
	else
	{
		fpathSetMaxRoute(FPATH_MAX_ROUTE_INIT);
		setWarzoneKeyNumeric("maxRoute", FPATH_MAX_ROUTE_INIT);
	}*/

	// //////////////////////////
	//	getWarzoneKey("mouse", &val, 0);		// mouse
	//	multitype // alliance // power // base // limits // tech
	// keymaps

	// /////////////////////////
	//  multiplayer stuff.
	// /////////////////////////

	// game name
	if (getWarzoneKeyString("gameName", sBuf))
	{
		strcpy(game.name, sBuf);
	}
	else
	{
		setWarzoneKeyString("gameName", "My Game");
	}

	// player name
	if (getWarzoneKeyString("playerName", sBuf))
	{
		strcpy((char*)sPlayer, sBuf);
	}
	else
	{
		setWarzoneKeyString("playerName","Player");
	}

	// map name
	if(getWarzoneKeyString("mapName", sBuf))
	{
		strcpy(game.map, sBuf);
	}
	else
	{
		strcpy(game.map, DEFAULTMAPNAME);
		setWarzoneKeyString("mapName", game.map);
	}

	// power
	if(getWarzoneKeyNumeric("power", &val))
	{
		game.power = val;
	}
	else
	{
		game.power = LEV_MED;
		setWarzoneKeyNumeric("power", game.power);
	}

	// fog
	if(getWarzoneKeyNumeric("fog", &val))
	{
		game.fog= val;
	}
	else
	{
		game.fog= TRUE;
		setWarzoneKeyNumeric("fog", game.fog);
	}

	//type
	if(getWarzoneKeyNumeric("type", &val))
	{
		game.type =(UBYTE)val;
	}
	else
	{
		game.type = CAMPAIGN;
		setWarzoneKeyNumeric("type", game.type);
	}

	//base
	if(getWarzoneKeyNumeric("base", &val))
	{
		game.base =(UBYTE)val;
	}
	else
	{
		game.base = CAMP_BASE;
		setWarzoneKeyNumeric("base", game.base);
	}

	//limit
	if(getWarzoneKeyNumeric("limit", &val))
	{
		game.limit=(UBYTE)val;
	}
	else
	{
		game.limit = NOLIMIT;
		setWarzoneKeyNumeric("limit", game.limit);
	}

	//maxplay
	if(getWarzoneKeyNumeric("maxPlay", &val))
	{
		game.maxPlayers =(UBYTE)val;
	}
	else
	{
		game.maxPlayers = 4;
		setWarzoneKeyNumeric("maxPlay", game.maxPlayers);
	}

	//compplay
	if(getWarzoneKeyNumeric("compPlay", &val))
	{
		game.bComputerPlayers= val;
	}
	else
	{
		game.bComputerPlayers = FALSE;
		setWarzoneKeyNumeric("compPlay", game.bComputerPlayers);
	}

	//alliance
	if(getWarzoneKeyNumeric("alliance", &val))
	{
		game.alliance =(UBYTE)val;
	}
	else
	{
		game.alliance = NO_ALLIANCES;
		setWarzoneKeyNumeric("alliance", game.alliance);
	}

	// force name
	if(getWarzoneKeyString("forceName", sBuf))
	{
		strcpy(sForceName, sBuf);
	}
	else
	{
		strcpy(sForceName, "Default");
		setWarzoneKeyString("forceName", sForceName);
	}


	// favourite phrases
	if(getWarzoneKeyString("phrase0", ingame.phrases[0]))
	{
		getWarzoneKeyString("phrase1", ingame.phrases[1]);
		getWarzoneKeyString("phrase2", ingame.phrases[2]);
		getWarzoneKeyString("phrase3", ingame.phrases[3]);
		getWarzoneKeyString("phrase4", ingame.phrases[4]);
	}
	else
	{
		memset(&ingame.phrases, 0, sizeof(ingame.phrases));
		setWarzoneKeyString("phrase0", ingame.phrases[0]);
		setWarzoneKeyString("phrase1", ingame.phrases[1]);
		setWarzoneKeyString("phrase2", ingame.phrases[2]);
		setWarzoneKeyString("phrase3", ingame.phrases[3]);
		setWarzoneKeyString("phrase4", ingame.phrases[4]);
	}

	// enemy/allies radar view
	if(getWarzoneKeyNumeric("radarObjectMode", &val))
	{
		bEnemyAllyRadarColor =(BOOL)val;
	} else {
		bEnemyAllyRadarColor = FALSE;
		setWarzoneKeyNumeric("radarObjectMode", (SDWORD)bEnemyAllyRadarColor);
	}

	// mini-map terrain mode
	if(getWarzoneKeyNumeric("radarTerrainMode", &val))
	{
		radarDrawMode = val;

		if(radarDrawMode >= NUM_RADAR_MODES){
			ASSERT(FALSE, "loadConfig: wrong mini-map mode: %d", radarDrawMode);
			radarDrawMode = RADAR_MODE_DEFAULT;
		}
	} else {
		radarDrawMode = RADAR_MODE_DEFAULT;
		setWarzoneKeyNumeric("radarTerrainMode", radarDrawMode);
	}

	return closeWarzoneKey();
}

BOOL loadRenderMode(void)
{
	SDWORD val;

	if( !openWarzoneKey() ) {
		return FALSE;
	}

	if( getWarzoneKeyNumeric("fullscreen", &val) ) {
		war_setFullscreen(val);
	}

	// now load the desired res..
	// note that we only do this if we havent changed renderer..
	if ( getWarzoneKeyNumeric("width", &val) )
		pie_SetVideoBufferWidth(val);
	if ( getWarzoneKeyNumeric("height", &val) )
		pie_SetVideoBufferHeight(val);
	if ( getWarzoneKeyNumeric("bpp", &val) )
		pie_SetVideoBufferDepth(val);

	return closeWarzoneKey();
}

// ////////////////////////////////////////////////////////////////////////////
BOOL saveConfig(void)
{
	debug( LOG_WZ, "Writing prefs to registry\n" );

	if(!openWarzoneKey())
	{
		return FALSE;
	}

	// //////////////////////////
	// voicevol, fxvol and cdvol
	setWarzoneKeyNumeric("voicevol", mixer_GetWavVolume());
	setWarzoneKeyNumeric("fxvol", mixer_Get3dWavVolume());
	setWarzoneKeyNumeric("cdvol", mixer_GetCDVolume());
	setWarzoneKeyNumeric("playaudiocds", war_GetPlayAudioCDs());

	setWarzoneKeyNumeric("width", pie_GetVideoBufferWidth());
	setWarzoneKeyNumeric("height", pie_GetVideoBufferHeight());
	setWarzoneKeyNumeric("bpp", pie_GetVideoBufferDepth());
	setWarzoneKeyNumeric("fullscreen", war_getFullscreen());

	// dont save out the cheat mode.
	if(getDifficultyLevel()==DL_KILLER || getDifficultyLevel()== DL_TOUGH)
	{
		setDifficultyLevel(DL_NORMAL);
	}
	setWarzoneKeyNumeric("allowSubtitles", war_GetAllowSubtitles());
	setWarzoneKeyNumeric("debugmode", bAllowDebugMode);
	setWarzoneKeyNumeric("framerate", (SDWORD)getFramerateLimit());
	setWarzoneKeyNumeric("gamma", (SDWORD)gammaValue);
	setWarzoneKeyNumeric("scroll",(SDWORD)scroll_speed_accel);		// scroll
	setWarzoneKeyNumeric("difficulty", getDifficultyLevel());		// level
	setWarzoneKeyNumeric("barmode",(SDWORD)barMode);			//energybars
	setWarzoneKeyNumeric("visfog",(SDWORD)(!war_GetFog()));			// fogtype
	setWarzoneKeyNumeric("shake",(SDWORD)(getShakeStatus()));		// screenshake
	setWarzoneKeyNumeric("mouseflip",(SDWORD)(getInvertMouseStatus()));	// flipmouse
	setWarzoneKeyNumeric("shadows",(SDWORD)(getDrawShadows()));	// shadows
	setWarzoneKeyNumeric("sound", (SDWORD)war_getSoundEnabled());
	setWarzoneKeyNumeric("sequences",(SDWORD)(war_GetSeqMode()));		// sequences
	setWarzoneKeyNumeric("subtitles",(SDWORD)(seq_GetSubtitles()));		// subtitles
	setWarzoneKeyNumeric("reopenBuild",(SDWORD)(intGetReopenBuild()));	// build menu
//	setWarzoneKeyNumeric("maxRoute",(SDWORD)(fpathGetMaxRoute()));			// maximum routing

	setWarzoneKeyNumeric("radarObjectMode",(SDWORD)bEnemyAllyRadarColor);    // enemy/allies radar view
	setWarzoneKeyNumeric("radarTerrainMode",(SDWORD)radarDrawMode);

	if(!bMultiPlayer)
	{
		setWarzoneKeyNumeric("colour",(SDWORD)getPlayerColour(0));			// favourite colour.
	}
	else
	{
		debug( LOG_NEVER, "Writing multiplay prefs to registry\n" );
		if(NetPlay.bHost && ingame.localJoiningInProgress)
		{
			setWarzoneKeyString("gameName", game.name);				//  last hosted game
		}
		setWarzoneKeyString("mapName", game.map);				//  map name
		setWarzoneKeyNumeric("power", game.power);			// power
		setWarzoneKeyNumeric("type", game.type);				// game type
		setWarzoneKeyNumeric("base", game.base);				// size of base
		setWarzoneKeyNumeric("fog", game.fog);				// fog 'o war
		setWarzoneKeyNumeric("limit", game.limit);			// limits
		setWarzoneKeyNumeric("maxPlay", game.maxPlayers);		// max no of players
		setWarzoneKeyNumeric("compPlay", game.bComputerPlayers);	// allow pc players
		setWarzoneKeyNumeric("alliance", game.alliance);			// allow alliances
		setWarzoneKeyString("forceName", sForceName);			// force
		setWarzoneKeyString("playerName",(char*)sPlayer);		// player name
		setWarzoneKeyString("phrase0", ingame.phrases[0]);	// phrases
		setWarzoneKeyString("phrase1", ingame.phrases[1]);
		setWarzoneKeyString("phrase2", ingame.phrases[2]);
		setWarzoneKeyString("phrase3", ingame.phrases[3]);
		setWarzoneKeyString("phrase4", ingame.phrases[4]);
	}

	return closeWarzoneKey();
}

void closeConfig( void )
{
	registry_clear();
}
