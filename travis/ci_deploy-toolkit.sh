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

if [ "${TRAVIS_PULL_REQUEST}" != "false" ]; then
    echo "Will not build JavaScript toolkit for pull requests. Skipping it."
    exit 0
fi

cd ${OUTPUT_DIRECTORY}
git checkout ${GH_PAGES_BRANCH}

echo "Configuring git push"
git config user.name "JavaScript toolkit builder"
git config user.email "${COMMIT_AUTHOR_EMAIL}"

echo "Syncing from origin..."
# clean-up
git fetch origin --prune
# make sure that we are on the correct branch (gh-pages=
git checkout ${GH_PAGES_BRANCH}
git pull origin ${GH_PAGES_BRANCH} --verbose

# Now that we're all set up, we can squash merge temporary output into gh-pages
# "squash merge" reduces all temporary commits to a single one
echo "Squash merge commits from ${TEMPORARY_OUTPUT_BRANCH} into ${GH_PAGES_BRANCH}"
git merge --squash ${TEMPORARY_OUTPUT_BRANCH}

echo "Running git commit"
git commit -m "Auto-commit of toolkit build for rism-ch/verovio@${SHA}" # TODO: include build-branch in commit message?

echo "Running git status"
git status

# Push all changes in one commit to the gh-pages branch
echo "Pushing final commit"
git push origin ${GH_PAGES_BRANCH}

# After all, it is safe to delete the temporary output branch locally and on remote
echo "Delete ${TEMPORARY_OUTPUT_BRANCH}"
git branch -D ${TEMPORARY_OUTPUT_BRANCH}
git push origin :${TEMPORARY_OUTPUT_BRANCH}
