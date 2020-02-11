
#include "stdio.h"
#include "stdbool.h"

// define Toolkit as void since we only deal with pointer
typedef void Toolkit;

//include the header with the C functions
#include "../../tools/c_wrapper.h"

int main()
{
    printf("Calling constructor\n");
    void* pointer = vrvToolkit_constructorResourcePath("../../data");
    printf("Pointer value %p\n",pointer);
    const char * options = vrvToolkit_getAvailableOptions(pointer);
    printf("%s", options);
}
