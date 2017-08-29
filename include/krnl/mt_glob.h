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

/* ---------------------------- mt_glob.h --------------------------------- */

/* ------------------------------------------------------------------------ */
/*                  GENERAL CONVENTIONS FOR THE MT PACKAGE                  */
/* ------------------------------------------------------------------------ */

#ifndef MT_GLOBAL_INCLUDED
#define MT_GLOBAL_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------------ */
/*                                MACROS                                    */
/* ------------------------------------------------------------------------ */

/*
Maximum dimension allowed for a Multi-Tesselation and for the space
embedding it.
*/
#define MT_MAX_DIM 10

/* 
Type of the indexes describing the entities (vertices, cells, nodes, arcs)
inside an MT.
*/
#define MT_INDEX unsigned int

/*
Null index. When the result of an operation that should retrn an MT entity
(vertex, cell, node, arc) is undefined, the null index is returned.
*/
#define MT_NULL_INDEX 0

/*
Maximum length of a string (used for filenames and keywords).
*/
#define MT_MAX_STRLEN 100

/*
Maximum length of an object description.
*/
#define MT_DESCR_LEN 255

/* ------------------------------------------------------------------------ */
/*                             ERROR UTILITIES                              */
/* ------------------------------------------------------------------------ */

/*
char *msg = error / warning message, 
char *fun = name of the function in which the problem has occurred
char *cls = class in which the problem has occurred
*/

extern void MT_Warning(char * msg, char * fun);
extern void MT_Error(char * msg, char * fun);    
extern void MT_FatalError(char * msg, char * fun);

extern void MT_WarningC(char * msg, char * cls, char * fun);
extern void MT_ErrorC(char * msg, char * cls, char * fun);
extern void MT_FatalErrorC(char * msg, char * cls, char * fun);

/* ------------------------------------------------------------------------ */
/*                           DEBUGGING UTILITIES                            */
/* ------------------------------------------------------------------------ */

/*
This macro must be over-written in the single files by defining
MT_DEBUG_THIS_FILE as 1 before including any MT header.
*/
#ifndef MT_DEBUG_THIS_FILE
#define MT_DEBUG_THIS_FILE 0
#endif

/*
Memory check.
*/
#ifdef MT_MEMDEBUG
extern void MT_CheckMemory(char * msg);
#else
#define MT_CheckMemory(msg) 
#endif /* MT_MEMDEBUG */

/*
Print message.
*/
#ifdef MT_DEBUG
extern void MT_Message(char * msg, char * fun);
extern void MT_MessageC(char * msg, char * cls, char * fun);
#else   
#define MT_Message(msg,fun)
#define MT_MessageC(msg,cls,fun)
#endif /* MT_DEBUG */

/* ------------------------------------------------------------------------ */

#endif /* MT_GLOBAL_INCLUDED */
