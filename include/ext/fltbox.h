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

/* ------------------------------- fltbox.h ------------------------------- */

#ifndef MT_BOXFILTER_INCLUDED
#define MT_BOXFILTER_INCLUDED

#include "mt_cond.h"

/* ------------------------------------------------------------------------ */
/*                 EXTRACTION CONDITION DEPENDING ON A BOX                  */
/* ------------------------------------------------------------------------ */

/*
This file defines a resolution filter condition accepting a tile if it
fits into an axis-parallel box with given side lengths.
*/

/* ------------------------------------------------------------------------ */
/*                    CLASS FOR A LOCATION-INDEPENDENT                      */
/* ------------------------------------------------------------------------ */

/*
This class implements a box with a given size, but without a location 
in space.
*/

typedef class FreeBoxClass * FreeBox;

class FreeBoxClass
{

/* ------------------------------------------------------------------------ */
/*                        Internal structure                                */
/* ------------------------------------------------------------------------ */

  protected:
 
  /*
  Side lengths. Only the first d will be used if the box is in 
  d-dimensional space.     
  */
  float box_dim[MT_MAX_DIM];

  /*
  Number of coordinated of the embedding space.
  */
  int b_dim;

/* ------------------------------------------------------------------------ */
/*                          Access functions                                */
/* ------------------------------------------------------------------------ */

   public:

   /*
   Return the number of coordinates of the space in which the box is defined.
   */
   inline int BoxDim(void) {  return (b_dim);  }
          
/* ------------------------------------------------------------------------ */
/*                       Functions to modify the box                        */
/* ------------------------------------------------------------------------ */

   public:
   
   void SetBox (float * side_len);
   void SetEdgeLength (int i, float w) ;
   
   /*
   The following functions make sense only if number of
   dimensions is 2, or 3, respectively.
   */

   void SetBox (float lx, float ly);
   void SetBox (float lx, float ly, float lz);

   inline void SetLengthX (float wx)   {  SetEdgeLength(0,wx);  }
   inline void SetLengthY (float wy)   {  SetEdgeLength(1,wy);  }
   inline void SetLengthZ (float wz)   {  SetEdgeLength(2,wz);  }

/* ------------------------------------------------------------------------ */
/*                       Functions to return the box                        */
/* ------------------------------------------------------------------------ */

   /*
   Return a pointer to the array of edge lengths.
   */
   void TheBox (float ** side_len);

   /*
   The following functions make sense only if number of
   dimensions is 2, or 3, respectively.
   */
   
   void TheBox (float *lx, float *ly);
   void TheBox (float *lx, float *ly, float *lz);
   
/* ------------------------------------------------------------------------ */
/*                          Creation of the box                             */
/* ------------------------------------------------------------------------ */

   public:
   
   /*
   Create a d-dimensional box with the given min and max coordinates.
   */
   FreeBoxClass(int d, float * side_len);

   /*
   Create a d-dimensional box of null size where both max and min
   coordinates are all zeroes.
   */
   FreeBoxClass(int d);
                  
};

/* ------------------------------------------------------------------------ */
/*                       BOX-BASED RESOLUTION CONDITION                     */
/* ------------------------------------------------------------------------ */
  
/*
This extraction condition accepts a tile if it fits in a given axis-parallel 
box. It is suitable for generic k-dimensional MTs in d dimensions.
*/

typedef class BoxFilterClass * BoxFilter;

class BoxFilterClass : public FreeBoxClass, public MT_CondClass
{
   public:

   /*
   Implementation of abstract function from superclass MT_CondClass.
   */
   int MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag);

   /*
   Create a box with the given side lenghts. 
   */
   BoxFilterClass(int dim, float * side_len);

};

/* ------------------------------------------------------------------------ */

#endif /* MT_BOXFILTER_INCLUDED */
