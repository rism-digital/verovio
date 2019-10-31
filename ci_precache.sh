#!/usr/bin/env bash

# This script builds the JavaScript toolkit and automatically commits
# It uses an encrypted GH_TOKEN setting in Travis to check out the latest versoin,
# build the toolkit, commit the changes, and then push.

set -ev # Exit with nonzero exit code if anything fails

if [ "${TRAVIS_BRANCH}" != "${BUILD_BRANCH}" ]; then
    echo "Will not build JavaScript toolkit for branch ${TRAVIS_BRANCH}"
    exit 1
fi

# remove empty emsdk folder created by travis cache before
rm -rf $EMSCRIPTEN_DIRECTORY
# install emscripten
echo "Cloning emscripten"
git clone $EMSCRIPTEN_REPOSITORY $EMSCRIPTEN_DIRECTORY
cd $EMSCRIPTEN_DIRECTORY
./emsdk install latest
cd ..

# copy gh-pages of verovio repository
echo "Cloning ${VEROVIO_REPOSITORY} into ${GH_PAGES_DIRECTORY}"
rm -rf $GH_PAGES_DIRECTORY
git clone --single-branch --branch gh-pages ${VEROVIO_REPOSITORY} ${GH_PAGES_DIRECTORY}

# make build
cd ./tools
cmake .
make -j3
echo "Update the documentation of the option list"
./verovio -? > $OUTPUT_DIRECTORY/_includes/cli.txt
cd ..

# add new files to git stage of gh-pages directory
cd ${GH_PAGES_DIRECTORY}

# Record intention to add files later to commit. An entry for the path is placed in the index with no content.
echo "git add"
git add -A

echo "git status"
git status

# Return to the root
cd ..
