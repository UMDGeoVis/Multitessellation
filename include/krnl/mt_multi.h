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

/* ----------------------------- mt_multi.h ------------------------------- */

#ifndef MT_MULTI_INCLUDED
#define MT_MULTI_INCLUDED

#include "mt_tile.h"

/* ------------------------------------------------------------------------ */
/*                 CLASS FOR A GENERIC MULTI-TESSELATION                    */
/* ------------------------------------------------------------------------ */

/*
This file defines a class for the Multi-Tesselation, called
MT_MultiTesselationClass. This class is a subclass of MT_TileSetClass since,
as a tile set, an MT has vertices and tiles. In addition, an MT has nodes 
and arcs.
*/

/* ------------------------------------------------------------------------ */
/*                                  MACROS                                  */
/* ------------------------------------------------------------------------ */

/*
Filename extension for files containing Multi-Tesselations
(Multi-Tesselation-File).
*/
#define MT_MULTI_TESSELATION_EXT ".mtf"

/* ------------------------------------------------------------------------ */
/*              Data structures for the entities of an MT                   */ 
/* ------------------------------------------------------------------------ */

/*
Data structure for a tile-to-arc reference.
*/
typedef struct MT_TileToArcStruct 
{
  MT_INDEX my_arc;      /* index in array A_Arr of the arc whose label
                           contains the tile */
} MT_TileToArc;

/*
Data structure for an arc.
*/
typedef struct MT_ArcStruct 
{
  MT_INDEX my_tile[2];  /* index in array T_Arr of the first and last tile
                           labelling the arc (tiles labelling the same arc are
                           stored at consecutive positions in T_Arr) */
  MT_INDEX my_node[2];  /* indices in array N_Arr of the sorce and destination
                           node of the arc */
  MT_INDEX my_next;     /* index in A_Arr of the next arc having the same
                           destination node */
} MT_Arc;

/*
Data structure for a node.
*/
typedef struct MT_NodeStruct 
{
  MT_INDEX my_out[2]; /* indices in array A_Arr of the first and last arc 
                         outgoing from the node */
  MT_INDEX my_in[2];  /* index in array A_Arr of the first incoming arc,
                         and number of incoming arcs */  
} MT_Node;

/* ------------------------------------------------------------------------ */
/*                      THE MULTI-TESSELATION CLASS                         */
/* ------------------------------------------------------------------------ */

/* 
Every MT entity (vertex, tile, node, arc) is identified by its index 
(corresponding to positions in the internal arrays of the MT).
The first element (position 0) of each array is used to encode the null 
element (value returned by operations when the result is undefined).
Valid indices start from 1.
*/

typedef class MT_MultiTesselationClass * MT_MultiTesselation;

class MT_MultiTesselationClass : public MT_TileSetClass
{

/* ------------------------------------------------------------------------ */ 
/*                         Internal structure                               */
/* ------------------------------------------------------------------------ */

  protected:

  /*
  Array of tile-to-arc references.
  The size of this array is T_Num, inherited from class MT_TileSetClass.
  Arrays T_Arr and TA_Arr are aligned (each tile has the same index in both
  arrays), and tiles labelling the same arc are stored at consecutive 
  positions.
  */
  MT_TileToArc * TA_Arr;

  /*
  Number of arcs, and array of arcs.
  Arcs outgoing from the same node are are stored at consecutive positions.
  */
  MT_INDEX A_Num;
  MT_Arc * A_Arr;

  /*
  Number of nodes, and array of nodes. 
  The root lies at position 1, the drain lies at the last position.
  */
  MT_INDEX N_Num;      
  MT_Node * N_Arr; 

/* ------------------------------------------------------------------------ */
/*                            Class keyword                                 */
/* ------------------------------------------------------------------------ */

  /*
  Class keyword, redefined from superclass MT_TileSetClass.
  */
  inline char * MT_ClassKeyword(void)
  {  return (char *)"MT_MultiTesselation";  }  
  
/* ------------------------------------------------------------------------ */
/*                               Global information                         */
/* ------------------------------------------------------------------------ */

  public:
  
  /* 
  Number of nodes and arcs.
  */
  inline MT_INDEX MT_ArcNum()      { return (A_Num); }
  inline MT_INDEX MT_NodeNum()     { return (N_Num); }

/* ------------------------------------------------------------------------ */
/*                       Information on DAG structure                       */
/* ------------------------------------------------------------------------ */

  /*
  Root and drain.
  */
  inline MT_INDEX MT_Root()  { return ( 1 ); }   
  inline MT_INDEX MT_Drain() { return ( N_Num ); }  

  /*
  Number of arcs entering and leaving a node.
  */
  inline int MT_NumInArcs(MT_INDEX n)
  {  return ( N_Arr[n].my_in[1] );  }
  
  inline int MT_NumOutArcs(MT_INDEX n)
  {  return ( 1 + N_Arr[n].my_out[1] - N_Arr[n].my_out[0]);  }

  /*
  Arcs entering and leaving a node.
  */
  inline MT_INDEX MT_FirstInArc(MT_INDEX n)   
  { return ( N_Arr[n].my_in[0] ); }

  inline MT_INDEX MT_NextInArc(MT_INDEX n, MT_INDEX a)
  { return ( A_Arr[a].my_next ); }

  inline MT_INDEX MT_FirstOutArc(MT_INDEX n)  
  { return ( N_Arr[n].my_out[0] ); }

  inline MT_INDEX MT_NextOutArc(MT_INDEX n, MT_INDEX a) 
  { 
    return ( ((a<=A_Num) && (MT_ArcSource(a+1)==n)) ? 
                  a+1 : MT_NULL_INDEX ); 
  }

  /*
  Number of tiles labelling an arc.
  */
  inline int MT_NumArcTiles(MT_INDEX a) 
  {  return ( 1 + A_Arr[a].my_tile[1] - A_Arr[a].my_tile[0] );  }

  /*
  Tiles labelling an arc.
  */
  inline MT_INDEX MT_FirstArcTile(MT_INDEX a) 
  { return ( A_Arr[a].my_tile[0] ); }
  
  inline MT_INDEX MT_NextArcTile(MT_INDEX a, MT_INDEX t)
  { return ( (t<A_Arr[a].my_tile[1]) ? t+1 : MT_NULL_INDEX ); }

  /*
  Source and destination of an arc.
  */
  inline MT_INDEX MT_ArcSource(MT_INDEX a)
  { return ( A_Arr[a].my_node[0] ); }

  inline MT_INDEX MT_ArcDest(MT_INDEX a)
  { return ( A_Arr[a].my_node[1] ); }

  /*
  Arc containing a tile in its label.
  */
  inline MT_INDEX MT_TileArc(MT_INDEX t) 
  { return ( TA_Arr[t].my_arc ); }

/* ------------------------------------------------------------------------ */
/*                          Derived DAG operations                          */
/* ------------------------------------------------------------------------ */

  /*
  Tiles of a node (tiles created by a node), and their number.
  */
  inline MT_INDEX MT_FirstCreatedTile(MT_INDEX n)  
  {  return ( MT_FirstArcTile(MT_FirstOutArc(n)) ); }

  MT_INDEX MT_NextCreatedTile(MT_INDEX n, MT_INDEX t);

  int MT_NumCreatedTiles(MT_INDEX n);
  
  /*
  Tiles of the interference set of a node (tiles removed by a node),
  and their number.
  */
  inline MT_INDEX MT_FirstRemovedTile(MT_INDEX n)
  {  return ( MT_FirstArcTile(MT_FirstInArc(n)) ); }

  MT_INDEX MT_NextRemovedTile(MT_INDEX n, MT_INDEX t);

  int MT_NumRemovedTiles(MT_INDEX n);

  /*
  Node containing a tile (node creating a tile) and node whose 
  interference set contains a tile (node removing a tile).
  */
  inline MT_INDEX MT_TileCreator(MT_INDEX t) 
  { return ( MT_ArcSource(MT_TileArc(t)) ); }

  inline MT_INDEX MT_TileRemover(MT_INDEX t) 
  { return ( MT_ArcDest(MT_TileArc(t)) ); }

  /*
  First parent and child of a node.
  */
  inline MT_INDEX MT_FirstParent(MT_INDEX n)
  { return ( MT_ArcSource(MT_FirstInArc(n)) ); }

  inline MT_INDEX MT_FirstChild(MT_INDEX n)
  { return ( MT_ArcDest(MT_FirstOutArc(n)) ); }

/* ------------------------------------------------------------------------ */
/*                          Creation and deletion                           */
/* ------------------------------------------------------------------------ */

  public: //protected: 

  /*
  Add an arc of index i, source s and destination d.
  */
  int MT_AddArc (MT_INDEX i, MT_INDEX s, MT_INDEX d);
  
  /*
  Add a tile t to the label of an arc a.
  */
  int MT_AddTileArc (MT_INDEX t, MT_INDEX a);

  public:
  
  /*
  Set the dimensions of the arrays.
  Function MT_SetTileNum is redefined from superclass MT_TileSetClass
  in order to allocate the tile-to-arc array as well.
  Return 1 on success, 0 on failure.
  */
  int MT_SetTileNum (MT_INDEX i);
  int MT_SetArcNum (MT_INDEX i);
  int MT_SetNodeNum (MT_INDEX i);

  /*
  Create a Multi-Tesselation given the dimensions of the embedding space 
  and of the tiles. The MT contains no elements.
  */
  MT_MultiTesselationClass ( int vert_dim, int tile_dim );

  /*
  Destroy a Multi-Tesselation.
  */
  virtual ~MT_MultiTesselationClass ();

/* ------------------------------------------------------------------------ */
/*                            Reading functions                             */
/* ------------------------------------------------------------------------ */

  public:

  /*
  Read an arc and load it into the arc array at position a.
  Return 1 on success, 0 on failure.
  */
  int MT_ReadArc(FILE * fd, MT_INDEX a, int file_encoding);

  /*
  Read the list of tiles labelling an Arc, and load them into the arc.
  Return 1 on success, 0 on failure.
  */
  int MT_ReadArcLabel(FILE * fd, MT_INDEX a, int file_encoding);

  /*
  Functions redefined from superclass MT_TileSetClass.
  */
  int MT_ReadObjParams(FILE * fd);
  int MT_ReadBody(FILE * fd, int file_encoding);

/* ------------------------------------------------------------------------ */
/*                            Writing functions                             */
/* ------------------------------------------------------------------------ */

  /*
  Write an Arc.
  */
  void MT_WriteArc(FILE * fd, MT_INDEX a, int file_encoding);

  /*
  Write the list of tiles labelling an Arc. 
  */
  void MT_WriteArcLabel(FILE * fd, MT_INDEX a, int file_encoding);
   
  /*
  Functions redefined from superclass MT_TileSetClass.
  */
  void MT_WriteObjParams(FILE * fd);
  void MT_WriteBody(FILE * fd, int file_encoding);

/* ------------------------------------------------------------------------ */
/*                                 Friends                                  */
/* ------------------------------------------------------------------------ */

   friend class MT_BuildingInterfaceClass; 

};

/* ------------------------------------------------------------------------ */

#endif  /* MT_MULTI_INCLUDED */
