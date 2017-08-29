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

/* ------------------------------ fcssegm.c ------------------------------- */

#include "geo.h"
#include "fcssegm.h"

/* ------------------------------------------------------------------------ */
/*                          CLASS FOR A SEGMENT                             */
/* ------------------------------------------------------------------------ */

void SegmentClass :: SetSegment(float * coord1, float * coord2)
{  int i;
   for (i=0; i<SegmentDim(); i++) 
   {  endp1[i] = coord1[i];  endp2[i] = coord2[i];  }
}

void SegmentClass :: TheSegment(float * coord1, float * coord2)
{  int i;
   for (i=0; i<SegmentDim(); i++)
   {   coord1[i] = endp1[i];  coord2[i] = endp2[i];  }
}
   
void SegmentClass :: SetSegment(float x1, float y1, float x2, float y2)
{
   endp1[0] = x1; endp1[1] = y1;
   endp2[0] = x2; endp2[1] = y2;
}
   
void SegmentClass :: 
     TheSegment(float * x1, float * y1, float * x2, float * y2)
{
   (*x1) = endp1[0]; (*y1) = endp1[1];
   (*x2) = endp2[0]; (*y2) = endp2[1];
}

void SegmentClass :: SetSegment(float x1, float y1, float z1,
                                float x2, float y2, float z2)
{
   endp1[0] = x1; endp1[1] = y1; endp1[2] = z1;  
   endp2[0] = x2; endp2[1] = y2; endp2[2] = z2;
}

void SegmentClass :: TheSegment(float * x1, float * y1, float * z1,
                                float * x2, float * y2, float * z2)
{
   (*x1) = endp1[0]; (*y1) = endp1[1]; (*z1) = endp1[2];  
   (*x2) = endp2[0]; (*y2) = endp2[1]; (*z2) = endp2[2];  
}
      
SegmentClass :: SegmentClass(int d)
{
   int i;
   s_dim = d;
   for (i=0; i<d; i++)  {  endp1[i] = endp2[i] = 0.0;  }
}

/* ------------------------------------------------------------------------ */
/*                   FOCUS CONSISTING OF A SEGMENT IN 2D                    */
/* ------------------------------------------------------------------------ */

Segment2FocusOnTrianglesClass  :: 
Segment2FocusOnTrianglesClass(float x1, float y1, float x2, float y2)
: SegmentClass(2)
{  SetSegment(x1,y1, x2,y2);  }
                                       
int Segment2FocusOnTrianglesClass  ::
    MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag)
{
  MT_INDEX * v; 
  int i;     

  /* we assume that the tile is a triangle, thus three vertices */
  m->MT_TileVertices(t,&v);

  if ( !PointOutTriangle2D(endp1[0], endp1[1], 
                        m->MT_VertexX(v[0]),m->MT_VertexY(v[0]),
                        m->MT_VertexX(v[1]),m->MT_VertexY(v[1]),
                        m->MT_VertexX(v[2]),m->MT_VertexY(v[2])) )
  {  /* first endpoint inside triangle */
     return 1;
  }

  if ( !PointOutTriangle2D(endp2[0], endp2[1], 
                        m->MT_VertexX(v[0]),m->MT_VertexY(v[0]),
                        m->MT_VertexX(v[1]),m->MT_VertexY(v[1]),
                        m->MT_VertexX(v[2]),m->MT_VertexY(v[2])) )
  {  /* second endpoint inside triangle*/
     return 1;
  }
  
  for (i=0;i<3;i++)
  {
    if (SegmentsIntersect2D(endp1[0], endp1[1], endp2[0], endp2[1], 
                        m->MT_VertexX(v[i]),m->MT_VertexY(v[i]),
                        m->MT_VertexX(v[(i+1)%3]),m->MT_VertexY(v[(i+1)%3])) )
    {  /* segment intersects i-th triangle edge */
       return 1;
    }
    if (SegmentsOverlap2D(endp1[0], endp1[1], endp2[0], endp2[1], 
                        m->MT_VertexX(v[i]),m->MT_VertexY(v[i]),
                        m->MT_VertexX(v[(i+1)%3]),m->MT_VertexY(v[(i+1)%3])) )
    {
       /* segment overlaps i-th triangle edge */
       return 1;
    }
  }
  /* segment does not intersect triangle */
  return 0;  
}

int Segment2FocusOnTrianglesClass  :: MT_IsGood(MT_MultiTesselation m)
{
  if (m->MT_TileDim() != 2)
  {
     MT_Warning((char *)"Need two-dimensional tiles",
                (char *)"Segment2FocusOnTrianglesClass::MT_IsGood");
     return 0;
  }
  return 1;
}

/* ------------------------------------------------------------------------ */
/*       FOCUS CONSISTING OF THE LOCUS OF POINTS ABOVE A SEGMENT IN 3D      */
/* ------------------------------------------------------------------------ */

Segment3FocusOnTriFieldClass ::
Segment3FocusOnTriFieldClass(WithFieldClass * m1, 
                             WithTileErrorClass * m2,
                             float x1, float y1, float z1,
                             float x2, float y2, float z2)
: SegmentClass(3)
{
   SetSegment(x1,y1,z1, x2,y2,z2);  
   my_mt1 = m1;  my_mt2 = m2;
}

/*
Return 1 iff the triangle tile lies at least partially above the segment.
In the loose version, consider the triangle as lowered of an amount
equal to its error.
*/

/*
Since many checks need to be made about the position of the two segment 
endpoints w.r.t. the triangle vertices,
many turns would be computed repeatedly in several places of the code.
Thus, we compute the turns once for all and record the results in some
flags.
The following macros define the meaning of each bit in the flags.
*/

#define L_BIT1 0x1   /* bit raised if endp1 turns left w.r.t. edge */
#define N_BIT1 0x2   /* bit raised if endp1 aligned w.r.t. edge */
#define R_BIT1 0x4   /* bit raised if endp1 turns right w.r.t. edge */
#define L_BIT2 0x10  /* bit raised if endp2 turns left w.r.t. edge */
#define N_BIT2 0x20  /* bit raised if endp2 aligned w.r.t. edge */
#define R_BIT2 0x40  /* bit raised if endp2 turns right w.r.t. edge */
#define LS_BIT 0x100 /* bit raised if vertex turns left w.r.t. segment */
#define NS_BIT 0x200 /* bit raised if vertex aligned w.r.t. segment */
#define RS_BIT 0x400 /* bit raised if vertex turns right w.r.t. segment */

int Segment3FocusOnTriFieldClass ::
    MT_EvalCond(MT_MultiTesselation m, MT_INDEX t, int flag)
{
  MT_INDEX * v; /* triangle vertices */
  int i;       
  /* flags encoding the turns of the two segment endpoints w.r.t. each
     triangle edge, and the turns of each triangle vertex w.r.t. the 
     segment.
     turn_flag[i] encodes the turns of endp1 and endp2 w.r.t. edge 
     v[i],v[(i+1)%3], and the turns of v[i] w.r.t. segment endp1,endp2 */
  int turn_flag[3];
  /* auxiliary variables explained later */
  float * thirdv, * testp;

  for (i=0;i<3;i++) turn_flag[i] = 0x0;

  m->MT_TileVertices(t,&v);

/*printf("Seg2Half (%.0f %.0f %.1f, %.0f %.0f %.1f, %.0f %.0f %.1f)\n",
 m->MT_VertexX(v[0]), m->MT_VertexY(v[0]), my_mt1->VertexField(v[0]), 
 m->MT_VertexX(v[1]), m->MT_VertexY(v[1]), my_mt1->VertexField(v[1]), 
 m->MT_VertexX(v[2]), m->MT_VertexY(v[2]), my_mt1->VertexField(v[2]));*/

  for (i=0;i<3;i++)
  {
    
    if ( PointsCoincide2D(endp1[0], endp1[1],
                          m->MT_VertexX(v[i]), m->MT_VertexY(v[i])) )
    {  /* endp1 and v[i] coincide in 2D, check field values */
       if ( my_mt1->VertexField(v[i]) > 
            ( (flag==MT_STRICT) ? endp1[2] : 
                                  endp1[2]-(my_mt2->TileError(t)) ) )
       return 1;
    }

    if ( PointsCoincide2D(endp2[0], endp2[1],
                          m->MT_VertexX(v[i]), m->MT_VertexY(v[i])) )
    {  /* endp2 and v[i] coincide in 2D, check field values */
       if ( my_mt1->VertexField(v[i]) > 
            ( (flag==MT_STRICT) ? endp2[2] :
                                  endp2[2]-(my_mt2->TileError(t)) ) )
       return 1;
    }

    switch ( PointTurn2D(endp1[0], endp1[1], 
                       m->MT_VertexX(v[i]),m->MT_VertexY(v[i]),
                       m->MT_VertexX(v[(i+1)%3]),m->MT_VertexY(v[(i+1)%3])) )
    {
       case LEFT_TURN:  turn_flag[i] |= L_BIT1;  break;
       case NO_TURN:    turn_flag[i] |= N_BIT1;  break;
       case RIGHT_TURN: turn_flag[i] |= R_BIT1;  break;
    }

    switch ( PointTurn2D(endp2[0], endp2[1], 
                       m->MT_VertexX(v[i]),m->MT_VertexY(v[i]),
                       m->MT_VertexX(v[(i+1)%3]),m->MT_VertexY(v[(i+1)%3])) )
    {
       case LEFT_TURN:  turn_flag[i] |= L_BIT2;  break;
       case NO_TURN:    turn_flag[i] |= N_BIT2;  break;
       case RIGHT_TURN: turn_flag[i] |= R_BIT2;  break;
    }

    if ( (turn_flag[i]&R_BIT1) && (turn_flag[i]&R_BIT2) )
    {  /* both segment endpoints are on the right of the same triangle edge,
          the segment does not intersect the triangle */
       return 0;
    }
  }

  for (i=0;i<3;i++)
  {
    switch ( PointTurn2D(m->MT_VertexX(v[i]),m->MT_VertexY(v[i]),
                         endp1[0], endp1[1], endp2[0], endp2[1]) )
    {
       case LEFT_TURN:  turn_flag[i] |= LS_BIT;  break;
       case NO_TURN:    turn_flag[i] |= NS_BIT;  break;
       case RIGHT_TURN: turn_flag[i] |= RS_BIT;  break;
    }
  }

//printf(   "Flags(2) = %x %x %x\n",turn_flag[0], turn_flag[1], turn_flag[2]);

  if ( (turn_flag[0]&turn_flag[1]&turn_flag[2]&LS_BIT) ||
       (turn_flag[0]&turn_flag[1]&turn_flag[2]&RS_BIT) )
  {  /* the three triangle vertices are all on the left or all
        on the right of the segment */
     return 0;
  }

//printf(   "Flags(1) = %x %x %x\n",turn_flag[0], turn_flag[1], turn_flag[2]);

  for (i=0;i<3;i++)
  {

    if ( (turn_flag[i]&N_BIT1) && (turn_flag[(i+1)%3]&L_BIT1) &&
         (turn_flag[(i+2)%3]&L_BIT1) )
    {  /* endp1 lies on the i-th triangle edge, check field values */
       /* consider turn of  v[i],v[(i+1)%3],endp1  on the x-field plane,
          if v[i],v[(i+1)%3] do not have the same x, otherwise consider
          their turn on the y-field plane */
       if ( m->MT_VertexX(v[i]) < m->MT_VertexX(v[(i+1)%3]) )
       {  if ( PointTurn2D(endp1[0], endp1[2],
                        m->MT_VertexX(v[i]),
                        my_mt1->VertexField(v[i]),
                        m->MT_VertexX(v[(i+1)%3]),
                        my_mt1->VertexField(v[(i+1)%3]))
               == DOWN_TURN )
          return 1;
       }  
       else if ( m->MT_VertexX(v[i]) > m->MT_VertexX(v[(i+1)%3]) )
            {  if ( PointTurn2D(endp1[0], endp1[2],
                        m->MT_VertexX(v[(i+1)%3]),
                        my_mt1->VertexField(v[(i+1)%3]),
                        m->MT_VertexX(v[i]),
                        my_mt1->VertexField(v[i]))
                    == DOWN_TURN )
               return 1;
            }
            else if ( m->MT_VertexY(v[i]) < m->MT_VertexY(v[(i+1)%3]) )
                 {  if ( PointTurn2D(endp1[1], endp1[2],
                            m->MT_VertexY(v[i]),
                            my_mt1->VertexField(v[i]),
                            m->MT_VertexY(v[(i+1)%3]),
                            my_mt1->VertexField(v[(i+1)%3]))
                        == DOWN_TURN )
                    return 1;
                 }  
                 else if ( m->MT_VertexY(v[i]) > m->MT_VertexY(v[(i+1)%3]) )
                      {  if ( PointTurn2D(endp1[1], endp1[2],
                            m->MT_VertexY(v[(i+1)%3]),
                            my_mt1->VertexField(v[(i+1)%3]),
                            m->MT_VertexY(v[i]),
                            my_mt1->VertexField(v[i]))
                            == DOWN_TURN )
                         return 1;
                      }
    }

    if ( (turn_flag[i]&N_BIT2) && (turn_flag[(i+1)%3]&L_BIT2) &&
         (turn_flag[(i+2)%3]&L_BIT2) )
    {  /* endp2 lies on the i-th triangle edge, check field values */
       /* consider turn of  v[i],v[(i+1)%3],endp2  on the x-field plane,
          if v[i],v[(i+1)%3] do not have the same x, otherwise consider
          their turn on the y-field plane */
       if ( m->MT_VertexX(v[i]) < m->MT_VertexX(v[(i+1)%3]) )
       {  if ( PointTurn2D(endp2[0], endp2[2],
                        m->MT_VertexX(v[i]),
                        my_mt1->VertexField(v[i]),
                        m->MT_VertexX(v[(i+1)%3]),
                        my_mt1->VertexField(v[(i+1)%3]))
               == DOWN_TURN )
          return 1;
       }  
       else if ( m->MT_VertexX(v[i]) > m->MT_VertexX(v[(i+1)%3]) )
            {  if ( PointTurn2D(endp2[0], endp2[2],
                        m->MT_VertexX(v[(i+1)%3]),
                        my_mt1->VertexField(v[(i+1)%3]),
                        m->MT_VertexX(v[i]),
                        my_mt1->VertexField(v[i]))
                    == DOWN_TURN )
               return 1;
            }
            else if ( m->MT_VertexY(v[i]) < m->MT_VertexY(v[(i+1)%3]) )
                 {  if ( PointTurn2D(endp2[1], endp2[2],
                            m->MT_VertexY(v[i]),
                            my_mt1->VertexField(v[i]),
                            m->MT_VertexY(v[(i+1)%3]),
                            my_mt1->VertexField(v[(i+1)%3]))
                        == DOWN_TURN )
                    return 1;
                 }  
                 else if ( m->MT_VertexY(v[i]) > m->MT_VertexY(v[(i+1)%3]) )
                      {  if ( PointTurn2D(endp2[1], endp2[2],
                            m->MT_VertexY(v[(i+1)%3]),
                            my_mt1->VertexField(v[(i+1)%3]),
                            m->MT_VertexY(v[i]),
                            my_mt1->VertexField(v[i]))
                            == DOWN_TURN )
                         return 1;
                      }
    }

    if ( (turn_flag[i]&N_BIT1) && (turn_flag[i]&N_BIT2) &&
         ( ( (turn_flag[(i+2)%3]&L_BIT1) && (turn_flag[(i+2)%3]&R_BIT2) ) ||
           ( (turn_flag[(i+2)%3]&R_BIT1) && (turn_flag[(i+2)%3]&L_BIT2) ) ) )
    {  /* v[i] is in the interior of the segment, check field values */
       /* consider turn of  v[i],endp1,endp2  on the x-field plane,
          if endp1,endp2 do not have the same x, otherwise consider
          their turn on the y-field plane */
       if ( endp1[0] < endp2[0] )
       {  if ( PointTurn2D(m->MT_VertexX(v[i]),my_mt1->VertexField(v[i]),
                           endp1[0], endp1[2], endp2[0], endp2[2])
               != DOWN_TURN )
          return 1;
       }  
       else if ( endp1[0] > endp2[0] )
            {  if ( PointTurn2D(m->MT_VertexX(v[i]),my_mt1->VertexField(v[i]),
                                endp2[0], endp2[2], endp1[0], endp1[2]) 
                    != DOWN_TURN )
               return 1;
            }
            else if ( endp1[1] < endp2[1] )
                 {  if ( PointTurn2D(m->MT_VertexY(v[i]),
                                     my_mt1->VertexField(v[i]),
                                     endp1[1], endp1[2], endp2[1], endp2[2])
                        != DOWN_TURN )
                    return 1;
                 }  
                 else if ( endp1[1] > endp2[1] )
                      {  if ( PointTurn2D(m->MT_VertexY(v[i]),
                                          my_mt1->VertexField(v[i]),
                                          endp2[1], endp2[2], 
                                          endp1[1], endp1[2])
                            != DOWN_TURN )
                         return 1;
                      }
    }

  }

  if ( (turn_flag[0]&L_BIT1) && (turn_flag[1]&L_BIT1) && (turn_flag[2]&L_BIT1) )
  {  /* first segment endpoint is inside triangle, or on its boundary,
        check field values */
     if ( FourPointTurn(endp1[0], endp1[1], 
               ((flag == MT_STRICT) ? endp1[2] : 
                                      endp1[2] - my_mt2->TileError(t)),
               m->MT_VertexX(v[0]),m->MT_VertexY(v[0]),my_mt1->VertexField(v[0]),
               m->MT_VertexX(v[1]),m->MT_VertexY(v[1]),my_mt1->VertexField(v[1]),
               m->MT_VertexX(v[2]),m->MT_VertexY(v[2]),my_mt1->VertexField(v[2]) )
               == DOWN_TURN )
     return 1;
  }

  if ( (turn_flag[0]&L_BIT2) && (turn_flag[1]&L_BIT2) && (turn_flag[2]&L_BIT2) )
  {  /* second segment endpoint is inside triangle, or on its boundary,
        check field values */
     if ( FourPointTurn(endp2[0], endp2[1], 
               ((flag == MT_STRICT) ? endp2[2] : 
                                      endp2[2] - my_mt2->TileError(t)),
               m->MT_VertexX(v[0]),m->MT_VertexY(v[0]),my_mt1->VertexField(v[0]),
               m->MT_VertexX(v[1]),m->MT_VertexY(v[1]),my_mt1->VertexField(v[1]),
               m->MT_VertexX(v[2]),m->MT_VertexY(v[2]),my_mt1->VertexField(v[2]) )
               == DOWN_TURN )
     return 1;
  }


  for (i=0;i<3;i++)
  {  
     if ( ( (turn_flag[i]&L_BIT1) && (turn_flag[i]&R_BIT2) ) ||
          ( (turn_flag[i]&L_BIT2) && (turn_flag[i]&R_BIT1) ) ||
          ( (turn_flag[i]&R_BIT1) && (turn_flag[i]&L_BIT2) ) ||
          ( (turn_flag[i]&R_BIT2) && (turn_flag[i]&L_BIT1) ) )
     {  /* segment intersects the supporting line of i-th edge in 2D */
        if ( ( (turn_flag[i]&LS_BIT) && (turn_flag[(i+1)%3]&RS_BIT) ) ||
             ( (turn_flag[i]&RS_BIT) && (turn_flag[(i+1)%3]&LS_BIT) ) ||
             ( (turn_flag[(i+1)%3]&LS_BIT) && (turn_flag[i]&RS_BIT) ) ||
             ( (turn_flag[(i+1)%3]&RS_BIT) && (turn_flag[i]&LS_BIT) ) )
        {  /* i-th edge intersects the supporting line of segment in 2D */
           /* segment and i-the edge cross in 2D, check field values */
           /* Segment endp1-endp2 is above i-the edge (=v1-v2)
              if endp1 is above triangle v1-v2-endp2, assuming that
              v1-v2-endp2 turn counterclockwise in 2D.
              We know that at least one segment endpoints is on the left
              of the edge in 2D, we take that endpoint as the third vertex
              of the triangle. */
           /* thirdv = segment endpoint used as third triangle vertex
              testp = segment endpoint used as test point */
           if (turn_flag[i]&L_BIT1)
           {  /* endp1 is on the left of i-th edge */
              thirdv = endp1; testp = endp2;
           }
           else
           {  /* endp2 is on the left of i-th edge */
              thirdv = endp2; testp = endp1;
           }
           if ( FourPointTurn(testp[0],testp[1],
               ((flag == MT_STRICT) ? testp[2] : 
                                      testp[2] - my_mt2->TileError(t)),
               m->MT_VertexX(v[i]),m->MT_VertexY(v[i]),my_mt1->VertexField(v[i]),
               m->MT_VertexX(v[(i+1)%3]),m->MT_VertexY(v[(i+1)%3]),
               my_mt1->VertexField(v[(i+1)%3]),
               thirdv[0],thirdv[1],
               ((flag == MT_STRICT) ? thirdv[2] : 
                                      thirdv[2] - my_mt2->TileError(t)) )
               != UP_TURN )
           return 1;
        }
     }

  }
  return 0;  
}

int Segment3FocusOnTriFieldClass :: MT_IsGood(MT_MultiTesselation m)
{
  if ( (m->MT_TileDim() != 2) || (m->MT_VertexDim() < 3) )
  {
     MT_Warning((char *)"Need two-dimensional tiles in at least three dimensions",
                (char *)"Segment3FocusOnTrianglesClass::MT_IsGood");
     return 0;
  }
  return 1;
}

/* ------------------------------------------------------------------------ */
