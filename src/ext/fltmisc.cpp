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

/* ------------------------------ fltmisc.c ------------------------------- */

#include "geo.h"
#include "fltmisc.h"

/* ------------------------------------------------------------------------ */
/*                   TESTING THE THRESHOLD ON TILE ERRORS                   */
/* ------------------------------------------------------------------------ */

int TileErrFilterClass ::
    MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag)
{
  return ( my_mt->TileError(t) < my_thr->ThresholdValue(m,t) );
}

TileErrFilterClass ::
TileErrFilterClass(WithTileErrorClass * m, Threshold tr)   
{
  my_mt = m;  my_thr = tr;
}
   
/* ------------------------------------------------------------------------ */
/*                   TESTING THE THRESHOLD ON TILE AREAS                    */
/* ------------------------------------------------------------------------ */

int TriangleArea2FilterClass ::
    MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag)
{
  float area;         /* tile area */
  MT_INDEX * v;       /* tile vertices */

  m->MT_TileVertices(t,&v);
  area = TriangleArea2D(m->MT_VertexX(v[0]), m->MT_VertexY(v[0]),
                        m->MT_VertexX(v[1]), m->MT_VertexY(v[1]),
                        m->MT_VertexX(v[2]), m->MT_VertexY(v[2]));
  return ( area < my_thr->ThresholdValue(m,t) );
}

int TriangleArea2FilterClass :: MT_IsGood(MT_MultiTesselation m)
{
  if (m->MT_TileDim() != 2)
  {
     MT_Warning((char *)"Need two-dimensional tiles",
                (char *)"TriangleArea2FilterClass::MT_IsGood");
     return 0;
  }
  if (!my_thr->IsGood(m))
  {
     MT_Warning((char *)"Threshold not good for this MT",
                (char *)"TriangleArea2FilterClass::MT_IsGood");
     return 0;
  }
  return 1;
}

TriangleArea2FilterClass :: TriangleArea2FilterClass(Threshold tr)
{  my_thr = tr;  }

/* ------------------------------------------------------------------------ */

int TriangleArea3FilterClass ::
    MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag)
{
  float area;         /* tile area */
  MT_INDEX * v;       /* tile vertices */
     
  m->MT_TileVertices(t,&v);
  area = TriangleArea(m->MT_VertexX(v[0]),
                      m->MT_VertexY(v[0]),
                      m->MT_VertexZ(v[0]),
                      m->MT_VertexX(v[1]),
                      m->MT_VertexY(v[1]),
                      m->MT_VertexZ(v[1]),
                      m->MT_VertexX(v[2]),
                      m->MT_VertexY(v[2]),
                      m->MT_VertexZ(v[2]));
  return ( area < my_thr->ThresholdValue(m,t) );
}

void TriangleArea3FilterClass ::
    MT_MinMax(MT_MultiTesselation m, float *min, float *max)
{
  float area;         /* tile area */
  MT_INDEX t, * v;       /* tile vertices */
   
  m->MT_TileVertices(1,&v);
  *min = *max = TriangleArea(m->MT_VertexX(v[0]),
                      m->MT_VertexY(v[0]),
                      m->MT_VertexZ(v[0]),
                      m->MT_VertexX(v[1]),
                      m->MT_VertexY(v[1]),
                      m->MT_VertexZ(v[1]),
                      m->MT_VertexX(v[2]),
                      m->MT_VertexY(v[2]),
                      m->MT_VertexZ(v[2]));

  for (t=2; t<=m->MT_TileNum(); t++)
  {
    m->MT_TileVertices(t,&v);
    area = TriangleArea(m->MT_VertexX(v[0]),
                        m->MT_VertexY(v[0]),
                        m->MT_VertexZ(v[0]),
                        m->MT_VertexX(v[1]),
                        m->MT_VertexY(v[1]),
                        m->MT_VertexZ(v[1]),
                        m->MT_VertexX(v[2]),
                        m->MT_VertexY(v[2]),
                        m->MT_VertexZ(v[2]));
	if (area > *max)
	  *max = area;
	else if (area < *min)
	  *min = area;
  }
}
TriangleArea3FilterClass :: TriangleArea3FilterClass (Threshold tr)
{  my_thr = tr;  }

int TriangleArea3FilterClass :: MT_IsGood(MT_MultiTesselation m)
{
  if ( (m->MT_TileDim() != 2) || (m->MT_VertexDim() < 3) )
  {
     MT_Warning((char *)"Need two-dimensional tiles in at least three dimensions",
                (char *)"TriangleArea3FilterClass::MT_IsGood");
     return 0;
  }
  if (!my_thr->IsGood(m))
  {
     MT_Warning((char *)"Threshold not good for this MT",
                (char *)"TriangleArea2FilterClass::MT_IsGood");
     return 0;
  }
  return 1;
}

/* ------------------------------------------------------------------------ */
/*         TESTING THE THRESHOLD ON THE RADIUS OF THE CIRCUMCIRCLE          */
/* ------------------------------------------------------------------------ */

int Circumradius2FilterClass ::
    MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag)
{
  float rad;          /* radius of the circumcircle */
  MT_INDEX * v;       /* tile vertices */
   
  m->MT_TileVertices(t,&v);
  rad = CircumRadius2D(m->MT_VertexX(v[0]), m->MT_VertexY(v[0]),
                       m->MT_VertexX(v[1]), m->MT_VertexY(v[1]),
                       m->MT_VertexX(v[2]), m->MT_VertexY(v[2]));
  return ( rad < my_thr->ThresholdValue(m,t) );
}

Circumradius2FilterClass :: Circumradius2FilterClass(Threshold tr)
{  my_thr = tr;  }

int Circumradius2FilterClass :: MT_IsGood(MT_MultiTesselation m)
{
  if (m->MT_TileDim() != 2)
  {
     MT_Warning((char *)"Need two-dimensional tiles",
                (char *)"Circumradius2FilterClass::MT_IsGood");
     return 0;
  }
  if (!my_thr->IsGood(m))
  {
     MT_Warning((char *)"Threshold not good for this MT",
                (char *)"TriangleArea2FilterClass::MT_IsGood");
     return 0;
  }
  return 1;
}

/* ------------------------------------------------------------------------ */

int Circumradius3FilterClass ::
    MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag)
{
  float rad;          /* radius of the circumcircle */
  MT_INDEX * v;       /* tile vertices */
   
  m->MT_TileVertices(t,&v);
  rad = CircumRadius(m->MT_VertexX(v[0]), m->MT_VertexY(v[0]),
                                          m->MT_VertexZ(v[0]),
                     m->MT_VertexX(v[1]), m->MT_VertexY(v[1]),
                                          m->MT_VertexZ(v[1]),
                     m->MT_VertexX(v[2]), m->MT_VertexY(v[2]),
                                          m->MT_VertexZ(v[2]));
  return ( rad < my_thr->ThresholdValue(m,t) );
}

Circumradius3FilterClass :: Circumradius3FilterClass(Threshold tr)
{  my_thr = tr;  }

int Circumradius3FilterClass :: MT_IsGood(MT_MultiTesselation m)
{
  if ( (m->MT_TileDim() != 2) || (m->MT_VertexDim() < 3) )
  {
     MT_Warning((char *)"Need two-dimensional tiles in at least three dimensions",
                (char *)"Circumradius3FilterClass::MT_IsGood");
     return 0;
  }
  if (!my_thr->IsGood(m))
  {
     MT_Warning((char *)"Threshold not good for this MT",
                (char *)"TriangleArea2FilterClass::MT_IsGood");
     return 0;
  }
  return 1;
}

/* ------------------------------------------------------------------------ */
