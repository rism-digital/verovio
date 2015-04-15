#!/usr/bin/env bash

cd ..
output="./include/vrv/git_commit.h"
COMMIT=`git describe --abbrev=7 --always --dirty`

echo "////////////////////////////////////////////////////////" > $output
echo "/// Git commit version file generated at compilation ///" >> $output
echo "////////////////////////////////////////////////////////" >> $output
echo "" >> $output
echo "#define GIT_COMMIT \"$COMMIT\"" >> $output
echo "" >> $output
