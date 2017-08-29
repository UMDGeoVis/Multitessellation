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

/* 
BOX DEMO
by Paola Magillo - DISI - University of Genova - Italy
Written JANUARY 1999, revised OCTOBER 1999
*/

/* ------------------------------------------------------------------------- */
/*                                include                                    */
/* ------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "demo1.h"

/* ------------------------------------------------------------------------- */
/*                           global variables                                */
/* ------------------------------------------------------------------------- */

/* Surface MT. */
SurfaceWithError mt = NULL;

/* Extractor. */
MT_Extractor ex = NULL;
int extractor_type = DYNAMIC_EXTR;

/* Input file names. */
char file_name[MT_MAX_STRLEN] = "";  /* for the MT */
char file_name1[MT_MAX_STRLEN] = ""; /* for tile errors */
char file_name2[MT_MAX_STRLEN] = ""; /* for vertex normals */

/* Extraction focus and filter. */
Box3FocusOnTrianglesClass * focus = NULL;
TileErrFilterClass * filter = NULL;

/* Threshold used inside resolution filter. */
UnifThresholdClass * unif_thr;

/* Factor for error threshold: 1 = min error, 0 = max error. */
float error_factor = 1.0;

/* ------------------------------------------------------------------------- */
/*                               functions                                   */
/* ------------------------------------------------------------------------- */

/* Process command line options. */
void setOptions(int argc, char * argv[])
{
   if (argc<2) /* no command line options */
   {
     fprintf(stderr,"Box Demo. Syntax: %s XXX\n",argv[0]);
     fprintf(stderr,"Load MT, errors, normals from files XXX.mtf, XXX.err, XXX.nrm\n");
     fprintf(stderr,"For instance: %s ../data_demo1/bunny\n",argv[0]);
     exit(1);
   }
   /* else */
   strcpy(file_name,argv[1]);
   strcpy(file_name1,file_name);
   strcpy(file_name2,file_name);
   strcat(file_name,".mtf");    /* MT file */
   strcat(file_name1,".err");   /* tile error file */
   strcat(file_name2,".nrm");   /* vertex normal file */
}

/* ------------------------------------------------------------------------ */

/* Load MT and tile errors. */
void loadMT()
{
   printf("Loading MT from files %s, %s, %s...\n",
          file_name, file_name1, file_name2);
   mt = new SurfaceWithErrorClass();
   if(!mt->MT_Read(file_name))
       MT_FatalError("Cannot read MT file","loadMT");
   if(!mt->ReadTileErrors(file_name1))
       MT_FatalError("Cannot read tile error file","loadMT");
   if (!mt->ReadVertexNormals(file_name2))
       MT_FatalError("Cannot read vertex normal file","loadMT");
   printf("...MT, errors, normals loaded\n");
}

/* ------------------------------------------------------------------------ */

/* Prepare extractor of the given type. */
void prepareExtractor()
{
//  char msg[MT_MAX_STRLEN];

  switch (extractor_type)
  {
     case STATIC_EXTR: 
          ex =  new MT_StaticExtractorClass(mt);
//          sprintf(msg,"%s","STATIC EXTRACTOR"); 
          break;
     case DYNAMIC_EXTR:
          ex =  new MT_DynamicExtractorClass(mt);
//          sprintf(msg,"%s","DYNAMIC EXTRACTOR");
          break;
     case LOCAL_EXTR:
          ex =  new MT_LocalExtractorClass(mt);
//          sprintf(msg,"%s","LOCAL EXTRACTOR");
          break;
     
  }
//  printf("==== %s ====\n",msg);
}

/* ------------------------------------------------------------------------ */

/* Play one step forward. */
void advanceGame(float dx, float dy, float dz)
{
  float incr[3];
  incr[0] = dx * (mt->MT_MaxX()-mt->MT_MinX()) / 50.0;
  incr[1] = dy * (mt->MT_MaxY()-mt->MT_MinY()) / 50.0;
  incr[2] = dz * (mt->MT_MaxZ()-mt->MT_MinZ()) / 50.0;
  focus->Translate( incr );
  ex->MT_ExtractTesselation();
//  fprintf(stderr,"\r Extracted triangles: %d    ",
//          ex->MT_AllExtractedTilesNum());
}

/* ------------------------------------------------------------------------ */

/* Inizialization. */
void startGame(int argc, char **argv)
{
  float mean_dim;
  float bx[6]; /* box */
  float err;   /* value of error threshold */

  setOptions(argc,argv); 
  loadMT();
  prepareExtractor();
  mean_dim = ( (mt->MT_MaxX()-mt->MT_MinX()) +
               (mt->MT_MaxY()-mt->MT_MinY()) +
               (mt->MT_MaxX()-mt->MT_MinX()) ) / 6.0;
  bx[0] = mt->MT_MinX() - mean_dim*0.05; 
  bx[1] = mt->MT_MinY() - mean_dim*0.05; 
  bx[2] = mt->MT_MaxZ() - mean_dim*0.05;
  bx[3] = mt->MT_MinX() + mean_dim*0.5;
  bx[4] = mt->MT_MinY() + mean_dim*0.5;
  bx[5] = mt->MT_MaxZ() + mean_dim*0.5;
  focus = new Box3FocusOnTrianglesClass(mt, bx[0], bx[1],
                                            bx[2], bx[3],
                                            bx[4], bx[5]);
  ex->MT_SetFocus( focus );
  err = mt->MinError() + (1.0 - error_factor) * mt->MaxError();
  unif_thr = new UnifThresholdClass( err );
  filter = new TileErrFilterClass( mt, unif_thr );
  ex->MT_SetFilter( filter );
  ex->MT_ExtractTesselation();
}

/* Restore inizial box position. */
void restoreGame()
{
  float mean_dim;
  float bx[6]; /* box */
  float err;   /* value of error threshold */

  mean_dim = ( (mt->MT_MaxX()-mt->MT_MinX()) +
               (mt->MT_MaxY()-mt->MT_MinY()) +
               (mt->MT_MaxX()-mt->MT_MinX()) ) / 6.0;
  bx[0] = mt->MT_MinX() - mean_dim*0.05; 
  bx[1] = mt->MT_MinY() - mean_dim*0.05; 
  bx[2] = mt->MT_MaxZ() - mean_dim*0.05;
  bx[3] = mt->MT_MinX() + mean_dim*0.5;
  bx[4] = mt->MT_MinY() + mean_dim*0.5;
  bx[5] = mt->MT_MaxZ() + mean_dim*0.5;
  focus->SetBox(bx[0], bx[1], bx[2], bx[3], bx[4], bx[5]);
  err = mt->MinError() + (1.0 - error_factor) * mt->MaxError();
  unif_thr->SetThreshold( err );
  ex->MT_ExtractTesselation();
}

/* ------------------------------------------------------------------------ */

/* Termination. */
void endGame(void)
{
  if (ex) delete ex;
  if (mt) delete mt;
  if (focus) delete focus;
  if (unif_thr) delete unif_thr;
  if (filter) delete filter;
  fprintf(stderr,"Normal termination\n");
  exit(0);
}

/* ------------------------------------------------------------------------ */

/* Get extractor type. */
int getExtractor(void)
{  return (extractor_type);  }

/* Change extractor type. */
void changeExtractor(void)
{
  switch(extractor_type)
  {
    case STATIC_EXTR:  extractor_type = DYNAMIC_EXTR; break;
    case DYNAMIC_EXTR: extractor_type = LOCAL_EXTR; break;
    case LOCAL_EXTR:   extractor_type = STATIC_EXTR; break;
  }
  prepareExtractor();
  ex->MT_SetFocus( focus );
  ex->MT_SetFilter( filter );
  ex->MT_ExtractTesselation();
}

/* ------------------------------------------------------------------------ */

/* Get reference value for error threshold */
float getErrorBound(void)
{  return error_factor;  }

/* Change reference value for error threshold */
void changeErrorBound(float v)
{
  if ( (v>=0.0) && (v<=1.0) )
  {  
     float err; /* value of error threshold */
     error_factor = v;  
     err = mt->MinError() + (1.0 - error_factor) * mt->MaxError();
     unif_thr->SetThreshold( err );
     ex->MT_ExtractTesselation();
  }
}  

/* ------------------------------------------------------------------------ */

/* Scale the box keeping its center: sc = +1,-1 (larger,smaller) */
void scaleBox(int sc)
{
  float bx[6]; /* the box */
  float c;  /* one coordinate of center of the box */
  float s;  /* one side of the box */
  int i;
  focus->TheBox(&bx[0], &bx[1], &bx[2], &bx[3], &bx[4], &bx[5]);
  for (i=0; i<3; i++)
  {  c = 0.5*(bx[i]+bx[i+3]);
     s = 0.5*(bx[i+3]-bx[i]);
     bx[i] = c - (1.0 + 0.1 * (float)sc) * s;
     bx[i+3] = c + (1.0 + 0.1 * (float)sc) * s;
  }
  focus->SetBox(bx[0], bx[1], bx[2], bx[3], bx[4], bx[5]);
  ex->MT_ExtractTesselation();
}

/* ------------------------------------------------------------------------ */
