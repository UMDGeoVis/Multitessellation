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

/* ------------------------------ fcsbox.c ------------------------------- */

#include "geo.h"
#include "fcsbox.h"

/* ------------------------------------------------------------------------ */
/*                    CLASS FOR AN AXIS-PARALLEL BOX                        */
/* ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */
/*                       Functions to modify the box                        */
/* ------------------------------------------------------------------------ */

void BoxClass :: SetBox (float * min_coord, float * max_coord)
{
   int i;
   for (i=0; i<BoxDim(); i++)  
   {  minF[i] = min_coord[i];
      maxF[i] = max_coord[i];
   }
}

void BoxClass :: SetBox (float x1, float y1, float x2, float y2)
{
   minF[0] = ((x1 < x2) ? x1 : x2);
   minF[1] = ((y1 < y2) ? y1 : y2);
   maxF[0] = ((x1 > x2) ? x1 : x2);
   maxF[1] = ((y1 > y2) ? y1 : y2);
}

void BoxClass :: SetBox (float x1, float y1, float z1,
                         float x2, float y2, float z2)
{
   minF[0] = ((x1 < x2) ? x1 : x2);
   minF[1] = ((y1 < y2) ? y1 : y2);
   minF[2] = ((z1 < z2) ? z1 : z2);
   maxF[0] = ((x1 > x2) ? x1 : x2);
   maxF[1] = ((y1 > y2) ? y1 : y2);
   maxF[2] = ((z1 > z2) ? z1 : z2);
}

void BoxClass :: MoveTo (float * anchor_point)
{
   int i;
   float w;
   for (i=0; i<BoxDim(); i++)
   {  w = maxF[i] - minF[i];
      minF[i] = maxF[i] = anchor_point[i];
      maxF[i] += w;
   }
}
      
void BoxClass :: Translate (float * trans_vector)
{
   int i;
   for (i=0; i<BoxDim(); i++)
   {  minF[i] += trans_vector[i];
      maxF[i] += trans_vector[i];
   }
}

void BoxClass :: MoveTo (float anchorX, float anchorY)
{
   float w;
   w = maxF[0] - minF[0];
   minF[0] = maxF[0] = anchorX;
   maxF[0] += w;
   w = maxF[1] - minF[1];
   minF[1] = maxF[1] = anchorY;
   maxF[1] += w;
}
      
void BoxClass :: Translate (float vectorX, float vectorY)
{
   minF[0] += vectorX;
   maxF[0] += vectorX;  
   minF[1] += vectorY;
   maxF[1] += vectorY;  
}

void BoxClass :: MoveTo (float anchorX, float anchorY, float anchorZ)
{
   int i;
   float w;
   for (i=0; i<3; i++)
   {  w = maxF[0] - minF[0];
      switch (i)
      {  case 0: minF[i] = maxF[i] = anchorX; break;
         case 1: minF[i] = maxF[i] = anchorY; break;
         case 2: minF[i] = maxF[i] = anchorZ; break;
      }
      maxF[i] += w;
   }
}
      
void BoxClass :: Translate (float vectorX, float vectorY, float vectorZ)
{
   minF[0] += vectorX;
   maxF[0] += vectorX;  
   minF[1] += vectorY;
   maxF[1] += vectorY;
   minF[2] += vectorZ;
   maxF[2] += vectorZ;  
}

/* ------------------------------------------------------------------------ */
/*                       Functions to return the box                        */
/* ------------------------------------------------------------------------ */

void BoxClass :: TheBox (float ** min_coord, float ** max_coord)
{  (*min_coord) = minF;   (*max_coord) = maxF;  }

void BoxClass :: TheBox (float *x1, float *y1, float *x2, float *y2)
{
   (*x1) = minF[0]; (*y1) = minF[1]; 
   (*x2) = maxF[0]; (*y2) = maxF[1];
}
   
void BoxClass :: TheBox (float *x1, float *y1, float *z1,
                       float *x2, float *y2, float *z2)
{
   (*x1) = minF[0]; (*y1) = minF[1]; (*z1) = minF[2];
   (*x2) = maxF[0]; (*y2) = maxF[1]; (*z2) = maxF[2];
}
   
/* ------------------------------------------------------------------------ */
/*                          Creation of the box                             */
/* ------------------------------------------------------------------------ */

BoxClass :: BoxClass(int d, float * min_coord, float * max_coord)
{
   b_dim = d; 
   SetBox(min_coord, max_coord);
}

BoxClass :: BoxClass(int d)
{
   int i;
   b_dim = d; 
   for (i=0; i<b_dim; i++)  {  minF[i] = maxF[i] = 0.0;  }
}
                  
/* ------------------------------------------------------------------------ */
/*              TWO-DIMENSIONAL BOX ON MTs WITH TRIANGULAR TILES            */
/* ------------------------------------------------------------------------ */

Box2FocusOnTrianglesClass ::
Box2FocusOnTrianglesClass(float * min_coord, float * max_coord)
: BoxClass(2, min_coord, max_coord)
{ }

Box2FocusOnTrianglesClass ::
Box2FocusOnTrianglesClass(float x1, float y1, float x2, float y2)
: BoxClass(2)
{  SetBox(x1,y1,x2,y2);  }
      
int Box2FocusOnTrianglesClass :: 
    MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag)
{ 
  int i;
  MT_INDEX * v; /* triangle vertices */
  float x[3], y[3];
  
  m->MT_TileVertices(t, &v); 
  /* we assume that the tile is a triangle, thus three vertices. */
  for (i=0;i<3;i++)
  {
    x[i] = m->MT_VertexX(v[i]);
    y[i] = m->MT_VertexY(v[i]);
  }  
  if ( ClipTriangle2D(minF[0],minF[1],maxF[0],maxF[1], x, y) )  
  {  return 1;  }
  return 0;
}

int Box2FocusOnTrianglesClass :: MT_IsGood(MT_MultiTesselation m)
{
  if (m->MT_TileDim() != 2)
  {
     MT_Warning((char *)"Need two-dimensional tiles",
                (char *)"Box2FocusOnTrianglesClass::MT_IsGood");
     return 0;
  }
  return 1;
}

/* ------------------------------------------------------------------------ */
/*            THREE-DIMENSIONAL BOX ON MTs WITH TRIANGULAR TILES            */
/* ------------------------------------------------------------------------ */

Box3FocusOnTrianglesClass ::
Box3FocusOnTrianglesClass(WithTileError m,
                          float * min_coord, float * max_coord)
: BoxClass(3, min_coord, max_coord)
{  my_mt = m;  } 

Box3FocusOnTrianglesClass ::
Box3FocusOnTrianglesClass(WithTileError m,
                          float x1, float y1, float z1,
                          float x2, float y2, float z2)
: BoxClass(3)
{
   SetBox(x1,y1,z1, x2,y2,z2);
   my_mt = m;
}

int Box3FocusOnTrianglesClass :: 
    MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag)
{ 
  MT_INDEX * v;   /* triangle vertices */
  float x[3], y[3], z[3];
  int i;
  float minXX = minF[0], minYY = minF[1], minZZ = minF[2];
  float maxXX = maxF[0], maxYY = maxF[1], maxZZ = maxF[2];
  
  m->MT_TileVertices(t, &v);
  /* we assume that the tile is a triangle, thus three vertices. */
  for (i=0;i<3;i++)
  {
    x[i] = m->MT_VertexX(v[i]);
    y[i] = m->MT_VertexY(v[i]);
    z[i] = m->MT_VertexZ(v[i]);
  }  
  /* if flag is MT_LOOSE, expand the tile of an amount equal to its
     approximation error. */
  if (flag==MT_LOOSE) 
  {  
    minXX -= my_mt->TileError(t); maxXX += my_mt->TileError(t);
    minYY -= my_mt->TileError(t); maxYY += my_mt->TileError(t);
    minZZ -= my_mt->TileError(t); maxZZ += my_mt->TileError(t);
  }
  if ( ClipTriangle3D(minXX,minYY,minZZ, maxXX,maxYY,maxZZ, x,y,z) )
  {  return 1;  }
  return 0;
}

int Box3FocusOnTrianglesClass :: MT_IsGood(MT_MultiTesselation m)
{
  if ( (m->MT_TileDim() != 2) || (m->MT_VertexDim() < 3) )
  {
     MT_Warning((char *)"Need two-dimensional tiles in at least three dimensions",
                (char *)"Box3FocusOnTrianglesClass::MT_IsGood");
     return 0;
  }
  return 1;
}

/* ------------------------------------------------------------------------ */
/*            THREE-DIMENSIONAL BOX ON MTs WITH TETRAHEDRAL TILES           */
/* ------------------------------------------------------------------------ */

Box3FocusOnTetrahedraClass ::
Box3FocusOnTetrahedraClass(float * min_coord, float * max_coord)
: BoxClass(3, min_coord, max_coord)
{ }

Box3FocusOnTetrahedraClass ::
Box3FocusOnTetrahedraClass(float x1, float y1, float z1,
                           float x2, float y2, float z2)
: BoxClass(3)
{  SetBox(x1,y1,z1, x2,y2,z2);  }

int Box3FocusOnTetrahedraClass ::
    MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag)
{ 
  MT_INDEX * v;  /* tetrahedron vertices */
  float * c[4];  /* vertex coordinates */
  float x[3], y[3], z[3]; /* vertex coordinates */
  int i, j;
 
  m->MT_TileVertices(t, &v);
  /* we assume that the tile is a tetrahedron, thus four vertices. */

  /* check if one tetrahedron vertex is inside box */
  for (i=0;i<4;i++)
  {
    m->MT_VertexCoords(v[i], &c[i]);
    if ( (minF[0]<=c[i][0]) && (c[i][0]<=maxF[0]) &&
         (minF[1]<=c[i][1]) && (c[i][1]<=maxF[1]) &&
         (minF[2]<=c[i][2]) && (c[i][2]<=maxF[2]) ) return 1;
  }

  /* check if one box vertex is inside tetrahedron */
  if ( !PointOutTetra(minF[0],minF[1],minF[2], c[0],c[1],c[2],c[3]) )
     return 1;
  if ( !PointOutTetra(minF[0],minF[1],maxF[2], c[0],c[1],c[2],c[3]) )
     return 1;
  if ( !PointOutTetra(minF[0],maxF[1],minF[2], c[0],c[1],c[2],c[3]) )
     return 1;
  if ( !PointOutTetra(maxF[0],minF[1],minF[2], c[0],c[1],c[2],c[3]) )
     return 1;
  if ( !PointOutTetra(maxF[0],maxF[1],maxF[2], c[0],c[1],c[2],c[3]) )
     return 1;
  if ( !PointOutTetra(maxF[0],maxF[1],minF[2], c[0],c[1],c[2],c[3]) )
     return 1;
  if ( !PointOutTetra(maxF[0],minF[1],maxF[2], c[0],c[1],c[2],c[3]) )
     return 1;
  if ( !PointOutTetra(minF[0],maxF[1],maxF[2], c[0],c[1],c[2],c[3]) )
     return 1;

  /* check if one triangular facet intersects box */
  for (i=0;i<4;i++)   
  {
    for (j=0;j<3;j++)
    {  x[j] = m->MT_VertexX(v[(i+j)%4]);
       y[j] = m->MT_VertexY(v[(i+j)%4]);
       z[j] = m->MT_VertexZ(v[(i+j)%4]);
    }
    if ( ClipTriangle3D(minF[0],minF[1],minF[2],
                        maxF[0],minF[1],minF[2], x,y,z) ) 
    return 1;
  }

  /* box and tetrahedron do not intersect each other */
  return 0;
}

int Box3FocusOnTetrahedraClass :: MT_IsGood(MT_MultiTesselation m)
{
  if (m->MT_TileDim() != 3)
  {
     MT_Warning((char *)"Need two-dimensional tiles in at least three dimensions",
                (char *)"Box3FocusOnTrianglesClass::MT_IsGood");
     return 0;
  }
  return 1;
}

/* ------------------------------------------------------------------------ */
