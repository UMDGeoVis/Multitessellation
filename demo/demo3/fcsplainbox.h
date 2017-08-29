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

/* ------------------------------ fcsbox.h ------------------------------- */

#include "mt_cond.h"
#include "fcsbox.h"

#ifndef MT_SIMPBOX_FOCUS_INCLUDED
#define MT_SIMPBOX_FOCUS_INCLUDED

/* ------------------------------------------------------------------------ */
/*                 FOCUS CONDITION TESTING A TILE AGAINST A BOX             */
/* ------------------------------------------------------------------------ */

/*
This file defines focus contitions that evaluate to true on tiles
intersecting an axis-parallel cuboid.
The strict and loose evaluation mode are the same.
*/

/* ------------------------------------------------------------------------ */
/*                         FOCUS CONSISTING OF A                            */
/*            THREE-DIMENSIONAL BOX ON MTs WITH TRIANGULAR TILES            */
/* ------------------------------------------------------------------------ */

/*
The box is in three dimensions, i.e., it is an axis-parallel cuboid.
This condition applies to two-dimensional MTs (tiles are triangles)
in 3D, or in dD with d>=3. 
In the last case just the first three vertex coordinates are considered. 
A tile is in the focus either if the interior of the tile intersects the box,
or if the boundary of the tile touches the box.
*/

typedef class SimpleBox3FocusOnTrianglesClass * SimpleBox3FocusOnTriangles;

class SimpleBox3FocusOnTrianglesClass : public BoxClass, public MT_CondClass
{

   public:

   /*
   Constructors of the focus. Parameter m is the reference MT.
   */
   SimpleBox3FocusOnTrianglesClass(float * min_coord, float * max_coord);
   
   SimpleBox3FocusOnTrianglesClass(float x1, float y1, float z1,
                                   float x2, float y2, float z2);
   
   /*                            
   Implementation of abstract function from superclass MT_CondClass.
   */
   int MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag);
              
   /*
   This condition is good only for MTs with triangular tiles embedded
   in at least three dimensions.
   */
   int MT_IsGood(MT_MultiTesselation m);

};

/* ------------------------------------------------------------------------ */

#endif /* MT_SIMPBOX_FOCUS_INCLUDED */
