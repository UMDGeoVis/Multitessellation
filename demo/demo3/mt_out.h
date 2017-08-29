/* ------------------------------ mt_out.h ------------------------------- */

#ifndef MT_OUT_INCLUDED
#define MT_OUT_INCLUDED

#include "mt_extra.h"
//#include "stdafx.h"


/* ----------------------------------------------------------------------- */
/*            FUNCTIONS FOR WRITING THE EXTRACTED TESSELATION              */
/* ----------------------------------------------------------------------- */

/*
This file provides functions that, given an extractor, active on an MT, 
write its currently extracted tesselation.

The extracted tesselation can be written in one of the following two 
formats (let d be the dimension of the embedding space and k be the 
dimension of the tiles):

RAW
For each tile:
    list of the d coordinates of its k+1 vertices

INDEXED
For each vertex:
    list of its  d coordinates 
For each tile:
    list of the indices of its k+1 vertices (starting from 0)

It is possible to write other information for vertices and tiles 
(see flags below).
*/

/* ----------------------------------------------------------------------- */
/*       Flags controlling the additional information to be written        */
/* ----------------------------------------------------------------------- */

#define MT_NO_FLAGS    0x00
#define MT_ADJ_FLAG    0x01  /* write tile-to-tile adjacency information */

/* ------------------------------------------------------------------------ */
/*                         Avaliable functions                              */
/* ------------------------------------------------------------------------ */

/*
Write an extracted tesselation on a file in the two formats.
Return 1 if OK, 0 if some error has occurred.
The fisrt two functions write just the active tiles. The last two functions
write all extracted tiles.
*/
int MT_WriteRaw(FILE * fd, MT_Extractor e, int flags);
int MT_WriteIndexed(FILE * fd, MT_Extractor e, int flags);
int MT_WriteAllRaw(FILE * fd, MT_Extractor e, int flags);
int MT_WriteAllIndexed(FILE * fd, MT_Extractor e, int flags);

/* ------------------------------------------------------------------------ */

#endif /* MT_OUT_INCLUDED */
