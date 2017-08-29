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

/* ----------------------------- mt_ass.h --------------------------------- */

#ifndef MT_ASSOC_INCLUDED
#define MT_ASSOC_INCLUDED

#include "mt_glob.h"

/* ------------------------------------------------------------------------ */
/*                    CLASSES FOR ASSOCIATION TABLES                        */
/* ------------------------------------------------------------------------ */

/*
These classes implement association tables used as auxiliary structures
inside MT tracers and MT builders.
*/

/* ------------------------------------------------------------------------ */
/*                                 MACRO                                    */
/* ------------------------------------------------------------------------ */

/*
Hash function.
*/
#define MT_HASH_NUMBER(k)  ((k) % table_size)

/* ------------------------------------------------------------------------ */
/*                      INDEX - TO - INDEX ASSOCIATION                      */
/* ------------------------------------------------------------------------ */

/* 
Class MT_IndexToIndexClass implements associations MT_INDEX --> MT_INDEX. 
We assume that, for any key k, only one pair k --> i exists in the table.

Used within MT builders to record:
- associations tile --> node that either creates or removes the tile.
- association tile --> tile for renumbering tiles.
*/

/* ------------------------------------------------------------------------ */
/*                          AUXILIARY STRUCTURE                             */
/* ------------------------------------------------------------------------ */

typedef struct MT_Assoc1Struct
{  
  MT_INDEX my_key;
  MT_INDEX my_info;
  struct MT_Assoc1Struct * my_next;
} * MT_Assoc1;

/* ------------------------------------------------------------------------ */
/*                         THE ASSOCIATION CLASS                            */
/* ------------------------------------------------------------------------ */

typedef class MT_IndexToIndexClass * MT_IndexToIndex;

class MT_IndexToIndexClass
{
  protected:
  
  /* 
  The association table and the number of entries in it.
  */
  MT_Assoc1 * my_table;
  int table_size;
  
  public:
  
  /*
  Create table given the number of entries. The table is initially empty.
  Return 1 on success, 0 on failure.
  */
  MT_IndexToIndexClass(int s);

  /* 
  Add association index k --> index i in the table.
  Return 1 on success, 0 otherwise.
  */
  int AddAssoc(MT_INDEX k, MT_INDEX i);

  /* 
  Search the table for an association index k --> index i. If found,
  return i. If no association is present for key k, return MT_NULL_INDEX.
  */ 
  MT_INDEX LookAssoc(MT_INDEX k);

  /* 
  Search the table for an association index k --> index i. If found,
  return i and remove the association k --> i from the table.
  If no association is present for key k, return MT_NULL_INDEX.
  */
  MT_INDEX GetAssoc(MT_INDEX k);

  /*
  Delete associaton table.
  */
  ~MT_IndexToIndexClass(void);

};

/* ------------------------------------------------------------------------ */
/*               INDEX - TO - INDEX PAIR ASSOCIATION                        */
/* ------------------------------------------------------------------------ */

/* 
Class MT_IndexToIndexPairClass implements associations 
MT_INDEX --> (MT_INDEX --> MT_INDEX). 
Given a primary key k1 and a secondary key k2, several entries of the 
form k1 --> k2 --> i may exist in the table.
Thus, for each pair (k1,k2) a list of MT_INDEXes is maintained.
Thus, a generic element looks like k1 --> (k2 --> [i1,...,iM]), and must be 
though as a set of M entries k1 --> (k2 --> i1), ..., k1 --> (k2 --> iM).
*/

/* ------------------------------------------------------------------------ */
/*                         AUXILIARY STRUCTURES                             */
/* ------------------------------------------------------------------------ */

typedef struct MT_Assoc2InfoStruct
{
  MT_INDEX my_info;
  struct MT_Assoc2InfoStruct * my_next;
} * MT_Assoc2Info;

typedef struct MT_Assoc2Struct
{  
  MT_INDEX my_key1;      /* node, primary key */
  MT_INDEX my_key2;      /* node, secondary key */
  MT_Assoc2Info my_info; /* list of tiles */
  struct MT_Assoc2Struct * my_next;
} * MT_Assoc2;

/* ------------------------------------------------------------------------ */
/*                          THE ASSOCIATION CLASS                           */
/* ------------------------------------------------------------------------ */

typedef class MT_IndexToIndexPairClass * MT_IndexToIndexPair;

class MT_IndexToIndexPairClass
{

  protected:

  /* 
  The association table and the number of entries in it.
  */
  MT_Assoc2 * my_table;
  int table_size;

  /*
  Number of MT_Assoc2 in the table. 
  */
  int table_count;
  
  public:
  
  /*
  Create table given the number of entries. 
  Return 1 on success, 0 on failure.
  */
  MT_IndexToIndexPairClass(int s);

  /* 
  Add association k1 --> (k2 --> ei) in the table.
  Return 1 on success, 0 otherwise.
  */
  int AddAssoc(MT_INDEX k1, MT_INDEX k2, MT_INDEX i);

  /* 
  Search the table for an association k1 --> (k2 --> i). If found,
  return k2 and i. Return 1 if key k2 was present, 0 otherwise.
  */ 
  int LookAssoc(MT_INDEX k1, MT_INDEX * k2, MT_INDEX * i);

  /* 
  Search the table for an association k1 --> (k2 --> i). If found,
  return k2 and i, and remove the association k1 --> (k2 --> i) from 
  the table. Return 1 if key k2 was present, 0 otherwise.
  */
  int GetAssoc(MT_INDEX k1, MT_INDEX * k2, MT_INDEX * i);

  /*
  Return the number of associations k1 --> (k2 --> i), with distinct
  pairs (k1,k2), present in this table.
  */
  inline int MT_AssocNum(void)  {  return (table_count);  }

  /*
  Delete associaton table.
  */
  ~MT_IndexToIndexPairClass(void);
  
};

#endif /* MT_ASSOC_INCLUDED */
