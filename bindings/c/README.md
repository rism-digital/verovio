## C function interface

- To use Verovio with any language that supports a plain C function interface you will first need to build Verovio as a library.

      cd verovio/tools
      cmake -DBUILD_AS_LIBRARY=ON .
      make

- The compiled library (```libverovio.so```)will contain callable C symbols. These wrapper symbols are defined in ```verovio/emscripten/c_wrapper.cpp```
- You will need to provide your own header for C or any other language, for each function from ```c_wrapper.cpp``` that you wish to call.

## Complete example in C
```c
//c_example.c
#include "stdio.h"
#include "stdbool.h"

//define Toolkit as void since we only deal with pointer
typedef void Toolkit;

//function declarations for any functions we need from c_wrapper.cpp
void *vrvToolkit_constructor();
void *vrvToolkit_constructorResourcePath(const char * resourcePath);
void vrvToolkit_destructor(Toolkit *tk);
bool vrvToolkit_edit(Toolkit *tk, const char *editorAction);
const char *vrvToolkit_getAvailableOptions(Toolkit *tk);
const char *vrvToolkit_getElementAttr(Toolkit *tk, const char *xmlId);
const char *vrvToolkit_getElementsAtTime(Toolkit *tk, int millisec);
const char *vrvToolkit_getHumdrum(Toolkit *tk);
const char *vrvToolkit_getLog(Toolkit *tk);
const char *vrvToolkit_getMEI(Toolkit *tk, int page_no, bool score_based);
const char *vrvToolkit_getMIDIValuesForElement(Toolkit *tk, const char *xmlId);
const char *vrvToolkit_getOptions(Toolkit *tk, bool default_values);
int vrvToolkit_getPageCount(Toolkit *tk);
int vrvToolkit_getPageWithElement(Toolkit *tk, const char *xmlId);
double vrvToolkit_getTimeForElement(Toolkit *tk, const char *xmlId);
const char *vrvToolkit_getVersion(Toolkit *tk);
bool vrvToolkit_loadData(Toolkit *tk, const char *data);
const char *vrvToolkit_renderToMIDI(Toolkit *tk, const char *c_options);
const char *vrvToolkit_renderToSVG(Toolkit *tk, int page_no, const char *c_options);
const char *vrvToolkit_renderToTimemap(Toolkit *tk);
void vrvToolkit_redoLayout(Toolkit *tk);
void vrvToolkit_redoPagePitchPosLayout(Toolkit *tk);
const char *vrvToolkit_renderData(Toolkit *tk, const char *data, const char *options);
void vrvToolkit_setOptions(Toolkit *tk, const char *options);

int main()
{
  printf("Calling constructor\n");
  void* pointer = NULL;
  pointer = vrvToolkit_constructorResourcePath("/path/to/verovio/data");
  printf("Pointer value %p\n",pointer);
  const char * options = NULL;
  options = vrvToolkit_getAvailableOptions(pointer);
  printf("%s", options);

}

```

- To run this example place the compiled library (```libverovio.so```) in the same directory. Then you can use gcc to compile the example and link to the pre-built library.

      gcc c_example.c -o c_example -L. -lverovio

- Run (without changing your default LD LIBRARY PATH):

      LD_LIBRARY_PATH=. ./c_example 
