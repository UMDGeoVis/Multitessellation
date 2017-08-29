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

/* ------------------------------ fcswedge.h ------------------------------ */

#ifndef MT_WEDGEFOCUS_INCLUDED
#define MT_WEDGEFOCUS_INCLUDED

#include "mt_cond.h"

/* ------------------------------------------------------------------------ */
/*                      FOCUS SET CONSISTING OF A WEDGE                     */
/* ------------------------------------------------------------------------ */

/*
This file defines a focus set consisting of a wedge. Examples are an
angular sector in 2D, an unbounded pyramid with four sides in 3D.
This is useful in visualization to represent the view field: the extracted
tesselation will be restricted to lie inside the view field.
*/

/* ------------------------------------------------------------------------ */
/*                            CLASS FOR A WEDGE                             */
/* ------------------------------------------------------------------------ */

class WedgeClass
{
   protected:

   /*
   Number of coordinates of the embedding space.
   */
   int w_dim;

   /* 
   Coordinates of the vertex of the wedge.
   */
   float v_coord[MT_MAX_DIM];

   /*
   Direction of the medial axis of the wedge.
   */
   float w_axis[MT_MAX_DIM];
   
   /*
   Half opening angle in each reference direction orthogonal to 
   the medial axis.
   The number of such directions is equal to w_dim - 1 (just one
   in 2D, two in 3D).
   */
   float open_angle[MT_MAX_DIM-1];
   
   /*
   Vectors defining the reference directions for the opening angles.
   In 2D, we need no vector.
   In 3D, we need one vector (the "up vector") for defining one of
   the two reference directions. The other one is the one orthogonal
   to the first one and to the medial axis.
   */
   float open_dir[MT_MAX_DIM-2][MT_MAX_DIM];

/* ------------------------------------------------------------------------ */
/*                          Access functions                                */
/* ------------------------------------------------------------------------ */
 
   public:
   
   /*
   Return the number of coordinates of the embedding space.
   */
   inline int WedgeDim(void)  {  return (w_dim);  }

   /*
   Return and set the vertex of the wedge. Parameter coord is an array
   of as many float as the dimension of the embedding space.
   */
   void TheVertex(float * coord);
   void SetVertex(float * coord);
   void TranslateVertex(float * incr);

   /*
   Shortcuts for wedges in 2D.
   */
   inline void TheVertex(float * x, float * y)
   {  (*x) = v_coord[0];  (*y) = v_coord[1];  }
   inline void SetVertex(float x, float y)  
   {  v_coord[0] = x;  v_coord[1] = y;  }
   inline void TranslateVertex(float dx, float dy)
   {  v_coord[0] += dx; v_coord[1] += dy;  }
   
   /*
   Return a point lying on the medial axis of the wedge.
   The point lies at distance dist from the vertex of the wedge.
   */
   void TheAxisPoint(float * coord, float dist = 1.0);

   /*
   Set / return the medial axis of the wedge by means of
   its directional coefficients.
   */
   void SetAxisDir(float * dir);
   void TheAxisDir(float * dir);
   
   /*
   Set the medial axis of the wedge by means of a point.
   The medial axis is defined as the half-line joining the vertex 
   of the wedge to such point.
   */
   void SetAxisPoint(float * coord);

   /*
   Shortcuts for wedges in 2D.
   */
   void TheAxisPoint(float * x, float * y, float dist = 1.0);
   void SetAxisPoint(float x, float y);
   void SetAxisDir(float dx, float dy);
   void TheAxisDir(float * dx, float * dy);

   /*
   Return and set the i-th opening angle.
   The angle must be between 0 and PI/2.
   */
   inline float TheAngle(int i)  {  return (open_angle[i]);  }
   void SetAngle(int i, float a);

   /*
   For wedges in 2D. Return a point lying on the left and on the right
   side of the wedge at distance dist from the vertex.
   */
   void TheLeftPoint(float * x, float * y, float dist = 1.0);
   void TheRightPoint(float * x, float * y, float dist = 1.0);
  
   /*
   Create a wedge in a d-dimensional space. The vertex is the origin,
   the reference point lies at distance 1 from the origin in the 
   direction of the last coordinate axis of the space.
   The reference directions for the opening angles undefined.
   All opening angles are zero (the wedge is a half-line).
   */
   WedgeClass(int d);   
};

/* ------------------------------------------------------------------------ */
/*                  FOCUS CONSISTING OF A WEDGE IN 2D                       */
/* ------------------------------------------------------------------------ */

/*
The focus is an angular sector in 2D.
This focus applies with MTs where tiles are triangles.
If the dimension of the embedding space is >2, then the extra coordinates
are not considered.
*/

typedef class Wedge2FocusOnTrianglesClass * Wedge2FocusOnTriangles;

class Wedge2FocusOnTrianglesClass : public WedgeClass, public MT_CondClass
{
   public:
   
   /* 
   Implementation of abstract function from superclass MT_CondClass.
   */
   int MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag);

   /* 
   Create a sector with vertex in (xv,yv), medial axis the line joining
   (xv,yv) to (xr,yr), and opening angle equal to a.
   */
   Wedge2FocusOnTrianglesClass(float xv, float yv,
                               float xr, float yr, float a);

   /*
   This condition is good only for MTs with triangular tiles.
   */
   int MT_IsGood(MT_MultiTesselation m);
};

#endif /* MT_WEDGEFOCUS_INCLUDED */

/* ------------------------------------------------------------------------ */
