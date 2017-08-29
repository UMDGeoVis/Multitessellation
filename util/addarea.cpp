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

/* --------------------------- addAreaError.c ---------------------------- */

#include "mt_extra.h"
#include "mt_cond.h"
#include "fieldmt.h"
#include "geo.h"

/* ----------------------------------------------------------------------- */
/*       COMPUTE TRIANGLE AREAS AND SAVE THEM AS TRIANGLE ERRORS           */
/* ----------------------------------------------------------------------- */

/* ----------------------------------------------------------------------- */
/*                         Global variables                                */
/* ----------------------------------------------------------------------- */

FieldWithError mt;       /* the MT for which triangle areas are computed */
TileErrorBuildingInterface bi;  /* building interface for triangle areas */
float factor = 1.0;      /* multiplicative factor for areas */

/* ----------------------------------------------------------------------- */
/*                         Auxiliary function                              */
/* ----------------------------------------------------------------------- */

/*
Compute triangle area and return it
*/
float computeTriangleArea(MT_INDEX t)
{
  MT_INDEX * v;   /* vertices of current triangle */
  float x[3];     /* x coordinates of vertices */
  float y[3];     /* y coordinates of vertices */
  float z[3];     /* z coordinates of vertices */
  int i;
  float a;
  
  mt->MT_TileVertices(t, &v);
  for (i=0; i<3; i++)
  {
    x[i] = mt->MT_VertexX(v[i]);
    y[i] = mt->MT_VertexY(v[i]);
    z[i] = mt->MT_VertexZ(v[i]);
  }
  a = TriangleArea(x[0],y[0],z[0],
                   x[1],y[1],z[1],
                   x[2],y[2],z[2]);
  if (a<=0.0) fprintf(stderr,"Area nulla\n");
  return a*factor;
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
    fprintf(stderr,"Usage: %s mt_file area_file [factor]\n",argv[0]);
    fprintf(stderr,"  Compute triangle areas for MT contained in file mt_file\n");
    fprintf(stderr,"  and write such areas in file area_file.\n");
    fprintf(stderr,"  Such areas are multiplied by factor, if given.\n");
    exit(1);
  }
  
  if (argc>=4)
  {  if (sscanf(argv[3],"%f", &factor)!=1)
     fprintf(stderr,"Invalid factor, ignored\n");
     factor = 1.0;
  }
  /* Create and read the mt. */
  fd = fopen(argv[1],"r");
  if (!fd) 
  {  fprintf(stderr,"Cannot open MT file %s\n",argv[1]);
     exit(1);
  }
  mt = new FieldWithErrorClass(3,2);
  mt->MT_Read(fd);
  fclose(fd);   
  fprintf(stderr,"MT loaded from file %s.\n",argv[1]);
 
  /* Create vertex normal building interface */
  bi = new TileErrorBuildingInterfaceClass();
  
  /* Record vertex normals into the building interface */
  bi->StartTileErrorHistory();
  for (i=1; i<=mt->MT_TileNum(); i++)
  {
     bi->MakeTileError(i, computeTriangleArea(i));
  }
  bi->EndTileErrorHistory();
  
  /* convert triangle area from building interface to tile error
     table of the MT */
  bi->SetTargetTileErrorTable(mt);
  bi->ConvertTileErrors();
  
  /* write tile errors */
  fd = fopen(argv[2],"w");
  if (!fd) 
  {  fprintf(stderr,"Cannot open output file %s\n",argv[2]);
     exit(1);
  }
  mt->WriteTileErrors(fd);
  fclose(fd);   
  fprintf(stderr,"Areas written as tile errors on file %s.\n",argv[2]);

  /* free allocated memory */
  delete bi;
  delete mt;
}

/* ----------------------------------------------------------------------- */
