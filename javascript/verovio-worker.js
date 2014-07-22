
// For some reason, we need to include module.js - it seems to be missing when
// called from a worker. This might be due to a compilation optimization?
importScripts("module.js", "verovio-toolkit-tmp.js");

var vrvToolkit = new verovio.toolkit();

this.onmessage = function(event) {

    //postMessage("Reply from web worker");
	// here we need to handle the verovio different messages (setOptions, loadData, etc.)
	
}

//Implementation of web worker thread code
//setInterval(function() { runEveryXSeconds() }, 5000);

//function runEveryXSeconds() {
//    postMessage("Calling back at : " + vrvToolkit.getPageCount() ) ;
//}