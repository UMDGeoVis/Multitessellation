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
TERRAIN DEMO
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

#include "demo2.h"

/* ------------------------------------------------------------------------- */
/*                           global variables                                */
/* ------------------------------------------------------------------------- */

/* Field MT. */
FieldWithError mt = NULL;

/* Extractor. */
MT_Extractor ex = NULL;
int extractor_type = DYNAMIC_EXTR;

/* Input file names. */
char file_name[MT_MAX_STRLEN] = "";  /* for the MT */
char file_name1[MT_MAX_STRLEN] = ""; /* for tile errors */

/* Extraction focus and filter. */
Wedge2FocusOnTrianglesClass * focus = NULL;
TileErrFilterClass * filter = NULL;

/* Threshold used inside resolution filter, 
   and dependency law used inside the threshold. */
PDist2ThresholdOnTriangles dist_thr;
DepLaw law; 

/* Container for vertex normals */
ExtraVertexNormal vn = NULL;

/* Parameters describing the circular movement. */
float center[2];   /* coordinates of the center (only for old version) */
float radius;      /* radius (only for old version) */
float curr_angle;  /* current angle */
float curr_pos[2]; /* current position */


/* Factor for error threshold (it will be multipled by the height range
   of the terrain). */
float error_factor = 0.005;

/* ------------------------------------------------------------------------- */
/*                               functions                                   */
/* ------------------------------------------------------------------------- */

void setOptions(int argc, char * argv[])
{
   if (argc<2) /* no command line options */
   {
     fprintf(stderr,"Terrain Demo. Syntax: %s XXX\n",argv[0]);
     fprintf(stderr,"Load MT and tile errors from files XXX.mtf and XXX.err\n");
     fprintf(stderr,"For instance: %s ../data_demo2/marcy200\n",argv[0]);
     exit(1);
   }
   /* else */
   strcpy(file_name,argv[1]);
   strcpy(file_name1,file_name);
   strcat(file_name,".mtf");    /* MT file */
   strcat(file_name1,".err");   /* tile error file */
}

/* ------------------------------------------------------------------------ */

/* Load MT and tile errors. */
void loadMT()
{
   printf("Loading MT from files %s, %s...\n",
          file_name, file_name1);
   mt = new FieldWithErrorClass(3,2);
   if(!mt->MT_Read(file_name))
       MT_FatalError("Cannot read MT file","loadMT");
   if(!mt->ReadTileErrors(file_name1))
       MT_FatalError("Cannot read tile error file","loadMT");
   printf("...MT and tile errors loaded\n");
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
  if (!vn) vn = new ExtraVertexNormalClass(mt);
}

/* ------------------------------------------------------------------------ */

/* Play one step forward:
   motion_dir = direction of motion: 0 = stop, 1 = forward, -1 = backward
   motion_ang = angle of motion: 0 = straight, 1 = left, -1 = right */
void advanceGame(int motion_dir, int motion_ang)
{
  float dir[2];

  if (motion_ang) curr_angle += ( (float)motion_ang * 0.05 );
  /* now vector dir is used for the direction of movement */
  dir[0] = - sin(curr_angle);
  dir[1] =  cos(curr_angle);
  focus->SetAxisDir(dir[0], dir[1]);
  
  if (motion_dir)
  {
    /* now vector dir is used for the translation vector */
    dir[0] *= ( (float)motion_dir * 0.004 * (mt->MT_MaxX()-mt->MT_MinX()) );
    dir[1] *= ( (float)motion_dir * 0.004 * (mt->MT_MaxY()-mt->MT_MinY()) );
    if ( ! ( (curr_pos[0] >= mt->MT_MaxX()) && (dir[0] > 0.0) ) &&
         ! ( (curr_pos[0] <= mt->MT_MinX()) && (dir[0] < 0.0) ) )
       curr_pos[0] += dir[0];
                      (mt->MT_MaxX()-mt->MT_MinX()) * (float)motion_dir;
    if ( ! ( (curr_pos[1] >= mt->MT_MaxY()) && (dir[1] > 0.0) ) &&
         ! ( (curr_pos[1] <= mt->MT_MinY()) && (dir[1] < 0.0) ) )
       curr_pos[1] += dir[1];
    focus->SetVertex(curr_pos[0], curr_pos[1]);
    dist_thr->SetPoint(curr_pos[0], curr_pos[1]);
  }

  ex->MT_ExtractTesselation();
  vn->ComputeVertexNormals(ex);
//  fprintf(stderr,"\r Extracted triangles: %d    ",
//          ex->MT_AllExtractedTilesNum());
}

void advanceGame_old(void)
{
  float dir[2]; /* direction of movement */

  curr_angle += 0.008;

  curr_pos[0] = center[0] + radius * cos(curr_angle);
  curr_pos[1] = center[1] + radius * sin(curr_angle);
  dir[0] = - sin(curr_angle);
  dir[1] =  cos(curr_angle);

  focus->SetVertex(curr_pos[0], curr_pos[1]);
  focus->SetAxisDir(dir[0], dir[1]);
  dist_thr->SetPoint(curr_pos[0], curr_pos[1]);
  ex->MT_ExtractTesselation();
  vn->ComputeVertexNormals(ex);
//  fprintf(stderr,"\r Extracted triangles: %d    ",
//          ex->MT_AllExtractedTilesNum());
}

/* ------------------------------------------------------------------------ */

/* Inizialization. */
void startGame(int argc, char **argv)
{
  float dir[2]; /* direction of movement */
  float opening_angle; /* half opening angle of the wedge */
  float vpt, dist, vdist; /* parameters of dependency law */
  float deltaX, deltaY;
  
  setOptions(argc,argv); 
  loadMT();
  prepareExtractor();
  
  deltaX = mt->MT_MaxX() - mt->MT_MinX();
  deltaY = mt->MT_MaxY() - mt->MT_MinY();
  
  center[0] = 0.5 * (mt->MT_MaxX() + mt->MT_MinX());
  center[1] = 0.5 * (mt->MT_MaxY() + mt->MT_MinY());
  if (deltaX<deltaY) radius = 0.25 * deltaX;
  else radius = 0.25 * deltaY;
  curr_angle = 0.0; 
  opening_angle = 0.4; 

  curr_pos[0] = center[0] + radius * cos(curr_angle);
  curr_pos[1] = center[1] + radius * sin(curr_angle);
  dir[0] = - sin(curr_angle);
  dir[1] =  cos(curr_angle);

  focus = new Wedge2FocusOnTrianglesClass(curr_pos[0], curr_pos[1],
/* PARTE SOSTITUITA
                      curr_pos[0]+dir[0], curr_pos[1]+dir[1], opening_angle);
FINO QUI */
                      curr_pos[0]+radius*dir[0], curr_pos[1]+radius*dir[1],
                      opening_angle);
/* FINE PARTE NUOVA */                      
  ex->MT_SetFocus( focus );

  vpt = 0.0;   /* value at point curr_pos */
  if (deltaX<deltaY) dist = 0.075 * deltaX;
  else dist = 0.075 * deltaY; /* reference distance from curr_pos */
  vdist = error_factor * (mt->MaxVertexField() - mt->MinVertexField());

  law = new DepLawClass (0.0, vpt, dist*dist, vdist, MT_SQRT_DEP);
//printf("Law: 0.0, %f, %f, %f, %d\n", vpt, dist*dist, vdist, MT_SQRT_DEP);
//printf("Dist: (%f,%f)\n", curr_pos[0], curr_pos[1]);
  dist_thr = new PDist2ThresholdOnTrianglesClass(curr_pos[0], curr_pos[1], law);
  filter = new TileErrFilterClass(mt, dist_thr);                              
  ex->MT_SetFilter( filter );

  ex->MT_ExtractTesselation();
  vn->ComputeVertexNormals(ex);
}

/* ------------------------------------------------------------------------ */

/* Termination. */
void endGame(void)
{
  if (ex) delete ex;
  if (mt) delete mt;
  if (focus) delete focus;
  if (law) delete law;
  if (dist_thr) delete dist_thr;
  if (filter) delete filter;
  if (vn) delete vn;
  fprintf(stderr,"Normal termination\n");
  exit(0);
}

/* ------------------------------------------------------------------------ */

int scaleHeights(float * factor)
{  float hor, tmp, zed;
   hor = mt->MT_MaxX() - mt->MT_MinX();
   if ( (tmp = mt->MT_MaxY() - mt->MT_MinY()) > hor) hor = tmp;
   zed = mt->MT_MaxZ() - mt->MT_MinZ();
   (*factor) = 0.05*(hor/zed);
   return 1;
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
  vn->ComputeVertexNormals(ex);
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
     float vpt, dist, vdist; /* parameters of dependency law */
     int depend;
     error_factor = v;
     law->TheLaw(&vpt, &dist, &vdist, &depend);    
     vdist = error_factor * (mt->MaxVertexField() - mt->MinVertexField());
     law->SetLaw(0.0, vpt, dist, vdist, depend); 
     ex->MT_ExtractTesselation();
     vn->ComputeVertexNormals(ex);
  }
}  

/* ------------------------------------------------------------------------ */
