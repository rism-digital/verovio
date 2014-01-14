cmake_minimum_required(VERSION 2.8)

project(Aruspix)

add_definitions(-g)

include_directories(/usr/local/include)

link_libraries()

include_directories(src/mus src/ src/tinyxml)

add_executable (aruspix 
	src/mus/main.cpp 
	src/mus/mus.cpp
	src/mus/musaligner.cpp
	src/mus/musapp.cpp
	src/mus/musbarline.cpp 
	src/mus/musbboxdc.cpp 
	src/mus/musbeam.cpp
	src/mus/musclef.cpp 
	src/mus/muscontroller.cpp
	src/mus/musdc.cpp 
	src/mus/musdoc.cpp 	
	src/mus/musdurationinterface.cpp 
	src/mus/musio.cpp 
	src/mus/musiodarms.cpp
	src/mus/musiomei.cpp 
	src/mus/musiomusxml.cpp
	src/mus/musiopae.cpp 
	src/mus/muskeysig.cpp
	src/mus/muslayer.cpp
	src/mus/muslayerelement.cpp 
	src/mus/musleipzigbbox.cpp
	src/mus/musmensur.cpp 
	src/mus/musmeasure.cpp
	src/mus/musmultirest.cpp
	src/mus/musnote.cpp 
	src/mus/musobject.cpp 
	src/mus/muspage.cpp 
	src/mus/muspitchinterface.cpp 
	src/mus/muspositioninterface.cpp 
	src/mus/musrc.cpp 
	src/mus/musrc_beam.cpp  
	src/mus/musrc_bezier.cpp 
	src/mus/musrc_element.cpp 
	src/mus/musrc_graph.cpp 
	src/mus/musrc_page.cpp
	src/mus/musrc_tuplet.cpp
	src/mus/musrest.cpp 
	src/mus/musscoredef.cpp
	src/mus/musslur.cpp
	src/mus/musstaff.cpp 
	src/mus/mussvgdc.cpp 
	src/mus/mussymbol.cpp 
	src/mus/mussystem.cpp 
	src/mus/mustie.cpp
	src/mus/mustuplet.cpp
	src/tinyxml/tinystr.cpp
	src/tinyxml/tinyxml.cpp
	src/tinyxml/tinyxmlerror.cpp
	src/tinyxml/tinyxmlparser.cpp

	)

install (TARGETS aruspix DESTINATION bin)
INSTALL(DIRECTORY data/svg/ DESTINATION share/aruspix/svg FILES_MATCHING PATTERN "*.xml")
