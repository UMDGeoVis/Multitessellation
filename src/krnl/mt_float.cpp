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

/* ------------------------------ mt_float.c ------------------------------ */

#include "mt_float.h"

/* ------------------------------------------------------------------------ */
/*                          float ATTRIBUTE CLASS                             */
/* ------------------------------------------------------------------------ */

int MT_FloatTableClass :: MT_SetAttrNum(MT_INDEX n)
{
  att_Arr = (float *) calloc (n+1, sizeof(float));
  if (!att_Arr)
  {
     MT_ErrorC((char *)"Cannot allocate memory",(char *)"MT_FloatTableClass",(char *)"MT_SetAttrNum");
     return 0;
  }
  my_num = n;
  return 1;
}  

int MT_FloatTableClass :: 
    MT_ReadAttr(FILE * fd, MT_INDEX i, int file_encoding)
{
  char err_msg[MT_MAX_STRLEN]; 
  strcpy(err_msg, "float");
  strcat(err_msg, "expected");
  switch (file_encoding)
  {  case MT_BINARY_ENCODING:
        if (fread(&att_Arr[i], sizeof(float), 1, fd) != 1 )
        {
           MT_WarningC(err_msg,(char *)"MT_FloatTableClass",(char *)"MT_ReadAttr");
           return 0;
        }
        break;
     case MT_ASCII_ENCODING:
        if ( fscanf(fd, "%g", &att_Arr[i]) != 1 )
        {
           MT_WarningC(err_msg,(char *)"MT_FloatTableClass",(char *)"MT_ReadAttr");
           return 0;
          }
        break;   
  }
  if (att_Arr[i] < bound[0]) bound[0] = att_Arr[i];
  else if (att_Arr[i] > bound[1]) bound[1] = att_Arr[i];
  return 1;
}

void MT_FloatTableClass :: 
     MT_WriteAttr(FILE * fd, MT_INDEX i, int file_encoding)
{
  switch (file_encoding)
  {  case MT_BINARY_ENCODING:
        fwrite(&att_Arr[i], sizeof(float), 1, fd);
        break;
     case MT_ASCII_ENCODING:
        fprintf(fd, "%g", att_Arr[i]);
        fprintf(fd, "\n", att_Arr[i]);
        break;   
  }
}

/* ------------------------------------------------------------------------ */

int MT_FloatTableClass :: MT_ReadObjParams(FILE * fd)
{  
  int an; 
  if (!MT_SearchKeyword(fd, (char *)MT_OBJECT_KW)) return 0;
  if (fscanf(fd, "%d", &an)==1)
  {  if (an>=0)
     {  my_num =  an;
        return 1;
     }
  }   
  /* else */
  MT_ErrorC((char *)"Invalid object parameters",(char *)"MT_FloatTableClass",
            (char *)"MT_ReadObjParams");
  return 0;
}

int MT_FloatTableClass :: MT_ReadBody(FILE * fd, int file_encoding)
{  
  MT_SetAttrNum(my_num);
  return ( MT_AttrTableClass::MT_ReadBody(fd,file_encoding) );
}

void MT_FloatTableClass :: MT_WriteObjParams(FILE * fd)
{  
  MT_WriteKeyword(fd, (char *)MT_OBJECT_KW);
  fprintf(fd, " %d\n", my_num);
}

/* ------------------------------------------------------------------------ */

MT_FloatTableClass :: MT_FloatTableClass(void) 
{
//  MT_MessageC((char *)"","MT_FloatTableClass","Constructor");
  my_num = 0; 
  att_Arr = NULL;
  bound[0] = FLT_MAX;
  bound[1] = FLT_MIN;
}

MT_FloatTableClass ::  ~MT_FloatTableClass(void) 
{
//  MT_MessageC("","MT_FloatTableClass","Destructor");
  if (att_Arr) {  free (att_Arr);  }  }

/* ------------------------------------------------------------------------ */
/*                 BUILDING INTERFACE FOR float ATTRIBUTES                    */
/* ------------------------------------------------------------------------ */

MT_FloatBuildingInterfaceClass :: MT_FloatBuildingInterfaceClass( void )
{
  spec_attr = new MT_FloatTableClass ();
  MT_Init(spec_attr);
  spec_target_attr = NULL;
}

MT_FloatBuildingInterfaceClass :: ~MT_FloatBuildingInterfaceClass( void )
{
  if (spec_attr) delete spec_attr;
  spec_attr = NULL;  my_attr = NULL;
}

int MT_FloatBuildingInterfaceClass :: MT_MakeAttrValue(MT_INDEX i, float v)
{
  if (my_state!=MT_TRACING_STATE)
  {  MT_ErrorC((char *)"Attribute history not started, or already ended",
               (char *)"MT_FloatBuildingInterfaceClass", (char *)"MT_MakeAttrValue");
     return 0;
  }                     
  if (i == MT_NULL_INDEX)
  {  MT_Warning((char *)"Unknown MT element",(char *)"MT_MakeAttrValue"); 
     return 0;
  }
  spec_attr->MT_SetAttrValue(1, v);
  MT_WriteIndex(aux_fd, i, MT_AUX_ENCODING);
  my_attr->MT_WriteAttr(aux_fd, 1, MT_AUX_ENCODING);
  my_num++;
  return 1;
}


/* ------------------------------------------------------------------------ */
/*                           float VECTOR ATTRIBUTE                           */
/* ------------------------------------------------------------------------ */

int MT_FloatVectorTableClass :: 
    MT_ReadAttr(FILE * fd, MT_INDEX i, int file_encoding)
{
  int j;
  for (j=0; j<vector_size; j++)
  {
    switch (file_encoding)
    {  case MT_BINARY_ENCODING:
          if (fread(&av_Arr[j+i*vector_size], sizeof(float), 1, fd) != 1 )
          {
             MT_ErrorC((char *)"Cannot read vector element",
                       (char *)"MT_FloatVectorTableClass",(char *)"MT_ReadAttr");
             return 0;
          }
          break;
       case MT_ASCII_ENCODING:
          if ( fscanf(fd, "%g", &av_Arr[j+i*vector_size]) != 1 )
          {
             MT_ErrorC((char *)"Cannot read vector element",
                       (char *)"MT_FloatVectorTableClass",(char *)"MT_ReadAttr");
             return 0;
          }
          break;   
    }
  }
  return 1;
}

void MT_FloatVectorTableClass ::
     MT_WriteAttr(FILE * fd, MT_INDEX i, int file_encoding)
{
  int j;
  for (j=0; j<vector_size; j++)
  {   
    switch (file_encoding)
    {  case MT_BINARY_ENCODING:
          fwrite(&av_Arr[j+i*vector_size], sizeof(float), 1, fd);
          break;
       case MT_ASCII_ENCODING:
          fprintf(fd, "%g", av_Arr[j+i*vector_size]);
          if (j<vector_size-1)   fprintf(fd, " ");
          else    fprintf(fd, "\n");
          break;   
    }
  }
}

int MT_FloatVectorTableClass :: MT_ReadClassAndParams(FILE * fd)
{
  int s;
  if (!MT_SearchKeyword(fd, MT_ClassKeyword())) return 0;
  if (fscanf(fd, "%d", &s)==1)
  {  if (s == vector_size) return 1;  }
  /* else */
  MT_Error((char *)"Class parameters do not match with this object",
           (char *)"MT_FloatVectorTableClass::MT_ReadClassAndParams");
  return 0;
}

int MT_FloatVectorTableClass :: MT_ReadObjParams(FILE * fd)
{
  int an;
  if (!MT_SearchKeyword(fd, (char *)MT_OBJECT_KW)) return 0;
  if (fscanf(fd, "%d", &an)==1)
  {  if (an>=0)
     {  my_num = an;
        return 1;
     }
   }
   /* else */
   MT_Error((char *)"Invalid object parameters",
            (char *)"MT_FloatVectorTableClass::MT_ReadObjParams");
   return 0;
}

int MT_FloatVectorTableClass :: MT_ReadBody(FILE * fd, int file_encoding)
{  
  unsigned int i;
  /* allocate float array */
  if (!MT_SetAttrNum(my_num)) return 0;
  /* read the values and fill the array */
  for (i=1; i<my_num+1; i++)
  {  if (!MT_ReadAttr(fd, i, file_encoding)) return 0;  }
  return 1;
}

void MT_FloatVectorTableClass :: MT_WriteClassAndParams(FILE * fd)
{
  MT_WriteKeyword(fd, MT_ClassKeyword());
  fprintf(fd, " %d\n", vector_size);
}
  
void MT_FloatVectorTableClass :: MT_WriteObjParams(FILE * fd)
{  
  MT_WriteKeyword(fd, (char *)MT_OBJECT_KW);
  fprintf(fd, " %d\n", my_num);
}

void MT_FloatVectorTableClass :: MT_WriteBody(FILE * fd, int file_encoding)
{  
  unsigned int i;
  for (i=1; i<my_num+1; i++)
  {  MT_WriteAttr(fd, i, file_encoding);  }
}

/* ------------------------------------------------------------------------ */

int MT_FloatVectorTableClass :: MT_SetAttrNum(MT_INDEX n)
{
   av_Arr = (float *) calloc ( (n+1)*vector_size, sizeof(float));
   if (!av_Arr)
   {  MT_ErrorC((char *)"Failed allocation",
                (char *)"MT_FloatVectorTableClass", (char *)"MT_SetAttrNum");
      return 0;
   }
   my_num = n;
   return 1;
}  

void MT_FloatVectorTableClass :: MT_SetAttrVector(MT_INDEX i, float *v)
{ 
  int j;
  for (j=0; j<vector_size; j++)
  {  av_Arr[j+i*vector_size] = v[j];  }
}
  
/* ------------------------------------------------------------------------ */

MT_FloatVectorTableClass :: MT_FloatVectorTableClass(int num) 
{  my_num = 0; 
   vector_size = num;
   av_Arr = NULL;
}
       
MT_FloatVectorTableClass :: ~MT_FloatVectorTableClass(void)
{  if (my_num) free(av_Arr);  av_Arr = NULL;  }

/* ------------------------------------------------------------------------ */
/*               BUILDING INTERFACE FOR float VECTOR ATTRIBUTES               */  
/* ------------------------------------------------------------------------ */

MT_FloatVectorBuildingInterfaceClass :: MT_FloatVectorBuildingInterfaceClass( int num )
{
  spec_attr = new MT_FloatVectorTableClass (num);
  MT_Init(spec_attr);
  spec_target_attr = NULL;
}

MT_FloatVectorBuildingInterfaceClass :: ~MT_FloatVectorBuildingInterfaceClass( void )
{
  if (spec_attr) free (spec_attr);
  my_attr = NULL; spec_attr = NULL;
}

int MT_FloatVectorBuildingInterfaceClass :: MT_MakeAttrVector(MT_INDEX i, float *v)
{
  if (i == MT_NULL_INDEX)
  {  MT_Warning((char *)"Unknown MT element",(char *)"MT_MakeAttrVector");
     return 0;
  }
  spec_attr->MT_SetAttrVector(1, v);
  MT_WriteIndex(aux_fd, i, MT_AUX_ENCODING);
  my_attr->MT_WriteAttr(aux_fd, 1, MT_AUX_ENCODING);
  my_num++;
  return 1;
}

/* ------------------------------------------------------------------------ */

