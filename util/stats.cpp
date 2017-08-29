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

/* ----------------------------- stats.c ---------------------------------- */

#include "mt_multi.h"

/* ----------------------------------------------------------------------- */
/*                         STATISTICS ON AN MT                             */
/* ----------------------------------------------------------------------- */

/* 
Read MT from file, write statistics to standard output.
- number of nodes, arcs, tiles, vertices
- number of tiles and vertices of the tesselation at minimum resolution
- number of tiles and vertices of the tesselation at maximum resolution
- ratio between total number of tiles and number of tiles at maximum
  resolution
- maximum length of a root-to-drain path
- number of children of the root node
- maximum and mean number of children of an internal node
- maximum and mean number of new tiles of an internal node
- number of parents of the drain node
- maximum and mean number of parents of an internal node
- maximum and mean number of old tiles of an internal node
*/

/* ----------------------------------------------------------------------- */
/*                         Auxiliary class                                 */
/* ----------------------------------------------------------------------- */

/*
This class allows to read just the header of an MT file and to find
out the dimensions of the MT tiles and of the embedding space.
*/

class CheckDimensionsClass : public MT_MultiTesselationClass
{
  protected:
  
  inline int MT_ReadBody(FILE * fd, int file_encoding) {  return 1; }
  int MT_ReadClassAndParams(FILE * fd);
  
  public:
  
  /*
  Constructor (just a dummy initialization of the superclass).
  */
  inline CheckDimensionsClass(void): MT_MultiTesselationClass(1,1) {}
};

int CheckDimensionsClass :: MT_ReadClassAndParams(FILE * fd)
{ 
  int vd, td;
  if (!MT_SearchKeyword(fd, MT_ClassKeyword())) return 0;
  if (fscanf(fd, "%d %d", &vd, &td)==2)
  {   V_Dim = vd;  T_Dim = td;  }
  return 1;
}

/* ----------------------------------------------------------------------- */
/*                           global variables                              */
/* ----------------------------------------------------------------------- */

char fileName[255]; /* file containing the MT */
MT_MultiTesselation myMT;

/* ----------------------------------------------------------------------- */
/*                         auxiliary functions                             */
/* ----------------------------------------------------------------------- */

/* termination due to error */
void fatalError(char *message)
{
    fprintf(stderr, "Fatal error: %s\n", message);
    exit(1);
}

/* management of command line options */
void setOptions(int argc, char * argv[])
{
   if (argc==1) /* no command line options */
   {
     fprintf(stderr,"Usage: %s XXX\n",argv[0]);
     fprintf(stderr,"Print statistics on the MT contained in file XXX\n"); 
     exit(1);
   }
   strcpy(fileName,argv[1]); /* file name to open */
}

/* load MT from file fileName */
void loadMT(void)
{
   FILE *fd;
   CheckDimensionsClass * chk;
   
   printf("Loading MT from file %s...\n",fileName);
   fd = fopen(fileName,"r");
   if(!fd)
       fatalError("Cannot open file");    

   /* Use chk to read the MT dimensions, close and re-open input file */
   chk = new CheckDimensionsClass();
   chk->MT_Read(fd);
   fclose(fd);
   fd = fopen(fileName,"r");

   /* Create mt with the correct dimensions just read */
   myMT = new MT_MultiTesselationClass(chk->MT_VertexDim(), chk->MT_TileDim());

   if (myMT->MT_Read(fd) != 1)
       fatalError("Cannot read MT from given file");
   printf("...MT loaded\n");
   delete chk;
}

/* compute the maximum lenght of a path from the root to any node */
void maxPaths(MT_INDEX nn, unsigned int * arr)
{
  MT_INDEX a;
  unsigned int x;
  
  for (a = myMT->MT_FirstOutArc(nn);
       a != MT_NULL_INDEX;
       a = myMT->MT_NextOutArc(nn,a))
  {
    x = arr[nn] + 1;
    if (x > arr[myMT->MT_ArcDest(a)]) 
    {
       arr[myMT->MT_ArcDest(a)] = x;
       maxPaths(myMT->MT_ArcDest(a),arr);
    }
  }
}

void statMT(void)
{
  /* auxiliary array */
  unsigned int * aux_arr;
  /* counters */
  int i,j,k,h,m;
  /* variables for nodes, arcs, tiles, vertices */
  MT_INDEX n, a, t, * v;

  printf("===MT STATISTICS===\n");
  /*  number of nodes, arcs, tiles, vertices */
  printf("nodes = %d (number of nodes in the DAG)\n", myMT->MT_NodeNum());
  printf("arcs = %d (number of arcs in the DAG)\n", myMT->MT_ArcNum());
  printf("tiles = %d (total number of tiles)\n", myMT->MT_TileNum());
  printf("verts = %d (total number of vertices)\n", myMT->MT_VertexNum());

  /* now aux_arr will be used as a bit vector if a vertex belongs to the
     tesselation */
  aux_arr = (unsigned int*) 
                  calloc (myMT->MT_VertexNum()+1,sizeof(unsigned int));
  
  /* number of tiles and of vertices of the tesselation a minimum
     resolution and number of children of the root */
  n = myMT->MT_Root();
  k = h = 0; /* count outgoing arcs and new tiles */
  m = 0; /* count vertices */
  for(a = myMT->MT_FirstOutArc(n); 
      a != MT_NULL_INDEX; 
      a = myMT->MT_NextOutArc(n,a))
  {
    k += 1; /* count outgoing arcs */
    h += myMT->MT_NumArcTiles(a); /* count tiles */
    for (t = myMT->MT_FirstArcTile(a);
         t != MT_NULL_INDEX;
         t = myMT->MT_NextArcTile(a,t))
    {
      v = myMT->MT_TileVertices(t);
      for (i=0; i<myMT->MT_NumTileVertices(); i++)
      {
         if (aux_arr[v[i]] == 0) { aux_arr[v[i]] = 1; m++; } 
      }
    }
  }
  printf("root-out-arcs = %d (arcs outgoing from the root node)\n", k);
  printf("root-tiles = %d (tiles at mimimum resolution)\n", h);
  printf("root-verts = %d (vertices at mimimum resolution)\n", m);
  
  /* number of tiles and of vertices of the tesselation 
    at maximum resolution, and number of parents of the drain node */
  n = myMT->MT_Drain();
  k = h = 0; /* count incoming arcs and old tiles */
  m = 0; /* count vertices */
  for(a = myMT->MT_FirstInArc(n); 
      a != MT_NULL_INDEX; 
      a = myMT->MT_NextInArc(n,a))
  {
    k += 1; /* count incoming arcs */
    h += myMT->MT_NumArcTiles(a); /* count tiles */
    for (t = myMT->MT_FirstArcTile(a);
         t != MT_NULL_INDEX;
         t = myMT->MT_NextArcTile(a,t))
    {
      v = myMT->MT_TileVertices(t);
      for (i=0; i<myMT->MT_NumTileVertices(); i++)
      {
         if (aux_arr[v[i]] != 2) { aux_arr[v[i]] = 2; m++; } 
      }
    }
  }
  printf("drain-in-arcs = %d (arcs entering the drain node)\n", k);
  printf("drain-tiles = %d (tiles at maximum resolution)\n", h);
  printf("drain-verts = %d (vertices at maximum resolution)\n", m);

  /* ratio between total number of tiles and number of tiles at
     maximum resolution */
  printf("tiles/drain-tiles = %f (compression factor)\n",
         (float)(myMT->MT_TileNum()) / (float)(h));
         
  /* now aux_arr will be used to contain the maximum lenght of a path
     from the root to each node */
  free(aux_arr);
  aux_arr = (unsigned int*) 
                  calloc (myMT->MT_NodeNum()+1,sizeof(unsigned int));
  
  /* maximum lenght of a root-to-drain path */
  maxPaths(myMT->MT_Root(),aux_arr);
  printf("height = %d (maximum path lenght)\n",aux_arr[myMT->MT_Drain()]);
  
  /* maximum and mean number of children of an internal node,
     maximum and mean number of new tiles of an internal node */
  h = k = 0; /* maximum and mean number children */
  m = j = 0; /* maximum and mean number of new tiles */
  for (n = 2; n<myMT->MT_NodeNum(); n++)
  {
    if (myMT->MT_NumOutArcs(n) > h) h = myMT->MT_NumOutArcs(n);
    k += myMT->MT_NumOutArcs(n);
    i = 0; /* count tiles of node n */
    for (a = myMT->MT_FirstOutArc(n);
         a != MT_NULL_INDEX;
         a = myMT->MT_NextOutArc(n,a))
    {  i += myMT->MT_NumArcTiles(a);  }
    if (i > m) m = i;
    j += i;
  }
  if (myMT->MT_NodeNum() > 2)
  {
     printf("out-arcs max = %d, avg = %f (arcs leaving an internal node)\n",
             h, (float)(k) / (float)(myMT->MT_NodeNum()-2));
     printf("new-tiles max = %d, avg = %f (new tiles of an internal node)\n",
             m, (float)(j) / (float)(myMT->MT_NodeNum()-2));
  }
     
  /* maximum and mean number of parents for an internal node, and 
     maximum and mean number of old tiles of an internal node */
  h = k = 0; /* max and mean number of parents */
  m = j = 0; /* max and mean number of tiles */
  for (n = 2; n < myMT->MT_NodeNum(); n++)
  {
    if (myMT->MT_NumInArcs(n) > h) h = myMT->MT_NumInArcs(n);
    k += myMT->MT_NumInArcs(n);
    i = 0; /* count old tiles of node n */
    for (a = myMT->MT_FirstInArc(n);
         a != MT_NULL_INDEX;
         a = myMT->MT_NextInArc(n,a))
    {  i += myMT->MT_NumArcTiles(a);  }
    if (i > m) m = i;
    j += i;
  }
  if (myMT->MT_NodeNum() > 2)
  {
     printf("in-arcs max = %d, avg = %f (arcs entering an internal node)\n",
             h, (float)(k) / (float)(myMT->MT_NodeNum()-2));
     printf("old-tiles max = %d, avg = %f (old tiles of an internal node)\n",
             m, (float)(j) / (float)(myMT->MT_NodeNum()-2));
  }
}

void main(int argc, char **argv)
{
    setOptions(argc,argv);
    loadMT();
    statMT();
}
