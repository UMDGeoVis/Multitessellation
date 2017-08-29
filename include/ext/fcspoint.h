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

/* ------------------------------ fcspoint.h ------------------------------ */

#ifndef MT_POINTFOCUS_INCLUDED
#define MT_POINTFOCUS_INCLUDED

#include "mt_cond.h"

/* ------------------------------------------------------------------------ */
/*                     CONDITIONS USING A POINT                             */
/* ------------------------------------------------------------------------ */

/*
This file defines a number of extraction conditions based on a point.
These are focus conditions which return TRUE iff a tile contains the point.
Threshold conditions can also be defined which constrain the distance
of a tile from the point to be within some threshold, but this is done
in another file (t_misc).
*/

/* ------------------------------------------------------------------------ */
/*                          CLASS FOR A POINT                               */
/* ------------------------------------------------------------------------ */

class PointClass 
{

/* ------------------------------------------------------------------------ */
/*                        Internal structure                                */
/* ------------------------------------------------------------------------ */

  protected:
  
  /*
  Coordinates of the reference point. Only the first d coordinates will be
  used if the point is considered as embedded in d-dimensional space.
  */
  float p_coord[MT_MAX_DIM];
  
  /*
  Number of coordinates of the embedding space.
  */
  int p_dim;
  
/* ------------------------------------------------------------------------ */
/*                          Access functions                                */
/* ------------------------------------------------------------------------ */

  public:

  /*
  Return the number of coordinates of the reference point.
  */
  inline int PointDim(void) {  return (p_dim);  }
  
  /*
  Return and set the reference point.
  */
  void SetPoint(float * coord);
  void ThePoint(float * coord);

  /*
  The following functions are shortcuts to be used if the point is
  in 2D or 3D.
  */
  void SetPoint(float x, float y);
  void ThePoint(float *x, float *y);
  void SetPoint(float x, float y, float z);
  void ThePoint(float *x, float *y, float *z);

   /*
   Translate the point.
   */  
   void Translate (float * trans_vector);

   /*
   Shortcuts for points in 2D or 3D.
   */
   void Translate (float vectorX, float vectorY);
   void Translate (float vectorX, float vectorY, float vectorZ);

  /*
  Create a point with the given number of coordinates, located at the origin.
  */
  PointClass(int d);

};

/* ------------------------------------------------------------------------ */
/*                       FOCUS CONSISTING OF A POINT                        */
/* ------------------------------------------------------------------------ */

/*
These conditions implement a focus set consisting of a point.
A tile is in the focus if it contains the point either in its interior
or on its boundary.
*/

/* ------------------------------------------------------------------------ */
/*                   FOCUS CONSISTING OF A POINT IN 2D                      */
/* ------------------------------------------------------------------------ */

/*
This class implements a focus set consisting of a point in 2D. 
It applies to MTs where tiles are triangles, embedded in at least 2D.
If the dimension of the embedding space is >2, then the extra coordinates
are not considered. This is equivalent to applying the point-in-tile
test to the projections of the MT tiles on the x-y plane.
*/

/* ------------------------------------------------------------------------ */

typedef class Point2FocusOnTrianglesClass * Point2FocusOnTriangles;

class Point2FocusOnTrianglesClass : public PointClass, public MT_CondClass
{
  public:

  /*
  Creator of the focus.
  */
  Point2FocusOnTrianglesClass(float x, float y);

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
/*                   FOCUS CONSISTING OF A POINT IN 3D                      */
/* ------------------------------------------------------------------------ */

/*
This class implements a focus set consisting of a point in 3D. 
It applies to MTs where tiles are tetrahedra, embedded in at least 3D.
If the dimension of the embedding space is >3, then the extra coordinates
are not considered. This is equivalento to applying the point-in-tile
test to the projections of the MT tiles in the x-y-z space.
*/

/* ------------------------------------------------------------------------ */

typedef class Point3FocusOnTetrahedraClass * Point3FocusOnTetrahedra;

class Point3FocusOnTetrahedraClass : public PointClass, public MT_CondClass
{
  public:

  /*
  Creator of the focus.
  */
  Point3FocusOnTetrahedraClass(float x, float y, float z);

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

#endif /* MT_POINTFOCUS_INCLUDED */
