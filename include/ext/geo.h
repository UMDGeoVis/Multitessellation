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

/* -------------------------------- geo.h --------------------------------- */

#include <math.h>
#include <float.h>

#ifndef GEO_INCLUDED
#define GEO_INCLUDED

#ifndef PI
#define PI 3.14159265358979323846
#endif

/* ------------------------------------------------------------------------ */
/*            GEOMETRIC UTILITIES ON POINTS, LINES, TRIANGLES               */
/* ------------------------------------------------------------------------ */

/*
This file contains functions implementing geometric computations and
tests on in 2D and 3D.
Points, lines, triangles are simply given as tuples of floats without
assuming any special data structure for them.

Something about tetrahedra has also been added later.
*/

/* ------------------------------------------------------------------------ */
/*                    Tolerance used in computations                        */
/* ------------------------------------------------------------------------ */

#define ZERO (10E-14)
#define Coincide(a,b) (fabs((a)-(b))<=ZERO)

/* ------------------------------------------------------------------------ */
/*                     Basic arithmetic functions                           */
/* ------------------------------------------------------------------------ */

#define Square(x) ((x)*(x))

/* 
Calculate determinant  |a b|
                       |c d|
*/                          
#define Det2D(a,b,c,d)  ( (a*d)-(b*c) )


/* 
Calculate sign of determinant  |a b|
                               |c d|
*/
int DetSign2D (double a, double b, double c, double d);

/* 
Calculate determinant  |a1 a2 a3|
                       |b1 b2 b3|
                       |c1 c2 c3|
*/                          
#define Det3D(a1,a2,a3,b1,b2,b3,c1,c2,c3)	\
	( a1*Det2D(b2,b3,c2,c3) - a2*Det2D(b1,b3,c1,c3) + a3*Det2D(b1,b2,c1,c2) )

/* 
Calculate sign of determinant |a1 a2 a3|
                              |b1 b2 b3|
                              |c1 c2 c3|
*/
int DetSign3D(double a1, double a2, double a3, 
              double b1, double b2, double b3,
              double c1, double c2, double c3);

void normalize(float *x, float *y, float *z);
void normalize2D(float *x, float *y);

/* ------------------------------------------------------------------------ */
/*                               Distances                                  */
/* ------------------------------------------------------------------------ */

#define SquaredPointDist(x1,y1,z1,x2,y2,z2)	\
	( Square((x2)-(x1)) + Square((y2)-(y1)) + Square((z2)-(z1)) )

#define SquaredPointDist2D(x1,y1,x2,y2)	\
	( Square((x2)-(x1)) + Square((y2)-(y1)) )

/*
Distance of point (xp,yp) from line ax+by+c=0
*/
#define SquaredPointLineDist2D(xp,yp,a,b,c)	\
	( Square (a*xp+b*yp+c) / ( Square(a)+Square(b) ) )

#define PointsCoincide(x1,y1,z1,x2,y2,z2)	\
	( Coincide((x1),(x2)) && Coincide((y1),(y2)) && Coincide((z1),(z2)) )

#define PointsCoincide2D(x1,y1,x2,y2)	\
	( Coincide((x1),(x2)) && Coincide((y1),(y2)) )

/*
Distance of point (xp,yp) from triangle whose vertices are contained
in arrays x[3] and y[3].
*/
float SquaredPointTriangleDist2D(float xp, float yp, float * x, float * y);

/*
Maximum distance of point (xp,yp) from a point of the triangle whose 
vertices are contained in arrays x[3] and y[3].
*/
float SquaredPointTriangleMaxDist2D(float xp, float yp,    
                                    float * x, float * y);

/* ------------------------------------------------------------------------ */
/*                          Approximated distances                          */
/* ------------------------------------------------------------------------ */

/*
Approximated squared distance of a point p from a triangle v1,v2,v3
in space. Parameters p, v1,v2,v3 represent points and are arrays of 
(at least) three floats.
*/
float ApproxSquaredDistanceTriangle(float * v1, float * v2, float * v3,
                                    float * p);

/*
Approximated squared distance of a point p from a tetrahedron v1,v2,v3,v4
in space. Parameters p, v1,v2,v3,v4 represent points and are arrays of
(at least) three floats.
*/
float ApproxSquaredDistanceTetra(float * v1, float * v2, 
                                 float * v3, float * v4, float * p);

/* ------------------------------------------------------------------------ */
/*                                 Turns                                    */ 
/* ------------------------------------------------------------------------ */

#define LEFT_TURN -1
#define UP_TURN -1
#define NO_TURN 0
#define RIGHT_TURN 1
#define DOWN_TURN 1

int PointTurn(float x, float y, float z,
              float x1, float y1, float z1, float x2, float y2, float z2);

#define FourPointTurn(x,y,z,x1,y1,z1,x2,y2,z2,x3,y3,z3)	\
	DetSign3D ((x1)-(x), (y1)-(y), (z1)-(z),	\
		(x2)-(x1), (y2)-(y1), (z2)-(z1),	\
		(x3)-(x1), (y3)-(y1), (z3)-(z1) )

#define PointTurn2D(x,y,x1,y1,x2,y2)	\
	DetSign2D ((x)-(x1), (y)-(y1), (x2)-(x1), (y2)-(y1))

/* ------------------------------------------------------------------------ */
/*                           Point-in-box test                              */
/* ------------------------------------------------------------------------ */

#define PointInBox(x,y,z,x1,y1,z1,x2,y2,z2)	\
	( (x)>(x1) && (x)<(x2) && (y)>(y1) && (y)<(y2) && (z)>(z1) && (z)<(z2) )

#define PointInBox2D(x,y,x1,y1,x2,y2)	\
	( (x)>(x1) && (x)<(x2) && (y)>(y1) && (y)<(y2) )

/* ------------------------------------------------------------------------ */
/*                 Intersection of segments and lines                       */
/* ------------------------------------------------------------------------ */

/*
Test whether segment a intersects the supporting line of segment b
*/
int SegLineIntersect
      (float ax1,float ay1,float az1, float ax2,float ay2,float az2,
       float bx1,float by1,float bz1, float bx2,float by2,float bz2);

int SegLineIntersect2D
      (float ax1,float ay1, float ax2,float ay2,
       float bx1,float by1, float bx2,float by2);

int SegmentsIntersect2D
      (float ax1,float ay1, float ax2,float ay2,
       float bx1,float by1, float bx2,float by2);

/*
Test if two segments lie on the same supporting line
*/
int SegmentsAligned2D
      (float ax1,float ay1, float ax2,float ay2,
       float bx1,float by1, float bx2,float by2);

/*
Test if two segments are partially overlapped
*/
int SegmentsOverlap2D
      (float ax1,float ay1, float ax2,float ay2,
       float bx1,float by1, float bx2,float by2);

/* ------------------------------------------------------------------------ */
/*                          Area of a triangle                              */
/* ------------------------------------------------------------------------ */

float TriangleArea (float x1, float y1, float z1,
                    float x2, float y2, float z2,
                    float x3, float y3, float z3);

float TriangleArea2D (float x1, float y1,
                      float x2, float y2,
                      float x3, float y3);

/* ------------------------------------------------------------------------ */
/*                         Normal to a triangle                             */
/* ------------------------------------------------------------------------ */

void TriangleNormal (float x1, float y1, float z1,
                     float x2, float y2, float z2,
                     float x3, float y3, float z3,
                     float *x, float *y, float *z);

//void TriangleNormalv (float * v1, float * v2, float * v3, float * nrm);

#define TriangleNormalv(vv1,vv2,vv3,vnrmp)	\
	( TriangleNormal((vv1)[0],(vv1)[1],(vv1)[2],	\
	(vv2)[0],(vv2)[1],(vv2)[2],(vv3)[0],(vv3)[1],(vv3)[2],	\
	&(vnrmp)[0], &(vnrmp)[1], &(vnrmp)[2]) )

/* ------------------------------------------------------------------------ */
/*                       Gravity center of a triangle                       */
/* ------------------------------------------------------------------------ */

/*
Gravity center of a triangle in 3D and in 3D.
*/

void TriangleCenter (float x1, float y1, float z1,
                     float x2, float y2, float z2,
                     float x3, float y3, float z3,
                     float *cx, float *cy, float *cz);

void TriangleCenter2D (float x1, float y1,
                       float x2, float y2,
                       float x3, float y3,
                       float *cx, float *cy);

/*
Gravity center of a tetrahedron in 3D.
Here the tetrahedron is given as four arrays v1[3], v2[3], v3[3], v4[3]
containing the coordinates of its four vertices.
*/

void TetraCenter (float *v1, float *v2, float *v3, float *v4,
                  float *cx, float *cy, float *cz);

/* ------------------------------------------------------------------------ */
/*                      Point-in-triangle test                              */
/* ------------------------------------------------------------------------ */

/*
Test if point strictly inside or strictly outside triangle.
*/

int PointInTriangle (float x, float y, float z,
                     float x1, float y1, float z1,
                     float x2, float y2, float z2,
                     float x3, float y3, float z3);

int PointInTriangle2D (float x, float y,
                       float x1, float y1,
                       float x2, float y2,
                       float x3, float y3);

int PointOutTriangle2D (float x, float y,
                        float x1, float y1,
                        float x2, float y2,
                        float x3, float y3);

/* ------------------------------------------------------------------------ */
/*                      Point-in-tetrahedron test                           */
/* ------------------------------------------------------------------------ */

/*
Test if point strictly inside or strictly outside tetrahedron.
Here the tetrahedron is given as four arrays v1[3], v2[3], v3[3], v4[3]
containing the coordinates of its four vertices.
*/

int PointInTetra (float xp, float yp, float zp,
                  float * v1, float * v2, float * v3, float * v4);

int PointOutTetra (float xp, float yp, float zp,
                  float * v1, float * v2, float * v3, float * v4);

/* ------------------------------------------------------------------------ */
/*                  Cos of angle formed by two vectors                      */
/* ------------------------------------------------------------------------ */

float CosAngle (float ax1,float ay1,float az1, float ax2,float ay2,float az2,
                float bx1,float by1,float bz1, float bx2,float by2,float bz2);

float CosAngle2D (float ax1, float ay1, float ax2, float ay2,
                  float bx1, float by1, float bx2, float by2);
               
/* ------------------------------------------------------------------------ */
/*                       Intersection test w.r.t. a box                     */
/* ------------------------------------------------------------------------ */

/*
Return 1 if the segment/triangle is at least partially inside the box.
*/

int ClipLine2D (float minX, float minY, float maxX, float maxY, /* box */
                float x1, float y1, float x2, float y2); /* line */

int ClipTriangle2D(float minX, float minY, float maxX, float maxY, /* box */
                   float x[3], float y[3]); /* triangle */

int ClipLine3D (float minX, float minY, float minZ,
                float maxX, float maxY, float maxZ, /* box */
                float x1, float y1, float z1, 
                float x2, float y2, float z2); /* line */

int ClipTriangle3D(float minX, float minY, float minZ,
                   float maxX, float maxY, float maxZ, /* box */
                   float x[3], float y[3], float z[3]); /* triangle */

/* ------------------------------------------------------------------------ */
/*              Radius of the circumcircle of a triangle                    */
/* ------------------------------------------------------------------------ */

float CircumRadius(float x1, float y1, float z1,
                   float x2, float y2, float z2,
                   float x3, float y3, float z3);

float CircumRadius2D(float x1, float y1,  float x2, float y2, 
                     float x3, float y3);

#endif /* GEO_INCLUDED */
