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

cd ${OUTPUT_DIRECTORY}
git checkout ${UPDATE_TOOLKIT_BRANCH}

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
# make sure that we are on the correct branch
git checkout ${UPDATE_TOOLKIT_BRANCH}
git pull origin ${UPDATE_TOOLKIT_BRANCH} --verbose

echo "Pushing commits"
# Now that we're all set up, we can push latest changes.
git push origin ${UPDATE_TOOLKIT_BRANCH}
