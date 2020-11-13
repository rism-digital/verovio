#!/bin/bash

# This script need to be run from ./doc in the branch with the changes
# add parameter true to rebuild the develop (reference) branch and tests

# The path to the directory of tests in verovio.org
testdir="/Users/laurent/Documents/verovio/gh-pages/_tests"
# The path to the directory for the output of the develop (reference) branch
indir1="/Users/laurent/tmp/test-output-dev"
# The path to the directory for the output of the branch with the changes (PR)
indir2="/Users/laurent/tmp/test-output-pr"
# The path to the directory where the diff results will be written
outdir="/Users/laurent/tmp/test-output-diff"
# The path to the directory containing the develop branch - used only for rebuilding
devdir="/Users/laurent/projects/verovio_lpugin"

# The version of python we want to use
PYTHON=python3.8

# Store the path where we are
home=`pwd`

# Check if we need to rebuild the develop (reference) branch and tests
build_dev=$1
if [ ! -z $build_dev ]; then
    cd $devdir
    git pull
    cd bindings
    cmake ../cmake -B python -DBUILD_AS_PYTHON=ON -DNO_HUMBRUM_SUPPORT=ON
    cd python
    make -j8

    $PYTHON ../../doc/test-suite.py $testdir $indir1
    ../../doc/test-suite-to-png.sh $indir1

    cd $home
fi

cd ../bindings
cmake ../cmake -B python -DBUILD_AS_PYTHON=ON -DNO_HUMBRUM_SUPPORT=ON
cd python
make -j8

$PYTHON ../../doc/test-suite.py $testdir $indir2
../../doc/test-suite-to-png.sh $indir2

$PYTHON ../../doc/test-suite-diff.py $indir2 $indir1 $outdir

open $outdir/index.html
