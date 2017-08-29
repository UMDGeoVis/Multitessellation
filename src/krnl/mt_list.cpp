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

/* ----------------------------- mt_list.c ------------------------------ */

#include "mt_list.h"

/* ---------------------------------------------------------------------- */
/*                     LINKED LISTS OF MT INDEXES                         */
/* ---------------------------------------------------------------------- */

int MT_ListClass :: MT_AddElem(MT_INDEX i)
{
   int c;
   for (c=0;c<cursor_num;c++)
   {  if (curr[c] == MT_NULL_INDEX) curr[c] = i;  }
   if (last)
   {
     next[last] = i;
     prev[i] = last;
     next[i] = MT_NULL_INDEX;
     last = i;
   }
   else
   {
     next[i] = prev[i] = MT_NULL_INDEX;
     first = last = i;
   }
   return 1;
}

int MT_ListClass :: MT_RemElem(MT_INDEX i)
{
   int c;
   for (c=0;c<cursor_num;c++)
   {  if (i==curr[c]) curr[c] = next[i];  }
   if (i==last)
     last = prev[i];
   else
     prev[next[i]] = prev[i];
   if (i==first)
     first = next[i];
   else
     next[prev[i]] = next[i];
   next[i] = prev[i] = MT_NULL_INDEX;
   return 1;
}

/* ---------------------------------------------------------------------- */

MT_ListClass :: MT_ListClass(int size, int n_curs)
{
   prev = (MT_INDEX *) calloc ( size, sizeof(MT_INDEX) );
   next = (MT_INDEX *) calloc ( size, sizeof(MT_INDEX) );
   curr = (MT_INDEX *) calloc ( n_curs, sizeof(MT_INDEX) );
   /* calloc also initializes all array entries to zeroes */
   if (! (prev && next && curr) )
   {
     if (prev) free(prev);
     if (next) free(next);
     if (curr) free(curr);
     MT_FatalError((char*)"Allocation failed",(char *)"MT_ListClass Creator");
   }
   first = last = MT_NULL_INDEX;
   cursor_num = n_curs;
}

/* ---------------------------------------------------------------------- */

MT_ListClass :: ~MT_ListClass(void)
{
   free(prev);
   free(next);
   free(curr);
}

/* ---------------------------------------------------------------------- */

void MT_ListClass :: MT_EmptyList(void)
{
  MT_INDEX i;
  
     MT_CurrInit(1);
     while ( MT_CurrElem(1) != MT_NULL_INDEX )
     {
       i = MT_CurrElem(1);
       MT_CurrAdvance(1);
       MT_RemElem(i);
     }
}

/* ---------------------------------------------------------------------- */

int MT_ListClass :: MT_IsInList(MT_INDEX i)
{
  if (i==first) return 1;
  /* else the list contains at least two elements */
  return (prev[i] || next[i]);
}
