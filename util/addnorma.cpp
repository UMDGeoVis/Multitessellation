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

/* ----------------------------- addnorma.c ------------------------------ */

#include "mt_extra.h"
#include "mt_cond.h"
#include "surfmt.h"
#include "geo.h"

/* ----------------------------------------------------------------------- */
/*                COMPUTE VERTEX NORMALS FOR A SURFACE MT                  */
/* ----------------------------------------------------------------------- */

/*
REMARK: we assume that the triangulation at full resolution contains all
vertices of the MT. This is true in MTs build through iterative vertex
insertion/deletion (including edge collapse which do not move the surviving
vertex of the collapsed edge).
*/

/*
We use a static extractor and a filter condition identically false on all
tiles, and extract the tesselation at maximum resolutiuon available in the
MT. In our assumption, such tesselation contains all vertices. 
We compute vertex normals on such tesselation and assign them to the MT 
vertices.
*/

/* ----------------------------------------------------------------------- */
/*                         Global variables                                */
/* ----------------------------------------------------------------------- */

SurfaceWithError mt;     /* the MT for which normals are computed */
MT_StaticExtractor ex;   /* the extractor */
MT_FalseCond flt;        /* filter identically false */
VertexNormalBuildingInterface bi; /* building interface for vertex normals */

MT_INDEX * extra_trgs;  /* array extracted triangles */
MT_INDEX extra_count;   /* counter for extracted triangles */

float * VN_Arr; /* array: vertex normals for all MT vertices */
int * VN_count; /* array: counters of incident triangles for all vertices */

/* ----------------------------------------------------------------------- */
/*                         Auxiliary function                              */
/* ----------------------------------------------------------------------- */

/*
Compute vertex normals on the extracted tesselation.
*/
void computeVertexNormals(void)
{
  MT_INDEX * v;   /* vertices of current triangle */
  float n[3];     /* components of normal to current triangle */
  MT_INDEX i;
  int j;
  
  for (i=0; i<extra_count; i++)
  {
     mt->MT_TileVertices(extra_trgs[i], &v);
     TriangleNormalv (mt->MT_VertexCoords(v[0]),
                      mt->MT_VertexCoords(v[1]),
                      mt->MT_VertexCoords(v[2]), n);

     for (j=0;j<3;j++)
     {
        VN_count[v[j]]++;
        VN_Arr[3*v[j]] += n[0];
        VN_Arr[3*v[j]+1] += n[1];
        VN_Arr[3*v[j]+2] += n[2];
     }
  }
  for (i=0; i<extra_count; i++)
  {
     mt->MT_TileVertices(extra_trgs[i], &v);
     for (j=0;j<3;j++)
     {  if (VN_count[v[j]] != 0) /* see [*] below */
        {
          VN_Arr[3*v[j]] /=  VN_count[v[j]];
          VN_Arr[3*v[j]+1] /=  VN_count[v[j]];
          VN_Arr[3*v[j]+2] /=  VN_count[v[j]];
          /* [*] set 0 for avoiding examining this vertex another time */
          VN_count[v[j]] = 0;
          normalize(&VN_Arr[3*v[j]], &VN_Arr[3*v[j]+1], &VN_Arr[3*v[j]+2]);
        }
     }
  }
}

/* ----------------------------------------------------------------------- */
/*                                    MAIN                                 */
/* ----------------------------------------------------------------------- */
                               
int main (int argc, char ** argv)
{
  FILE * fd;
  MT_INDEX i;
 
  if (argc<3)
  {
    fprintf(stderr,"Usage: %s mt_file normal_file\n",argv[0]);
    fprintf(stderr,"  Compute normals for MT contained in file mt_file\n");
    fprintf(stderr,"  and write such normals in file normal_file.\n");
    exit(1);
  }
  
  /* Create and read the mt. */
  fd = fopen(argv[1],"r");
  if (!fd) 
  {  fprintf(stderr,"Cannot open MT file %s\n",argv[1]);
     exit(1);
  }
  mt = new SurfaceWithErrorClass();
  mt->MT_Read(fd);
  fclose(fd);   
  fprintf(stderr,"MT loaded from file %s.\n",argv[1]);

  /* Create extractor, filter, and extract tesselation. */
  ex = new MT_StaticExtractorClass(mt);
  flt = new MT_FalseCondClass();
  ex->MT_SetFilter(flt);
  ex->MT_ExtractTesselation();
  extra_count = ex->MT_ExtractedTiles(&extra_trgs);

  /* Allocate auxiliary arrays */
  VN_count = (int *) calloc (mt->MT_VertexNum()+1, sizeof(int) );
  VN_Arr = (float *) calloc ( 3*(mt->MT_VertexNum()+1), sizeof(float) );

  /* Compute vertex normals on extracted tesselation. */
  computeVertexNormals();
  
  /* delete what is not needed any more */
  free (VN_count);
  delete ex;
  delete flt;
  
  /* Create vertex normal building interface */
  bi = new VertexNormalBuildingInterfaceClass();
  
  /* Record vertex normals into the building interface */
  bi->StartVertexNormalHistory();
  for (i=1; i<=mt->MT_VertexNum(); i++)
  {
     bi->MakeVertexNormal(i, VN_Arr+3*i);
  }
  bi->EndVertexNormalHistory();
  
  /* convert vertex normal from building interface to vertex normal
     table of the MT */
  bi->SetTargetVertexNormalTable(mt);
  bi->ConvertVertexNormals();
  
  /* write vertex normals */
  fd = fopen(argv[2],"w");
  if (!fd) 
  {  fprintf(stderr,"Cannot open output file %s\n",argv[2]);
     exit(1);
  }
  mt->WriteVertexNormals(fd);
  fclose(fd);   
  fprintf(stderr,"Normals written on file %s.\n",argv[2]);

  /* free allocated memory */
  delete bi;
  delete mt;
  free (VN_Arr);
}

/* ----------------------------------------------------------------------- */
