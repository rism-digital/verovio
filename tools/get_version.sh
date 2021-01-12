#!/usr/bin/env sh

major=$(cat ../include/vrv/vrvdef.h | grep "#define VERSION_MAJOR" | cut -d\  -f3)
minor=$(cat ../include/vrv/vrvdef.h | grep "#define VERSION_MINOR" | cut -d\  -f3)
revision=$(cat ../include/vrv/vrvdef.h | grep "#define VERSION_REVISION" | cut -d\  -f3)
is_dev=$(cat ../include/vrv/vrvdef.h | grep "#define VERSION_DEV" | cut -d\  -f3)

dev=""
if [ $is_dev = "true" ]; then 
    dev=".dev0"
fi

echo "$major.$minor.$revision$dev"
