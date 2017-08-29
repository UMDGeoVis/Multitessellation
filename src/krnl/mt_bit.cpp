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

/* ------------------------------ mt_bit.c ------------------------------- */

#include "mt_bit.h"

/* ------------------------------------------------------------------------ */
/*                         THE BIT VECTOR CLASS                             */
/* ------------------------------------------------------------------------ */

int MT_BitClass :: MT_SetBitNum(MT_INDEX n)
{
  int i  = n / sizeof(MT_BLOCK) + ( (n % sizeof(MT_BLOCK)) ? 1 : 0 );
  bit_Arr = (MT_BLOCK *) calloc (i+1, sizeof(MT_BLOCK));
  /* calloc cleans all bits to zero */
  if (!bit_Arr)
  {  MT_Error((char *)"Failed allocation", (char *)"MT_BitClass::MT_SetBitNum");
     return 0;
  }
  /* else */  
  blk_Num = i;
  return 1;
}  

/* ------------------------------------------------------------------------ */

void MT_BitClass :: MT_SetBit(MT_INDEX i, int b)
{
  int block = i / sizeof(MT_BLOCK);
  int offset = i % sizeof(MT_BLOCK);
  int filter = ( 1 << offset );
  if (b) /* we are raising a bit */
     bit_Arr[block] |= filter;
  else   /* we are lowering a bit */
     bit_Arr[block] &= (~filter);
}

int MT_BitClass :: MT_TheBit(MT_INDEX i)
{
  int block = i / sizeof(int);
  int offset = i % sizeof(int);
  int filter = ( 1 << offset );
  return ( bit_Arr[block] & filter );
}

/* ------------------------------------------------------------------------ */

MT_BitClass :: MT_BitClass(void) 
{  blk_Num = 0; 
   bit_Arr = NULL;
}

MT_BitClass ::  ~MT_BitClass(void) 
{  if (blk_Num) {  free(bit_Arr);  }  }

/* ------------------------------------------------------------------------ */
