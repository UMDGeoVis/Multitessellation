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

/* ------------------------------ mt_float.h ------------------------------ */

#ifndef MT_FLOAT_ATTR_INCLUDED
#define MT_FLOAT_ATTR_INCLUDED

#include <limits.h>
#include <values.h>

#include "mt_attr.h"
#include "mt_abld.h"

/* ------------------------------------------------------------------------ */
/*                  FLOATS AND FLOAT VECTORS AS MT ATTRIBUTES               */
/* ------------------------------------------------------------------------ */

/*
This file defines classes to:
- implement float attributes inside an MT (class MT_FloatTableClass)
- record float attributes and build an attribute table from them
  (class MT_FloatBuildingInterfaceClass)
- implement float vector attributes inside an MT 
  (class MT_FloatVectorTableClass)
- record float vector attributes and build an attribute table from them
  (class MT_FloatVectorBuildingInterfaceClass)
  
Float attributes are useful, for instance, to implement approximation 
errors of tiles, and field values associated with vertices of an MT.
Float vector attributes are useful, for instance, to implement vertex 
normals in an MT.
*/

/* ------------------------------------------------------------------------ */
/*                          float ATTRIBUTE CLASS                             */
/* ------------------------------------------------------------------------ */

typedef class MT_FloatTableClass * MT_FloatTable;

class MT_FloatTableClass : public MT_AttrTableClass
{

  private:

  /* 
  Array of attributes. It will have as many elements as the number 
  of entities (vertices, tiles, nodes, or arcs) of the MT it refers to.
  */
  float * att_Arr; 

  /*
  Store the minimum and the maximum of the values contained in the array.
  */
  float bound[2];

  public:

  inline char * MT_ClassKeyword()  {  return (char *)"MT_FloatTable";  }

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
  inline void MT_SetAttrValue(MT_INDEX i, float v)  {  att_Arr[i] = v;  }
  
  /*
  Get the attribute value for element of index i.
  */
  inline float MT_AttrValue(MT_INDEX i)  {  return att_Arr[i];  }

  /*
  Get the minimum and maximum value of the attribute.
  */
  inline float MT_MinValue(void)  {  return bound[0];  }
  inline float MT_MaxValue(void)  {  return bound[1];  }

  /*
  Creator and destructor.
  */
  MT_FloatTableClass(void);
  ~MT_FloatTableClass(void);

};

/* ------------------------------------------------------------------------ */
/*                 BUILDING INTERFACE FOR float ATTRIBUTES                    */
/* ------------------------------------------------------------------------ */

typedef class MT_FloatBuildingInterfaceClass * MT_FloatBuildingInterface;

class MT_FloatBuildingInterfaceClass : public MT_AttrBuildingInterfaceClass
{
  private:
  
  /*
  These point to the same objects as my_attr and target_attr of 
  superclass MT_AttrBuildingInterfaceClass.
  */
  MT_FloatTableClass * spec_attr;
  MT_FloatTableClass * spec_target_attr;

  public:
  
  /*
  Record the attribute for a certain MT element (tile or vertex).
  Parameter i must be the index of a tile already made (i.e., for which 
  MT_MakeTile has been called), or the index of a vertex already used (i.e., 
  for which MT_UseVertex has been called), respectively.
  */
  int MT_MakeAttrValue(MT_INDEX i, float v);

  /*
  Set the float attribute table to be built.
  */
  inline void MT_SetTargetAttrTable(MT_FloatTableClass * at)
  {  target_attr = spec_target_attr = at;  }

  /*
  Return the float attribute table. To be called after having built it.
  */
  inline MT_FloatTableClass * MT_TheAttrTable(void)
  {  return (spec_target_attr);  }

  /*
  Creator and destructor.
  */
  MT_FloatBuildingInterfaceClass( void );
  ~MT_FloatBuildingInterfaceClass( void );

};

/* ------------------------------------------------------------------------ */
/*                     float VECTOR ATTRIBUTE CLASS                         */
/* ------------------------------------------------------------------------ */

typedef class MT_FloatVectorTableClass * MT_FloatVectorTable;

class MT_FloatVectorTableClass : public MT_AttrTableClass
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
  float * av_Arr; 
    
  public:

  inline char * MT_ClassKeyword()  {  return (char *)"MT_FloatVectorTable";  }

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
  void MT_SetAttrVector(MT_INDEX i, float *v);
  inline void MT_SetAttrVectorEntry(MT_INDEX i, int j, float v)
  {  av_Arr[j+i*vector_size] = v;  }

  /*
  Get the whole vector, or an element of it, for entity of index i.
  The array must not be allocated nor freed by the caller.
  */
  inline float * MT_TheVector(MT_INDEX i)  {  return &av_Arr[i];  }
  inline float MT_TheVectorEntry(MT_INDEX i, int j)
  {  return av_Arr[j+i*vector_size];  }
  /**** ci andrebbe anche versione che fa la copia del pezzo di array */
  
  /*
  Creator and destructor.
  Parameter num is the number of elements in each vector.
  */
  MT_FloatVectorTableClass(int num);
  ~MT_FloatVectorTableClass(void);

};

/* ------------------------------------------------------------------------ */
/*               BUILDING INTERFACE FOR float VECTOR ATTRIBUTES               */
/* ------------------------------------------------------------------------ */

typedef class MT_FloatVectorBuildingInterfaceClass * MT_FloatVectorBuildingInterface;    

class MT_FloatVectorBuildingInterfaceClass : public MT_AttrBuildingInterfaceClass
{
  private:
  
  /*
  These point to the same objects as my_attr and target_attr of
  superclass MT_AttrBuildingInterfaceClass.
  */
  MT_FloatVectorTableClass * spec_attr;
  MT_FloatVectorTableClass * spec_target_attr;

  public:
  
  /*
  Record the vector for a certain MT element (tile or vertex).
  Parameter i must be the index of a tile already made (i.e., for which
  MT_MakeTile has been called), or the index of a vertex already used (i.e.,
  for which MT_UseVertex has been called), respectively.
  Return 1 on success, 0 on failure.
  */
  int MT_MakeAttrVector(MT_INDEX i, float *v);

  /*
  Set the MT_floatVEC attribute table to be built.
  */
  inline void MT_SetTargetAttrTable(MT_FloatVectorTableClass * at)
  {  target_attr = spec_target_attr = at;  }

  /*
  Return the floatVEC attribute table. To be called after having built it.
  */
  inline MT_FloatVectorTableClass * MT_TheAttrTable(void)
  {  return (spec_target_attr);  }

  /*
  Create an attribute building interface for attributes consisting of vectors 
  of num elements. Destroy a building interface. 
  */
  MT_FloatVectorBuildingInterfaceClass( int num );
  ~MT_FloatVectorBuildingInterfaceClass( void );

};

/* ------------------------------------------------------------------------ */

#endif /* MT_FLOAT_ATTR_INCLUDED */

