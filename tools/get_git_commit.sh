#!/usr/bin/env sh

cd ..

output="./include/vrv/git_commit.h"
COMMIT=""

if [ ! -d "./.git" ]; then
     echo "This is not a git directory and the git commit sha will remain undefined"
else
    SHA=$(git describe  --exclude '*' --abbrev=7 --always --dirty)
    if [ -z "$SHA" ]; then
        echo "Undefined git commit version"
    else
        COMMIT="-$SHA"
    fi
fi

echo "////////////////////////////////////////////////////////" > $output
echo "/// Git commit version file generated at compilation ///" >> $output
echo "////////////////////////////////////////////////////////" >> $output
echo "" >> $output
echo "#define GIT_COMMIT \"$COMMIT\"" >> $output
echo "" >> $output
