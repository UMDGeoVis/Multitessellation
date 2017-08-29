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

/* ---------------------------- mt_attr.h --------------------------------- */


#ifndef MT_ATTR_INCLUDED
#define MT_ATTR_INCLUDED

#include "mt_prst.h"

/* ------------------------------------------------------------------------ */
/*                    A CLASS FOR STORING MT ATTRIBUTES                     */
/* ------------------------------------------------------------------------ */

/*
This file defines the interface of an abstract class storing attributes 
for the entities (vertices, tiles) of an MT, called MT_AttrTableClass.
Subclasses of this class will represent specific types of attributes.
The interface contains functions to read and write the value of the 
attribute for an entity, and to get and set such value.
*/

/* ------------------------------------------------------------------------ */
/*                                  MACROS                                  */
/* ------------------------------------------------------------------------ */

/*
Filename extension for files containing MT attributes (Multi-Attribute-File).
*/
#define MT_MULTI_ATTR_EXT ".maf"

/* ------------------------------------------------------------------------ */
/*                         THE MT ATTRIBUTE CLASS                           */
/* ------------------------------------------------------------------------ */

typedef class MT_AttrTableClass * MT_AttrTable;

class MT_AttrTableClass : public MT_PersistentClass
{

/* ------------------------------------------------------------------------ */
/*                            Internal structure                            */
/* ------------------------------------------------------------------------ */

  protected:

  /*
  Number of attributes stored. 
  This will be the same as the number of vertices or tiles in an MT.
  In the subclasses, attribute values will be stored in arrays. 
  The arrays will be aligned with the corresponding vertex or tile
  array in the MT.
  As in the MT, valid indexes start from 1, thus the number of array
  entries will be my_num+1.
  */
  MT_INDEX my_num;
    
/* ------------------------------------------------------------------------ */
/*                            Access primitives                             */
/* ------------------------------------------------------------------------ */

  public:

  /*
  Return the number of attributes stored.
  */
  inline MT_INDEX MT_AttrNum(void)  {  return my_num;  }

  /*
  Set the dimension of the attribute array, and allocate the array.
  */
  virtual int MT_SetAttrNum(MT_INDEX n) = 0;

  /*
  Read the attribute for the MT entity of index i. 
  Return 1 on success, 0 on failure.
  */
  virtual int MT_ReadAttr(FILE * fd, MT_INDEX i, int file_encoding) = 0;

  /*
  Read the attributes for all MT entities.
  Function redefined from superclass MT_PersistentClass.
  */
  virtual int MT_ReadBody(FILE * fd, int file_encoding);

  /*
  Write the attribute of MT the entity of index i.
  */
  virtual void MT_WriteAttr(FILE * fd, MT_INDEX i, int file_encoding) = 0;

  /*
  Write the attributes for all MT entities.
  Function redefined from superclass MT_PersistentClass.
  */
  virtual void MT_WriteBody(FILE * fd, int file_encoding);

  private:
  
  /*
  Set the dimension of the attribute array, without touching the array.
  Used during construction of the attribute, within class 
  MT_AttrConverterClass.
  */
  inline void MT_WeakSetAttrNum(MT_INDEX n) {  my_num = n;  }

  friend class MT_AttrBuildingInterfaceClass;

};

/* ------------------------------------------------------------------------ */

#endif /* MT_ATTR_INCLUDED */
