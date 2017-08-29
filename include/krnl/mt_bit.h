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

/* ------------------------------ mt_bit.h -------------------------------- */

#ifndef MT_BIT_INCLUDED
#define MT_BIT_INCLUDED

#include "mt_glob.h"

/* ------------------------------------------------------------------------ */
/*                           BIT VECTOR CLASS                               */
/* ------------------------------------------------------------------------ */

/*
This file defines class MT_BitClass, a bit vector indexed on MT_INDEXes
with functions to set and get bit entries.
*/

/* ------------------------------------------------------------------------ */
/*                             MACROS                                       */
/* ------------------------------------------------------------------------ */

/*
Type used for one block in the bit array.
*/
#define MT_BLOCK unsigned int

/*
Format string for writing a block.
*/
#define MT_BLOCK_STRING "%u"

/* ------------------------------------------------------------------------ */
/*                         THE BIT VECTOR CLASS                             */
/* ------------------------------------------------------------------------ */

typedef class MT_BitClass * MT_Bit;

class MT_BitClass 
{

  protected:

  /* 
  Bit array. Indeed, it is an array of blocks but the class provides
  functions to access it bit-wise.
  */
  MT_BLOCK * bit_Arr; 

  /*
  Number of elements (blocks) in the bit array.
  */
  MT_INDEX blk_Num;

  public:

  /* 
  Allocate the attribute array to n positions.
  Return 1 on success, 0 otherwise.
  */
  int MT_SetBitNum(MT_INDEX n);

  /*
  Set and get the i-th entry of the bit vector.
  */
  void MT_SetBit(MT_INDEX i, int b);
  int MT_TheBit(MT_INDEX i);

  /*
  Creator and destructor.
  */
  MT_BitClass(void);
  ~MT_BitClass(void);

};

/* ------------------------------------------------------------------------ */

#endif /* MT_BIT_INCLUDED */
