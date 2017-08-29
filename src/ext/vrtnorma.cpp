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

/* ----------------------------- vrtnorma.c ------------------------------- */

#include "vrtnorma.h"

/* ------------------------------------------------------------------------ */
/*                    Adding surface normals to an MT                       */
/* ------------------------------------------------------------------------ */

WithVertexNormalTableClass :: WithVertexNormalTableClass(void)
{  my_norm = new MT_FloatVectorTableClass(3);  }

WithVertexNormalTableClass :: ~WithVertexNormalTableClass(void)
{  if (my_norm) delete my_norm; my_norm = NULL;  }
 
void WithVertexNormalTableClass ::
     VertexNormal(MT_INDEX v, float *x, float *y, float *z)
{
  (*x) = my_norm->MT_TheVectorEntry(v,0);
  (*y) = my_norm->MT_TheVectorEntry(v,1);
  (*z) = my_norm->MT_TheVectorEntry(v,2);
}
  
int WithVertexNormalTableClass :: ReadVertexNormals(FILE * fd)
{  return ( my_norm->MT_Read(fd) );  }

int WithVertexNormalTableClass :: ReadVertexNormals(char * fname)
{  
  FILE * fd = fopen(fname,"r");
  int ok = 0;
  if (fd) ok = ReadVertexNormals(fd);
  fclose(fd);
  return ok;
}

void WithVertexNormalTableClass :: 
     WriteVertexNormals(FILE * fd, int file_encoding)
{  my_norm->MT_Write(fd, file_encoding);  }

int WithVertexNormalTableClass :: 
    WriteVertexNormals(char * fname, int file_encoding)
{
  FILE * fd = fopen(fname,"w");
  int ok = 0;
  if (fd) {  WriteVertexNormals(fd, file_encoding); ok = 1;  }
  fclose(fd);
  return ok;
}

void WithVertexNormalTableClass :: SetVertexNormalDescription(char * s)
{  my_norm->MT_SetDescription(s);  }

void WithVertexNormalTableClass :: TheVertexNormalDescription(char * s)
{  my_norm->MT_TheDescription(s);  }

/* ------------------------------------------------------------------------ */

VertexNormalBuildingInterfaceClass :: 
VertexNormalBuildingInterfaceClass(void) : MT_BuildingBaseClass()
{  norm_build = new MT_FloatVectorBuildingInterfaceClass(3);  }
 
VertexNormalBuildingInterfaceClass :: 
~VertexNormalBuildingInterfaceClass(void)
{  if (norm_build) delete norm_build;
   norm_build = NULL;
}

void VertexNormalBuildingInterfaceClass ::
     MakeVertexNormal(MT_INDEX v, float * norm)
{  norm_build->MT_MakeAttrVector(v,norm);  }

void VertexNormalBuildingInterfaceClass :: StartVertexNormalHistory(void)
{  norm_build->MT_StartAttrHistory();  }

void VertexNormalBuildingInterfaceClass :: EndVertexNormalHistory(void)
{  norm_build->MT_EndAttrHistory();  }

int VertexNormalBuildingInterfaceClass :: ConvertVertexNormals(void) 
{  ConnectVertexAttrBuilder(norm_build);
   return ( norm_build->MT_AttrConvert() );
}

void VertexNormalBuildingInterfaceClass :: 
     WriteVertexNormals(FILE * fd, int file_encoding)
{  norm_build->MT_Write(fd, file_encoding);  }


int VertexNormalBuildingInterfaceClass :: 
    WriteVertexNormals(char * fname, int file_encoding)
{
  FILE * fd = fopen(fname,"w");
  int ok = 0;
  if (fd) {  WriteVertexNormals(fd, file_encoding);  ok = 1;  }
  fclose(fd);
  return ok;
}

void VertexNormalBuildingInterfaceClass :: ReadVertexNormals(FILE * fd)
{  ConnectVertexAttrBuilder(norm_build);
   norm_build->MT_Read(fd);
}

int VertexNormalBuildingInterfaceClass :: ReadVertexNormals(char * fname)
{  
  FILE * fd = fopen(fname,"r");
  int ok = 0;
  if (fd) {  ReadVertexNormals(fd); ok = 1;  }
  fclose(fd);
  return ok;
}


void VertexNormalBuildingInterfaceClass ::
     SetVertexNormalDescription(char * s)
{  norm_build->MT_SetDescription(s);  }

void VertexNormalBuildingInterfaceClass ::
     TheVertexNormalDescription(char * s)
{  norm_build->MT_TheDescription(s);  }

/* ------------------------------------------------------------------------ */
