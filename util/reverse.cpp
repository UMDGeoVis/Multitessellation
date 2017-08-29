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

/* ------------------------------ reverse.c ------------------------------- */

#include "mt_multi.h"

/* ------------------------------------------------------------------------ */
/*                       REVERSE A TWO-DIMENSIONAL MT                       */
/* ------------------------------------------------------------------------ */

/*
Reverse the orientation of tiles (triangles) of a two-dimensional MT
embedded in three dimensions.
This corresponds to turning a surface upside down.
*/

/* ------------------------------------------------------------------------ */
/*                            Auxiliary function                            */
/* ------------------------------------------------------------------------ */

/*
Reverse the orientation of all triangles in the MT.
*/
void reverse_triangles(MT_MultiTesselation m)
{
   int i;
   MT_INDEX * v, tmp;
   
   for (i=1;i<=m->MT_TileNum();i++)
   {
      m->MT_TileVertices(i,&v);
      /* permute vertices 0 1 2 --> 2 1 0 */
      tmp = v[0]; v[0] = v[2]; v[2] = tmp;
   } 
}

/* ------------------------------------------------------------------------ */
/*                               Main program                               */
/* ------------------------------------------------------------------------ */

void main(int argc, char ** argv)
{
   MT_MultiTesselation mm;
   FILE * fd;

   if (argc<3)
   {
     fprintf(stderr,"Usage: %s input_mt output_mt\n",argv[0]);
     fprintf(stderr,"  Reverse the orientation of triangles\n");
     fprintf(stderr,"  in a 2-dimensional MT embedded in 3D space.\n");
     exit(1);
   }
   
   /* Create and read the mt. */
   fd = fopen(argv[1],"r");
   if (!fd)
   {  fprintf(stderr,"Cannot open MT file %s\n",argv[1]);
      exit(1);
   }
   mm = new MT_MultiTesselationClass(3,2);
   mm->MT_Read(fd);
   fclose(fd);
   fprintf(stderr,"Input MT loaded from file %s.\n",argv[1]);
     
   /* Reverse the mt and write it. */
   reverse_triangles(mm);
   fd = fopen(argv[2],"w");
   if (!fd)
   {  fprintf(stderr,"Cannot open output file %s\n",argv[2]);
      exit(1);
   }
   mm->MT_Write(fd);
   fprintf(stderr,"Reversed MT written on file %s.\n",argv[2]);
   
   /* free allocated memory */
   delete mm;
}

/* ------------------------------------------------------------------------ */
