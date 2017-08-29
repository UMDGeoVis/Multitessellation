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

/* ----------------------------- mt_ass.c --------------------------------- */

#include "mt_ass.h"

/* ------------------------------------------------------------------------ */
/*                      INDEX - TO - INDEX ASSOCIATION                      */
/* ------------------------------------------------------------------------ */

MT_IndexToIndexClass :: MT_IndexToIndexClass(int s)
{
  if (s>0)
  {  table_size = s;
     my_table = (MT_Assoc1 *) calloc (table_size, sizeof(MT_Assoc1));
     /* calloc automatically initializes all table entries to NULL */
     if (!my_table)
     {  MT_Error((char *)"Failed allocation",(char *)"MT_IndexToIndexClass Creator");
     }
  }
  else 
  {  MT_Error((char *)"Positive table size needed",(char *)"MT_IndexToIndexClass Creator");
  }
}

/* ------------------------------------------------------------------------ */

int MT_IndexToIndexClass :: AddAssoc(MT_INDEX k, MT_INDEX i)
{
  int p;
  MT_Assoc1 aux;

  p = MT_HASH_NUMBER(k);
  aux = (MT_Assoc1) malloc (sizeof(struct MT_Assoc1Struct));
  if (!aux)
  {  MT_Error((char *)"Failed Allocation",(char *)"MT_IndexToIndexClass::AddAssoc");
     return 0;
  }
  aux->my_key = k;
  aux->my_info = i;
  aux->my_next = my_table[p];
  my_table[p] = aux;
  return 1;
}

/* ------------------------------------------------------------------------ */

MT_INDEX MT_IndexToIndexClass :: LookAssoc(MT_INDEX k)
{
  int p;
  MT_Assoc1 aux2;

  p = MT_HASH_NUMBER(k);
  aux2 = my_table[p];
  while (aux2 && (aux2->my_key != k))
  {
    aux2 = aux2->my_next;
  }
  if (aux2) /* the key is k, return the info */
     return (aux2->my_info);
  else /* the key is not present */
     return (MT_NULL_INDEX);
}

/* ------------------------------------------------------------------------ */

MT_INDEX MT_IndexToIndexClass :: GetAssoc(MT_INDEX k)
{
  int p;
  MT_Assoc1 aux1, aux2;
  MT_INDEX i;

  p = MT_HASH_NUMBER(k);
  aux1 = NULL;
  aux2 = my_table[p];
  while (aux2 && (aux2->my_key != k))
  {
    aux1 = aux2;
    aux2 = aux2->my_next;
  }
  if (aux2) /* the key is k, return the info and delete aux2 */
  {
    i = aux2->my_info;
    if (aux1) aux1->my_next = aux2->my_next;
    else my_table[p] = aux2->my_next;
    free(aux2);
  }
  else /* the key is not present */
    i = MT_NULL_INDEX;
  return i;
}

/* ------------------------------------------------------------------------ */

MT_IndexToIndexClass :: ~MT_IndexToIndexClass(void)
{
  int p;
  MT_Assoc1 aux;

  for (p=0; p<table_size; p++)
  {
    while (my_table[p])
    {
       aux = my_table[p];
       my_table[p] = my_table[p]->my_next;
       free(aux);
    }
  }
  free (my_table);
}

/* ------------------------------------------------------------------------ */
/*               INDEX - TO - INDEX PAIR ASSOCIATION                        */
/* ------------------------------------------------------------------------ */

MT_IndexToIndexPairClass :: MT_IndexToIndexPairClass(int s)
{
  if (s>0)
  {  table_size = s;
     my_table = (MT_Assoc2 *) calloc (table_size, sizeof(MT_Assoc2));
     /* calloc automatically initializes all table entries to NULL */
     if (!my_table)
     {  MT_Error((char *)"Failed allocation",(char *)"MT_IndexToIndexClass Creator");
     }
     table_count = 0;
  }
  else
  {  MT_Error((char *)"Positive table size needed",
              (char *)"MT_IndexToIndexPairClass Creator");
  }
}

int MT_IndexToIndexPairClass :: 
    AddAssoc(MT_INDEX k1, MT_INDEX k2, MT_INDEX i)
{
  int p;
  MT_Assoc2Info aux;
  MT_Assoc2 aux2;

  aux = (MT_Assoc2Info) malloc (sizeof(struct MT_Assoc2InfoStruct));
  if (!aux)
  {  MT_Error((char *)"Failed Allocation",(char *)"MT_IndexToIndexPairClass::AddAssoc");
     return 0;
  }
  aux->my_info = i;
  aux->my_next = NULL;
  p = MT_HASH_NUMBER(k1);
  aux2 = my_table[p];
  while (aux2 && ( (aux2->my_key1 != k1) || (aux2->my_key2 != k2) ))
  {
    aux2 = aux2->my_next;
  }
  if (aux2) /* the key was already present */
  { 
    aux->my_next = aux2->my_info;
    aux2->my_info = aux;
  }
  else /* create an element for the key */
  {
    aux2 = (MT_Assoc2) malloc (sizeof(struct MT_Assoc2Struct));
    if (!aux2)
    {  MT_Error((char *)"Failed Allocation",(char *)"MT_IndexToIndexPairClass::AddAssoc");
       return 0;
    }
    table_count ++;
    aux2->my_key1 = k1;
    aux2->my_key2 = k2;
    aux2->my_info = aux;
    aux2->my_next = my_table[p];
    my_table[p] = aux2;
  }
  return 1;
}

/* ------------------------------------------------------------------------ */

int MT_IndexToIndexPairClass :: 
    LookAssoc(MT_INDEX k1, MT_INDEX * k2, MT_INDEX * i)
{
  int p;
  MT_Assoc2 aux2;
  
  p = MT_HASH_NUMBER(k1);
  aux2 = my_table[p];
  while (aux2 && (aux2->my_key1 != k1))
  {
     aux2 = aux2->my_next;
  }
  if (aux2) /* the key is k1, return the information */
  {
    (*k2) = aux2->my_key2;
    (*i) = aux2->my_info->my_info;
    return 1;
  }
  else /* the key is not present */
  {
    (*k2) = (*i) = MT_NULL_INDEX;
    return 0;
  }  
}
      
/* ------------------------------------------------------------------------ */

int MT_IndexToIndexPairClass :: 
    GetAssoc(MT_INDEX k1, MT_INDEX * k2, MT_INDEX * i)
{
  int p;
  MT_Assoc2 aux1, aux2;
  MT_Assoc2Info aux;
  
  p = MT_HASH_NUMBER(k1);
  aux1 = NULL;
  aux2 = my_table[p];
  while (aux2 && (aux2->my_key1 != k1))
  {
    aux1 = aux2;
    aux2 = aux2->my_next;
  }
  if (aux2) /* the key is k1, return the information and delete aux2 */
  {
    (*k2) = aux2->my_key2;
    (*i) = aux2->my_info->my_info;
    if (aux2->my_info->my_next == NULL) /* delete aux2 */
    {
      if (aux1) aux1->my_next = aux2->my_next;
      else my_table[p] = aux2->my_next;
      free(aux2);
      table_count --;
    }
    else /* delete the first element of aux2->my_info */
    {
      aux = aux2->my_info;
      aux2->my_info = aux->my_next;
      free(aux);
    }
    return 1;
  }
  else /* the key is not present */
  {
    (*k2) = (*i) = MT_NULL_INDEX;
    return 0;
  }  
}

/* ------------------------------------------------------------------------ */

MT_IndexToIndexPairClass :: ~MT_IndexToIndexPairClass(void)
{
  int p;
  MT_Assoc2 aux1;
  MT_Assoc2Info aux;

  for (p=0; p<table_size; p++)
  {
    while (my_table[p])
    {
       aux1 = my_table[p];
       while (aux1->my_info)
       {  aux = aux1->my_info;
          aux1->my_info = aux1->my_info->my_next;
          free(aux);
       }
       my_table[p] = my_table[p]->my_next;
       free(aux1);
    }
  }
  free (my_table);
}   
      
/* ------------------------------------------------------------------------ */
