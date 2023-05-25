#!/bin/bash

mkdir -p src/main/java/org/rismch/verovio
mkdir -p target
mkdir -p target/classes/META-INF/lib

# generate the git commit include file
cd ..
../tools/get_git_commit.sh
cd java

swig -c++ -java -package org.rismch.verovio -outdir src/main/java/org/rismch/verovio verovio.i

SRCFILES=$(\ls ../../src/*.cpp ../../libmei/dist/*.cpp ../../libmei/addons/*.cpp)

FILES="$SRCFILES \
 ../../src/pugi/pugixml.cpp \
 ../../src/midi/Binasc.cpp \
 ../../src/midi/MidiEvent.cpp \
 ../../src/midi/MidiEventList.cpp \
 ../../src/midi/MidiFile.cpp \
 ../../src/midi/MidiMessage.cpp \
 ../../src/hum/humlib.cpp \
 ../../src/json/jsonxx.cc \
 ../../src/crc/crc.cpp"

CXXOPTS="-g -fpic -std=c++17 -I../../include -I../../include/vrv -I../../include/json -I../../include/hum -I../../include/crc -I../../include/midi -I../../include/pugi -I../../include/zip -I../../libmei/addons -I../../libmei/dist -I/opt/local/include/ "

PATHS=""
unamestr=$(uname)
if [[ "$unamestr" == 'Linux' ]]; then
    PATHS="-I$JAVA_HOME/include -I$JAVA_HOME/include/linux " # paths to java libraries
elif [[ "$unamestr" == 'Darwin' ]]; then
    #JAVA_HOME="/opt/homebrew/Cellar/openjdk/20.0.1/libexec/openjdk.jdk/Contents/Home"
    JAVA_HOME=$(/usr/libexec/java_home)
    PATHS="-I$JAVA_HOME/include -I$JAVA_HOME/include/darwin"
fi

g++ -shared -o target/libverovio.jnilib $CXXOPTS $PATHS $FILES verovio_wrap.cxx
cp target/libverovio.jnilib target/classes/META-INF/lib
