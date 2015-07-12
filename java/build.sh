#!/bin/bash

mkdir -p src/main/java/org/rismch/verovio
mkdir -p target
mkdir -p target/classes/META-INF/lib

# generate the git commit include file
../tools/get_git_commit.sh

swig -c++ -java -package org.rismch.verovio -outdir src/main/java/org/rismch/verovio verovio.i

FILES="../src/accid.cpp \
             ../src/aligner.cpp \
             ../src/att.cpp \
             ../src/barline.cpp \
             ../src/bboxdevicecontext.cpp \
             ../src/beam.cpp \
             ../src/chord.cpp \
             ../src/clef.cpp \
             ../src/custos.cpp \
             ../src/devicecontext.cpp \
             ../src/doc.cpp \
             ../src/dot.cpp \
             ../src/drawinglistinterface.cpp \
             ../src/durationinterface.cpp \
             ../src/editorial.cpp \
             ../src/glyph.cpp \
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
             ../src/measureelement.cpp \
             ../src/mrest.cpp \
             ../src/multirest.cpp \
             ../src/note.cpp \
             ../src/object.cpp \
             ../src/page.cpp \
             ../src/pitchinterface.cpp \
             ../src/positioninterface.cpp \
             ../src/view.cpp \
             ../src/view_beam.cpp \
             ../src/view_element.cpp \
             ../src/view_graph.cpp \
             ../src/view_mensural.cpp \
             ../src/view_page.cpp \
             ../src/view_tuplet.cpp \
             ../src/rest.cpp \
             ../src/scoredef.cpp \
             ../src/slur.cpp \
             ../src/space.cpp \
             ../src/staff.cpp \
             ../src/style.cpp \
             ../src/svgdevicecontext.cpp \
             ../src/syl.cpp \
             ../src/system.cpp \
             ../src/tie.cpp \
             ../src/timeinterface.cpp \
             ../src/toolkit.cpp \
             ../src/tuplet.cpp \
             ../src/verse.cpp \
             ../src/vrv.cpp \
             ../src/pugixml.cpp \
             ../libmei/atts_shared.cpp \
             ../libmei/atts_cmn.cpp \
             ../libmei/atts_critapp.cpp \
             ../libmei/atts_mensural.cpp \
             ../libmei/atts_pagebased.cpp"

CXXOPTS="-g -fpic -I../include -I../include/vrv -I../libmei -I/opt/local/include/ -I/System/Library/Frameworks/JavaVM.framework/Headers/"
g++ -shared -o target/libverovio.jnilib $CXXOPTS $FILES verovio_wrap.cxx
cp target/libverovio.jnilib target/classes/META-INF/lib

