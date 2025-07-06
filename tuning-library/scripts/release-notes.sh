#!/bin/bash

cat <<- EOH
# Automated build of surge-synth-team tuning library executables

Look, the tuning library is a C++ library which you use inside other synths.
Your most likely usage of it is as a developer implemeenting tuning. But we
do make a binary of our command line exe and test exe available at the back
of our pipeline. If that's something you'd like to download, you can do so here!

EOH
date
echo ""
echo "Most recent commits:" 
echo ""
git log --pretty=oneline | head -5
