
var verovio = verovio || {};

/***************************************************************************************************************************/
// Proxy the exported c++ methods
verovio.vrvInterfaceController = verovio.vrvInterfaceController || {};
// Constructor and destructor
// InterfaceController *constructor()
verovio.vrvInterfaceController.constructor = Module.cwrap('vrvInterfaceController_constructor', 'number', []);
// void destructor(InterfaceController *ic)
verovio.vrvInterfaceController.destructor = Module.cwrap('vrvInterfaceController_destructor', null, ['number']);

// char *getLog(InterfaceController *ic)
verovio.vrvInterfaceController.getLog = Module.cwrap('vrvInterfaceController_getLog', 'string', ['number']);

// int getPageCount(InterfaceController *ic)
verovio.vrvInterfaceController.getPageCount = Module.cwrap('vrvInterfaceController_getPageCount', 'number', ['number']);

// bool loadData(InterfaceController *ic, char * data )
verovio.vrvInterfaceController.loadData = Module.cwrap('vrv_InterfaceController_loadData', 'number', ['number', 'string']);

// char *renderData(InterfaceController *ic, char *data, char *options )
verovio.vrvInterfaceController.renderData = Module.cwrap('vrvInterfaceController_renderData', 'string', ['number', 'string', 'string']);

// char *renderPage(InterfaceController *ic, int pageNo, char *rendering_options )
verovio.vrvInterfaceController.renderPage = Module.cwrap('vrvInterfaceController_renderPage', 'string', ['number', 'number', 'string']);

// void setOptions(InterfaceController *ic, options) 
verovio.vrvInterfaceController.setOptions = Module.cwrap('vrvInterfaceController_setOptions', null, ['number', 'string']);


// A pointer to the object - only one instance can be created for now
verovio.ptr = 0;

// add a listener that will delete the object (if necessary) when the page is left
window.addEventListener ("unload", function () {
	if (verovio.ptr != 0) {
		verovio.vrvInterfaceController.destructor( verovio.ptr );
	}
});
/***************************************************************************************************************************/

/***************************************************************************************************************************
 exported class
 Example code 
 vrvToolkit = new verovio.toolkit();
 vrvToolkit.loadData( data );

 vrvToolkit.destroy();
*/
verovio.toolkit = function() {
	// check if we already have one instance
	if (verovio.ptr != 0) {
		console.log("For now only one instance of the toolkit can be created");
		this.ptr = verovio.ptr;
		return;
	}
	// if not, then create it
	this.ptr = verovio.vrvInterfaceController.constructor();
	verovio.ptr = this.ptr;
}

verovio.toolkit.prototype.destroy = function () {
  	verovio.vrvInterfaceController.destructor(this.ptr);
	verovio.ptr = 0;
};

verovio.toolkit.prototype.getLog = function () {
  	return verovio.vrvInterfaceController.getLog(this.ptr);
};

verovio.toolkit.prototype.getPageCount = function () {
  	return verovio.vrvInterfaceController.getPageCount(this.ptr);
};

verovio.toolkit.prototype.loadData = function (data) {
  	return verovio.vrvInterfaceController.loadData(this.ptr, data);
};

verovio.toolkit.prototype.renderData = function (data, options) {
  	return verovio.vrvInterfaceController.renderData(this.ptr, data, options);
};

verovio.toolkit.prototype.renderPage = function (page_no, options) {
  	return verovio.vrvInterfaceController.renderPage(this.ptr, page_no, options);
};

verovio.toolkit.prototype.setOptions = function (options) {
	verovio.vrvInterfaceController.setOptions(this.ptr, options);
};
/***************************************************************************************************************************/