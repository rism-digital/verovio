Build instructions
------------------

Building the demo for Qt involves following steps:

1) Build the Verovio C++ project as a library and link the library to the tools directory. This can be done with the following commands:

```
mkdir VEROVIO_ROOT/tools/build-library
cd VEROVIO_ROOT/tools/build-library
cmake -DBUILD_AS_LIBRARY=ON ..
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

