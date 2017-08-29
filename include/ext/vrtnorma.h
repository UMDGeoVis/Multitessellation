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

/* ----------------------------- vrtnorma.h ------------------------------- */

#ifndef VERTEX_NORMAL_INCLUDED
#define VERTEX_NORMAL_INCLUDED

#include "mt_multi.h"
#include "mt_attr.h"
#include "mt_float.h"

/* ------------------------------------------------------------------------ */
/*                    Adding surface normals to an MT                       */
/* ------------------------------------------------------------------------ */

/*
Bare interface containing the additional features of an MT with
surface normals associated with its vertices.
*/

typedef class WithVertexNormalClass * WithVertexNormal;

class WithVertexNormalClass 
{
  public:
  virtual void VertexNormal(MT_INDEX v, float *x, float *y, float *z) = 0;
  virtual float VertexNX(MT_INDEX v) = 0;
  virtual float VertexNY(MT_INDEX v) = 0;
  virtual float VertexNZ(MT_INDEX v) = 0;
};

/* ------------------------------------------------------------------------ */

/*
Implementation of the bare interface storing attributes in memory.
*/

typedef class WithVertexNormalTableClass * WithVertexNormalTable;

class WithVertexNormalTableClass : public WithVertexNormalClass
{
  protected:
  
  /*
  Auxiliary attribute table used to store vertex normals.
  */
  MT_FloatVectorTable my_norm;

  public:
  
  /*
  Creator and destructor.
  */
  WithVertexNormalTableClass(void);
  ~WithVertexNormalTableClass(void);
 
  /*
  Functions redefined from superclass WithVertexNormalClass.
  */
  void VertexNormal(MT_INDEX v, float *x, float *y, float *z);
  
  inline float VertexNX(MT_INDEX v)
  {  return my_norm->MT_TheVectorEntry(v,0);  }
  
  inline float VertexNY(MT_INDEX v) 
  {  return my_norm->MT_TheVectorEntry(v,1);  }
  
  inline float VertexNZ(MT_INDEX v) 
  {  return my_norm->MT_TheVectorEntry(v,2);  }

  /*
  Read / write the vertex normals.
  */
  int ReadVertexNormals(FILE * fd);
  void WriteVertexNormals(FILE * fd, int file_encoding = MT_ASCII_ENCODING);
  int ReadVertexNormals(char * fname);
  int WriteVertexNormals(char * fname, int file_encoding = MT_ASCII_ENCODING);

  /*
  Set and return the textual description used in the vertex
  normal file. String s is at most MT_DESCR_LEN characters.
  */
  void SetVertexNormalDescription(char * s);
  void TheVertexNormalDescription(char * s);

  protected:
  
  inline MT_FloatVectorTable TheVertexNormalTable(void)
  {  return my_norm;  }

  friend class VertexNormalBuildingInterfaceClass;
};

/* ------------------------------------------------------------------------ */

/*
Class for tracing the history of an MT with normals associated
with its vertices, and for converting such history into 
an MT with normals associated with its vertices.
*/

typedef class VertexNormalBuildingInterfaceClass
        * VertexNormalBuildingInterface;

class VertexNormalBuildingInterfaceClass : 
      public virtual MT_BuildingBaseClass
{
  protected:
  
  MT_FloatVectorBuildingInterface norm_build;
  
  public:

  /*
  Constructor and destructor.
  */
  VertexNormalBuildingInterfaceClass(void);
  ~VertexNormalBuildingInterfaceClass(void);

  /*
  Record the normal for vertex v.
  */
  void MakeVertexNormal(MT_INDEX v, float * norm);
  
  /*
  Start and end tracing vertex normals.
  */
  void StartVertexNormalHistory(void);
  void EndVertexNormalHistory(void);

  /*
  Build a vertex normal table from the traced vertex normals.
  */
  int ConvertVertexNormals(void);

  /*
  Write the traced vertex normals into a file.
  */
  void WriteVertexNormals(FILE * fd, int file_encoding);
  int WriteVertexNormals(char * fname, int file_encoding);
  
  /*
  Read previously traced vertex normals from a file and 
  build a vertex normal table from them.
  */
  void ReadVertexNormals(FILE * fd);
  int ReadVertexNormals(char * fd);

  /*
  Set the vertex normal table to be built.
  */
  inline void SetTargetVertexNormalTable(WithVertexNormalTable m)
  {  norm_build->MT_SetTargetAttrTable(m->TheVertexNormalTable());  }

  /*
  Set and return the textual description used in the vertex
  normal file. String s is at most MT_DESCR_LEN characters.
  */
  void SetVertexNormalDescription(char * s);
  void TheVertexNormalDescription(char * s);

};

/* ------------------------------------------------------------------------ */

#endif /* VERTEX_NORMAL_INCLUDED */
