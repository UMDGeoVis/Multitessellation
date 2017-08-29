--------------
   MT DEMOS
--------------

demo1: Box demo
 Magic lens on a 3D object.
 It needs an MT with pre-computed triangle errors and vertex normals.

demo2: Terrain demo 
 Flying over a terrain.
 It needs an MT with pre-computed triangle errors.

demo3: Box demo 
 Magic lens on a 3D object.
 Similar to demo1, but simply the MT is required (no other information).

demo4: Uniform mesh demo (command-line version)
 Extraction of a mesh at uniform resolution for a 3D object.
 It works off-line. Output mesh is written on a file.

They are all compiled by:

- cd demoN
- make demoN (version with dynamic MT libraries)
- make demoNstatic (version with static MT libraries)

Instructions to run and use each demo are contained in the specific 
sub-directories.
