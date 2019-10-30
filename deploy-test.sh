#!/usr/bin/env bash

# This script builds the JavaScript toolkit and automatically commits
# It uses an encrypted GH_TOKEN setting in Travis to check out the latest versoin,
# build the toolkit, commit the changes, and then push.

set -e # Exit with nonzero exit code if anything fails

if [ "${TRAVIS_BRANCH}" != "travis-test" ]; then
    echo "Will not build JavaScript toolkit for branch ${TRAVIS_BRANCH}"
    exit 1
fi

if [ -z "$1" ]; then
    echo "No argument for BUILDTARGET supplied"
    exit 1
fi

BUILDTARGET="$1"
echo "1: $1"
echo "BUILDTARGET: $BUILDTARGET"
echo "{BUILDTARGET}: ${BUILDTARGET}"

if [[ "$BUILDTARGET" == nohumdrum ]]; then
    echo "Building toolkit without humdrum"

elif [[ "$BUILDTARGET" == light ]]; then
    echo "Building toolkit without humdrum as light version"

elif  [[ "$BUILDTARGET" == wasm ]]; then
    echo "Building toolkit without humdrum as wasm"

elif [[ "$BUILDTARGET" == default ]]; then
    echo "Building default toolkit (with humdrum)"

else
  echo "No BUILDTARGET matched: $BUILDTARGET"
fi
