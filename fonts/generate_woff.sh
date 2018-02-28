#!/bin/bash

font="VerovioText-1.0.sfd"

shopt -s expand_aliases
[[ -f ~/.bash_profile ]] && source ~/.bash_profile

#./generate_ff.sh $1
fontforge generate_ff.py $font

# base64
woffFont=${1%.sfd}.woff
w=$(base64 $woffFont)

# output
cat woff-1.txt > woff.xml
echo $w >> woff.xml
cat woff-2.txt >> woff.xml

mv woff.xml ../data
