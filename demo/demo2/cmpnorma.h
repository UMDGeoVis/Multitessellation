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

/* ----------------------------- cmpnorma.h ------------------------------ */

#ifndef COMPUTE_NORMALS_INCLUDED
#define COMPUTE_NORMALS_INCLUDED

#include "mt_extra.h"
#include "vrtnorma.h"
#include "geo.h"

/* ----------------------------------------------------------------------- */
/*                        COMPUTE VERTEX NORMALS                           */
/* ----------------------------------------------------------------------- */

/*
This file defines a class to compute and store vertex normals for 
a triangulation extracted from a surface MT.
*/

/* ----------------------------------------------------------------------- */
/*                     CLASS FOR COMUTING NORMALS                          */
/* ----------------------------------------------------------------------- */

typedef class ExtraVertexNormalClass * ExtraVertexNormal;

class ExtraVertexNormalClass : public WithVertexNormalTableClass
{
  protected:

  /*
  The Multi-Tesselation from which tesselations are extracted.
  */
  MT_MultiTesselation mt;
  
  /*
  Array containing, for each MT vertex, the counter of
  its incident triangles in the currently extracted tesselation.
  */
  short int * VN_count;

  public:
  
  /*
  Compute vertex normals for the tesselation currently extracted in
  extractor ex. The extractor must have been created on the same MT mt.
  */
  void ComputeAllVertexNormals(MT_Extractor ex);

  /*
  Compute vertex normals for the active part of the tesselation currently
  extracted in extractor ex. The extractor must have been created on the 
  same MT mt.
  */
  void ComputeVertexNormals(MT_Extractor ex);
 
  /*
  Constructor and destructor.
  */
  ExtraVertexNormalClass(MT_MultiTesselation mt);
  ~ExtraVertexNormalClass(void);
};

/* ----------------------------------------------------------------------- */

#endif /* COMPUTE_NORMALS_INCLUDED */
