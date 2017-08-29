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

/* ----------------------------- cmpnorma.c ------------------------------ */

#include "cmpnorma.h"

/* ----------------------------------------------------------------------- */
/*                        COMPUTE VERTEX NORMALS                           */
/* ----------------------------------------------------------------------- */

void ExtraVertexNormalClass ::
     ComputeAllVertexNormals(MT_Extractor ex)
{
  MT_INDEX * extra_trgs;  /* array extracted triangles */
  MT_INDEX extra_count;   /* counter for extracted triangles */
  MT_INDEX * v;   /* vertices of current triangle */
  float n[3];     /* components of normal to current triangle */
  MT_INDEX i;
  int j;

  extra_count = ex->MT_AllExtractedTiles(&extra_trgs);  
  for (i=0; i<extra_count; i++)
  {
     mt->MT_TileVertices(extra_trgs[i], &v);
     TriangleNormalv (mt->MT_VertexCoords(v[0]),
                      mt->MT_VertexCoords(v[1]),
                      mt->MT_VertexCoords(v[2]), n);
                                                 

     for (j=0;j<3;j++)
     {
        VN_count[v[j]]++;
        my_norm->MT_SetAttrVectorEntry(v[j],0,VertexNX(v[j])+n[0]);
        my_norm->MT_SetAttrVectorEntry(v[j],1,VertexNY(v[j])+n[1]);
        my_norm->MT_SetAttrVectorEntry(v[j],2,VertexNZ(v[j])+n[2]);
     }
  }
  for (i=0; i<extra_count; i++)
  {
     mt->MT_TileVertices(extra_trgs[i], &v);
     for (j=0;j<3;j++)
     {  if (VN_count[v[j]] != 0) /* see [*] below */
        {
          n[0] = VertexNX(v[j])/VN_count[v[j]];
          n[1] = VertexNY(v[j])/VN_count[v[j]];
          n[2] = VertexNZ(v[j])/VN_count[v[j]];
          normalize(&n[0],&n[1],&n[2]);
          my_norm->MT_SetAttrVector(v[j],n);
          VN_count[v[j]] = 0; /* [*] this vertex has been examined */
        }
     }
  }
}

void ExtraVertexNormalClass ::
     ComputeVertexNormals(MT_Extractor ex)
{
  MT_INDEX * extra_trgs;  /* array extracted triangles */
  MT_INDEX extra_count;   /* counter for extracted triangles */
  MT_INDEX * v;   /* vertices of current triangle */
  float n[3];     /* components of normal to current triangle */
  MT_INDEX i;
  int j;

  extra_count = ex->MT_ExtractedTiles(&extra_trgs);  
  for (i=0; i<extra_count; i++)
  {
     mt->MT_TileVertices(extra_trgs[i], &v);
     TriangleNormalv (mt->MT_VertexCoords(v[0]),
                      mt->MT_VertexCoords(v[1]),
                      mt->MT_VertexCoords(v[2]), n);
                                                 

     for (j=0;j<3;j++)
     {
        VN_count[v[j]]++;
        my_norm->MT_SetAttrVectorEntry(v[j],0,VertexNX(v[j])+n[0]);
        my_norm->MT_SetAttrVectorEntry(v[j],1,VertexNY(v[j])+n[1]);
        my_norm->MT_SetAttrVectorEntry(v[j],2,VertexNZ(v[j])+n[2]);
     }
  }
  for (i=0; i<extra_count; i++)
  {
     mt->MT_TileVertices(extra_trgs[i], &v);
     for (j=0;j<3;j++)
     {  if (VN_count[v[j]] != 0) /* see [*] below */
        {
          n[0] = VertexNX(v[j])/VN_count[v[j]];
          n[1] = VertexNY(v[j])/VN_count[v[j]];
          n[2] = VertexNZ(v[j])/VN_count[v[j]];
          normalize(&n[0],&n[1],&n[2]);
          my_norm->MT_SetAttrVector(v[j],n);
          VN_count[v[j]] = 0; /* [*] this vertex has been examined */
        }
     }
  }
}

/* ----------------------------------------------------------------------- */

ExtraVertexNormalClass ::
ExtraVertexNormalClass(MT_MultiTesselation m)
: WithVertexNormalTableClass()
{
  if ( (m->MT_VertexDim()!=3) || (m->MT_TileDim()!=2) )
  MT_Error("Not suitable MT","ExtraVertexNormalClass Constructor");
  mt = m;
  VN_count = (short int *) calloc (mt->MT_VertexNum()+1, sizeof(short int) );
  my_norm->MT_SetAttrNum(mt->MT_VertexNum()+1);
}

ExtraVertexNormalClass :: ~ExtraVertexNormalClass(void)
{
  if (VN_count) delete VN_count; VN_count = NULL;
}

/* ----------------------------------------------------------------------- */
