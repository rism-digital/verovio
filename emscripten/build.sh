#/bin/bash

VEROVIO_ROOT=../
VEROVIO_INCLUDE=../include/vrv
EMCC=`which emcc`

if [ ! -d build ]; then mkdir build; fi

if [ ! -d data ]; then mkdir data; fi

echo "Sync svg resources"
cp -r ../data/svg data/

echo "Compliling"
# we use ASM_JS=0 because otherwise it does not work with FireFox
# memory is increased (TOTAL_MEMORY and TOTAL_STACK) for processing large files (tested up to 7MB)

python $EMCC --closure 0 -O2 \
	-s ASM_JS=1 \
	-s OUTLINING_LIMIT=160000 \
	-I./lib/jsonxx \
	-I$VEROVIO_INCLUDE \
	-I$VEROVIO_ROOT/tinyxml \
	-DUSE_EMSCRIPTEN \
	-s TOTAL_MEMORY=256*1024*1024 \
	-s TOTAL_STACK=16*1024*1024 \
	./emscripten_main.cpp \
	$VEROVIO_ROOT/src/vrv.cpp \
	$VEROVIO_ROOT/src/aligner.cpp \
	$VEROVIO_ROOT/src/app.cpp \
	$VEROVIO_ROOT/src/barline.cpp \
	$VEROVIO_ROOT/src/measure.cpp \
	$VEROVIO_ROOT/src/bboxdc.cpp \
	$VEROVIO_ROOT/src/beam.cpp \
	$VEROVIO_ROOT/src/clef.cpp \
	$VEROVIO_ROOT/src/interfacecontroller.cpp \
	$VEROVIO_ROOT/src/devicecontext.cpp \
	$VEROVIO_ROOT/src/doc.cpp \
	$VEROVIO_ROOT/src/durationinterface.cpp \
	$VEROVIO_ROOT/src/io.cpp \
	$VEROVIO_ROOT/src/iodarms.cpp \
	$VEROVIO_ROOT/src/iomei.cpp \
	$VEROVIO_ROOT/src/iomusxml.cpp \
	$VEROVIO_ROOT/src/iopae.cpp \
	$VEROVIO_ROOT/src/keysig.cpp \
	$VEROVIO_ROOT/src/layer.cpp \
	$VEROVIO_ROOT/src/layerelement.cpp \
	$VEROVIO_ROOT/src/leipzigbbox.cpp \
	$VEROVIO_ROOT/src/mensur.cpp \
	$VEROVIO_ROOT/src/mrest.cpp \
	$VEROVIO_ROOT/src/multirest.cpp \
	$VEROVIO_ROOT/src/note.cpp \
	$VEROVIO_ROOT/src/object.cpp \
	$VEROVIO_ROOT/src/page.cpp \
	$VEROVIO_ROOT/src/pitchinterface.cpp \
	$VEROVIO_ROOT/src/positioninterface.cpp \
	$VEROVIO_ROOT/src/view.cpp \
	$VEROVIO_ROOT/src/view_beam.cpp \
	$VEROVIO_ROOT/src/view_bezier.cpp \
	$VEROVIO_ROOT/src/view_element.cpp \
	$VEROVIO_ROOT/src/view_graph.cpp \
	$VEROVIO_ROOT/src/view_page.cpp \
	$VEROVIO_ROOT/src/view_tuplet.cpp \
	$VEROVIO_ROOT/src/rest.cpp \
	$VEROVIO_ROOT/src/scoredef.cpp \
	$VEROVIO_ROOT/src/slur.cpp \
	$VEROVIO_ROOT/src/staff.cpp \
	$VEROVIO_ROOT/src/svgdc.cpp \
	$VEROVIO_ROOT/src/symbol.cpp \
	$VEROVIO_ROOT/src/system.cpp \
	$VEROVIO_ROOT/src/tie.cpp \
	$VEROVIO_ROOT/src/tuplet.cpp \
	$VEROVIO_ROOT/tinyxml/tinystr.cpp \
	$VEROVIO_ROOT/tinyxml/tinyxml.cpp \
	$VEROVIO_ROOT/tinyxml/tinyxmlerror.cpp \
	$VEROVIO_ROOT/tinyxml/tinyxmlparser.cpp \
	$VEROVIO_ROOT/src/pugixml.cpp \
	lib/jsonxx/jsonxx.cc \
	--embed-file data/svg/ \
	-s EXPORTED_FUNCTIONS="['_convertMusic']" \
    -o build/verovio.js

if [ $? -eq 0 ]; then echo "Done."; fi

# Unused

#	lib/regex/regcomp.c \
#	lib/regex/regexec.c \
#	lib/regex/regfree.c \
#	lib/regex/regerror.c \

#/Users/xhero/Downloads/e2fsprogs-1.42.8/lib/uuid/result.bc \
#--embed-file data/svg/ \
#	llvm_regexp/regcomp.c \
#	llvm_regexp/regexec.c \
#	llvm_regexp/regfree.c \
#	llvm_regexp/regerror.c \
#	llvm_regexp/regstrlcpy.c \

#	apple_regex/regcomp.c \
#	apple_regex/regexec.c \
#	apple_regex/regfree.c \
#	apple_regex/regerror.c \
