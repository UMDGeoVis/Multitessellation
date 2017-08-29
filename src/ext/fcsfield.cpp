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

/* ------------------------------ fcsfield.c ------------------------------ */

#include "geo.h"
#include "fcsfield.h"
#include "mt_extra.h"

/* ------------------------------------------------------------------------ */
/*                  GENERIC CLASS FOR A SET OF FIELD VALUES                 */
/* ------------------------------------------------------------------------ */

FieldValFocusClass ::
FieldValFocusClass(WithFieldClass * m1, WithTileErrorClass * m2,
                   float val) : FieldValClass(val)
{  my_mt1 = m1;  my_mt2 = m2;  }

FieldValFocusClass ::
FieldValFocusClass(WithFieldClass * m1, WithTileErrorClass * m2,
                   int num, float * val) : FieldValClass(num, val)
{  my_mt1 = m1;  my_mt2 = m2;  }

int FieldValClass :: SetFieldValNum(int num)
{
  if (val_Num != num)
  {  
     val_Arr = (float *) realloc(val_Arr, num*sizeof(float));
     if (!val_Arr)
     {   MT_Error((char *)"Failed allocation",(char *)"FieldValClass::SetFieldVal");     
         return 0;
     }
  }
  val_Num = num;
  return 1;
}

void FieldValClass :: SetFieldVal(int num, float * val)
{
  int i;
  if (SetFieldValNum(num))
  {  
     for (i=0;i<num;i++)  {  val_Arr[i] = val[i];  }
  }
}

void FieldValClass :: SetFieldVal(int num, float val0, float step)
{
  int i;
  if (SetFieldValNum(num))
  {
     for (i=0;i<num;i++)  {  val_Arr[i] = val0 + i*step;  }
  }
}  

FieldValClass :: FieldValClass(int num, float * val)
{  
  val_Num = 0;
  val_Arr = NULL;
  SetFieldVal(num, val);
}

FieldValClass :: FieldValClass(float val)
{  
  val_Num = 0;
  val_Arr = NULL;
  SetFieldVal(1, &val);
}

FieldValClass :: FieldValClass(int num, float val0, float step)
{
  val_Num = 0;
  val_Arr = NULL;
  SetFieldVal(num,val0,step);
}  

FieldValClass :: ~FieldValClass(void)
{
  if (val_Arr) free(val_Arr);
  val_Arr = NULL;
  val_Num = 0;
}

/* ------------------------------------------------------------------------ */
/*                 FOCUS CONSITING OF A SET OF FIELD VALUES                 */
/* ------------------------------------------------------------------------ */

int FieldValFocusClass ::
    MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag)
{
  MT_INDEX * v;
  int i, j;
  /* flags if the field in at least one tile vertex is higher/lower than
     the reference value */
  int higher, lower;
  /* if loose evaluation, the reference value becomes an interval and
     below are its extreme values */
  float val_low, val_hi; 
 
  /* for each field value, check if the tile contains it */
  for (j=0; j<val_Num; j++)
  {
    higher = 0; lower = 0;
    if (flag==MT_LOOSE) 
    { 
      val_low = val_Arr[j] + my_mt2->TileError(t);
      val_hi = val_Arr[j] - my_mt2->TileError(t);
    }
    else  val_low = val_hi = val_Arr[j];
    m->MT_TileVertices(t, &v);
    for (i=0; i<m->MT_NumTileVertices(); i++)
    {
      higher = ( higher || ( (my_mt1->VertexField(v[i])+ZERO ) >= val_hi) );
      lower = ( lower || ( (my_mt1->VertexField(v[i])-ZERO ) <= val_low) );
      if (higher && lower) return 1;
    }
  }
  return 0;
}

FieldValFocusClass ::
FieldValFocusClass(WithFieldClass * m1, WithTileErrorClass * m2,
                   int num, float val0, float step)
: FieldValClass(num, val0, step)
{  my_mt1 = m1;  my_mt2 = m2;  }

/* ------------------------------------------------------------------------ */

int HeightSegment (MT_MultiTesselation m, WithFieldClass * mf,
                   MT_INDEX t, float val,
                   float *x1, float *y1, float *x2, float *y2)
{
  MT_INDEX * v;
  float x[3], y[3];
  int i, j;
  float p;
  
  m->MT_TileVertices(t, &v);
  j = 0;
  /* check is some vertex has the given field value */
  for (i=0;i<3;i++)
  {  
    if ( mf->VertexField(v[i]) == val )
    {  
       x[j] = m->MT_VertexX(v[i]);
       y[j] = m->MT_VertexY(v[i]);  
       j++;
    }
  }      
  /* find the edges that intersect field value val */
  for (i=0;i<3;i++)
  {
    if ( ( (mf->VertexField(v[i])<val) && (mf->VertexField(v[(i+1)%3])>val) ) 
       ||
       ( (mf->VertexField(v[i])>val) && (mf->VertexField(v[(i+1)%3])<val) ) )
    {
      /* edge intersects value val, find in which point using the 
         parametric equation of the edge */
      p = ( val - mf->VertexField(v[i]) ) /
          ( mf->VertexField(v[(i+1)%3]) - mf->VertexField(v[i]) );
      x[j] = m->MT_VertexX(v[i]) * (1-p) + m->MT_VertexX(v[(i+1)%3]) * p;
      y[j] = m->MT_VertexY(v[i]) * (1-p) + m->MT_VertexY(v[(i+1)%3]) * p;
      j++;  
    }
  }
  switch (j)
  { 
    case 0: 
      /* no intersections */
      return 0;
    case 1: 
      /* interesection is just one point */
      return 0;
    case 2:
      /* two intersection points, return segment joining them */
      (*x1) = x[0]; (*y1) = y[0];
      (*x2) = x[1]; (*y2) = y[1];
      return 1;
    default:
      /* this can only happen if the triangle is flat and lies at the
         given height */
      MT_Warning((char *)"Three intersection points",(char *)"HeightSegment");
      return 0;
  }
}

int HeightSection (MT_MultiTesselation m, WithFieldClass * mf,
                   MT_INDEX t, float val, 
                   float *pt1, float *pt2, float * pt3, float *pt4)
{
  MT_INDEX * v;
  float x[8], y[8], z[8];
  int i, j, k;
  float p;
  
  m->MT_TileVertices(t, &v);
  j = 0;
  /* check is some vertex has the given field value */
  for (i=0;i<4;i++)
  {  
    if ( mf->VertexField(v[i]) == val )
    {  
       x[j] = m->MT_VertexX(v[i]);
       y[j] = m->MT_VertexY(v[i]);  
       z[j] = m->MT_VertexZ(v[i]); 
printf("Vert %d-esimo di %d ha valore di campo dato\n",i,t);
       j++;
    }
  }      
  /* find the edges that intersect field value val */
  for (i=0;i<4;i++)
  for (k=i+1;k<4;k++)
  {
    if ( ( (mf->VertexField(v[i])<val) && (mf->VertexField(v[k])>val) ) 
       ||
       ( (mf->VertexField(v[i])>val) && (mf->VertexField(v[k])<val) ) )
    {
      /* edge intersects value val, find in which point using the 
         parametric equation of the edge */
      p = ( val - mf->VertexField(v[i]) ) /
          ( mf->VertexField(v[k]) - mf->VertexField(v[i]) );
      x[j] = m->MT_VertexX(v[i]) * (1-p) + m->MT_VertexX(v[k]) * p;
      y[j] = m->MT_VertexY(v[i]) * (1-p) + m->MT_VertexY(v[k]) * p;
      z[j] = m->MT_VertexZ(v[i]) * (1-p) + m->MT_VertexZ(v[k]) * p;
printf("Lato %d-%d di %d ha valore di campo dato\n",i,k,t);
      j++;  
    }
  }
printf("Numero intersez per %d = %d\n",t,j);
  switch (j)
  { 
    case 0: 
      /* no intersections */
      return 0;
    case 1: 
      /* interesection is just one point */
      return 0;
    case 2:
      /* two intersection points */
      return 0;
    case 3:
      /* three intersection points */
      pt1[0] = x[0]; pt1[1] = y[0]; pt1[2] = z[0]; 
      pt2[0] = x[1]; pt2[1] = y[1]; pt2[2] = z[1];
      pt3[0] = x[2]; pt3[1] = y[2]; pt3[2] = z[2];
      return 1;
    case 4:
      /* four intersection points */
      pt1[0] = x[0]; pt1[1] = y[0]; pt1[2] = z[0];
      pt2[0] = x[1]; pt2[1] = y[1]; pt2[2] = z[1];
      pt3[0] = x[2]; pt3[1] = y[2]; pt3[2] = z[2]; 
      pt4[0] = x[3]; pt4[1] = y[3]; pt4[2] = z[3];
      return 2;
    default:
      /* this can only happen if the field is constant in the tetrahedron 
         and it is equal to the given value */
      MT_Warning((char *)"Four intersection points",(char *)"HeightSegment");
      return 0;
  }
}

void HeightSegments (MT_Extractor e, float val, 
                     WithFieldClass * mf, MT_TileSet s)
{
  MT_INDEX * active_tri;
  MT_INDEX num_active;
  MT_INDEX i;
  float p1[2], p2[2];
  MT_INDEX v[2];

  num_active = e->MT_ExtractedTiles(&active_tri);
  /* set the size of the segment set based on a pessimistic estimate */
  s->MT_SetVertexNum(2*num_active);
  s->MT_SetTileNum(num_active);
  /* compute segments and put them into the set */
  for (i=0; i<num_active; i++)
  {
    HeightSegment(e->MT_TheMT(), mf, active_tri[i], 
                  val, &p1[0], &p1[1], &p2[0], &p2[1]);
    v[0] = 2*i; v[1] = 2*i+1;
    s->MT_AddVertex(v[0], p1);
    s->MT_AddVertex(v[1], p2);
    s->MT_AddTile(i, v);
  }
}
