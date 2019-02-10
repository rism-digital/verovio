#!/bin/bash

# specify custom Qt directory
# qmake="~/Qt/5.9.1/gcc_64/bin/qmake"

verovio_root="$(pwd)/../.."

if [[ "$@" == "clean" ]]
then
	rm -r $verovio_root/tools/build-library
	rm $verovio_root/tools/libverovio.so
	rm -r $verovio_root/bindings/qt/build-library
	rm -r $verovio_root/bindings/qt/build-demo
	exit 0
fi

if [ -z "$qmake" ]
then
	echo "No user-specified qmake. Check system..."
	if [ -x "$(command -v qmake)" ]
	then
		echo "Found qmake"
		qmake="qmake"
	else
		echo "Could not find qmake. Make sure it is in your path or manually specify the qmake variable"
		exit 1
	fi
fi

# build verovio CPP lib
mkdir "$verovio_root/tools/build-library"
cd "$verovio_root/tools/build-library"
cmake -DBUILD_AS_LIBRARY=ON ..
make -j8
ln -s "$(pwd)/libverovio.so" ..

# build verovio Qt lib
mkdir "$verovio_root/bindings/qt/build-library"
cd "$verovio_root/bindings/qt/build-library"
$qmake ../lib
make -j8

# build verovio Qt demo
mkdir "$verovio_root/bindings/qt/build-demo"
cd "$verovio_root/bindings/qt/build-demo"
$qmake ../demo
make -j8

# To start the Qt demo add the directories for the CPP and Qt libraries to LD_LIBRARY_PATH
# LD_LIBRARY_PATH=../../../tools/:../build-library ./verovio-qt-demo 
