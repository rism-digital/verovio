/***************************************************************************************************************************/
// Proxy the exported c++ methods
verovio.vrvToolkit = verovio.vrvToolkit || {};

// Constructor and destructor
// Toolkit *constructor()
verovio.vrvToolkit.constructor = Module.cwrap('vrvToolkit_constructor', 'number', []);

// void destructor(Toolkit *ic)
verovio.vrvToolkit.destructor = Module.cwrap('vrvToolkit_destructor', null, ['number']);

// bool edit(Toolkit *ic, const char *editorAction) 
verovio.vrvToolkit.edit = Module.cwrap('vrvToolkit_edit', 'number', ['number', 'string']);

// char *getElementsAtTime(Toolkit *ic, int time )
verovio.vrvToolkit.getElementsAtTime = Module.cwrap('vrvToolkit_getElementsAtTime', 'string', ['number', 'number']);

// char *getElementAttr(Toolkit *ic, const char *xmlId )
verovio.vrvToolkit.getElementAttr = Module.cwrap('vrvToolkit_getElementAttr', 'string', ['number', 'string']);

// char *getLog(Toolkit *ic)
verovio.vrvToolkit.getLog = Module.cwrap('vrvToolkit_getLog', 'string', ['number']);

// char *getMEI(Toolkit *ic, int pageNo, int scoreBased )
verovio.vrvToolkit.getMEI = Module.cwrap('vrvToolkit_getMEI', 'string', ['number', 'number', 'number']);

// char *getHumdrum(Toolkit *ic)
verovio.vrvToolkit.getHumdrum = Module.cwrap('vrvToolkit_getHumdrum', 'string');

// int getPageCount(Toolkit *ic)
verovio.vrvToolkit.getPageCount = Module.cwrap('vrvToolkit_getPageCount', 'number', ['number']);

// int getPageWithElement(Toolkit *ic, const char *xmlId)
verovio.vrvToolkit.getPageWithElement = Module.cwrap('vrvToolkit_getPageWithElement', 'number', ['number', 'string']);

// double getTimeForElement(Toolkit *ic, const char *xmlId)
verovio.vrvToolkit.getTimeForElement = Module.cwrap('vrvToolkit_getTimeForElement', 'number', ['number', 'string']);

// char *getVersion(Toolkit *ic)
verovio.vrvToolkit.getVersion = Module.cwrap('vrvToolkit_getVersion', 'string', ['number']);

// bool loadData(Toolkit *ic, const char *data )
verovio.vrvToolkit.loadData = Module.cwrap('vrvToolkit_loadData', 'number', ['number', 'string']);

// void redoLayout(Toolkit *ic)
verovio.vrvToolkit.redoLayout = Module.cwrap('vrvToolkit_redoLayout', null, ['number']);

// void redoPagePitchPosLayout(Toolkit *ic)
verovio.vrvToolkit.redoPagePitchPosLayout = Module.cwrap('vrvToolkit_redoPagePitchPosLayout', null, ['number']);

// char *renderData(Toolkit *ic, const char *data, const char *options )
verovio.vrvToolkit.renderData = Module.cwrap('vrvToolkit_renderData', 'string', ['number', 'string', 'string']);

// char *renderPage(Toolkit *ic, int pageNo, const char *rendering_options )
verovio.vrvToolkit.renderPage = Module.cwrap('vrvToolkit_renderPage', 'string', ['number', 'number', 'string']);

// char *renderToMidi(Toolkit *ic, const char *rendering_options )
verovio.vrvToolkit.renderToMidi = Module.cwrap('vrvToolkit_renderToMidi', 'string', ['number', 'string']);

// char *renderToTimemap(Toolkit *ic )
verovio.vrvToolkit.renderToTimemap = Module.cwrap('vrvToolkit_renderToTimemap', 'string', ['number']);

// void setOptions(Toolkit *ic, const char *options) 
verovio.vrvToolkit.setOptions = Module.cwrap('vrvToolkit_setOptions', null, ['number', 'string']);

// A pointer to the object - only one instance can be created for now
verovio.ptr = 0;

/***************************************************************************************************************************/

verovio.toolkit = function() {
	// check if we already have one instance
	if (verovio.ptr !== 0) {
		console.warn("For now only one instance of the toolkit can be created");
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

verovio.toolkit.prototype.edit = function (editorAction) {
	return verovio.vrvToolkit.edit(this.ptr, JSON.stringify(editorAction));
};

verovio.toolkit.prototype.getElementsAtTime = function (millisec) {
	return JSON.parse(verovio.vrvToolkit.getElementsAtTime(this.ptr, millisec));
};

verovio.toolkit.prototype.getLog = function () {
	return verovio.vrvToolkit.getLog(this.ptr);
};

verovio.toolkit.prototype.getMEI = function (pageNo, scoreBased) {
	return verovio.vrvToolkit.getMEI(this.ptr, pageNo, scoreBased);
};

verovio.toolkit.prototype.getHumdrum = function () {
	return verovio.vrvToolkit.getHumdrum(this.ptr);
};

verovio.toolkit.prototype.getPageCount = function () {
	return verovio.vrvToolkit.getPageCount(this.ptr);
};

verovio.toolkit.prototype.getPageWithElement = function (xmlId) {
	return verovio.vrvToolkit.getPageWithElement(this.ptr, xmlId);
};

verovio.toolkit.prototype.getTimeForElement = function (xmlId) {
	return verovio.vrvToolkit.getTimeForElement(this.ptr, xmlId);
};

verovio.toolkit.prototype.getVersion = function () {
	return verovio.vrvToolkit.getVersion(this.ptr);
};

verovio.toolkit.prototype.loadData = function (data) {
	return verovio.vrvToolkit.loadData(this.ptr, data);
};

verovio.toolkit.prototype.redoLayout = function () {
	verovio.vrvToolkit.redoLayout(this.ptr);
}

verovio.toolkit.prototype.redoPagePitchPosLayout = function () {
	verovio.vrvToolkit.redoPagePitchPosLayout(this.ptr);
}

verovio.toolkit.prototype.renderData = function (data, options) {
    return verovio.vrvToolkit.renderData(this.ptr, data, JSON.stringify(options));
};

verovio.toolkit.prototype.renderPage = function (pageNo, options) {
    return verovio.vrvToolkit.renderPage(this.ptr, pageNo, JSON.stringify(options));
};

verovio.toolkit.prototype.renderToMidi = function (options) {
    return verovio.vrvToolkit.renderToMidi(this.ptr, JSON.stringify(options));
};

verovio.toolkit.prototype.renderToTimemap = function () {
    return verovio.vrvToolkit.renderToTimemap(this.ptr);
};

verovio.toolkit.prototype.setOptions = function (options) {
	verovio.vrvToolkit.setOptions(this.ptr, JSON.stringify(options));
};

verovio.toolkit.prototype.getElementAttr = function (xmlId) {
	return JSON.parse(verovio.vrvToolkit.getElementAttr(this.ptr, xmlId));
};

/***************************************************************************************************************************/

// If the window object is defined (if we are not within a WebWorker)...
if ((typeof window !== "undefined") && (window.addEventListener))
{
	// Add a listener that will delete the object (if necessary) when the page is closed
	window.addEventListener ("unload", function () {
		if (verovio.ptr != 0) {
			verovio.vrvToolkit.destructor( verovio.ptr );
		}
	});
}
