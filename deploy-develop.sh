#!/usr/bin/env bash

# This script builds the JavaScript toolkit and automatically commits
# It uses an encrypted GH_TOKEN setting in Travis to check out the Guidelines, build them, commit
# the changes, and then push.

set -e # Exit with nonzero exit code if anything fails

if [ "${TRAVIS_BRANCH}" != "develop" ]; then
    echo "Will not build JavaScript toolkit for branch ${TRAVIS_BRANCH}"
    exit 0
fi

# Get the music-encoding revision
SHA=`git rev-parse --verify HEAD`
SHORT_SHA=`git rev-parse --verify --short HEAD`

GH_PAGES_REPOSITORY="https://${GH_USERNAME}:${GH_TOKEN}@github.com/rism-ch/verovio"
GH_PAGES_DIRECTORY="gh-pages"

# Clone the docs repo.
echo "Cloning ${GH_PAGES_REPOSITORY}"
git clone --single-branch --branch gh-pages ${GH_PAGES_REPOSITORY} ${GH_PAGES_DIRECTORY}


CURRENT_PATH=`pwd`
OUTPUT_DIRECTORY="${CURRENT_PATH}/${GH_PAGES_DIRECTORY}"

ls $OUTPUT_DIRECTORY

cd ./tools
#cmake .
#make
#./verovio -? > $OUTPUT_DIRECTORY/_includes/cli.txt
cd ..

cd ./emscripten

./buildToolkit -c -H
cp build/verovio-toolkit.js* $OUTPUT_DIRECTORY/javascript/develop/

#./buildToolkit -c -H -l
#cp build/verovio-toolkit-light.js* $OUTPUT_DIRECTORY/javascript/develop/

#./buildToolkit -c -H -w
#cp build/verovio*wasm* $OUTPUT_DIRECTORY/javascript/develop/

#./buildToolkit -c
#cp build/*-hum.js* $OUTPUT_DIRECTORY/javascript/develop/

# Return to the root
cd ..

# 
cd ${GH_PAGES_DIRECTORY}

echo "Configuring git push"
git config user.name "Documentation Builder"
git config user.email "${COMMIT_AUTHOR_EMAIL}"

git status

git add -A
git commit -m "Auto-commit of documentation build for music-encoding/music-encoding@${SHA}"

echo "Syncing from origin..."
git pull

echo "Pushing commits"
# Now that we're all set up, we can push.
# git push ${DOCS_REPOSITORY} ${DOCS_BRANCH}
