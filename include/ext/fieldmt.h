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

/* ------------------------------ fieldmt.h ------------------------------- */

#ifndef FIELD_MT_INCLUDED
#define FIELD_MT_INCLUDED

#include "tilerror.h"
#include "vrtfield.h"

/* ------------------------------------------------------------------------ */
/*                     A SAMPLE MT WITH ATTRIBUTES                          */
/* ------------------------------------------------------------------------ */

/*
An MT with error values associated with its tiles and field values
associated with its vertices.
*/

typedef class FieldWithErrorClass * FieldWithError;

class FieldWithErrorClass : public MT_MultiTesselationClass,
                            public WithTileErrorTableClass,
                            public WithFieldClass
{
  public:
  
  /*
  Return the field value for a vertex v.
  We assume that the MT is given in the space containing the graph of 
  the field, thus the field value is the last coordinate of each vertex.
  */
  inline float VertexField(MT_INDEX v) 
  {  return MT_VertexCoord(v, MT_VertexDim()-1);  }

  /*
  Return the maximum and minimum field over all vertices.
  */
  inline float MaxVertexField(void)
  {  return MT_MaxCoord(MT_VertexDim()-1);  }
  inline float MinVertexField(void)
  {  return MT_MinCoord(MT_VertexDim()-1);  }

  /*
  Constructor.
  */
  inline FieldWithErrorClass(int vert_dim, int tile_dim)
  :      MT_MultiTesselationClass(vert_dim, tile_dim),
         WithTileErrorTableClass()
  { }
};

/* ------------------------------------------------------------------------ */

/*
The interface for building an MT with error values associated with its tiles
and field values associated with its vertices.
*/

typedef class 
        FieldWithErrorBuildingInterfaceClass * FieldWithErrorBuildingInterface;

class FieldWithErrorBuildingInterfaceClass : 
                 public virtual MT_BuildingInterfaceClass,
                 public virtual TileErrorBuildingInterfaceClass
{
  public:
  
  /*
  Constructor.
  */
  inline FieldWithErrorBuildingInterfaceClass(void)
  :      MT_BuildingInterfaceClass(),
         TileErrorBuildingInterfaceClass()
  { }
  
};

/* ------------------------------------------------------------------------ */

#endif /* FIELD_MT_INCLUDED */
