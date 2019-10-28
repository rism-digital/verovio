#!/usr/bin/env bash

# This script builds the JavaScript toolkit and automatically commits
# It uses an encrypted GH_TOKEN setting in Travis to check out the latest versoin,
# build the toolkit, commit the changes, and then push.

set -e # Exit with nonzero exit code if anything fails

if [ "${TRAVIS_BRANCH}" != "travis-test" ]; then
    echo "Will not build JavaScript toolkit for branch ${TRAVIS_BRANCH}"
    exit 0
fi

# Get the rism-ch revision
SHA=`git rev-parse --verify HEAD`

GH_PAGES_REPOSITORY="https://${GH_TOKEN}@github.com/rism-ch/verovio"
GH_PAGES_DIRECTORY="gh-pages"

# Clone the gh-pages repo.
echo "Cloning ${GH_PAGES_REPOSITORY}"
git clone --single-branch --branch gh-pages ${GH_PAGES_REPOSITORY} ${GH_PAGES_DIRECTORY}


CURRENT_PATH=`pwd`
OUTPUT_DIRECTORY="${CURRENT_PATH}/${GH_PAGES_DIRECTORY}"

cd ./tools
cmake .
make
echo "Update the documentation of the option list"
./verovio -? > $OUTPUT_DIRECTORY/_includes/cli.txt
cd ..

cd ./emscripten

echo "Building toolkit without humdrum as wasm"
./buildToolkit -c -H -w
cp build/verovio*wasm* $OUTPUT_DIRECTORY/javascript/develop/

# Return to the root
cd ..

echo "Back to root"
echo "Configuring git"

cd $OUTPUT_DIRECTORY/javascript/develop/
ls -alh
