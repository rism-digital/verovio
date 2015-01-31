#!/bin/bash

./generate_ff.sh $1

# base64
font=${1%.sfd}.woff
w=`base64 $font`

# output
cat woff-1.txt > woff.xml 
echo $w >> woff.xml
cat woff-2.txt >> woff.xml 

mv woff.xml ../data