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

/* ----------------------------- tilerror.c ------------------------------- */

#include "tilerror.h"

/* ------------------------------------------------------------------------ */
/*                    Adding tile errors to an MT                           */
/* ------------------------------------------------------------------------ */

WithTileErrorTableClass :: WithTileErrorTableClass(void)
{  my_error = new MT_FloatTableClass();  }

WithTileErrorTableClass :: ~WithTileErrorTableClass(void)
{  if (my_error) delete my_error; my_error = NULL;  }

int WithTileErrorTableClass :: ReadTileErrors(FILE * fd)
{  return ( my_error->MT_Read(fd) );  }

int WithTileErrorTableClass :: ReadTileErrors(char * fname)
{
  FILE * fd = fopen(fname,"r");
  int ok = 0;
  if (fd) ok = ReadTileErrors(fd);
  fclose(fd);
  return ok;
}

void WithTileErrorTableClass :: WriteTileErrors(FILE * fd, int file_encoding)
{  my_error->MT_Write(fd, file_encoding);  }

int WithTileErrorTableClass ::
    WriteTileErrors(char * fname, int file_encoding)
{
  FILE * fd = fopen(fname,"w");
  int ok = 0;
  if (fd) {  WriteTileErrors(fd, file_encoding); ok = 1;  }
  fclose(fd);
  return ok;
}

void WithTileErrorTableClass :: SetTileErrorDescription(char * s)
{  my_error->MT_SetDescription(s);  }

void WithTileErrorTableClass :: TheTileErrorDescription(char * s)
{  my_error->MT_TheDescription(s);  }

/* ------------------------------------------------------------------------ */

TileErrorBuildingInterfaceClass :: 
TileErrorBuildingInterfaceClass(void) : MT_BuildingBaseClass()
{  error_build = new MT_FloatBuildingInterfaceClass();  }

TileErrorBuildingInterfaceClass ::
~TileErrorBuildingInterfaceClass(void)
{  if (error_build) delete error_build;
   error_build = NULL;
}
 
void TileErrorBuildingInterfaceClass :: MakeTileError(MT_INDEX t, float err)
{  error_build->MT_MakeAttrValue(t,err);  }
  
void TileErrorBuildingInterfaceClass :: StartTileErrorHistory(void)
{  error_build->MT_StartAttrHistory();  }

void TileErrorBuildingInterfaceClass :: EndTileErrorHistory(void)
{  error_build->MT_EndAttrHistory();  }

int TileErrorBuildingInterfaceClass :: ConvertTileErrors(void) 
{  ConnectTileAttrBuilder(error_build);
   return ( error_build->MT_AttrConvert() );
}

void TileErrorBuildingInterfaceClass ::
     WriteTileErrors(FILE * fd, int file_encoding)
{  error_build->MT_Write(fd, file_encoding);  }


int TileErrorBuildingInterfaceClass ::
    WriteTileErrors(char * fname, int file_encoding)
{
  FILE * fd = fopen(fname,"w");
  int ok = 0;
  if (fd) {  WriteTileErrors(fd, file_encoding); ok = 1;  }
  fclose(fd);
  return ok;
}

void TileErrorBuildingInterfaceClass :: ReadTileErrors(FILE * fd)
{  ConnectTileAttrBuilder(error_build);
   error_build->MT_Read(fd);
}

int TileErrorBuildingInterfaceClass :: ReadTileErrors(char * fname)
{
  FILE * fd = fopen(fname,"r");
  int ok = 0;
  if (fd) {  ReadTileErrors(fd); ok = 1;  }
  fclose(fd);
  return ok;
}

void TileErrorBuildingInterfaceClass :: SetTileErrorDescription(char * s)
{  error_build->MT_SetDescription(s);  }

void TileErrorBuildingInterfaceClass :: TheTileErrorDescription(char * s)
{  error_build->MT_TheDescription(s);  }

/* ------------------------------------------------------------------------ */
