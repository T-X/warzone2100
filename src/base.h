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
 * base.h
 *
 * Definitions for the base object type.
 *
 */
/***************************************************************************/

#ifndef _base_h
#define _base_h

/***************************************************************************/

#include "deliverance.h"
#include "displaydef.h"

#define TURRET_ROTATION_RATE    360
/***************************************************************************/

typedef enum _object_type
{
	OBJ_DROID,		// Droids
	OBJ_STRUCTURE,	// All Buildings
	OBJ_FEATURE,	// Things like roads, trees, bridges, fires
	OBJ_BULLET,		// Comes out of guns, stupid :-)
	OBJ_TARGET,		// for the camera tracking
} OBJECT_TYPE;

/***************************************************************************/

#define BASE_ELEMENTS1(pointerType) \
	OBJECT_TYPE			type;		/* The type of object */ \
	UDWORD				id;			/* ID number of the object */ \
	UWORD				x,y,z;		/* Object's location */ \
	float				direction;	/* Object's direction +ve rotation about y axis*/ \
	SWORD				pitch;		/* Object's pitch +ve nose up*/ \
	SWORD				roll		/* Object's roll +ve left up, right down */

#define BASE_ELEMENTS2(pointerType) \
	SCREEN_DISP_DATA	sDisplay;	/* screen coordinate details */ \
	UBYTE				player;		/* Which player the object belongs to */ \
	UBYTE				group;		/* Which group selection is the droid currently in? */ \
	UBYTE				selected;	/* Whether the object is selected */ \
									/* might want this elsewhere */ \
	UBYTE				cluster;	/* Which cluster the object is a member of */ \
	UBYTE				visible[MAX_PLAYERS];	/* Whether object is visible to specific player */	\
	UDWORD				died;		/* When an object was destroyed, if 0 still alive */ \
	/*BOOL			    (*damage)(pointerType	*psObject, */ \
								  /*UDWORD		damage, */ \
								  /* UDWORD		weaponClass); - Damage function */ \
	/*UDWORD				emissionInterval;	how frequently does it puff out damage smoke?*/ \
	UDWORD				lastEmission;	/* when did it last puff out smoke? */ \
	/* Data for fire damage */ \
	BOOL				inFire;		/* TRUE if the object is in a fire */ \
	UDWORD				burnStart;	/* When the object entered the fire */ \
	UDWORD				burnDamage /* How much damage has been done since the object */ \
									/* entered the fire */

#define NEXTOBJ(pointerType) \
	pointerType			*psNext	/* Pointer to next object in list */

#define PREVOBJ(pointerType) \
	pointerType			*psPrev	/* Pointer to previois object in list */

#define	BASE_ELEMENTS(pointerType)	\
	BASE_ELEMENTS1(pointerType);	\
	NEXTOBJ(pointerType);			\
	BASE_ELEMENTS2(pointerType)

#define SIMPLE_ELEMENTS(pointerType)	\
	BASE_ELEMENTS1(pointerType);		\
	NEXTOBJ(pointerType)

/***************************************************************************/

typedef struct _base_object
{
	BASE_ELEMENTS( struct _base_object );
}
BASE_OBJECT;

typedef struct SIMPLE_OBJECT
{
	SIMPLE_ELEMENTS( struct SIMPLE_OBJECT );
}
SIMPLE_OBJECT;

/***************************************************************************/

#endif

/***************************************************************************/
