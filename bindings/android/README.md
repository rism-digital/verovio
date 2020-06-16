About
-----

This allows verovio to be built as a native library for Android. It does not include any Java bindings.


Build instructions
------------------

As a prerequisite you have to install the Android NDK. 

```
export ANDROID_NDK_ROOT="/opt/Android/android-ndk-r21b"       # adjust to NDK install ANDROID_NDK
ndk_build=$ANDROID_NDK_ROOT/ndk-build
cd jni
$ndk_build
```
