#!/usr/bin/env sh

cd ..
cd ..
output="./bindings/iOS/all.h"

echo "#ifndef all_h" > $output
echo "#define all_h" >> $output

for header in $(find ./include -name '*.h'); do
echo "#import <VerovioFramework/$(basename $header)>" >> $output	
done

echo "#endif /* all_h */" >> $output
echo "" >> $output
