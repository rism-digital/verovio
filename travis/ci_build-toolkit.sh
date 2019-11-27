#!/usr/bin/env bash

# This script builds the JavaScript toolkit and automatically commits
# It uses an encrypted GH_TOKEN setting in Travis to check out the latest version,
# build the toolkit, commit the changes, and then push.

set -ev # -e: Exit with nonzero exit code if anything fails
        # -v: verbose mode; print every command in travis output

if [ "${TRAVIS_BRANCH}" != "${BUILD_BRANCH}" ]; then
    echo "Will not build JavaScript toolkit for branch ${TRAVIS_BRANCH}"
    exit 1
fi

if [ "${TRAVIS_PULL_REQUEST}" != "false" ]; then
    echo "Will not build JavaScript toolkit for pull requests. Skipping it."
    exit 0
fi

if [ -z "$1" ]; then
    echo "No argument for BUILDTARGET supplied"
    exit 1
else
  BUILDTARGET="$1"
fi


###############
# Build toolkit
###############

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


##################################
# Push changes to temporary output
##################################

cd ${OUTPUT_DIRECTORY}
git checkout ${TEMPORARY_OUTPUT_BRANCH}

echo "Configuring git push"
git config user.name "JavaScript toolkit builder"
git config user.email "${COMMIT_AUTHOR_EMAIL}"

echo "Running git add"
git add -A

echo "Running git commit"
git commit -m "Auto-commit of ${BUILDTARGET} toolkit build for rism-ch/verovio@${SHA}"

echo "Syncing from origin..."
# clean-up
git fetch origin --prune
# make sure that we are on the correct branch (temporary output)
git checkout ${TEMPORARY_OUTPUT_BRANCH}
git pull origin ${TEMPORARY_OUTPUT_BRANCH} --verbose

echo "Pushing commits"
# Now that we're all set up, we can push latest changes to temporary output
git push origin ${TEMPORARY_OUTPUT_BRANCH}
