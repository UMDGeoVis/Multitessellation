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
#include "tilerror.h"

#ifndef MT_BOX_FOCUS_INCLUDED
#define MT_BOX_FOCUS_INCLUDED

/* ------------------------------------------------------------------------ */
/*                 FOCUS CONDITION TESTING A TILE AGAINST A BOX             */
/* ------------------------------------------------------------------------ */

/*
This file defines focus contitions that evaluate to true on tiles
intersecting an axis-parallel box.
*/

/* ------------------------------------------------------------------------ */
/*                    CLASS FOR AN AXIS-PARALLEL BOX                        */
/* ------------------------------------------------------------------------ */

typedef class BoxClass * Box;

class BoxClass
{

/* ------------------------------------------------------------------------ */
/*                        Internal structure                                */
/* ------------------------------------------------------------------------ */

  protected:
 
  /*
  Min and max coordinates of the box on each axis. Only the first d 
  positions of the arrays will be used if box is in d-dimensional space.
  */
  float minF[MT_MAX_DIM];
  float maxF[MT_MAX_DIM];

  /*
  Number of coordinated of the embedding space.
  */
  int b_dim;

/* ------------------------------------------------------------------------ */
/*                          Access functions                                */
/* ------------------------------------------------------------------------ */

   public:

   /*
   Return the number of coordinates of the space in which the box is defined.
   */
   inline int BoxDim(void) {  return (b_dim);  }
          
/* ------------------------------------------------------------------------ */
/*                       Functions to modify the box                        */
/* ------------------------------------------------------------------------ */

   public:
   
   void SetBox (float * min_coord, float * max_coord);

   /*
   The following functions make sense only if number of
   dimensions is 2, or 3, respectively.
   */

   void SetBox (float x1, float y1, float x2, float y2);
   void SetBox (float x1, float y1, float z1,
                float x2, float y2, float z2);

   inline void SetEdgeLength (int i, float w) {  maxF[i] = minF[i] + w;  }

   /*
   The following functions make sense only if number of
   dimensions is >= 1, 2, or 3, respectively.
   */
   inline void SetLengthX (float wx)   {  SetEdgeLength(0,wx);  }
   inline void SetLengthY (float wy)   {  SetEdgeLength(1,wy);  }
   inline void SetLengthZ (float wz)   {  SetEdgeLength(2,wz);  }

   /*
   Move the box to a certain location, and translate it by a vector.
   */
   void MoveTo (float * anchor_point);
   void Translate (float * trans_vector);

   /*
   The following functions make sense only if number of
   dimensions is 2, or 3, respectively.
   */

   void MoveTo (float anchorX, float anchorY);
   void Translate (float vectorX, float vectorY);
   void MoveTo (float anchorX, float anchorY, float anchorZ);
   void Translate (float vectorX, float vectorY, float vectorZ);

/* ------------------------------------------------------------------------ */
/*                       Functions to return the box                        */
/* ------------------------------------------------------------------------ */

   /*
   Return pointers to the two arrays of min and max box coordinates.
   */
   void TheBox (float ** min_coord, float ** max_coord);

   /*
   The following functions make sense only if number of
   dimensions is 2, or 3, respectively.
   */
   
   void TheBox (float *x1, float *y1, float *x2, float *y2);   
   void TheBox (float *x1, float *y1, float *z1,
                float *x2, float *y2, float *z2);
   
/* ------------------------------------------------------------------------ */
/*                          Creation of the box                             */
/* ------------------------------------------------------------------------ */

   public:
   
   /*
   Create a d-dimensional box with the given min and max coordinates.
   */
   BoxClass(int d, float * min_coord, float * max_coord);

   /*
   Create a d-dimensional box of null size where both max and min
   coordinates are all zeroes.
   */
   BoxClass(int d);
};

/* ------------------------------------------------------------------------ */
/*                       FOCUS CONSISTING OF A BOX                          */
/* ------------------------------------------------------------------------ */

/*
These conditions implement a focus set consisting of a box.
A tile is in the focus either if the interior of the tile intersects the box,
or if the boundary of the tile touches the box.
*/

/* ------------------------------------------------------------------------ */
/*              TWO-DIMENSIONAL BOX ON MTs WITH TRIANGULAR TILES            */
/* ------------------------------------------------------------------------ */

/*
The box is in two dimensions, i.e., it is an axis-parallel rectangle.
This condition applies to two-dimensional MTs (tiles are triangles)
in 2D, or in dD with d>=2. 
In the last case just the first two vertex coordinates are considered. 
The strict and the loose evaluation are the same.
*/

typedef class Box2FocusOnTrianglesClass * Box2FocusOnTriangles;

class Box2FocusOnTrianglesClass : public BoxClass, public MT_CondClass
{

   public:
   
   /*
   Constructors of the focus.
   */
   Box2FocusOnTrianglesClass(float * min_coord, float * max_coord);
   Box2FocusOnTrianglesClass(float x1, float y1, float x2, float y2);
   
   /*                            
   Implementation of abstract function from superclass MT_CondClass.
   */
   int MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag);
   
   /*
   This condition is good only for MTs with triangular tiles.
   */
   int MT_IsGood(MT_MultiTesselation m);
};

/* ------------------------------------------------------------------------ */
/*            THREE-DIMENSIONAL BOX ON MTs WITH TRIANGULAR TILES            */
/* ------------------------------------------------------------------------ */

/*
The box is in three dimensions, i.e., it is an axis-parallel cuboid.
This condition applies to two-dimensional MTs (tiles are triangles)
in 3D, or in dD with d>=3. 
In the last case just the first three vertex coordinates are considered. 
This condition requires that the MT has error values associated with its
tiles.

The strict and loose evaluation mode are implemented as follows:
1) flag == MT_LOOSE:
   intersection test takes into account the tile error Err:
   the result is 1 iff the tile, expanded of an amount equal to Err,
   intersects the box.
2) flag == MT_STRICT:
   intersection test returns 1 iff the tile intersects the box
   (the tile error is not used).
*/

/*
This focus set can only be applied to MTs that are subclasses
of MT_WithTileErrorClass and implement the function MT_TileError.
*/

typedef class Box3FocusOnTrianglesClass * Box3FocusOnTriangles;

class Box3FocusOnTrianglesClass : public BoxClass, public MT_CondClass
{

   protected:
   
   WithTileError my_mt;
   
   public:

   /*
   Constructors of the focus. Parameter m is the reference MT.
   */
   Box3FocusOnTrianglesClass(WithTileError m,
                             float * min_coord, float * max_coord);
   
   Box3FocusOnTrianglesClass(WithTileError m, 
                             float x1, float y1, float z1,
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
/*            THREE-DIMENSIONAL BOX ON MTs WITH TETRAHEDRAL TILES           */
/* ------------------------------------------------------------------------ */

/*
The box is in three dimensions, i.e., it is an axis-parallel cuboid.
This condition applies to three-dimensional MTs (tiles are tetrahedra)
in 3D, or in dD with d>=3. 
In the last case just the first three vertex coordinates are considered. 
The strict and loose evaluation mode are the same.
*/

typedef class Box3FocusOnTetrahedraClass * Box3FocusOnTetrahedra;

class Box3FocusOnTetrahedraClass : public BoxClass, public MT_CondClass
{

   public:
   
   /*
   Constructors of the focus.
   */
   Box3FocusOnTetrahedraClass(float * min_coord, float * max_coord);
   Box3FocusOnTetrahedraClass(float x1, float y1, float z1,
                              float x2, float y2, float z2);
   
   /*                            
   Implementation of abstract function from superclass MT_CondClass.
   */
   int MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag);

   /*
   This condition is good only for MTs with tetrahedral tiles.
   */
   int MT_IsGood(MT_MultiTesselation m);

};

/* ------------------------------------------------------------------------ */

#endif /* MT_BOX_FOCUS_INCLUDED */
