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

/* ---------------------------- mt_attr.c --------------------------------- */

#include "mt_attr.h"

/* ------------------------------------------------------------------------ */
/*                    A CLASS FOR STORING MT ATTRIBUTES                     */
/* ------------------------------------------------------------------------ */

int MT_AttrTableClass :: MT_ReadBody(FILE * fd, int file_encoding)
  {  
     unsigned int i;
     for (i=1; i<my_num+1; i++)
     {
        if (!MT_ReadAttr(fd, i, file_encoding)) return 0;
     }
     return 1;
  }

void MT_AttrTableClass :: MT_WriteBody(FILE * fd, int file_encoding)
{  
   unsigned int i;
   for (i=1; i<my_num+1; i++)
   {  MT_WriteAttr(fd, i, file_encoding);  }
}

/* ------------------------------------------------------------------------ */
