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

/* ------------------------------ mt_tile.h ------------------------------- */

#ifndef MT_TESSELATION_INCLUDED
#define MT_TESSELATION_INCLUDED

#include "mt_prst.h"

/* ------------------------------------------------------------------------ */
/*                       A PERSISTENT TESSELATION                           */
/* ------------------------------------------------------------------------ */

/*
This file contains the interface of a tile set, i.e., a class
of objects composed by vertices and tiles, called MT_TileSetClass.
Vertices are points in a d-dimensional space, tiles are k-dimensional
simplexes, for any d>=2 and k<=d.
Subclasses of this class will be MT (Multi-Tesselation), and the MT history.
Its superclass is MT_PersistentClass.

The internal implementation records the number of coordinates for each vertex
(dimension of the embedding space) and the number of vertices of each tile
(1 + the dimension of the tiles).
The interface adds functions to read and write vertices and tiles.
*/

/* ------------------------------------------------------------------------ */
/*                      AUXILIARY DATA STRUCTURES                           */
/* ------------------------------------------------------------------------ */

/*
Data structure for a vertex embedded in a d-dimensional space for generic d.
*/
typedef struct MT_VertexStruct
{
  float * my_coord;  /* pointer to an array containing the d coordinates */
} MT_Vertex;
  
/*
Data structure for a tile (a k-simplex).
*/
typedef struct MT_TileStruct
{
  MT_INDEX * my_vertex; /* indices of the tile's vertices in array V_Arr */
} MT_Tile;

/*
An array of vertices and an array of tiles are maintained.
Every vertex and tile is identified by its index (position) in such array.
The first element (position 0) of each array is used to encode the null
entity (value returned by operations when the result is undefined).
Valid indices start from 1.
*/
  
/* ------------------------------------------------------------------------ */
/*                          THE TESSELATION CLASS                           */
/* ------------------------------------------------------------------------ */

typedef class MT_TileSetClass * MT_TileSet;

class MT_TileSetClass : public virtual MT_PersistentClass
{

/* ------------------------------------------------------------------------ */
/*                       Internal structure                                 */
/* ------------------------------------------------------------------------ */

  protected:

  /*
  Number of coordinates needed to describe each vertex, i.e., dimensions
  of the embedding space.
  */
  int V_Dim; 

  /*
  Number of vertices of each tile (simplex), i.e., 1 + the dimension of 
  each tile.
  */
  int T_Dim;         

  /*
  Number of vertices, and array of vertices.
  */
  MT_INDEX V_Num;
  MT_Vertex * V_Arr;
  
  /*
  Number of tiles, and array of tiles.
  */
  MT_INDEX T_Num;
  MT_Tile * T_Arr;
  
  /*
  Array of V_Dim positions containing the minimum value and the maximum
  value, over all vertices, of each coordinate.
  */
  float * F_min; 
  float * F_max;
                                                          
  /*
  Array storing all vertex coordinates, entries inside V_Arr point here.
  */
  float * all_coord;

  /*
  Array storing all tile vertices, entries inside T_Arr point here.
  */
  MT_INDEX * all_vert;
  
/* ------------------------------------------------------------------------ */
/*                         Global information                               */
/* ------------------------------------------------------------------------ */

  public:

  /*
  Class keyword, redefined from superclass MT_PersistentClass.
  */
  inline char * MT_ClassKeyword(void)
  {  return (char *)"MT_TileSet";  }

  /*
  Dimensions of the embedding space and of the tiles (simplexes).
  */
  inline int MT_VertexDim(void)       { return (V_Dim); }
  inline int MT_TileDim(void)         { return (T_Dim-1); }
  inline int MT_NumTileVertices(void) { return (T_Dim); }

  /* 
  Number of vertices and tiles.
  */
  inline MT_INDEX MT_VertexNum(void)   { return (V_Num); }
  inline MT_INDEX MT_TileNum(void)     { return (T_Num); }

/* ------------------------------------------------------------------------ */
/*                              Bounding box                                */
/* ------------------------------------------------------------------------ */

  /* 
  Fill the two given arrays with the V_Dim minimum coordinate values
  and with the V_Dim maximum coordinate values of the MT's vertices.
  Arrays min_values and max_values must have at least V_Dim positions.
  */
  void MT_BoundingBox(float * min_values, float * max_values);

  /*
  Return the min and max value of the i-th coordinate
  */
  inline void MT_CoordRange(int i, float * min_v, float * max_v)
  {  (*min_v) = F_min[i]; (*max_v) = F_max[i];  }

  inline float MT_MinCoord(int i) { return ( F_min[i] ); }
  inline float MT_MaxCoord(int i) { return ( F_max[i] ); }

  /*
  Shortcuts for the first three coordinates.
  Getting the Z coordinate is only safe if we are in 3D.
  */
  inline float MT_MinX(void) { return ( F_min[0] ); }
  inline float MT_MinY(void) { return ( F_min[1] ); }
  inline float MT_MinZ(void) { return ( F_min[2] ); }
  inline float MT_MaxX(void) { return ( F_max[0] ); }
  inline float MT_MaxY(void) { return ( F_max[1] ); }
  inline float MT_MaxZ(void) { return ( F_max[2] ); }

/* ------------------------------------------------------------------------ */
/*                          Information for vertices                        */
/* ------------------------------------------------------------------------ */

  /* 
  Return a pointer to the array of coordinates of vertex v.
  */
  inline void MT_VertexCoords(MT_INDEX v, float ** f)
  {  (*f) = V_Arr[v].my_coord;  }
  inline float * MT_VertexCoords(MT_INDEX v)
  {  return (V_Arr[v].my_coord);  }

  /*
  Copy the coordinates of vertex v in array f, f must have at least
  V_Dim positions.
  */
  void MT_CopyVertexCoords(MT_INDEX v, float * f);

  /* 
  Return the i-th coordinate of v. Parameter i must be 0 <= i < V_Dim.
  */
  inline float MT_VertexCoord(MT_INDEX v, int i)
  { return ( V_Arr[v].my_coord[i] ); } 

  /*
  Shortcuts for the first three coordinates.
  Getting the Z coordinate is only safe if we are in 3D.
  */
  inline float MT_VertexX(MT_INDEX v) { return ( V_Arr[v].my_coord[0] ); }
  inline float MT_VertexY(MT_INDEX v) { return ( V_Arr[v].my_coord[1] ); } 
  inline float MT_VertexZ(MT_INDEX v) { return ( V_Arr[v].my_coord[2] ); } 
  
/* ------------------------------------------------------------------------ */
/*                         Information for tiles                            */
/* ------------------------------------------------------------------------ */

  /*
  Return a pointer to the array of vertices of tile t.
  */
  inline void MT_TileVertices(MT_INDEX t, MT_INDEX ** v)
  {  (*v) = T_Arr[t].my_vertex;  }
  inline MT_INDEX * MT_TileVertices(MT_INDEX t)
  {  return (T_Arr[t].my_vertex);  }

  /*
  Copy the vertices of tile t into array v, v must have 
  at least T_Dim positions.
  */
  void MT_CopyTileVertices(MT_INDEX t, MT_INDEX * v);

  /* 
  Return the i-th vertex of tile t.
  */
  inline MT_INDEX MT_TileVertex(MT_INDEX t, int i)
  {  return ( T_Arr[t].my_vertex[i] );  }
  
  /*
  Return the index (in the range 0..T_Dim) of vertex v in tile t.
  Return -1 if v is not a vertex of t.
  */
  int MT_TileVertexPosition(MT_INDEX t, MT_INDEX v);
 
  /*
  Return the index (in the range 0..T_Dim) of that vertex in tile t
  which is different from the i1-th vertex of t1.
  Tile t must contain all vertices of t1, except the i1-th one, i.e.,
  t1 be adjacent to t along the face opposite to such vertex.
  Otherwise, return -1.
  */
  int MT_OppositeVertexPosition(MT_INDEX t, MT_INDEX t1, int i1);

/* ------------------------------------------------------------------------ */
/*                   Initialization, creation and deletion                  */
/* ------------------------------------------------------------------------ */

  protected:

  /*
  This constructor does nothing. It is used just in subclasses.
  */
  inline MT_TileSetClass (void) { }
  /*********** CONTROLLA SE SERVE ***************/

  public:

  /*
  Create a tile set given the dimensions of the embedding space and
  of the tiles. The resulting structure contains no vertices and not tiles.
  Only the bounding box is allocated.
  The dimension of the embedding space must be equal to, or greater than
  the dimension of the tiles.
  */
  MT_TileSetClass ( int vert_dim, int tile_dim );

  /*
  Set the dimensions of the vertex and tile arrays.
  Return 1 on success, 0 on failure.
  */
  virtual int MT_SetVertexNum (MT_INDEX i);
  virtual int MT_SetTileNum (MT_INDEX i);
  
  /*
  Add a vertex, add a tile.
  */
  void MT_AddVertex(MT_INDEX v, float * coord_array);
  void MT_AddTile(MT_INDEX t, MT_INDEX * vert_array);

  /*
  Destroy a tile set.
  */
  virtual ~MT_TileSetClass ();

/* ------------------------------------------------------------------------ */
/*                          READ / WRITE  PRIMITIVES                        */
/* ------------------------------------------------------------------------ */

  public:

  /*
  Read a vertex, and load it into the vertex array at position v.
  Return 1 on success, 0 on failure.
  */
  int MT_ReadVertex(FILE * fd, MT_INDEX v, int file_encoding);

  /*
  Read a tile, and load it into the tile array at position t.
  Return 1 on success, 0 on failure.
  */
  int MT_ReadTile(FILE * fd, MT_INDEX t, int file_encoding);

  /*
  Read T_Dim vertex indexes into array v. Used inside MT_ReadTile.
  */
  int MT_ReadVertexTuple(FILE * fd, MT_INDEX * v, int file_encoding);

  /*
  Functions redefined from superclass MT_PersistentClass.
  */
  int MT_ReadClassAndParams(FILE * fd);
  int MT_ReadObjParams(FILE * fd);
  int MT_ReadBody(FILE * fd, int file_encoding);

  /*
  Write a vertex, write a tile.
  */
  void MT_WriteVertex(FILE * fd, MT_INDEX v, int file_encoding);
  void MT_WriteTile(FILE * fd, MT_INDEX t, int file_encoding);

  /*
  Functions redefined from superclass MT_PersistentClass.
  */
  void MT_WriteClassAndParams(FILE * fd);
  void MT_WriteObjParams(FILE * fd);
  void MT_WriteBody(FILE * fd, int file_encoding);
           
};

/* ------------------------------------------------------------------------ */

#endif  /* MT_TESSELATION_INCLUDED */
