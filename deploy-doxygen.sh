#!/usr/bin/env bash

# This script builds the doxygen documentation and automatically commits
# It uses an encrypted GH_TOKEN setting in Travis to check out the latest version,
# build the doc, commit the changes, and then push.

set -e # Exit with nonzero exit code if anything fails

if [ "${TRAVIS_BRANCH}" != "develop" ]; then
    echo "Will not build doxygen documentation for branch ${TRAVIS_BRANCH}"
    exit 0
fi

SHA=$(git rev-parse --verify HEAD)

DOXYGEN_REPOSITORY="https://${GH_TOKEN}@github.com/rism-ch/verovio-doxygen"
DOXYGEN_DIRECTORY="doxygen"

# Clone the doxygen repo.
echo "Cloning ${DOXYGEN_REPOSITORY}"
git clone ${DOXYGEN_REPOSITORY} ${DOXYGEN_DIRECTORY}


CURRENT_PATH=$(pwd)
OUTPUT_DIRECTORY="${CURRENT_PATH}/${DOXYGEN_DIRECTORY}"

cd ./doc
(cat verovio.conf ; echo "OUTPUT_DIRECTORY = ${OUTPUT_DIRECTORY}") | doxygen -

# Return to the root
cd ..

#
cd ${DOXYGEN_DIRECTORY}

echo "Configuring git push"
git config user.name "Documentation deployment"
git config user.email "${COMMIT_AUTHOR_EMAIL}"

git status

git add -A
git commit -m "Auto-commit of documentation build for rism-ch/verovio-doxygen@${SHA}"

echo "Syncing from origin..."
git pull

echo "Pushing commits"
# Now that we're all set up, we can push.
# git push ${DOXYGEN_REPOSITORY} master
