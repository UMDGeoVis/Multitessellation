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

/* ------------------------------ fcsrange.h ------------------------------ */

#ifndef MT_RANGEFOCUS_INCLUDED
#define MT_RANGEFOCUS_INCLUDED

#include "mt_cond.h"
#include "fcspoint.h"
#include "tilerror.h"

/* ------------------------------------------------------------------------ */
/*                     CONDITIONS USING A RANGE                             */
/* ------------------------------------------------------------------------ */

/*
This file defines a number of extraction conditions based on a range,
i.e., a ball centered at a given point, and having a given radius.
These conditions return 1 iff a tile intersects the range.
*/

/* ------------------------------------------------------------------------ */
/*                  CLASS FOR TILE-TO-POINT DISTANCE                        */
/* ------------------------------------------------------------------------ */

class PointDistClass : public PointClass
{

  protected:
  
  PointDistClass(int d) : PointClass(d)  { }

  public:
  
/* ------------------------------------------------------------------------ */
/*           Squared distance of a tile from the reference point            */
/* ------------------------------------------------------------------------ */

  /*
  More functions will be added as soon as they are developed.
  Some functions are approximated, and they output the minimum among the
  distances from the vertices and from the center of the tile (while the
  true minimum distance may occur inside the tile, and be smaller).
  */
  
  /*
  Exact distance for triangles when the reference point is in 2D.
  */
  float SquaredDistanceTriangle2(MT_TileSet m, MT_INDEX t);

  /*
  Approximated distance for triangles when the reference point is in 3D.
  */
  float ApproxSquaredDistanceTriangle3(MT_TileSet m, MT_INDEX t);

  /*
  Approximated distance for tetrahedra when the reference point is in 3D.
  */
  float ApproxSquaredDistanceTetra3(MT_TileSet m, MT_INDEX t);

};

/* ------------------------------------------------------------------------ */
/*                      FOCUS CONSISTING OF A RANGE                         */
/* ------------------------------------------------------------------------ */

class RangeClass : public PointDistClass
{

  protected: 
  
  /*
  radius of the range;
  */
  float radius;

  public:
  
  /*
  Set and return the radius.
  */
  inline float SetRadius(float r)  {  return (radius = r);  }
  inline float TheRadius(void)  {  return (radius);  }
  
  /*
  Create a range of dimension d, centered at the origin, with radius r.
  */
  RangeClass(int d, float r);
};

/* ------------------------------------------------------------------------ */
/*                2D RANGE FOCUS ON MTs WITH TRIANGULAR TILES               */
/* ------------------------------------------------------------------------ */

/*
This class implements a focus consisting of a 2D range, i.e., a circle in
the x-y plane. This focus only applies to MTs where tiles are triangles.
If the embedding space has more than two dimensions, only the first
two vertex coordinates are considered.
*/

typedef class Range2FocusOnTrianglesClass * Range2FocusOnTriangles;

class Range2FocusOnTrianglesClass : public RangeClass, public MT_CondClass
{
  public:
  
  /*
  Create a condition given the center (x,y) and the radius r of the circle.
  */
  Range2FocusOnTrianglesClass(float x, float y, float r);
    
  /*
  Return 1 if the tile-to-point distance is <= radius, 0 otherwise.
  The strict and the loose evaluation are the same.
  Assume that the tile is a triangle.
  */
  int MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag);

  /*
  This condition is good only for MTs with triangular tiles.
  */
  int MT_IsGood(MT_MultiTesselation m);
};

/* ------------------------------------------------------------------------ */
/*                3D RANGE FOCUS ON MTs WITH TRIANGULAR TILES               */
/* ------------------------------------------------------------------------ */

/*
This class implements a focus consisting of a 3D range, i.e., a sphere in
3D space. This focus only applies to MTs where tiles are triangles.
If the embedding space has more than three dimensions, only the first
three vertex coordinates are considered.
This condition requires that the MT has error values associated with its
tiles.

The strict and loose evaluation mode are implemented as follows:
1) flag == MT_LOOSE:
   intersection test takes into account the tile error Err:
   the result is 1 iff the tile, expanded of an amount equal to Err,
   intersects the circle (if the point-tile distance is <= radius+error).
2) flag == MT_STRICT:
   intersection test returns 1 iff the tile intersects the sphere
   (if the point-tile distance is <= radius), the tile error is not used.
*/

/*
This focus set can only be applied to MTs that are subclasses
of MT_WithTileErrorClass and implement the function MT_TileError.
*/

typedef class Range3FocusOnTrianglesClass * Range3FocusOnTriangles;

class Range3FocusOnTrianglesClass : public RangeClass, public MT_CondClass
{

  protected:
  
  WithTileError my_mt;

  public:
  
  /*
  Create a condition given the center (x,y,z) and the radius r of the sphere.
  Parameter m is the reference MT.
  */
  Range3FocusOnTrianglesClass(WithTileErrorClass * m,
                              float x, float y, float z, float r);

  /*
  Implementation of abstract function from superclass MT_CondClass.
  Assume that the tile is a triangle. Use approximated distance.
  */
  int MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag);

  /*
  This condition is good only for MTs with triangular tiles embedded in
  at least three dimensions.
  */
  int MT_IsGood(MT_MultiTesselation m);

};

/* ------------------------------------------------------------------------ */
/*                3D RANGE FOCUS ON MTs WITH TETRAHEDRAL TILES              */
/* ------------------------------------------------------------------------ */

/*
This class implements a focus consisting of a 3D range, i.e., a sphere in
3D space. This focus only applies to MTs where tiles are tetrahedra.
If the embedding space has more than three dimensions, only the first
three vertex coordinates are considered.
*/

typedef class Range3FocusOnTetrahedraClass * Range3FocusOnTetrahedra;

class Range3FocusOnTetrahedraClass : public RangeClass, public MT_CondClass
{

  public:

  /*
  Create a condition given the center (x,y,z) and the radius r of the sphere.
  */
  Range3FocusOnTetrahedraClass(float x, float y, float z, float r);
    
  /*
  Return 1 if the point-tile distance is <= radius, 0 otherwise.
  The strict and the loose evaluation are the same.
  Assume that the tile is a tetrahedron.
  */
  int MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag);

  /*
  This condition is good only for MTs with tetrahedral tiles.
  */
  int MT_IsGood(MT_MultiTesselation m);

};

/* ------------------------------------------------------------------------ */

#endif /* MT_RANGEFOCUS_INCLUDED */
