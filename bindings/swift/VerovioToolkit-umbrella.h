#ifdef __OBJC__
  #if TARGET_OS_IOS || TARGET_OS_TV || TARGET_OS_VISION
    #import <UIKit/UIKit.h>
  #elif TARGET_OS_OSX
    #import <AppKit/AppKit.h>
  #endif
#endif

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