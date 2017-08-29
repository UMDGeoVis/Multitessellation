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

/* ----------------------------- mt_cond.h ------------------------------- */

#ifndef MT_CONDITION_INCLUDED
#define MT_CONDITION_INCLUDED

#include "mt_multi.h"

/* ------------------------------------------------------------------------ */
/*                    BOOLEAN CONDITIONS ON MT TILES                        */
/* ------------------------------------------------------------------------ */

/*
This file defines an abstract class, called MT_CondClass, which provides the
the interface of boolean conditions on MT tiles.
Subclasses of MT_CondClass will implement resolution filters and focus 
sets for extracting tesselations from an MT.

Class MT_CondClass exports a boolean predicate on MT tiles, called
MT_EvalCond. 
MT_EvalCond has an additional parameter that specifies the mode of
evaluation:

1) LOOSE evaluation: 
   used inside extraction algorithms when performing expansion and 
   contraction stage
2) STRICT evaluation: 
   used at the end of extraction when selecting the active triangles
   from the extracted tesselation

The loose and strict versions may be equal in some subclasses.
*/

/* ------------------------------------------------------------------------ */
/*                                 MACROS                                   */
/* ------------------------------------------------------------------------ */

/*
Flag for mode of evaluating the condition.
*/
#define MT_LOOSE 0
#define MT_STRICT 1

/* ------------------------------------------------------------------------ */
/*                      A GENERIC TILE CONDITION                            */
/* ------------------------------------------------------------------------ */

typedef class MT_CondClass *  MT_Cond;

class MT_CondClass
{
  public:

   /*
   Return 1 if a tile satisfies the condition, 0 otherwise.
   Parameter flag specifies the mode of evaluation (flag == MT_LOOSE or 
   MT_STRICT).
   */
   virtual int MT_EvalCond(MT_MultiTesselation m, 
                           MT_INDEX t, int flag) = 0;
   /*
   Return 1 if this condition is good for the given MT.
   By default, it is always good. Specific conditions may be good 
   only for certain MTs (e.g., 2D tiles embedded in 2D).
   */
   virtual inline int MT_IsGood(MT_MultiTesselation m)  {  return 1;  }
};

/* ------------------------------------------------------------------------ */
/*             CONDITIONS IDENTICALLY TRUE AND IDENTICALLY FALSE            */
/* ------------------------------------------------------------------------ */

/*
The true condition returns 1 on any tile. As a resolution filtering
condition, it produces the coarsest mesh available in an MT. As a focus
condition, it sets the interesting area to the whole space.
Note that, within an extractor, the same effect is obtained with a null
resolution condition and a null focus condition, respectively.

The false condition returns 0 on any tile. As a resolution filtering 
condition, it produces the most refined mesh available in an MT. As a 
focus condition, it sets the interesting area to be empty.
*/
 
typedef class MT_TrueCondClass * MT_TrueCond;
typedef class MT_FalseCondClass * MT_FalseCond;

class MT_TrueCondClass : public MT_CondClass
{
   public:

   inline int MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag)
   {   return 1;   }

   inline  MT_TrueCondClass(void)  { }

};

class MT_FalseCondClass : public MT_CondClass
{
   public:

   inline int MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag)
   {   return 0;   }

   inline  MT_FalseCondClass(void)  { }

};

/* ------------------------------------------------------------------------ */
/*            COMPOSITION OF CONDITIONS THROUGH BOOLEAN OPERATORS           */
/* ------------------------------------------------------------------------ */

/*
This class allows combining conditions through boolean operators AND and OR.
When applied to focus conditions, boolean operators have the following 
meaning: AND = intersection of focus areas, OR = union.
*/

#define MT_AND_OP 1
#define MT_OR_OP  2

typedef class MT_CompoundCondClass * MT_CompoundCond;

class MT_CompoundCondClass : public MT_CondClass
{
  protected:
  
  /*
  The boolean operator.
  */
  int my_op;

  /*
  The conditions acting as arguments for the operator.
  */
  MT_Cond arg[2];

  public:
  
  /*
  Evaluation through boolean combination of the results of the two
  argument conditions.
  */
  int MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag);

  /*
  Constructor given the operator and the two argument conditions.
  */
  MT_CompoundCondClass(int op, MT_Cond c1, MT_Cond c2);
        
};
  
  
/* ------------------------------------------------------------------------ */

#endif /* MT_CONDITION_INCLUDED */
