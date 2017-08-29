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

/* ------------------------------- fltbox.c ------------------------------- */

#include "fltbox.h"

/* ------------------------------------------------------------------------ */
/*                            FREE BOX CLASS                                */
/* ------------------------------------------------------------------------ */
          
/* ------------------------------------------------------------------------ */
/*                       Functions to modify the box                        */
/* ------------------------------------------------------------------------ */
   
void FreeBoxClass :: SetBox (float * side_len)
{
  int i;
  for (i=0; i<BoxDim(); i++)  {  box_dim[i] = side_len[i];  }  
}

void FreeBoxClass :: SetEdgeLength (int i, float w) 
{  box_dim[i] = w;  }

void FreeBoxClass :: SetBox (float lx, float ly)
{   box_dim[0] = lx;  box_dim[1] = ly;  }

void FreeBoxClass :: SetBox (float lx, float ly, float lz)
{   box_dim[0] = lx;  box_dim[1] = ly;  box_dim[2] = lz;  }

/* ------------------------------------------------------------------------ */
/*                       Functions to return the box                        */
/* ------------------------------------------------------------------------ */

void FreeBoxClass :: TheBox (float ** side_len)
{   (*side_len) = box_dim;  }

void FreeBoxClass :: TheBox (float *lx, float *ly)
{  (*lx) = box_dim[0];  (*ly) = box_dim[1];  }
   
void FreeBoxClass :: TheBox (float *lx, float *ly, float *lz)
{  (*lx) = box_dim[0];  (*ly) = box_dim[1];  (*lz) = box_dim[2];  }
   
/* ------------------------------------------------------------------------ */
/*                          Creation of the box                             */
/* ------------------------------------------------------------------------ */

FreeBoxClass ::  FreeBoxClass(int d, float * side_len)
{
  b_dim = d; 
  SetBox(side_len);
}

FreeBoxClass :: FreeBoxClass(int d)
{
  int i;
  b_dim = d; 
  for (i=0; i<b_dim; i++)  {  box_dim[i] = 0.0;  }
}

/* ------------------------------------------------------------------------ */
/*                       BOX-BASED RESOLUTION CONDITION                     */
/* ------------------------------------------------------------------------ */

int BoxFilterClass :: MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag)
{
   MT_INDEX * v;          /* for the vertices of t */
   float * f;             /* for vertex coordinates */
   float low[MT_MAX_DIM]; /* for the minimum coordinates of t */
   float hi[MT_MAX_DIM];  /* for the maximum coordinates of t */
   int i, j;
   
   m->MT_TileVertices(t,&v);
   for (i=0; i<BoxDim(); i++)
   {
     low[i] = m->MT_MaxCoord(i);
     hi[i] = m->MT_MinCoord(i);
   }
   for (j=0; j<=m->MT_TileDim(); j++)
   {  m->MT_VertexCoords(v[j],&f);
      for (i=0; i<BoxDim(); i++)
      {  if (f[i]<low[i]) low[i] = f[i];  
         else if (f[i]>hi[i]) hi[i] = f[i];  
      }
   }
   for (i=0; i<BoxDim(); i++)
   {  if ( (hi[i]-low[i]) > box_dim[i] ) return 0;  }
   return 1;
}

BoxFilterClass :: BoxFilterClass(int dim, float * side_len)
:      FreeBoxClass(dim, side_len)
{ }

/* ------------------------------------------------------------------------ */
