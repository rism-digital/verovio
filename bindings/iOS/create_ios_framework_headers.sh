#!/usr/bin/env sh

cd ..
cd ..
output="./bindings/iOS/all.h"

echo "#ifndef all_h" > "$output"
echo "#define all_h" >> "$output"

# Directories with the headers of the embedded libraries in external
external="./external/crc ./external/jsonxx ./external/midifile/include ./external/tuning-library/include"

for header in $(find ./include $external -name '*.h' -not -path '*win32*' | sed 's|.*/||' | sort); do
    echo "#import <VerovioFramework/$header>" >> "$output"
done

echo "#endif /* all_h */" >> "$output"
