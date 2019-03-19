## C function interface

- To use Verovio with any language that supports a plain C function interface you will first need to build Verovio as a library

      cd tools
      cmake -DBUILD_AS_LIBRARY=ON .
      make

- The compiled library (`libverovio.so`) will contain callable C symbols. These wrapper symbols are defined in `./tools/c_wrapper.h`

## Complete example in C

- To run the `main.c` example you can use gcc to compile the example and link to the pre-built library.

      gcc main.c -o main -L../../tools -lverovio

- Run (without changing your default LD LIBRARY PATH):

      LD_LIBRARY_PATH=../../tools ./main 
