#!/bin/bash

mkdir -p src/main/java/org/rismch/verovio
mkdir -p target
mkdir -p target/classes/META-INF/lib

# generate the git commit include file
cd ..
../tools/get_git_commit.sh
cd java

swig -c++ -java -package org.rismch.verovio -outdir src/main/java/org/rismch/verovio verovio.i

SRCFILES=$(\ls ../../src/*.cpp)

FILES="$SRCFILES \
 ../../src/pugi/pugixml.cpp \
 ../../src/midi/Binasc.cpp \
 ../../src/midi/MidiEvent.cpp \
 ../../src/midi/MidiEventList.cpp \
 ../../src/midi/MidiFile.cpp \
 ../../src/midi/MidiMessage.cpp \
 ../../src/hum/humlib.cpp \
 ../../src/json/jsonxx.cc \
 ../../libmei/attconverter.cpp \
 ../../libmei/atts_analytical.cpp \
 ../../libmei/atts_cmn.cpp \
 ../../libmei/atts_cmnornaments.cpp \
 ../../libmei/atts_critapp.cpp \
 ../../libmei/atts_gestural.cpp \
 ../../libmei/atts_externalsymbols.cpp \
 ../../libmei/atts_facsimile.cpp \
 ../../libmei/atts_mei.cpp \
 ../../libmei/atts_mensural.cpp \
 ../../libmei/atts_midi.cpp \
 ../../libmei/atts_neumes.cpp \
 ../../libmei/atts_pagebased.cpp \
 ../../libmei/atts_shared.cpp \
 ../../libmei/atts_visual.cpp"

CXXOPTS="-g -fpic -std=c++17 -I../../include -I../../include/vrv -I../../include/json -I../../include/hum -I../../include/midi -I../../include/pugi -I../../include/utf8 -I../../libmei -I/opt/local/include/ "

PATHS=""
unamestr=$(uname)
if [[ "$unamestr" == 'Linux' ]]; then
    PATHS="-I$JAVA_HOME/include -I$JAVA_HOME/include/linux " # paths to java libraries
elif [[ "$unamestr" == 'Darwin' ]]; then
    JAVA_HOME=$(/usr/libexec/java_home)
    PATHS="-I$JAVA_HOME/include -I$JAVA_HOME/include/darwin"
fi

g++ -shared -o target/libverovio.jnilib $CXXOPTS $PATHS $FILES verovio_wrap.cxx
cp target/libverovio.jnilib target/classes/META-INF/lib
