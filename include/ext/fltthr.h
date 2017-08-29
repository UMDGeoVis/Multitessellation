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

/* ------------------------------- fltthr.h ------------------------------- */

#ifndef MT_THRESHOLD_INCLUDED
#define MT_THRESHOLD_INCLUDED

#include "mt_cond.h"   /* redundant, included by fcsrange.h */
#include "fcsrange.h"  /* for class PointDistClass */

/* ------------------------------------------------------------------------ */
/*                         BASIC THRESHOLD FUNCTIONS                        */
/* ------------------------------------------------------------------------ */

/*
This file defines classes for threshold functions.
Given a tile in an MT, a threshold function provides a value that can be
used within a resolution condition to constrain some feature of the tile
(e.g., its size, its approximation error) to be within that value.
*/

/* ------------------------------------------------------------------------ */
/*                             GENERIC THRESHOLD                            */
/* ------------------------------------------------------------------------ */

/*
This abstract class defines the generic interface of any threshold.
*/

typedef class ThresholdClass * Threshold;

class ThresholdClass
{
  public:
  
  /*
  Return the value of the threshold on a tile.
  */
  virtual float ThresholdValue(MT_MultiTesselation m, MT_INDEX t) = 0;
  
  /*
  Return 1 if this threshold can be evaluated on the given MT.
  Some subclasses of thresholds may require tiles and/or embedding space
  of a certain dimension.
  */
  virtual inline int IsGood(MT_MultiTesselation m)  {  return 1;  }
};

/* ------------------------------------------------------------------------ */
/*                              UNIFORM THRESHOLD                           */
/* ------------------------------------------------------------------------ */

/*
This subclass implements a uniform threshold.
*/

typedef class UnifThresholdClass * UnifThreshold;

class UnifThresholdClass : public ThresholdClass
{

  protected:
  
  float unif_value;

  public:

  /*
  Creator.
  */
  inline UnifThresholdClass(float v)  {  unif_value = v;  }
  
  /*
  Set and return the threshold value.
  */
  inline float SetThreshold(float v) {  return ( unif_value = v );  }
  inline float TheThreshold(void)  {  return (unif_value);  }

  /*
  Implementation of the inherited function.
  */
  inline float ThresholdValue(MT_MultiTesselation m, MT_INDEX t)
  {  return ( TheThreshold() );  }
    
};

/* ------------------------------------------------------------------------ */
/*         THRESHOLD BASED ON A TILE FEATURE DESCRIBED BY A FLOAT           */
/* ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */
/*                       TYPES OF DEPENDENCY LAWS                           */
/* ------------------------------------------------------------------------ */

/*
The law takes a float argument arg, which must be >=0.
The law describes a curve of a certain type that passes through the
following two points in the arg-value plane:
(arg0, value0) and (arg1,value1),
where arg0, value0, arg1 and value1 are the parameters describing the law.
Given arg, the law computes the point (arg, value) belonging to the curve, 
and returns value.
The type of curve is determined by the dependency rule, and may be linear,
quadratic or exponential.
The law can be set in such a way to return a specific value if the argument 
is too big or too small.
*/

/*
Macros for linear, quadratic, square root, and exponential dependency.
*/
#define MT_LINEAR_DEP 1
#define MT_QUADR_DEP  2
#define MT_SQRT_DEP   3
#define MT_EXPON_DEP  9999

typedef class DepLawClass * DepLaw;

class DepLawClass
{

/* ------------------------------------------------------------------------ */
/*                        Internal structure                                */
/* ------------------------------------------------------------------------ */

  protected:
  
  /*
  Parameters describing the curve. The curves passes through the points
  (arg0,value0), (arg1,value1).
  */
  float arg0;
  float value0;
  float arg1;
  float value1;
  
  /*
  Type of dependency (linear, quadratic, exponential).
  */
  int dep_type;
  
/* ------------------------------------------------------------------------ */
/*                          Access functions                                */
/* ------------------------------------------------------------------------ */

  public:

  /*
  Return the value at a certain arg.
  */
  virtual float ComputeValue(float arg);

  /*
  Set and get the parameters of the law (not the management of too big
  arguments).
  */
  void SetLaw(float a0, float v0, float a1, float v1, int dep);
  void TheLaw(float * a0, float * v0, float * a1, float * v1, int * dep);

  /*
  For backward compatibility.
  */
  inline void SetLaw(float v0, float a1, float v1, int dep)
  {  SetLaw(0.0, v0, a1, v1, dep);  } 
  inline void TheLaw(float * v0, float * a1, float * v1, int * dep)
  {  float dummy;
     TheLaw(&dummy, v0, a1, v1, dep);
  }

  /*
  Constructor. 
  */
  DepLawClass (float a0, float v0, float a1, float v1, int dep);
  inline DepLawClass (float v0, float a1, float v1, int dep) 
  {  DepLawClass(0.0, v0, a1, v1, dep);  }

};

/* ------------------------------------------------------------------------ */
/*               THRESHOLD USING A CERTAIN DEPENDENCY LAW                   */
/* ------------------------------------------------------------------------ */

/*
These thresholds compute their value by applying a law to a given
tile feature, which is expressed as a float number.
Examples are: area, approximation error, distance from a point, etc.
Here, thresholds applying the law to the distance of the tile from a 
point are defined.
*/


/* ------------------------------------------------------------------------ */
/*     THRESHOLD FOR TRIANGLES DEPENDING ON DISTANCE FROM A POINT IN 2D     */
/* ------------------------------------------------------------------------ */

/*
This threshold considers the distance from a point in two dimensions
and applies to MTs where tiles are triangles.
If the MT is embedded in more than 2D, only the first two coordinates
of each vertex are considered for computing the distance.
*/

typedef class PDist2ThresholdOnTrianglesClass * PDist2ThresholdOnTriangles;

class PDist2ThresholdOnTrianglesClass : public ThresholdClass,
                                        public PointDistClass
{
  protected:
  
  DepLaw my_law;

  public:

  /*
  Return the threshold value at a certain arg.
  */
  float ThresholdValue(MT_MultiTesselation m, MT_INDEX t);

  /*
  Create a threshold with the reference point at (x,y), and the given law.
  */
  PDist2ThresholdOnTrianglesClass (float x, float y, DepLaw l);

  /*
  This threshold requires two-dimensional tiles.
  */
  int IsGood(MT_MultiTesselation m);

};

/* ------------------------------------------------------------------------ */
/*     THRESHOLD FOR TRIANGLES DEPENDING ON DISTANCE FROM A POINT IN 3D     */
/* ------------------------------------------------------------------------ */

/*
This threshold considers the distance from a point in three dimensions
and applies to MTs where tiles are triangles.
If the MT is embedded in more than 3D, only the first three coordinates
of each vertex are considered for computing the distance.
*/

typedef class PDist3ThresholdOnTrianglesClass * PDist3ThresholdOnTriangles;

class PDist3ThresholdOnTrianglesClass : public ThresholdClass,
                                        public PointDistClass
{
  protected:
  
  DepLaw my_law;

  public:

  /*
  Return the threshold value at a certain arg.
  */
  float ThresholdValue(MT_MultiTesselation m, MT_INDEX t);

  /*
  Create a threshold with the reference point at (x,y,z), and the given law.
  */
  PDist3ThresholdOnTrianglesClass (float x, float y, float z, DepLaw l);

  /*
  This threshold requires two-dimensional tiles in 3D.
  */
  int IsGood(MT_MultiTesselation m);
};

/* ------------------------------------------------------------------------ */
/*    THRESHOLD FOR TETRAHEDRA DEPENDING ON DISTANCE FROM A POINT IN 3D     */
/* ------------------------------------------------------------------------ */

/*
This threshold considers the distance from a point in three dimensions
and applies to MTs where tiles are tetrahedra.
If the MT is embedded in more than 3D, only the first three coordinates
of each vertex are considered for computing the distance.
*/

typedef class PDist3ThresholdOnTetrahedraClass * PDist3ThresholdOnTetrahedra;

class PDist3ThresholdOnTetrahedraClass : public ThresholdClass,
                                         public PointDistClass
{
  protected:
  
  DepLaw my_law;

  public:

  /*
  Return the threshold value at a certain arg.
  */
  float ThresholdValue(MT_MultiTesselation m, MT_INDEX t);

  /*
  Create a threshold with the reference point at (x,y,z), and the given law.
  */
  PDist3ThresholdOnTetrahedraClass (float x, float y, float z,  DepLaw l);

  /*
  This threshold requires three-dimensional tiles.
  */
  int IsGood(MT_MultiTesselation m);
};

/* ------------------------------------------------------------------------ */

#endif /* MT_THRESHOLD_INCLUDED */
