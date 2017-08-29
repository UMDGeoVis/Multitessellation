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

/* ------------------------------ mt_bool.c ------------------------------- */

#include "mt_bool.h"

/* ------------------------------------------------------------------------ */
/*                         BOOLEAN ATTRIBUTES                               */
/* ------------------------------------------------------------------------ */

int MT_BoolTableClass :: MT_SetAttrNum(MT_INDEX n)
{  return ( MT_SetBitNum(n) );  }

/* ------------------------------------------------------------------------ */

int MT_BoolTableClass :: 
    MT_ReadAttr(FILE * fd, MT_INDEX i, int file_encoding)
{
  int b;
  switch (file_encoding)
  {  case MT_BINARY_ENCODING:
        if (fread(&b, sizeof(int), 1, fd) != 1 )
        {
           MT_Error((char *)"Read error",(char *)"MT_BoolTableClass::MT_ReadAttr");
           return 0;
        }
        break;
     case MT_ASCII_ENCODING:
        if ( fscanf(fd, "%d", &b) != 1 )
        {
           MT_Error((char *)"Read error",(char *)"MT_BoolTableClass::MT_ReadAttr");
           return 0;
        }
        break;   
  }
  if ( (b!=0) && (b!=1) )
  {
     MT_Error((char *)"Boolean expected",(char *)"MT_BoolTableClass::MT_ReadAttr");
     return 0;
  }
  MT_SetBit(i,b);
  return 1;
}

void MT_BoolTableClass :: 
     MT_WriteAttr(FILE * fd, MT_INDEX i, int file_encoding)
{
  int b = MT_TheBit(i);
  switch (file_encoding)
  {  case MT_BINARY_ENCODING:
        fwrite(&b, sizeof(int), 1, fd);
        break;
     case MT_ASCII_ENCODING:
        fprintf(fd, "%d\n", b);
        break;   
  }
}

/* ------------------------------------------------------------------------ */

int MT_BoolTableClass :: 
    MT_ReadBlock(FILE * fd, MT_INDEX block, int file_encoding)
{
  switch (file_encoding)
  {  case MT_BINARY_ENCODING:
        if (fread(&bit_Arr[block], sizeof(MT_BLOCK), 1, fd) == 1 )
           return 1;
        break;
     case MT_ASCII_ENCODING:
        if ( fscanf(fd, MT_BLOCK_STRING, &bit_Arr[block]) == 1 )
           return 1;
        break;   
  }
  MT_Error((char *)"Read error",(char *)"MT_BoolTableClass::MT_ReadBlock");
  return 0;
}

void MT_BoolTableClass :: 
     MT_WriteBlock(FILE * fd, MT_INDEX block, int file_encoding)
{
  switch (file_encoding)
  {  case MT_BINARY_ENCODING:
        fwrite(&bit_Arr[block], sizeof(MT_BLOCK), 1, fd);
        break;
     case MT_ASCII_ENCODING:
        fprintf(fd, MT_BLOCK_STRING, bit_Arr[block]); 
        fprintf(fd,"\n");
        break;   
  }
}

/* ------------------------------------------------------------------------ */

int MT_BoolTableClass ::
    MT_ReadBody(FILE * fd, int file_encoding)
{
  unsigned int i;
  MT_SetAttrNum(my_num);
  for (i=1; i<=blk_Num; i++)
  {
     if (!MT_ReadBlock(fd, i, file_encoding)) return 0;
  }
  return 1;
}

void MT_BoolTableClass ::
     MT_WriteBody(FILE * fd, int file_encoding)
{
  unsigned int i;
  for (i=1; i<=blk_Num; i++)
  {  MT_WriteBlock(fd, i, file_encoding);  }
}

/* ------------------------------------------------------------------------ */

int MT_BoolTableClass :: MT_ReadObjParams(FILE * fd)
{  int an;
   if (!MT_SearchKeyword(fd, (char *)MT_OBJECT_KW)) return 0;
   if (fscanf(fd, "%d", &an)==1)
   {  if (an>=0)
      {  my_num = an;
         return 1;
      }
   }
   /* else */
   MT_Error((char *)"Invalid object parameters",
            (char *)"MT_BoolTableClass::MT_ReadObjParams");
   return 0;
}

void MT_BoolTableClass :: MT_WriteObjParams(FILE * fd)
{  
  MT_WriteKeyword(fd, (char *)MT_OBJECT_KW);
  fprintf(fd, " %d\n", my_num);
}

/* ------------------------------------------------------------------------ */

MT_BoolTableClass :: MT_BoolTableClass(void) : MT_BitClass()  { }

/*
This seems to do nothing but it calls the destructor of the superclass.
*/
MT_BoolTableClass ::  ~MT_BoolTableClass(void)  { }
/********* CONTROLLA SE VERO **********/

/* ------------------------------------------------------------------------ */
/*                 BUILDING INTERFACE FOR BIT ATTRIBUTES                    */
/* ------------------------------------------------------------------------ */

MT_BoolBuildingInterfaceClass :: MT_BoolBuildingInterfaceClass( void )
{
  spec_attr = new MT_BoolTableClass ();
  MT_Init(spec_attr);
  spec_target_attr = NULL;
}

MT_BoolBuildingInterfaceClass :: ~MT_BoolBuildingInterfaceClass( void )
{
  if (spec_attr) delete spec_attr;
  spec_attr = NULL;  my_attr = NULL;
}

int MT_BoolBuildingInterfaceClass :: MT_MakeBoolAttr(MT_INDEX i, int b)
{
  if (my_state!=MT_TRACING_STATE)
  {  MT_Error((char *)"Attribute history not started, or already ended",
               (char *)"MT_BoolBuildingInterfaceClass::MT_MakeBoolAttr");
     return 0;
  }   
  if (i == MT_NULL_INDEX)
  {  MT_Warning((char *)"Unknown MT element",(char *)"MT_MakeAttrValue");
     return 0;
  }
  spec_attr->MT_SetAttrValue(1, b);
  MT_WriteIndex(aux_fd, i, MT_AUX_ENCODING);
  my_attr->MT_WriteAttr(aux_fd, 1, MT_AUX_ENCODING);
  my_num++;
  return 1;
}

/* ------------------------------------------------------------------------ */
