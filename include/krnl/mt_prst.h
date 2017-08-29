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

/* ------------------------------ mt_prst.h ------------------------------- */

#ifndef MT_PERSISTENT_INCLUDED
#define MT_PERSISTENT_INCLUDED

#include "mt_glob.h"

/* ------------------------------------------------------------------------ */
/*             A PERSISTENT OBJECT CLASS IN THE MT LIBRARY                  */
/* ------------------------------------------------------------------------ */

/*
This file contains the interface of a generic persistent class of objects 
in the MT library, called MT_PersistentClass.
As a persistent class, we mean a class of object that can be read from /
written to a file.
MT_PersistentClass is a virtual class.
The interface contains functions to read and write an object of the class.
*/

/* ------------------------------------------------------------------------ */
/*                            MT FILE FORMAT                                */
/* ------------------------------------------------------------------------ */

/*
A file containing a persistent object starts with a header specifying:
- a keyword specifying that it is an MT file
- the class of object contained in the file
- class-specific parameters used in the class constructor
- parameters used to allocate internal structures of this object
- text describing the object contained in the file (optional, and
  ignored on reading)
- the type of encoding (binary or ascii) for the body
The header is followed by a body containing the data for the object.
*/

/* ------------------------------------------------------------------------ */
/*                              MACROS                                      */
/* ------------------------------------------------------------------------ */

/* 
Keyword of all MT files.
*/
#define MT_FILE_KW "MT-FILE"

/*
Keywords for starting the various sections of an MT file.
*/
#define MT_CLASS_KW "MT-CLASS"
#define MT_OBJECT_KW "MT-OBJECT"
#define MT_START_DESCRIPTION_KW "MT-START-DESCRIPTION"
#define MT_END_DESCRIPTION_KW "MT-END-DESCRIPTION"
#define MT_ENCODE_KW "MT-ENCODE"
#define MT_BODY_KW "MT-BODY"

/* 
Keywords for the type of encoding.
*/
#define MT_ASCII_KW "ASCII"
#define MT_BINARY_KW "BINARY"
#define MT_ASCII_ENCODING 0
#define MT_BINARY_ENCODING 1

/*
Encoding format for auxiliary files.
*/
//#ifdef
#define MT_AUX_ENCODING MT_ASCII_ENCODING
//#else
//#define MT_AUX_ENCODING MT_BINARY_ENCODING
//#endif

/* ------------------------------------------------------------------------ */
/*                          THE PERSISTENT CLASS                            */
/* ------------------------------------------------------------------------ */

typedef class MT_PersistentClass * MT_Persistent;

class MT_PersistentClass
{

  protected:

  /*
  Variable containing the textual description of this object.
  */
  char my_descr[MT_DESCR_LEN];

/* ------------------------------------------------------------------------ */
/*                  Class keyword and object description                    */
/* ------------------------------------------------------------------------ */

  public:

  /*
  Keyword for the class of objects contained in the file.
  It will be implemented in the subclasses.
  */
  virtual char * MT_ClassKeyword(void) = 0;
  
  /*
  Set the textual description of this object. String s must not be longer
  than MT_DESCR_LEN characters. If longer, extra characters are ignored.
  If a '\n' is desired to appear before and after the description, these 
  must be specified as part of the description itself.
  */
  void MT_SetDescription(char * s);

  /*
  Copy the textual description of this object into string s.
  String s must be long enough. A length of MT_DESCR_LEN characters 
  (plus one for the final '\0') is always sufficient.
  */
  void MT_TheDescription(char * s);

  /*
  Return a pointer to the string containing 
  the textual description of this object. 
  */
  char * MT_TheDescription(void) {  return (my_descr);  }

  protected:
  
  /*
  This creator can only be used in subclasses.
  */
  MT_PersistentClass ( void );

/* ------------------------------------------------------------------------ */
/*                           Reading functions                              */
/* ------------------------------------------------------------------------ */

  public:

  /*
  Search for a specific keyword in the input file, skipping all
  characters from the current position to the keyword.
  If flag load_descr==1, then the skipped characters are loaded into the
  object description my_descr. Return 1 on success, 0 on failure.
  */
  int MT_SearchKeyword(FILE * fd, char * keyword, int load_descr = 0);

  /*
  Search and read the keyword of MT files.
  Return 1 on success, 0 on failure.
  */
  int MT_ReadFileKw(FILE * fd);

  /*
  Read an MT index, read an integer counter.
  */
  int MT_ReadIndex(FILE * fd, MT_INDEX * i, int file_encoding);
  int MT_ReadCount(FILE * fd, int * i, int file_encoding);

  /*
  Read the class stored in the file and its parameters.
  Here, it is assumed that the class has no parameters.
  In the subclasses, the function may be redefined to read class
  parameters as well and use them to initialize this object.
  Return 1 on success, 0 on failure.
  */
  virtual int MT_ReadClassAndParams(FILE * fd);

  /*
  Read the parameters (typically, sizes) of the object stored in the 
  file. Here, it is assumed that no parameters are present.
  In the subclasses, the function may be redefined to read object
  parameters as well and use them to allocate the internal structures
  of this object.
  Return 1 on success, 0 on failure.
  */
  virtual int MT_ReadObjParams(FILE * fd);

  /*
  Read the textual description of the object stored in the file and
  store it into my_descr.  Return 1 on success, 0 on failure.
  */
  int MT_ReadDescription(FILE * fd);

  /*
  Read the encoding of the file, and return it. The file encoding will
  be used for reading the body.  Return 1 on success, 0 on failure.
  */
  int MT_ReadEncoding(FILE * fd, int * file_encoding);

  /*
  Read the content of the file and fill the internal data structures
  of this object with the data contained in it.
  Return 1 on success, 0 on failure.
  This function will be implemented in the subclasses.
  */
  virtual int MT_ReadBody(FILE * fd, int file_encoding) = 0;

  /*
  Read the whole file. Return 1 on success, 0 on failure.
  If successful, use the data contained in the file to fill this object.
  */
  int MT_Read(FILE * fd);
  int MT_Read(char * fname);

/* ------------------------------------------------------------------------ */
/*                            Writing functions                             */
/* ------------------------------------------------------------------------ */

  /*
  Write a keyword. 
  */
  void MT_WriteKeyword(FILE * fd, char * keyword);

  /*
  Write an MT index, write an integer counter.
  */
  void MT_WriteIndex(FILE * fd, MT_INDEX i, int file_encoding);
  void MT_WriteCount(FILE * fd, int i, int file_encoding);

  /*
  Write the keyword of MT files.
  */
  void MT_WriteFileKw(FILE * fd);
  
  /*
  Write the class and its parameters. Here, it is assumed that the
  class has no parameters.  In the subclasses, this function can be
  redefined to write class parameters as well.
  */
  virtual void MT_WriteClassAndParams(FILE * fd);

  /*
  Write the parameters (sizes) of this object. Here, it is assumed
  that there are no parameters. In the subclasses, this function can be
  redefined to write object parameters as well.
  */
  virtual void MT_WriteObjParams(FILE * fd);
  
  /*
  Write the textual description of this object contained in string my_descr.
  */
  void MT_WriteDescription(FILE * fd);

  /*
  Write the encoding of the file. The same encoding 
  will be used for writing the body.
  */
  void MT_WriteEncoding(FILE * fd, int file_encoding);

  /*
  Write the content of the file taking the information form the 
  internal data structures of this object.
  */
  virtual void MT_WriteBody(FILE * fd, int file_encoding) = 0;

  /*
  Write the whole file, taking the object description from string my_descr.
  */
  virtual void MT_Write(FILE * fd, int file_encoding = MT_ASCII_ENCODING);
  virtual int MT_Write(char * fname, int file_encoding = MT_ASCII_ENCODING);
  

};

/* ------------------------------------------------------------------------ */

#endif  /* MT_PERSISTENT_INCLUDED */
