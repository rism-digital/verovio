#ifndef FOUNDATION_EXPORT
  #if defined(__cplusplus)
    #define FOUNDATION_EXPORT extern "C"
  #else
    #define FOUNDATION_EXPORT extern
  #endif
#endif

#import "veroviotoolkit/c_wrapper.h"

FOUNDATION_EXPORT double VerovioToolkitVersionNumber;
FOUNDATION_EXPORT const unsigned char VerovioToolkitVersionString[];

