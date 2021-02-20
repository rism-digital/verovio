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
# The path to the file containing the shortlist - file can be empty but must be there
shortlist="/Users/laurent/tmp/shortlist.txt"

# The version of python we want to use (we can be more specific here, e.g., python3.8)
PYTHON=python3.9

# Store the path where we are
home=`pwd`

# Ask if we want to empty the files previously generated
read -p "Do you want to empty $indir1 $indir2 and $outdir ? [Y]" -n 1 -r
echo    # move to a new line
if [[ $REPLY =~ ^[Y]$ ]]
then
    echo "Emptying directories ..."
    rm $indir1/*/*.png
    rm $indir1/*/*.svg
    rm $indir2/*/*.png
    rm $indir2/*/*.svg
    rm $outdir/*/*.png
    rm $outdir/index.html
fi

# Check if we need to rebuild the develop (reference) branch and tests
build_dev=$1
if [ ! -z $build_dev ]; then
    cd $devdir
    git pull
    cd bindings
    cmake ../cmake -B python -DBUILD_AS_PYTHON=ON -DNO_HUMDRUM_SUPPORT=ON -DVRV_DYNAMIC_CAST=ON
    cd python
    make -j8

    $PYTHON ../../doc/test-suite.py "$testdir" "$indir1"

    cd $home
fi

cd ../bindings
cmake ../cmake -B python -DBUILD_AS_PYTHON=ON -DNO_HUMDRUM_SUPPORT=ON -DVRV_DYNAMIC_CAST=ON
cd python
make -j8

$PYTHON ../../doc/test-suite.py "$testdir" "$indir2" --shortlist "$shortlist"

$PYTHON ../../doc/test-suite-diff.py "$indir2" "$indir1" "$outdir"

open $outdir/index.html
