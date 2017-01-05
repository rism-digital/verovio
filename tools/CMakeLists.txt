cmake_minimum_required(VERSION 2.8)

project(Verovio)

set(CMAKE_BUILD_TYPE Release)

option(NO_PAE_SUPPORT     "Disable Plain and Easy support" OFF)
option(NO_HUMDRUM_SUPPORT "Disable Humdrum support"        OFF)

include_directories(
    ../include
    ../include/midi
    ../include/hum
    ../include/pugi
    ../include/utf8
    ../include/vrv
    ../libmei
)

if(MSVC)
    add_definitions(/W2)
    add_definitions(/wd4244)          # suppress warning of possible loss of precision
    add_definitions(-DNO_PAE_SUPPORT) # regex is working differently under Windows so PAE is not supported (yet)
    include_directories(../include/win32)
else()
    add_definitions(-Wall)
    add_definitions(-g)
    add_definitions(-std=c++11)
endif()

execute_process(
    COMMAND ../tools/get_git_commit.sh
    OUTPUT_VARIABLE GIT_COMMIT
)

if(NO_PAE_SUPPORT)
    add_definitions(-DNO_PAE_SUPPORT)
endif()

if(NO_HUMDRUM_SUPPORT)
    add_definitions(-DNO_HUMDRUM_SUPPORT)
else()
    file(GLOB hum_SRC "../src/hum/*.cpp")
endif()

file(GLOB verovio_SRC "../src/*.cpp")
file(GLOB midi_SRC "../src/midi/*.cpp")

add_executable (
    verovio
    main.cpp
    ${verovio_SRC}
    ${hum_SRC}
    ${midi_SRC}
    ../src/pugi/pugixml.cpp
    ../libmei/attconverter.cpp
    ../libmei/atts_cmn.cpp
    ../libmei/atts_critapp.cpp
    ../libmei/atts_externalsymbols.cpp
    ../libmei/atts_mei.cpp
    ../libmei/atts_mensural.cpp
    ../libmei/atts_midi.cpp
    ../libmei/atts_pagebased.cpp
    ../libmei/atts_shared.cpp
)

install(
    TARGETS verovio
    DESTINATION /usr/local/bin
)
install(
    DIRECTORY ../data/
    DESTINATION share/verovio
    FILES_MATCHING PATTERN "*.xml"
)
