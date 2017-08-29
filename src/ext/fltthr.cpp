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

/* ------------------------------- fltthr.c ------------------------------- */

/* ------------------------------------------------------------------------ */
/*                         BASIC THRESHOLD FUNCTIONS                        */
/* ------------------------------------------------------------------------ */
  
#include "geo.h"
#include "fltthr.h"

/* ------------------------------------------------------------------------ */
/*                       TYPES OF DEPENDENCY LAWS                           */
/* ------------------------------------------------------------------------ */

void DepLawClass :: SetLaw(float a0, float v0, float a1, float v1, int dep)
{
  if ( (a0<0.0)||(a1<=a0) )
     MT_FatalError((char *)"Invalid argument",(char *)"DepLawClass :: SetLaw");
  arg0 = a0; value0 = v0; arg1 = a1; value1 = v1; dep_type = dep;
}
  
void DepLawClass ::
     TheLaw(float * a0, float * v0, float * a1, float * v1, int * dep)
{
  (*a0) = arg0; (*v0) = value0; 
  (*a1) = arg1; (*v1) = value1; 
  (*dep) = dep_type;
}

float DepLawClass :: ComputeValue(float arg)
{ 
  float v; /* return value */
  float delta, delta1;

  if (arg < arg0)  return value0;
  
  delta = arg-arg0;
  delta1 = arg1-arg0;
  switch ( dep_type )
  {
    case MT_LINEAR_DEP:
         v = ( value1 - value0 ) * ( delta/delta1 ) + value0;
         break;
    case MT_QUADR_DEP:
         v = ( value1 - value0 ) *
             ( (delta*delta)/(delta1*delta1) ) + value0;
         break;
    case MT_SQRT_DEP:
         v = ( value1 - value0 ) * ( sqrt(delta)/sqrt(delta1) ) + value0;
         break;
    case MT_EXPON_DEP: 
         v = ( value1 - value0 ) * exp(1 - (delta1/delta)) + value0;
         break;
  }
  return v;
}

DepLawClass :: DepLawClass (float a0, float v0, float a1, float v1, int dep)
{  SetLaw(a0,v0,a1,v1,dep);  }
  
/* ------------------------------------------------------------------------ */
/*     THRESHOLD FOR TRIANGLES DEPENDING ON DISTANCE FROM A POINT IN 2D     */
/* ------------------------------------------------------------------------ */

float PDist2ThresholdOnTrianglesClass :: 
      ThresholdValue(MT_MultiTesselation m, MT_INDEX t)
{
  float d = sqrt(SquaredDistanceTriangle2(m,t));
  return ( my_law->ComputeValue(d) );
}

PDist2ThresholdOnTrianglesClass :: 
PDist2ThresholdOnTrianglesClass (float x, float y, DepLaw l)
:  PointDistClass(2)
{  SetPoint(x,y); my_law = l;  }

int PDist2ThresholdOnTrianglesClass :: IsGood(MT_MultiTesselation m)
{  return (m->MT_TileDim()==2);  }

/* ------------------------------------------------------------------------ */
/*     THRESHOLD FOR TRIANGLES DEPENDING ON DISTANCE FROM A POINT IN 3D     */
/* ------------------------------------------------------------------------ */

float PDist3ThresholdOnTrianglesClass ::
      ThresholdValue(MT_MultiTesselation m, MT_INDEX t)
{
  float d = sqrt(ApproxSquaredDistanceTriangle3(m,t));
  return ( my_law->ComputeValue(d) );
}

PDist3ThresholdOnTrianglesClass ::
PDist3ThresholdOnTrianglesClass (float x, float y, float z,  DepLaw l)
:  PointDistClass(3)
{  SetPoint(x,y,z); my_law = l;  }

int PDist3ThresholdOnTrianglesClass :: IsGood(MT_MultiTesselation m)
{  return ( (m->MT_TileDim()==2) && (m->MT_TileDim()>=3) );  }

/* ------------------------------------------------------------------------ */
/*    THRESHOLD FOR TETRAHEDRA DEPENDING ON DISTANCE FROM A POINT IN 3D     */
/* ------------------------------------------------------------------------ */

float PDist3ThresholdOnTetrahedraClass ::
      ThresholdValue(MT_MultiTesselation m, MT_INDEX t)
{
  float d = sqrt(ApproxSquaredDistanceTetra3(m,t));
  return ( my_law->ComputeValue(d) );
}

PDist3ThresholdOnTetrahedraClass :: 
PDist3ThresholdOnTetrahedraClass (float x, float y, float z,  DepLaw l)
:  PointDistClass(3)
{  SetPoint(x,y,z); my_law = l;  }

int PDist3ThresholdOnTetrahedraClass :: IsGood(MT_MultiTesselation m)
{  return (m->MT_TileDim()==3);  }

/* ------------------------------------------------------------------------ */
