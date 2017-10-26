// Example file for increasing available memory - to be inserted before verovio-toolkit.js
// TOTAL_MEMORY has to be 2 * TOTAL_STACK

var Module;if(typeof Module==="undefined")Module={};

Module.TOTAL_MEMORY = 1024*1024*1024;
Module.TOTAL_STACK = 512*1024*1024;