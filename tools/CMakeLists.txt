cmake_minimum_required(VERSION 2.8)

project(Verovio)

SET(CMAKE_BUILD_TYPE Release)

add_definitions(-g)

EXEC_PROGRAM(../tools/get_git_commit.sh ARGS OUTPUT_VARIABLE GIT_COMMIT)

include_directories(/usr/local/include ../include ../include/vrv ../libmei)

add_executable (verovio 
	main.cpp
	../src/accid.cpp
	../src/aligner.cpp
	../src/att.cpp
	../src/barline.cpp
	../src/bboxdevicecontext.cpp
	../src/beam.cpp
	../src/chord.cpp
	../src/clef.cpp
	../src/custos.cpp
	../src/devicecontext.cpp
	../src/doc.cpp	
	../src/dot.cpp
	../src/drawinglistinterface.cpp
	../src/durationinterface.cpp
	../src/editorial.cpp
	../src/glyph.cpp
	../src/io.cpp
	../src/iodarms.cpp
	../src/iomei.cpp
	../src/iopae.cpp
	../src/iomusxml.cpp
	../src/keysig.cpp
	../src/layer.cpp
	../src/layerelement.cpp
	../src/mensur.cpp
	../src/metersig.cpp
	../src/measure.cpp
	../src/measureelement.cpp
	../src/mrest.cpp
	../src/multirest.cpp
	../src/note.cpp
	../src/object.cpp
	../src/page.cpp
	../src/pitchinterface.cpp
	../src/positioninterface.cpp
	../src/view.cpp
	../src/view_beam.cpp
	../src/view_element.cpp
	../src/view_graph.cpp
	../src/view_mensural.cpp
	../src/view_page.cpp
	../src/view_tuplet.cpp
	../src/rest.cpp
	../src/scoredef.cpp
	../src/slur.cpp
	../src/space.cpp
	../src/staff.cpp
	../src/style.cpp
	../src/svgdevicecontext.cpp
	../src/syl.cpp
	../src/system.cpp
	../src/tie.cpp
	../src/timeinterface.cpp
	../src/toolkit.cpp
	../src/tuplet.cpp
	../src/verse.cpp
	../src/vrv.cpp
	../src/pugixml.cpp
	#../libmei/atts_analysis.cpp
	#../libmei/atts_cmnornaments.cpp
	#../libmei/atts_edittrans.cpp
	#../libmei/atts_figtable.cpp
	#../libmei/atts_header.cpp
	#../libmei/atts_lyrics.cpp
	#../libmei/atts_midi.cpp
	../libmei/atts_shared.cpp
	../libmei/atts_cmn.cpp
	../libmei/atts_critapp.cpp
	#../libmei/atts_facsimile.cpp
	#../libmei/atts_harmony.cpp
	#../libmei/atts_linkalign.cpp
	../libmei/atts_mensural.cpp
	../libmei/atts_pagebased.cpp
	#../libmei/atts_neumes.cpp
	#../libmei/atts_tablature.cpp
	)

install (TARGETS verovio DESTINATION /usr/local/bin)
INSTALL(DIRECTORY ../data/ DESTINATION share/verovio FILES_MATCHING PATTERN "*.xml")
