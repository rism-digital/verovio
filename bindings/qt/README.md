Build instructions
------------------

Running the demo for Qt involves following steps:

1) Build the Verovio C++ project as a library. This can be done with the following commands:

    mkdir VEROVIO_ROOT/build
    cd VEROVIO_ROOT/build
    cmake -DBUILD_AS_LIBRARY=ON ../tools/

2) Build the verovio-qt library:
	- You need a Qt version >= 5.8
	- Open the VEROVIO_ROOT/qt/lib/verovio-qt.pro file in QtCreator
	- Compile the project
	- Create a symlink from VEROVIO_ROOT/qt/build-qt-Desktop* to VEROVIO_ROOT/qt/build

3) Build the verovio-qt-demo:
	- Open VEROVIO_ROOT/qt/demo/verovio-qt-demo.pro with QtCreator
	- Compile the project
	- Run it
