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

/* ----------------------------- binascii.c ------------------------------ */

#include "mt_multi.h"

/* ----------------------------------------------------------------------- */
/*                CONVERT AN MT FILE INTO BINARY/ASCII FORMAT              */
/* ----------------------------------------------------------------------- */

/*
This program reads an MT and writes it by using either the binary or the
ascii format, as specified by the macro WHICH_ENCODING.
*/

/* ----------------------------------------------------------------------- */
/*                                 MACRO                                   */
/* ----------------------------------------------------------------------- */

/*
This macro decides the output format of the conversion. It must be set to
either MT_ASCII_ENCODING or MT_BINARY_ENCODING when compiling the program.
*/

/*******
#define WHICH_ENCODING MT_ASCII_ENCODING
#define WHICH_ENCODING MT_BINARY_ENCODING
*******/
  
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
/*                                    MAIN                                 */
/* ----------------------------------------------------------------------- */
                               
int main (int argc, char ** argv)
{
  char in_name[MT_MAX_STRLEN];
  char out_name[MT_MAX_STRLEN];
  FILE * in_fd, * out_fd;
  CheckDimensionsClass * chk;
  MT_MultiTesselation mt;  /* the MT */

  if (argc < 3)
  {
    switch (WHICH_ENCODING)
    { case MT_ASCII_ENCODING:
        fprintf(stderr,"Convert an MT into ascii format\n");
        break;
      case MT_BINARY_ENCODING:
        fprintf(stderr,"Convert an MT into binary format\n");
        break;
    }
    fprintf(stderr,"Need parameters: input_file output_file\n");
    exit(1);
  }       
   
  strcpy(in_name,argv[1]);
  strcpy(out_name,argv[2]);
  fprintf(stderr,"Loading from %s, writing to %s\n", in_name, out_name);
  in_fd = fopen(in_name,"r");
  if (!in_fd)  MT_FatalError("Cannot open input file","main");
  out_fd = fopen(out_name,"w");
  if (!out_fd)  MT_FatalError("Cannot open output file","main");
  
  /* Use chk to read the MT dimensions, close and re-open input file */
  chk = new CheckDimensionsClass();
  chk->MT_Read(in_fd);
  fclose(in_fd);
  in_fd = fopen(in_name,"r");
  
  /* Create mt with the correct dimensions just read */
  mt = new MT_MultiTesselationClass(chk->MT_VertexDim(), chk->MT_TileDim());
  
  /* Read the MT */
  mt->MT_Read(in_fd);
  
  /* Write the MT in the specified format */
  mt->MT_Write(out_fd,WHICH_ENCODING);

  fclose(in_fd);
  fclose(out_fd);
  delete chk;
}

/* ----------------------------------------------------------------------- */
