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

/* ------------------------------ mt_tile.c ------------------------------- */

#include <float.h>

#include "mt_tile.h"

/* ------------------------------------------------------------------------ */
/*                          THE TESSELATION CLASS                           */
/* ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */
/*                              Bounding box                                */
/* ------------------------------------------------------------------------ */

void MT_TileSetClass :: 
     MT_BoundingBox(float * min_values, float * max_values)
{
  int i;
  for (i=0; i<V_Dim; i++)  { min_values[i] = F_min[i]; }
  for (i=0; i<V_Dim; i++)  { max_values[i] = F_max[i]; }
}

/* ------------------------------------------------------------------------ */
/*                        Information for vertices                          */
/* ------------------------------------------------------------------------ */

void MT_TileSetClass ::
     MT_CopyVertexCoords(MT_INDEX v, float * f)
{
  int i;
  for (i=0; i<V_Dim; i++)  { f[i] = V_Arr[v].my_coord[i]; }
}

/* ------------------------------------------------------------------------ */
/*                         Information for tiles                            */
/* ------------------------------------------------------------------------ */

void MT_TileSetClass ::
     MT_CopyTileVertices(MT_INDEX t, MT_INDEX * v)
{
  int i;
  for (i=0; i<T_Dim; i++)  {  v[i] = T_Arr[t].my_vertex[i]; }
}

int MT_TileSetClass :: MT_TileVertexPosition(MT_INDEX t, MT_INDEX v)
{
  int i;
  for (i=0; i<T_Dim; i++)
  {  if (v == T_Arr[t].my_vertex[i]) return i;  }
//  MT_Warning("Tile does not have the given vertex",
//             "MT_TileSetClass::MT_TileVertexPosition");
  return -1;
}

int MT_TileSetClass :: 
    MT_OppositeVertexPosition(MT_INDEX t, MT_INDEX t1, int i1)
{
  int j, j1;
  int i = 0, sum = 0;

  /* Search in t the position of each vertex of t1, except the 
     i1-th one. Accumulate the sum of all such positions on i.
     The position of the vertex of t not shared with t1 is obtained
     subtracting i from Sum_{j=0..T_Dim}. */

  for (j1=0; j1<T_Dim; j1++) sum += j1;
  
  for (j1=0; j1<T_Dim; j1++) /* for each vertex of t1 */
  {  
     if (j1 != i1) 
     {
        for (j=0; j<T_Dim; j++) /* for each vertex of t */
        {  
          if ( T_Arr[t1].my_vertex[j1] == T_Arr[t].my_vertex[j] )
          {  /* found, subtract from i */
             i+=j;  
             break;
          }
        }
        if (j==T_Dim)  
        {  /* not found, something wrong */
           MT_Warning((char *)"No common face",
                      (char *)"MT_TileSetClass::MT_OppositeVertexPosition");
           return (-1);
        }
     }
  }     
  return (sum - i);
}

/* ------------------------------------------------------------------------ */
/*                   Initialization, creation and deletion                  */
/* ------------------------------------------------------------------------ */

MT_TileSetClass :: MT_TileSetClass ( int vert_dim, int tile_dim )
{
  int j;

  MT_Message((char *)"",(char *)"MT_TileSet Constructor");
  if (tile_dim>vert_dim) 
  {  MT_Error((char *)"Embedding space cannot be lower-dimensional than that of tiles",
              (char *)"MT_TileSet Constructor");
  }
  else
  {  V_Dim = vert_dim;
     T_Dim = tile_dim+1; 
     V_Num = T_Num = 0;
     all_coord = NULL;
     all_vert = NULL;
     V_Arr = NULL;
     T_Arr = NULL;
     F_min = (float *) malloc (V_Dim*sizeof(float));
     F_max = (float *) malloc (V_Dim*sizeof(float));
     if (! (F_min && F_max) )
     {
        MT_FatalError((char *)"Allocation failed",(char *)"MT_TileSet Constructor");
     }
  //   else                         
  //   {  for (j=0; j<V_Dim; j++)   
  //      {  F_min[j] = FLT_MAX;  
  //         F_max[j] = FLT_MIN;  
  //      }
  //   }
   }
}

int MT_TileSetClass :: MT_SetVertexNum (MT_INDEX i)
{
  if (all_coord) free (all_coord); all_coord = NULL;
  if (V_Arr) free (V_Arr); V_Arr = NULL;
  all_coord = (float *) malloc ( (i+1)*V_Dim*sizeof(float) );
  V_Arr = (MT_Vertex *) calloc ( i+1, sizeof(MT_Vertex) );
  if (V_Arr && all_coord)
  { 
    unsigned int j;
    V_Num = i;
    for (j=0; j<=V_Num; j++)
    {
      V_Arr[j].my_coord = all_coord + j*V_Dim;
    }
    return 1;
  }
  /* else */
  MT_Error((char *)"Allocation failed",(char *)"MT_TileSetClass::MT_SetVertexNum");
  V_Num = 0;
  return 0;
}

int MT_TileSetClass :: MT_SetTileNum (MT_INDEX i)
{
  if (all_vert) free (all_vert); all_vert = NULL;
  if (T_Arr) free (T_Arr); T_Arr = NULL;
  all_vert = (MT_INDEX *) calloc ( (i+1)*T_Dim, sizeof(MT_INDEX) );
  T_Arr = (MT_Tile *) calloc ( i+1, sizeof(MT_Tile) );
  if (T_Arr && all_vert)
  { 
    unsigned int j;
    T_Num = i;
    for (j=0; j<=T_Num; j++)
    {
      T_Arr[j].my_vertex = all_vert + j*T_Dim;
    }
    return 1;
  }
  /* else */
  MT_Error((char *)"Allocation failed",(char *)"MT_TileSetClass::MT_SetTileNum");
  T_Num = 0; 
  return 0;
}
  
void MT_TileSetClass :: MT_AddVertex(MT_INDEX v, float * coord_array)
{
  int j;
  for (j=0; j<V_Dim; j++)
  {
    V_Arr[v].my_coord[j] = coord_array[j];
 //   if ( V_Arr[v].my_coord[j] < F_min[j] ) F_min[j] = V_Arr[v].my_coord[j];
 //   if ( V_Arr[v].my_coord[j] > F_max[j] ) F_max[j] = V_Arr[v].my_coord[j];
  }
}

void MT_TileSetClass :: MT_AddTile(MT_INDEX t, MT_INDEX * vert_array)
{
  int j;
  for (j=0; j<T_Dim; j++)
  {
    T_Arr[t].my_vertex[j] = vert_array[j];
  }
}

MT_TileSetClass :: ~MT_TileSetClass ()
{
  MT_Message((char *)"",(char *)"MT_TileSet Destructor");
  if (all_vert) free (all_vert); all_vert = NULL;
  if (all_coord) free (all_coord); all_coord = NULL;
  if (V_Arr) free (V_Arr); V_Arr = NULL;
  if (T_Arr) free (T_Arr); T_Arr = NULL;
  if (F_min) free(F_min); F_min = NULL;
  if (F_max) free(F_max); F_max = NULL;
}

/* ------------------------------------------------------------------------ */
/*                          READ / WRITE  PRIMITIVES                        */
/* ------------------------------------------------------------------------ */

int MT_TileSetClass :: 
    MT_ReadVertex(FILE * fd, MT_INDEX v, int file_encoding)
{
  int j, flag;
  float coord[MT_MAX_DIM];
  /* read vertex coordinates */
  switch (file_encoding)
  {
     case MT_BINARY_ENCODING:
       flag = 0;
       for (j=0; j<V_Dim; j++)
       {  flag += fread(&coord[j], sizeof(float), 1, fd);  }
       break;
     case MT_ASCII_ENCODING:
       flag = 0;
       for (j=0; j<V_Dim; j++)
       flag += fscanf(fd, "%f", &coord[j]);
       break;
  }
  if (flag != V_Dim)
  {   MT_Error((char *)"Wrong number of coordinates",
               (char *)"MT_TileSetClass::MT_ReadVertex");
      return 0;
  }
  /* else */
  MT_AddVertex(v, coord);
  return 1;
}

int MT_TileSetClass :: 
    MT_ReadVertexTuple(FILE * fd, MT_INDEX * v, int file_encoding)
{
  int j, flag = 0;
  for (j=0; j<T_Dim; j++)
     flag += MT_ReadIndex(fd, &v[j], file_encoding);  
  if (flag != T_Dim)
  {   MT_Error((char *)"Wrong number of vertices",
              (char *)"MT_TileSetClass::MT_ReadTile");
      return 0;
  }
  return 1;
}

int MT_TileSetClass :: 
    MT_ReadTile(FILE * fd, MT_INDEX t, int file_encoding)
  {
    MT_INDEX vert[MT_MAX_DIM];
    if (MT_ReadVertexTuple(fd, vert, file_encoding)) 
    {  MT_AddTile(t,vert);
       return 1;
    }
    return 0;
  }

int MT_TileSetClass :: MT_ReadClassAndParams(FILE * fd)
{ 
  int vd, td;
  if (!MT_SearchKeyword(fd, MT_ClassKeyword())) return 0;
  if (fscanf(fd, "%d %d", &vd, &td)==2)
  {  if ( (vd==V_Dim) && (td==T_Dim) ) return 1;  }
  /* else */
  MT_Error((char *)"Class parameters do not match with this object",
           (char *)"MT_TileSetClass::MT_ReadClassAndParams");
  return 0;
}

int MT_TileSetClass :: MT_ReadObjParams(FILE * fd)
{
  int vn, tn;
  if (!MT_SearchKeyword(fd, (char *)MT_OBJECT_KW)) return 0;
  if (fscanf(fd, "%d %d", &vn, &tn)==2)
  {  if ( (vn>=0) && (tn>=0) ) 
     {  V_Num = vn;
        T_Num = tn;
        return 1;
     }
  }
  /* else */
  MT_Error((char *)"Invalid object parameters",
           (char *)"MT_TileSetClass::MT_ReadObjParams");
  return 0;
}

int MT_TileSetClass :: MT_ReadBody(FILE * fd, int file_encoding)
{  
   unsigned int i;
   int j;
   /* allocate vertex array and tile array */
   if (!MT_SetVertexNum(V_Num)) return 0;
   if (!MT_SetTileNum(T_Num)) return 0;
   /* read vertices */
   for (i=1; i<V_Num+1; i++)
   {  if (!MT_ReadVertex(fd, i, file_encoding)) return 0;  }
   for (i=1; i<V_Num+1; i++)
   {
	 for (j=0; j<V_Dim; j++)
     {
        if ( (i==1) || (V_Arr[i].my_coord[j] < F_min[j]) ) F_min[j] = V_Arr[i].my_coord[j];
        if ( (i==1) || (V_Arr[i].my_coord[j] > F_max[j]) ) F_max[j] = V_Arr[i].my_coord[j];
     }
   }
   /* read tiles */
   for (i=1; i<T_Num+1; i++)
   {  if (!MT_ReadTile(fd, i, file_encoding)) return 0;  }
   return 1;
}

/* ------------------------------------------------------------------------ */

void MT_TileSetClass ::
     MT_WriteVertex(FILE * fd, MT_INDEX v, int file_encoding)
{
  int j;
  switch (file_encoding)
  {  case MT_BINARY_ENCODING:
       for (j=0; j<V_Dim; j++)
       {  fwrite(&V_Arr[v].my_coord[j], sizeof(float), 1, fd);  }
       break;
     case MT_ASCII_ENCODING:
       for (j=0; j<V_Dim; j++)
       {   if (j<V_Dim-1) fprintf(fd, "%g ", V_Arr[v].my_coord[j]);
           else fprintf(fd, "%g\n", V_Arr[v].my_coord[j]);
       }
       break;
  }
}

void MT_TileSetClass :: 
     MT_WriteTile(FILE * fd, MT_INDEX t, int file_encoding)
{
  int j;
  for (j=0; j<T_Dim; j++)
  {
     MT_WriteIndex(fd,T_Arr[t].my_vertex[j],file_encoding);
     if (file_encoding==MT_ASCII_ENCODING)
     {   if (j<T_Dim-1) fprintf(fd, " ");
         else fprintf(fd, "\n");
     }
  }
}

void MT_TileSetClass :: MT_WriteClassAndParams(FILE * fd)
{  
   MT_WriteKeyword(fd, (char *)MT_ClassKeyword());
   fprintf(fd, " %d %d\n", V_Dim, T_Dim);
}

void MT_TileSetClass :: MT_WriteObjParams(FILE * fd)
{  
   MT_WriteKeyword(fd, (char *)MT_OBJECT_KW);
   fprintf(fd, " %d %d\n", V_Num, T_Num);
}

void MT_TileSetClass :: MT_WriteBody(FILE * fd, int file_encoding)
{  
   unsigned int i;
   /* write vertices */
   for (i=1; i<V_Num+1; i++)
   {  MT_WriteVertex(fd, i, file_encoding);  }
   /* write tiles */
   for (i=1; i<T_Num+1; i++)
   {  MT_WriteTile(fd, i, file_encoding);  }
}
           
/* ------------------------------------------------------------------------ */
