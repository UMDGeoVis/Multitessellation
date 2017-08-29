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

/* ---------------------------- mt_build.h -------------------------------- */

#ifndef MT_BUILD_INCLUDED
#define MT_BUILD_INCLUDED

#include "mt_multi.h" /* for the Multi-Triangulation */
#include "mt_ass.h"   /* for association tables */

/* ------------------------------------------------------------------------ */
/*                           MT BUILDING INTERFACE                          */
/* ------------------------------------------------------------------------ */

/*
This file defines a class providing functions to trace the history of 
an iterative process of refinement or simplification of a tesselation,
and to build an MT from such history.
There are two options:
- perform History tracing and MT construction within the same program
- write the history on a file; another program will read the file
  and build the MT.
*/

/* ------------------------------------------------------------------------ */
/*                                  MACROS                                  */
/* ------------------------------------------------------------------------ */

/*
Types of history.
*/
#define MT_REFINING 0
#define MT_COARSENING 1

/*
Keyword telling the type of history .
*/
#define MT_REFINE_KW "MT-REFINE"
#define MT_COARSEN_KW "MT-COARSEN"

/*
Filename extension for files containing MT histories
(Multi-Tesselation-History).
*/
#define MT_HISTORY_EXT ".mth"

/*
Indexes for the array of auxiliary files.
*/
#define NUM_AUX_HISTORY_FILES 4
#define VERT_AUX_HISTORY_FILE 0  /* vertices */
#define TILE_AUX_HISTORY_FILE 1  /* tiles */
#define UPDT_AUX_HISTORY_FILE 2  /* updates */
#define THIS_AUX_HISTORY_FILE 3  /* tiles of the current update */

/*
Possible internal states for the MT building interface.
*/
#define MT_INITIAL_STATE    0
#define MT_TRACING_STATE    1
#define MT_TRACED_STATE     2
#define MT_FINAL_STATE      3

/* ------------------------------------------------------------------------ */
/*                      AUXILIARY DATA STRUCTURES                           */
/* ------------------------------------------------------------------------ */

/*
This list type is used to collect tiles created and removed in an update.
*/
typedef struct IndexListStruct
{
  MT_INDEX elem;
  struct IndexListStruct * next;
} *  IndexList;

/* ------------------------------------------------------------------------ */
/*                               BASE CLASS                                 */
/* ------------------------------------------------------------------------ */

/*
This is a class defined in mt_abld.h, it is needed by the base class.
*/
typedef class MT_AttrBuildingInterfaceClass * MT_AttrBuildingInterface;

/*
This part of an MT building interface must be seen also by the attribute 
building interface. 
This it has been placed in a common virtual superclass of both.
*/

class MT_BuildingBaseClass
{
  protected:
  
  /*
  Association table containing associations tile --> tile, used during 
  history conversion for renumbering tiles in such a way that tiles 
  labelling the same arc have consecutive indexes.
  */
  MT_IndexToIndex table3;

  /*
  Connect an attribute building interface for building vertex or tile
  attributes. If for tiles, pass the association table to an attribute
  building interface.
  */
  void ConnectVertexAttrBuilder(MT_AttrBuildingInterface abi);
  void ConnectTileAttrBuilder(MT_AttrBuildingInterface abi);

  /*
  Constructor.
  */
  MT_BuildingBaseClass(void);

};

/* ------------------------------------------------------------------------ */
/*                         MT BUILDING INTERFACE                            */
/* ------------------------------------------------------------------------ */

typedef class MT_BuildingInterfaceClass * MT_BuildingInterface;

class MT_BuildingInterfaceClass : public MT_PersistentClass,
                                  public virtual MT_BuildingBaseClass
{

/* ------------------------------------------------------------------------ */
/*                         INTERNAL STRUCTURE                               */
/* ------------------------------------------------------------------------ */

  protected:

  /*
  Type of history (refinement or simplification).
  */
  int my_type;

  /*
  Internal state of the MT building interface.
  */
  int my_state;
  
/* ------------------------------------------------------------------------ */
/*                            History tracing                               */
/* ------------------------------------------------------------------------ */

  /* 
  Auxiliary files and auxiliary file names.
  */
  FILE * aux_fd[NUM_AUX_HISTORY_FILES]; 
  char aux_name[NUM_AUX_HISTORY_FILES][MT_MAX_STRLEN];

  /*
  Auxiliary tile set used to store the last recorded vertex and the last
  recorded tile.
  */
  MT_TileSet my_tileset;
  
  /*
  Update counter, equal to the number assigned to the last recorded update
  (0 at the beginning, then equal to the number of the current update -1).
  */
  MT_INDEX U_Num;

  /*
  Vertex counter, equal to the number assigned to the last recorded vertex 
  (0 at the beginning).
  */
  MT_INDEX V_Num;
  
  /*
  Tile counter, equal to the number assigned to the last recorded tile 
  (0 at the beginning).
  The counter includes temporary tiles (i.e., tiles created and removed in 
  the same update).
  Thus, the set of indexes of tiles actually used in the updates are not 
  necessarily consecutive. However during MT construction
  this set will be compacted into a consecutive set of indexes. 
  */
  MT_INDEX T_Num;  
  
  /*
  Array, and counter of the number of vertices already specified for the 
  next tile. The counter must be equal to my_tileset->T_Dim when the tile 
  is made with MT_MakeTile.
  */
  MT_INDEX my_vert[MT_MAX_DIM];
  int my_ind;

  /*
  The following variables are needed to detect and prune "temporary" tiles,
  i.e., tiles that are temporarily created and then removed within the same
  update.
  */
  
  /*
  Index of the first tile created in the current update.
  Used to detect temporary tiles of the current update.
  */
  MT_INDEX first_tri;

  /*
  Counter of temporary tiles created during the whole process.
  */
  int temp_Num;

  /*
  List of tiles removed in the current update, and number of its elements.
  */
  IndexList old_list; 
  int old_Num;

  /*
  List of tiles created in the current update, pointer to its last element,
  and number of its elements.
  New elements are inserted at the end, thus obtaining a list
  sorted by increasing index.
  */
  IndexList new_list, new_last;
  int new_Num;
    
  /*
  Lists old_list, new_list do not contain temporary tiles. A tile is removed
  from new_list as soon as it is discoveder to be temporary, and it is not
  inserted in old_list.
  */

/* ------------------------------------------------------------------------ */
/*                          History conversion                              */  
/* ------------------------------------------------------------------------ */

  /*
  The Multi-Tesselation to be built.
  */
  MT_MultiTesselation my_mt;

  /* 
  Association table containing associations tile --> node (node that 
  created the tile in case of refinement, node that removed the tile 
  in case of coarsening history).
  */
  MT_IndexToIndex table1;

  /*
  Association table containing associations node --> (node --> tile),
  with the meaning arc_source --> (arc_destination --> label_tile).
  Note that many tiles may be associated with the same pair of nodes.
  */
  MT_IndexToIndexPair table2;
    
  /*
  Association table table3, inherited by superclass MT_BuildingBaseClass,
  contains associations tile --> tile, used for
  renumbering tiles in such a way that tiles labelling the same arc have
  consecutive indexes.
  */

  /*
  The dimension of the three association tables is equal, and it is
  determined based on the number of tiles in the MT.
  The number of entries in table2, with distinct pairs of primary and
  secondary keys, will give the number of arcs of the MT.
  */
  
/* ------------------------------------------------------------------------ */
/*                           Auxiliary functions                            */
/* ------------------------------------------------------------------------ */

  protected:

  /*
  Set the dimensions and allocate the three association tables.
  Used for history conversion. Return 1 on success, 0 on failure.
  */
  int InitTables(int s);

/* ------------------------------------------------------------------------ */
/*                     Functions for tracing a history                      */
/* ------------------------------------------------------------------------ */

  public:

  /*
  Set the history type and start the history.
  The parameter is either MT_REFINING or MT_COARSENING.
  Return 1 on success, 0 on failure.
  */
  int MT_StartHistory(int vert_dim, int tile_dim, int history_type);

  /*
  Record the use of a vertex within a new tile to be created.
  Called by passing the vertex index.
  Return a new value to be assigned to the vertex index.
  */
  MT_INDEX MT_UseVertex(MT_INDEX i, float * coord_array);
	  
  /*
  Record the creation of a new tile (formed by the vertices on which
  MT_UseVertex has just been called).
  Return a new value to be assigned to the tile index.
  */
  MT_INDEX MT_MakeTile(void);

  /*
  Record the deletion of a tile. Called by passing the tile index.
  Return 1 on success, 0 on failure.
  */
  int MT_KillTile(MT_INDEX i);
       
  /*
  Record the fact that the update is completed.
  Called just after the creation of the initial tesselation, and after
  each update. Return 1 on success, 0 on failure.
  */
  int MT_EndUpdate(void);
       
  /*
  End the history. Return 1 on success, 0 on failure.
  */
  int MT_EndHistory(void);

/* ------------------------------------------------------------------------ */
/*                     Functions for converting a history                   */
/* ------------------------------------------------------------------------ */

  /*
  Set the MT to be built. It must be called before MT_Convert or MT_Read.
  */
  inline void MT_SetTarget(MT_MultiTesselation m)   {  my_mt = m;  }

  /*
  Build the target MT from the history traced just now.
  First an MT must be set with MT_Target. Return 1 on success, 0 on failure.
  */
  int MT_Convert(void);

/* ------------------------------------------------------------------------ */
/*                           Read/write functions                           */
/* ------------------------------------------------------------------------ */

  public:

  inline char * MT_ClassKeyword(void)
  {  return (char *)"MT_History";  }

  /*
  Functions redefined from superclass MT_PersistentClass.
  */
  int MT_ReadObjParams(FILE * fd);
  int MT_ReadBody(FILE * fd, int file_encoding);
  int MT_Read(FILE * fd);
  int MT_Read(char * fname);
    
  void MT_WriteObjParams(FILE * fd);
  void MT_WriteBody(FILE * fd, int file_encoding);
  void MT_Write(FILE * fd, int file_encoding = MT_ASCII_ENCODING);
  int MT_Write(char * fname, int file_encoding = MT_ASCII_ENCODING);

  /*
  The inherited functions behave as follows:
  MT_Write: write the MT history traced just now on a file
            (if not yet converted into an MT).
  MT_Read: read a history from a file and builds the target MT.
  */

/* ------------------------------------------------------------------------ */
/*                          Creation and deletion                           */
/* ------------------------------------------------------------------------ */

  public: 

  MT_BuildingInterfaceClass(void);
  ~MT_BuildingInterfaceClass(void);

};

/* ------------------------------------------------------------------------ */

#endif /* MT_BUILD_INCLUDED */
