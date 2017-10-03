cmake_minimum_required(VERSION 2.8)

project(Verovio)

set(CMAKE_BUILD_TYPE Release)

option(NO_DARMS_SUPPORT         "Disable Humdrum support"                      ON)
option(NO_PAE_SUPPORT           "Disable Plaine & Easie support"               OFF)
option(NO_HUMDRUM_SUPPORT       "Disable Humdrum support"                      OFF)
option(MUSICXML_DEFAULT_HUMDRUM "Enable MusicXML to Humdrum by default"        OFF)
option(BUILD_AS_LIBRARY         "Build verovio as library"                     OFF)

if (NO_HUMDRUM_SUPPORT AND MUSICXML_DEFAULT_HUMDRUM)
    message(SEND_ERROR "Default MusicXML to Humdrum cannot be enabled by default without Humdrum support")
endif()

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

if(NO_DARMS_SUPPORT)
    add_definitions(-DNO_DARMS_SUPPORT)
endif()

if(NO_PAE_SUPPORT)
    add_definitions(-DNO_PAE_SUPPORT)
endif()

if(NO_HUMDRUM_SUPPORT)
    add_definitions(-DNO_HUMDRUM_SUPPORT)
else()
    file(GLOB hum_SRC "../src/hum/*.cpp")
    if (MUSICXML_DEFAULT_HUMDRUM)
        add_definitions(-DMUSICXML_DEFAULT_HUMDRUM)
    endif()
endif()

file(GLOB verovio_SRC "../src/*.cpp")
file(GLOB midi_SRC "../src/midi/*.cpp")

# Add header files to custom target, otherwise they are not shown in some IDEs (e.g. QtCreator)
file(GLOB_RECURSE LibFiles "../include/*.h")
add_custom_target(headers SOURCES ${LibFiles})

set(all_SRC
    ${verovio_SRC}
    ${hum_SRC}
    ${midi_SRC}
    ../src/pugi/pugixml.cpp
    ../libmei/attconverter.cpp
    ../libmei/atts_analytical.cpp
    ../libmei/atts_cmn.cpp
    ../libmei/atts_cmnornaments.cpp
    ../libmei/atts_critapp.cpp
    ../libmei/atts_gestural.cpp
    ../libmei/atts_externalsymbols.cpp
    ../libmei/atts_mei.cpp
    ../libmei/atts_mensural.cpp
    ../libmei/atts_midi.cpp
    ../libmei/atts_pagebased.cpp
    ../libmei/atts_shared.cpp
    ../libmei/atts_visual.cpp
)

if (BUILD_AS_LIBRARY)
    message(STATUS "build verovio as library")
    add_library (
        verovio
        SHARED
        ${all_SRC}
    )
else()
    message(STATUS "build verovio as executable")
    add_executable (
        verovio
        main.cpp
        ${all_SRC}
    )
endif()


install(
    TARGETS verovio
    DESTINATION /usr/local/bin
)
install(
    DIRECTORY ../data/
    DESTINATION share/verovio
    FILES_MATCHING PATTERN "*.xml"
)
