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

/* ------------------------------ fcsfield.h ------------------------------ */

#ifndef MT_FIELDFOCUS_INCLUDED
#define MT_FIELDFOCUS_INCLUDED

#include "tilerror.h"
#include "vrtfield.h"
#include "mt_cond.h"

/* ------------------------------------------------------------------------ */
/*                     CONDITIONS USING FIELD VALUES                        */
/* ------------------------------------------------------------------------ */

/*
This file defines focus conditions for MTs representing scalar fields.
The condition evaluates TRUE if a tile contains a certain field value,
or at least one value of a certain set of field values.
Field values are known at the vertices of the tile through function
Field. The field is interpolated linearly inside the tile.
*/

/* ------------------------------------------------------------------------ */
/*                  GENERIC CLASS FOR A SET OF FIELD VALUES                 */
/* ------------------------------------------------------------------------ */

class FieldValClass
{
   protected:

   /* 
   Number of values in the set, array of values in the set.
   */
   int val_Num;
   float * val_Arr;

/* ------------------------------------------------------------------------ */
/* Set and get the field values of the set.                                 */
/* ------------------------------------------------------------------------ */

   protected:

   /*
   Allocate space for num values, the actual values are undefined.
   */
   int SetFieldValNum(int num);

   public:

   /*
   Return the number of values in the set.
   */
   inline int FieldValNum(void)  {  return(val_Num);  }

   /* 
   Fill the set with num values contained in the given array.
   */
   void SetFieldVal(int num, float * val);

   /*
   Fill the set with num values, where the first one is val0, 
   and the i-th one is val0 + (i*step).
   */
   void SetFieldVal(int num, float val0, float step);

   /*
   Set, and return the i-th value, with i in the range 0..val_Num.
   */
   inline void SetFieldVal(int i, float val)  {  val_Arr[i] = val;  }
   inline float TheFieldVal(int i)  {  return (val_Arr[i]);  }
   
/* ------------------------------------------------------------------------ */
/* Creation and deletion                                                    */
/* ------------------------------------------------------------------------ */

   /*
   Create a set containing just one field value.
   */
   FieldValClass(float val);

   /* 
   Create a set of num field values contained in the given array.
   */
   FieldValClass(int num, float * val);

   /*
   Create a set of num field values, where the first one is val0, 
   and the i-th one is val0 + (i*step).
   */
   FieldValClass(int num, float val0, float step);
   
   /*
   Deallocate the array of values.
   */
   ~FieldValClass(void);
   
};

/* ------------------------------------------------------------------------ */
/*                 FOCUS CONSITING OF A SET OF FIELD VALUES                 */
/* ------------------------------------------------------------------------ */

/*
If the MT represents a d-dimensional scalar field, the focus set is a set 
of hyperplanes in (d+1)-dimensional space (the space containing the graph
of the field), orthogonal to the (d+1)-th axis and corresponding to
a set of field values.
A tile is in the focus if its image in (d+1)-space intersects at least
one of the hyperplanes.

The strict and loose evaluation mode are implemented as follows:
1) flag == MT_LOOSE:
   intersection test takes into account the tile error Err:
   the result is 1 iff the tile, expanded of an amount equal to Err,
   intersects the hyperplane.
2) flag == MT_STRICT:
   intersection test returns 1 iff the tile intersects the hyperplane
   (the tile error is not used).
*/

/*
This focus set can only be applied to MTs that are subclasses
of WithFieldClass and WithTileErrorClass, and implement functions 
TileError and VertexField.
*/

typedef class FieldValFocusClass * FieldValFocus;

class FieldValFocusClass : public FieldValClass, public MT_CondClass
{

   protected:

   /*
   These two variables point to the same MT seen in its two aspects.
   */
   WithFieldClass * my_mt1;
   WithTileErrorClass * my_mt2;
      
   public:
   
   /* 
   Implementation of evaluation function from superclass MT_CondClass.
   */
   int MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag);

   /*
   Create a set containing just one field value.
   */
   FieldValFocusClass(WithFieldClass * m1, WithTileErrorClass * m2,
                      float val);

   /* 
   Create a set of num field values contained in the given array.
   */
   FieldValFocusClass(WithFieldClass * m1, WithTileErrorClass * m2,
                      int num, float * val);

   /*
   Create a set of num field values, where the first one is val0, 
   and the i-th one is val0 + (i*step).
   */
   FieldValFocusClass(WithFieldClass * m1, WithTileErrorClass * m2,
                      int num, float val0, float step);
   
   inline ~FieldValFocusClass(void) { }
   
};

/*********** DA SPOSTARE POI ***************/

/* 
Return the segment (x1,y1)-(x2,y2) intersection of triangle t with
plane at field value equal to val.
Return 1 if the intersection is a segment (the intersection exists and
does not degenerate into a point), 0 otherwise.
Parameters m and mf point to the same object, seen under its two
relevant superclasses.
*/
int HeightSegment (MT_MultiTesselation m, WithFieldClass * mf, 
                   MT_INDEX t, float val,
                   float *x1, float *y1, float * x2, float *y2);

/*
Return the intersection of tetrahedron t with the hyperplane at field value 
equal to val. The intersection (if it exists and does not degenerate into 
a point or a segment) can be either a triangle or a quadrilateral.
Return 1 if the intersection is one triangle, 2 if it is a quadrilateral
In other cases (intersection does not exist of degenerates into a 
point or a segment), return 0.
Parameters pti1, pt2, pt3, pt4, are arrays of three floats each, and
represent the vertices of the intersection.
If the intersection is one triangle, its vertices are pt1, pt2, pt3.
If the intersection is a quadrilateral, its vertices are
pt1,pt2,pt3,pt4.
In the current version, vertices are returned in a random order.
Parameters m and mf point to the same object, seen under its two
relevant superclasses.
*/
int HeightSection (MT_MultiTesselation m, WithFieldClass * mf,
                   MT_INDEX t, float val,
                   float *pt1, float *pt2, float *pt3, float *pt4);

#endif /* MT_FIELDFOCUS_INCLUDED */

/* ------------------------------------------------------------------------ */
