#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "mt_extra.h"  /* the extractor of tesselations */
#include "mt_out.h"
#include "fltmisc.h"   /* the resolution filter conditions */

/* Surface MT. */
MT_MultiTesselation mt = NULL;

/* Extractor. */
MT_Extractor ex = NULL;

/* Input file names. */
char file_name[MT_MAX_STRLEN] = "";  /* for the MT */

/* Extraction filter. */
TriangleArea3FilterClass * filter = NULL;

/* Threshold used inside resolution filter. */
UnifThresholdClass * unif_thr;

/* Factor for area threshold: 1 = min, 0 = max. */
float area_factor = 1.0, minArea, maxArea;

int main(int argc, char * argv[]) {
  FILE *fd;
  float quality;

  if (argc<4) { /* no command line options */
    fprintf(stderr,"Syntax: %s XXX YY ZZZ\n",argv[0]);
    fprintf(stderr,"Load MT from file XXX.mtf, extract a mesh at quality YY, and save it on file ZZZ\n");
    fprintf(stderr,"For instance: %s ../data_demo1/bunny 3.5 bunny_mesh.off\n",argv[0]);
    fprintf(stderr,"Quality is in the range 0.0 (min) - 10.0 (max)\n");
    exit(1);
  }
  /* else */
  strcpy(file_name,argv[1]);
  strcat(file_name,".mtf");    /* MT file */

  quality = atof(argv[2]);
  
  fprintf(stderr, "Loading MT from file %s...\n", file_name);
  fd = fopen(file_name,"r");
  if(!fd)
    MT_FatalError("Cannot open MT file","loadMT");
  mt = new MT_MultiTesselationClass(3,2);
  if (mt->MT_Read(fd) != 1)
    MT_FatalError("Cannot read MT from MT file","loadMT");
  fclose(fd);
  printf("...MT loaded\n");

  ex = new MT_StaticExtractorClass(mt);
  unif_thr = new UnifThresholdClass(area_factor);
  filter = new TriangleArea3FilterClass( unif_thr );
  filter->MT_MinMax(mt, &minArea, &maxArea);
  printf("Tile area - min %f - max %f\n", minArea, maxArea);

  if (quality <= 0.0)
    area_factor = maxArea;
  else if (10.0 <= quality)
    area_factor = minArea;
  else
    area_factor = (maxArea-minArea)/pow(2.0,quality)+minArea;

  unif_thr->SetThreshold(area_factor);  
  ex->MT_SetFilter( filter );
  ex->MT_ExtractTesselation();

  fprintf(stderr, "Saving file %s...\n", argv[3]);
  fd  = fopen(argv[3], "w");
  if(!fd)
  {
    fprintf(stderr, "Cannot open/create %s file\n", argv[3]);
    exit(1);
  }
  else
  {
    MT_WriteAllIndexed(fd, ex, MT_NO_FLAGS);
    fclose(fd);
    printf("...done\n");
  }
  return 0;
}
