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

/* ------------------------------ fcssegm.h ------------------------------- */

#ifndef MT_SEGFOCUS_INCLUDED
#define MT_SEGFOCUS_INCLUDED

#include "mt_cond.h"
#include "tilerror.h"
#include "vrtfield.h"

/* ------------------------------------------------------------------------ */
/*                       CONDITIONS USING A SEGMENT                         */
/* ------------------------------------------------------------------------ */

/*
This file defines a number of extraction conditions based on a segment.
*/

/* ------------------------------------------------------------------------ */
/*                          CLASS FOR A SEGMENT                             */
/* ------------------------------------------------------------------------ */

class SegmentClass
{
   protected:

   /* 
   Coordinates of the endpoints of the segment.
   */
   float endp1[MT_MAX_DIM], endp2[MT_MAX_DIM];

   /*
   Number of coordinates of the embedding space.
   */
   int s_dim;

/* ------------------------------------------------------------------------ */
/*                          Access functions                                */
/* ------------------------------------------------------------------------ */
 
   public:

   /*
   Return the number of coordinates of the segment endpoints.
   */
   inline int SegmentDim(void)  {  return (s_dim);  }

   /*
   Return and set the segment endpoints.
   */
   void SetSegment(float * coord1, float * coord2);
   void TheSegment(float * coord1, float * coord2);
   
   /*
   The following functions are shortcuts to be used if the segment
   is in 2D or 3D.
   */
   void SetSegment(float x1, float y1, float x2, float y2);
   void TheSegment(float * x1, float * y1, float * x2, float * y2);
   void SetSegment(float x1, float y1, float z1,
                   float x2, float y2, float z2);
   void TheSegment(float * x1, float * y1, float * z1,
                   float * x2, float * y2, float * z2);
   /* 
   Create a segment with the given number of coordinates, both
   endpoints are at the origin (the segment degenerates into a point).
   */
   SegmentClass(int d);
};

/* ------------------------------------------------------------------------ */
/*                   FOCUS CONSISTING OF A SEGMENT IN 2D                    */
/* ------------------------------------------------------------------------ */

/*
This class implements a focus set consisting of a segment in 2D.
A tile is in the focus if it intersects the segment either with its 
interior or with its boundary.
Such focus applies to MTs where tiles are triangles, embedded in at least 2D.
If the dimension of the embedding space is >2, then the extra coordinates
are not considered. This is equivalent to applying the intersection
test to the projections of the MT tiles on the x-y plane.
*/

/* ------------------------------------------------------------------------ */

typedef class Segment2FocusOnTrianglesClass * Segment2FocusOnTriangles;

class Segment2FocusOnTrianglesClass : public SegmentClass,
                                      public MT_CondClass
{

  public:
  
  /*
  Creator of the focus.
  */
  Segment2FocusOnTrianglesClass(float x1, float y1, float x2, float y2);

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
/*       FOCUS CONSISTING OF THE LOCUS OF POINTS ABOVE A SEGMENT IN 3D      */
/* ------------------------------------------------------------------------ */

/*
This focus applies to MTs which represent two-dimensional scalar fields.
The focus is embedded in the 3D space that contains the graph of the field.
It consists of the locus of points that are located on or above a segment 
in 3D (all points (x,y,z) such that a point (x,y,s) belongs to the 
segment, with s<=z).
It is useful for testing the visibility between two points on a terrain.

The strict and loose evaluation mode are implemented as follows:
1) flag == MT_LOOSE:
   intersection test takes into account the tile error Err:
   the result is 1 iff the tile, expanded of an amount equal to Err,
   lies at least partially above the segment.
2) flag == MT_STRICT:
   intersection test returns 1 iff the tile lies at least partially above the 
   segment (the tile error is not used).
*/

/*
This focus set can only be applied to MTs that are subclasses
of WithFieldClass and WithTileErrorClass, and implement functions
TileError and Field.
*/

/* ------------------------------------------------------------------------ */

typedef class Segment3FocusOnTriFieldClass * Segment2FocusOnTriField;

class Segment3FocusOnTriFieldClass : public SegmentClass, public MT_CondClass
{
  protected:

  /*
  These two variables point to the same MT seen in its two aspects.
  */
  WithFieldClass * my_mt1;
  WithTileErrorClass * my_mt2;
     
  public: 
   
  /*
  Creator of the focus.
  */
  Segment3FocusOnTriFieldClass(WithFieldClass * m1,
                               WithTileErrorClass * m2,
                               float x1, float y1, float z1,
                               float x2, float y2, float z2);

  /* 
  Implementation of abstract function from superclass MT_CondClass.
  */
  int MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag);

  /*
  This condition is good only for MTs with triangular tiles embedded in
  at least three dimensions.
  */
  int MT_IsGood(MT_MultiTesselation m);

};

#endif /* MT_SEGFOCUS_INCLUDED */

/* ------------------------------------------------------------------------ */
