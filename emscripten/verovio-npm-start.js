
var verovio = verovio || {};

/*  beginning of the init function that adjsut the memory (if required) and load the module */

init = function (memory, createInstance = false) {
	
	// If we ask for creating an instance are already have one, just call the contructor
	if (createInstance && verovio.ptr != undefined) {
		return new verovio.toolkit();
	}
	
	var Module;if(typeof Module==="undefined")Module={};
	if (memory > 0) {
		if (memory < 256) {
			console.warn("Memory setting ignore because it should be at least 256");
		}
		else if (memory >= 1024) {
			console.warn("Memory setting ignored because it should be less than 1024");
		}
		else {
			Module.TOTAL_MEMORY = 2*memory*1024*1024;
			Module.TOTAL_STACK = memory*1024*1024;
			console.info("Maxmimum memory increased to %d MB", memory);
		}
	}
	
	// The verovio-proxy.js	follows...
	