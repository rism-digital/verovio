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

# Get the rism-ch revision
SHA=$(git rev-parse --verify HEAD)

cd ${GH_PAGES_DIRECTORY}

echo "Configuring git push"
git config user.name "JavaScript toolkit builder"
git config user.email "${COMMIT_AUTHOR_EMAIL}"

git add -A

echo "Running git status"
git status

echo "Running git commit"
git commit -m "Auto-commit of toolkit build for rism-ch/verovio@${SHA}"

git remote show origin
git remote show https://${GH_TOKEN}@github.com/musicEnfanthen/verovio   # TODO: remove this in production

echo "Syncing from origin..."
# git pull --verbose  # TODO: use this in production
git pull https://${GH_TOKEN}@github.com/musicEnfanthen/verovio --verbose

echo "Pushing commits"
# Now that we're all set up, we can push.
# git push ${VEROVIO_REPOSITORY} ${GH_PAGES_DIRECTORY}  # TODO: use this in production
git push https://${GH_TOKEN}@github.com/musicEnfanthen/verovio ${GH_PAGES_DIRECTORY}
