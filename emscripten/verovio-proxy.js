
var verovio = verovio || {};

/***************************************************************************************************************************/
// Proxy the exported c++ methods
verovio.vrvToolkit = verovio.vrvToolkit || {};

// Constructor and destructor
// Toolkit *constructor()
verovio.vrvToolkit.constructor = Module.cwrap('vrvToolkit_constructor', 'number', []);

// void destructor(Toolkit *ic)
verovio.vrvToolkit.destructor = Module.cwrap('vrvToolkit_destructor', null, ['number']);

// char *getLog(Toolkit *ic)
verovio.vrvToolkit.getLog = Module.cwrap('vrvToolkit_getLog', 'string', ['number']);

// int getPageCount(Toolkit *ic)
verovio.vrvToolkit.getPageCount = Module.cwrap('vrvToolkit_getPageCount', 'number', ['number']);

// int getPageWithElement(Toolkit *ic, const char *xmlId)
verovio.vrvToolkit.getPageWithElement = Module.cwrap('vrvToolkit_getPageWithElement', 'number', ['number', 'string']);

// bool loadData(Toolkit *ic, const char *data )
verovio.vrvToolkit.loadData = Module.cwrap('vrvToolkit_loadData', 'number', ['number', 'string']);

// void redoLayout(Toolkit *ic)
verovio.vrvToolkit.redoLayout = Module.cwrap('vrvToolkit_redoLayout', null, ['number']);

// char *renderData(Toolkit *ic, const char *data, const char *options )
verovio.vrvToolkit.renderData = Module.cwrap('vrvToolkit_renderData', 'string', ['number', 'string', 'string']);

// char *renderPage(Toolkit *ic, int pageNo, const char *rendering_options )
verovio.vrvToolkit.renderPage = Module.cwrap('vrvToolkit_renderPage', 'string', ['number', 'number', 'string']);


// char *getMEI(Toolkit *ic, int pageNo )
verovio.vrvToolkit.getMEI = Module.cwrap('vrvToolkit_getMEI', 'string', ['number', 'number']);

// void setOptions(Toolkit *ic, const char *options) 
verovio.vrvToolkit.setOptions = Module.cwrap('vrvToolkit_setOptions', null, ['number', 'string']);

// A pointer to the object - only one instance can be created for now
verovio.ptr = 0;

// add a listener that will delete the object (if necessary) when the page is left
window.addEventListener ("unload", function () {
	if (verovio.ptr != 0) {
		verovio.vrvToolkit.destructor( verovio.ptr );
	}
});

/***************************************************************************************************************************/

verovio.toolkit = function() {
	// check if we already have one instance
	if (verovio.ptr != 0) {
		console.log("For now only one instance of the toolkit can be created");
		this.ptr = verovio.ptr;
		return;
	}
	// if not, then create it
	this.ptr = verovio.vrvToolkit.constructor();
	verovio.ptr = this.ptr;
}

verovio.toolkit.prototype.destroy = function () {
  	verovio.vrvToolkit.destructor(this.ptr);
	verovio.ptr = 0;
};

verovio.toolkit.prototype.getLog = function () {
  	return verovio.vrvToolkit.getLog(this.ptr);
};

verovio.toolkit.prototype.getPageCount = function () {
  	return verovio.vrvToolkit.getPageCount(this.ptr);
};

verovio.toolkit.prototype.getPageWithElement = function (xmlId) {
  	return verovio.vrvToolkit.getPageWithElement(this.ptr, xmlId);
};

verovio.toolkit.prototype.loadData = function (data) {
  	return verovio.vrvToolkit.loadData(this.ptr, data);
};

verovio.toolkit.prototype.redoLayout = function () {
  	verovio.vrvToolkit.redoLayout(this.ptr);
}

verovio.toolkit.prototype.renderData = function (data, options) {
  	return verovio.vrvToolkit.renderData(this.ptr, data, options);
};

verovio.toolkit.prototype.renderPage = function (page_no, options) {
  	return verovio.vrvToolkit.renderPage(this.ptr, page_no, options);
};

verovio.toolkit.prototype.getMEI = function (page_no) {
  	return verovio.vrvToolkit.getMEI(this.ptr, page_no);
};

verovio.toolkit.prototype.setOptions = function (options) {
	verovio.vrvToolkit.setOptions(this.ptr, options);
};

/***************************************************************************************************************************/
