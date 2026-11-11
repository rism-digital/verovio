#!/bin/bash

# This script builds and runs the tests of the runtime fonts
# The optional parameter is the build directory (./build by default)

set -e

cd "$(dirname "$0")"
tests=$(pwd)
root=$(cd ../../..; pwd)
build=${1:-$tests/build}
mkdir -p "$build"

# Build Verovio as a library for the registration tests and the command-line tool for the option tests
cmake -S "$root/cmake" -B "$build/lib" -DCMAKE_BUILD_TYPE=Release -DBUILD_AS_LIBRARY=ON -DNO_HUMDRUM_SUPPORT=ON
cmake --build "$build/lib" --parallel
cmake -S "$root/cmake" -B "$build/cli" -DCMAKE_BUILD_TYPE=Release -DNO_HUMDRUM_SUPPORT=ON
cmake --build "$build/cli" --parallel

includes=""
for dir in include include/crc include/midi include/hum include/json include/pugi include/tuning-library include/zip \
    include/vrv libmei/dist libmei/addons; do
    includes="$includes -I$root/$dir"
done
c++ -std=c++20 -O2 $includes registration.cpp -L"$build/lib" -lverovio -Wl,-rpath,"$build/lib" \
    -o "$build/registration"

echo "Running the registration tests..."
"$build/registration" \
    "$tests/fonts/Tinos-Regular.ttf" \
    "$root/fonts/Bravura/Bravura.otf" \
    "$root/data/fonts/Bravura_metadata.json" \
    "$tests/fonts/Tinos-Regular.woff" \
    "$tests/fonts/Tinos-Regular.woff2" \
    "$root/data/fonts/Tinos-Italic.woff2" \
    "$root/data/fonts/Tinos-Bold.woff2" \
    "$root/data/fonts/Tinos-BoldItalic.woff2" \
    "$root/data" \
    "$tests/text-heavy.mei" \
    "$tests/VerovioTestLigature.ttf" \
    "$tests/VerovioTestMusic.ttf" \
    "$tests/VerovioTestMusic_metadata.json" \
    "$tests/VerovioTestLigatureConflict.ttf" \
    "$tests/VerovioTestLocalizedBold.ttf" \
    "$tests/VerovioTestHyphen.ttf"

echo "Running the command-line tests..."
verovio="$build/cli/verovio"
# A file name with "=" checks that the alias is split at the first "="
aliased="$build/VerovioTest=Ligature.ttf"
cp "$tests/VerovioTestLigature.ttf" "$aliased"
svg=$("$verovio" -r "$root/data" --font-add-text-as "QS=$aliased" --font-add-text-as "LongQS=$aliased" \
    --font-add-music-as "VM=$tests/VerovioTestMusic.ttf" --svg-text-as-paths -o - "$tests/alias-dir.mei")
if [[ "$svg" != *"text-8BFEB250B0FDDA0E-4-"* ]]; then
    echo "The CLI did not render QS with the registered ligature face"
    exit 1
fi
if [[ "$svg" != *'font-family="VM"'* ]]; then
    echo "The CLI did not retain the selected VM music family"
    exit 1
fi
if error=$("$verovio" -r "$root/data" --font-add-text-as "missing-separator" -o - "$tests/alias-dir.mei" 2>&1 >/dev/null); then
    echo "Malformed aliased CLI registration unexpectedly succeeded"
    exit 1
fi
if [[ "$error" != *"expects ALIAS=FILE"* ]]; then
    echo "Malformed aliased CLI registration did not explain the required syntax"
    exit 1
fi

echo "All font tests passed"
