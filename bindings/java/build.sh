#!/bin/bash

mkdir -p src/main/java/org/rismch/verovio
mkdir -p target
mkdir -p target/classes/META-INF/lib

# generate the git commit include file
../tools/get_git_commit.sh

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
 ../../libmei/attconverter.cpp \
 ../../libmei/atts_analytical.cpp \
 ../../libmei/atts_cmn.cpp \
 ../../libmei/atts_cmnornaments.cpp \
 ../../libmei/atts_critapp.cpp \
 ../../libmei/atts_gestural.cpp \
 ../../libmei/atts_externalsymbols.cpp \
 ../../libmei/atts_mei.cpp \
 ../../libmei/atts_mensural.cpp \
 ../../libmei/atts_midi.cpp \
 ../../libmei/atts_pagebased.cpp \
 ../../libmei/atts_shared.cpp \
 ../../libmei/atts_visual.cpp"

CXXOPTS="-g -fpic -std=c++11 -I../../include -I../../include/vrv -I../../include/hum -I../../include/midi -I../../include/pugi -I../../include/utf8 -I../../libmei -I/opt/local/include/ -I/System/Library/Frameworks/JavaVM.framework/Headers/"
g++ -shared -o target/libverovio.jnilib $CXXOPTS $FILES verovio_wrap.cxx
cp target/libverovio.jnilib target/classes/META-INF/lib
