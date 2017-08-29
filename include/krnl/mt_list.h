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

/* ------------------------------ mt_list.h ----------------------------- */

#ifndef MT_LIST_INCLUDED
#define MT_LIST_INCLUDED

#include "mt_glob.h"

/* ---------------------------------------------------------------------- */
/*                     LINKED LISTS OF MT INDEXES                         */
/* ---------------------------------------------------------------------- */

/* 
Class for a doubly linked list of elements of type MT_INDEX,
Used to implement lists of MT nodes and lists of MT arcs.
The implementation is static, based on arrays.
A list can support several cursors, the maximum number of such
cursors is determined at the creation of the list.
*/

typedef class MT_ListClass * MT_List;

class MT_ListClass
{
   private:
   
   /*
   Arrays containing the previous and the next element of every 
   MT entity. If an entity is not in the list, the corresponding
   array entries contain MT_NULL_INDEX.
   */
   MT_INDEX * prev;
   MT_INDEX * next;

   /*
   First and last element of the list.
   */
   MT_INDEX first;
   MT_INDEX last;
   
   /*
   Array of cursors, and number of cursors.
   */
   MT_INDEX * curr;
   int cursor_num;

/* ---------------------------------------------------------------------- */
/*                         Creation and deletion                          */
/* ---------------------------------------------------------------------- */

   public:

   /*
   Create list that can contain size elements, and supporting n_curs 
   cursors. 
   */
   MT_ListClass(int size, int n_curs);  
   
   /* 
   Delete list and release memory.
   */
   ~MT_ListClass(void);
   
   /* 
   Empty the list without releasing it.
   */
   void MT_EmptyList(void);

/* ---------------------------------------------------------------------- */
/*                    Functions for scanning the list                     */
/* ---------------------------------------------------------------------- */

   public:

   inline MT_INDEX MT_FirstElem(void)  {  return (first);  }
   inline MT_INDEX MT_NextElem(MT_INDEX i)  {  return (next[i]);  }
   inline MT_INDEX MT_CurrElem(int c)  {  return (curr[c]);  }

   inline void MT_CurrInit(int c)    {  curr[c] = first;  }
   inline void MT_CurrAdvance(int c) {  curr[c] = next[curr[c]];  }
   inline void MT_CurrFlush(int c)   {  curr[c] = last;  }
   inline void MT_CurrReach(int c1,int c2) {  curr[c1] = curr[c2];  }


/* ---------------------------------------------------------------------- */
/*                   Functions for modifying the list                     */
/* ---------------------------------------------------------------------- */

/* 
We assume that an MT_INDEX is added to a list only if not present, and
it is deleted from a list only if present. No checks are made.
Insertion and deletion are implemented in such a way to allow continuing 
scanning the list through a cursor.
*/
   /*
   Add an element at the end of the list. I the list was empty, set 
   the cursor on the new element.
   */
   int MT_AddElem(MT_INDEX i); 
     
   /*
   Delete an element. If the cursor was on the element to be deleted, 
   then move the cursor on the next element.
   */
   int MT_RemElem(MT_INDEX i);

   /* 
   Test whether an MT_INDEX is in the list.
   */
   int MT_IsInList(MT_INDEX i);
};

/* ---------------------------------------------------------------------- */
  
#endif /* MT_LIST_INCLUDED */

