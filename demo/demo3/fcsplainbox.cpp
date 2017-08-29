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
#include "fcsplainbox.h"
                  
/* ------------------------------------------------------------------------ */
/*            THREE-DIMENSIONAL BOX ON MTs WITH TRIANGULAR TILES            */
/* ------------------------------------------------------------------------ */

SimpleBox3FocusOnTrianglesClass ::
SimpleBox3FocusOnTrianglesClass(float * min_coord, float * max_coord)
: BoxClass(3, min_coord, max_coord)
{  } 

SimpleBox3FocusOnTrianglesClass ::
SimpleBox3FocusOnTrianglesClass(float x1, float y1, float z1,
                                float x2, float y2, float z2)
: BoxClass(3)
{
   SetBox(x1,y1,z1, x2,y2,z2);
}

int SimpleBox3FocusOnTrianglesClass :: 
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
  if ( ClipTriangle3D(minXX,minYY,minZZ, maxXX,maxYY,maxZZ, x,y,z) )
  {  return 1;  }
  return 0;
}

int SimpleBox3FocusOnTrianglesClass :: MT_IsGood(MT_MultiTesselation m)
{
  if ( (m->MT_TileDim() != 2) || (m->MT_VertexDim() < 3) )
  {
     MT_Warning("Need two-dimensional tiles in at least three dimensions",
                "SimpleBox3FocusOnTrianglesClass::MT_IsGood");
     return 0;
  }
  return 1;
}

/* ------------------------------------------------------------------------ */
