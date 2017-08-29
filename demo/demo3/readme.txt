-------------------------------
     MT Box demo (demo3)
-------------------------------

This demo uses a magic lens on a 3D object.

This demo requires a two-dimensional MT embedded in three dimensions.

Let the MT be contained in a file named xxx.mtf.
To run the demo, type:
  demo3 xxx 

For instance, try:
  demo3 ../data_demo1/bunny

To exit the demo, strike key "Q" or "q" when the focus is in one of the 
demo windows.

The program works similarly to demo1, with some differences.
It uses triangle areas as a measure for triangle accuracy (demo1 uses
pre-computed errors). And it does not support smooth shading in 
rendering the mesh (demo1 does it by exploiting pre-computed normals).

The program extracts a triangle mesh whose triangles have areas within 
a given bound inside a cubic box (the "magic lens"), and an arbitrarily 
large area outside it.
The extraction parameters are:
- a focus condition s.t. active triangles are the ones intersecting a
  cubic box;
- a resolution filter condition s.t. a triangle is feasible if its
  area is below a given threshold.

The user can move the box interactively, and can change the extraction
parameters. 

One window is used for visualization:
- the whole surface is shown at the center of the window in an orthogonal
  projection
- the box is shown in red, the active triangles of the object are 
  drawn in jellow, the other triangles in white
- the three axes of the reference system are shown (the x,y,z axis are
  drawn in red, green, blue respectively)
The viewed scene can be rotated and zoomed as explained next.

Another window shows the available commmands for interacting with the
program.

User interaction is done by pressing the following keys when the 
mouse focus is in one of the two windows of the program.

Box movement
*  r/R: start moving the box in the negative and in the positive direction 
   of the x axis (the red axis)
*  g/G: start moving the box in the negative and in the positive direction 
   of the y axis (the green axis)
*  b/B: start moving the box in the negative and in the positive direction 
   of the z axis (the blue axis)
*  end: stop moving the box
*  home: bring the box to its initial position and size
*  pageup / pagedown: increase/decrease the speed of the box

Triangle mesh extraction
*  e / E: switch extractor type (dynamic / local)
*  > / <: increase / decrease area threshold used inside the box
*  + / -: enlarge / shrink the box

Drawing
*  w / W: switch to wireframe style (hidden line)
*  f / F: switch to solid style, flat shading
*  i / o: zoom in / out

Rotations and translations
*  F1: switch between rotation and translation mode
*  left arrow / right arrow:
   if rotation mode, rotate around the z axis of the scene (the green axis);
   if translation mode, move the scene horizontally
*  up arrow / down arrow: 
   if rotation mode, rotate around an axis parallel to the horizontal edge of
   the window; if translation mode, move the scene vertically

Termination
*  q / Q: quit

Visualization in hidden wireframe may sometimes produce artifacts
(dashed and dotted lines, or portions of lines not properly hidden).


