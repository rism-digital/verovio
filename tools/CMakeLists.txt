cmake_minimum_required(VERSION 2.8)

project(Verovio)

SET(CMAKE_BUILD_TYPE Release)

option(NO_PAE_SUPPORT "Disable Plain and Easy support" OFF)

option(NO_HUMDRUM_SUPPORT "Disable Humdrum support" OFF)

add_definitions(-g)
add_definitions(-std=c++11)

EXEC_PROGRAM(../tools/get_git_commit.sh ARGS OUTPUT_VARIABLE GIT_COMMIT)

include_directories(/usr/local/include ../include ../include/midi ../include/hum ../include/pugi ../include/utf8 ../include/vrv ../libmei)

if(NO_PAE_SUPPORT)
  add_definitions(-DNO_PAE_SUPPORT)
endif()

if(NO_HUMDRUM_SUPPORT)
  add_definitions(-DNO_HUMDRUM_SUPPORT)
endif()

add_executable (verovio 
	main.cpp
	../src/accid.cpp
	../src/aligner.cpp
	../src/anchoredtext.cpp
	../src/att.cpp
	../src/barline.cpp
	../src/bboxdevicecontext.cpp
	../src/beam.cpp
	../src/chord.cpp
	../src/clef.cpp
	../src/custos.cpp
	../src/devicecontext.cpp
	../src/dir.cpp
	../src/doc.cpp	
	../src/dot.cpp
	../src/drawinginterface.cpp
	../src/durationinterface.cpp
	../src/dynam.cpp
	../src/editorial.cpp
	../src/floatingelement.cpp
	../src/glyph.cpp
	../src/hairpin.cpp
	../src/io.cpp
	../src/iodarms.cpp
	../src/iomei.cpp
	../src/iopae.cpp
	../src/iohumdrum.cpp
	../src/iomusxml.cpp
	../src/keysig.cpp
	../src/layer.cpp
	../src/layerelement.cpp
	../src/mensur.cpp
	../src/metersig.cpp
	../src/measure.cpp
	../src/mrest.cpp
	../src/multirest.cpp
	../src/note.cpp
	../src/object.cpp
	../src/octave.cpp
	../src/page.cpp
	../src/pedal.cpp
	../src/pitchinterface.cpp
	../src/positioninterface.cpp
	../src/proport.cpp
	../src/rest.cpp
	../src/rpt.cpp
	../src/scoredef.cpp
	../src/scoredefinterface.cpp
	../src/slur.cpp
	../src/space.cpp
	../src/staff.cpp
	../src/style.cpp
	../src/svgdevicecontext.cpp
	../src/syl.cpp
	../src/system.cpp
	../src/tempo.cpp
	../src/text.cpp
	../src/textdirinterface.cpp
	../src/textelement.cpp
	../src/tie.cpp
	../src/timeinterface.cpp
	../src/timestamp.cpp
	../src/toolkit.cpp
	../src/tuplet.cpp
	../src/verse.cpp
	../src/view.cpp
	../src/view_beam.cpp
	../src/view_element.cpp
	../src/view_floating.cpp
	../src/view_graph.cpp
	../src/view_mensural.cpp
	../src/view_page.cpp
	../src/view_text.cpp
	../src/view_tuplet.cpp
	../src/vrv.cpp
	../src/pugi/pugixml.cpp
	../src/midi/Binasc.cpp
	../src/midi/MidiEvent.cpp
	../src/midi/MidiEventList.cpp
	../src/midi/MidiFile.cpp
	../src/midi/MidiMessage.cpp
	../src/hum/humlib.cpp
	../libmei/attconverter.cpp
	../libmei/atts_cmn.cpp
	../libmei/atts_critapp.cpp
	../libmei/atts_mei.cpp
	../libmei/atts_mensural.cpp
	../libmei/atts_pagebased.cpp
	../libmei/atts_shared.cpp
	)

install (TARGETS verovio DESTINATION /usr/local/bin)
INSTALL(DIRECTORY ../data/ DESTINATION share/verovio FILES_MATCHING PATTERN "*.xml")
