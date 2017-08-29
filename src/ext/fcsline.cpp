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

/* ------------------------------ fcsline.c ------------------------------- */

#include "geo.h"
#include "fcsline.h"

/* ------------------------------------------------------------------------ */
/*                       CLASS FOR A POLYGONAL LINE                         */
/* ------------------------------------------------------------------------ */

int LineClass :: SetPointNum(int num)
{
  int i;
  if (p_num != num)
  {  
     p_coord = (float **) realloc(p_coord, p_num*sizeof(float*));
     if (!p_coord)
     {   MT_Error((char *)"Failed allocation",(char *)"LineClass::SetPointNum");     
         return 0;
     }
     for (i=0; i<num; i++)
     {
        p_coord[i] = (float *) calloc (l_dim, sizeof(float));
        if (!p_coord[i])
        {   MT_Error((char *)"Failed allocation",(char *)"LineClass::SetPointNum");
            return 0;
        }
     }
  }
  p_num = num;
  return 1;
}

void LineClass :: SetPoint(int i, float * coord)
{  int j;
   for (j=0; j<LineDim(); j++) 
   {  p_coord[i][j] = coord[j];  }
}
   
void LineClass :: ThePoint(int i, float * coord)
{  int j;
   for (j=0; j<LineDim(); j++)
   {   coord[j] = p_coord[i][j];  }
}
   
void LineClass :: SetPoint(int i, float x, float y)
{   p_coord[i][0] = x; p_coord[i][1] = y;  }
   
void LineClass :: ThePoint(int i, float * x, float * y)
{  (*x) = p_coord[i][0]; (*y) = p_coord[i][1];  }

void LineClass :: SetPoint(int i, float x, float y, float z)
{   p_coord[i][0] = x; p_coord[i][1] = y;  
    p_coord[i][2] = z;
}
   
void LineClass :: ThePoint(int i, float * x, float * y, float * z)
{  (*x) = p_coord[i][0]; (*y) = p_coord[i][1];  
   (*z) = p_coord[i][2];
}

LineClass :: LineClass(int d)
{
   l_dim = d;
   p_num = 0;
   p_coord = NULL;
}

LineClass :: ~LineClass()
{
  if (p_coord)  {  free(p_coord); p_coord = NULL;  }
  p_num = 0;
}

/* ------------------------------------------------------------------------ */
/*                  FOCUS CONSISTING OF A POLYGONAL LINE                    */ 
/* ------------------------------------------------------------------------ */

Line2FocusOnTrianglesClass :: 
Line2FocusOnTrianglesClass(int num, float *x, float *y) : LineClass(2)
{
  if (SetPointNum(num))
  {
    int i;
    for (i=0; i<num; i++)
    {  SetPoint(i, x[i], y[i]);  }
  }
}

/* ------------------------------------------------------------------------ */

int Line2FocusOnTrianglesClass :: 
    MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag)
{
  MT_INDEX * v;           /* triangle vertices */
  int i, j;
  float xp, yp, xp1, yp1; /* two points of the line */
     
  m->MT_TileVertices(t,&v);
  for (j=0;j<NumPoints();j++)
  {
    ThePoint(j, &xp, &yp);
    if ( !PointOutTriangle2D(xp, yp,
                        m->MT_VertexX(v[0]),m->MT_VertexY(v[0]),
                        m->MT_VertexX(v[1]),m->MT_VertexY(v[1]),
                        m->MT_VertexX(v[2]),m->MT_VertexY(v[2])) )
    {  /* j-the line point is inside triangle */
       return 1;
    }
  }
  for (j=0;j<NumPoints()-1;j++)
  {
    for (i=0;i<3;i++)
    {
      ThePoint(j, &xp, &yp);
      ThePoint(j+1, &xp1, &yp1);
      if (SegmentsIntersect2D(xp, yp, xp1, yp1,
                        m->MT_VertexX(v[i]),m->MT_VertexY(v[i]),
                        m->MT_VertexX(v[(i+1)%3]),m->MT_VertexY(v[(i+1)%3])) )
      {  /* j-the line segment intersects i-th triangle edge */
         return 1;
      }
      if (SegmentsOverlap2D(xp, yp, xp1, yp1,
                        m->MT_VertexX(v[i]),m->MT_VertexY(v[i]),
                        m->MT_VertexX(v[(i+1)%3]),m->MT_VertexY(v[(i+1)%3])) )
      {
         /* j-the line segment overlaps i-th triangle edge */
         return 1;
      }
    }
  }
  /* line does not intersect triangle */
  return 0;  
}

int Line2FocusOnTrianglesClass :: MT_IsGood(MT_MultiTesselation m)
{
  if (m->MT_TileDim() != 2)
  {
     MT_Warning((char *)"Need two-dimensional tiles",
                (char *)"Line2FocusOnTrianglesClass::MT_IsGood");
     return 0;
  }
  return 1;
}


/* ------------------------------------------------------------------------ */
