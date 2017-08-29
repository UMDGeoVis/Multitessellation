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

/* ------------------------------ fcsrange.c ------------------------------ */

#include "geo.h"
#include "fcsrange.h"

/* ------------------------------------------------------------------------ */
/*                      CLASS FOR TILE-TO-POINT DISTANCE                    */
/* ------------------------------------------------------------------------ */

float PointDistClass ::
      SquaredDistanceTriangle2(MT_TileSet m, MT_INDEX t)
{
  float d;
  MT_INDEX * v; /* tile vertices */
  float x[3], y[3]; /* for vertex coordinates */
  int i;
  m->MT_TileVertices(t, &v);
  /* we assume that the tile is a triangle, thus three vertices */
  for (i=0; i<3; i++)
  {  x[i] = m->MT_VertexX(v[i]);
     y[i] = m->MT_VertexY(v[i]);
  }
  d = SquaredPointTriangleDist2D(p_coord[0],p_coord[1],x,y);
  return (d);
}
  
/* ------------------------------------------------------------------------ */

float PointDistClass ::
      ApproxSquaredDistanceTriangle3(MT_TileSet m, MT_INDEX t)
{ 
  MT_INDEX * v;           /* tile vertices */
  int i;                  /* vertex counter */
  float c[MT_MAX_DIM];    /* tile gravity center */
  float d, d1;            /* distances */

  m->MT_TileVertices(t,&v);
  TriangleCenter(m->MT_VertexX(v[0]),m->MT_VertexY(v[0]),m->MT_VertexZ(v[0]),
                 m->MT_VertexX(v[1]),m->MT_VertexY(v[1]),m->MT_VertexZ(v[1]),
                 m->MT_VertexX(v[2]),m->MT_VertexY(v[2]),m->MT_VertexZ(v[2]),
                 &c[0], &c[1], &c[2]);
  d = SquaredPointDist(c[0],c[1],c[2], p_coord[0],p_coord[1],p_coord[2]);
  /* we assume that the tile is a triangle, thus three vertices */
  for (i=0; i<3; i++)
  {
    d1 = SquaredPointDist(m->MT_VertexX(v[i]),m->MT_VertexY(v[i]),
                          m->MT_VertexZ(v[i]),
                          p_coord[0],p_coord[1],p_coord[2]);
    d = ( (d < d1) ? d : d1 );
  }
  return (d);
}  

/* ------------------------------------------------------------------------ */

float PointDistClass ::
      ApproxSquaredDistanceTetra3(MT_TileSet m, MT_INDEX t)
{ 
  MT_INDEX * v;           /* tile vertices */
  int i;                  /* vertex counter */
  float c[3];             /* tile gravity center */
  float * p[4];           /* vertex coordinates */
  float d, d1;            /* distances */

  m->MT_TileVertices(t, &v);
  /* we assume that the tile is a tetrahedron, thus four vertices. */
  for (i=0; i<4; i++)
  {  m->MT_VertexCoords(v[i], &p[i]);  }
  if ( PointInTetra(p_coord[0],p_coord[1],p_coord[2], 
                    p[0],p[1],p[2],p[3]) )
      return 0.0;
  TetraCenter(p[0],p[1],p[2],p[3], &c[0], &c[1], &c[3]);    
  d = SquaredPointDist(c[0],c[1],c[2], p_coord[0],p_coord[1],p_coord[2]);
  for (i=0; i<4; i++)
  {
    d1 = SquaredPointDist(p[i][0],   p[i][1],   p[i][2], 
                          p_coord[0],p_coord[1],p_coord[2]);
    d = ( (d < d1) ? d : d1 );
  }
  return (d);
}  

/* ------------------------------------------------------------------------ */
/*                      FOCUS CONSISTING OF A RANGE                         */
/* ------------------------------------------------------------------------ */

RangeClass :: RangeClass(int d, float r) : PointDistClass(d)
{  SetRadius(r);  }

/* ------------------------------------------------------------------------ */
/*                2D RANGE FOCUS ON MTs WITH TRIANGULAR TILES               */
/* ------------------------------------------------------------------------ */

Range2FocusOnTrianglesClass ::
Range2FocusOnTrianglesClass(float x, float y, float r) : RangeClass(2,r)
{  SetPoint(x,y);  }

int Range2FocusOnTrianglesClass ::
    MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag)
{
  return ( SquaredDistanceTriangle2(m,t) < (radius*radius) );
}

int Range2FocusOnTrianglesClass :: MT_IsGood(MT_MultiTesselation m)
{
  if (m->MT_TileDim() != 2)
  {
     MT_Warning((char *)"Need two-dimensional tiles",
                (char *)"Range2FocusOnTrianglesClass::MT_IsGood");
     return 0;
  }
  return 1;
}

/* ------------------------------------------------------------------------ */
/*                3D RANGE FOCUS ON MTs WITH TRIANGULAR TILES               */
/* ------------------------------------------------------------------------ */

Range3FocusOnTrianglesClass ::
Range3FocusOnTrianglesClass(WithTileErrorClass * m,
                            float x, float y, float z, float r)
: RangeClass(3,r)
{  SetPoint(x,y,z); my_mt = m;  }

int Range3FocusOnTrianglesClass :: 
    MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag)
{  
   float r = radius;
   if (flag==MT_LOOSE) r += my_mt->TileError(t);
   return ( ApproxSquaredDistanceTriangle3(m,t) < (r*r) );
}

int Range3FocusOnTrianglesClass :: MT_IsGood(MT_MultiTesselation m)
{
  if ( (m->MT_TileDim() != 2) || (m->MT_VertexDim() < 3) )
  {
     MT_Warning((char *)"Need two-dimensional tiles in at least three dimensions",
                (char *)"Range3FocusOnTrianglesClass::MT_IsGood");
     return 0;
  }
  return 1;
}

/* ------------------------------------------------------------------------ */
/*                3D RANGE FOCUS ON MTs WITH TETRAHEDRAL TILES              */
/* ------------------------------------------------------------------------ */

Range3FocusOnTetrahedraClass ::
Range3FocusOnTetrahedraClass(float x, float y, float z, float r)
: RangeClass(3,r)
{  SetPoint(x,y,z);  }

int Range3FocusOnTetrahedraClass ::
    MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag)
{
   return ( ApproxSquaredDistanceTetra3(m,t) < (radius*radius) );
}

int Range3FocusOnTetrahedraClass :: MT_IsGood(MT_MultiTesselation m)
{
  if (m->MT_TileDim() != 3)
  {
     MT_Warning((char *)"Need three-dimensional tiles",
                (char *)"Range3FocusOnTetrahedraClass::MT_IsGood");
     return 0;
  }
  return 1;
}

/* ------------------------------------------------------------------------ */
