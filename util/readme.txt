UTILITY PROGRAMS FOR MT CONSTRUCTION
====================================

This directory contains a collection of utility programs related
to the construction of a Multi-Tesselation.


PROGRAM "reverse": REVERSE THE ORIENTATION OF A SURFACE MT
----------------------------------------------------------

Input: a two-dimemsional MT embedded in three dimension (triangles in 3D).
Output: a two-dimemsional MT embedded in three dimension (triangles in 3D).

Syntax: reverse input_mt output_mt

The output MT is equal to the input MT, but all its triangles are reversed:
if the three vertices of a triangle T are sorted as A,B,C in the inout MT, 
then the vertices of T in the output MT are sorted as C,B,A.

This operation correspond to exchange the inner with the outer side, and
conversely, in the surface represented by the MT .

PROGRAM "addnorma": ADD VERTEX NORMALS TO A SURFACE MT
------------------------------------------------------

Input: a two-dimemsional MT embedded in three dimensions (triangles in 3D).
Output: a float vector attribute table (with vector lenght = 3) containing
        vertex normals for the given MT.

Syntax: addnorma mt_file normal_file

Important: it is assumed that the tesselation at maximum resolution
available in the MT contains all the vertices of the MT. This is true for
Multi-Triangulation built through incremental insertion/deletion of
vertices, and for those built through edge collapse provided that the vertex
resulting from collapsing an edge coincides with one of the two edge
endpoints. 

Vertex normals are computed on the tesselation at maximum resolution
extracted from the MT.

PROGRAMS "mtbinary" and "mtascii": CONVERT AN MT INTO BINARY/ASCII FORMAT
-------------------------------------------------------------------------

Input: an MT.
Output: the same MT written in binary/ascii format.

Syntax: mtbinary input_mt output_mt
        mtascii input_mt output_mt

The source file for both programs is binascii.c.

PROGRAM "stats": STATISTICAL INFORMATION ABOUT AN MT
----------------------------------------------------

Input: an MT.
Output: statistical information, written to standard output.

Syntax: stats mt_file

Print statistical information:
total number of nodes, arcs, tiles and vertices in the MT;
number of tiles and vertices in the tesselations at minimum and 
maximum resolution;
ratio between the total number of MT tiles and the number of 
tiles at maximum resolution;
number of old and new triangles of a node of the MT;
number of incoming and outgoing arcs of a node of the MT;
maximum lenght of a path in the MT.
