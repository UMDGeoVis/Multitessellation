/* ------------------------------ mt_out.c ------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mt_out.h"
//#include "stdafx.h"


/* ----------------------------------------------------------------------- */
/*            FUNCTIONS FOR WRITING THE EXTRACTED TESSELATION              */
/* ----------------------------------------------------------------------- */

/*
Markers for vertices taking part in the extracted tesselation.
Such vertices are the only ones to be reported.
*/

#define IS_USED -1
#define IS_NOT_USED 0

/* ----------------------------------------------------------------------- */
/*                           Auxiliary functions                           */
/* ----------------------------------------------------------------------- */

static int MT_WriteRawAux(FILE *fd, MT_Extractor e, int flags,
                          MT_INDEX * tiles, MT_INDEX count)
{
   MT_INDEX t;          /* variable for tile */
   MT_INDEX * v;        /* for the vertices of t */
   float * f;           /* for the coordinates of v[i] */
   int i,j,k;
   int i1;
   int t_dim, v_dim;    /* number of vertices per tile and
                           number of coordinates per vertex */
   int * Tbit_vector;   /* for the extracted tiles */
   MT_INDEX t1, v1;
   int is_adj;

   t_dim = e->MT_TheMT()->MT_NumTileVertices();
   v_dim = e->MT_TheMT()->MT_VertexDim();

   if (flags & MT_ADJ_FLAG)
   {
     Tbit_vector = (int *)
     calloc (e->MT_TheMT()->MT_TileNum()+1, sizeof(int));
     if (!Tbit_vector)
     {
        MT_Warning("Allocation failed","MT_WriteIndexed");
        return 0;
     }
     for (j=0;j<count;j++)
     {
        Tbit_vector[tiles[j]] = j+1;
     }
   }
    
   fprintf(fd,"%d\n",count);
   for (j=0;j<count;j++)
   {
      t = tiles[j];
      e->MT_TheMT()->MT_TileVertices(t,&v);
      for (i=0;i<t_dim;i++)
      {   
          e->MT_TheMT()->MT_VertexCoords(v[i],&f);
          for (k=0;k<v_dim;k++)
          {
             if (k<(v_dim-1)) fprintf(fd,"%f ",f[k]);
             else fprintf(fd,"%f\n",f[k]);
          }
      }
      if (flags & MT_ADJ_FLAG)
      for (i=0;i<t_dim;i++)
      {
#ifdef VERSIONE_FEB11
          is_adj = e->MT_TileAdj(t,t,v[i],&t1,&i1);
          if ( (!is_adj) ||
               (Tbit_vector[t1] == IS_NOT_USED) ) fprintf(fd,"%d ",-1);
          else fprintf(fd,"%d ",Tbit_vector[t1]-1);
#else
          t1 = e->MT_TileAdj(t,i);
          if ( (t1==MT_NULL_INDEX) ||
               (Tbit_vector[t1] == IS_NOT_USED) ) fprintf(fd,"%d ",-1);
          else fprintf(fd,"%d ",Tbit_vector[t1]-1);
#endif
      }
      fprintf(fd,"\n");
   }

   if (flags & MT_ADJ_FLAG) free(Tbit_vector);

   return 1;
}

/* ----------------------------------------------------------------------- */

static int MT_WriteIndexedAux(FILE * fd, MT_Extractor e, int flags,
                              MT_INDEX * tiles, MT_INDEX count)
{
   MT_INDEX t;          /* variable for tile */
   MT_INDEX * v;        /* for the vertices of t */
   float * f;           /* for the coordinates of v[i] */
   int i,j,k;
   int t_dim, v_dim;    /* number of vertices per tile and
                           number of coordinates per vertex */
   int * Vbit_vector;   /* for the vertices of the extracted tiles */

   int * Tbit_vector;   /* for the extracted tiles */
   MT_INDEX t1, v1;
   int is_adj;
   int i1;

   MT_INDEX v_count = 0; /* counter of extracted vertices */
   MT_INDEX ind = 0;     /* progressive number given to vertices */
   
   Vbit_vector = (int *) 
            calloc (e->MT_TheMT()->MT_VertexNum()+1, sizeof(int));
   if (!Vbit_vector)
   {
      MT_Warning("Allocation failed","MT_WriteIndexed");
      return 0;
   }

   t_dim = e->MT_TheMT()->MT_TileDim()+1;
   v_dim = e->MT_TheMT()->MT_VertexDim();

   if (flags & MT_ADJ_FLAG)
   {
     Tbit_vector = (int *)
     calloc (e->MT_TheMT()->MT_TileNum()+1, sizeof(int));
     if (!Tbit_vector)
     {
        MT_Warning("Allocation failed","MT_WriteIndexed");
        return 0;
     }
   }

   /* count elements of bit vector */
   for (j=0;j<count;j++)
   {
      t = tiles[j];
/********printf("%d\n",t);*******/
      e->MT_TheMT()->MT_TileVertices(t,&v);
      for (i=0;i<t_dim;i++)
      {
        if (!Vbit_vector[v[i]]) 
        {  Vbit_vector[v[i]] = IS_USED; v_count++;  }
      }
     if (flags & MT_ADJ_FLAG)  {  Tbit_vector[t] = j+1;  }
   }

   fprintf(fd,"OFF\n");

   /* write vertices */
   fprintf(fd,"%d %d 0\n",v_count,count);
   for (j=0;j<count;j++)
   {
      t = tiles[j];
      e->MT_TheMT()->MT_TileVertices(t,&v);
      for (i=0;i<t_dim;i++)
      {
        if (Vbit_vector[v[i]]==IS_USED)
        {
           Vbit_vector[v[i]] = ++ind;
           e->MT_TheMT()->MT_VertexCoords(v[i],&f);
           for (k=0;k<v_dim;k++)
           {
              if (k<(v_dim-1)) fprintf(fd,"%f ",f[k]);
              else fprintf(fd,"%f\n",f[k]);
           }
        }
      }
   }

   /* write tiles */
//   fprintf(fd,"%d\n",count);
   for(j=0;j<count;j++)
   {
      t = tiles[j];
      e->MT_TheMT()->MT_TileVertices(t,&v);
      fprintf(fd,"%d  ",t_dim);
      for (i=0;i<t_dim;i++)
      {  if (i<t_dim-1) 
             fprintf(fd,"%d ", Vbit_vector[v[i]]-1);
         else
             fprintf(fd,"%d\n", Vbit_vector[v[i]]-1);
      }
      /* write adjacent tiles if required */
      if (flags & MT_ADJ_FLAG)
      {
         for (i=0;i<t_dim;i++)
         {
           t1 = e->MT_TileAdj(t,i);
           if ( (t1==MT_NULL_INDEX) ||
                (Tbit_vector[t1] == IS_NOT_USED) ) fprintf(fd,"%d ",-1);
           else fprintf(fd,"%d ",Tbit_vector[t1]-1);
         }
         fprintf(fd,"\n");
      }
   }
   free(Vbit_vector);
   if (flags & MT_ADJ_FLAG) free(Tbit_vector);

   return 1;
}

/* ----------------------------------------------------------------------- */
/*                           Funzioni esportate                            */
/* ----------------------------------------------------------------------- */

int MT_WriteRaw(FILE * fd, MT_Extractor e, int flags)
{
   MT_INDEX count;   /* number of extracted tiles */
   MT_INDEX * tiles; /* array of extracted tiles */

   count = e->MT_ExtractedTiles(&tiles);
   if (count)  return (MT_WriteRawAux (fd,e,flags,tiles,count) );
   else return 1;
}

int MT_WriteIndexed(FILE * fd, MT_Extractor e, int flags)
{
   MT_INDEX count;   /* number of extracted tiles */
   MT_INDEX * tiles; /* array of extracted tiles */

   count = e->MT_ExtractedTiles(&tiles);
   if (count)  return (MT_WriteIndexedAux (fd,e,flags,tiles,count) );
   else return 1;
}

int MT_WriteAllRaw(FILE * fd, MT_Extractor e, int flags)
{
   MT_INDEX count;   /* number of extracted tiles */
   MT_INDEX * tiles; /* array of extracted tiles */
   
   count = e->MT_AllExtractedTiles(&tiles);
   if (count)  return (MT_WriteRawAux (fd,e,flags,tiles,count) );
   else return 1;
}

int MT_WriteAllIndexed(FILE * fd, MT_Extractor e, int flags)
{
   MT_INDEX count;   /* number of extracted tiles */
   MT_INDEX * tiles; /* array of extracted tiles */    

   count = e->MT_AllExtractedTiles(&tiles);
   if (count)  return (MT_WriteIndexedAux (fd,e,flags,tiles,count) );
   else return 1;
}
