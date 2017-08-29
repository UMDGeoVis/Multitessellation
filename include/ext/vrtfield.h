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

/* ------------------------------ mt_with.h ------------------------------- */

#ifndef VERTEX_FIELD_INCLUDED
#define VERTEX_FIELD_INCLUDED

#include "mt_multi.h"
#include "mt_attr.h"

/* ------------------------------------------------------------------------ */
/*                    Adding field values to an MT                          */
/* ------------------------------------------------------------------------ */

/*
Bare interface containing the additional features of an MT with field
values associated with its vertices.
*/

typedef class WithFieldClass * WithField;

class WithFieldClass
{
  public:
  
  /*
  Return the field value at a vertex v.
  */
  virtual float VertexField(MT_INDEX v) = 0;
  
  /*
  Return the maximum and minimum field value over all vertices.
  */
  virtual float MaxVertexField(void) = 0;
  virtual float MinVertexField(void) = 0;  
};

/* ------------------------------------------------------------------------ */

#endif /* VERTEX_FIELD_INCLUDED */
