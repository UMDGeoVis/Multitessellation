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

/* ------------------------------ fcsline.h ------------------------------- */

#ifndef MT_LINEFOCUS_INCLUDED
#define MT_LINEFOCUS_INCLUDED

#include "mt_cond.h"

/* ------------------------------------------------------------------------ */
/*                     CONDITIONS USING A POLYGONAL LINE                    */
/* ------------------------------------------------------------------------ */

/*
This file defines a focus consisting of a polygonal line.
*/

/* ------------------------------------------------------------------------ */
/*                         CLASS FOR A POLYGONAL LINE                       */
/* ------------------------------------------------------------------------ */

class LineClass 
{
   private:

   /*
   Number of coordinates of the embedding space.
   */
   int l_dim;
   
   /* 
   Number of points and coordinates of the points of the line.
   It will be allocated as an array p_coord[p_num][l_dim].
   */
   int p_num;
   float ** p_coord;

/* ------------------------------------------------------------------------ */
/*                          Access functions                                */
/* ------------------------------------------------------------------------ */

   protected:
   
   /*
   Allocate space for num points. The coordinates of such points are all
   set to zeroes. return 1 on success, 0 otherwise.
   */
   int SetPointNum(int num);

   public:

   /*
   Return the number of coordinates of the space containing the line.
   */
   inline int LineDim(void)  {  return (l_dim);  }

   /*
   Return the number of line points.
   */
   inline int NumPoints(void)  {  return (p_num);  }

   /*
   Set the i-th point of the line (0<=i<p_num).
   Array coord must have as many as l_dim elements.
   */
   void SetPoint(int i, float * coord);
   
   /*
   Copy the i-th point of the line (0<=i<p_num) into array coord.
   Array coord must have as many as l_dim elements.
   */
   void ThePoint(int i, float * coord);
   
   /*
   Shortcuts to be used if the line is in 2D or 3D.
   */
   void SetPoint(int i, float x, float y);
   void ThePoint(int i, float * x, float * y);
   void SetPoint(int i, float x, float y, float z);
   void ThePoint(int i, float * x, float * y, float * z);

   /* 
   Create a line in the given number of dimensions, the line is 
   empty (it has no points).
   */
   LineClass(int d);

   /*
   Deallocate space used for storing the points of this line.
   */
   ~LineClass();
};

/* ------------------------------------------------------------------------ */
/*                  FOCUS CONSISTING OF A POLYGONAL LINE                    */
/* ------------------------------------------------------------------------ */

/*
The focus is a polygonal line in two dimensions.
A tile is in the focus if it intersects the line either with its
interior or with its boundary.
It applies to MTs where tiles are triangles, embedded in at least 2D.
If the dimension of the embedding space is >2, then the extra coordinates
are not considered.
*/

typedef class Line2FocusOnTrianglesClass * Line2FocusOnTriangles;

class Line2FocusOnTrianglesClass : public LineClass, public MT_CondClass
{
   public:
   
   /* 
   Implementation of abstract function from superclass MT_CondClass.
   */
   int MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag);

   /* 
   Create a line with num points, whose coordinates are contained
   in arrays x and y.
   */
   Line2FocusOnTrianglesClass(int num, float *x, float *y);
   
   /*
   This condition is good only for MTs with triangular tiles.
   */
   int MT_IsGood(MT_MultiTesselation m);   
};

#endif /* MT_LINEFOCUS_INCLUDED */

/* ------------------------------------------------------------------------ */
