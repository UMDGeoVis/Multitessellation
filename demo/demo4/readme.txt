---------------------------------------------------------
     Uniform mesh demo - command-line version (demo4)
---------------------------------------------------------

This extracts one mesh at a uniform resolution and writes it on a file. 
There is no graphical user interface.

This demo requires a two-dimensional MT embedded in three dimensions.

Let the MT be contained in a file named xxx.mtf.
To run the demo, type:
  demo4 xxx yy zzz
where
- yy is the value for area threshold (see below)
- zzz is the output file name

For instance, try:
  demo4 ../data_demo1/bunny 0.5 mesh.off

This demo uses triangle areas as a measure for triangle accuracy.

The program extracts a triangle mesh whose triangles have areas within 
a given bound inside a cubic box (the "magic lens"), and an arbitrarily 
large area outside it.
The extraction parameters are:
- a focus condition s.t. active triangles are the ones intersecting a
  cubic box;
- a resolution filter condition s.t. a triangle is feasible if its
  area is below a given threshold.

The area threshold (parameter yy on command-line) is a number 
between 0.0 (min accuracy) and 10.0 (max accuracy).
