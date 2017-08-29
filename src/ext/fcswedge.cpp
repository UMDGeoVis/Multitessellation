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

/* ------------------------------ fcswedge.c ------------------------------ */

#include "geo.h"
#include "fcswedge.h"

/* ------------------------------------------------------------------------ */
/*                      FOCUS SET CONSISTING OF A WEDGE                     */
/* ------------------------------------------------------------------------ */

void WedgeClass :: SetAngle(int i, float a)
{  
   if ( (a<0) || (a>(0.5*PI)) )
        MT_Error((char *)"Invalid opening angle",(char *)"WedgeClass::SetAngle");
   else open_angle[i] = a;
}

void WedgeClass :: TheVertex(float * coord)
{
  int i;
  for (i=0; i<w_dim; i++)  {  coord[i] = v_coord[i];  }
}

void WedgeClass :: SetVertex(float * coord)
{
  int i;
  for (i=0; i<w_dim; i++)  {  v_coord[i] = coord[i];  }
}

void WedgeClass :: TranslateVertex(float * incr)
{
  int i;
  for (i=0; i<w_dim; i++)  {  v_coord[i] += incr[i];  }
}

void WedgeClass :: TheAxisPoint(float * coord, float dist)
{
  int i;
  for (i=0; i<w_dim; i++)  
  {  coord[i] = v_coord[i] + dist * w_axis[i];  }
}

void WedgeClass :: SetAxisDir(float * dir)
{
  /* normalize the distance vector */
  float dist = 0.0;
  int i;
  for (i=0; i<w_dim; i++)  
  {  dist += (dir[i]) * (dir[i]);  }
  if (dist>ZERO)
  {  dist = (float) sqrt(dist);
     for (i=0; i<w_dim; i++)
     {  w_axis[i] = (dir[i]) / dist;  }
  }
  else MT_Error((char *)"Invalid axis",(char *)"WedgeClass::SetAxis");
}

void WedgeClass :: TheAxisDir(float * dir)
{
  int i;
  for (i=0; i<w_dim; i++)
  {  dir[i] = w_axis[i];  }
}

void WedgeClass :: SetAxisPoint(float * coord)
{
  /* distance between the vertex and the given point, used to
     normalize the axis direction */
  float dist = 0.0;
  int i;
  for (i=0; i<w_dim; i++)  
  {  dist += (v_coord[i] - coord[i]) * (v_coord[i] - coord[i]);  }
  if (dist>ZERO)
  {  dist = (float) sqrt(dist);
     for (i=0; i<w_dim; i++)
     {  w_axis[i] = (coord[i]-v_coord[i]) / dist;  }
  }
  else MT_Error((char *)"Invalid axis point",(char *)"WedgeClass::SetAxisPoint");
}

void WedgeClass :: TheAxisPoint(float * x, float * y, float dist)
{
  (*x) = v_coord[0] + dist * w_axis[0];
  (*y) = v_coord[1] + dist * w_axis[1];
}

void WedgeClass :: SetAxisDir(float dx, float dy)
{
  normalize2D(&dx, &dy);
  {  w_axis[0] = dx;  w_axis[1] = dy;  }
}

void WedgeClass :: TheAxisDir(float * dx, float * dy)
{
  (*dx) = w_axis[0];  (*dy) = w_axis[1];
}

void WedgeClass :: SetAxisPoint(float x, float y)
{  SetAxisDir(x-v_coord[0], y-v_coord[1]);  }

void WedgeClass ::  TheLeftPoint(float * x, float * y, float dist)
{
  (*x) = v_coord[0] + 
              dist * ( w_axis[0] * cos(open_angle[0]) -
                       w_axis[1] * sin(open_angle[0]) );
  (*y) = v_coord[1] + 
              dist * ( w_axis[0] * sin(open_angle[0]) +
                       w_axis[1] * cos(open_angle[0]) );
}



void WedgeClass ::  TheRightPoint(float * x, float * y, float dist)
{
  (*x) = v_coord[0] + 
              dist * ( w_axis[0] * cos(-open_angle[0]) -
                       w_axis[1] * sin(-open_angle[0]) );
  (*y) = v_coord[1] + 
              dist * ( w_axis[0] * sin(-open_angle[0]) +
                       w_axis[1] * cos(-open_angle[0]) );
}

WedgeClass :: WedgeClass(int d)
{
  int i, j;
  w_dim = d;
  for (i=0; i<w_dim; i++)  
  {  v_coord[i] = 0.0;
     w_axis[i] = (float) ( (i<(w_dim-1)) ? 0.0 : 1.0 );
     open_angle[i] = 0.0;
     for (j=0; j<w_dim; j++)  {  open_dir[i][j] = 0.0;  }
  }
}

/* ------------------------------------------------------------------------ */
/*                  FOCUS CONSISTING OF A WEDGE IN 2D                       */
/* ------------------------------------------------------------------------ */

Wedge2FocusOnTrianglesClass ::
Wedge2FocusOnTrianglesClass(float xv, float yv, float xr, float yr, float a)
:    WedgeClass(2)
{  
  SetVertex(xv,yv);
  SetAxisPoint(xr,yr);
  SetAngle(0,a);
}

int Wedge2FocusOnTrianglesClass ::
    MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag)
{
  MT_INDEX * v;       /* triangle vertices */
  float p_left[2];    /* a point lying on the left side of the wedge */
  float p_right[2];   /* a point lying on the right side of the wedge */
  short int which_turn[3];
  int i;
  
  TheLeftPoint(&p_left[0],&p_left[1]);
  TheRightPoint(&p_right[0],&p_right[1]);

  m->MT_TileVertices(t, &v);
  /* check if any triangle vertex is inside wedge */
  for (i=0;i<3;i++)
  {
    which_turn[i] = 0x0;
    if (PointTurn2D(v_coord[0], v_coord[1], p_right[0], p_right[1],
                    m->MT_VertexX(v[i]),m->MT_VertexY(v[i])) == LEFT_TURN)
    {  which_turn[i] |= 0x1;  }
    if (PointTurn2D(p_left[0], p_left[1], v_coord[0],v_coord[1],
                    m->MT_VertexX(v[i]),m->MT_VertexY(v[i])) == LEFT_TURN)
    {  which_turn[i] |= 0x2;  }
    if ( which_turn[i] == 0x3 ) /* both left turns, vertex inside wedge */
    {  return 1;  }
  }
  /* check if any triangle edge crosses wedge */
  for (i=0;i<3;i++)
  {
    if ( (which_turn[i] | which_turn[(i+1)%3]) == 0x3 ) 
    /* one vertex turns right-left and the other one left-right */
    switch (which_turn[i])
    {
       case 0x1: 
         if (PointTurn2D(m->MT_VertexX(v[i]),m->MT_VertexY(v[i]),
                         m->MT_VertexX(v[(i+1)%3]),m->MT_VertexY(v[(i+1)%3]),
                         v_coord[0], v_coord[1]) == RIGHT_TURN)
         {  return 1;  }
         break;
       case 0x2:
         if (PointTurn2D(m->MT_VertexX(v[i]),m->MT_VertexY(v[i]),
                         m->MT_VertexX(v[(i+1)%3]),m->MT_VertexY(v[(i+1)%3]),
                         v_coord[0], v_coord[1]) == LEFT_TURN)
         {  return 1;  }
         break;
    }
  }
  return 0;
}

int Wedge2FocusOnTrianglesClass :: MT_IsGood(MT_MultiTesselation m)
{
  if (m->MT_TileDim() != 2)
  {
     MT_Warning((char *)"Need two-dimensional tiles",
                (char *)"Wedge2FocusOnTrianglesClass::MT_IsGood");
     return 0;
  }
  return 1;
}

/* ------------------------------------------------------------------------ */
