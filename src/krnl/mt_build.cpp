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

/* ---------------------------- mt_build.c -------------------------------- */

#include "mt_build.h"
#include "mt_abld.h"

/* ------------------------------------------------------------------------ */
/*                         BUILDING BASE CLASS                              */
/* ------------------------------------------------------------------------ */

void MT_BuildingBaseClass :: 
    ConnectVertexAttrBuilder(MT_AttrBuildingInterface abi)
{  }

void MT_BuildingBaseClass :: 
    ConnectTileAttrBuilder(MT_AttrBuildingInterface abi)
{  abi->MT_SetTileRenumbering(table3);  }

MT_BuildingBaseClass :: MT_BuildingBaseClass(void)
{  table3 = NULL;  }

/* ------------------------------------------------------------------------ */
/*                           MT BUILDING INTERFACE                          */
/* ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */
/*                         Local macros and variables                       */
/* ------------------------------------------------------------------------ */

/*
Prefixes and extension of the auxiliary file names for history tracing.
*/
static char * aux_prf[NUM_AUX_HISTORY_FILES] =
  {  (char *)"vert_", (char *)"tile_", (char *)"updt_", (char *)"this_"  };
#define MT_AUX_EXT ".aux"

/* ------------------------------------------------------------------------ */
/*                           Auxiliary functions                            */
/* ------------------------------------------------------------------------ */

int MT_BuildingInterfaceClass :: InitTables(int s)
{
  table1 = new MT_IndexToIndexClass(s);
  table2 = new MT_IndexToIndexPairClass(s);
//MT_CheckMemory("Create le prime due");/**********/
  table3 = new MT_IndexToIndexClass(s);
  if ( ! (table1 && table2 && table3) )
  {  MT_Error((char *)"Allocation failed",(char *)"MT_BuildingInterfaceClass::InitTables");
     return 0;
  }
  return 1;
}

/* ------------------------------------------------------------------------ */
/*                        Functions for history tracing                     */
/* ------------------------------------------------------------------------ */

int MT_BuildingInterfaceClass :: 
    MT_StartHistory(int vert_dim, int tile_dim, int history_type)
{
  int i;
  char base_name[MT_MAX_STRLEN]; /* base name for auxiliary files */

  if (my_state!=MT_INITIAL_STATE)
  {  MT_Error((char *)"A history or an MT already in memory",
              (char *)"MT_BuildingInterfaceClass::MT_StartHistory");
     return 0;
  }
  my_tileset = new MT_TileSetClass (vert_dim, tile_dim);
  if (!my_tileset)
  {  MT_Error((char *)"Allocation failed",
              (char *)"MT_BuildingInterfaceClass::MT_StartHistory");
     return 0;
  }
  if (! my_tileset->MT_SetVertexNum(1) ) return 0;
  if (! my_tileset->MT_SetTileNum(1) ) return 0;  

  sprintf(base_name, "%d", (this));
  for (i=0;i<NUM_AUX_HISTORY_FILES;i++)
  {
    strcpy(aux_name[i],aux_prf[i]);
    strcat(aux_name[i], base_name);
    strcat(aux_name[i],MT_AUX_EXT);
  }
  my_type = history_type;
  for (i=0;i<NUM_AUX_HISTORY_FILES;i++)
  {
    aux_fd[i] = fopen(aux_name[i],"w"); 
    if (!aux_fd[i]) 
    {  MT_Error((char *)"Cannot open auxiliary file",
                (char *)"MT_BuildingInterfaceClass::MT_StartHistory");
       return 0;
    }
  }
  my_state = MT_TRACING_STATE;
  return 1;
}

MT_INDEX MT_BuildingInterfaceClass :: 
         MT_UseVertex(MT_INDEX i, float * coord_array)
{
  if (my_state!=MT_TRACING_STATE)
  {  MT_Error((char *)"History not started, or already ended",
              (char *)"MT_BuildingInterfaceClass::MT_UseVertex");
     return 0;
  }
  if (my_ind > my_tileset->MT_NumTileVertices())
  {  MT_Error((char *)"Extra vertex",(char *)"MT_BuildingInterfaceClass::MT_UseVertex");
     return i;
  }
  my_tileset->MT_AddVertex(1, coord_array);
  if (i == 0) /* il vertice non era mai stato usato prima */
  {
     i = ++V_Num;
     my_tileset->MT_WriteVertex(aux_fd[VERT_AUX_HISTORY_FILE], 
                 1, MT_AUX_ENCODING);
  }
  my_vert[my_ind++] = i;
  return i;
}

MT_INDEX MT_BuildingInterfaceClass :: MT_MakeTile(void)
{
  IndexList aux;

  if (my_state!=MT_TRACING_STATE)
  {  MT_Error((char *)"History not started, or already ended",
              (char *)"MT_BuildingInterfaceClass::MT_MakeTile");
     return 0;
  }
  if (my_ind != my_tileset->MT_NumTileVertices())
  {  MT_Error((char *)"Wrong number of tile vertices",
              (char *)"MT_BuildingInterfaceClass::MT_MakeTile");
     return MT_NULL_INDEX;
  }
  T_Num++;
  my_tileset->MT_AddTile(1, my_vert);
  my_tileset->MT_WriteIndex(aux_fd[THIS_AUX_HISTORY_FILE], 
                            T_Num, MT_AUX_ENCODING);
  my_tileset->MT_WriteTile(aux_fd[THIS_AUX_HISTORY_FILE], 
                           1, MT_AUX_ENCODING);
  my_ind = 0;
  /* save tile index in new_list */
  aux = (IndexList) malloc(sizeof(struct IndexListStruct));
  aux->elem = T_Num;
  aux->next = NULL;
  if (!new_list) 
  {  new_list = new_last = aux;  }
  else 
  {  new_last->next = aux; new_last = aux;  }
  new_Num++;
  return T_Num;
}

int MT_BuildingInterfaceClass :: MT_KillTile(MT_INDEX i)
{
  IndexList aux, cursor1, cursor2;

  if (my_state!=MT_TRACING_STATE)
  {  MT_Error((char *)"History not started, or already ended",
              (char *)"MT_BuildingInterfaceClass::MT_KillTile");
     return 0;
  }
  if (i == 0)
  {  MT_Error((char *)"Tile never created", (char *)"MT_BuildingInterfaceClass::MT_KillTile");
     return 0;
  }
  if (i>=first_tri) 
  {
    /* the tile has been created in this update, it is a temporary tile */
    /* delete i from new_list */
    cursor1 = NULL; cursor2 = new_list;
    while (cursor2 && (cursor2->elem<i) )
    {  cursor1 = cursor2; cursor2 = cursor2->next;  }
    if (cursor2 && (cursor2->elem == i) )
    {  aux = cursor2;
       if (cursor1) cursor1->next = cursor2->next;
       else new_list = cursor2->next;  
       free(aux);
    }
    else
    {  MT_Error((char *)"Tile never created",(char *)"MT_BuildingInterfaceClass::MT_KillTile");
       return 0;
    }
    /* one less new tile, one more temporary tile */
    new_Num--;
    temp_Num++;
  }
  else
  {
    /* the tile is not temporary */
    /* save tile in old_list */
    aux = (IndexList) malloc(sizeof(struct IndexListStruct));
    aux->elem = i;
    aux->next = old_list;
    old_list = aux;
    old_Num++;
  }
  return 1;
}
       
int MT_BuildingInterfaceClass :: MT_EndUpdate(void)
{
  MT_INDEX i;
  IndexList aux;

  if (my_state!=MT_TRACING_STATE)
  {  MT_Error((char *)"History not started, or already ended",
              (char *)"MT_BuildingInterfaceClass::MT_EndUpdate");
     return 0;
  }
  /* move the tiles created in the current update to the tile file,
     except the temporary ones */
  fclose(aux_fd[THIS_AUX_HISTORY_FILE]);
  aux_fd[THIS_AUX_HISTORY_FILE] = fopen(aux_name[THIS_AUX_HISTORY_FILE], "r");
  aux = new_list;
  while ( aux )
  {
     MT_ReadIndex(aux_fd[THIS_AUX_HISTORY_FILE], &i, MT_AUX_ENCODING);
     my_tileset->MT_ReadTile(aux_fd[THIS_AUX_HISTORY_FILE], 
                             1, MT_AUX_ENCODING);
     if (i==aux->elem)
     {  /* not a temporary tile */
        my_tileset->MT_WriteTile(aux_fd[TILE_AUX_HISTORY_FILE], 
                                 1, MT_AUX_ENCODING);
        aux = aux->next;
     }
     /* else this is a temporary tile, skip it and read the next one */
  }
  fclose(aux_fd[THIS_AUX_HISTORY_FILE]);
  aux_fd[THIS_AUX_HISTORY_FILE] = fopen(aux_name[THIS_AUX_HISTORY_FILE], "w");
  
  /* write the current update on the update file */

  /* write the list of removed tiles and free it */
  MT_WriteCount(aux_fd[UPDT_AUX_HISTORY_FILE], old_Num, MT_AUX_ENCODING);
  if (MT_AUX_ENCODING==MT_ASCII_ENCODING)
      fprintf(aux_fd[UPDT_AUX_HISTORY_FILE], "\n");
  while (old_list)
  {
    MT_WriteIndex(aux_fd[UPDT_AUX_HISTORY_FILE], 
                  old_list->elem, MT_AUX_ENCODING);
    aux = old_list;
    old_list = old_list->next;
    free(aux);
  } 
  if (MT_AUX_ENCODING==MT_ASCII_ENCODING) 
      fprintf(aux_fd[UPDT_AUX_HISTORY_FILE],"\n");
  old_Num = 0;

  /* write the list of created tiles and free it */
  MT_WriteCount(aux_fd[UPDT_AUX_HISTORY_FILE], new_Num, MT_AUX_ENCODING);
  if (MT_AUX_ENCODING==MT_ASCII_ENCODING)
      fprintf(aux_fd[UPDT_AUX_HISTORY_FILE], "\n");
  while (new_list)
  {  
    MT_WriteIndex(aux_fd[UPDT_AUX_HISTORY_FILE],
                  new_list->elem, MT_AUX_ENCODING);
    aux = new_list;
    new_list = new_list->next;
    free(aux);
  }
  new_last = NULL;
  if (MT_AUX_ENCODING==MT_ASCII_ENCODING)
      fprintf(aux_fd[UPDT_AUX_HISTORY_FILE], "\n");
  new_Num = old_Num = 0;
  first_tri = T_Num+1;
  U_Num++; 
  return 1;                     
}       

int MT_BuildingInterfaceClass :: MT_EndHistory(void)
{
  int i;
  if (my_state!=MT_TRACING_STATE)
  {  MT_Warning((char *)"History not started, or already ended",
                (char *)"MT_BuildingInterfaceClass::MT_EndHistory");
     return 0;
  }
  /* close auxiliary files */
  for (i=0;i<NUM_AUX_HISTORY_FILES;i++) fclose(aux_fd[i]);
  T_Num -= temp_Num;
  my_state = MT_TRACED_STATE; 
  return 1;
}

/* ------------------------------------------------------------------------ */
/*                          Read/write functions                            */
/* ------------------------------------------------------------------------ */

int MT_BuildingInterfaceClass :: MT_ReadObjParams(FILE * fd)
{
  int vn, tn, nn;
  char aux[MT_MAX_STRLEN];
  if (!MT_SearchKeyword(fd, (char *)MT_OBJECT_KW)) return 0;
  if (fscanf(fd, "%d %d %d %s", &vn, &tn, &nn, &aux)==4)
  {
    if ( (vn>=0) && (tn>=0) && (nn>=0) )
    {  
       V_Num = vn;  T_Num = tn;  U_Num = nn;
       if (strcmp(aux,MT_REFINE_KW)==0) 
       {  my_type = MT_REFINING; return 1;  }
       if (strcmp(aux,MT_COARSEN_KW)==0) 
       {  my_type = MT_COARSENING; return 1;  }
    }
  }
  MT_Error((char *)"Invalid object parameters",
           (char *)"MT_BuildingInterfaceClass::MT_ReadObjParams");
  return 0;
}

int MT_BuildingInterfaceClass :: MT_ReadBody(FILE * fd, int file_encoding)
{
  int flag;                /* result of reading operations */
  int count;
  unsigned int i;            /* counters */
  MT_INDEX a;              /* arc */
  MT_INDEX t, t1;          /* tiles */
  MT_INDEX n, n1;          /* nodes */
  MT_INDEX temp;
 
  /* set the number of MT vertices, tiles and nodes */
  my_mt->MT_SetVertexNum(V_Num);
  my_mt->MT_SetTileNum(T_Num);
  my_mt->MT_SetNodeNum(U_Num);
  
  /* read vertices */
  for (i=1; i<=my_mt->MT_VertexNum(); i++)
  {  my_mt->MT_ReadVertex(fd, i, file_encoding);  }

  /* create auxiliary tables */
  if (my_mt->MT_TileNum()<100) flag = InitTables(10);
  else flag = InitTables(my_mt->MT_TileNum()/10);
  if (!flag)
  {  MT_Error((char *)"Cannot allocate auxiliary structures",
              (char *)"MT_BuildingInterfaceClass::MT_ReadBody");
     return 0;
  }

  /* read the updates */
  for (n=1; n<=my_mt->MT_NodeNum(); n++)
  {
    /* read number of old tiles for this update (number of removed/created 
       tiles in case of refinement/simplification, respectively) */
    flag = MT_ReadCount(fd, &count, file_encoding);
    if (flag == EOF) return 0;
    for (i=0;i<(unsigned int) count;i++) /* loop on old tiles */
    {
      flag = MT_ReadIndex(fd, &t, file_encoding);
      if (flag == EOF) return 0;
      /* retrieve node n1 which created t (in case of refinement),
         retrieve node n1 which removed t (in case of simplification) */
      n1 = table1->GetAssoc(t);
      if (my_type==MT_COARSENING) table2->AddAssoc(n,n1,t);
           /* record that t is in the label of arc (n,n1) */
      else table2->AddAssoc(n1,n,t);
           /* record that t is in the label of arc (n1,n) */
    }
    /* read number of new tiles for this update (number of created/removed 
       tiles in case of refinement/simplification, respectively) */    
    flag = MT_ReadCount(fd, &count, file_encoding);
    if (flag == EOF) return 0;
    for (i=0;i<(unsigned int) count;i++) /* loop on new tiles */
    {
      flag = MT_ReadIndex(fd, &t, file_encoding);
      if (flag == EOF) return 0;
      /* record that n is the node that created t (in case of refinement),
         or that n is the node that removed t (in case of simplification) */
      table1->AddAssoc(t,n);
    }
  }
  /* set the number of MT arcs */
  my_mt->MT_SetArcNum(table2->MT_AssocNum());

  /* find the arcs and renumber tiles in such a way that tiles labelling 
     the same arc have consecutive indexes */
  a = 0;
  t1 = 0;
  for (n=1;n<=my_mt->MT_NodeNum();n++)
  {
    table2->GetAssoc(n,&n1,&t);
    while (n1 != MT_NULL_INDEX)
    {
      a++;
      if (my_type==MT_COARSENING) 
           my_mt->MT_AddArc(a,my_mt->MT_NodeNum()-n+1,
                              my_mt->MT_NodeNum()-n1+1);
           /* in case of simplification reverse the numbering of nodes */
      else my_mt->MT_AddArc(a,n,n1);
      if (my_type==MT_COARSENING)
           temp = my_mt->MT_NodeNum() + 1 - my_mt->MT_ArcDest(a);
      else temp = my_mt->MT_ArcDest(a);
      while (n1 == temp)
      {
        t1++;
        table3->AddAssoc(t,t1);
        my_mt->MT_AddTileArc(t1,a);
        table2->GetAssoc(n,&n1,&t);
      }
    }
  }
  
  /* read tiles, and skip temporary ones */
  temp = 0;
  for (t=1; (t-temp)<=my_mt->MT_TileNum(); t++)
  {
    t1 = table3->LookAssoc(t);
    /* if t is temporary, then t1 is MT_NULL_INDEX */
    if (t1) /* not a temporary tile */
       flag = my_mt->MT_ReadTile(fd, t1, file_encoding);
    else temp++;
  }

  delete table1; table1 = NULL;
  delete table2; table2 = NULL;
  /* table3 is not deleted since it may be passed to an attribute builder */
  
  return 1;  
}   

int MT_BuildingInterfaceClass :: MT_Read(FILE * fd)
{
  if (my_state != MT_INITIAL_STATE)
  {  MT_Warning((char *)"A history is already in memory",
                (char *)"MT_BuildingInterfaceClass::MT_Read");
     return 0;
  }
  if (!my_mt)
  {  MT_Warning((char *)"A target MT must be set first",
                (char *)"MT_BuildingInterfaceClass::MT_Read");
     return 0;
  }
  if (MT_PersistentClass::MT_Read(fd))
  {  my_mt->MT_SetDescription(my_descr);
     my_state = MT_FINAL_STATE;
     return 1;
  }
  return 0;
}

int MT_BuildingInterfaceClass :: MT_Read(char * fname)
{
  FILE * fd = fopen(fname,"r");
  int ok = 0;
  if (fd) ok = MT_Read(fd);
  fclose(fd);
  return ok;
}

/* ------------------------------------------------------------------------ */

void MT_BuildingInterfaceClass :: MT_WriteObjParams(FILE * fd)
{
  MT_WriteKeyword(fd, (char *)MT_OBJECT_KW);
  fprintf(fd, " %d %d %d ", V_Num, T_Num, U_Num);
  switch (my_type)
  {  case MT_REFINING:
           fprintf(fd, "%s\n", MT_REFINE_KW);
           break;
     case MT_COARSENING: 
           fprintf(fd, "%s\n", MT_COARSEN_KW);
           break;
  }
}               

void MT_BuildingInterfaceClass :: MT_WriteBody(FILE * fd, int file_encoding)
{
  MT_INDEX i,j,b;
  int a;
  
  /* move vertices from the vertex file to the history file */
  aux_fd[VERT_AUX_HISTORY_FILE] = fopen(aux_name[VERT_AUX_HISTORY_FILE],"r");
  for (i=0;i<V_Num;i++)
  {
    my_tileset->MT_ReadVertex(aux_fd[VERT_AUX_HISTORY_FILE],
                              1, MT_AUX_ENCODING);
    my_tileset->MT_WriteVertex(fd, 1, file_encoding);
  } 
  fclose(aux_fd[VERT_AUX_HISTORY_FILE]);
  /* empty the vertex file */
  aux_fd[VERT_AUX_HISTORY_FILE] = fopen(aux_name[VERT_AUX_HISTORY_FILE],"w");
  fclose(aux_fd[VERT_AUX_HISTORY_FILE]);

  /* move the updates from the update file to the history file */
  aux_fd[UPDT_AUX_HISTORY_FILE] = fopen(aux_name[UPDT_AUX_HISTORY_FILE],"r");
  for (i=0;i<U_Num;i++)
  {
    MT_ReadCount(aux_fd[UPDT_AUX_HISTORY_FILE], &a, MT_AUX_ENCODING);
    /* a = number of tiles removed in the update */
    MT_WriteCount(fd, a, file_encoding);
    for (j=0;j<(unsigned int) a;j++)
    {
      MT_ReadIndex(aux_fd[UPDT_AUX_HISTORY_FILE], &b, MT_AUX_ENCODING);
      /* b = index of a removed tile */
      MT_WriteIndex(fd, b, file_encoding);
    }
    if (file_encoding==MT_ASCII_ENCODING)  fprintf(fd,"\n");
    MT_ReadCount(aux_fd[UPDT_AUX_HISTORY_FILE], &a, MT_AUX_ENCODING);
    /* a = number of tiles created in the update */
    MT_WriteCount(fd, a, file_encoding);
    for (j=0;j<(unsigned int) a;j++)
    {
      MT_ReadIndex(aux_fd[UPDT_AUX_HISTORY_FILE], &b, MT_AUX_ENCODING);
      /* b = index of a tile created in this update */
      MT_WriteIndex(fd, b, file_encoding);
    }
    if (file_encoding==MT_ASCII_ENCODING)  fprintf(fd,"\n");
  } 
  fclose(aux_fd[UPDT_AUX_HISTORY_FILE]);
  /* empty the update file */
  aux_fd[UPDT_AUX_HISTORY_FILE] = fopen(aux_name[UPDT_AUX_HISTORY_FILE],"w");
  fclose(aux_fd[UPDT_AUX_HISTORY_FILE]);

  /* move tiles from the tile file to the history file */
  aux_fd[TILE_AUX_HISTORY_FILE] = fopen(aux_name[TILE_AUX_HISTORY_FILE],"r");
  for (i=0;i<T_Num;i++)
  {
    my_tileset->MT_ReadTile(aux_fd[TILE_AUX_HISTORY_FILE],
                            1, MT_AUX_ENCODING);
    my_tileset->MT_WriteTile(fd, 1, file_encoding);
  }
  fclose(aux_fd[TILE_AUX_HISTORY_FILE]);
  /* empty the tile file */
  aux_fd[TILE_AUX_HISTORY_FILE] = fopen(aux_name[TILE_AUX_HISTORY_FILE],"w");
  fclose(aux_fd[TILE_AUX_HISTORY_FILE]);
  /* delete the auxiliary tile set */
  delete my_tileset; my_tileset = NULL;
}

void MT_BuildingInterfaceClass :: MT_Write(FILE * fd, int file_encoding)
{
  if (my_state != MT_TRACED_STATE)
  {  MT_Warning((char *)"An MT history must be traced first",
                (char *)"MT_BuildingInterfaceClass::MT_Write");
  }
  else
  {  MT_PersistentClass::MT_Write(fd,file_encoding);
     my_state = MT_INITIAL_STATE;
  }
}

int MT_BuildingInterfaceClass :: MT_Write(char * fname, int file_encoding)
{
  FILE * fd = fopen(fname,"w");
  int ok = 0;
  if (fd) {  MT_Write(fd,file_encoding);  ok = 1;  }
  fclose(fd);
  return ok;
}

/* ------------------------------------------------------------------------ */

int MT_BuildingInterfaceClass :: MT_Convert(void)
{
  int flag;                /* result of reading operations */
  int count;
  unsigned int i;            /* counters */
  MT_INDEX a;              /* arc */
  MT_INDEX t, t1;          /* tiles */
  MT_INDEX n, n1;          /* nodes */
  MT_INDEX temp;
 
  if (my_state != MT_TRACED_STATE)
  {  MT_Warning((char *)"An MT history must be traced first",
                (char *)"MT_BuildingInterfaceClass::MT_Convert");
     return 0;
  }
  if (!my_mt)
  {  MT_Warning((char *)"A target MT must be set first",
                (char *)"MT_BuildingInterfaceClass::MT_Convert");
     return 0;
  }
  if (my_mt->MT_VertexDim() != my_tileset->MT_VertexDim())
  {  MT_Warning((char *)"Number of MT vertex coords does not match with history",
                (char *)"MT_BuildingInterfaceClass::MT_Convert");
     return 0;
  }
  if (my_mt->MT_TileDim() != my_tileset->MT_TileDim())
  {  MT_Warning((char *)"Dimension of MT tiles does not match with history",
                (char *)"MT_BuildingInterfaceClass::MT_Convert");
     return 0;
  }

  /* set the number of MT vertices, tiles and nodes */
  my_mt->MT_SetVertexNum(V_Num);
  my_mt->MT_SetTileNum(T_Num);
  my_mt->MT_SetNodeNum(U_Num);
  
  /* read vertices from the vertex file */
  aux_fd[VERT_AUX_HISTORY_FILE] = fopen(aux_name[VERT_AUX_HISTORY_FILE],"r");
  for (i=1; i<=V_Num; i++)
  {  my_mt->MT_ReadVertex(aux_fd[VERT_AUX_HISTORY_FILE], i, MT_AUX_ENCODING);
  }
  fclose(aux_fd[VERT_AUX_HISTORY_FILE]);
  /* empty the vertex file */
  aux_fd[VERT_AUX_HISTORY_FILE] = fopen(aux_name[VERT_AUX_HISTORY_FILE],"w");
  fclose(aux_fd[VERT_AUX_HISTORY_FILE]);

  /* create auxiliary tables */
  if (T_Num<100) flag = InitTables(10);
  else flag = InitTables(T_Num/10);
  if (!flag)
  {  MT_Error((char *)"Cannot allocate auxiliary structures",
              (char *)"MT_BuildingInterfaceClass::MT_Convert");
     return 0;
  }

  /* read the updates from the update file */
  aux_fd[UPDT_AUX_HISTORY_FILE] = fopen(aux_name[UPDT_AUX_HISTORY_FILE], "r");
  for (n=1; n<=U_Num; n++)
  {
    /* read number of old tiles for this update (number of removed/created 
       tiles in case of refinement/simplification, respectively) */
    flag = MT_ReadCount(aux_fd[UPDT_AUX_HISTORY_FILE],
                        &count, MT_AUX_ENCODING);
    if (flag == EOF) return 0;
    for (i=0;i<(unsigned int) count;i++) /* loop on old tiles */
    {
      flag = MT_ReadIndex(aux_fd[UPDT_AUX_HISTORY_FILE], &t, MT_AUX_ENCODING);
      if (flag == EOF) return 0;
      /* retrieve node n1 which created t (in case of refinement),
         retrieve node n1 which removed t (in case of simplification) */
      n1 = table1->GetAssoc(t);
      if (my_type==MT_COARSENING) table2->AddAssoc(n,n1,t);
           /* record that t is in the label of arc (n,n1) */
      else table2->AddAssoc(n1,n,t);
           /* record that t is in the label of arc (n1,n) */
    }
    /* read number of new tiles for this update (number of created/removed 
       tiles in case of refinement/simplification, respectively) */    
    flag = MT_ReadCount(aux_fd[UPDT_AUX_HISTORY_FILE],
                        &count, MT_AUX_ENCODING);
    if (flag == EOF) return 0;
    for (i=0;i<(unsigned int) count;i++) /* loop on new tiles */
    {
      flag = MT_ReadIndex(aux_fd[UPDT_AUX_HISTORY_FILE], &t, MT_AUX_ENCODING);
      if (flag == EOF) return 0;
      /* record that n is the node that created t (in case of refinement),
         or that n is the node that removed t (in case of simplification) */
      table1->AddAssoc(t,n);
    }
  }
  fclose(aux_fd[UPDT_AUX_HISTORY_FILE]);
  /* empty the update file */
  aux_fd[UPDT_AUX_HISTORY_FILE] = fopen(aux_name[UPDT_AUX_HISTORY_FILE],"w");
  fclose(aux_fd[UPDT_AUX_HISTORY_FILE]);

  /* set the number of MT arcs */
  my_mt->MT_SetArcNum(table2->MT_AssocNum());

  /* find the arcs and renumber tiles in such a way that tiles labelling 
     the same arc have consecutive indexes */
  a = 0;
  t1 = 0;
  for (n=1;n<=my_mt->MT_NodeNum();n++)
  {
    table2->GetAssoc(n,&n1,&t);
    while (n1 != MT_NULL_INDEX)
    {
      a++;
      if (my_type==MT_COARSENING) 
           my_mt->MT_AddArc(a,my_mt->MT_NodeNum()-n+1,
                              my_mt->MT_NodeNum()-n1+1);
           /* in case of simplification reverse the numbering of nodes */
      else my_mt->MT_AddArc(a,n,n1);
      if (my_type==MT_COARSENING)
           temp = my_mt->MT_NodeNum() + 1 - my_mt->MT_ArcDest(a);
      else temp = my_mt->MT_ArcDest(a);
      while (n1 == temp)
      {
        t1++;
        table3->AddAssoc(t,t1);
        my_mt->MT_AddTileArc(t1,a);
        table2->GetAssoc(n,&n1,&t);
      }
    }
  }
  
  /* read tiles from the tile file, and skip temporary ones */
  temp = 0;
  aux_fd[TILE_AUX_HISTORY_FILE] = fopen(aux_name[TILE_AUX_HISTORY_FILE],"r");
  for (t=1; (t-temp)<=my_mt->MT_TileNum(); t++)
  {
    t1 = table3->LookAssoc(t);
    /* if t is temporary, then t1 is MT_NULL_INDEX */
    if (t1) /* not a temporary tile */
       flag = my_mt->MT_ReadTile(aux_fd[TILE_AUX_HISTORY_FILE], 
                                 t1, MT_AUX_ENCODING);
    else temp++;
  }
  fclose(aux_fd[TILE_AUX_HISTORY_FILE]);
  /* empty the tile file */
  aux_fd[TILE_AUX_HISTORY_FILE] = fopen(aux_name[TILE_AUX_HISTORY_FILE],"w");
  fclose(aux_fd[TILE_AUX_HISTORY_FILE]);

  delete table1; table1 = NULL;
  delete table2; table2 = NULL;
  /* table3 is not deleted since it may be passed to an attribute builder */
  my_mt->MT_SetDescription(my_descr);
  my_state = MT_FINAL_STATE;
  return 1;  
}   

/* ------------------------------------------------------------------------ */
/*                          Creation and deletion                           */
/* ------------------------------------------------------------------------ */

MT_BuildingInterfaceClass :: MT_BuildingInterfaceClass (void)
: MT_BuildingBaseClass()
{
  MT_Message((char *)"",(char *)"MT_BuildingInterface Constructor");

  /* for history tracing */
  my_tileset = NULL;
  V_Num = T_Num = U_Num = 0;
  my_ind = 0; 
  first_tri = 0;
  old_list = new_list = new_last = NULL;
  old_Num = new_Num = temp_Num = 0;
                
  /* for history conversion */
  my_mt = NULL;
  table1 = NULL;
  table2 = NULL;
  table3 = NULL;
  
  my_state = MT_INITIAL_STATE;
}

MT_BuildingInterfaceClass :: ~MT_BuildingInterfaceClass(void)
{
  MT_Message((char *)"",(char *)"MT_BuildingInterface Destructor");
  if (my_tileset) delete my_tileset; my_tileset = NULL;
  if (table1) delete table1; table1 = NULL;
  if (table2) delete table2; table2 = NULL;
  if (table3) delete table3; table3 = NULL;
  /* leave the mt which must be deleted separately */
}

/* ------------------------------------------------------------------------ */
