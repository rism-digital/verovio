	// ... verovio-proxy.js preceed
	
	//console.log(Module.getMemory());
	
	// Check what to return - an instance, or the contructor method
	if (createInstance) {
		return new verovio.toolkit();
	}
	else {
		return { toolkit:verovio.toolkit };	
	}
}

// Minimal function
verovio.toolkit = function() {
	return init(0, true);
}

// Both exports
module.exports = { toolkit:verovio.toolkit };
module.exports.init = init;
