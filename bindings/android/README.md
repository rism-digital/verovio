## About

This allows verovio to be built as a native library for Android. It does not include any Java bindings.

DISCLAIMER: It has been reported on various occasions that this is not working properly. There seem to be other way to have it working, and contributions would be welcome.

## Build instructions

As a prerequisite you have to install the Android NDK. 

```sh
export ANDROID_NDK_ROOT="/opt/Android/android-ndk-r21b" # adjust to NDK install ANDROID_NDK
ndk_build=$ANDROID_NDK_ROOT/ndk-build
cd jni
$ndk_build
```
