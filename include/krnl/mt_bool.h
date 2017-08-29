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

/* ------------------------------ mt_bool.h ------------------------------- */

#ifndef MT_BOOL_ATTR_INCLUDED
#define MT_BOOL_ATTR_INCLUDED

#include "mt_attr.h"
#include "mt_abld.h"
#include "mt_bit.h"

/* ------------------------------------------------------------------------ */
/*                  BOOLEANS (BITS) AS MT ATTRIBUTES                        */
/* ------------------------------------------------------------------------ */

/*
This file defines classes to:
- implement tables of boolean (bit) attributes inside an MT (class 
  MT_BoolTableClass)
- record boolean attributes while tracing a history and converting them
  into an MT attribute file (class MT_BoolBuildingInterfaceClass).
*/

/* ------------------------------------------------------------------------ */
/*                           BIT ATTRIBUTES                                 */
/* ------------------------------------------------------------------------ */

typedef class MT_BoolTableClass * MT_BoolTable;

class MT_BoolTableClass : public MT_AttrTableClass, public MT_BitClass
{
  public:

  inline char * MT_ClassKeyword()  {  return (char *)"MT_BoolTable";  }

  /* 
  Allocate the attribute array to n positions.
  Return 1 on success, 0 on failure.
  */
  int MT_SetAttrNum(MT_INDEX n);

  /*
  Read/write functions inherited from superclass MT_AttrTableClass.
  Read and write an individual boolean attribute as an integer (0/1). 
  Used while tracing the history.
  Different functions, which read/write one bit block at a time,
  are used for the attribute file.
  */
  int MT_ReadAttr(FILE * fd, MT_INDEX i, int file_encoding);
  void MT_WriteAttr(FILE * fd, MT_INDEX i, int file_encoding);

  /*
  New read/write functions. Read and write a block of attributes.
  */
  int MT_ReadBlock(FILE * fd, MT_INDEX block, int file_encoding);
  void MT_WriteBlock(FILE * fd, MT_INDEX block, int file_encoding);

  /*
  Functions redefined in order to read/write blocks of attributes.
  */
  int MT_ReadBody(FILE * fd, int file_encoding);
  void MT_WriteBody(FILE * fd, int file_encoding);

  /*
  Functions redefined from superclass MT_PersistentClass.
  */
  int MT_ReadObjParams(FILE * fd);
  void MT_WriteObjParams(FILE * fd);

  /*
  Get/set the attribute value. Parameter/return value contains 0 or 1.
  */
  inline void MT_SetAttrValue(MT_INDEX i, int a)  {  MT_SetBit(i,a);  }
  int MT_TheAttrValue(MT_INDEX i)  {  return MT_TheBit(i);  }

  /*
  Creator and destructor.
  */
  MT_BoolTableClass(void);
  ~MT_BoolTableClass(void);

};

/* ------------------------------------------------------------------------ */
/*               BUILDING INTERFACE FOR BOOLEAN (BIT) ATTRIBUTES            */
/* ------------------------------------------------------------------------ */

typedef class MT_BoolBuildingInterfaceClass * MT_BoolBuildingInterface;

class MT_BoolBuildingInterfaceClass : public MT_AttrBuildingInterfaceClass
{
  private:
  
  /*
  These point to the same object as my_attr and target_attr of 
  superclass MT_AttrBuildingInterfaceClass.
  */
  MT_BoolTable spec_attr;
  MT_BoolTable spec_target_attr;

  public:
  
  /*
  Record the attribute for a certain MT element (tile or vertex).
  Parameter i must be the index of a tile already made (i.e., for which 
  MT_MakeTile has been called), or the index of a vertex already used (i.e., 
  for which MT_UseVertex has been called), respectively.
  */
  int MT_MakeBoolAttr(MT_INDEX i, int b);

  /*
  Set the attribute table to be built.
  */
  inline void MT_SetTargetAttrTable(MT_BoolTable at)
  {  target_attr = spec_target_attr = at;  }

  /*
  Return the attribute table. To be called after having built it.
  */
  inline MT_BoolTable MT_TheAttrTable(void)
  {  return (spec_target_attr);  }

  /*
  Creator and destructor.
  */
  MT_BoolBuildingInterfaceClass( void );
  ~MT_BoolBuildingInterfaceClass( void );
};

/* ------------------------------------------------------------------------ */

#endif /* MT_BOOL_ATTR_INCLUDED */
