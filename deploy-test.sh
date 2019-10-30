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
echo "1: $1"
echo "BUILDFLAG: $BUILDFLAG"
echo "{BUILDFLAG}: ${BUILDFLAG}"

if ["$BUILDFLAG" == "woh"]; then
    echo "Building toolkit without humdrum"

elif ["$BUILDFLAG" == "wohl"]; then
    echo "Building toolkit without humdrum as light version"

elif  ["$BUILDFLAG" == "wohw"]; then
    echo "Building toolkit without humdrum as wasm"

elif ["$BUILDFLAG" == "default"]; then
    echo "Building default toolkit (with humdrum)"

else
  echo "No buildflag matched: $BUILDFLAG"
fi
