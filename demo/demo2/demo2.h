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
TERRAIN DEMO
by Paola Magillo - DISI - University of Genova - Italy
Written JANUARY 1999, revised OCTOBER 1999
*/

#ifndef DEMO2_INCLUDED
#define DEMO2_INCLUDED

/* ------------------------------------------------------------------------- */
/*                                include                                    */
/* ------------------------------------------------------------------------- */

#include "mt_extra.h"  /* the extractor of tesselations */
#include "fieldmt.h"   /* the MT for scalar fields */
#include "cmpnorma.h"  /* auxiliary function for computing vertex normals */

#include "fltmisc.h"   /* the resolution filter conditions */
#include "fcswedge.h"  /* the wedge-based focus condition */

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

/* Field MT. */
extern FieldWithError mt;

/* Extractor. */
extern MT_Extractor ex;
extern int extractor_type; /* one of STATIC_EXTR, DYNAMIC_EXTR, LOCAL_EXTR */

/* Wedge focus. */
extern Wedge2FocusOnTrianglesClass * focus;

/* Container for vertex normals */
extern ExtraVertexNormal vn;

/* ------------------------------------------------------------------------- */
/*                           exported functions                              */
/* ------------------------------------------------------------------------- */

/* Inizialization. */
extern void startGame(int argc, char **argv);

/* Play one step forward:
   motion_dir = direction of motion: 0 = stop, 1 = forward, -1 = backward
   motion_ang = angle of motion: 0 = straight, 1 = left, -1 = right */
extern void advanceGame(int motion_dir, int motion_ang);

/* Termination. */
extern void endGame(void);

/* Get and change extractor type. */
extern int getExtractor(void);
extern void changeExtractor(void);

/* Compute scale factor for z against x and y. */
extern int scaleHeights(float * factor);

/* Get and change reference value for error threshold */
extern float getErrorBound(void);
extern void changeErrorBound(float v);

/* ------------------------------------------------------------------------- */

#endif /* DEMO2_INCLUDED */
