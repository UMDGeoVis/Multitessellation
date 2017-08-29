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

/* ------------------------------ mt_int.h ------------------------------ */

#ifndef MT_INT_ATTR_INCLUDED
#define MT_INT_ATTR_INCLUDED

#include <limits.h>
#include <values.h>

#include "mt_attr.h"
#include "mt_abld.h"

/* ------------------------------------------------------------------------ */
/*               INTEGERS AND INTEGER VECTORS AS MT ATTRIBUTES              */
/* ------------------------------------------------------------------------ */

/*
This file defines classes to:
- implement integer attributes inside an MT (class MT_IntTableClass)
- record integer attributes and build an attribute table from them
  (class MT_IntBuildingInterfaceClass)
- implement integer vector attributes inside an MT
  (class MT_InttVectorTableClass)
- record integer vector attributes and build an attribute table from them
  (class MT_IntVectorBuildingInterfaceClass)
*/
      
/* ------------------------------------------------------------------------ */
/*                          int ATTRIBUTE CLASS                             */
/* ------------------------------------------------------------------------ */

typedef class MT_IntTableClass * MT_IntTable;

class MT_IntTableClass : public MT_AttrTableClass
{

  private:

  /* 
  Array of attributes. It will have as many elements as the number 
  of entities (vertices, tiles, nodes, or arcs) of the MT it refers to.
  */
  int * att_Arr; 

  /*
  Store the minimum and the maximum of the values contained in the array.
  */
  int bound[2];

  public:

  inline char * MT_ClassKeyword()  {  return (char *)"MT_IntTable";  }

  /* 
  Allocate the attribute array to n positions.
  Return 1 if successful, 0 otherwise.
  */
  int MT_SetAttrNum(MT_INDEX n);

  /*
  Read the attribute for MT entity i. Return 1 on success, 0 on failure.
  */
  int MT_ReadAttr(FILE * fd, MT_INDEX i, int file_encoding);

  /*
  Write the attribute of MT entity i.
  */
  void MT_WriteAttr(FILE * fd, MT_INDEX i, int file_encoding);

  /*
  Functions redefined from superclass MT_PersistentClass.
  */
  int MT_ReadObjParams(FILE * fd);
  int MT_ReadBody(FILE * fd, int file_encoding);
  void MT_WriteObjParams(FILE * fd);

  /*
  Set the value of the attribute for MT entity i. 
  */
  inline void MT_SetAttrValue(MT_INDEX i, int v)  {  att_Arr[i] = v;  }
  
  /*
  Get the attribute value for element of index i.
  */
  inline int MT_AttrValue(MT_INDEX i)  {  return att_Arr[i];  }

  /*
  Get the minimum and maximum value of the attribute.
  */
  inline int MT_MinValue(void)  {  return bound[0];  }
  inline int MT_MaxValue(void)  {  return bound[1];  }

  /*
  Creator and destructor.
  */
  MT_IntTableClass(void);
  ~MT_IntTableClass(void);

};

/* ------------------------------------------------------------------------ */
/*                 BUILDING INTERFACE FOR int ATTRIBUTES                    */
/* ------------------------------------------------------------------------ */

typedef class MT_IntBuildingInterfaceClass * MT_IntBuildingInterface;

class MT_IntBuildingInterfaceClass : public MT_AttrBuildingInterfaceClass
{
  private:
  
  /*
  These point to the same objects as my_attr and target_attr of 
  superclass MT_AttrBuildingInterfaceClass.
  */
  MT_IntTableClass * spec_attr;
  MT_IntTableClass * spec_target_attr;

  public:
  
  /*
  Record the attribute for a certain MT element (tile or vertex).
  Parameter i must be the index of a tile already made (i.e., for which 
  MT_MakeTile has been called), or the index of a vertex already used (i.e., 
  for which MT_UseVertex has been called), respectively.
  */
  int MT_MakeAttrValue(MT_INDEX i, int v);

  /*
  Set the int attribute table to be built.
  */
  inline void MT_SetTargetAttrTable(MT_IntTableClass * at)
  {  target_attr = spec_target_attr = at;  }

  /*
  Return the int attribute table. To be called after having built it.
  */
  inline MT_IntTableClass * MT_TheAttrTable(void)
  {  return (spec_target_attr);  }

  /*
  Creator and destructor.
  */
  MT_IntBuildingInterfaceClass( void );
  ~MT_IntBuildingInterfaceClass( void );

};

/* ------------------------------------------------------------------------ */
/*                     int VECTOR ATTRIBUTE CLASS                           */
/* ------------------------------------------------------------------------ */

typedef class MT_IntVectorTableClass * MT_IntVectorTable;

class MT_IntVectorTableClass : public MT_AttrTableClass
{

  private:

  /*
  Number of elements in each vector.
  */
  int vector_size;
    
  /* 
  Array of attributes, it contains all vectors one after the other.
  It will have as many elements as vector_size times the number of 
  entities (vertices, tiles, nodes, or arcs) of the MT it refers to.
  */
  int * av_Arr; 
    
  public:

  inline char * MT_ClassKeyword()  {  return (char *)"MT_IntVectorTable";  }

  /*
  Read the vector for the MT entity i. Return 1 on success, 0 on failure.
  */
  int MT_ReadAttr(FILE * fd, MT_INDEX i, int file_encoding);

  /*
  Write the vector for the MT entity i.
  */
  void MT_WriteAttr(FILE * fd, MT_INDEX i, int file_encoding);

  /*
  Functions redefined from superclass MT_PersistentClass.
  */
  int MT_ReadClassAndParams(FILE * fd);
  int MT_ReadObjParams(FILE * fd);
  int MT_ReadBody(FILE * fd, int file_encoding);
  void MT_WriteClassAndParams(FILE * fd);
  void MT_WriteObjParams(FILE * fd);
  void MT_WriteBody(FILE * fd, int file_encoding);

  /* 
  Allocate the attribute array to n vectors of vector_size positions each.
  Return 1 on success, 0 on failure.
  */
  int MT_SetAttrNum(MT_INDEX n);

  /*
  Set the vector for MT entity i, copying it from vector v, or set just
  one entry.
  */
  void MT_SetAttrVector(MT_INDEX i, int *v);
  inline void MT_SetAttrVectorEntry(MT_INDEX i, int j, int v)
  {  av_Arr[j+i*vector_size] = v;  }

  /*
  Get the whole vector, or an element of it, for entity of index i.
  The array must not be allocated nor freed by the caller.
  */
  inline int * MT_TheVector(MT_INDEX i)  {  return &av_Arr[i];  }
  inline int MT_TheVectorEntry(MT_INDEX i, int j)
  {  return av_Arr[j+i*vector_size];  }
  /**** ci andrebbe anche versione che fa la copia del pezzo di array */
  
  /*
  Creator and destructor.
  Parameter num is the number of elements in each vector.
  */
  MT_IntVectorTableClass(int num);
  ~MT_IntVectorTableClass(void);

};

/* ------------------------------------------------------------------------ */
/*               BUILDING INTERFACE FOR int VECTOR ATTRIBUTES               */
/* ------------------------------------------------------------------------ */

typedef class MT_IntVectorBuildingInterfaceClass * MT_IntVectorBuildingInterface;    

class MT_IntVectorBuildingInterfaceClass : public MT_AttrBuildingInterfaceClass
{
  private:
  
  /*
  These point to the same objects as my_attr and target_attr of
  superclass MT_AttrBuildingInterfaceClass.
  */
  MT_IntVectorTableClass * spec_attr;
  MT_IntVectorTableClass * spec_target_attr;

  public:
  
  /*
  Record the vector for a certain MT element (tile or vertex).
  Parameter i must be the index of a tile already made (i.e., for which
  MT_MakeTile has been called), or the index of a vertex already used (i.e.,
  for which MT_UseVertex has been called), respectively.
  Return 1 on success, 0 on failure.
  */
  int MT_MakeAttrVector(MT_INDEX i, int *v);

  /*
  Set the MT_intVEC attribute table to be built.
  */
  inline void MT_SetTargetAttrTable(MT_IntVectorTableClass * at)
  {  target_attr = spec_target_attr = at;  }

  /*
  Return the intVEC attribute table. To be called after having built it.
  */
  inline MT_IntVectorTableClass * MT_TheAttrTable(void)
  {  return (spec_target_attr);  }

  /*
  Create an attribute building interface for attributes consisting of vectors 
  of num elements. Destroy a building interface. 
  */
  MT_IntVectorBuildingInterfaceClass( int num );
  ~MT_IntVectorBuildingInterfaceClass( void );

};

/* ------------------------------------------------------------------------ */

#endif /* MT_INT_ATTR_INCLUDED */

