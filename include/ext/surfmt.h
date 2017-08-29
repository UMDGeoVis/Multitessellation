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

/* ------------------------------ surfmt.h -------------------------------- */

#ifndef SURFACE_MT_INCLUDED
#define SURFACE_MT_INCLUDED

#include "tilerror.h"
#include "vrtnorma.h"

/* ------------------------------------------------------------------------ */
/*                     A SAMPLE MT WITH ATTRIBUTES                          */
/* ------------------------------------------------------------------------ */

/*
An MT with error values associated with its tiles and normals associated 
with its vertices. The MT is two-dimensional (tiles are triangles) and 
it is embedded in 3D space.
*/

typedef class SurfaceWithErrorClass * SurfaceWithError;

class SurfaceWithErrorClass : public MT_MultiTesselationClass,
                              public WithTileErrorTableClass,
                              public WithVertexNormalTableClass
{ 
  public:

  /*
  Constructor.
  */
  inline SurfaceWithErrorClass(void)
  :      MT_MultiTesselationClass(3,2),
         WithTileErrorTableClass(),
         WithVertexNormalTableClass()
  { }

};

/* ------------------------------------------------------------------------ */

/*
The interface for building an MT with error values associated with its tiles
and normals associated with its vertices.
*/

typedef class SurfaceWithErrorBuildingInterfaceClass
         * SurfaceWithErrorBuildingInterface;

class SurfaceWithErrorBuildingInterfaceClass 
      : public MT_BuildingInterfaceClass,
        public TileErrorBuildingInterfaceClass,
        public VertexNormalBuildingInterfaceClass
{
  public:
  
  /*
  Constructor.
  */
  inline SurfaceWithErrorBuildingInterfaceClass(void)
  :      MT_BuildingInterfaceClass(),
         TileErrorBuildingInterfaceClass(),
         VertexNormalBuildingInterfaceClass()
  { }
  
  /*
  Function redefined from superclass MT_BuildingInterfaceClass.
  */
  inline int MT_StartHistory(int history_type)
  {  return 
      ( MT_BuildingInterfaceClass::MT_StartHistory(3,2,history_type) );
  }

};

/* ------------------------------------------------------------------------ */

#endif /* SURFACE_MT_INCLUDED */
