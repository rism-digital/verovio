## C function interface

To use Verovio with any language that supports a plain C function interface you will first need to build Verovio as a library.
The compiled library (`libverovio.so`/`verovio.dll`) will contain callable C symbols. These wrapper symbols are defined in `./tools/c_wrapper.h`

### Building libverovio.so on Linux

```sh
cd tools
cmake -DBUILD_AS_LIBRARY=ON .
make
```

### Building verovio.dll on Windows using Microsoft Visual Studio Build Tools 2022

In addition to Microsoft Visual Studio Build Tools 2022, also [Make](https://gnuwin32.sourceforge.net/packages/make.htm) is used.

Open `x64 Native Tools Command Prompt for VS 2022` and enter:

```
cd cmake
cmake -G "Unix Makefiles" -DBUILD_AS_LIBRARY=ON -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=TRUE .
make
```


## Complete example in C

To run the `main.c` example you can use gcc to compile the example and link to the pre-built library.

```sh
gcc main.c -o main -L../../tools -lverovio
```

Run (without changing your default LD LIBRARY PATH):

```sh
LD_LIBRARY_PATH=../../tools ./main
```
