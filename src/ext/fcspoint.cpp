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

/* ------------------------------ fcspoint.c ------------------------------ */

#include "geo.h"
#include "fcspoint.h"

/* ------------------------------------------------------------------------ */
/*                          CLASS FOR A POINT                               */
/* ------------------------------------------------------------------------ */

void PointClass :: SetPoint(float * coord)
{  int i;
   for (i=0; i<PointDim(); i++) p_coord[i] = coord[i];
}

void PointClass :: ThePoint(float * coord)
{  int i;
   for (i=0; i<PointDim(); i++) coord[i] = p_coord[i];
}

void PointClass :: SetPoint(float x, float y)
{  p_coord[0] = x;  p_coord[1] = y;  }

void PointClass :: ThePoint(float *x, float *y)
{  (*x) = p_coord[0]; (*y) = p_coord[1];  }

void PointClass :: SetPoint(float x, float y, float z)
{  p_coord[0] = x;  p_coord[1] = y;  p_coord[2] = z;  }

void PointClass :: ThePoint(float *x, float *y, float *z)
{  (*x) = p_coord[0]; (*y) = p_coord[1]; (*z) = p_coord[2];  }
  
void PointClass :: Translate (float * trans_vector)
{
   int i;
   for (i=0; i<PointDim(); i++)
   {  p_coord[i] += trans_vector[i];  }
}
      
void PointClass :: Translate (float vectorX, float vectorY)
{
   p_coord[0] += vectorX;
   p_coord[1] += vectorY;
}

      
void PointClass :: Translate (float vectorX, float vectorY, float vectorZ)
{
   p_coord[0] += vectorX;
   p_coord[1] += vectorY;
   p_coord[2] += vectorZ;
}

PointClass :: PointClass(int d) 
{  int i;
   p_dim = d;
   for (i=0; i<d; i++)  {  p_coord[i] = 0.0;  }
}

/* ------------------------------------------------------------------------ */
/*                   FOCUS CONSISTING OF A POINT IN 2D                      */
/* ------------------------------------------------------------------------ */

Point2FocusOnTrianglesClass :: Point2FocusOnTrianglesClass(float x, float y)
: PointClass(2)
{  SetPoint(x,y);  }
  
int Point2FocusOnTrianglesClass :: 
    MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag)
{
  MT_INDEX * v; /* triangle vertices */
  float x[3], y[3];
  int i;
  
  m->MT_TileVertices(t, &v);
  /* we assume that the tile is a triangle, thus three vertices. */
  for (i=0;i<3;i++)
  {
    x[i] = m->MT_VertexX(v[i]);
    y[i] = m->MT_VertexY(v[i]);
  }
  return ( !PointOutTriangle2D(p_coord[0], p_coord[1],
                               x[0],y[0],x[1],y[1],x[2],y[2]) );
}

int Point2FocusOnTrianglesClass :: MT_IsGood(MT_MultiTesselation m)
{
  if (m->MT_TileDim() != 2)
  {
     MT_Warning((char *)"Need two-dimensional tiles",
                (char *)"Point2FocusOnTrianglesClass::MT_IsGood");
     return 0;
  }
  return 1;
}

/* ------------------------------------------------------------------------ */
/*                   FOCUS CONSISTING OF A POINT IN 3D                      */
/* ------------------------------------------------------------------------ */

Point3FocusOnTetrahedraClass :: 
Point3FocusOnTetrahedraClass(float x, float y, float z) : PointClass(3)
{  SetPoint(x,y,z);  }
     
int Point3FocusOnTetrahedraClass :: 
    MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag)
{
  MT_INDEX * v;  /* tetrahedron vertices */
  float * c[4];  /* vertex coordinates */
  int i;
  
  m->MT_TileVertices(t, &v);
  /* we assume that the tile is a tetrahedron, thus four vertices. */
  for (i=0; i<4; i++)
  {  m->MT_VertexCoords(v[i],&c[i]);  }  
  return ( !PointOutTetra(p_coord[0], p_coord[1], p_coord[2], 
                          c[0], c[1], c[2], c[3]) );
}

int Point3FocusOnTetrahedraClass :: MT_IsGood(MT_MultiTesselation m)
{
  if (m->MT_TileDim() != 3)
  {
     MT_Warning((char *)"Need three-dimensional tiles",
                (char *)"Point3FocusOnTetrahedraClass::MT_IsGood");
     return 0;
  }
  return 1;
}

/* ------------------------------------------------------------------------ */
