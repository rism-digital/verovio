#!/bin/bash

mkdir -p src/main/java/org/rismch/verovio
mkdir -p target
mkdir -p target/classes/META-INF/lib

# generate the git commit include file
../tools/get_git_commit.sh

swig -c++ -java -package org.rismch.verovio -outdir src/main/java/org/rismch/verovio verovio.i

FILES="../src/accid.cpp \
             ../src/aligner.cpp \
             ../src/anchoredtext.cpp \
             ../src/att.cpp \
             ../src/barline.cpp \
             ../src/bboxdevicecontext.cpp \
             ../src/beam.cpp \
             ../src/chord.cpp \
             ../src/clef.cpp \
             ../src/custos.cpp \
             ../src/devicecontext.cpp \
             ../src/dir.cpp \
             ../src/doc.cpp \
             ../src/dot.cpp \
             ../src/drawinginterface.cpp \
             ../src/durationinterface.cpp \
             ../src/dynam.cpp \
             ../src/editorial.cpp \
             ../src/ending.cpp \
             ../src/floatingelement.cpp \
             ../src/glyph.cpp \
             ../src/hairpin.cpp \
             ../src/io.cpp \
             ../src/iodarms.cpp \
             ../src/iomei.cpp \
             ../src/iopae.cpp \
             ../src/iomusxml.cpp \
             ../src/keysig.cpp \
             ../src/layer.cpp \
             ../src/layerelement.cpp \
             ../src/mensur.cpp \
             ../src/metersig.cpp \
             ../src/measure.cpp \
             ../src/mrest.cpp \
             ../src/multirest.cpp \
             ../src/note.cpp \
             ../src/object.cpp \
             ../src/octave.cpp \
             ../src/page.cpp \
             ../src/pedal.cpp \
             ../src/pitchinterface.cpp \
             ../src/positioninterface.cpp \
             ../src/proport.cpp \
             ../src/rest.cpp \
             ../src/rpt.cpp \
             ../src/scoredef.cpp \
             ../src/scoredefinterface.cpp \
             ../src/slur.cpp \
             ../src/space.cpp \
             ../src/staff.cpp \
             ../src/style.cpp \
             ../src/svgdevicecontext.cpp \
             ../src/syl.cpp \
             ../src/system.cpp \
             ../src/tempo.cpp \
             ../src/text.cpp \
             ../src/textdirinterface.cpp \
             ../src/textelement.cpp \
             ../src/tie.cpp \
             ../src/timeinterface.cpp \
             ../src/timestamp.cpp \
             ../src/toolkit.cpp \
             ../src/tuplet.cpp \
             ../src/verse.cpp \
             ../src/view.cpp \
             ../src/view_beam.cpp \
             ../src/view_element.cpp \
             ../src/view_floating.cpp \
             ../src/view_graph.cpp \
             ../src/view_mensural.cpp \
             ../src/view_page.cpp \
             ../src/view_text.cpp \
             ../src/view_tuplet.cpp \
             ../src/vrv.cpp \
             ../src/pugi/pugixml.cpp \
             ../src/midi/Binasc.cpp \
             ../src/midi/MidiEvent.cpp \
             ../src/midi/MidiEventList.cpp \
             ../src/midi/MidiFile.cpp \
             ../src/midi/MidiMessage.cpp \
             ../libmei/attconverter.cpp \
             ../libmei/atts_cmn.cpp \
             ../libmei/atts_critapp.cpp \
             ../libmei/atts_mei.cpp \
             ../libmei/atts_mensural.cpp \
             ../libmei/atts_pagebased.cpp \
             ../libmei/atts_shared.cpp"

CXXOPTS="-g -fpic -std=c++11 -I../include -I../include/vrv -I../include/midi -I../include/pugi -I../include/utf8 -I../libmei -I/opt/local/include/ -I/System/Library/Frameworks/JavaVM.framework/Headers/"
g++ -shared -o target/libverovio.jnilib $CXXOPTS $FILES verovio_wrap.cxx
cp target/libverovio.jnilib target/classes/META-INF/lib

