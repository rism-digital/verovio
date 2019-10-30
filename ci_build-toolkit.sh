#!/usr/bin/env bash

# This script builds the JavaScript toolkit and automatically commits
# It uses an encrypted GH_TOKEN setting in Travis to check out the latest version,
# build the toolkit, commit the changes, and then push.

set -e # Exit with nonzero exit code if anything fails

if [ "${TRAVIS_BRANCH}" != "${BUILD_BRANCH}" ]; then
    echo "Will not build JavaScript toolkit for branch ${TRAVIS_BRANCH}"
    exit 1
fi

if [ -z "$1" ]; then
    echo "No argument for BUILDTARGET supplied"
    exit 1
else
  BUILDTARGET="$1"
  echo "$BUILDTARGET" # TODO: remove
fi

# activate and source emscripten tools
./emsdk/emsdk activate latest
source ./emsdk/emsdk_env.sh

cd ./emscripten

# build toolkit depending on build target
if [[ "$BUILDTARGET" == nohumdrum ]]; then
    echo "Building toolkit without humdrum"
    ./buildToolkit -c -H
    cp build/verovio-toolkit.js* $OUTPUT_DIRECTORY/javascript/develop/

elif [[ "$BUILDTARGET" == light ]]; then
    echo "Building toolkit without humdrum as light version"
    ./buildToolkit -c -H -l
    cp build/verovio-toolkit-light.js* $OUTPUT_DIRECTORY/javascript/develop/

elif [[ "$BUILDTARGET" == wasm ]]; then
    echo "Building toolkit without humdrum as wasm"
    ./buildToolkit -c -H -w
    cp build/verovio*wasm* $OUTPUT_DIRECTORY/javascript/develop/

elif [[ "$BUILDTARGET" == default ]]; then
    echo "Building default toolkit (with humdrum)"
    ./buildToolkit -c
    cp build/*-hum.js* $OUTPUT_DIRECTORY/javascript/develop/

else
  echo "No BUILDTARGET matched: $BUILDTARGET"
  exit 1
fi

# Return to the root
cd ..
