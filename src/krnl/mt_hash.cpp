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

/* ----------------------------- mt_hash.c --------------------------------- */

#include "mt_hash.h"

/* ------------------------------------------------------------------------- */
/*  HASH TABLES FOR GENERATING ADJACENCY LINKS IN THE EXTRACTED TESSELATION  */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/*                      CLASS FOR THE BUCKET LISTS                           */
/* ------------------------------------------------------------------------- */

MT_FacetTileClass :: 
MT_FacetTileClass (int ind, MT_INDEX t,
                   MT_INDEX sum_v, MT_INDEX min_v, MT_INDEX max_v)
{
  my_i = ind;
  my_t = t;
  my_sum = sum_v;
  my_min = min_v;
  my_max = max_v;
}

/* ------------------------------------------------------------------------- */
/*                         CLASS FOR THE HASH TABLE                          */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/*                          Auxiliary functions                              */
/* ------------------------------------------------------------------------- */

void MT_HashTableClass :: 
     MT_ComputeHints(MT_INDEX v, MT_INDEX t, 
                     MT_INDEX * sum_v, MT_INDEX * min_v, MT_INDEX * max_v)
{
  int i, first_time;
  MT_INDEX aux_v;

  first_time = 1;
  for (i=0; i<=my_mt->MT_TileDim();i++)
  {
    if ( (aux_v = my_mt->MT_TileVertex(t,i)) != v)
    {  if (first_time)
       {  (*sum_v) = (*min_v) = (*max_v) = aux_v;
          first_time = 0;
       }
       else 
       {  (*sum_v) += aux_v;
          if (aux_v < (*min_v)) (*min_v) = aux_v;
          else if (aux_v > (*max_v)) (*max_v) = aux_v;
       }
    }
  }
}

/* ------------------------------------------------------------------------- */

int MT_HashTableClass ::
    MT_SameFacet(MT_FacetTile el, MT_INDEX v, MT_INDEX t,
                 MT_INDEX sum_v, MT_INDEX min_v, MT_INDEX max_v)
{
  int i, j, diff;
  MT_INDEX v1, v2;

  /* use hints */
  if (el->my_sum != sum_v) return 0;
  if (el->my_min != min_v) return 0;
  if (el->my_max != max_v) return 0;

  if (my_mt->MT_TileDim()<4) return 1;
    
  /* the following is only executed when MT_TileDim()>=4 */

  /* look at all the vertices of the two facets */
  diff = 0;
  /* consider every vertex of the facet of this element */
  for (i=0; (!diff) && (i<=my_mt->MT_TileDim()); i++)
  {  
     if (i != el->my_i)
     {
        diff = 1;
        v1 = my_mt->MT_TileVertex(el->my_t,i);
        /* search for v1 in the facet of t, diff becomes ==0 if found */
        for (j=0; diff && (j<=my_mt->MT_TileDim()); j++)
        {  v2 = my_mt->MT_TileVertex(t,j);
           if (v2 != v)   diff = (v1!=v2);  
        }
     }
  }
  if (diff) return 0;
  return 1;
}

/* ------------------------------------------------------------------------- */
/*                       Operations on the hash table                        */
/* ------------------------------------------------------------------------- */

MT_HashTableClass :: 
MT_HashTableClass(int table_size, MT_MultiTesselation m)
{  
  my_table = (MT_FacetTile*) calloc (table_size, sizeof(MT_FacetTile));
  if (!my_table)
  {  MT_Error((char *)"Allocation failed",(char *)"MT_HashTableClass::MT_HashInit");  }
  else 
  {  my_size = table_size;
     my_mt = m;
  }
}

/* ------------------------------------------------------------------------- */

MT_HashTableClass :: ~MT_HashTableClass(void)
{
  if (my_size>0)
  {
     int i;
     MT_FacetTile aux;
     for (i=0;i<my_size;i++) 
     {
        while  (aux = my_table[i])
        {
           my_table[i] = my_table[i]->my_next;
           delete aux;
        }
     }
     free(my_table);
     my_table = NULL;
     my_size = 0;
   }
}

/* ------------------------------------------------------------------------- */

int MT_HashTableClass :: 
    MT_HashGet (int ind, MT_INDEX t, int * ii, MT_INDEX * tt)
{
  int i;
  MT_INDEX sum_v, min_v, max_v;
  MT_FacetTile aux; MT_FacetTile aux1;

  if (!my_table)  {  return MT_NULL_INDEX;  }
  MT_ComputeHints(my_mt->MT_TileVertex(t,ind), t, &sum_v, &min_v, &max_v); 
  i = MT_HashIndex (sum_v, min_v, max_v);
  if (!my_table[i]) return MT_NULL_INDEX;
  if ( MT_SameFacet(my_table[i], my_mt->MT_TileVertex(t,ind), t, 
                    sum_v, min_v, max_v) )
  {  (*ii) = my_table[i]->my_i;
     (*tt) = my_table[i]->my_t;
     aux = my_table[i];
     my_table[i] = my_table[i]->my_next;
     free(aux);
     return 1;
  }
  aux = my_table[i];
  while (aux->my_next)
  {
    aux1 = aux->my_next;
    if ( MT_SameFacet(aux1, my_mt->MT_TileVertex(t,ind), t, 
                      sum_v, min_v, max_v) )
    {  (*ii) = aux1->my_i;
       (*tt) = aux1->my_t;
       aux->my_next = aux1->my_next;
       free(aux1);
       return 1;
    }
    else aux = aux1;
  }     
  (*ii) = -1;
  (*tt) = MT_NULL_INDEX;
//fprintf(stderr,"...Non trovato\n");/*********/
  return 0; 
}

/* ------------------------------------------------------------------------- */
   
int MT_HashTableClass :: MT_HashPut (int ind, MT_INDEX t)
{ 
  int i;
  MT_INDEX sum_v, min_v, max_v;
  MT_FacetTile aux;
  
//fprintf(stderr,"Inserisco %d(%d-esima faccia)\n",t,ind);/*********/
  MT_ComputeHints(my_mt->MT_TileVertex(t,ind), t, &sum_v, &min_v, &max_v);
  aux = new MT_FacetTileClass(ind, t, sum_v, min_v, max_v);
  if (!aux)
  {  MT_Error((char *)"Allocation failed",(char *)"MT_HashTableClass::MT_HashPut");
     return 0;
  }
  i = MT_HashIndex (sum_v, min_v, max_v);
  aux->my_next = my_table[i];
  my_table[i] = aux;
  return 1;
}

/* ------------------------------------------------------------------------- */

int MT_HashTableClass :: MT_HashPop(int * ind, MT_INDEX * t)
{ 
  int i;
  MT_FacetTile aux;

  i=0;
  while ( (i<my_size) && (!my_table[i]) ) i++;
  if (i==my_size)
  {
    /* the table is empty */
    (*ind) = -1;
    (*t) = MT_NULL_INDEX;
    return 0;
  }  
  aux = my_table[i];
  (*ind) = aux->my_i;
  (*t) = aux->my_t;
  my_table[i] = aux->my_next;
  free(aux);
  aux = NULL;
  return 1;
}

/* ------------------------------------------------------------------------- */
