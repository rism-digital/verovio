#!/usr/bin/env bash

# This script builds the JavaScript toolkit and automatically commits
# It uses an encrypted GH_TOKEN setting in Travis to check out the latest versoin,
# build the toolkit, commit the changes, and then push.

set -e # Exit with nonzero exit code if anything fails

if [ "${TRAVIS_BRANCH}" != "travis-test" ]; then
    echo "Will not build JavaScript toolkit for branch ${TRAVIS_BRANCH}"
    exit 0
fi

BUILDFLAG="$1"
echo BUILDFLAG

if [BUILDFLAG == "woh"]; then
    echo "Building toolkit without humdrum"
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

if [BUILDFLAG == "woh"]; then
    echo "Building toolkit without humdrum"
    ./buildToolkit -c -H
    cp build/verovio-toolkit.js* $OUTPUT_DIRECTORY/javascript/develop/
fi

if [BUILDFLAG == "wohl"]; then
    echo "Building toolkit without humdrum as light version"
    ./buildToolkit -c -H -l
    cp build/verovio-toolkit-light.js* $OUTPUT_DIRECTORY/javascript/develop/
fi

if [BUILDFLAG == "wohw"]; then
    echo "Building toolkit without humdrum as wasm"
    ./buildToolkit -c -H -w
    cp build/verovio*wasm* $OUTPUT_DIRECTORY/javascript/develop/
fi

if [BUILDFLAG == "default"]; then
    echo "Building default toolkit (with humdrum)"
    ./buildToolkit -c
    cp build/*-hum.js* $OUTPUT_DIRECTORY/javascript/develop/
fi


# Return to the root
cd ..

# 
cd ${GH_PAGES_DIRECTORY}

echo "Configuring git push"
git config user.name "JavaScript toolkit builder"
git config user.email "${COMMIT_AUTHOR_EMAIL}"

git status

git add -A
git commit -m "Auto-commit of toolkit build for rism-ch/verovio@${SHA}"

echo "Syncing from origin..."
git pull

echo "Pushing commits"
# Now that we're all set up, we can push.
# git push ${GH_PAGES_REPOSITORY} travis-test
