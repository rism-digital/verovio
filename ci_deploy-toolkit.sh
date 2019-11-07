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

if [ -z "$1" ]; then
    echo "No argument for BUILDTARGET supplied"
    exit 1
else
  BUILDTARGET="$1"
fi

cd ${GH_PAGES_DIRECTORY}

echo "Configuring git push"
git config user.name "JavaScript toolkit builder"
git config user.email "${COMMIT_AUTHOR_EMAIL}"

echo "Running git remote show origin"
git remote show origin

echo "Running git add"
git add -A

echo "Running git status"
git status

echo "Get the rism-ch revision"
SHA=$(git rev-parse --verify HEAD)

echo "Running git commit"
git commit -m "Auto-commit of ${BUILDTARGET} toolkit build for rism-ch/verovio@${SHA}"

echo "Syncing from origin..."
git pull --verbose

echo "Pushing commits"
# Now that we're all set up, we can push.
git push ${VEROVIO_REPOSITORY} ${GH_PAGES_DIRECTORY}
