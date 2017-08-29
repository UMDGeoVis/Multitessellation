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

/* ------------------------------ mt_prst.c ------------------------------- */

#include "mt_prst.h"

/* ------------------------------------------------------------------------ */
/*             A PERSISTENT OBJECT CLASS IN THE MT LIBRARY                  */
/* ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */
/*                           Object description                             */
/* ------------------------------------------------------------------------ */

void MT_PersistentClass :: MT_SetDescription(char * s)
{
  int i;
  for (i=0; ( (i<MT_DESCR_LEN-1) && (s[i]!='\0') ); i++)
  {  my_descr[i] = s[i];  }
  my_descr[i] = '\0';
  if (s[i]!='\0') MT_Warning((char *)"Description too long, truncated",
                             (char *)"MT_PersistentClass::MT_SetDescription");
}

void MT_PersistentClass :: MT_TheDescription(char * s)
{
  int i;     
  for (i=0; (my_descr[i]!='\0'); i++)  {  s[i] = my_descr[i];  }
}

MT_PersistentClass :: MT_PersistentClass ( void )
{
  MT_Message((char *)"",(char *)"MT_PersistentClass Constructor");
  MT_SetDescription((char *)"");
}

/* ------------------------------------------------------------------------ */
/*                           Reading functions                              */
/* ------------------------------------------------------------------------ */

int MT_PersistentClass :: 
    MT_SearchKeyword(FILE * fd, char * keyword, int load_descr)
{
   int i = 0; /* cursor on keyword */
   int j = 0; /* cursor on my_descr */ 
   char c;
   while ( (i<strlen(keyword)) && (fscanf(fd,"%c",&c)==1) )
   {  if (c==keyword[i]) i++;
      else 
      {  if ( (load_descr) && ((j+i+1)<MT_DESCR_LEN-1) )
         {  int k;
            /* if a non-null prefix of the keyword has been read,
               copy such characters into my_descr */
            for (k=0; k<i; k++)  {  my_descr[j++] = keyword[k];  }
            /* then copy the current character into my_descr */
            my_descr[j++] = c;
         }
         i = 0;
      }
   }
   if (load_descr) my_descr[j] = '\0';
   return (i==strlen(keyword));
}

int MT_PersistentClass :: MT_ReadFileKw(FILE * fd)
{  return MT_SearchKeyword(fd, (char *)MT_FILE_KW);  }
  
inline int MT_PersistentClass :: MT_ReadClassAndParams(FILE * fd)
{
  if (!MT_SearchKeyword(fd, MT_ClassKeyword())) return 0;
  return 1;
}

int MT_PersistentClass :: 
    MT_ReadIndex(FILE * fd, MT_INDEX * i, int file_encoding)
{
  switch (file_encoding)
  {
    case MT_BINARY_ENCODING:
        if (fread(i, sizeof(MT_INDEX), 1, fd) == 1) return 1;
        break;
    case MT_ASCII_ENCODING:
        if (fscanf(fd, "%d", i) == 1) return 1;
        break;
  }
  MT_Error((char *)"Index expected",(char *)"MT_PersistentClass::MT_ReadIndex");
  return 0;
}

int MT_PersistentClass :: 
    MT_ReadCount(FILE * fd, int * i, int file_encoding)
{
  switch (file_encoding)
  {
    case MT_BINARY_ENCODING:
        if (fread(i, sizeof(int), 1, fd) == 1) return 1;
        break;
    case MT_ASCII_ENCODING:
        if (fscanf(fd, "%d", i) == 1) return 1;
        break;
  }
  MT_Error((char *)"Integer expected",(char *)"MT_PersistentClass::MT_ReadCount");
  return 0;
}

int MT_PersistentClass :: MT_ReadObjParams(FILE * fd)
{
  if (!MT_SearchKeyword(fd, (char *)MT_OBJECT_KW)) return 0;
  return 1;
}

int MT_PersistentClass :: MT_ReadDescription(FILE * fd)
{
  int l;
  if (!MT_SearchKeyword(fd, (char *)MT_START_DESCRIPTION_KW, 0)) return 0;
  if (!MT_SearchKeyword(fd, (char *)MT_END_DESCRIPTION_KW, 1)) return 0;
  /* remove last character if eoln */
  if ( (l = strlen(my_descr)) && my_descr[l-1]=='\n')
      my_descr[l-1] = '\0';
  return 1;
}

int MT_PersistentClass ::
    MT_ReadEncoding(FILE * fd, int * file_encoding)
{
  char aux[20];
  if (!MT_SearchKeyword(fd, (char *)MT_ENCODE_KW)) return 0;
  if (fscanf(fd, "%20s", aux) != EOF)
  {  if (strcmp(aux,MT_ASCII_KW)==0) 
        (*file_encoding) = MT_ASCII_ENCODING;
     else if (strcmp(aux,MT_BINARY_KW)==0) 
             (*file_encoding) = MT_BINARY_ENCODING;
          else 
          {  MT_Error((char *)"Invalid encoding specification", 
                      (char *)"MT_PersistentClass::MT_ReadEncoding");
             return 0;
          }
  }
  if ((*file_encoding)==MT_BINARY_ENCODING)
  {
    /* read the end-of-line (and possible separators before it)
       in order to reach the beginning of the body */
    char c = ' ';
    while (c!='\n') fscanf(fd, "%c", &c);
  }
  return 1;
}

int MT_PersistentClass :: MT_Read(FILE * fd)
{
  int file_encoding;
  if (! MT_ReadFileKw(fd) ) 
  {   MT_Warning((char *)"Failed MT_ReadFileKw",(char *)"MT_PersistentClass::MT_Read");
      return 0;
  }
  if (! MT_ReadClassAndParams(fd) ) 
  {   MT_Warning((char *)"Failed MT_ReadClassAndParams",(char *)"MT_PersistentClass::MT_Read");
      return 0;
  }
  if (! MT_ReadObjParams(fd) ) 
  {   MT_Warning((char *)"Failed MT_ReadObjParams",(char *)"MT_PersistentClass::MT_Read");
      return 0;
  }
  if (! MT_ReadDescription(fd) ) 
  {   MT_Warning((char *)"Failed MT_ReadDescription",(char *)"MT_PersistentClass::MT_Read");
      return 0;
  }
  if (! MT_ReadEncoding(fd, &file_encoding) ) 
  {   MT_Warning((char *)"Failed MT_ReadEncoding",(char *)"MT_PersistentClass::MT_Read");
      return 0;
  }
  if (! MT_ReadBody(fd, file_encoding) ) 
  {   MT_Warning((char *)"Failed MT_ReadBody",(char *)"MT_PersistentClass::MT_Read");
      return 0;
  }
  return 1;
}

int MT_PersistentClass :: MT_Read(char * fname)
{
  FILE * fd = fopen(fname,"r");
  int ok = 0;
  if (fd) ok = MT_Read(fd);
  fclose(fd);
  return ok;
}

/* ------------------------------------------------------------------------ */
/*                            Writing functions                             */
/* ------------------------------------------------------------------------ */

void MT_PersistentClass :: MT_WriteKeyword(FILE * fd, char * keyword)
{  fprintf(fd, "%s", keyword);  }

void MT_PersistentClass :: MT_WriteFileKw(FILE * fd)
{
  MT_WriteKeyword(fd, (char *)MT_FILE_KW);
  fprintf(fd, "\n");
}

void MT_PersistentClass ::
     MT_WriteIndex(FILE * fd, MT_INDEX i, int file_encoding)
{
  switch (file_encoding)
  {  case MT_BINARY_ENCODING:
       fwrite(&i, sizeof(MT_INDEX), 1, fd); 
       break;
     case MT_ASCII_ENCODING:
       fprintf(fd, "%d ", i);
       break;
  }
}

void MT_PersistentClass ::
     MT_WriteCount(FILE * fd, int i, int file_encoding)
{
  switch (file_encoding)
  {  case MT_BINARY_ENCODING:
       fwrite(&i, sizeof(int), 1, fd); 
       break;
     case MT_ASCII_ENCODING:
       fprintf(fd, "%d ", i);
       break;
  }
}

void MT_PersistentClass :: MT_WriteClassAndParams(FILE * fd)
{  
  MT_WriteKeyword(fd, MT_ClassKeyword());
  fprintf(fd, "\n");
}

void MT_PersistentClass :: MT_WriteObjParams(FILE * fd)
{  
  MT_WriteKeyword(fd, (char *)MT_OBJECT_KW);
  fprintf(fd, "\n");
}
  
void MT_PersistentClass :: MT_WriteDescription(FILE * fd)
{ 
  MT_WriteKeyword(fd, (char *)MT_START_DESCRIPTION_KW);
  /* if description is not empty and does not start with eoln,
     add eoln before it */
  if ( (my_descr[0]!='\0') && (my_descr[0]!='\n') ) fprintf(fd, "\n");
  fprintf(fd, "%s\n",my_descr);
  MT_WriteKeyword(fd, (char *)MT_END_DESCRIPTION_KW);
  fprintf(fd, "\n");
}

void MT_PersistentClass :: MT_WriteEncoding(FILE * fd, int file_encoding)
  {
    MT_WriteKeyword(fd, (char *)MT_ENCODE_KW);
    fprintf(fd," ");
    switch (file_encoding)
    {  case MT_ASCII_ENCODING: MT_WriteKeyword(fd, (char *)MT_ASCII_KW); break;
       case MT_BINARY_ENCODING: MT_WriteKeyword(fd, (char *)MT_BINARY_KW); break;
    }
    fprintf(fd,"\n");
  }
  
void MT_PersistentClass ::
     MT_Write(FILE * fd, int file_encoding)
{
  MT_WriteFileKw(fd);
  MT_WriteClassAndParams(fd);
  MT_WriteObjParams(fd);
  MT_WriteDescription(fd);
  MT_WriteEncoding(fd, file_encoding);
  MT_WriteBody(fd, file_encoding);
  fprintf(fd, "\n");           
}

int MT_PersistentClass ::
    MT_Write(char * fname, int file_encoding)
{
  FILE * fd = fopen(fname,"w");
  int ok = 0;
  if (fd) {  MT_Write(fd,file_encoding);  ok = 1;  }
  fclose(fd);
  return ok;
}

/* ------------------------------------------------------------------------ */
