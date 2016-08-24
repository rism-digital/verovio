cmake_minimum_required(VERSION 2.8)

project(Verovio)

SET(CMAKE_BUILD_TYPE Release)

option(NO_PAE_SUPPORT "Disable Plain and Easy support" OFF)

add_definitions(-g)
add_definitions(-Wall)
add_definitions(-std=c++11)

EXEC_PROGRAM(../tools/get_git_commit.sh ARGS OUTPUT_VARIABLE GIT_COMMIT)

include_directories(../include ../include/midi ../include/pugi ../include/utf8 ../include/vrv ../include/win32 ../libmei)

if(NO_PAE_SUPPORT)
  add_definitions(-DNO_PAE_SUPPORT)
endif()

FILE(GLOB verovio_SRC "../src/*.cpp")

add_executable (verovio 
	main.cpp
	${verovio_SRC}
	../src/pugi/pugixml.cpp
	../src/midi/Binasc.cpp
	../src/midi/MidiEvent.cpp
	../src/midi/MidiEventList.cpp
	../src/midi/MidiFile.cpp
	../src/midi/MidiMessage.cpp
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
