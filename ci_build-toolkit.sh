#!/usr/bin/env bash

# This script builds the JavaScript toolkit and automatically commits
# It uses an encrypted GH_TOKEN setting in Travis to check out the latest version,
# build the toolkit, commit the changes, and then push.

set -ev # Exit with nonzero exit code if anything fails

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
cd ${EMSCRIPTEN_DIRECTORY}
./emsdk activate latest
source ./emsdk_env.sh
cd ..

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

# add new files to git stage of gh-pages directory
cd ${GH_PAGES_DIRECTORY}

# Record intention to add files later to commit. An entry for the path is placed in the index with no content.
echo "git add --intent-to-add"
git add --intent-to-add

echo "git status"
git status

# Return to the root
cd ..
