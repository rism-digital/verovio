## About

This allows verovio to be built as a native library for Android. It does not include any Java bindings.

DISCLAIMER: It has been reported on various occasions that this is not working properly. There seem to be other way to have it working, and contributions would be welcome.

### Build with ndk-build

```sh
export ANDROID_NDK_ROOT="/opt/Android/android-ndk-r21b" # adjust to NDK install ANDROID_NDK
ndk_build=$ANDROID_NDK_ROOT/ndk-build
cd jni
$ndk_build
```

### Build with CMake

Set `NDK` and the appropriate `ABI` (e.g., to `armeabi-v7a`) environment variables and run:
```
 cmake -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake -DANDROID_ABI=$ABI -DBUILD_AS_ANDROID_LIBRARY=ON ../../cmake
 ```