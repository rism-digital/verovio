Build instructions
------------------

Building the demo for Qt involves following steps:

1) Build the Verovio C++ project as a library and link the library to the tools directory. This can be done with the following commands:

```
mkdir VEROVIO_ROOT/tools/build-library
cd VEROVIO_ROOT/tools/build-library
cmake -DBUILD_AS_LIBRARY=ON ../../cmake
make -j8
ln -s $(pwd)/libverovio.so ..
```

2) Build the verovio-qt library. Make sure that qmake is in your $PATH. You need a Qt version >= 5.8.

```
mkdir VEROVIO_ROOT/bindings/qt/build-library
cd VEROVIO_ROOT/bindings/qt/build-library
qmake ../lib
make -j8
```

3) Build the verovio-qt-demo:

```
mkdir VEROVIO_ROOT/bindings/qt/build-demo
cd VEROVIO_ROOT/bindings/qt/build-demo
qmake ../demo
make -j8
```

Start Qt Demo
-------------

To start the Qt demo add the directories for the CPP and Qt libraries to LD_LIBRARY_PATH

```
LD_LIBRARY_PATH=../../../tools/:../build-library ./verovio-qt-demo
```

Android
-------

The demo can also be compiled for Android. This requires following steps:

1) Build the Verovio C++ project for Android: VEROVIO_ROOT/bindings/android. See there for build instructions.

2) Build the verovio-qt library for Android:
    - The steps are basically the same as in the normal build instructons.
    - Make sure to use the qmake version for android (e.g. QTDIR/5.15.0/android/bin/qmake)
    - Set the ANDROID_NDK_ROOT accordingly

```
export ANDROID_NDK_ROOT="/opt/Android/android-ndk-r21b"
mkdir VEROVIO_ROOT/bindings/qt/build-android
cd VEROVIO_ROOT/bindings/qt/build-android
QTDIR/android/bin/qmake ../lib/verovio-qt.pro -spec android-clang ANDROID_ABIS="armeabi-v7a"
make -j8
```

3) Building the verovio-qt-demo from command line is possible, but if you want to deploy the demo to your phone it is easier to use QtCreator:
    - Open the verovio-qt-demo.pro file with QtCreator
    - Configure the project for an Android kit (e.g. "Android for armeabi-v7a (GCC 4.9 Qt 5.8.0 for Android armv72")
    - Build and deploy to your Android device.

Known issues: The "Choose file" dialog is severely broken and it opens behind the Options pane.
