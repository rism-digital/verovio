#!/usr/bin/env bash

# This script builds the JavaScript toolkit and automatically commits
# It uses an encrypted GH_TOKEN setting in Travis to check out the latest versoin,
# build the toolkit, commit the changes, and then push.

set -ev # Exit with nonzero exit code if anything fails

if [ "${TRAVIS_BRANCH}" != "${BUILD_BRANCH}" ]; then
    echo "Will not build JavaScript toolkit for branch ${TRAVIS_BRANCH}"
    exit 1
fi

# TODO: remove
echo "Folder structure"
ls -alh

# Get the rism-ch revision
SHA=$(git rev-parse --verify HEAD)

cd ${GH_PAGES_DIRECTORY}

echo "Configuring git push"
git config user.name "JavaScript toolkit builder"
git config user.email "${COMMIT_AUTHOR_EMAIL}"

echo "Running git status"
git status

git add -A

echo "Running git status"
git status

echo "Running git commit"
git commit -m "Auto-commit of toolkit build for rism-ch/verovio@${SHA}"

git diff --cached  ## TODO: remove

echo "Syncing from origin..."
git pull

echo "Pushing commits"
# Now that we're all set up, we can push.
# git push ${GH_PAGES_REPOSITORY} develop
