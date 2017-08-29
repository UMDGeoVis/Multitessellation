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

/* ---------------------------- mt_abld.h --------------------------------- */


#ifndef MT_ATTR_BUILD_INCLUDED
#define MT_ATTR_BUILD_INCLUDED

#include "mt_attr.h"
#include "mt_build.h"

/* ------------------------------------------------------------------------ */
/*                    A CLASS FOR STORING MT ATTRIBUTES                     */
/* ------------------------------------------------------------------------ */

/*
This file defines an abstract class, called MT_AttrBuildingInterfaceClass.
The interface of such class provides functions to
record the attributes of tiles or vertices during an iterative process of
refinement or simplification of a tesselation,
and to add them to an MT which is being built. 
*/

/* ------------------------------------------------------------------------ */
/*                                  MACROS                                  */
/* ------------------------------------------------------------------------ */

/* 
Filename extension for files containind histories of MT attributes
(Multi-Attribute-History).
*/
#define MT_ATTR_HISTORY_EXT ".mah"

/* ------------------------------------------------------------------------ */
/*                    MT ATTRIBUTE BUILDING INTERFACE                       */
/* ------------------------------------------------------------------------ */

typedef class MT_AttrBuildingInterfaceClass * MT_AttrBuildingInterface;

class MT_AttrBuildingInterfaceClass : public MT_PersistentClass
{

/* ------------------------------------------------------------------------ */
/*                         INTERNAL STRUCTURE                               */
/* ------------------------------------------------------------------------ */

  protected:

  /*
  Internal state of the MT attribute building interface.
  */
  int my_state;

/* ------------------------------------------------------------------------ */
/*                         Attribute tracing                                */
/* ------------------------------------------------------------------------ */

  protected:

  /* 
  Auxiliary file and its name.
  */
  FILE * aux_fd;
  char aux_name[MT_MAX_STRLEN];

  /*
  Number of attributes recorded by now.
  At the end, it should be equal to the number of elements that get 
  attributes (e.g., equal to the number of tiles if attributes are for 
  tiles. However, the same MT entity (e.g., the same tile) is allowed to 
  have its attribute recorded more than once in the history: simply, the last
  value over-writes the previous ones.
  If, for some MT entity, the attribute is recorded twice, then such entity
  is counted twice in counter my_num.
  */
  MT_INDEX my_num;

  /*
  Attribute table used for input/output, it contains just one element, i.e., 
  the last element read.
  It is used here to abstract from the specific attribute type.
  */
  MT_AttrTable my_attr;

/* ------------------------------------------------------------------------ */
/*                        Attribute conversion                              */
/* ------------------------------------------------------------------------ */

  protected:
  
  /*
  Maximum index of an entity getting some attribute.
  */
  MT_INDEX max_num;

  /*
  Association table used for renumbering tiles, used when attributes are
  for tiles. It comes from the MT builder.
  */
  MT_IndexToIndex table3;

  /*
  Attribute table to be built.
  */
  MT_AttrTable target_attr;
  
/* ------------------------------------------------------------------------ */
/*                           Auxiliary functions                            */
/* ------------------------------------------------------------------------ */
 
  protected:

  /*
  This constructor is used just in subclasses.
  */
  MT_AttrBuildingInterfaceClass ( void );

  /*
  Auxiliary function for initialization used inside the constructor.
  The parameter is the attribute table to be used for my_attr.
  It must be of the correct subclass corresponding to the type of
  attributes that we want to trace and build.
  */
  virtual void MT_Init(MT_AttrTableClass * the_attr);

/* ------------------------------------------------------------------------ */
/*             Functions for tracing and converting attributes              */
/* ------------------------------------------------------------------------ */

  public:

  /*
  Start the attribute history.
  */
  virtual int MT_StartAttrHistory(void);

  /*
  End the attribute history. 
  */
  virtual int MT_EndAttrHistory(void);

  /*
  Set the association table containing the tile remumbering.
  If attributes are for tiles, this function must be called with the same 
  table produced by the MT building interface at the end of MT conversion.
  It must be called before MT_Read or MT_AttrConvert.
  If attributes are for vertices, simply call with a NULL association table,
  or do not call it at all.
  */
  inline void MT_SetTileRenumbering(MT_IndexToIndex tr)  {  table3 = tr;  }

  /*
  Set the attribute table to be built.
  */
  inline void MT_SetTargetAttrTable(MT_AttrTable at)
  {  target_attr = at;  }

  /*
  Build the target attribute table from the attributes traced just now.
  If the attributes are for tiles, table3 is used to skip attributes of 
  temporary tiles, and to renumber the ones of non-temporary tiles.
  */
  virtual int MT_AttrConvert(void);

/* ------------------------------------------------------------------------ */
/*                         Read/write functions                             */
/* ------------------------------------------------------------------------ */

  public:

  inline char * MT_ClassKeyword(void)
  {  return (char *)"MT_AttrHistory";  }

  /*
  Functions redefined from superclass MT_PersistentClass.
  */
  virtual int MT_ReadObjParams(FILE * fd);
  virtual int MT_ReadBody(FILE * fd, int file_encoding);
  virtual int MT_Read(FILE * fd);
  virtual int MT_Read(char * fname);
  
  virtual void MT_WriteObjParams(FILE * fd);
  virtual void MT_WriteBody(FILE * fd, int file_encoding);
  virtual void MT_Write(FILE * fd, int file_encoding = MT_ASCII_ENCODING);
  virtual int MT_Write(char * fname, int file_encoding = MT_ASCII_ENCODING);

  /*
  The inherited functions behave as follows:
  MT_Write: write the attribute history traced just now on a file
            (if not yet converted into an attribute table).
  MT_Read: read an attribute history from a file and builds the target
           attribute table.
  */

};

/* ------------------------------------------------------------------------ */

#endif /* MT_ATTR_BUILD_INCLUDED */
