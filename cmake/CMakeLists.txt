cmake_minimum_required(VERSION 3.15)

file(READ "../include/vrv/vrvdef.h" VRVDEF_H)

string(REGEX MATCH "#define VERSION_MAJOR ([0-9]+)" _ ${VRVDEF_H})
set(VRV_VERSION_MAJOR ${CMAKE_MATCH_1})

string(REGEX MATCH "#define VERSION_MINOR ([0-9]+)" _ ${VRVDEF_H})
set(VRV_VERSION_MINOR ${CMAKE_MATCH_1})

string(REGEX MATCH "#define VERSION_REVISION ([0-9]+)" _ ${VRVDEF_H})
set(VRV_VERSION_REVISION ${CMAKE_MATCH_1})

project(Verovio VERSION ${VRV_VERSION_MAJOR}.${VRV_VERSION_MINOR}.${VRV_VERSION_REVISION})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if (APPLE)
    set(CMAKE_MACOSX_RPATH ON)
endif (APPLE)

option(VRV_DYNAMIC_CAST         "Enable <dynamic_cast>"                        OFF)
option(NO_DARMS_SUPPORT         "Disable DARMS support"                        ON)
option(NO_PAE_SUPPORT           "Disable Plaine & Easie support"               OFF)
option(NO_ABC_SUPPORT           "Disable ABC support"                          OFF)
option(NO_GABC_SUPPORT          "Disable GABC support"                         OFF)
option(NO_MUSICXML_SUPPORT      "Disable MusicXML support"                     OFF)
option(NO_MXL_SUPPORT           "Disable compressed MusicXML support"          OFF)
option(NO_HUMDRUM_SUPPORT       "Disable Humdrum support"                      OFF)
option(MUSICXML_DEFAULT_HUMDRUM "Enable MusicXML to Humdrum by default"        OFF)
option(NO_EDIT_SUPPORT          "Disable editor code"                          OFF)
option(NO_RUNTIME               "Disable runtime clock support"                ON)
option(BUILD_AS_LIBRARY         "Build Verovio as library"                     OFF)
option(BUILD_AS_ANDROID_LIBRARY "Build Verovio as library for Android"         OFF)
option(USE_PAE_OLD_PARSER       "Use old PAE parser"                           OFF)

if (NO_HUMDRUM_SUPPORT AND MUSICXML_DEFAULT_HUMDRUM)
    message(FATAL_ERROR "Default MusicXML to Humdrum cannot be enabled by default without Humdrum support")
endif()

# Sets the compile-time value for the RESOURCE_DIR macro in the source code.
add_compile_definitions(RESOURCE_DIR="${CMAKE_INSTALL_PREFIX}/share/verovio")

include_directories(
    ../include
    ../include/crc
    ../include/midi
    ../include/hum
    ../include/json
    ../include/pugi
    ../include/tuning-library
    ../include/zip
    ../include/vrv
    ../libmei/dist
    ../libmei/addons
)

if(WIN32)
    include_directories(../include/win32)
endif()

if(MSVC)
    add_compile_options(/bigobj)
    add_definitions(/W2)
    add_definitions(/wd4244)          # suppress warning of possible loss of precision
    add_definitions(-DNO_PAE_SUPPORT) # regex is working differently under Windows so PAE is not supported (yet)
    add_definitions(-DUSE_PAE_OLD_PARSER)
    add_definitions(/std:c++20)
else()
    if(CMAKE_BUILD_TYPE MATCHES Debug)
        message(STATUS "Enabling debug information")
        add_definitions(-g)
        add_definitions(-DDEBUG)
    endif()

    # jsonxx raises -Wdollar-in-identifier-extension
    # gcc 8.3.1 does not like -Wdollar-in-identifier-extension option.
    add_definitions(-Wall -W -pedantic -Wno-unused-parameter -Wno-dollar-in-identifier-extension)
    add_definitions(-std=c++20)

    # extra warnings similar to Xcode compiling settings (most probably covered by -Wall):
    # https://github.com/llvm-mirror/clang/blob/master/include/clang/Basic/DiagnosticGroups.td
    # non-gcc: add_definitions(-Wblock-capture-autoreleasing)
    # non-gcc: add_definitions(-Wbool-conversion)
    # non-gcc: add_definitions(-Wconstant-conversion)
    add_definitions(-Wdeprecated-declarations)
    add_definitions(-Wempty-body)
    # non-gcc: add_definitions(-Wenum-conversion)
    # non-gcc: add_definitions(-Winfinite-recursion)
    # non-gcc (ObjC only): add_definitions(-Wint-conversion)
    add_definitions(-Winvalid-offsetof)
    # non-gcc: add_definitions(-Wmove)
    # non-gcc: add_definitions(-Wno-c++11-extensions)
    # non-gcc: add_definitions(-Wno-comma)
    add_definitions(-Wno-conversion)
    # non-gcc: add_definitions(-Wno-exit-time-destructors)
    add_definitions(-Wno-float-conversion)
    # non-gcc: add_definitions(-Wno-four-char-constants)
    add_definitions(-Wno-missing-braces)
    add_definitions(-Wno-missing-field-initializers)
    # non-gcc (ObjC only): add_definitions(-Wno-missing-prototypes)
    # non-gcc: add_definitions(-Wno-newline-eof)
    # non-gcc: add_definitions(-Wno-non-virtual-dtor)
    add_definitions(-Wno-overloaded-virtual)
    # non-gcc: add_definitions(-Wno-semicolon-before-method-body)
    add_definitions(-Wno-shadow)
    add_definitions(-Wno-sign-conversion)
    add_definitions(-Wno-trigraphs)
    add_definitions(-Wno-unknown-pragmas)
    add_definitions(-Wno-unused-label)
    # non-gcc: add_definitions(-Wnon-literal-null-conversion)
    # non-gcc: add_definitions(-Wobjc-literal-conversion)
    add_definitions(-Wparentheses)
    # non-gcc: add_definitions(-Wrange-loop-analysis)
    # non-gcc: add_definitions(-Wshorten-64-to-32)
    # non-gcc: (ObjC only: add_definitions(-Wstrict-prototypes)
    add_definitions(-Wswitch)
    add_definitions(-Wuninitialized)
    add_definitions(-Wunreachable-code)
    add_definitions(-Wunused-function)
    add_definitions(-Wunused-value)
    add_definitions(-Wunused-variable)

endif()

execute_process(
    COMMAND ../tools/get_git_commit.sh
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    OUTPUT_VARIABLE GIT_COMMIT
)

if(VRV_DYNAMIC_CAST)
    add_definitions(-DVRV_DYNAMIC_CAST)
endif()

if(NO_DARMS_SUPPORT)
    add_definitions(-DNO_DARMS_SUPPORT)
endif()

if(NO_PAE_SUPPORT)
    add_definitions(-DNO_PAE_SUPPORT)
elseif(USE_PAE_OLD_PARSER)
    add_definitions(-DUSE_PAE_OLD_PARSER)
endif()

if(NO_ABC_SUPPORT)
    add_definitions(-DNO_ABC_SUPPORT)
endif()

if(NO_GABC_SUPPORT)
    add_definitions(-DNO_GABC_SUPPORT)
endif()

if(NO_MUSICXML_SUPPORT)
    add_definitions(-DNO_MUSICXML_SUPPORT)
endif()

if(NO_MXL_SUPPORT)
    add_definitions(-DNO_MXL_SUPPORT)
endif()

if(NO_HUMDRUM_SUPPORT)
    add_definitions(-DNO_HUMDRUM_SUPPORT)
else()
    file(GLOB hum_SRC "../src/hum/*.cpp")
    if (MUSICXML_DEFAULT_HUMDRUM)
        add_definitions(-DMUSICXML_DEFAULT_HUMDRUM)
    endif()
endif()

if(NO_EDIT_SUPPORT)
    add_definitions(-DNO_EDIT_SUPPORT)
endif()

if(NO_RUNTIME)
    add_definitions(-DNO_RUNTIME)
endif()

file(GLOB verovio_SRC "../src/*.cpp")
file(GLOB libmei_dist_SRC "../libmei/dist/*.cpp")
file(GLOB libmei_addons_SRC "../libmei/addons/*.cpp")
file(GLOB midi_SRC "../src/midi/*.cpp")
file(GLOB crc_SRC "../src/crc/*.cpp")

# Add header files to custom target, otherwise they are not shown in some IDEs (e.g. QtCreator)
file(GLOB_RECURSE LibFiles "../include/*.h")
add_custom_target(headers SOURCES ${LibFiles})

set(all_SRC
    ${verovio_SRC}
    ${libmei_dist_SRC}
    ${libmei_addons_SRC}
    ${hum_SRC}
    ${crc_SRC}
    ${midi_SRC}
    ../src/json/jsonxx.cc
    ../src/pugi/pugixml.cpp
)

##################
# Shared library #
##################

if (BUILD_AS_LIBRARY OR BUILD_AS_ANDROID_LIBRARY)
    message(STATUS "***** Building Verovio as shared library *****")
    if(WIN32)
		add_definitions(-DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS)
    endif()
    add_library(verovio SHARED "../tools/c_wrapper.cpp" ${all_SRC})
    # list all headers to be copied for all installation
    file(GLOB_RECURSE all_HEADERS "../include/*/*.h*" "../libmei/*/*.h*" "../tools/c_wrapper.h")
    configure_file(verovio.pc.in verovio.pc @ONLY)

###########
# WASM BC #
###########

elseif (BUILD_AS_WASM)
    if (EMSCRIPTEN_VERSION VERSION_LESS "3.1.27")
        message(FATAL_ERROR "You need em++ 3.1.27 or greater")
    endif()
    message(STATUS "***** Building Verovio as WASM BC *****")
    add_library(verovio STATIC ../tools/c_wrapper.cpp ${all_SRC})

#################
# Python module #
#################

elseif (BUILD_AS_PYTHON)
    message(STATUS "***** Building Verovio as Python module *****")

    # clear out policies
    cmake_policy(SET CMP0078 NEW)
    cmake_policy(SET CMP0086 NEW)

    find_package(SWIG REQUIRED)
    include(${SWIG_USE_FILE})
    set(CMAKE_SWIG_FLAGS "")
    set(verovio_python_use_sabi FALSE)
    if(NOT "${SKBUILD_SABI_VERSION}" STREQUAL "")
        set(verovio_python_use_sabi TRUE)
    endif()

    set(verovio_python_components Interpreter Development.Module)
    if(NOT "${SKBUILD_SABI_COMPONENT}" STREQUAL "")
        list(APPEND verovio_python_components ${SKBUILD_SABI_COMPONENT})
    endif()
    find_package(Python COMPONENTS ${verovio_python_components} REQUIRED)

    if(${SWIG_VERSION} VERSION_GREATER_EQUAL "4.1.0")
        list(APPEND CMAKE_SWIG_FLAGS "-doxygen")
        if(NOT verovio_python_use_sabi)
            list(APPEND CMAKE_SWIG_FLAGS "-fastproxy")
            list(APPEND CMAKE_SWIG_FLAGS "-olddefs")
        endif()
    else()
        list(APPEND CMAKE_SWIG_FLAGS "-py3;-DPY3")
    endif()

    set_source_files_properties(../bindings/python/verovio.i PROPERTIES CPLUSPLUS ON)
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/verovio")
    set(CMAKE_SWIG_OUTDIR "${CMAKE_CURRENT_BINARY_DIR}/verovio")

    # needed for static build on linux
    add_definitions(-fPIC)

    include_directories(${Python_INCLUDE_DIRS})
    message(STATUS "***** Building for Python ${Python_VERSION} *****")
    set(verovio_python_abi_mode "version-specific")
    set(verovio_python_module_target Python::Module)
    unset(verovio_python_module_suffix)
    unset(verovio_python_limited_api_hex)

    if(verovio_python_use_sabi AND Python_INTERPRETER_ID STREQUAL "Python")
        string(REPLACE "." ";" verovio_python_sabi_version "${SKBUILD_SABI_VERSION}")
        list(GET verovio_python_sabi_version 0 verovio_python_sabi_major)
        list(LENGTH verovio_python_sabi_version verovio_python_sabi_len)
        if(verovio_python_sabi_len GREATER 1)
            list(GET verovio_python_sabi_version 1 verovio_python_sabi_minor)
        else()
            set(verovio_python_sabi_minor 2)
        endif()

        math(EXPR verovio_python_limited_api_hex
             "(${verovio_python_sabi_major} << 24) + (${verovio_python_sabi_minor} << 16)"
             OUTPUT_FORMAT HEXADECIMAL)
        set(verovio_python_abi_mode "stable ABI ${SKBUILD_SABI_VERSION}+")
        set(verovio_python_module_target Python::SABIModule)
        if(DEFINED Python_SOSABI AND NOT "${Python_SOSABI}" STREQUAL "")
            set(verovio_python_module_suffix ".${Python_SOSABI}${CMAKE_SHARED_MODULE_SUFFIX}")
        endif()
    endif()
    message(STATUS "***** Python extension ABI mode: ${verovio_python_abi_mode} *****")

    add_library(verovio STATIC ${all_SRC})
    swig_add_library(_verovio LANGUAGE python TYPE MODULE SOURCES ../bindings/python/verovio.i)
    target_link_libraries(_verovio PRIVATE verovio ${verovio_python_module_target})
    if(DEFINED verovio_python_limited_api_hex)
        target_compile_definitions(_verovio PRIVATE Py_LIMITED_API=${verovio_python_limited_api_hex})
        set_property(TARGET _verovio APPEND PROPERTY SWIG_COMPILE_DEFINITIONS
                     Py_LIMITED_API=${verovio_python_limited_api_hex})
    endif()
    set_target_properties(_verovio PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/verovio"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/verovio"
        PREFIX ""
    )
    if(DEFINED verovio_python_module_suffix)
        set_target_properties(_verovio PROPERTIES SUFFIX "${verovio_python_module_suffix}")
    elseif(WIN32)
        set_target_properties(_verovio PROPERTIES SUFFIX ".pyd")
    elseif(DEFINED Python_SOABI)
        set_target_properties(_verovio PROPERTIES SUFFIX ".${Python_SOABI}${CMAKE_SHARED_MODULE_SUFFIX}")
    endif()

#####################
# Command-line tool #
#####################

else()
    message(STATUS "***** Building Verovio as command-line tool *****")
    add_executable(verovio ../tools/main.cpp ${all_SRC})

endif()

if (BUILD_AS_ANDROID_LIBRARY)
    find_library(log-lib log)
    # Setting for the linker to align segments at 16 KB (0x4000)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,max-page-size=0x4000")
    target_link_libraries(verovio ${log-lib})
endif()

if (BUILD_AS_PYTHON)
    install(TARGETS _verovio
            RUNTIME DESTINATION verovio
            LIBRARY DESTINATION verovio
    )
    install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/verovio/verovio.py"
            "../bindings/python/__init__.py"
            "../bindings/python/py.typed"
            "../bindings/python/verovio.pyi"
            DESTINATION verovio
    )
    install(
        DIRECTORY ../data/
        DESTINATION verovio/data
        FILES_MATCHING PATTERN "*.xml" PATTERN "*.svg" PATTERN "*.css" PATTERN "*.json"
    )
else()
    install(TARGETS verovio
            # for executables and dll on Win
            RUNTIME DESTINATION bin
            # shared libraries
            LIBRARY DESTINATION lib
            # for static libraries
            ARCHIVE DESTINATION lib
            # public headers
            INCLUDES DESTINATION include
    )
    install(
        DIRECTORY ../data/
        DESTINATION share/verovio
        FILES_MATCHING PATTERN "*.xml" PATTERN "*.svg" PATTERN "*.css" PATTERN "*.json"
    )
endif()

# install all headers in /usr/local/include/verovio
if (BUILD_AS_LIBRARY)
    install(FILES ${all_HEADERS} DESTINATION include/verovio)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/verovio.pc DESTINATION lib/pkgconfig)
endif()
