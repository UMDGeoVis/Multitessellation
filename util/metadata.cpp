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
Program created in August 2006 by modifying previous program stats.cpp.
Extracted statistical information have been restricted to those
used as metadata in the shape repository of the European Network
of Excellence Aim@Shape.
Read MT from file, write statistics to standard output.
- type of tesselation represented in the MT (surface or volume mesh)
- number of tiles and vertices of the tesselation at minimum resolution
- number of tiles and vertices of the tesselation at maximum resolution
- maximum and mean number of new tiles of an internal node
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

void statMT(void)
{
  /* auxiliary array */
  unsigned int * aux_arr;
  /* counters */
  int i,j,h,m;
  /* variables for nodes, arcs, tiles, vertices */
  MT_INDEX n, a, t, * v;

  printf("===MT METADATA===\n");

  if (myMT->MT_TileDim() == 2) printf("Mesh type = surface mesh\n");
  else printf("Mesh type = volume mesh\n");

  /* now aux_arr will be used as a bit vector if a vertex belongs to the
     tesselation */
  aux_arr = (unsigned int*) 
                  calloc (myMT->MT_VertexNum()+1,sizeof(unsigned int));
  
  /* number of tiles and of vertices of the tesselation a minimum
     resolution */
  n = myMT->MT_Root();
  h = 0; /* count new tiles */
  m = 0; /* count vertices */
  for(a = myMT->MT_FirstOutArc(n); 
      a != MT_NULL_INDEX; 
      a = myMT->MT_NextOutArc(n,a))
  {
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
  printf("Number of tiles at mimimum resolution = %d\n", h);
  printf("Number of vertices at mimimum resolution) = %d\n", m);
  
  /* number of tiles and of vertices of the tesselation 
    at maximum resolution */
  n = myMT->MT_Drain();
  h = 0; /* count old tiles */
  m = 0; /* count vertices */
  for(a = myMT->MT_FirstInArc(n); 
      a != MT_NULL_INDEX; 
      a = myMT->MT_NextInArc(n,a))
  {
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
  printf("Number of tiles at maximum resolution = %d\n", h);
  printf("Number of vertices at maximum resolution = %d\n", m);

  /* maximum number of new tiles of an internal node */
  m = j = 0; /* maximum and mean number of new tiles */
  for (n = 2; n<myMT->MT_NodeNum(); n++)
  {
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
     printf("Granularity (max tiles involved ");
     printf("in an elementary simplification move) = %d\n", m);
  }

}

int main(int argc, char **argv)
{
    setOptions(argc,argv);
    loadMT();
    statMT();
}
