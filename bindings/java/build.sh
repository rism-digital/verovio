#!/bin/bash

mkdir -p src/main/java/org/verovio/lib
mkdir -p target
mkdir -p target/classes/META-INF/lib

# generate the git commit include file
cd ..
../tools/get_git_commit.sh
cd java

swig -c++ -java -package org.verovio.lib -outdir src/main/java/org/verovio/lib verovio.i

SRCFILES=$(\ls ../../src/*.cpp ../../libmei/dist/*.cpp ../../libmei/addons/*.cpp)

FILES="$SRCFILES \
 ../../src/hum/humlib.cpp \
 ../../external/crc/crc.cpp \
 ../../external/jsonxx/jsonxx.cc \
 ../../external/midifile/src/Binasc.cpp \
 ../../external/midifile/src/MidiEvent.cpp \
 ../../external/midifile/src/MidiEventList.cpp \
 ../../external/midifile/src/MidiFile.cpp \
 ../../external/midifile/src/MidiMessage.cpp \
 ../../external/pugixml/src/pugixml.cpp \
 ../../external/harfbuzz/src/verovio-harfbuzz.cc \
 ../../external/woff2/src/verovio-woff2.cc \
 target/verovio-brotli.o"

CXXOPTS="-g -fpic -std=c++20 -DBROTLI_STATIC -DHB_LEAN -DHB_MINI -DHB_OPTIMIZE_SIZE_MORE -DHB_CONFIG_OVERRIDE_H=\"verovio-hb-config.h\" -I../../include -I../../include/vrv -I../../include/hum -I../../libmei/addons -I../../libmei/dist -I../../external/crc -I../../external/jsonxx -I../../external/midifile/include -I../../external/miniz-cpp -I../../external/pugixml/src -I../../external/tuning-library/include -I../../external/harfbuzz/src -I../../external/woff2/include -I../../external/brotli/c/include -I/opt/local/include/ "

PATHS=""
unamestr=$(uname)
if [[ "$unamestr" == 'Linux' ]]; then
    PATHS="-I$JAVA_HOME/include -I$JAVA_HOME/include/linux " # paths to java libraries
elif [[ "$unamestr" == 'Darwin' ]]; then
    #JAVA_HOME="/opt/homebrew/Cellar/openjdk/20.0.1/libexec/openjdk.jdk/Contents/Home"
    JAVA_HOME=$(/usr/libexec/java_home)
    PATHS="-I$JAVA_HOME/include -I$JAVA_HOME/include/darwin"
fi

cc -c -fpic -O2 -DBROTLI_STATIC -I../../external/brotli/c/include ../../external/brotli/c/verovio-brotli.c -o target/verovio-brotli.o
g++ -shared -o target/libverovio.jnilib $CXXOPTS $PATHS $FILES verovio_wrap.cxx
cp target/libverovio.jnilib target/classes/META-INF/lib
