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

/* ------------------------------ fcsview.h ------------------------------- */

#ifndef MT_VIEWFOCUS_INCLUDED
#define MT_VIEWFOCUS_INCLUDED

#include "mt_cond.h"
#include "fcswedge.h"

/* ------------------------------------------------------------------------ */
/*                      FOCUS SET FOR PERSPECTIVE VIEW                      */
/* ------------------------------------------------------------------------ */

/*
Two-dimensional version of the view frustum in perspective view:
it is a trapezoidal region.
This file specializes a focus set consisting of a wedge.
The only changements are the following:
- the wedge is restricted to within a range of distances from
  its vertex (like the near and the far distance in viewing)
- the evaluation of the condition, if strict, saves the triangle
  containing the vertex of the wedge into a local variable
  (this is used when moving on a terrain to retrieve the
  height of the terrain at viewpoint)
*/

/* ------------------------------------------------------------------------ */
/*           SPECIALIZATION OF FOCUS CONSISTING OF A WEDGE IN 2D            */
/* ------------------------------------------------------------------------ */

typedef class View2FocusOnTrianglesClass * View2FocusOnTriangles;

class View2FocusOnTrianglesClass : public Wedge2FocusOnTrianglesClass
{
   private:
   
#ifdef TENERE_TRIANGOLO 
   /*
   The triangle containing the vertex of the wedge.
   */
   MT_INDEX the_triangle;
#endif /* TENERE_TRIANGOLO */

   /* 
   Distance from the vertex of the two clipping planes.
   Only the part of the wedge lying between these two distances is condidered.
   */
   float near_dist;
   float far_dist;
      
   public:
   
   int MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag);

   View2FocusOnTrianglesClass(float xv, float yv,
                              float xr, float yr, float a);

   /*
   Return the index of triangle containing the vertex of the
   wedge. It must be called just after extracting a
   tesselation. The geometry of this triangle can be found by
   accessing the MT.
   */
   MT_INDEX TriangleHavingVertex(MT_MultiTesselation m);
   
   /*
   Return the height of the surface at the vertex of the wedge,
   based on the last extraction performed.
   The given MT must be embedded at least in 3D.
   */
   float VertexHeight(MT_MultiTesselation m);

   /*
   Set and return the two reference distances. The values of n and f 
   must be positive and n<f.
   */
   inline void SetDistances(float n, float f)
   {  near_dist = n; far_dist = f;  }
   inline float NearDistance(void)  {  return (near_dist);  }
   inline float FarDistance(void)  {  return (far_dist);  }
   
};

#include "mt_extra.h"

/*
Search, among all currently extracted tiles, the one containing 
a given point (x,y). If no such triangle exists, return MT_NULL_INDEX.
Search both active and non-active extracted triangles.
The given extractor must be working on an MT embedded at least in 3D.
*/
MT_INDEX TriangleHavingVertex(MT_Extractor e, float x, float y);

/*
Given a triangle that contains (x,y), return the height of 
such triangle over (x,y).
The given MT must be embedded at least in 3D.
*/
float VertexHeight(MT_MultiTesselation m, MT_INDEX t, float x, float y);

#endif /* MT_VIEWFOCUS_INCLUDED */

/* ------------------------------------------------------------------------ */
