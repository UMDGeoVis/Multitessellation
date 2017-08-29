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

/* ----------------------------- mt_cond.c ------------------------------- */

#include "mt_cond.h"

/* ------------------------------------------------------------------------ */
/*            COMPOSITION OF CONDITIONS THROUGH BOOLEAN OPERATORS           */
/* ------------------------------------------------------------------------ */



int MT_CompoundCondClass :: 
    MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag)
{   
   switch (my_op)
   {
      case MT_AND_OP:
        if (!arg[0]) if (!arg[1]) return 1;
                else return arg[1]->MT_EvalCond(m,t,flag);
        else if (!arg[1]) return arg[0]->MT_EvalCond(m,t,flag);
        return ( arg[0]->MT_EvalCond(m,t,flag) &&
                 arg[1]->MT_EvalCond(m,t,flag) );
      case MT_OR_OP:
        if (!arg[0]) return 1;
        if (!arg[1]) return 1;
        return ( arg[0]->MT_EvalCond(m,t,flag) ||
                 arg[1]->MT_EvalCond(m,t,flag) );
   }
   MT_FatalError((char *)"Invalid operator",(char *)"MT_CompoundCondClass::MT_EvalCond");
   return 0; /* never reached */
}

MT_CompoundCondClass :: 
MT_CompoundCondClass(int op, MT_Cond c1, MT_Cond c2)
{
   switch (op)
   {
      case MT_AND_OP:
      case MT_OR_OP:
        if ( !(c2&&c2) ) MT_Error((char *)"Need two argument conditions",
                                  (char *)"MT_CompoundCondClass Constructor");
        arg[0] = c1;
        arg[1] = c2;
        my_op = op;
        break;
      default:
        MT_FatalError((char *)"Invalid operator",
                      (char *)"MT_CompoundCondClass Constructor");
   }
}
  
/* ------------------------------------------------------------------------ */
