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

/* -------------------------------- geo.c --------------------------------- */

#include "geo.h"

#include <stdio.h> //***DA TOGLIERE*** 

/* ------------------------------------------------------------------------ */

/*  Funzioni geometriche di utilita' sui punti, segmenti,  triangoli, ecc   */

/* ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */

/* auxiliary arithmetic functions */

/* ------------------------------------------------------------------------ */

/* 
Calculate sign of determinant  |a b|
                               |c d|.
*/
int DetSign2D (double a, double b, double c, double d)
{
  float t1, t2;
  t1 = (a*d); t2 = (b*c);
  if ( t1 > (t2+ZERO) ) return 1;
  if ( t2 > (t1+ZERO) ) return -1;
  return 0;  
}

/* 
Calculate sign of determinant |a1 a2 a3|
                              |b1 b2 b3|
                              |c1 c2 c3|
*/
int DetSign3D(double a1, double a2, double a3, 
              double b1, double b2, double b3,
              double c1, double c2, double c3)
{  
  float d = Det3D(a1,a2,a3,b1,b2,b3,c1,c2,c3);
  if (fabs(d)<=ZERO) return 0;
  return ( (d>0.0) ? 1 : -1 );
}

void normalize(float *x, float *y, float *z)
{
  float l = sqrt ( ((*x) * (*x)) + ((*y) * (*y)) + ((*z) * (*z)) );
  if ( ! (l<ZERO) )
  {
    (*x) /= l;
    (*y) /= l;
    (*z) /= l;
  }
}

void normalize2D(float *x, float *y)
{
  float l = sqrt ( ((*x) * (*x)) + ((*y) * (*y)) );
  if ( ! (l<ZERO) )
  {
    (*x) /= l;
    (*y) /= l;
  }
}

/* ------------------------------------------------------------------------ */

/* distances */

/* ------------------------------------------------------------------------ */

/*
Distance of point (xp,yp) from triangle whose vertices are contained
in arrays x[3] and y[3].
*/
float SquaredPointTriangleDist2D(float xp, float yp, float * x, float * y)
{
  int i;              /* vertex counter */
  float d;            /* the computed distance */
  int right_count;    /* count right turns */
  int right_ind[2];   /* for indices of edges that turn right */
  float a,b,c;        /* coeff. of the straight-line supporing the edge */

  right_count = 0;
  for (i=0;i<3;i++)
  {
    if (PointTurn2D(xp,yp, x[i],y[i], x[(i+1)%3], y[(i+1)%3]) == RIGHT_TURN)
    {
      right_ind[right_count] = i;
      right_count++; 
    }
  }
  switch (right_count)
  {
    case 0:   /* reference point either inside or on the tile boundary */
      d = 0.0; 
      break;
    case 1:   /* take distance from the edge that turns right, such edge
                 is stored in right_ind */
      i = right_ind[0];
      if (CosAngle2D(x[i],y[i], x[(i+1)%3],y[(i+1)%3],
                     x[(i+1)%3],y[(i+1)%3], xp,yp) < 0)
      {   d = SquaredPointDist2D (xp, yp, x[(i+1)%3],y[(i+1)%3]);  }
      else if (CosAngle2D(x[(i+1)%3], y[(i+1)%3], x[i],y[i],
                          x[i],y[i], xp, yp) < 0)
           {   d = SquaredPointDist2D (xp, yp, x[i], y[i]);  }
           else
           {
              b = x[(i+1)%3] - x[i];
              a = - ( y[(i+1)%3] - y[i] );
              c = - ( y[i]*b + x[i]*a );
              d = SquaredPointLineDist2D (xp,yp, a,b,c);
           }
      break;
    case 2:   /* take distance from the common vertex of the two edges
                 that turn right, such edges are stored in right_ind */
      i = 3 - (3-right_ind[0]-((right_ind[0]+1)%3))
            - (3-right_ind[1]-((right_ind[1]+1)%3));
      d = SquaredPointDist2D (xp,yp, x[i],y[i]);
      break;
  }
  return (d);
}  

/*
Maximum distance of point (xp,yp) from a point of the triangle whose 
vertices are contained in arrays x[3] and y[3].
*/
float SquaredPointTriangleMaxDist2D(float xp, float yp, 
                                    float * x, float * y)
{
  float d, d1; /* distances */
  int i;       /* vertex counter */
  d = 0.0;
  for (i=0; i<3; i++)
  {
     d1 = SquaredPointDist2D (xp,yp, x[i],y[i]);
     if (d1>d) d = d1;
  }
  return d;
}

/* ------------------------------------------------------------------------ */

/* Approximated distances */

/* ------------------------------------------------------------------------ */

float ApproxSquaredDistanceTriangle(float * v1, float * v2, float * v3,
                                    float * p)
{ 
  float c[3];    /* triangle gravity center */
  float d, d1;   /* distances */

  TriangleCenter(v1[0],v1[1],v1[2], v2[0],v2[1],v2[2],
                 v3[0],v3[1],v3[2], &c[0], &c[1], &c[2]);
  d = SquaredPointDist(c[0],c[1],c[2], p[0],p[1],p[2]);
  d1 = SquaredPointDist(v1[0],v1[1],v1[2], p[0],p[1],p[2]);
  d = ( (d < d1) ? d : d1 );
  d1 = SquaredPointDist(v2[0],v2[1],v2[2], p[0],p[1],p[2]);
  d = ( (d < d1) ? d : d1 );
  d1 = SquaredPointDist(v3[0],v3[1],v3[2], p[0],p[1],p[2]);
  d = ( (d < d1) ? d : d1 );
  return (d);
}  

/* ------------------------------------------------------------------------ */

float ApproxSquaredDistanceTetra(float * v1, float * v2, 
                                 float * v3, float * v4, float * p)
{ 
  float c[3];    /* tetrahedron gravity center */
  float d, d1;   /* distances */

  if ( PointInTetra(p[0],p[1],p[2], v1,v2,v3,v4) )
      return 0.0;
  TetraCenter(v1,v2,v3,v4, &c[0], &c[1], &c[3]);    
  d = SquaredPointDist(c[0],c[1],c[2], p[0],p[1],p[2]);
  d1 = SquaredPointDist(v1[0],v1[1],v1[2], p[0],p[1],p[2]);
  d = ( (d < d1) ? d : d1 );
  d1 = SquaredPointDist(v2[0],v2[1],v2[2], p[0],p[1],p[2]);
  d = ( (d < d1) ? d : d1 );
  d1 = SquaredPointDist(v3[0],v3[1],v3[2], p[0],p[1],p[2]);
  d = ( (d < d1) ? d : d1 );
  d1 = SquaredPointDist(v4[0],v4[1],v4[2], p[0],p[1],p[2]);
  d = ( (d < d1) ? d : d1 );
  return (d);
}  

/* ------------------------------------------------------------------------ */
 
/* turns */

/* ------------------------------------------------------------------------ */

int PointTurn (float x, float y, float z,
               float x1, float y1, float z1, float x2, float y2, float z2)
{
  /****************** da fare ***************/
  return 0;
}

/* ------------------------------------------------------------------------ */

/* intersection of segments and lines */

/*
Test whether segment a intersects the supporting line of segment b
*/

int SegLineIntersect
      (float ax1,float ay1,float az1, float ax2,float ay2,float az2,
       float bx1,float by1,float bz1, float bx2,float by2,float bz2)
{
  int t1, t2;
  t1 = PointTurn (ax1,ay1,az1,bx1,by1,bz1,bx2,by2,bz2);
  t2 = PointTurn (ax2,ay2,az2,bx1,by1,bz1,bx2,by2,bz2);
  return ( (t1) && (t1==-t2) );
}

int SegLineIntersect2D
      (float ax1,float ay1, float ax2,float ay2,
       float bx1,float by1, float bx2,float by2)
{
  int t1, t2;
  t1 = PointTurn2D (ax1,ay1,bx1,by1,bx2,by2);
  t2 = PointTurn2D (ax2,ay2,bx1,by1,bx2,by2);
  return ( (t1) && (t1==-t2) );
}

int SegmentsIntersect 
      (float ax1,float ay1,float az1, float ax2,float ay2,float az2,
       float bx1,float by1,float bz1, float bx2,float by2,float bz2)
{
  return 
    ( SegLineIntersect(ax1,ay1,az1,ax2,ay2,az2,bx1,by1,bz1,bx2,by2,bz2)
      &&
      SegLineIntersect(bx1,by1,bz1,bx2,by2,bz2,ax1,ay1,az1,ax2,ay2,az2) );
}

int SegmentsIntersect2D
      (float ax1,float ay1, float ax2,float ay2,          
       float bx1,float by1, float bx2,float by2)
{
  return 
    ( SegLineIntersect2D (ax1,ay1,ax2,ay2,bx1,by1,bx2,by2)
      &&
      SegLineIntersect2D (bx1,by1,bx2,by2,ax1,ay1,ax2,ay2) );
}

/*
Test if two segments lie on the same supporting line
*/
int SegmentsAligned2D
      (float ax1,float ay1, float ax2,float ay2,
       float bx1,float by1, float bx2,float by2)
{  
   return ( (PointTurn2D(ax1,ay1,bx1,by1,bx2,by2)==NO_TURN) &&
            (PointTurn2D(ax2,ay2,bx1,by1,bx2,by2)==NO_TURN) );
}

/*
Test if two segments are partially overlapped
*/
int SegmentsOverlap2D
      (float ax1,float ay1, float ax2,float ay2,
       float bx1,float by1, float bx2,float by2)
{
   if ( SegmentsAligned2D(ax1,ay1,ax2,ay2,bx1,by1,bx2,by2) )
   {
      float t1, t2, d;
      /* considerando l'equazione del primo segmento come
         (x,y) = (ax1,ay1) + t * (ax2-ax1,ay2-ay1), con 0<=t<=1, calcola i
         coefficienti di (bx1,by1) e (bx2,by2) e ritorna 1 se non sono
         entrambi < 0 o entrambi > 1. Basta calcolarli tramite la x o la y
         perche' i due segmenti sono allineati. */ 
      if ( fabs(d=(ax2-ax1)) > ZERO )
      {  t1 = (bx1-ax1)/d; t2 = (bx2-ax1)/d;
      }
      else 
      {  d = ay2-ay1;
         t1 = (by1-ay1)/d;  t2 = (by2-ay1)/d; 
      } 
      if ((t1<0.0) && (t2<0.0)) return 0;
      if ((t1>1.0) && (t2>1.0)) return 0;
      return 1;
   }
   else return 0;
}

/* ------------------------------------------------------------------------ */

/* area of a triangle */

float TriangleArea (float x1, float y1, float z1,
                    float x2, float y2, float z2,
                    float x3, float y3, float z3)
{
  float l1, l2, l3; /* the three sides */
  float p; /* half perimeter */
  l1 = sqrt ( SquaredPointDist(x1,y1,z1,x2,y2,z2) );
  l2 = sqrt ( SquaredPointDist(x2,y2,z2,x3,y3,z3) );
  l3 = sqrt ( SquaredPointDist(x3,y3,z3,x1,y1,z1) );
  p = 0.5 * (l1+l2+l3);
  return ( sqrt ( p * (p-l1) * (p-l2) * (p-l3) ) );
}

float TriangleArea2D (float x1, float y1,
                      float x2, float y2,
                      float x3, float y3)
{
  float l1, l2, l3; /* the three sides */
  float p; /* half perimeter */
  l1 = sqrt ( SquaredPointDist2D (x1,y1,x2,y2) );
  l2 = sqrt ( SquaredPointDist2D (x2,y2,x3,y3) );
  l3 = sqrt ( SquaredPointDist2D (x3,y3,x1,y1) );
  p = 0.5 * (l1+l2+l3);
  return ( sqrt ( p * (p-l1) * (p-l2) * (p-l3) ) );
}

/* ------------------------------------------------------------------------ */

/* normal to a triangle */

void TriangleNormal (float x1, float y1, float z1,
                     float x2, float y2, float z2,
                     float x3, float y3, float z3,
                     float *x, float *y, float *z)
{
  double  a[3];
  double  b[3];
  double  norma;

  a[0] = x1-x2; a[1] = y1-y2; a[2] = z1-z2;
  b[0] = x1-x3; b[1] = y1-y3; b[2] = z1-z3;
  (*x) = a[1]*b[2] - a[2]*b[1];
  (*y) = a[2]*b[0] - a[0]*b[2];
  (*z) = a[0]*b[1] - a[1]*b[0];
  norma = sqrt( ((*x) * (*x)) + ((*y) * (*y)) + ((*z) * (*z)) );
  (*x) /= norma;
  (*y) /= norma;
  (*z) /= norma;
}

/* NOW A MACRO IN geo.h
void TriangleNormalv (float * v1, float * v2, float * v3, float * nrm)
{  
  TriangleNormal(v1[0],v1[1],v1[2], v2[0],v2[1],v2[2], v3[0],v3[1],v3[2],
                 &nrm[0], &nrm[1], &nrm[2]);
}
*/

/* ------------------------------------------------------------------------ */

/* gravity center of a triangle */

void TriangleCenter (float x1, float y1, float z1,
                     float x2, float y2, float z2,
                     float x3, float y3, float z3,
                     float *cx, float *cy, float *cz)
{
  (*cx) = (x1+x2+x3) / 3;
  (*cy) = (y1+y2+y3) / 3;
  (*cz) = (z1+z2+z3) / 3;
}

void TriangleCenter2D (float x1, float y1,
                       float x2, float y2,
                       float x3, float y3,
                       float *cx, float *cy)
{
  (*cx) = (x1+x2+x3) / 3;
  (*cy) = (y1+y2+y3) / 3;
}

void TetraCenter (float *v1, float *v2, float *v3, float *v4,
                  float *cx, float *cy, float *cz)
{
   (*cx) = (v1[0]+v2[0]+v3[0]+v4[0]) / 4;
   (*cy) = (v1[1]+v2[1]+v3[1]+v4[1]) / 4;
   (*cz) = (v1[2]+v2[2]+v3[2]+v4[2]) / 4;
}

/* ------------------------------------------------------------------------ */

/* point - in - triangle test */

int PointInTriangle (float x, float y, float z,
                     float x1, float y1, float z1,
                     float x2, float y2, float z2,
                     float x3, float y3, float z3)
{
  /****************** da fare ***************/
  fprintf(stderr,"PointInTriangle: Sorry, not implemented\n");
  return 0;
}

int PointInTriangle2D (float x, float y,
                       float x1, float y1,
                       float x2, float y2,
                       float x3, float y3)
{
  return ( (PointTurn2D(x,y,x1,y1,x2,y2) == LEFT_TURN) &&
           (PointTurn2D(x,y,x2,y2,x3,y3) == LEFT_TURN) &&
           (PointTurn2D(x,y,x3,y3,x1,y1) == LEFT_TURN) );
}

int PointOutTriangle2D (float x, float y,
                           float x1, float y1,
                           float x2, float y2,
                           float x3, float y3)
{
  return ( (PointTurn2D(x,y,x1,y1,x2,y2) == RIGHT_TURN) ||
           (PointTurn2D(x,y,x2,y2,x3,y3) == RIGHT_TURN) ||
           (PointTurn2D(x,y,x3,y3,x1,y1) == RIGHT_TURN) );
}

/* ------------------------------------------------------------------------ */
/*                      Point-in-tetrahedron test                           */
/* ------------------------------------------------------------------------ */

/*
Here the tetrahedron is given as three arrays x[4], y[4], z[4] containing
the coordinates of its four vertices
*/

int PointInTetra (float xp, float yp, float zp,
                  float * v1, float * v2, float * v3, float * v4)
{
  static float * v[4] = {v1, v2, v3, v4};
  int i, turn;
  for (i=0;i<4;i++)
  {
    turn = FourPointTurn(xp, yp, zp,
                         v[i][0],       v[i][1],       v[i][2],
                         v[(i+1)%4][0], v[(i+1)%4][1], v[(i+1)%4][2],
                         v[(i+2)%4][0], v[(i+2)%4][1], v[(i+2)%4][2]);
    if ((turn == NO_TURN) ||
        (turn != FourPointTurn(v[(i+3)%4][0], v[(i+3)%4][1], v[(i+3)%4][2],
                               v[i][0],       v[i][1],       v[i][2],
                               v[(i+1)%4][0], v[(i+1)%4][1], v[(i+1)%4][2],
                               v[(i+2)%4][0], v[(i+2)%4][1], v[(i+2)%4][2]) ))
    return 0;
  }
  return 1;
}

int PointOutTetra (float xp, float yp, float zp,
                   float * v1, float * v2, float * v3, float * v4)
{
  static float * v[4] = {v1, v2, v3, v4};
  int i, turn;
  for (i=0;i<4;i++)
  {
    turn = FourPointTurn(xp, yp, zp,
                         v[i][0],       v[i][1],       v[i][2],
                         v[(i+1)%4][0], v[(i+1)%4][1], v[(i+1)%4][2],
                         v[(i+2)%4][0], v[(i+2)%4][1], v[(i+2)%4][2]);
    if ((turn != NO_TURN) &&
        (turn != FourPointTurn(v[(i+3)%4][0], v[(i+3)%4][1], v[(i+3)%4][2],
                               v[i][0],       v[i][1],       v[i][2],
                               v[(i+1)%4][0], v[(i+1)%4][1], v[(i+1)%4][2],
                               v[(i+2)%4][0], v[(i+2)%4][1], v[(i+2)%4][2]) ))
    return 1;
  }
  return 0;
}

/* ------------------------------------------------------------------------ */

/* cos of angle formed by two vectors */

float CosAngle (float ax1,float ay1,float az1, float ax2,float ay2,float az2,
                float bx1,float by1,float bz1, float bx2,float by2,float bz2)
{
  float vx[2], vy[2], vz[2]; /* vector directions */
  float l[2]; /* vector lengths */
  vx[0] = ax2-ax1; vy[0] = ay2-ay1; vz[0] = az2-az1; 
  vx[1] = bx2-bx1; vy[1] = by2-by1; vz[1] = bz2-bz1;
  l[0] = sqrt (vx[0]*vx[0] + vy[0]*vy[0] + vz[0]*vz[0]);
  l[1] = sqrt (vx[1]*vx[1] + vy[1]*vy[1] + vz[1]*vz[1]); 
  if (l[0] > ZERO) 
  {  vx[0] /= l[0]; vy[0] /= l[0]; vz[0] /= l[0];  }
  if (l[1] > ZERO)
  {  vx[1] /= l[1]; vy[1] /= l[1]; vz[1] /= l[1];  }
  return ( vx[0]*vx[1] + vy[0]*vy[1] + vz[0]*vz[1] );
}

float CosAngle2D (float ax1, float ay1, float ax2, float ay2,
                  float bx1, float by1, float bx2, float by2)
{
  float vx[2], vy[2]; /* vector directions */
  float l[2]; /* vector lengths */
  vx[0] = ax2-ax1; vy[0] = ay2-ay1;
  vx[1] = bx2-bx1; vy[1] = by2-by1;
  l[0] = sqrt (vx[0]*vx[0] + vy[0]*vy[0]);
  l[1] = sqrt (vx[1]*vx[1] + vy[1]*vy[1]);
  if (l[0] > ZERO) 
  {  vx[0] /= l[0]; vy[0] /= l[0];  }
  if (l[1] > ZERO) 
  {  vx[1] /= l[1]; vy[1] /= l[1];  }
  return ( vx[0]*vx[1] + vy[0]*vy[1] );
}

int PointAngle2D (float x, float y, float x1, float y1, float x2, float y2)
{
  float vx[2], vy[2]; /* vector directions */
  float val;
  vx[0] = x1-x; vy[0] = y1-y;
  vx[1] = x2-x1; vy[1] = y2-y1;
  val = ( vx[0]*vx[1] + vy[0]*vy[1] );
  if (val > ZERO ) return (+1);
  if (val < ZERO ) return (-1);
  return 0;
}

/* ------------------------------------------------------------------------ */
/*                       Intersection test w.r.t. a box                     */
/* ------------------------------------------------------------------------ */

/*
WITH A TWO-DIMENSIONAL BOX
Implementation based on the line clipping algorithm by Liang-Barsky.
The basic idea is the following:
Consider the parametric equation of the segment to be clipped:

x(u) = x1 + u*(x2-x1)             with  0<=u<=1
y(u) = y1 + u*(y2-y1)
The segment intersects the box if there exists u, 0<=u<=1, such that
minX <= x(u) <= maxX   and   minY <= y(u) <= maxY
or, equivalently
minX-x1 <= u*(x2-x1) <= maxX-x1   and   minY-y1 <= u*(y2-y1) <= maxY-y1

We have four inequalities that give four conditions on u.
Check if the four conditions are mutually consistent and 
consistent with condition 0<=u<=1.
*/

/* 
Restrict the admissible interval [u1,u2] by intersecting it with the
half-line, solution of inequality u*p <= q.
Return 1 if the resulting interval is not empty, 0 otherwise.
*/
static int ClipTest2D (float p, float q, float * u1, float * u2)
{
  float r;

  if (p<0.0)
  {
    r = q/p;
    if (r > (*u2)) return 0;
    else if (r > (*u1)) (*u1) = r;
  }
  else
  {
    if (p>0.0)
    {
      r = q/p;
      if (r < (*u1)) return 0;
      else if (r < (*u2)) (*u2) = r;
    }  
    else
    {
      /* p==0.0 line parallel to clipping edge */
      if (q<0.0) return 0;
    }
  }
  return 1;
}

/*
Return 1 if the segment is at least partially inside the box.
*/
int ClipLine2D (float minX, float minY, float maxX, float maxY, /* box */
                float x1, float y1, float x2, float y2) /* line */
{
  float u1 = 0.0, u2 = 1.0; /* admissible interval, initially all [0,1] */
  float dx = x2-x1, dy = y2-y1;  
  if ( 
       ClipTest2D(-dx, x1-minX, &u1, &u2) &&
       ClipTest2D(dx, maxX-x1, &u1, &u2) &&
       ClipTest2D(-dy, y1-minY, &u1, &u2) &&
       ClipTest2D(dy, maxY-y1, &u1, &u2) 
     )
  {  return 1;  }
  return 0;
}

/*
Return 1 if the triangle is at least partially inside the box.
*/
int ClipTriangle2D(float minX, float minY, float maxX, float maxY, /* box */
                   float x[3], float y[3]) /* triangle */
{
  int i;
  /* if a vertex is inside the box, then the triangle intersects the box */
  for (i=0;i<3;i++)
  {
    if ( (x[i]<maxX) && (x[i]>minX) && (y[i]<maxY) && (y[i]>minY) )
    {  return 1;  }
  }
  /* if an edge is at least partially inside the box, then the triangle
     intersects the box */
  for (i=0;i<3;i++)
  {
    if (ClipLine2D (minX,minY,maxX,maxY,
                    x[i],y[i],x[(i+1)%3],y[(i+1)%3]))
    {  return 1;  }
  }
  /* none of the three triangle edges intersects the box */  
  /* check if the triangle completely contains the box
     by applying the point-in-triangle test on any vertex of the box */
  if (PointInTriangle2D(minX,minY,x[0],y[0],x[1],y[1],x[2],y[2]))
  {  return 1;  } 
  return 0;
}

/* ------------------------------------------------------------------------ */
/*
WITH A THREE-DIMENSIONAL BOX

Again, the implementation is based on the line clipping algorithm by
Liang-Barsky.

By applying the same idea used in 2D, we get six inequalities on 
parameter u.
We check whether such inequalities are mutually consistent and
consistent with condition 0<=u<=1.
*/

/*
Restrict the admissible interval [u1,u2] by intersecting it with the
half-line, solution of inequality u*p <= q.
Return 1 if the resulting interval is not empty, 0 otherwise.
*/
static int ClipTest3D (float p, float q, float * u1, float * u2)
{
  float r;
  if (p<0.0)
  {
    r = q/p;
    if (r > (*u2)) return 0;
    else if (r > (*u1)) (*u1) = r;
  }
  else
  {
    if (p>0.0)
    {
      r = q/p;
      if (r < (*u1)) return 0;
      else if (r < (*u2)) (*u2) = r;
    }  
    else
    {
      /* p==0.0 line parallel to clipping edge */
      if (q<0.0) return 0;
    }
  }
  return 1;
}

/*
Return 1 if the segment is at least partially inside the box.
*/
int ClipLine3D (float minX, float minY, float minZ,
                float maxX, float maxY, float maxZ, /* box */
                float x1, float y1, float z1, 
                float x2, float y2, float z2) /* line */
{
  float u1 = 0.0, u2 = 1.0; /* admissible interval, initially all [0,1] */
  float dx = x2-x1, dy = y2-y1, dz = z2-z1;

  if ( ClipTest3D(-dx, x1-minX, &u1, &u2) &&
       ClipTest3D(dx, maxX-x1, &u1, &u2) &&
       ClipTest3D(-dy, y1-minY, &u1, &u2) &&
       ClipTest3D(dy, maxY-y1, &u1, &u2) &&
       ClipTest3D(-dz, z1-minZ, &u1, &u2) &&
       ClipTest3D(dz, maxZ-z1, &u1, &u2) )  
  {  return 1;  }
  return 0;
}

/*
Return 1 if the triangle is at least partially inside the box.
*/
int ClipTriangle3D(float minX, float minY, float minZ,
                   float maxX, float maxY, float maxZ, /* box */
                   float x[3], float y[3], float z[3]) /* triangle */
{                   
  int i;
  
  /* if a vertex is inside the box, then the triangle intersects the box */
  for (i=0;i<3;i++)
  {
    if ( (x[i]<maxX) && (x[i]>minX) && (y[i]<maxY) && (y[i]>minY) &&
         (z[i]<maxZ) && (z[i]>minZ) )
    {  return 1;  }
  }
  /* if an edge is at least partially inside the box, then the triangle
     intersects the box */
  for (i=0;i<3;i++)
  {
    if ( ClipLine3D (minX,minY,minZ,maxX,maxY,maxZ,
                     x[i],y[i],z[i],
                     x[(i+1)%3],y[(i+1)%3],z[(i+1)%3]) )
    {  return 1;  }
  }
  /* none of the three triangle edges intersects the box */
  /* check if the triangle completely contains the box
     by applying the point-in-triangle test on any vertex of the box */
  if ( (x[0]<maxX) && (x[0]>minX) &&
       (y[0]<maxY) && (y[0]>minY) &&
       (z[0]<maxZ) && (z[0]>minZ) )
  {  return 1;  }
  return 0;
}

/* ------------------------------------------------------------------------ */
/*              Radius of the circumcircle of a triangle                    */
/* ------------------------------------------------------------------------ */

float CircumRadius(float x1, float y1, float z1,
                   float x2, float y2, float z2,
                   float x3, float y3, float z3)
{
 float a,b,c,s,n,d;

 a = sqrt(SquaredPointDist(x1,y1,z1,x2,y2,z2));
 b = sqrt(SquaredPointDist(x2,y2,z2,x3,y3,z3));
 c = sqrt(SquaredPointDist(x3,y3,z3,x1,y1,z1));
 s = (a+b+c)/2.0;
 n = (a*b*c);
 d = ( 4.0* sqrt(s*(s-a)*(s-b)*(s-c)) );
 if (d < ZERO) 
 {  /* ERROR */ return 0.0;  }
 return (n/d);
}

float CircumRadius2D(float x1, float y1,  float x2, float y2, 
                     float x3, float y3)
{
 float a,b,c; /* edge lengths */
 float s; /* half perimeter */
 float n,d;
 
 a = sqrt(SquaredPointDist2D(x1,y1,x2,y2));
 b = sqrt(SquaredPointDist2D(x2,y2,x3,y3));
 c = sqrt(SquaredPointDist2D(x3,y3,x1,y1));
 s = (a+b+c)/2.0;
 n = (a*b*c);
 d = ( 4.0* sqrt(s*(s-a)*(s-b)*(s-c)) );
 if (d < ZERO)
 {  /* ERROR */ return 0.0;  }
 return (n/d);
}
