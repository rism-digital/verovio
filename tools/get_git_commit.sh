#!/usr/bin/env sh

cd ..
output="./include/vrv/git_commit.h"
COMMIT=$(git describe --abbrev=7 --always --dirty)

if [ -z "$COMMIT" ]; then
    echo "Undefined git commit version"
    COMMIT="[undefined]"
fi

echo "////////////////////////////////////////////////////////" > $output
echo "/// Git commit version file generated at compilation ///" >> $output
echo "////////////////////////////////////////////////////////" >> $output
echo "" >> $output
echo "#define GIT_COMMIT \"$COMMIT\"" >> $output
echo "" >> $output
