#!/bin/bash

# This is a little script that compiles verovio using buildToolkit
# and moves the output, verovio.js, into Neon/verovio-util. 
# It then starts up a local instance of Neon using yarn so the 
# developer can see the changes as soon as possible. 
# Please change [location_of_your_Neon] with the actual path. 
# For example: ~/Desktop/DDMAL


# Builds verovio for Neon
./buildToolkit -x "Gootville,Petaluma" -DHPX 

cd build

# Moves the needed verovio.js into Neon
cp verovio.js verovio_new.js

mv verovio_new.js [location_of_your_Neon]/Neon/verovio-util

cd [location_of_your_Neon]/Neon/verovio-util

rm verovio.js; mv verovio_new.js verovio.js

# Runs Neon
yarn build && yarn start
