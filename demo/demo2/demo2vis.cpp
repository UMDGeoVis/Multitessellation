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

/*
TERRAIN DEMO
by Paola Magillo - DISI - University of Genova - Italy
Written JANUARY 1999, revised OCTOBER 1999
*/

/* ----------------------------------------------------------------------- */
/*                              include                                    */
/* ----------------------------------------------------------------------- */

#include <stdlib.h>
#include <stdio.h>
//#include <values.h>
#include <GL/glut.h>
#include <math.h>

#include "geo.h" /* for TriangleNormal */
#include "demo2.h"

/* ----------------------------------------------------------------------- */
/*                               MACROS                                    */
/* ----------------------------------------------------------------------- */

/* External view sees the whole scene,
   internal view sees just the part insidse the frustum. */
#define EXTERNAL_VIEW 0
#define INTERNAL_VIEW 1

/* Drawing style: hidden wireframe, solid (flat shading),
   solid (smooth shading) */
#define HIDDEN_WIRE 1
#define SOLID_FACES 2
#define SMOOTH_SHAD 3

/* Constant for hidden wireframe. */
const float ZTWIST=0.000005f;  /*CIGNONI: 0.0005f;*/

/* Select algorithm for drawing in hidden wireframe mode. */
#ifdef HIDDEN_ZBUF
#define drawHidden drawHidden_zbuffer
        /* fast bu result is not always correct */
#else
#ifdef HIDDEN_STENCIL
#define drawHidden drawHidden_stencil
        /* correct but slow */
#else
#define drawHidden drawHidden_stencil
        /* default */
#endif
#endif

/* ----------------------------------------------------------------------- */
/*                               GLOBAL STATE                              */
/* ----------------------------------------------------------------------- */

/* Flag if idle callback set. */
int is_idle = 0;

/* Length of scene diagonal. */
float diagonal;

/* Max height of an extracted triangle. */
float max_height;
/* (already scaled i.e., multiplied by factor) */

/* offset for raising viewpoint */
float offset;

/* Scale factor for terrain heights. */
float factor = 1.0;

/* Flag if we are drawing active triangles. */
int drawing_active_tri;

/* Factors for view frustum. */
float x_a = 3.8;
float y_a = 8.0;

/* Window identifiers: 0 = internal view, 1 = external view, 2 = dialog. */
int window[3];

/* Colors for graphic windows. */ 
GLfloat back_color[3] = { 0.0, 0.6, 0.8 };
GLfloat fore_color[3] = { 1.0, 1.0, 1.0 };
GLfloat high_color[3] = { 0.5, 0.8, 0.5 }; /*0.7, 1.0, 0.7*/
GLfloat box_color[3]  = { 1.0, 0.0, 0.0 };
GLfloat fore_fillcolor[3] = { 0.5, 0.7, 0.5 };
GLfloat high_fillcolor[3] = { 0.1, 0.6, 0.1 };

/* Colors for dialog window */
GLfloat page_color[3] = { 1.0, 1.0, 1.0 };
GLfloat text_color[3] = { 0.0, 0.0, 0.0 };

/* Scale factor. */
static float zoom_factor = 1.0;

/* Translation movements. */
static float x_move = 0.0;
static float y_move = 0.0;
static float z_move = 0.0;

/* Drawing style. */
static int style = SOLID_FACES;

/* Window dimensions */
static float window_w[3] = { 360.0, 360.0, 280.0 };
static float window_h[3] = { 360.0, 360.0, 360.0 };

/* Motion parameters */
int direction = 0; /* 0 = stop, 1 = forward, -1 = backward */
int turnangle = 0; /* 0 = straight, 1 = left, -1 = right */

/* ----------------------------------------------------------------------- */
/*                      AUXILIARY FUNCTIONS FOR DRAWING                    */
/* ----------------------------------------------------------------------- */

void setTriangle9f(float x1, float y1, float z1,
                   float x2, float y2, float z2,	
                   float x3, float y3, float z3)
{
  float nx, ny, nz;
  /* the following function is defined in geo.h */
  TriangleNormal(x1,y1,z1,x2,y2,z2,x3,y3,z3,&nx,&ny,&nz);
    glNormal3f(nx,ny,nz);
    glVertex3f(x1,y1,z1);
    glVertex3f(x2,y2,z2);
    glVertex3f(x3,y3,z3);
  if (drawing_active_tri)
  {  if (z1 > max_height) max_height = z1;
     if (z2 > max_height) max_height = z2;
     if (z3 > max_height) max_height = z3;
  }
}

void setTriangle(MT_INDEX t)
{
  MT_INDEX * v;
  mt->MT_TileVertices(t,&v);
  setTriangle9f(mt->MT_VertexX(v[0]),mt->MT_VertexY(v[0]),
                factor*mt->MT_VertexZ(v[0]),
                mt->MT_VertexX(v[1]),mt->MT_VertexY(v[1]),
                factor*mt->MT_VertexZ(v[1]),
                mt->MT_VertexX(v[2]),mt->MT_VertexY(v[2]),
                factor*mt->MT_VertexZ(v[2]));
}

void setSmoothTriangle18f(float x1, float y1, float z1,
                          float nx1, float ny1, float nz1,
                          float x2, float y2, float z2,	
                          float nx2, float ny2, float nz2,
                          float x3, float y3, float z3,
                          float nx3, float ny3, float nz3)
{
    glNormal3f(nx1,ny1,nz1);
    glVertex3f(x1,y1,z1);
    glNormal3f(nx2,ny2,nz2);
    glVertex3f(x2,y2,z2);
    glNormal3f(nx3,ny3,nz3);
    glVertex3f(x3,y3,z3);
  if (drawing_active_tri)
  {  if (z1 > max_height) max_height = z1;
     if (z2 > max_height) max_height = z2;
     if (z3 > max_height) max_height = z3;
  }
}

void setSmoothTriangle(MT_INDEX t)
{
  MT_INDEX * v;
  mt->MT_TileVertices(t,&v);
  setSmoothTriangle18f(mt->MT_VertexX(v[0]),mt->MT_VertexY(v[0]),
                       factor*mt->MT_VertexZ(v[0]),
                       vn->VertexNX(v[0]),vn->VertexNY(v[0]),
                       vn->VertexNZ(v[0]),
                       mt->MT_VertexX(v[1]),mt->MT_VertexY(v[1]),
                       factor*mt->MT_VertexZ(v[1]),
                       vn->VertexNX(v[1]),vn->VertexNY(v[1]),
                       vn->VertexNZ(v[1]),
                       mt->MT_VertexX(v[2]),mt->MT_VertexY(v[2]),
                       factor*mt->MT_VertexZ(v[2]),
                       vn->VertexNX(v[2]),vn->VertexNY(v[2]),
                       vn->VertexNZ(v[2]));
}

/* Transform the scene in such a way that it fits in the default OpenGL
   view volume (the cube with coordinates between -1 and +1). */
void trasfScene(void)
{
 int view_type;

 if (glutGetWindow()==window[0])  view_type = EXTERNAL_VIEW;
 else   view_type = INTERNAL_VIEW;
 switch(view_type)
 {
   case EXTERNAL_VIEW:

     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     glTranslatef(x_move,y_move,z_move);

     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
     glScalef(zoom_factor*2.0/diagonal,
              zoom_factor*2.0/diagonal,
              zoom_factor*2.0/diagonal);
     glTranslatef(-0.5*(mt->MT_MaxX() + mt->MT_MinX()),
                  -0.5*(mt->MT_MaxY() + mt->MT_MinY()),
                  -0.5*factor*(mt->MT_MaxZ() + mt->MT_MinZ()));
     break;

   case INTERNAL_VIEW:
     {
       float center[2], direz[2], angle;
       focus->TheVertex(center);
       focus->TheAxisDir(direz);
       angle = focus->TheAngle(0);
       glMatrixMode(GL_PROJECTION);
       glLoadIdentity();
       gluPerspective(y_a*4.0*angle, x_a*1.0, 0.02, diagonal);

       glMatrixMode(GL_MODELVIEW);
       glLoadIdentity();
       gluLookAt(center[0], center[1], (1.0+offset)*max_height,
/* PARTE SOSTITUITA
                 center[0]+20.0*direz[0], center[1]+20.0*direz[1], 
DALLE DUE RIGHE SOTTO */
                 center[0]+0.1*diagonal*direz[0],
                 center[1]+0.1*diagonal*direz[1],
/* FINE SOSTITUZIONE */
                 (1.0-offset)*max_height,
                 0.0, 0.0, 1.0);
     }
     break;
 }
}       

void setLight(void)
{
  static GLfloat pos0[4] = { 0.0, 5.0, 6.0, 0.0 };
  static GLfloat ambient0[4] = { 0.0, 0.0, 0.0, 1.0 };
  static GLfloat diffuse0[4] = { 1.0, 1.0, 1.0, 1.0 };
  static GLfloat pos1[4] = { 5.0, 0.0, 5.0, 0.0 };
  static GLfloat ambient1[4] = { 0.4, 0.4, 0.3, 1.0 };
  static GLfloat diffuse1[4] = { 0.3, 0.4, 0.4, 1.0 };
  glLightfv(GL_LIGHT0, GL_POSITION, pos0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
  glLightfv(GL_LIGHT1, GL_POSITION, pos1);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
  glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
}

/* Redraw all windows. */
void redrawWindows(void)
{
  int w;
  for (w=0;w<3;w++)
  {  glutSetWindow(window[w]);
     glutPostRedisplay();
  }
}

/* ----------------------------------------------------------------------- */
/*                           DRAWING FUNCTIONS                             */
/* ----------------------------------------------------------------------- */

/* Draw view frustum. */
void drawFocus(void)
{
    float center[2], direz[2];
    float l[2], r[2];

    focus->TheVertex(center);
    focus->TheAxisDir(direz);
    focus->TheLeftPoint(&l[0],&l[1],diagonal);
    focus->TheRightPoint(&r[0],&r[1],diagonal);

    glPointSize(5.0);
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, box_color);
    glColor3fv(box_color);
    glBegin(GL_POINTS);
      glVertex3f(center[0], center[1], (1.0+offset)*max_height);
/*
      glVertex3f(center[0]+20.0*direz[0], center[1]+20.0*direz[1], 
      (1.0-offset)*max_height);
*/
    glEnd();
    glLineWidth(1.0);
    glBegin(GL_LINES);
      glVertex3f(center[0], center[1], (1.0+offset)*max_height);
      glVertex3f(l[0], l[1], (1.0+offset)*max_height);
      glVertex3f(center[0], center[1], (1.0+offset)*max_height);
      glVertex3f(r[0], r[1], (1.0+offset)*max_height);
    glEnd();
}

/* Wireframe. */
void drawWire(void)
{
  int i;
  MT_INDEX * active_tri;
  MT_INDEX * other_tri;
  MT_INDEX num_active;
  MT_INDEX num_other;
  
  ex->MT_AllExtractedTiles(&active_tri,&num_active,
                           &other_tri,&num_other);
  max_height = factor * mt->MT_MinZ();
  drawing_active_tri = 1;
  glLineWidth(1.0);
  for (i=0;i<num_active;i++)
  {
    glColor3fv(high_fillcolor);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glBegin(GL_TRIANGLES);
      setTriangle(active_tri[i]);
    glEnd();
    glColor3fv(high_color);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glBegin(GL_TRIANGLES);
      setTriangle(active_tri[i]); 
    glEnd();
  }

  drawing_active_tri = 0;
  for (i=0;i<num_other;i++)
  {
    glColor3fv(fore_fillcolor);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glBegin(GL_TRIANGLES);
      setTriangle(other_tri[i]);
    glEnd();
    glColor3fv(fore_color);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glBegin(GL_TRIANGLES);
      setTriangle(other_tri[i]); 
    glEnd();
  }
}

/* Solid, flat shading. */
void drawSolid(void)
{
  int i;
  MT_INDEX * active_tri;
  MT_INDEX * other_tri;
  MT_INDEX num_active;
  MT_INDEX num_other;
  
  ex->MT_AllExtractedTiles(&active_tri,&num_active,
                           &other_tri,&num_other);
  max_height = factor * mt->MT_MinZ();
  drawing_active_tri = 1;
  for (i=0;i<num_active;i++)
  {
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, high_color);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glBegin(GL_TRIANGLES);
      setTriangle(active_tri[i]); 
    glEnd();
  }

  drawing_active_tri = 0;
  for (i=0;i<num_other;i++)
  {
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, fore_color);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glBegin(GL_TRIANGLES);
      setTriangle(other_tri[i]); 
    glEnd();
  }
}

/* Solid, smooth shading. */
void drawSmooth(void)
{
  int i;
  MT_INDEX * active_tri;
  MT_INDEX * other_tri;
  MT_INDEX num_active;
  MT_INDEX num_other;
  
  ex->MT_AllExtractedTiles(&active_tri,&num_active,
                           &other_tri,&num_other);
  max_height = factor * mt->MT_MinZ();
  drawing_active_tri = 1;
  for (i=0;i<num_active;i++)
  {
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, high_color);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glBegin(GL_TRIANGLES);
      setSmoothTriangle(active_tri[i]); 
    glEnd();
  }

  /* Non-active tiles are drawn flat shading */
  drawing_active_tri = 0;
  for (i=0;i<num_other;i++)
  {
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, fore_color);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glBegin(GL_TRIANGLES);
      setTriangle(other_tri[i]); 
    glEnd();
  }
}

/* Hidden wireframe (draw edges on filled triangles with a changed z 
   range, fast bu result is not always correct). */
void drawHidden_zbuffer(void)
{
  int i;
  MT_INDEX * active_tri;
  MT_INDEX * other_tri;
  MT_INDEX num_active;
  MT_INDEX num_other;

  ex->MT_AllExtractedTiles(&active_tri,&num_active,
                           &other_tri,&num_other);

  /* draw filled faces in background color with changed depth range */
  glDepthFunc(GL_LEQUAL);
  glDepthRange(ZTWIST,1.0f);
  glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, high_fillcolor);
  glColor3fv(high_fillcolor); /* interior color */
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); 
  max_height = factor * mt->MT_MinZ();
  drawing_active_tri = 1;
  glBegin(GL_TRIANGLES);
  for (i=0;i<num_active;i++)
  {
    setTriangle(active_tri[i]);
  }
  glEnd();
  if (glutGetWindow()==window[0])  /* 2D */
  {
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, fore_fillcolor);
    glColor3fv(fore_fillcolor); /* interior color */
    drawing_active_tri = 0;
    glBegin(GL_TRIANGLES);
    for (i=0;i<num_other;i++)
    {
      setTriangle(other_tri[i]);
    }
    glEnd();
  }

  /* draw edges in foreground color with changed depth range */
  glDepthRange(0.0f,1.0f-ZTWIST); 
  glDepthFunc(GL_LEQUAL);
  glLineWidth(1.0);
  glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, high_color);
  glColor3fv(high_color); /* edge color */
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); 
  drawing_active_tri = 1;
  glBegin(GL_TRIANGLES);
  for (i=0;i<num_active;i++)
  {
      setTriangle(active_tri[i]);
  }
  glEnd();
  if (glutGetWindow()==window[0])  /* 2D */
  {
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, fore_color);
    glColor3fv(fore_color); /* edge color */
    drawing_active_tri = 0;
    glBegin(GL_TRIANGLES);
    for (i=0;i<num_other;i++)
    {
        setTriangle(other_tri[i]);
    }
    glEnd();
  }

  /* restore default state */
  glDepthRange(0.0f,1.0f);
  glDepthFunc(GL_LESS);
}

/* Hidden wireframe (use stencil functions, correct but slow). */
void drawHidden_stencil(void)
{
  int i;
  MT_INDEX * active_tri;
  MT_INDEX * other_tri;
  MT_INDEX num_active;
  MT_INDEX num_other;
  
  ex->MT_AllExtractedTiles(&active_tri,&num_active,
                           &other_tri,&num_other);
  max_height = factor * mt->MT_MinZ();
  drawing_active_tri = 1;
  glLineWidth(1.0);
  for (i=0;i<num_active;i++)
  {
    glStencilFunc(GL_ALWAYS,0,1);
    glStencilOp(GL_INVERT,GL_INVERT,GL_INVERT);
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, high_color);
    glColor3fv(high_color); /* border color */
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glBegin(GL_TRIANGLES);
      setTriangle(active_tri[i]); 
    glEnd();
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, high_fillcolor);
    glColor3fv(high_fillcolor); /* interior color */
    glStencilFunc(GL_EQUAL,0,1);
    glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glBegin(GL_TRIANGLES);
      setTriangle(active_tri[i]);
    glEnd();
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, high_color);
    glColor3fv(high_color); /* border color */
    glStencilFunc(GL_ALWAYS,0,1);
    glStencilOp(GL_INVERT,GL_INVERT,GL_INVERT);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glBegin(GL_TRIANGLES);
      setTriangle(active_tri[i]);
    glEnd();
  }

  if (glutGetWindow()==window[0])  /* 2D */
  {
    drawing_active_tri = 0;
    for (i=0;i<num_other;i++)
    {
      glStencilFunc(GL_ALWAYS,0,1);
      glStencilOp(GL_INVERT,GL_INVERT,GL_INVERT);
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, fore_color);
      glColor3fv(fore_color); /* border color */
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      glBegin(GL_TRIANGLES);
        setTriangle(other_tri[i]); 
      glEnd();
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, fore_fillcolor);
      glColor3fv(fore_fillcolor); /* interior color */
      glStencilFunc(GL_EQUAL,0,1);
      glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      glBegin(GL_TRIANGLES);
        setTriangle(other_tri[i]);
      glEnd();
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, fore_color);
      glColor3fv(fore_color); /* border color */
      glStencilFunc(GL_ALWAYS,0,1);
      glStencilOp(GL_INVERT,GL_INVERT,GL_INVERT);
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      glBegin(GL_TRIANGLES);
        setTriangle(other_tri[i]);
      glEnd();
    }
  }
}

void displayCallBack(void)
{
  switch (style)
  {
    case HIDDEN_WIRE:
      if (glutGetWindow()==window[0]) 
      {  /* simple wireframe, 2D */
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST); 
        glDisable(GL_STENCIL_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        trasfScene();
        drawWire();
        drawFocus();
      }
      else 
      { /* hidden wireframe, 3D */
        glDisable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST); 
        glEnable(GL_STENCIL_TEST);
        glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT
                                      | GL_DEPTH_BUFFER_BIT);
        trasfScene();
        drawHidden();
      }
      break; 
    case SOLID_FACES:
      glEnable(GL_LIGHTING);      
      glDisable(GL_LIGHT1);
      glEnable(GL_DEPTH_TEST); 
      glDisable(GL_STENCIL_TEST);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      setLight();
      trasfScene();
      drawSolid();
      if (glutGetWindow()==window[0]) 
      {
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        drawFocus();
      }
      break;
    case SMOOTH_SHAD:
      glEnable(GL_LIGHTING);
      glEnable(GL_LIGHT1);
      glEnable(GL_DEPTH_TEST);
      glDisable(GL_STENCIL_TEST);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      setLight();
      trasfScene();
      drawSmooth();
      if (glutGetWindow()==window[0]) 
      {
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        drawFocus();
      }
      break;      
  }
  glFlush();
  glutSwapBuffers();
}

void displayString(float x, float y, char * s)
{
  int i;
  glRasterPos2f(x,y);
  for (i=0; s[i]!='\0'; i++)
  {
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, s[i]);
  }
}

void dialogCallBack(void)
{
  char aux[255];
  
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST); 
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0,window_w[2],0,window_h[2]);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glColor3fv(text_color);
  displayString( 0.0,336.0,"*** TERRAIN DEMO ***");
  sprintf(aux,"TRIANGLES: %d, active %d",
              ex->MT_AllExtractedTilesNum(),
              ex->MT_ExtractedTilesNum());
  displayString( 0.0,316.0,aux);
  switch ( getExtractor() )
  {  case DYNAMIC_EXTR: sprintf(aux,"EXTRACTOR: dynamic"); break;
     case LOCAL_EXTR: sprintf(aux,"EXTRACTOR: local"); break;
     case STATIC_EXTR: sprintf(aux,"EXTRACTOR: static"); break;
  }
  displayString( 3.0,299.0,aux);
  displayString(10.0,282.0,"E: change extractor");
  sprintf(aux,"ERROR FACTOR: %f",getErrorBound());
  displayString( 3.0,265.0,aux);
  displayString(10.0,248.0,">: increase");
  displayString(10.0,231.0,"<: decrease");
  displayString( 3.0,214.0,"VIEW FRUSTUM");
  displayString(10.0,197.0,"+/-: enlarge/shrink");
  displayString( 3.0,180.0,"VIEW MODE");
  displayString(10.0,163.0,"S: smooth shading");
  displayString(10.0,146.0,"F: flat shading");
  displayString(10.0,129.0,"W: wire frame");
  displayString( 3.0,112.0,"ZOOM (overview window)");
  displayString(10.0, 95.0,"I/O: in/out");
  switch (direction)
  {  case  0: sprintf(aux,"MOTION: none"); break;
     case  1: sprintf(aux,"MOTION: forward"); break;
     case -1: sprintf(aux,"MOTION: backward"); break;
  }
  displayString( 3.0, 78.0,aux);
  displayString(10.0, 61.0,"right/left arrow: turn");
  displayString(10.0, 44.0,"up arrow: forward");
  displayString(10.0, 27.0,"down arrow: backward");
  displayString(10.0, 10.0,"end: stop");
  glutSwapBuffers();
}

void idleCallBack()
{
  int w;
  advanceGame(direction,turnangle);
  turnangle = 0;
  for (w=0;w<2;w++)
  {
    glutSetWindow(window[w]);
    displayCallBack();
  }
  glutSetWindow(window[2]);
  dialogCallBack();
}

void reshapeCallBack(int w, int h)
{
    int d, i;
    int curr_win = glutGetWindow();
    if (curr_win == window[0])
    {  d = ( (w>h) ? h : w );
       glViewport(0, 0, d, d);
       i = 0;
    }
    else 
    {  if (curr_win == window[1]) i = 1;
       else i = 2;
       glViewport(0, 0, w, h);
    }
    window_w[i] = w;
    window_h[i] = h;
}

void keyboardCallBack(unsigned char c, int x, int y)
{
  switch (c)
  {
    case 'i': case 'I':
      zoom_factor += 0.1;
      break;
    case 'o': case 'O':
      zoom_factor -= 0.1;
      break;
    case '+':
      focus->SetAngle(0,focus->TheAngle(0)+0.05);
      if (!is_idle) idleCallBack();
      break;
    case '-':
      focus->SetAngle(0,focus->TheAngle(0)-0.05);
      if (!is_idle) idleCallBack();
      break;
    case '>':
      changeErrorBound(getErrorBound()+0.001);
      if (!is_idle) idleCallBack();
      break;
    case '<':
      changeErrorBound(getErrorBound()-0.001);
      if (!is_idle) idleCallBack();
      break;
    case 'w': case 'W':
      style = HIDDEN_WIRE;
      break;
    case 'f': case 'F':
      style = SOLID_FACES;
      break;
    case 's': case 'S':
      style = SMOOTH_SHAD;
      break;
    case 'e': case 'E':
      changeExtractor();
      break;
    case 'q': case 'Q':
      endGame();
  }
  redrawWindows();
}

void specialCallBack(int c, int x, int y)
{
  switch (c)
  {
    case 100: /* left arrow */
      turnangle = 1;
      if (!is_idle) idleCallBack();
      break;
    case 101: /* up arrow */
      if (!is_idle) {  glutIdleFunc(idleCallBack); is_idle = 1;  }
      direction = 1;
      break;
    case 102: /* right arrow */
      turnangle = -1;
      if (!is_idle) idleCallBack();
      break;
    case 103: /* down arrow */
      if (!is_idle) {  glutIdleFunc(idleCallBack); is_idle = 1;  }
      direction = -1;
      break;
    case 107: /* end */
      direction = 0;
      glutIdleFunc(NULL); is_idle = 0;
      break;
   }
   redrawWindows();
}

/* ----------------------------------------------------------------------- */

void sceneLimits(void)
{
  float Xrange, Yrange, Zrange;
  Zrange = mt->MT_MaxZ() - mt->MT_MinZ();
  scaleHeights(&factor);
  Zrange *= factor;
  Xrange = mt->MT_MaxX() - mt->MT_MinX();
  Yrange = mt->MT_MaxY() - mt->MT_MinY();
  offset = 0.005; /**** PAOLA 11/9/02  0.0003*Zrange; *****/
  diagonal = sqrt(Zrange*Zrange + Xrange*Xrange + Yrange*Yrange);
}

/* ----------------------------------------------------------------------- */

int main(int argc, char **argv)
{
  int w;
  
  glutInit(&argc, argv);
  startGame(argc, argv);
  sceneLimits();
  
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize((int)window_w[0], (int)window_h[0]);
  window[0] = glutCreateWindow("OVERVIEW");
  glutInitWindowSize((int)window_w[1], (int)window_h[1]);
  window[1] = glutCreateWindow("EYE");
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize((int)window_w[2], (int)window_h[2]);
  window[2] = glutCreateWindow("DIALOG");
  
  for (w=0; w<2; w++)
  {
    glutSetWindow(window[w]);
    glClearColor(back_color[0],back_color[1],back_color[2],1.0);  
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);  
    glutDisplayFunc(displayCallBack);
    glutKeyboardFunc(keyboardCallBack);
    glutSpecialFunc(specialCallBack);
    glutReshapeFunc(reshapeCallBack);
  }
  glutSetWindow(window[2]);
  glClearColor(page_color[0],page_color[1],page_color[2],1.0);
  glutDisplayFunc(dialogCallBack);
  glutKeyboardFunc(keyboardCallBack);
  glutSpecialFunc(specialCallBack);
  glutReshapeFunc(reshapeCallBack);
  
  glutMainLoop();
}

/* ----------------------------------------------------------------------- */
