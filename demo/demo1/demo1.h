/*****************************************************************************
The Multi-Tesselation (MT) version 1.0, 1999.
A dimension-independent package for the representation and manipulation of
spatial objects as simplicial complexes at multiple resolutions.

Copyright (C) 1999 DISI - University of Genova, Italy.
Group of Geometric Modeling and Computer Graphics DISI.
Program written by Paola Magillo <magillo@disi.unige.it>.
DISI - University of Genova, Via Dodecaneso 35, 16146 Genova - ITALY.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*****************************************************************************/

/* 
BOX DEMO
by Paola Magillo - DISI - University of Genova - Italy
Written JANUARY 1999, revised OCTOBER 1999
*/

#ifndef DEMO1_INCLUDED
#define DEMO1_INCLUDED

/* ------------------------------------------------------------------------- */
/*                                include                                    */
/* ------------------------------------------------------------------------- */

#include "mt_extra.h"  /* the extractor of tesselations */
#include "surfmt.h"    /* the surface MT */

#include "fltmisc.h"   /* the resolution filter conditions */
#include "fcsbox.h"    /* the box-based focus conditions */

/* ------------------------------------------------------------------------- */
/*                                macros                                     */
/* ------------------------------------------------------------------------- */

/* Type of extractor. */
#define STATIC_EXTR  1
#define DYNAMIC_EXTR 2
#define LOCAL_EXTR   3

/* ------------------------------------------------------------------------- */
/*                           global variables                                */
/* ------------------------------------------------------------------------- */

/* Surface MT. */
extern SurfaceWithError mt;

/* Extractor. */
extern MT_Extractor ex;
extern int extractor_type; /* one of STATIC_EXTR, DYNAMIC_EXTR, LOCAL_EXTR */

/* Extraction focus. */
extern Box3FocusOnTriangles focus;

/* ------------------------------------------------------------------------- */
/*                           exported functions                              */
/* ------------------------------------------------------------------------- */

/* Inizialization. */
extern void startGame(int argc, char **argv);

/* Restore initial box position. */
extern void restoreGame(void);

/* Play one step forward, moving the box in direction (dx,dy,dz). */
extern void advanceGame(float dx, float dy, float dz);

/* Termination. */
extern void endGame(void);

/* Get and change extractor type. */
extern int getExtractor(void);
extern void changeExtractor(void);

/* Scale the box keeping its center: sc = +1,-1 (larger,smaller) */
extern void scaleBox(int sc);

/* Get and change reference value for error threshold */
extern float getErrorBound(void);
extern void changeErrorBound(float v);

/* ------------------------------------------------------------------------- */

#endif /* DEMO1_INCLUDED */
