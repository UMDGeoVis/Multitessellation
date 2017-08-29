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

/* ----------------------------- mt_multi.c ------------------------------- */

#include "mt_multi.h"

/* ------------------------------------------------------------------------ */
/*                      THE MULTI-TESSELATION CLASS                         */
/* ------------------------------------------------------------------------ */
  
/* ------------------------------------------------------------------------ */
/*                          Derived DAG operations                          */
/* ------------------------------------------------------------------------ */

MT_INDEX MT_MultiTesselationClass :: 
         MT_NextCreatedTile(MT_INDEX n, MT_INDEX t)
{
  MT_INDEX aux_t;
  if ((aux_t = MT_NextArcTile(MT_TileArc(t),t)) == MT_NULL_INDEX)
  {
    MT_INDEX aux_a;
    if ((aux_a = MT_NextOutArc(n,MT_TileArc(t)))!=MT_NULL_INDEX) 
      return MT_FirstArcTile(aux_a);
    else return MT_NULL_INDEX;
  }
  return aux_t; /* MT_NextArcTile(MT_TileArc(t),t);*/
}

int MT_MultiTesselationClass :: MT_NumCreatedTiles(MT_INDEX n)
{
  int count = 0;
  MT_INDEX a;
  for (a = MT_FirstOutArc(n); 
       a!= MT_NULL_INDEX; 
       a = MT_NextOutArc(n,a))
  {  count += MT_NumArcTiles(a);  }
  return (count);
}

MT_INDEX MT_MultiTesselationClass :: 
         MT_NextRemovedTile(MT_INDEX n, MT_INDEX t) 
{
  MT_INDEX aux_t;
  if ((aux_t = MT_NextArcTile(MT_TileArc(t),t)) == MT_NULL_INDEX)
  {
    MT_INDEX aux_a;
    if ((aux_a = MT_NextInArc(n,MT_TileArc(t)))!=MT_NULL_INDEX) 
      return MT_FirstArcTile(aux_a);
    else return MT_NULL_INDEX;
  }
  return aux_t; /* MT_NextArcTile(MT_TileArc(t),t);*/
}

int MT_MultiTesselationClass :: MT_NumRemovedTiles(MT_INDEX n)
{
  int count = 0;
  MT_INDEX a;
  for (a = MT_FirstInArc(n); 
       a!= MT_NULL_INDEX; 
       a = MT_NextInArc(n,a))
  {  count += MT_NumArcTiles(a);  }
  return (count);
}

/* ------------------------------------------------------------------------ */
/*                          Creation and deletion                           */
/* ------------------------------------------------------------------------ */

/*
In the functions below, sometimes malloc and sometimes calloc is used 
to allocate an array.  Calloc is used whenever all the array elements 
must be initialized with null values.
*/

int MT_MultiTesselationClass :: MT_AddArc (MT_INDEX i, MT_INDEX s, MT_INDEX d)
{
  A_Arr[i].my_node[0] = s;
  A_Arr[i].my_node[1] = d;
  if (N_Arr[s].my_out[0] == MT_NULL_INDEX) N_Arr[s].my_out[0] = i;
  N_Arr[s].my_out[1] = i;
  N_Arr[d].my_in[1]++;
  A_Arr[i].my_next = N_Arr[d].my_in[0];
  N_Arr[d].my_in[0] = i;
  return 1;
}

int MT_MultiTesselationClass :: MT_AddTileArc (MT_INDEX t, MT_INDEX a)
{
  TA_Arr[t].my_arc = a;
  if (A_Arr[a].my_tile[0] == MT_NULL_INDEX)  
  {  A_Arr[a].my_tile[0] = A_Arr[a].my_tile[1] = t; }
  else 
  {  
    if (t < A_Arr[a].my_tile[0]) A_Arr[a].my_tile[0] = t;  
    if (t > A_Arr[a].my_tile[1]) A_Arr[a].my_tile[1] = t;
  }
  return 1;
}

int MT_MultiTesselationClass :: MT_SetArcNum(MT_INDEX i)
{
  if (A_Arr) free (A_Arr); A_Arr = NULL;
  A_Arr = (MT_Arc *) calloc ( i+1, sizeof(MT_Arc) );
  if (A_Arr) 
  {  A_Num = i; return 1;  }
  /* else */
  MT_Error((char *)"Allocation failed",(char *)"MT_MultiTesselationClass::MT_SetArcNum");
  A_Num = 0;
  return 0;
}

int MT_MultiTesselationClass :: MT_SetNodeNum(MT_INDEX i)
{
  if (N_Arr) free (N_Arr); N_Arr = NULL;
  N_Arr = (MT_Node *) calloc ( i+1, sizeof(MT_Node) );
  if (N_Arr)
  {  N_Num = i; return 1;  }
  /* else */
  MT_Error((char *)"Allocation failed",(char *)"MT_MultiTesselationClass::MT_SetNodeNum");
  N_Num = 0;
  return 0;
}

int MT_MultiTesselationClass :: MT_SetTileNum (MT_INDEX i)
{
  if ( MT_TileSetClass::MT_SetTileNum(i) )
  {  if (TA_Arr) free (TA_Arr); TA_Arr = NULL;
     TA_Arr = (MT_TileToArc *) calloc ( i+1, sizeof(MT_TileToArc) );
     if (TA_Arr) 
     {  T_Num = i; return 1;  }
  }
  /* else */
  MT_Error((char *)"Allocation failed",(char *)"MT_MultiTesselationClass::MT_SetTileNum");
  T_Num = 0;
  return 0;
}

MT_MultiTesselationClass :: 
MT_MultiTesselationClass (int vert_dim, int tile_dim)
: MT_TileSetClass(vert_dim, tile_dim)
{
  MT_Message((char *)"",(char *)"MT_MultiTesselation Constructor");
  A_Num = N_Num = 0;
  TA_Arr = NULL;
  A_Arr = NULL;
  N_Arr = NULL;
}

MT_MultiTesselationClass :: ~MT_MultiTesselationClass ( void )
{
   MT_Message((char *)"",(char *)"MT_MultiTesselation Destructor");
   /* Arrays all_vert, all_coord, V_Arr, T_Arr, F_min, F_max are already
      freed by the destructor of superclass MT_TileSetClass */
   if (TA_Arr) free (TA_Arr); TA_Arr = NULL;
   if (A_Arr) free (A_Arr); A_Arr = NULL;
   if (N_Arr) free (N_Arr); N_Arr = NULL;
}


/* ------------------------------------------------------------------------ */
/*                            Reading functions                             */
/* ------------------------------------------------------------------------ */

int MT_MultiTesselationClass :: 
    MT_ReadArc(FILE * fd, MT_INDEX a, int file_encoding)
{
   MT_INDEX src, dst;
   if (MT_ReadIndex(fd, &src, file_encoding) &&
       MT_ReadIndex(fd, &dst, file_encoding) )
   {
      MT_AddArc(a,src,dst);
      return 1;
   }
   /* else */
   MT_Error((char *)"2 MT_INDEXes expected",
            (char *)"MT_MultiTesselationClass::MT_ReadArc");
   return 0;
}

int MT_MultiTesselationClass :: 
    MT_ReadArcLabel(FILE * fd, MT_INDEX a, int file_encoding)
{
   MT_INDEX t;
   do
   {
     if (!MT_ReadIndex(fd, &t, file_encoding))
     {
       MT_Error((char *)(char *)"MT_INDEX expected",
                (char *)"MT_MultiTesselationClass::MT_ReadArcLabel");
       return 0;
     }
     if (t != MT_NULL_INDEX)  MT_AddTileArc(t, a);
   }
   while ( t != MT_NULL_INDEX );
   return 1;
}

int MT_MultiTesselationClass :: MT_ReadObjParams(FILE * fd)
{  int vn, tn, nn, an;
   if (!MT_SearchKeyword(fd, (char *)MT_OBJECT_KW)) return 0;
   if (fscanf(fd, "%d %d %d %d", &vn, &tn, &nn, &an)==4)
   {  if ( (vn>=0) && (tn>=0) && (nn>=0) && (an>=0) ) 
      {  V_Num = vn;
         T_Num = tn;
         N_Num = nn;
         A_Num = an;
         return 1;
      }
   }
   /* else */
   MT_Error((char *)"Invalid object parameters",
            (char *)"MT_MultiTesselationClass::MT_ReadObjParams");
   return 0;
}

int MT_MultiTesselationClass :: MT_ReadBody(FILE * fd, int file_encoding)
{  
   int i;
   /* read vertices and tiles */
   if (!MT_TileSetClass::MT_ReadBody(fd, file_encoding)) return 0;
   /* allocate node array and arc array */
   if (!MT_SetNodeNum(N_Num)) return 0;
   if (!MT_SetArcNum(A_Num)) return 0;
   /* read arcs */
   for (i=1; i<A_Num+1; i++)
   {
     if (!MT_ReadArc(fd, i, file_encoding)) return 0;
     if (!MT_ReadArcLabel(fd, i, file_encoding)) return 0;
   }
   return 1;
}

/* ------------------------------------------------------------------------ */
/*                            Writing functions                             */
/* ------------------------------------------------------------------------ */

void MT_MultiTesselationClass :: 
     MT_WriteArc(FILE * fd, MT_INDEX a, int file_encoding)
{
   MT_INDEX src, dst;
   src = MT_ArcSource(a);
   dst = MT_ArcDest(a);
   MT_WriteIndex(fd,src,file_encoding);
   if (file_encoding==MT_ASCII_ENCODING) fprintf(fd, " ");
   MT_WriteIndex(fd,dst,file_encoding);
   if (file_encoding==MT_ASCII_ENCODING) fprintf(fd, "\n");
}

void MT_MultiTesselationClass :: 
     MT_WriteArcLabel(FILE * fd, MT_INDEX a, int file_encoding)
{
   MT_INDEX t;
   t = MT_FirstArcTile(a);
   while (t != MT_NULL_INDEX)
   {
      MT_WriteIndex(fd,t,file_encoding);
      if (file_encoding==MT_ASCII_ENCODING) fprintf(fd, " ");
      t = MT_NextArcTile(a,t);
   }
   /* write MT_NULL_INDEX as a terminator of the list */
   MT_WriteIndex(fd,t,file_encoding);
   if (file_encoding==MT_ASCII_ENCODING) fprintf(fd,"\n");
}
   
void MT_MultiTesselationClass :: MT_WriteObjParams(FILE * fd)
{  
   MT_WriteKeyword(fd, (char *)MT_OBJECT_KW);
   fprintf(fd, " %d %d %d %d\n", V_Num, T_Num, N_Num, A_Num);
}

void MT_MultiTesselationClass :: MT_WriteBody(FILE * fd, int file_encoding)
{  
   int i;
   /* write vertice and tiles */
   MT_TileSetClass::MT_WriteBody(fd, file_encoding);
   /* write arcs */
   for (i=1; i<A_Num+1; i++)
   {
      MT_WriteArc(fd, i, file_encoding);
      MT_WriteArcLabel(fd, i, file_encoding);
   }
}

/* ------------------------------------------------------------------------ */
