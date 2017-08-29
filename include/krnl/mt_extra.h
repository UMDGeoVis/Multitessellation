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

/* ----------------------------- mt_extra.h ------------------------------- */

#ifndef MT_EXTRA_INCLUDED
#define MT_EXTRA_INCLUDED

#include "mt_multi.h"
#include "mt_list.h"
#include "mt_hash.h"
#include "mt_cond.h"

/* ------------------------------------------------------------------------ */
/*               EXTRACTION  OF TESSELATIONS FROM AN MT                     */
/* ------------------------------------------------------------------------ */

/*
This file defines classes for the extractors. An extractor is the engine to
extract tesselations at a certain resolution form an MT.
The interface of a generic extractor is provided by the abstract class
MT_ExtractorClass.

Three types of extractor are available, which correspond to three subclasses
of MT_ExtractorClass:

- Global static extractor (class MT_StaticExtractorClass): extract tesselations
  covering the whole geometric object encoded in the MT, using an algorithm
  that always starts traversing the MT from scratch.

- Global Dynamic extractor (class MT_DynamicExtractorClass): extract tesselations
  covering the whole geometric object encoded in the MT, using an algorithm
  that updates the previously extracted tesselation.
  This extractor requires a larger auxiliary structures but is faster
  if the new tesselation to be extracted is close to the previous one
  (e.g., for animated sequences).

- Local static extractor (class MT_LocalExtractorClass): extract tesselations
  covering only the portion of geometric object which interferes with a 
  focus set in space, using an algorithm that always starts traversing the 
  MT from scratch.
  Only the parts of the MT which interfere with the focus set are traversed,
  thus the algorithm is faster than the global static one (especially for
  small focus sets).
*/

/* ------------------------------------------------------------------------ */
/*                                 MACROS                                   */
/* ------------------------------------------------------------------------ */

/*
Possible values of the flag for enabling/disabling generation of adjacency
information in the extracted tesselation (this is established when an
extractor is created).
*/
#define MT_ADJ_DISABLE 0
#define MT_ADJ_ENABLE  1

/* ------------------------------------------------------------------------ */
/*                        AUXILIARY DATA STRUCTURES                         */
/* ------------------------------------------------------------------------ */

/*
Structure to keep the topology of the extracted tesselation.
*/

typedef class MT_TopoClass * MT_Topo;

class MT_TopoClass
{
  protected:

  /*
  Array containing the indexes of adjacent tiles. The i-th position 
  stores the adiacent tile along the facet opposite to the i-th vertex.
  */
  MT_INDEX * T_adj;  

  /*
  Create an instance where array T_adj contains dim positions
  */
  MT_TopoClass (int dim);

  ~MT_TopoClass(void);
  
  friend class MT_ExtractorClass;
};
                                                      
/*
An array of elements of type MT_Topo will be used to store, for each tile, 
a pointer to the MT_TopoClass structure that contains the indexes of 
its adjacent tiles.
*/

/* ------------------------------------------------------------------------ */
/*                         GENERIC EXTRACTOR                                */
/* ------------------------------------------------------------------------ */

typedef class MT_ExtractorClass * MT_Extractor;

class MT_ExtractorClass
{
  
/* ------------------------------------------------------------------------ */
/*           Info about the underlying Multi-Tesselation                    */
/* ------------------------------------------------------------------------ */

   protected:

   /*
   Multi-tesselation on which the extractor operates.
   */
   MT_MultiTesselation mt;
   
   /* 
   Number of vertices of each tile in the MT (it is convenient
   to keep it explicitly even if redundant).
   */
   int mt_dim;

/* ------------------------------------------------------------------------ */
/*                    Current extraction parameters                         */
/* ------------------------------------------------------------------------ */

   /*
   Condition to decide whether a tile is refined enough.
   */
   MT_Cond resol_cnd; 
   
   /*
   Condition used to decide whether a tile lies in the area of interest
   defined by the focus set.
   */  
   MT_Cond place_cnd;

/* ------------------------------------------------------------------------ */
/*                 Current state of the MT traversal                        */
/* ------------------------------------------------------------------------ */

   /*
   Array indexed on nodes, for each nodes storing a marker whether the node
   is in the set of nodes before the current front.
   */
   unsigned int * N_set; 

   /*
   Counter of how many extractions have been made. Static extractors 
   use it as a marker in N_set for nodes that are in the set.
   */
   unsigned int extr_count;

   /*
   List of arcs of the current front.
   Tiles labelling the front arcs are the same as the tiles forming
   the current tesselation.
   Source nodes of the front arcs are the nodes that contribute to the
   current tesselation.
   */
   MT_List A_front; 

/* ------------------------------------------------------------------------ */
/*                Information about the current tesselation                 */
/* ------------------------------------------------------------------------ */

   /*
   Array of extracted tiles lying in the area of interest (active tiles),
   and array of extracted tiles lying outside the area of interest 
   (non-active tiles).
   */
   MT_INDEX * T_good; 
   MT_INDEX * T_bad;  
                         
   /*
   One array is maintained that contains first the tiles lying in the 
   area of interest and then those lying outside the area of interest.
   T_good points to the beginning of such array and
   T_bad points to the position of the first non-active tile.
   */
     
   /*
   Number of active extracted tiles, and total number of extracted tiles.
   The number of non-active extracted tiles is num_tiles-num_good.
   */
   int num_good; 
   int num_tiles;
   
   /* 
   Flag ==1 if the tile array is up-to-date, 0 otherwise. 
   The array is updated at the end of extraction.
   */
   short int tiles_ok;

/* ------------------------------------------------------------------------ */
/*                Information about generation of topology                  */
/* ------------------------------------------------------------------------ */

   /*
   Flag if topology generation enabled.
   */
   int topo_enabled;
   
   /*
   Array indexed on tiles, where each tile t points to the object of type
   MT_TopoClass storing its adjacent tiles (if t belongs to the current 
   tesselation), or points to NULL (otherwise). If topology is enabled,
   such array is updated during extraction and maintains the adjacency 
   information for the extracted tesselation. Otherwise, it is not used.
   */
   MT_Topo * topo; 

   /*
   Hash table used for adjacency reconstruction.
   */
   MT_HashTable hash_table; 

/* ----------------------------------------------------------------------- */
/*                          Statistical information                        */
/* ----------------------------------------------------------------------- */

  /*
  Flag if statistics are enabled.
  */
  int stat_enabled;

  /*
  Statistic information.
  */
  MT_INDEX n_visited; /* number of nodes visited by the extraction algo
                         and swept across the front */
  MT_INDEX n_inset;   /* number of nodes lying before the current front */
  MT_INDEX n_tested;  /* number of nodes tested during contraction stage
                         (only for dynamic extractor) */
  MT_INDEX a_visited; /* number of arcs visited by the extraction algo
                         and swept by the front */
  MT_INDEX a_infront; /* number of arcs in the current front */
  MT_INDEX a_tested;  /* number of arcs tested for moving the front */
  MT_INDEX t_visited; /* number of tiles visited by the extraction algo
                         and swept across the front */
  MT_INDEX t_tested;  /* number of tiles tested against the extraction
                         conditions */

/* ------------------------------------------------------------------------ */
/*                           EXPORTED FUNCTIONS                             */
/* ------------------------------------------------------------------------ */

   public:

/* ------------------------------------------------------------------------ */
/*                            The corresponding MT                          */
/* ------------------------------------------------------------------------ */
 
   inline MT_MultiTesselation MT_TheMT(void) {  return (mt);  }

/* ------------------------------------------------------------------------ */
/*             Setting and returning the extraction conditions              */
/* ------------------------------------------------------------------------ */

   /* 
   Associate a resolution filter condition with this extractor. Subsequent 
   modifications of the condition are visible to the extractor.
   If flt==NULL, then no condition is associated, and all tiles are feasible.
   Return 1 if successful, 0 otherwise.
   */ 
   int MT_SetFilter(MT_Cond flt);

   /* 
   Associate a focus condition with this extractor. Subsequent 
   modifications of the condition are visible to the extractor.
   If fcs==NULL, then no condition is associated, and all tiles are active.
   Return 1 if successful, 0 otherwise.
   */
   int MT_SetFocus(MT_Cond fcs);

   /* 
   Return the conditions associated with an extractor.
   */
   inline MT_Cond MT_TheFilter (void)   {  return (resol_cnd);  }
   inline MT_Cond MT_TheFocus(void)   {  return (place_cnd);  }
   
/* ------------------------------------------------------------------------ */
/*                       Extraction of a tesselation                        */
/* ------------------------------------------------------------------------ */

   /* 
   Update the current front (and thus the current tesselation) according to
   the current filter and focus conditions. Return the number of
   extracted tiles which are active w.r.t. the focus.
   */
   virtual MT_INDEX MT_ExtractTesselation( void );

/* ------------------------------------------------------------------------ */
/*                          Reading extracted tiles                         */
/* ------------------------------------------------------------------------ */

   /*
   Return an array containing the extracted active tiles. Such array
   must not be allocated nor deallocated by the user. Return the array size. 
   */
   MT_INDEX MT_ExtractedTiles(MT_INDEX ** t_arr);
     
   /*
   Return the number of extracted active tiles.
   */
   inline MT_INDEX MT_ExtractedTilesNum(void) { return ( num_good ); }

   /*
   Return two arrays containing the extracted active and non-active 
   tiles, respectively, and their sizes. 
   Such arrays must not be allocated nor deallocated 
   by the user. Return the total size of the two arrays.
   */
   MT_INDEX MT_AllExtractedTiles(MT_INDEX ** t_arr1, MT_INDEX * size1,
                                 MT_INDEX ** t_arr2, MT_INDEX * size2);
   /* 
   Return one array containing all the extracted tiles, and its size.
   */
   MT_INDEX MT_AllExtractedTiles(MT_INDEX ** t_arr);

   /*
   Return the total number of extracted tiles.
   */
   inline MT_INDEX MT_AllExtractedTilesNum(void)  {  return ( num_tiles );  }

/* ------------------------------------------------------------------------ */
/*                  Topology of the extracted tesselation                   */
/* ------------------------------------------------------------------------ */

  /*
  Check if topology generation enabled in this extractor.
  */
  inline int MT_AdjGenerationEnabled(void)  {  return (topo_enabled);  }

  /* 
  Return the i-th tile adjacent to tile t, i.e., the one adjacent to t along 
  the facet opposite to the i-th vertex of t. It must be 0<=0<mt_dim.
  Return MT_NULL_INDEX if the adjacent tile does not exist, or if topology
  generation is disabled.
  */
  MT_INDEX MT_TileAdj(MT_INDEX t, int i);

  /*
  Return the array of tiles adjacent to tile t. Return NULL if topology
  generation is disabled.
  */
  MT_INDEX * MT_TileAdjs(MT_INDEX t);

  /*
  As above, but make a copy of the array onto array ta, which
  must be long enough.
  */
  void MT_CopyTileAdjs(MT_INDEX t, MT_INDEX * ta);

/* ----------------------------------------------------------------------- */
/*                         Statistical information                         */
/* ----------------------------------------------------------------------- */

  public:
  
  /* 
  Reset statistics describing the algorithm (tested and visited).
  */
  void MT_ResetAlgoStat(void);
  
  /*
  Reset all statistics.
  */
  void MT_ResetAllStat(void);

  /*
  Enable statistics, compute n_inset and a_infront based on the current
  state of this extractor.
  */
  void MT_EnableStat(void);
  
  /*
  Disable statistics.
  */
  void MT_DisableStat(void);
  
  /*
  Check if statistics are enabled in the current state.
  */
  inline int MT_StatEnabled(void)  {  return (stat_enabled);  }

  /*
  Return the values of the statistics (if enabled). Otherwise return 0.
  */
  inline MT_INDEX MT_VisitedNodes(void) {  return (n_visited);  }
  inline MT_INDEX MT_UsedNodes(void)    {  return (n_inset);  }
  inline MT_INDEX MT_TestedNodes(void)  {  return (n_tested);  }
  inline MT_INDEX MT_VisitedArcs(void)  {  return (a_visited);  }
  inline MT_INDEX MT_UsedArcs(void)     {  return (a_infront);  }
  inline MT_INDEX MT_TestedArcs(void)   {  return (a_tested);  }
  inline MT_INDEX MT_VisitedTiles(void) {  return (t_visited);  }
  inline MT_INDEX MT_TestedTiles(void)  {  return (t_tested);  }

/***************************** TAPULLO PER LAVORO CAD ***********************/

  protected:  
  
  /* 
  Maximum number of tiles in an extracted tesselation. The extraction 
  algorithm stops as soon as it reaches this number.
  By default, it is so big tha any extracted tesselation is smaller.
  */
  MT_INDEX max_tile_num;
  
  public:

  /*
  Set and get the maximum number of extracted tiles.
  */
  inline void MT_SetUpperBound(MT_INDEX n)  {  max_tile_num = n;  }
  inline MT_INDEX MT_TheUpperBound(void)  {  return (max_tile_num);  }

/* ------------------------------------------------------------------------ */
/*                          Auxiliary functions                             */
/* ------------------------------------------------------------------------ */

   public:
   
   /*
   Test a tile against the focus set 
   */
   inline int MT_FocusTile(MT_INDEX t, int flag)
   {   return ( (!place_cnd) || place_cnd->MT_EvalCond(mt,t,flag) );   }
   
   /*
   Test a tile against the resolution filter
   */
   inline int MT_FilterTile(MT_INDEX t, int flag)
   {
      return ( (mt->MT_TileRemover(t) == mt->MT_Drain())
               ||
               (!resol_cnd) || resol_cnd->MT_EvalCond(mt,t,flag) );
   }            

   protected:
   
   /* 
   Test whether a node is in the set of nodes before the current front.
   */
   virtual int MT_IsInSet(MT_INDEX n);

   /*
   Retrieve extracted tiles from the front arcs.
   */
   inline MT_INDEX MT_FirstFrontTile(void) 
   {  return ( mt->MT_FirstArcTile(A_front->MT_FirstElem()) );  }
   MT_INDEX MT_NextFrontTile (MT_INDEX t);

   /*
   Add to / remove from the extracted tesselation the tiles of arc a.
   Update adjacency links if required.
   */
   virtual int MT_AddTiles(MT_INDEX a);
   virtual int MT_RemTiles(MT_INDEX a);
 
   /*
   Set t2 as the j-th adjacent tile to t1. Auxiliary function used 
   inside MT_SetAdjLink.
   */
   int OneWaySetAdjLink(MT_INDEX t1, MT_INDEX t2, int j);

   /*
   If t1, t2 are both not null, set adjacency link between t1 and t2,
   such that t2 is the i1-th adjacent tile of t1, and conversely.
   If one (e.g., t1) is null, set the i2-th adjacent tile of t2 to null.
   Create adjacency record for t1 or t2 if it does not exist.
   */
   int MT_SetAdjLink(MT_INDEX t1, MT_INDEX t2, int i1, int i2);
   
   /*
   Remove adjacency links stored for tile t, delete adjacency record.
   */
   int MT_RemAdjLinks(MT_INDEX t);

   /*
   Mark node n as belonging to the set of nodes before the front.
   */
   virtual void MT_AddToSet(MT_INDEX n);
   
   /*
   Sweep the front after a node (and after all its parents that are not yet
   above the front). Add such nodes to the set of nodes above the front, 
   update the current front and tesselation.
   Return 1 if OK, 0 if some error has occurred.
   */
   virtual int MT_IncludeNode (MT_INDEX n);

   /*
   Advance the current front (and expand the set of nodes above the 
   front, and update the current tesselation) in order to satisfy
   the extraction parameters (resolution filter and focus set).
   */
   int MT_ExpandExtractor (void);
   
   /* 
   Put the extractor in the initial state (current front just below
   the root).
   */
   int MT_ResetExtractor (void);

   /*
   Update the array of extracted tiles at the end of DAG traversal.
   */
   virtual int MT_ExtractionCompleted(void);
  
   /*
   Create an extractor, allocate the internal structures.
   If topo_flag == 1, enable generation of topological information.
   */
   MT_ExtractorClass(MT_MultiTesselation m, int topo_flag = MT_ADJ_DISABLE);

   public:

   /*
   Destroy an extractor, deallocate the internal structures.
   */
   virtual ~MT_ExtractorClass(void);
};

/* ------------------------------------------------------------------------ */
/*                        GLOBAL STATIC EXTRACTOR                           */
/* ------------------------------------------------------------------------ */

typedef class MT_StaticExtractorClass * MT_StaticExtractor;

class MT_StaticExtractorClass : public MT_ExtractorClass
{

/* ------------------------------------------------------------------------ */
/*                        creation and deletion                             */
/* ------------------------------------------------------------------------ */

   public:

   /*
   Create extractor for a given MT and put it in initial state:
   the current front is the one formed by the out-arcs of the root.
   */
   MT_StaticExtractorClass (MT_MultiTesselation m,
                            int topo_flag = MT_ADJ_DISABLE);

   /* 
   Destroy extractor, deallocate its internal variables.
   */
   ~MT_StaticExtractorClass (void);
   
};

/* ------------------------------------------------------------------------ */
/*                       GLOBAL DYNAMIC EXTRACTOR                           */
/* ------------------------------------------------------------------------ */

typedef class MT_DynamicExtractorClass * MT_DynamicExtractor;

class MT_DynamicExtractorClass : public MT_ExtractorClass
{
   protected:
   
/*------------------------------------------------------------------------ */
/*      Additional information on the current state of DAG traversal       */
/*------------------------------------------------------------------------ */

   /*
   Array N_set stores a counter used to detect whether a node is in the
   set of nodes before the front and whether it is a free node (a leaf).
   For nodes not in the set, the counter is == 0.
   For nodes in the set, it is == 1 plus the number of children in the set.
   Note that a node not in the set cannot have children in the set.
   */

   /*
   List of leaves (free nodes) of the current sub-DAG. A node is a leaf
   if its counter in N_set is == 1.
   */
   MT_List N_free;

   public:

/* ------------------------------------------------------------------------ */
/*                         creation and deletion                            */
/* ------------------------------------------------------------------------ */

   /*
   Create extractor for a given MT and put it in initial state:
   the current front is the one formed by the out-arcs of the root.
   */
   MT_DynamicExtractorClass (MT_MultiTesselation m, 
                             int topo_flag = MT_ADJ_DISABLE);

   /* 
   Destroy extractor, deallocate its internal variables.
   */
   ~MT_DynamicExtractorClass (void);

/* ------------------------------------------------------------------------ */
/*                Update of the extracted tesselation                       */
/* ------------------------------------------------------------------------ */

   MT_INDEX MT_ExtractTesselation( void ); 
   
/* ------------------------------------------------------------------------ */
/*                          Auxiliary functions                             */
/* ------------------------------------------------------------------------ */

   protected:
 
   /*
   Increment the counter of n in N_set and return the incremented counter.
   */
   inline int MT_IncrCount(MT_INDEX n)  { return ( ++N_set[n] ); }

   /* 
   Decrement the counter of n in N_set and return the decremented counter.
   */
   inline int MT_DecrCount(MT_INDEX n)  { return ( --N_set[n] ); }

   /*
   Sweep the current front before a node (which must be a leaf), remove
   the leaf from the set of nodes above the front, update the front
   and the extracted tesselation, update the set of leaves.
   Return 1 if OK, 0 if some error has occurred.
   */
   virtual int MT_ExcludeNode (MT_INDEX n);

   /*
   Move the current front backwards (shrink the set of nodes above the
   front and coarsen the current tesselation) as long as the
   extraction parameters allow it.
   */
   int MT_ContractExtractor (void);

   /*
   Redefined functions on the set of nodes before the front.
   A node is in the set if its counter in N_set is != 0.
   */
   int MT_IsInSet(MT_INDEX n);

   /*
   Mark a node as inserted in / removed from the set of nodes above the
   current front. Update the list of leaves accordingly.
   */
   void MT_AddToSet(MT_INDEX n);
   void MT_RemFromSet(MT_INDEX n);
         
};

/* ------------------------------------------------------------------------ */
/*                       LOCAL STATIC EXTRACTOR                             */
/* ------------------------------------------------------------------------ */

typedef class MT_LocalExtractorClass * MT_LocalExtractor;

class MT_LocalExtractorClass : public MT_ExtractorClass
{

   protected:
   
   /*
   The following two arrays are used to speed-up the algorithm:
   they store results of tests MT_FocusArc(a) already performed.
   Both arrays are indexed on arcs. For each arc they store a marker 
   whether MT_FocusArc(a) has been evaluated with a positive / negative
   result, respectively.
   A_pos e A_neg use for arcs the same marking value (extr_count) used for 
   nodes in N_set.
   */
   unsigned int * A_pos;
   unsigned int * A_neg;

   public:
   
/* ------------------------------------------------------------------------ */
/*                        creation and deletion                             */
/* ------------------------------------------------------------------------ */

   /*
   Create extractor for a given MT and put it in initial state:
   the current front is the one formed by the out-arcs of the root.
   */
   MT_LocalExtractorClass (MT_MultiTesselation m,
                           int topo_flag = MT_ADJ_DISABLE);

   /* 
   Destroy extractor, deallocate its internal variables.
   */
   ~MT_LocalExtractorClass (void);

/* ------------------------------------------------------------------------ */
/*                          Auxiliary functions                             */
/* ------------------------------------------------------------------------ */
   
   protected:

   /*
   Function redefined from superclass MT_ExtractorClass.
   */
   virtual int MT_IncludeNode (MT_INDEX n);
   
   /* 
   Test whether at least one tile labelling the arc is active.
   */
   int MT_FocusArc(MT_INDEX a);
   
   /* 
   Check whether MT_FocusArc has already been tested with a 
   positive / negative result on an arc.
   */
   inline int MT_HintYes(MT_INDEX a) {  return ( A_pos[a] == extr_count );  }
   inline int MT_HintNot(MT_INDEX a) {  return ( A_neg[a] == extr_count );  }
   
};

/* ------------------------------------------------------------------------ */

#endif   /* MT_EXTRA_INCLUDED */
