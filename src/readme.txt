=============================================
Instructions to compile the MT library.
=============================================

(updated on November 2016)


There are two libraries: kernel library and extension library.


To compile the kernel library:

"cd krnl"
"make all"
  or just one of "make libMT.so" or "make libMT.a"
  depending on which type of library you want to make


To compile the extension library:

"cd ext"
"make all"
  or just one of "make libMText.so" or "make libMText.a"
  depending on which type of library you want to make


The library files are placed within directory ../lib


Before compiling, you may have to change something within files 
krnl/makefile and ext/makefile according to the configuration
present on you computer.

