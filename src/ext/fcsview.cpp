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

/* ------------------------------ fcsview.c ------------------------------- */

#include "geo.h"
#include "fcsview.h"

/* ------------------------------------------------------------------------ */
/*                      FOCUS SET FOR PERSPECTIVE VIEW                      */
/* ------------------------------------------------------------------------ */


View2FocusOnTrianglesClass ::
View2FocusOnTrianglesClass(float xv, float yv, float xr, float yr, float a)
:    Wedge2FocusOnTrianglesClass(xv,yv,xr,yr,a)
{  
#ifdef TENERE_TRIANGOLO 
  the_triangle = MT_NULL_INDEX;
#endif /* TENERE_TRIANGOLO */
  near_dist = 0.0; far_dist = FLT_MAX;
}

int View2FocusOnTrianglesClass ::
    MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag)
{
  int res;
  MT_INDEX * v;
  float x[3], y[3];
  float d_min, d_max;
  int i;

  v = m->MT_TileVertices(t);
  for (i=0; i<3; i++)
  {
    x[i] = m->MT_VertexX(v[i]);
    y[i] = m->MT_VertexY(v[i]);
  }
  d_min = SquaredPointTriangleDist2D(v_coord[0],v_coord[1],x,y);
  d_max = SquaredPointTriangleMaxDist2D(v_coord[0],v_coord[1],x,y);
  if ( (d_max<near_dist*near_dist) || (d_min>far_dist*far_dist) )
     res = 0;
  else
     res = Wedge2FocusOnTrianglesClass::MT_EvalCond(m,t,flag);

#ifdef TENERE_TRIANGOLO 
  if (flag==MT_STRICT)
  {
    if (!PointOutTriangle2D(v_coord[0], v_coord[1],
                            x[0],y[0], x[1],y[1], x[2],y[2]) )
    {
      the_triangle = t;
    }
  }
#endif /* TENERE_TRIANGOLO */
  return res;
}

#ifdef TENERE_TRIANGOLO

MT_INDEX View2FocusOnTrianglesClass :: 
         TriangleHavingVertex(MT_MultiTesselation m)
{
   if (the_triangle != MT_NULL_INDEX)
   {
     MT_INDEX * v;  /* triangle vertices */
     float a,b,c;   /* triangle normal */
     m->MT_TileVertices(the_triangle, &v);

/* 
     if (PointOutTriangle2D(v_coord[0], v_coord[1],
                            m->MT_VertexX(v[0]),m->MT_VertexY(v[0]), 
                            m->MT_VertexX(v[1]),m->MT_VertexY(v[1]),
                            m->MT_VertexX(v[2]),m->MT_VertexY(v[2])) )
        the_triangle = MT_NULL_INDEX;
*/
   }
   return (the_triangle);
}

float View2FocusOnTrianglesClass :: VertexHeight(MT_MultiTesselation m)
{  
   if (m->MT_VertexDim()<3)
   {
     MT_Error((char *)"Must be in 3D space",
              (char *)"View2FocusOnTrianglesClass::VertexHeight");
     return (0.0);
   }
   if (the_triangle != MT_NULL_INDEX)
   {
     MT_INDEX * v;  /* triangle vertices */
     float a,b,c;   /* triangle normal */
     m->MT_TileVertices(the_triangle, &v);
/*
     if (PointOutTriangle2D(v_coord[0], v_coord[1],
                            m->MT_VertexX(v[0]),m->MT_VertexY(v[0]), 
                            m->MT_VertexX(v[1]),m->MT_VertexY(v[1]),
                            m->MT_VertexX(v[2]),m->MT_VertexY(v[2])) )
     {
       MT_Error("Invalid triangle",
                "View2FocusOnTrianglesClass::VertexHeight");
        return (0.0);
     }
*/
     TriangleNormal(
        m->MT_VertexX(v[0]),m->MT_VertexY(v[0]),m->MT_VertexZ(v[0]),
        m->MT_VertexX(v[1]),m->MT_VertexY(v[1]),m->MT_VertexZ(v[1]),
        m->MT_VertexX(v[2]),m->MT_VertexY(v[2]),m->MT_VertexZ(v[2]),
        &a, &b, &c);
     return ( m->MT_VertexZ(v[0]) +
              (-1.0/c) * ( a*(v_coord[0]-m->MT_VertexX(v[0])) +
                           b*(v_coord[1]-m->MT_VertexY(v[0])) ) );
   }
   MT_Error((char *)"No triangle",(char *)"View2FocusOnTrianglesClass::VertexHeight");
   return (0.0); 
}

#endif /* TENERE_TRIANGOLO */

MT_INDEX TriangleHavingVertex(MT_Extractor e, float x, float y)
{
  MT_INDEX * tri;
  MT_INDEX num_tri;
  MT_INDEX i, t;
  MT_INDEX * v;  /* triangle vertices */
  MT_MultiTesselation m = e->MT_TheMT();

  num_tri = e->MT_AllExtractedTiles(&tri);
  t = MT_NULL_INDEX;
  for (i=0; i<num_tri; i++)
  {
     m->MT_TileVertices(tri[i], &v);
     /* preliminary test on triangle bounding box */
     if ( (x<m->MT_VertexX(v[0])) && (x<m->MT_VertexX(v[1])) &&
          (x<m->MT_VertexX(v[2])) ) break;
     if ( (x>m->MT_VertexX(v[0])) && (x>m->MT_VertexX(v[1])) &&
          (x>m->MT_VertexX(v[2])) ) break;
     if ( (y<m->MT_VertexY(v[0])) && (y<m->MT_VertexY(v[1])) &&
          (y<m->MT_VertexY(v[2])) ) break;
     if ( (y>m->MT_VertexY(v[0])) && (y>m->MT_VertexY(v[1])) &&
          (y>m->MT_VertexY(v[2])) ) break;
     /* true point-in-triangle test */
     if (!PointOutTriangle2D(x,y,
                            m->MT_VertexX(v[0]),m->MT_VertexY(v[0]), 
                            m->MT_VertexX(v[1]),m->MT_VertexY(v[1]),
                            m->MT_VertexX(v[2]),m->MT_VertexY(v[2])) )
         t = tri[i];
  }
  return t;
}

float VertexHeight(MT_MultiTesselation m, MT_INDEX t, float x, float y)
{
   if (t != MT_NULL_INDEX)
   {
     MT_INDEX * v;  /* triangle vertices */
     float a,b,c;   /* triangle normal */
     m->MT_TileVertices(t, &v);

     TriangleNormal(
        m->MT_VertexX(v[0]),m->MT_VertexY(v[0]),m->MT_VertexZ(v[0]),
        m->MT_VertexX(v[1]),m->MT_VertexY(v[1]),m->MT_VertexZ(v[1]),
        m->MT_VertexX(v[2]),m->MT_VertexY(v[2]),m->MT_VertexZ(v[2]),
        &a, &b, &c);
     return (float) ( m->MT_VertexZ(v[0]) +
              (-1.0/c) * ( a*(x-m->MT_VertexX(v[0])) +
                           b*(y-m->MT_VertexY(v[0])) ) );
   }
   MT_Error((char *)"No triangle",(char *)"VertexHeight");
   return (0.0); 
}

/* ------------------------------------------------------------------------ */
