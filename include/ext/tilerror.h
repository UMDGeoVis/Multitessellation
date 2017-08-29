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

/* ----------------------------- tilerror.h ------------------------------- */

#ifndef TILE_ERR_INCLUDED
#define TILE_ERR_INCLUDED

#include "mt_multi.h"
#include "mt_attr.h"
#include "mt_float.h"

/* ------------------------------------------------------------------------ */
/*                    Adding tile errors to an MT                           */
/* ------------------------------------------------------------------------ */

/*
Bare interface containing the additional features of an
MT with error values associated with its tiles.
*/

typedef class WithTileErrorClass * WithTileError;

class WithTileErrorClass 
{
  public:

  /*
  Return the error associated with a tile t.
  */
  virtual float TileError(MT_INDEX t) = 0;

  /*
  Return the maximum and minimum error over all tiles.
  */
  virtual float MaxError(void) = 0;
  virtual float MinError(void) = 0;
};

/* ------------------------------------------------------------------------ */

/*
Implementation of the bare interface storing attributes in memory.
*/
class WithTileErrorTableClass : public WithTileErrorClass
{
  protected:
  
  /*
  Auxiliary attribute table used to store tile errors.
  */
  MT_FloatTable my_error;
  
  public:
  
  /*
  Creator and destructor.
  */
  WithTileErrorTableClass(void);
  ~WithTileErrorTableClass(void);

  /*
  Functions redefined from superclass WithTileErrorClass.
  */
  inline float TileError(MT_INDEX t)
  {  return my_error->MT_AttrValue(t);  }
  inline float MaxError(void)  {  return my_error->MT_MaxValue();  }
  inline float MinError(void)  {  return my_error->MT_MinValue();  }

  /*
  Read / write the tile errors.
  */
  int ReadTileErrors(FILE * fd);
  void WriteTileErrors(FILE * fd, int file_encoding = MT_ASCII_ENCODING);
  int ReadTileErrors(char * fname);
  int WriteTileErrors(char * fname, int file_encoding = MT_ASCII_ENCODING);

  /*
  Set and return the textual description used in the tile error file.
  String s is at most MT_DESCR_LEN characters.
  */
  void SetTileErrorDescription(char * s);
  void TheTileErrorDescription(char * s);

  protected:
  
  inline MT_FloatTable TheTileErrorTable(void)  {  return my_error;  }
  
  friend class TileErrorBuildingInterfaceClass;
};

/* ------------------------------------------------------------------------ */

/*
Class for tracing the history of an MT with error values associated
with its tiles, and for converting such history into an MT with error 
values associated with its tiles.
*/

typedef class TileErrorBuildingInterfaceClass * TileErrorBuildingInterface;

class TileErrorBuildingInterfaceClass : public virtual MT_BuildingBaseClass
{
  protected:
  
  /*
  Auxiliary building interface for float attributes.
  */
  MT_FloatBuildingInterface error_build;
  
  public:

  /*
  Constructor and destructor.
  */
  TileErrorBuildingInterfaceClass(void);
  ~TileErrorBuildingInterfaceClass(void);
 
  /*
  Record the error value for tile t.
  */
  void MakeTileError(MT_INDEX t, float err);
  
  /*
  Start and end the tracing of tile errors.
  */
  void StartTileErrorHistory(void);
  void EndTileErrorHistory(void);

  /*
  Build a tile error table from the traced errors.
  */
  int ConvertTileErrors(void);

  /*
  Write the traced tile errors.
  */
  void WriteTileErrors(FILE * fd, int file_encoding = MT_ASCII_ENCODING);
  int WriteTileErrors(char * fname, int file_encoding = MT_ASCII_ENCODING);

  /*
  Read previously traced tile errors and convert them into 
  a tile error table.
  */
  void ReadTileErrors(FILE * fd);
  int ReadTileErrors(char * fname);

  /*
  Set the tile error table to be built.
  */
  inline void SetTargetTileErrorTable(WithTileErrorTableClass * m)
  {  error_build->MT_SetTargetAttrTable(m->TheTileErrorTable());  }

  /*
  Set and return the textual description used in the tile error file.
  String s is at most MT_DESCR_LEN characters.
  */
  void SetTileErrorDescription(char * s);
  void TheTileErrorDescription(char * s);

};

/* ------------------------------------------------------------------------ */

#endif /* TILE_ERR_INCLUDED */
