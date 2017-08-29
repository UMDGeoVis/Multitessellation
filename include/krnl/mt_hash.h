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

/* ----------------------------- mt_hash.h --------------------------------- */

#ifndef MT_HASH_INCLUDED
#define MT_HASH_INCLUDED

#include "mt_multi.h"

/* ------------------------------------------------------------------------- */
/*  HASH TABLES FOR GENERATING ADJACENCY LINKS IN THE EXTRACTED TESSELATION  */
/* ------------------------------------------------------------------------- */

/*
This file defines class MT_FacetTileClass, which implememts
a hash table for facet-tile association.
A facet is specified by giving the opposite vertex to it in the tile.
*/

/* ------------------------------------------------------------------------- */
/*                      CLASS FOR THE BUCKET LISTS                           */
/* ------------------------------------------------------------------------- */

typedef class MT_FacetTileClass * MT_FacetTile;

class MT_FacetTileClass
{  
   protected:
   
   /*
   Basic information.
   */
   MT_INDEX my_t;    /* the tile */
   int my_i;         /* index in my_t of the vertex opposite to the facet */
   MT_FacetTile my_next;
   
   /*
   Hints for comparison of two facets.
   */
   MT_INDEX my_sum;  /* sum of the vertices of the facet */
   MT_INDEX my_max;  /* maximum vertex of the facet */
   MT_INDEX my_min;  /* minimum vertex of the facet */
   
   MT_FacetTileClass (int ind, MT_INDEX t,
                      MT_INDEX sum_v, MT_INDEX min_v, MT_INDEX max_v);
   
   friend class MT_HashTableClass;
};

/* ------------------------------------------------------------------------- */
/*                         CLASS FOR THE HASH TABLE                          */
/* ------------------------------------------------------------------------- */

typedef class MT_HashTableClass * MT_HashTable;

class MT_HashTableClass
{
   protected:

   MT_MultiTesselation my_mt;
   int my_size;             /* table size */
   MT_FacetTile * my_table; /* table to be allocated of size my_size */

/* ------------------------------------------------------------------------- */
/*                          Auxiliary functions                              */
/* ------------------------------------------------------------------------- */

   /*
   Hash code for a given facet of a given tile.
   */
   inline int MT_HashIndex(MT_INDEX sum_v, MT_INDEX min_v, MT_INDEX max_v)
   {  return ( (max_v - min_v) % my_size );  }

   /*
   Test whether the facet of an element is the same as the given facet.
   */
   int MT_SameFacet(MT_FacetTile el, MT_INDEX v, MT_INDEX t,
                    MT_INDEX sum_v, MT_INDEX min_v, MT_INDEX max_v);

   /*
   Compute hints for a given facet of a given tile.
   */
   void MT_ComputeHints(MT_INDEX v, MT_INDEX t,
                        MT_INDEX * sum_v, MT_INDEX * min_v, MT_INDEX * max_v);

/* ------------------------------------------------------------------------- */
/*                       Operations on the hash table                        */
/* ------------------------------------------------------------------------- */

   public:

   /*
   Creation and deletion.
   */
   MT_HashTableClass(int table_size, MT_MultiTesselation m);
   ~MT_HashTableClass(void);

   /*
   If a tile adjacent to the given facet of the given tile is in the table,
   then return such tile and facet and remove it from the table 
   (the return value is 1); otherwise return 0.
   */
   int MT_HashGet (int ind, MT_INDEX t, int * ii, MT_INDEX * tt);

   /*
   Add the facet-tile association to the table.
   Return 1 on success, 0 on failure. 
   */
   int MT_HashPut (int ind, MT_INDEX t);

   /* 
   Scan the remaining elements of the table and remove them 
   (they correspond to tiles with no adjacencies along the key facet.
   Return 0 if the table is empty, 1 otherwise.
   */
   int MT_HashPop(int * ind, MT_INDEX * t);
};

/* ------------------------------------------------------------------------- */

#endif /*  MT_HASH_INCLUDED */
