#!/usr/bin/env bash

function print_help {
	 echo "Usage:
-l		Light version with no increased memory allocation
-w 		WebWorker-compatible build.
-v N	Version number (e.g., 1.0.0); no number by default
-c		Turns on \"Chatty\" compiling; Will print the compiler progress" >&2 ; 
} 

VEROVIO_ROOT=../
VEROVIO_INCLUDE=../include
VEROVIO_INCLUDE_VRV=../include/vrv
VEROVIO_LIBMEI=../libmei
if command -v emcc >/dev/null 2>&1 ; then
	EMCC=`command -v emcc`
else
	echo >&2 "I require the emscripten compiler (emcc) but it's not installed.  Aborting.";
	exit 1;
fi

if [ ! -d build ]; then mkdir build; fi

if [ ! -d data ]; then mkdir data; fi

# Default is with ASM an large file support.
# Memory is increased (TOTAL_STACK) for processing large files (tested up to 7MB)
# Empirically, the memory amount required is approx. 5 times the file size (as an indication).
# We can disable this for a light version that uses the default memory settings 	
ASM="\
	-O2 --memory-init-file 0 \
	-s ASM_JS=1 \
	-s TOTAL_MEMORY=128*1024*1024 \
	-s TOTAL_STACK=64*1024*1024"
ASM_NAME=""
WEBWORKER_NAME=""

# default is master (no version)
VERSION=""
VERSION_NAME=""
CHATTY=""
WEBWORKER=false

# generate the git commit file
../tools/get_git_commit.sh 

while getopts "lwv:h:c" opt; do
	case $opt in
		l)
			echo "light version (-l)"
			ASM="\
				-O2 --memory-init-file 0 \
				-s ASM_JS=1 "
			ASM_NAME="-light"
			;;
		v)
			echo "version (-v) $OPTARG"
			VERSION="$OPTARG"
			VERSION_NAME="-$VERSION"
			;;
		c)
			CHATTY="-v"
			echo $EMCC
			;;
		w)
			WEBWORKER=true
			echo "building with webworker compatibility"
			WEBWORKER_NAME="-webworker"
			;;
		h)
			print_help
			exit 2
			;;
		\?)
			print_help
			exit 2
			;;
	esac
done

FILENAME="verovio-toolkit$ASM_NAME$WEBWORKER_NAME$VERSION_NAME.js"

echo "Sync svg resources"
cp -r ../data/* data/

echo "Compiling"

python $EMCC $CHATTY \
	-I./lib/jsonxx \
	-I$VEROVIO_INCLUDE \
	-I$VEROVIO_INCLUDE_VRV \
	-I$VEROVIO_LIBMEI \
	-DUSE_EMSCRIPTEN \
	$ASM \
	./emscripten_main.cpp \
	$VEROVIO_ROOT/src/vrv.cpp \
	$VEROVIO_ROOT/src/accid.cpp \
	$VEROVIO_ROOT/src/aligner.cpp \
	$VEROVIO_ROOT/src/att.cpp \
	$VEROVIO_ROOT/src/barline.cpp \
	$VEROVIO_ROOT/src/bboxdevicecontext.cpp \
	$VEROVIO_ROOT/src/beam.cpp \
	$VEROVIO_ROOT/src/chord.cpp \
	$VEROVIO_ROOT/src/clef.cpp \
	$VEROVIO_ROOT/src/custos.cpp \
	$VEROVIO_ROOT/src/devicecontext.cpp \
	$VEROVIO_ROOT/src/doc.cpp \
	$VEROVIO_ROOT/src/dot.cpp \
	$VEROVIO_ROOT/src/drawinglistinterface.cpp \
	$VEROVIO_ROOT/src/durationinterface.cpp \
	$VEROVIO_ROOT/src/editorial.cpp \
	$VEROVIO_ROOT/src/glyph.cpp \
	$VEROVIO_ROOT/src/io.cpp \
	$VEROVIO_ROOT/src/iodarms.cpp \
	$VEROVIO_ROOT/src/iomei.cpp \
	$VEROVIO_ROOT/src/iomusxml.cpp \
	$VEROVIO_ROOT/src/iopae.cpp \
	$VEROVIO_ROOT/src/keysig.cpp \
	$VEROVIO_ROOT/src/layer.cpp \
	$VEROVIO_ROOT/src/layerelement.cpp \
	$VEROVIO_ROOT/src/measure.cpp \
	$VEROVIO_ROOT/src/measureelement.cpp \
	$VEROVIO_ROOT/src/mensur.cpp \
	$VEROVIO_ROOT/src/metersig.cpp \
	$VEROVIO_ROOT/src/mrest.cpp \
	$VEROVIO_ROOT/src/multirest.cpp \
	$VEROVIO_ROOT/src/note.cpp \
	$VEROVIO_ROOT/src/object.cpp \
	$VEROVIO_ROOT/src/page.cpp \
	$VEROVIO_ROOT/src/pitchinterface.cpp \
	$VEROVIO_ROOT/src/positioninterface.cpp \
	$VEROVIO_ROOT/src/view.cpp \
	$VEROVIO_ROOT/src/view_beam.cpp \
	$VEROVIO_ROOT/src/view_element.cpp \
	$VEROVIO_ROOT/src/view_graph.cpp \
	$VEROVIO_ROOT/src/view_mensural.cpp \
	$VEROVIO_ROOT/src/view_page.cpp \
	$VEROVIO_ROOT/src/view_tuplet.cpp \
	$VEROVIO_ROOT/src/rest.cpp \
	$VEROVIO_ROOT/src/scoredef.cpp \
	$VEROVIO_ROOT/src/slur.cpp \
	$VEROVIO_ROOT/src/space.cpp \
	$VEROVIO_ROOT/src/staff.cpp \
	$VEROVIO_ROOT/src/style.cpp \
	$VEROVIO_ROOT/src/svgdevicecontext.cpp \
	$VEROVIO_ROOT/src/syl.cpp \
    $VEROVIO_ROOT/src/system.cpp \
	$VEROVIO_ROOT/src/tie.cpp \
	$VEROVIO_ROOT/src/timeinterface.cpp \
	$VEROVIO_ROOT/src/toolkit.cpp \
	$VEROVIO_ROOT/src/tuplet.cpp \
	$VEROVIO_ROOT/src/verse.cpp \
	$VEROVIO_ROOT/src/pugixml.cpp \
	$VEROVIO_ROOT/libmei/atts_cmn.cpp \
	$VEROVIO_ROOT/libmei/atts_critapp.cpp \
	$VEROVIO_ROOT/libmei/atts_mensural.cpp \
	$VEROVIO_ROOT/libmei/atts_shared.cpp \
	$VEROVIO_ROOT/libmei/atts_pagebased.cpp \
	lib/jsonxx/jsonxx.cc \
	--embed-file data/ \
	-s EXPORTED_FUNCTIONS="[\
		'_vrvToolkit_constructor',\
		'_vrvToolkit_destructor',\
		'_vrvToolkit_getLog',\
		'_vrvToolkit_getVersion',\
		'_vrvToolkit_getMEI',\
		'_vrvToolkit_getPageCount',\
		'_vrvToolkit_getPageWithElement',\
		'_vrvToolkit_loadData',\
		'_vrvToolkit_redoLayout',\
		'_vrvToolkit_renderData',\
		'_vrvToolkit_renderPage',\
		'_vrvToolkit_setOptions',\
		'_vrvToolkit_edit',\
		'_vrvToolkit_getElementAttr']" \
	-o build/verovio.js

if [ $? -eq 0 ]; then 
	echo "Done."
	# the wrapper is necessary with closure 1 for avoiding to conflict with globals
	if [ "$WEBWORKER" = true ]; then
		cat build/verovio.js verovio-proxy.js > "build/$FILENAME"
	else
		cat verovio-wrapper-start.js build/verovio.js verovio-wrapper-end.js verovio-proxy.js verovio-unload-listener.js > "build/$FILENAME"
	fi
	# all good
	echo "build/$FILENAME written"
	# create also a zip file if version name is given
	if [ -n "$VERSION_NAME" ]; then
		cd "build"
		zip "$FILENAME.zip" "$FILENAME"
		cd ..
		echo "build/$FILENAME.zip written"
	fi
fi
