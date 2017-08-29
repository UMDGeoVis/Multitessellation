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

/* ------------------------------ fltmisc.h ------------------------------- */

#ifndef MT_MISCFILTER_INCLUDED
#define MT_MISCFILTER_INCLUDED

#include "fltthr.h"
#include "tilerror.h"

/* ------------------------------------------------------------------------ */
/*               SOME RESOLUTION CONDITIONS USING THRESHOLDS                */
/* ------------------------------------------------------------------------ */

/*
This file defines a number of resolution filter conditions. These conditions 
impose that some feature of a tile is within a given threshold.
For instance, the feature may be: the area of a tile, its circumcircle, 
or some attribute such as the approximation error.
For instance, one may want that the area of extracted tiles is smaller
near the viewpoint used for rendering, and larger far from it.
This is achieved by using a threshold that depends from the distance 
of a tile from a reference point.
*/

/* ------------------------------------------------------------------------ */
/*                   TESTING THE THRESHOLD ON TILE ERRORS                   */
/* ------------------------------------------------------------------------ */

/*
This resolution condition can only be applied to MTs that are subclasses
of MT_WithTileErrorClass and implement the function MT_TileError.
Also, make sure that the MT and the threshold used are mutually compatible.
*/

typedef class TileErrFilterClass * TileErrFilter;

class TileErrFilterClass : public MT_CondClass
{
  protected:

  /*
  Explicit reference to the MT in order to access tile errors.
  */
  WithTileErrorClass * my_mt;

  /*
  The threshold imposed on tile errors.
  */
  Threshold my_thr;

  public:
  
  /*
  Return 1 if the error of the tile is <= the threshold value for 
  that tile. Return 0 otherwise.
  */
  int MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag);

  /*
  Create a condition given the reference MT m and the threshold tr to
  be imposed on its tiles.
  */
  TileErrFilterClass(WithTileErrorClass * m, Threshold tr);
};

/* ------------------------------------------------------------------------ */
/*                   TESTING THE THRESHOLD ON TILE AREAS                    */
/* ------------------------------------------------------------------------ */

/*
This resolution condition can only be applied to MTs with two-dimensional
tiles (i.e., triangles) in two dimensions.
If the embedding space has more than two dimensions, only the first
two are considered.
Also, make sure that the threshold used is compatible with such property.
*/

typedef class TriangleArea2FilterClass * TriangleArea2Filter;

class TriangleArea2FilterClass : public MT_CondClass
{
  protected:

  /*
  The threshold imposed on tile areas.
  */
  Threshold my_thr;

  public:
  
  /*
  Return 1 if the area of the tile is <= the threshold value for
  that tile. Return 0 otherwise.
  */
  int MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag);

  /*
  Create a condition given the threshold to be imposed on tile areas.
  */
  TriangleArea2FilterClass(Threshold tr);

  /*
  This condition is only good for MTs with triangular tiles.
  */
  int MT_IsGood(MT_MultiTesselation m);
};

/* ------------------------------------------------------------------------ */

/*
This resolution condition can only be applied to MTs with two-dimensional
tiles (i.e., triangles) in three dimensions.
If the embedding space has more than three dimensions, only the first
three are considered.
Also, make sure that the threshold used is compatible with such property.
*/

typedef class TriangleArea3FilterClass * TriangleArea3Filter;

class TriangleArea3FilterClass : public MT_CondClass
{
  protected:

  /*
  The threshold imposed on tile areas.
  */
  Threshold my_thr;

  public:
  
  /*
  Return 1 if the area of the tile is <= the threshold value for
  that tile. Return 0 otherwise.
  */
  int MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag);

  /*
  Create a condition given the threshold to be imposed on tile areas.
  */
  TriangleArea3FilterClass(Threshold tr);

  /*
  This condition is only good for MTs with triangular tiles embedded in
  at least three dimensions.
  */
  int MT_IsGood(MT_MultiTesselation m);

  void MT_MinMax(MT_MultiTesselation m, float *min, float *max);
};

/* ------------------------------------------------------------------------ */
/*         TESTING THE THRESHOLD ON THE RADIUS OF THE CIRCUMCIRCLE          */
/* ------------------------------------------------------------------------ */

/*
This resolution condition can only be applied to MTs with two-dimensional
tiles (i.e., triangles) in two or more dimensions.
If the MT is embedded in more than two dimensions, only the first two
vertex coordinates are considered for computing the circum-circle.
*/

typedef class Circumradius2FilterClass * Circumradius2Filter;

class Circumradius2FilterClass : public MT_CondClass
{
  protected:

  /*
  The threshold imposed on the radius of the circum-circle.
  */
  Threshold my_thr;

  public:
  
  /*
  Return 1 if the radius of the circumcircle to the tile is <= the 
  threshold value for that tile. Return 0 otherwise.
  */
  int MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag);

  /*
  Create a condition given the threshold to be imposed on the radius.
  */
  Circumradius2FilterClass(Threshold tr);

  /*
  This condition is only good for MTs with triangular tiles.
  */
  int MT_IsGood(MT_MultiTesselation m);
};

/* ------------------------------------------------------------------------ */

/*
This resolution filter condition can only be applied to MTs with 
two-dimensional tiles (i.e., triangles) in three or more dimensions.
If the MT is embedded in more than three dimensions, only the first three
vertex coordinates are considered for computing the circum-circle.
*/

typedef class Circumradius3FilterClass * Circumradius3Filter;

class Circumradius3FilterClass : public MT_CondClass
{
  protected:

  /*
  The threshold imposed on the radius of the circum-circle.
  */
  Threshold my_thr;

  public:
  
  /*
  Return 1 if the radius of the circumcircle to the tile is <= the 
  threshold value for that tile. Return 0 otherwise.
  */
  int MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag);

  /*
  Create a condition given the threshold to be imposed on the radius.
  */
  Circumradius3FilterClass(Threshold tr);

  /*
  This condition is only good for MTs with triangular tiles embedded in
  at least three dimensions.
  */
  int MT_IsGood(MT_MultiTesselation m);

};

/* ------------------------------------------------------------------------ */

#endif  /* MT_MISCFILTER_INCLUDED */
