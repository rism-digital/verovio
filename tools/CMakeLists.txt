cmake_minimum_required(VERSION 2.8)

project(Verovio)

add_definitions(-g)

include_directories(/usr/local/include ../include/vrv ../tinyxml)

add_executable (verovio 
	main.cpp 
	../src/aligner.cpp
	../src/app.cpp
	../src/barline.cpp 
	../src/bboxdevicecontext.cpp 
	../src/beam.cpp
	../src/clef.cpp 
	../src/devicecontext.cpp 
	../src/doc.cpp 	
	../src/durationinterface.cpp 
	../src/io.cpp 
	../src/iodarms.cpp
	../src/iomei.cpp 
	../src/iopae.cpp 
	../src/interfacecontroller.cpp
	../src/keysig.cpp
	../src/layer.cpp
	../src/layerelement.cpp 
	../src/leipzigbbox.cpp
	../src/mensur.cpp 
	../src/measure.cpp
	../src/mrest.cpp
	../src/multirest.cpp
	../src/note.cpp 
	../src/object.cpp 
	../src/page.cpp 
	../src/pitchinterface.cpp 
	../src/positioninterface.cpp 
	../src/view.cpp 
	../src/view_beam.cpp  
	../src/view_bezier.cpp 
	../src/view_element.cpp 
	../src/view_graph.cpp 
	../src/view_page.cpp
	../src/view_tuplet.cpp
	../src/rest.cpp 
	../src/scoredef.cpp
	../src/slur.cpp
	../src/staff.cpp 
	../src/svgdevicecontext.cpp 
	../src/symbol.cpp 
	../src/system.cpp 
	../src/tie.cpp
	../src/tuplet.cpp
	../src/vrv.cpp
	../tinyxml/tinystr.cpp
	../tinyxml/tinyxml.cpp
	../tinyxml/tinyxmlerror.cpp
	../tinyxml/tinyxmlparser.cpp
	../src/pugixml.cpp
	)

install (TARGETS verovio DESTINATION /usr/local/bin)
INSTALL(DIRECTORY data/svg/ DESTINATION share/verovio/svg FILES_MATCHING PATTERN "*.xml")
