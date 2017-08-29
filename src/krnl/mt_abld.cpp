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

/* ---------------------------- mt_abld.c --------------------------------- */

#include "mt_abld.h"

/* ------------------------------------------------------------------------ */
/*                         Local macros and variables                       */
/* ------------------------------------------------------------------------ */

/*
Prefix and extension of the auxiliary file name.
*/
#define MT_AUX_PRF "attr_"
#define MT_AUX_EXT ".aux"

/* ------------------------------------------------------------------------ */
/*                          Auxiliary functions                             */
/* ------------------------------------------------------------------------ */

MT_AttrBuildingInterfaceClass :: MT_AttrBuildingInterfaceClass(void) 
{  my_attr = NULL;
   target_attr = NULL;
   table3 = NULL;
   my_state = MT_INITIAL_STATE;
}

void MT_AttrBuildingInterfaceClass :: MT_Init(MT_AttrTableClass * the_attr)
{  my_attr = the_attr;
   my_attr->MT_SetAttrNum(1);
}

/* ------------------------------------------------------------------------ */
/*             Functions for tracing and converting attributes              */
/* ------------------------------------------------------------------------ */

int MT_AttrBuildingInterfaceClass :: MT_StartAttrHistory(void)
{
  char base_name[MT_MAX_STRLEN];
  
  if (my_state!=MT_INITIAL_STATE)
  {  MT_Error((char *)"An attribute history or table already in memory",
              (char *)"MT_AttrBuildingInterface::MT_StartAttrHistory");
     return 0;
  }
  sprintf(base_name, "%d", (this));
  strcpy(aux_name,MT_AUX_PRF);
  strcat(aux_name,base_name);
  strcat(aux_name,MT_AUX_EXT);
  aux_fd = fopen(aux_name,"w");
  if (!aux_fd)
  {  MT_Error((char *)"Cannot open auxiliary file", 
              (char *)"MT_AttrBuildingInterface::MT_StartAttrHistory");
     return 0;
  }
  my_num = 0;
  my_state = MT_TRACING_STATE;
  return 1;
}

int MT_AttrBuildingInterfaceClass :: MT_EndAttrHistory(void)
{
  if (my_state!=MT_TRACING_STATE)
  {  MT_Error((char *)"Attribute history not started, or already ended",
              (char *)"MT_AttrBuildingInterface::MT_EndAttrHistory");
     return 0;
  }
  fclose(aux_fd);
  my_state = MT_TRACED_STATE;
  return 1;
}

/* ------------------------------------------------------------------------ */
/*                           Read/write functions                           */
/* ------------------------------------------------------------------------ */

int MT_AttrBuildingInterfaceClass :: MT_ReadObjParams(FILE * fd)
{
  if (!MT_SearchKeyword(fd, (char *)MT_OBJECT_KW)) return 0;
  if (fscanf(fd, "%d ", &my_num) == 1)
  {  if (MT_SearchKeyword(fd, target_attr->MT_ClassKeyword()))
        return 1;
  }
  /* else */
  MT_Error((char *)"Invalid attribute type or parameters",
           (char *)"MT_AttrBuildingInterfaceClass::MT_ReadObjParams");
  return 0;
}

/* ------------------------------------------------------------------------ */

int MT_AttrBuildingInterfaceClass :: MT_ReadBody(FILE * fd, int file_encoding)
{
  unsigned int j;
  MT_INDEX i;

  max_num = 0;
  target_attr->MT_SetAttrNum(my_num);
  for (j=0; j<my_num; j++)
  {  
     MT_ReadIndex(fd, &i, file_encoding);
     if (i>max_num) max_num = i;
     if (table3)
        target_attr->MT_ReadAttr(fd, table3->LookAssoc(i), file_encoding);
     else
        target_attr->MT_ReadAttr(fd, i, file_encoding);
  }
  if (max_num != my_num) target_attr->MT_WeakSetAttrNum(max_num);
  return 1;
}

/* ------------------------------------------------------------------------ */

int MT_AttrBuildingInterfaceClass :: MT_Read(FILE * fd)
{
  if (my_state!=MT_INITIAL_STATE)
  {  MT_Warning((char *)"An attribute history is already in memory",
                (char *)"MT_AttrBuildingInterface::MT_Read");
     return 0;
  }
  if (!target_attr)
  {  MT_Warning((char *)"A target attribute table must be set first",
                (char *)"MT_AttrBuildingInterface::MT_Read");
     return 0;
  }
  if ( MT_PersistentClass::MT_Read(fd) )
  {  target_attr->MT_SetDescription(my_descr);
     my_state = MT_FINAL_STATE;
     return 1;
  }
  return 0;
}

int MT_AttrBuildingInterfaceClass :: MT_Read(char * fname)
{
  FILE * fd = fopen(fname,"r");
  int ok = 0;
  if (fd) ok = MT_Read(fd);
  fclose(fd);
  return ok;
}

/* ------------------------------------------------------------------------ */

void MT_AttrBuildingInterfaceClass :: MT_WriteObjParams(FILE * fd)
{
  MT_WriteKeyword(fd, (char *)MT_OBJECT_KW);
  fprintf(fd, " %d ",my_num);
  MT_WriteKeyword(fd, my_attr->MT_ClassKeyword());
  fprintf(fd,"\n");
}

/* ------------------------------------------------------------------------ */

void MT_AttrBuildingInterfaceClass :: 
     MT_WriteBody(FILE * fd, int file_encoding)
{
  MT_INDEX i, j;

  if (my_state!=MT_TRACED_STATE)
  {  MT_Error((char *)"An attribute history must be traced first",
              (char *)"MT_AttrBuildingInterface::MT_WriteBody");
  }

  /* move attributes from the auxiliary file to the final file, if
     attributes are for tiles, attributes of temporary tiles are omitted */
  aux_fd = fopen(aux_name,"r");
  if (!aux_fd)
  {  MT_Error((char *)"Cannot open auxiliary file",
              (char *)"MT_AttrBuildingInterface::MT_WriteBody");
  }
  for (i=0;i<my_num;i++)
  {
    MT_ReadIndex(aux_fd, &j, MT_AUX_ENCODING);
    my_attr->MT_ReadAttr(aux_fd, 1, MT_AUX_ENCODING);
    MT_WriteIndex(fd, j, file_encoding);
    my_attr->MT_WriteAttr(fd, 1, file_encoding);
    if (file_encoding==MT_ASCII_ENCODING) fprintf(fd, "\n");
  }
  fclose(aux_fd);
  /* empty the auxiliary file */
  aux_fd = fopen(aux_name,"w");
  fclose(aux_fd);
  /* delete the auxiliary attribute table */
  delete my_attr; my_attr = NULL;
}

/* ------------------------------------------------------------------------ */

void MT_AttrBuildingInterfaceClass :: MT_Write(FILE * fd, int file_encoding)
{
  if (my_state!=MT_TRACED_STATE)
  {  MT_Warning((char *)"An attribute history must be traced first",
                (char *)"MT_AttrBuildingInterface::MT_Write");
  }
  else
  {  MT_PersistentClass::MT_Write(fd,file_encoding);
     my_state = MT_INITIAL_STATE;
  }
}

int MT_AttrBuildingInterfaceClass ::
    MT_Write(char * fname, int file_encoding)
{
  FILE * fd = fopen(fname,"w");
  int ok = 0;
  if (fd) {  MT_Write(fd,file_encoding);  ok = 1;  }
  fclose(fd);
  return ok;
}

/* ------------------------------------------------------------------------ */

int MT_AttrBuildingInterfaceClass :: MT_AttrConvert(void)
{
  MT_INDEX i, j, k;
  
  if (my_state!=MT_TRACED_STATE)
  {  MT_Error((char *)"An attribute history must be traced first",
              (char *)"MT_AttrBuildingInterface::MT_AttrConvert");
     return 0;
  }
  target_attr->MT_SetAttrNum(my_num);
  max_num = 0;

  /* read attributes from the auxiliary file, if attributes are for 
     tiles, then table3 is used to skip attributes of temporary tiles */
  aux_fd = fopen(aux_name,"r");
  if (!aux_fd)
  {  MT_Error((char *)"Cannot open auxiliary file",
              (char *)"MT_AttrBuildingInterface::MT_AttrConvert");
     return 0;
  }
  for (i=0;i<my_num;i++)
  {
    MT_ReadIndex(aux_fd, &j, MT_AUX_ENCODING);
    if (table3)
       k = table3->LookAssoc(j);
    else 
       k = j;
    if (k != MT_NULL_INDEX)
    {  /* tile is not temporary */
       target_attr->MT_ReadAttr(aux_fd, k, MT_AUX_ENCODING);
       if (k>max_num) max_num = k;
    }
  }
  if (max_num != my_num) target_attr->MT_WeakSetAttrNum(max_num);
  fclose(aux_fd);
  /* empty the auxiliary file */
  aux_fd = fopen(aux_name,"w");
  fclose(aux_fd);
  target_attr->MT_SetDescription(my_descr);
  my_state = MT_INITIAL_STATE;
  return 1;
}

/* ------------------------------------------------------------------------ */
