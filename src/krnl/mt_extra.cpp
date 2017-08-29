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

/* ---------------------------- mt_extra.c -------------------------------- */

#include "mt_extra.h"

/* ------------------------------------------------------------------------ */
/*               EXTRACTION  OF TESSELATIONS FROM AN MT                     */
/* ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */
/*                        AUXILIARY DATA STRUCTURES                         */
/* ------------------------------------------------------------------------ */

MT_TopoClass :: MT_TopoClass (int dim)
{
  T_adj = (MT_INDEX *) calloc ( dim, sizeof(MT_INDEX) );
  /* calloc also initializes all entries to MT_NULL_INDEX */
  if (!T_adj)
  {  MT_FatalError((char *)"Failed allocation",(char *)"MT_TopoClass Constructor");  }
}

MT_TopoClass :: ~MT_TopoClass(void)  
{  free (T_adj); }

/* ------------------------------------------------------------------------ */
/*                       GENERIC  EXTRACTOR  CLASS                          */
/* ------------------------------------------------------------------------ */

/*
The implementations of functions are the ones of the global static extractor
*/


/* ------------------------------------------------------------------------ */
/*             Setting and returning the extraction conditions              */
/* ------------------------------------------------------------------------ */

int MT_ExtractorClass :: MT_SetFilter(MT_Cond flt)
{
   if ( (flt) && (!flt->MT_IsGood(mt)) )
   {  MT_Warning((char *)"Filter not good for MT",(char *)"MT_ExtractorClass::MT_SetFilter");
      return 0;
   }
   /* else */
   resol_cnd = flt;
   return 1;
}

int MT_ExtractorClass :: MT_SetFocus(MT_Cond fcs)
{
   if ( (fcs) && (!fcs->MT_IsGood(mt)) )
   {  MT_Warning((char *)"Focus not good for MT",(char *)"MT_ExtractorClass::MT_SetFilter");
      return 0;
   }
   /* else */
   place_cnd = fcs;
   return 1;
}

/* ------------------------------------------------------------------------ */
/*                       Extraction of a tesselation                        */
/* ------------------------------------------------------------------------ */

MT_INDEX MT_ExtractorClass :: MT_ExtractTesselation( void )
{
  /* 1) Reset */
  MT_ResetExtractor();
  /* 2) Expansion */
  MT_ExpandExtractor();
  /* copy the extracted tiles in the array */
  MT_ExtractionCompleted();
  return MT_ExtractedTilesNum();
}

/* ------------------------------------------------------------------------ */
/*                          Reading extracted tiles                         */
/* ------------------------------------------------------------------------ */

MT_INDEX MT_ExtractorClass :: MT_ExtractedTiles(MT_INDEX ** t_arr)
{
  if (tiles_ok)
  { 
    (*t_arr) = T_good;
    return num_good;
  }
  else 
  {
    MT_Error((char *)"Inconsistent state", (char *)"MT_ExtractorClass::MT_ExtractedTiles");
    (*t_arr) = NULL;
    return 0;
  }
}

/* ------------------------------------------------------------------------ */
/*                  Topology of the extracted tesselation                   */
/* ------------------------------------------------------------------------ */

MT_INDEX MT_ExtractorClass :: MT_TileAdj(MT_INDEX t, int i)
{
   if (!topo_enabled) return MT_NULL_INDEX;
   if ( (t == MT_NULL_INDEX) || (!topo[t]) ) return MT_NULL_INDEX;
   return ( topo[t]->T_adj[i] );
}

MT_INDEX * MT_ExtractorClass :: MT_TileAdjs(MT_INDEX t)
{
   if (!topo_enabled) return NULL;
   if ( (t == MT_NULL_INDEX) || (!topo[t]) ) return MT_NULL_INDEX;
   return ( topo[t]->T_adj );
}

void MT_ExtractorClass :: MT_CopyTileAdjs(MT_INDEX t, MT_INDEX * ta)
{
   int i;
   if (!topo_enabled || (t == MT_NULL_INDEX) || (!topo[t]) ) 
   {
     for (i=0; i<mt_dim; i++)
     {  ta[i] = MT_NULL_INDEX;  }
   }
   else
   {
     for (i=0; i<mt_dim; i++)
     {  ta[i] = topo[t]->T_adj[i];  }
   }
}

/* ------------------------------------------------------------------------ */
/*                          Auxiliary functions                             */
/* ------------------------------------------------------------------------ */

int MT_ExtractorClass :: MT_IsInSet(MT_INDEX n)
{  return ( N_set[n] == extr_count );  }
   
MT_INDEX MT_ExtractorClass :: MT_NextFrontTile (MT_INDEX t)
{
  MT_INDEX a;  /* variable for arc */
  MT_INDEX t1; /* variable for tile */
  a = mt->MT_TileArc(t);
  if ( (t1 = mt->MT_NextArcTile(a,t)) != MT_NULL_INDEX)
     return t1;
  else
  {
    if (a = A_front->MT_NextElem(a))
       return mt->MT_FirstArcTile(a);
    else return MT_NULL_INDEX;
  }
}

/* ------------------------------------------------------------------------ */

int MT_ExtractorClass :: 
    OneWaySetAdjLink(MT_INDEX t1, MT_INDEX t2, int j)
{
   if (t1!=MT_NULL_INDEX)
   {
     if (!topo[t1]) topo[t1] = new MT_TopoClass(mt_dim);
     if (!topo[t1])
     {  MT_Error((char *)"Failed allocation",(char *)"MT_ExtractorClass::MT_SetAdjLink");
        return 0;
     }
     topo[t1]->T_adj[j] = t2;
     return 1;
   }  
   return 1; /* if t1 is null, nothing to be done */
}

int MT_ExtractorClass ::
    MT_SetAdjLink(MT_INDEX t1, MT_INDEX t2, int i1, int i2)
{
   if (!topo_enabled) return 1;
   return ( OneWaySetAdjLink(t1,t2,i1) && 
            OneWaySetAdjLink(t2,t1,i2) );
}

int MT_ExtractorClass :: MT_RemAdjLinks(MT_INDEX t)
{
   if (!topo_enabled) return 1;
   if (topo[t]) 
   {
     delete topo[t];  topo[t] = NULL;
     return 1;
   }
   return 0;
}

/* ----------------------------------------------------------------------- */

int MT_ExtractorClass :: MT_ExpandExtractor (void)
{
  MT_INDEX a;    /* variable for arc */ 
  MT_INDEX t;    /* variable for tile */
  int flag;      /* boolean control variable */

  A_front->MT_CurrInit(0);
  /************ TAPULLO PER CAD ****************/
  while (A_front->MT_CurrElem(0) != MT_NULL_INDEX)
//  while ( (A_front->MT_CurrElem(0) != MT_NULL_INDEX) &&
//          (num_tiles<max_tile_num) )
  {
    if (stat_enabled)  a_tested++;

    a = A_front->MT_CurrElem(0);
    A_front->MT_CurrAdvance(0);
    /* check whether some tile of a must be refined.
       flag == 1 iff no tiles must be refined */
    flag = 1;
    t = mt->MT_FirstArcTile(a);
    while (flag && (t != MT_NULL_INDEX))
    {
       if (stat_enabled)  t_tested++;

       flag = ( (!MT_FocusTile(t,MT_LOOSE))
                 || 
                 MT_FilterTile(t,MT_LOOSE) );
              /* flag becomes == 0 if t not valid */ 
       t = mt->MT_NextArcTile(a,t);
    }
    /* if flag == 0, refine the current extracted tesselation
       by sweeping the front forward */
    if (!flag)  MT_IncludeNode(mt->MT_ArcDest(a));
  }
  return 1;
}

/* ------------------------------------------------------------------------ */

int MT_ExtractorClass :: MT_ResetExtractor(void)
{
   MT_INDEX t;

   if (stat_enabled)  MT_ResetAllStat();

   if (topo_enabled)
   {
     t = MT_FirstFrontTile();
     while (t != MT_NULL_INDEX)
     {
        MT_RemAdjLinks(t);
        t = MT_NextFrontTile(t);
     }
   }

   if (T_good)  {  free(T_good); T_good = NULL;  }
   extr_count++; /* this empties N_set */
   A_front->MT_EmptyList();
   num_good = num_tiles = 0;
   tiles_ok = 0;
   MT_IncludeNode(mt->MT_Root());
   return 1;
}

/* ------------------------------------------------------------------------ */

MT_ExtractorClass :: MT_ExtractorClass (MT_MultiTesselation m, int topo_flag)
{
   /* allocate just the fields, each derived class will add code to allocate
      additional fields and to put the extractor in an initial state */
   MT_Message((char *)"",(char *)"MT_Extractor Constructor");
   MT_Message( ((topo_flag) ? "Adj enabled" : "Adj_disabled"),
               (char *)"MT_Extractor Constructor");
   mt = m;
   mt_dim = mt->MT_NumTileVertices();
   /* no extracted tiles yet */
   num_good = num_tiles = 0;
   T_good = T_bad = NULL;
   /* no extraction conditions */
   resol_cnd = NULL;
   place_cnd = NULL;
   
   extr_count = 1;
   tiles_ok = 0;
   N_set = 
      (unsigned int *) calloc ( mt->MT_NodeNum()+1, sizeof(unsigned int) );
   A_front = new MT_ListClass(mt->MT_ArcNum()+1,1);
   if ( !(N_set && A_front) )
   {
     MT_FatalError((char *)"Allocation failed",(char *)"MT_Extractor Constructor");
   }
   topo_enabled = topo_flag;
   if (topo_enabled)
   {
     topo = (MT_Topo *) calloc ( mt->MT_TileNum()+1, sizeof(MT_Topo) );
     /* calloc is used to set all array elements to zeroes */
     hash_table = new MT_HashTableClass(10,mt);
     if ( !hash_table )
     {
       MT_FatalError((char *)"Allocation failed",(char *)"MT_Extractor Constructor");
     }
   } 

   /* by default, disable statistics */ 
   stat_enabled = 0;
   
   /********** TAPULLO PER CAD **********/
   max_tile_num = m->MT_TileNum();
}

MT_ExtractorClass :: ~MT_ExtractorClass (void)
{
   MT_Message((char *)"",(char *)"MT_Extractor Destructor");
   if (topo_enabled)
   {
     int i;
     if (topo)
     {
        if (tiles_ok)
          for (i=0;i<num_good;i++)  MT_RemAdjLinks(T_good[i]);
        free(topo);  topo = NULL;
     }
     delete hash_table;
   } 
   if (N_set) {  free(N_set); N_set = NULL;  }
   delete (A_front); A_front = NULL;
   num_good = num_tiles = 0;
   if (T_good)  {  free(T_good); T_good = NULL; T_bad = NULL;  }
   tiles_ok = 0;
}

/* ------------------------------------------------------------------------ */

MT_INDEX MT_ExtractorClass :: 
         MT_AllExtractedTiles (MT_INDEX ** t_arr1, MT_INDEX * size1,
                               MT_INDEX ** t_arr2, MT_INDEX * size2)
{
  if (tiles_ok)
  { 
    (*t_arr1) = T_good;
    (*t_arr2) = T_bad;
    (*size1) = num_good;
    (*size2) = (num_tiles - num_good); 
    return num_tiles;
  }
  else 
  {
    MT_Error((char *)"Inconsistent state", 
             (char *)"MT_ExtractorClass::MT_AllExtractedTiles");
    (*t_arr1) = (*t_arr2) = NULL;
    (*size1) = (*size2) = 0;
    return 0;
  }
}

MT_INDEX MT_ExtractorClass :: MT_AllExtractedTiles (MT_INDEX ** t_arr)
{
  if (tiles_ok)
  { 
    (*t_arr) = T_good;
    return num_tiles;
  }
  else 
  {
    MT_Error((char *)"Inconsistent state", 
             (char *)"MT_ExtractorClass::MT_AllExtractedTiles");
    (*t_arr) = NULL;
    return 0;
  }
}

/* ------------------------------------------------------------------------ */

int MT_ExtractorClass :: MT_IncludeNode (MT_INDEX n)
{
  MT_INDEX a; /* variable for arc */
  MT_INDEX p; /* variable for parent node */

/*************** TAPULLO PER CAD ************/
  /* se includendo questo nodo si sfora */
  if (num_tiles+2>max_tile_num) return 0;

  if (stat_enabled)  n_visited++;

  a = mt->MT_FirstInArc(n);
  while (a != MT_NULL_INDEX)
  {
    if (stat_enabled)  a_tested++;

    p = mt->MT_ArcSource(a);
/*************** TAPULLO PER CAD ************/
//    if (!MT_IsInSet(p)) MT_IncludeNode(p);
      if (!MT_IsInSet(p))  
      {  int flag = MT_IncludeNode(p);
         if (!flag) return 0;
      }
/**************** FINE TAPULLO **************/
      a = mt->MT_NextInArc(n,a);
  }

  MT_AddToSet(n);

  a = mt->MT_FirstInArc(n);
  while (a != MT_NULL_INDEX)
  {
    A_front->MT_RemElem(a);
    MT_RemTiles(a);
    a = mt->MT_NextInArc(n,a);
  }
  
  a = mt->MT_FirstOutArc(n);
  while (a != MT_NULL_INDEX)
  {
    A_front->MT_AddElem(a);
    MT_AddTiles(a);
    a = mt->MT_NextOutArc(n,a);
  }
  return 1;
}

/* ------------------------------------------------------------------------ */

void MT_ExtractorClass :: MT_AddToSet(MT_INDEX n)
{  
   if (stat_enabled)  n_inset++;
   N_set[n] = extr_count;
}

/* ------------------------------------------------------------------------ */

int MT_ExtractorClass :: MT_AddTiles (MT_INDEX a)
{
  if (stat_enabled)
  {  a_infront++;
     a_visited++;
     t_visited += mt->MT_NumArcTiles(a);
  }

  tiles_ok = 0;
  
  if (topo_enabled)
  {
     MT_INDEX t, t1; /* variables for 
     tiles */
     int i1;
//OTTOBRE     MT_INDEX * v;   /* for the vertices of t */
     int i;

     /* set adjacency links for the tiles of a, 
        since they are now inserted in the current tesselation */
     t = mt->MT_FirstArcTile(a);
     while (t != MT_NULL_INDEX)
     {  
//OTTOBRE        mt->MT_TileVertices(t,&v);
        for (i=0; i<mt_dim; i++)
        {  
           if (hash_table->MT_HashGet(i,t,&i1,&t1))
                 MT_SetAdjLink(t,t1,i,i1);
           else
           {
           /*****fprintf(stderr,"Metto in tabella %d(%d)\n",t,i);***/
                 hash_table->MT_HashPut(i,t);
           }
        }
        t = mt->MT_NextArcTile(a,t);
     }
  }
  num_tiles += mt->MT_NumArcTiles(a);
  return 1;
}

/* ------------------------------------------------------------------------ */

int MT_ExtractorClass :: MT_RemTiles(MT_INDEX a)
{
  if (stat_enabled)
  {  a_infront--;
     a_visited++;
     t_visited += mt->MT_NumArcTiles(a);
  }

  tiles_ok = 0;

  if (topo_enabled)
  {
    MT_INDEX t, t1, t2; /* variables for tiles */
    int i1, i2;
    MT_INDEX * v;   /* for the vertices of t */
    int i;

    /* collect the surviving adjacent tiles of the tiles of a, which
       are now removed from the current tesselation */
    t = mt->MT_FirstArcTile(a);
    while (t != MT_NULL_INDEX)
    {  
       mt->MT_TileVertices(t,&v);
       for (i=0; i<mt_dim; i++)
       {  
          if (!hash_table->MT_HashGet(i,t,&i2,&t2))
          if ( t1 = MT_TileAdj(t,i) )
          {
             i1 = mt->MT_OppositeVertexPosition(t1, t, i);
/***********/
//fprintf(stderr,"  %d %d-o vert di %d\n",mt->MT_TileVertex(t,i),i1,t1);
           hash_table->MT_HashPut(i1,t1);
//fprintf(stderr,"  Metto in tabella %d(%d)\n",t1,i1);
/************/
          }
       }
       MT_RemAdjLinks(t);
       t = mt->MT_NextArcTile(a,t);
    }
  }

  num_tiles -= mt->MT_NumArcTiles(a);
  return 1;
}

/* ------------------------------------------------------------------------ */

int MT_ExtractorClass :: MT_ExtractionCompleted(void)
{
   MT_INDEX t;  /* variable for tile */
   int i, j;    /* array indices */
   
   if (topo_enabled)
   {
     /* empty the hash table */
     while (hash_table->MT_HashPop(&i,&t));
   }

   /* Update the two arrays of extracted (active and non-active) tiles.
      Remember that it is one array containing first the active tiles and
      then the non-active ones, where T_good points to the first element
      and T_bad points to the first non-active element. */

   if (T_good) free(T_good);
   T_good = (MT_INDEX *) calloc ( num_tiles, sizeof(MT_INDEX) );
   if (!T_good)
   {
     MT_Error((char *)"Allocation failed", (char *)"MT_ExtractorClass::MT_ExtractionCompleted");
     return 0;
   }
  
   /* Fill the array with tiles, add the active ones moving forward from 
      position 0, and the non-active ones moving backward from position
      num_tiles-1.
      The two indices point to the position where the next active and
      non-active tile, respectively, will be put. */
       
   i = 0;
   j = num_tiles-1;
   t = MT_FirstFrontTile();
   while (t != MT_NULL_INDEX)
   { 
     if ( MT_FocusTile(t,MT_STRICT) )
        T_good[i++] = t;
     else
        T_good[j--] = t;     
     t = MT_NextFrontTile(t);
   }
   num_good = i;
   if (num_good<num_tiles)
      T_bad = T_good + i;
   else 
      T_bad = NULL;
   tiles_ok = 1;

   return 1;
}

/* ------------------------------------------------------------------------ */
/*                           EXTRACTION STATISTICS                          */
/* ------------------------------------------------------------------------ */

void MT_ExtractorClass :: MT_ResetAllStat(void)
{
  n_visited = n_inset = n_tested = 0;
  a_visited = a_infront = a_tested = 0;
  t_visited = t_tested = 0;
}

void MT_ExtractorClass :: MT_ResetAlgoStat(void)
{
  n_visited = n_tested = 0;
  a_visited = a_tested = 0;
  t_visited = t_tested = 0;
}

void MT_ExtractorClass :: MT_EnableStat(void)
{
  if (!stat_enabled)
  {
    MT_INDEX i;
    MT_ResetAllStat();
    /* compute n_inset */
    for (i=1; i<=mt->MT_NodeNum(); i++)
    {  if (MT_IsInSet(i)) n_inset++;  }
    /* compute a_infront */
    for (i=A_front->MT_FirstElem();
         i!=MT_NULL_INDEX;
         i=A_front->MT_NextElem(i)) 
    {  a_infront++;  }
    stat_enabled = 1;
  }
}

void MT_ExtractorClass :: MT_DisableStat(void)
{
  if (stat_enabled)
  {
    MT_ResetAllStat();
    stat_enabled = 0;
  }
}

/* ------------------------------------------------------------------------ */
/*                          GLOBAL STATIC EXTRACTOR                         */
/* ------------------------------------------------------------------------ */

MT_StaticExtractorClass :: 
MT_StaticExtractorClass(MT_MultiTesselation m, int topo_flag)
: MT_ExtractorClass(m,topo_flag)
{
   MT_Message((char *)"",(char *)"MT_StaticExtractor Constructor");
   /* set an initial state */
   MT_ResetAllStat();
   MT_IncludeNode(mt->MT_Root());
   MT_ExtractionCompleted();
}

MT_StaticExtractorClass :: ~MT_StaticExtractorClass()  
{
   MT_Message((char *)"",(char *)"MT_StaticExtractor Destructor");
}

/* ------------------------------------------------------------------------ */
/*                          GLOBAL DYNAMIC EXTRACTOR                        */
/* ------------------------------------------------------------------------ */

int MT_DynamicExtractorClass :: MT_IsInSet(MT_INDEX n)
{  return ( N_set[n] );  }
   
void MT_DynamicExtractorClass :: MT_AddToSet(MT_INDEX n)
{
   MT_INDEX a; /* variable for arc */
   MT_INDEX p; /* variable for parent node of n */

   if (stat_enabled)  n_inset++;

   MT_IncrCount(n); /* the counter should become 1 */
   /* loop on parents of n */
   a = mt->MT_FirstInArc(n);
   while (a!=MT_NULL_INDEX)
   {
     p = mt->MT_ArcSource(a);
     if (MT_IncrCount(p)==2)  /* if p is no more a leaf */
         N_free->MT_RemElem(p);
     a = mt->MT_NextInArc(n,a);
   }
   N_free->MT_AddElem(n); /* n is a leaf */
}

void MT_DynamicExtractorClass :: MT_RemFromSet(MT_INDEX n)
{
   MT_INDEX a; /* variable for arc */
   MT_INDEX p; /* variable for parent node of n */
   
   if (stat_enabled)  n_inset--;

   MT_DecrCount(n); /* the counter shold become 0 */
   /* loop on parents of n */
   a = mt->MT_FirstInArc(n);
   while (a!=MT_NULL_INDEX)
   {
      p = mt->MT_ArcSource(a);
      if (MT_DecrCount(p)==1) /* if p becomes a leaf */
          N_free->MT_AddElem(p);
      a = mt->MT_NextInArc(n,a);
   }
   N_free->MT_RemElem(n); /* n is no more a leaf */
}

/* ----------------------------------------------------------------------- */

int MT_DynamicExtractorClass :: MT_ExcludeNode (MT_INDEX n)
{
  MT_INDEX a; /* variable for arc */

  if (stat_enabled)  n_visited++;

  MT_RemFromSet(n);
  a = mt->MT_FirstInArc(n);
  while (a != MT_NULL_INDEX)
  {
    A_front->MT_AddElem(a);
    MT_AddTiles(a);
    a = mt->MT_NextInArc(n,a);
  }
  a = mt->MT_FirstOutArc(n);
  while (a != MT_NULL_INDEX)
  {
    A_front->MT_RemElem(a);
    MT_RemTiles(a);
    a = mt->MT_NextOutArc(n,a);
  }
  return 1;
}

/* ------------------------------------------------------------------------ */


int MT_DynamicExtractorClass :: MT_ContractExtractor (void)
{
  MT_INDEX n;    /* variable for node */ 
  MT_INDEX t;    /* variable for tile */
  int flag;      /* boolean control variable */

  if ( (N_free->MT_CurrElem(2) != N_free->MT_CurrElem(1)) && /* GENNAIO */
       (N_free->MT_FirstElem() == N_free->MT_CurrElem(1)) )
     /* if the first of N_free is one of those added in expansion,
        then nothing more to be done */
  {  N_free->MT_CurrFlush(0); N_free->MT_CurrAdvance(0);  }
  else
  {  N_free->MT_CurrInit(0);  }
  
  while (N_free->MT_CurrElem(0) != MT_NULL_INDEX)
  {
    n = N_free->MT_CurrElem(0);

    if (stat_enabled)  n_tested++;
    if (N_free->MT_CurrElem(0) == N_free->MT_CurrElem(1))
       /* if the next node is one of those added during expansion, then
          jump to the first node added in contraction stage */
    {  N_free->MT_CurrReach(0,2); N_free->MT_CurrAdvance(0);  }
    else
    {  N_free->MT_CurrAdvance(0);  }
    if (n != mt->MT_Root() )
    {
      /* Check the tiles of the interference set of n (the tiles removed by
         n) to see if n can be simplified. flag == 1 iff all such tiles are
         feasible for the resolution condition. */
      flag = 1;
      t = mt->MT_FirstRemovedTile(n);
      while (flag && (t != MT_NULL_INDEX))
      {
         if (stat_enabled)  t_tested++;
 
         flag = ( (!MT_FocusTile(t,MT_LOOSE))
                   || 
                   MT_FilterTile(t,MT_LOOSE) );
              /* flag becomes == 0 if t is not feasible */
         t = mt->MT_NextRemovedTile(n,t);
      }
      /* if flag == 1, then simplify */
      if ( flag )  MT_ExcludeNode(n);
    }
  }
  return 1;
}

/* --------------------------------------------------------------------------- */

MT_INDEX MT_DynamicExtractorClass :: MT_ExtractTesselation (void)
{

  if (stat_enabled)  MT_ResetAlgoStat();

  /* Put a cursor at the current end of the leaf list.
     This cursor allows recognizing where the new nodes added in 
     expansion stage start: these nodes do not need to be tested
     during contraction because it is known that they are necessary
     in order to satisfy the resolution filter. */
  N_free->MT_CurrFlush(1); 
  /* 1) Expansion */
  MT_ExpandExtractor();
  /* Put a cursor at the current end of the leaf list.
     This cursor allows recognizing where the new nodes added in 
     contraction start: these nodes must be tested during contraction.
     thus, contraction tests the nodes lying between the beginning of
     N_free and cursor 1, and between cursor 2 and the end of N_free. */
  N_free->MT_CurrFlush(2);   
  /* 2) Contraction */
  MT_ContractExtractor();
  /* copy the extracted tiles in the array */
  MT_ExtractionCompleted();
  return MT_ExtractedTilesNum();
}

/* ------------------------------------------------------------------------ */

MT_DynamicExtractorClass :: 
MT_DynamicExtractorClass(MT_MultiTesselation m, int topo_flag)
: MT_ExtractorClass(m,topo_flag)
{
   MT_Message((char *)"",(char *)"MT_DynamicExtractor Constructor");

   /* create N_free as a list with three cursors */
   N_free = new MT_ListClass(mt->MT_NodeNum()+1,3);
   if ( !N_free )
   {
      MT_FatalError((char *)"Allocation failed",(char *)"MT_DynamicExtractor Constructor");
   }

   /* set an initial state */
   MT_ResetAllStat();
   MT_IncludeNode(mt->MT_Root());
   MT_ExtractionCompleted();
}

MT_DynamicExtractorClass :: ~MT_DynamicExtractorClass(void) 
{
   MT_Message((char *)"",(char *)"MT_DynamicExtractor Destructor");
   /* deallocate the additional fields */
   delete (N_free);
}

/* ------------------------------------------------------------------------ */
/*                           LOCAL STATIC EXTRACTOR                         */
/* ------------------------------------------------------------------------ */

int MT_LocalExtractorClass :: MT_FocusArc(MT_INDEX a)
{
   MT_INDEX t;
   int flag = 0;
   
   /* check whether the result of the test is already known on a */
   if (MT_HintYes(a)) return 1;
   if (MT_HintNot(a)) return 0;
   /* if not known, look at the tiles of arc a */
   for (t = mt->MT_FirstArcTile(a); 
        ( (t != MT_NULL_INDEX) && !flag );
        t = mt->MT_NextArcTile(a,t))
   {
      if (stat_enabled)  t_tested++;

      flag = (MT_FocusTile(t,MT_LOOSE));
      /* record the computed result */
      if (flag) A_pos[a] = extr_count; else A_neg[a] = extr_count;
   }
   return flag;
}

int MT_LocalExtractorClass :: MT_IncludeNode (MT_INDEX n)
{
  MT_INDEX a; /* variable for arc */
  MT_INDEX p; /* variable for parent node */

  if (stat_enabled)  n_visited++;

  a = mt->MT_FirstInArc(n);
  while (a != MT_NULL_INDEX)
  {
    if (stat_enabled)  a_tested++;

    p = mt->MT_ArcSource(a);
    if ((!MT_IsInSet(p)) && MT_FocusArc(a))
    {
       MT_IncludeNode(p);
    }
    a = mt->MT_NextInArc(n,a);
  }

  MT_AddToSet(n);

  a = mt->MT_FirstInArc(n);
  while (a != MT_NULL_INDEX)
  {
    if ( A_front->MT_IsInList(a) )
    {
      A_front->MT_RemElem(a);
      MT_RemTiles(a);
    }
    a = mt->MT_NextInArc(n,a);
  }
  
  a = mt->MT_FirstOutArc(n);
  while (a != MT_NULL_INDEX)
  {
    if (MT_FocusArc(a))
    {
      A_front->MT_AddElem(a);
      MT_AddTiles(a);
    }
    a = mt->MT_NextOutArc(n,a);
  }
  return 1;
}

MT_LocalExtractorClass :: 
MT_LocalExtractorClass(MT_MultiTesselation m, int topo_flag)
: MT_ExtractorClass(m,topo_flag)
{
   MT_Message((char *)"",(char *)"MT_LocalExtractor Constructor");

   /* allocate the additional fields */
   A_pos = 
         (unsigned int *) calloc ( mt->MT_ArcNum()+1, sizeof(unsigned int) );
   A_neg =
         (unsigned int *) calloc ( mt->MT_ArcNum()+1, sizeof(unsigned int) );
   if ( !(A_pos && A_neg) )
   {
      MT_FatalError((char *)"Allocation failed",(char *)"MT_LocalExtractor Constructor");
   }

   /* set an initial state */
   MT_ResetAllStat();
   MT_IncludeNode(mt->MT_Root());
   MT_ExtractionCompleted();
}

MT_LocalExtractorClass :: ~MT_LocalExtractorClass(void)
{
   MT_Message((char *)"",(char *)"MT_LocalExtractor Destructor");
   /* deallocate the additional fields */
   delete (A_pos);
   delete (A_neg);
}

/* ------------------------------------------------------------------------ */
