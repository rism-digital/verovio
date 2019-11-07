#!/usr/bin/env bash

# This script builds the JavaScript toolkit and automatically commits
# It uses an encrypted GH_TOKEN setting in Travis to check out the latest versoin,
# build the toolkit, commit the changes, and then push.

set -ev # -e: Exit with nonzero exit code if anything fails
        # -v: verbose mode; print every command in travis output

if [ "${TRAVIS_BRANCH}" != "${BUILD_BRANCH}" ]; then
    echo "Will not build JavaScript toolkit for branch ${TRAVIS_BRANCH}"
    exit 1
fi

#####################
# install emscripten
#####################
echo "Remove empty ${EMSCRIPTEN_DIRECTORY} folder created by travis cache before"
rm -rf $EMSCRIPTEN_DIRECTORY

echo "Cloning emscripten"
git clone $EMSCRIPTEN_REPOSITORY $EMSCRIPTEN_DIRECTORY

echo "Installing emscripten"
cd $EMSCRIPTEN_DIRECTORY
./emsdk install latest

# Return to the root
cd ..

#############################################
# copy gh-pages branch of verovio repository
#############################################
echo "Remove empty ${GH_PAGES_DIRECTORY} folder created by travis cache before"
rm -rf $GH_PAGES_DIRECTORY

echo "Cloning branch ${GH_PAGES_BRANCH} of ${VEROVIO_REPOSITORY} into ${OUTPUT_DIRECTORY}"
git clone --single-branch --branch ${GH_PAGES_BRANCH} ${VEROVIO_REPOSITORY} ${OUTPUT_DIRECTORY}

# checkout new branch for toolkit updates
cd ${OUTPUT_DIRECTORY}

echo "Checkout ${TEMPORARY_OUTPUT_BRANCH} branch"
git checkout -b ${TEMPORARY_OUTPUT_BRANCH}

echo "Push new branch to remote and track it"
git push -u origin ${TEMPORARY_OUTPUT_BRANCH}

# Return to the root
cd ..

#############
# make build
#############
cd ./tools

cmake .
make -j3

echo "Update the documentation of the option list"
./verovio -? > $OUTPUT_DIRECTORY/_includes/cli.txt

# Return to the root
cd ..
