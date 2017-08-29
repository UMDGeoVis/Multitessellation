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

/* ---------------------------- mt_glob.c --------------------------------- */

/* ------------------------------------------------------------------------ */
/*                  GENERAL CONVENTIONS FOR THE MT PACKAGE                  */
/* ------------------------------------------------------------------------ */

#ifdef MT_MEMDEBUG
#include <malloc.h>
#endif /* MT_MEMDEBUG */

#include "mt_glob.h"

/* ------------------------------------------------------------------------ */
/*                             ERROR UTILITIES                              */
/* ------------------------------------------------------------------------ */

/* PAOLA 2016: refefined with string instead if char * to avoid warning */

void MT_Warning(char * msg, char * fun)
{  fprintf(stderr,"Warning [%s]: %s\n", fun, msg);  }

void MT_Error(char * msg, char * fun)         
{  fprintf(stderr,"Error [%s]: %s\n", fun, msg);  }

void MT_FatalError(char * msg, char * fun)
{  fprintf(stderr,"Fatal error [%s]: %s\n", fun, msg);  exit(1);  }

void MT_WarningC(char * msg, char * cls, char * fun)
{  fprintf(stderr,"Warning [%s::%s]: %s\n", cls, fun, msg);  }

void MT_ErrorC(char * msg, char * cls, char * fun)
{  fprintf(stderr,"Error [%s::%s]: %s\n", cls, fun, msg);  }

void MT_FatalErrorC(char * msg, char * cls, char * fun)
{  fprintf(stderr,"Fatal error [%s::%s]: %s\n", cls, fun, msg);  exit(1);  }

/* ------------------------------------------------------------------------ */
/*                           DEBUGGING UTILITIES                            */
/* ------------------------------------------------------------------------ */

#ifdef MT_MEMDEBUG

void MT_CheckMemory(char * msg)
{
  if (MT_DEBUG_THIS_FILE)
  {  struct mallinfo mm;
     mm = mallinfo();
     printf("%% MEMORY CHECK [%s]%%\n",msg);
     printf("%% Total allocated blocks=%d\n",mm.uordblks);
  }
}

#endif /* MT_MEMDEBUG */

#ifdef MT_DEBUG

void MT_Message(char * msg, char * fun)
{  if (MT_DEBUG_THIS_FILE)
      fprintf(stderr,"Debug [%s]: %s\n", fun, msg);
}

void MT_MessageC(char * msg, char * cls, char * fun)
{  if (MT_DEBUG_THIS_FILE) 
      fprintf(stderr,"Debug [%s::%s]: %s\n", cls, fun, msg);
}

#endif /* MT_DEBUG */

/* ------------------------------------------------------------------------ */
