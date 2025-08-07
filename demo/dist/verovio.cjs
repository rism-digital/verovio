'use strict';

const createEmscriptenProxy = (VerovioModule) => {
    return new Proxy({}, {
        get: (target, method) => {
            return (...args) => {
                return getToolkitFunction(VerovioModule, method)(...args);
            };
        },
    });
};

function getToolkitFunction(VerovioModule, method) {
    const mapping = {};

    // Constructor and destructor
    // Toolkit *constructor()
    mapping.constructor = VerovioModule.cwrap("vrvToolkit_constructor", "number", []);

    // void destructor(Toolkit *ic)
    mapping.destructor = VerovioModule.cwrap("vrvToolkit_destructor", null, ["number"]);

    // bool edit(Toolkit *ic, const char *editorAction) 
    mapping.edit = VerovioModule.cwrap("vrvToolkit_edit", "number", ["number", "string"]);

    // char *editInfo(Toolkit *ic)
    mapping.editInfo = VerovioModule.cwrap("vrvToolkit_editInfo", "string", ["number"]);

    // char *getAvailableOptions(Toolkit *ic)
    mapping.getAvailableOptions = VerovioModule.cwrap("vrvToolkit_getAvailableOptions", "string", ["number"]);

    // char *getDefaultOptions(Toolkit *ic)
    mapping.getDefaultOptions = VerovioModule.cwrap("vrvToolkit_getDefaultOptions", "string", ["number"]);

    // char *getDescriptiveFeatures(Toolkit *ic, const char *options)
    mapping.getDescriptiveFeatures = VerovioModule.cwrap("vrvToolkit_getDescriptiveFeatures", "string", ["number", "string"]);

    // char *getElementAttr(Toolkit *ic, const char *xmlId)
    mapping.getElementAttr = VerovioModule.cwrap("vrvToolkit_getElementAttr", "string", ["number", "string"]);

    // char *getElementsAtTime(Toolkit *ic, int time)
    mapping.getElementsAtTime = VerovioModule.cwrap("vrvToolkit_getElementsAtTime", "string", ["number", "number"]);

    // char *vrvToolkit_getExpansionIdsForElement(Toolkit *tk, const char *xmlId);
    mapping.getExpansionIdsForElement = VerovioModule.cwrap("vrvToolkit_getExpansionIdsForElement", "string", ["number", "string"]);

    // char *getHumdrum(Toolkit *ic)
    mapping.getHumdrum = VerovioModule.cwrap("vrvToolkit_getHumdrum", "string", ["number"]);

    // char *convertMEIToHumdrum(Toolkit *ic, const char *meiData)
    mapping.convertMEIToHumdrum = VerovioModule.cwrap("vrvToolkit_convertMEIToHumdrum", "string", ["number", "string"]);

    // char *convertHumdrumToHumdrum(Toolkit *ic, const char *humdrumData)
    mapping.convertHumdrumToHumdrum = VerovioModule.cwrap("vrvToolkit_convertHumdrumToHumdrum", "string", ["number", "string"]);

    // char *convertHumdrumToMIDI(Toolkit *ic, const char *humdrumData)
    mapping.convertHumdrumToMIDI = VerovioModule.cwrap("vrvToolkit_convertHumdrumToMIDI", "string", ["number", "string"]);

    // char *getLog(Toolkit *ic)
    mapping.getLog = VerovioModule.cwrap("vrvToolkit_getLog", "string", ["number"]);

    // char *getMEI(Toolkit *ic, const char *options)
    mapping.getMEI = VerovioModule.cwrap("vrvToolkit_getMEI", "string", ["number", "string"]);

    // char *vrvToolkit_getNotatedIdForElement(Toolkit *tk, const char *xmlId);
    mapping.getNotatedIdForElement = VerovioModule.cwrap("vrvToolkit_getNotatedIdForElement", "string", ["number", "string"]);

    // char *getOptions(Toolkit *ic)
    mapping.getOptions = VerovioModule.cwrap("vrvToolkit_getOptions", "string", ["number"]);

    // int getPageCount(Toolkit *ic)
    mapping.getPageCount = VerovioModule.cwrap("vrvToolkit_getPageCount", "number", ["number"]);

    // int getPageWithElement(Toolkit *ic, const char *xmlId)
    mapping.getPageWithElement = VerovioModule.cwrap("vrvToolkit_getPageWithElement", "number", ["number", "string"]);

    // double getTimeForElement(Toolkit *ic, const char *xmlId)
    mapping.getTimeForElement = VerovioModule.cwrap("vrvToolkit_getTimeForElement", "number", ["number", "string"]);

    // char *getTimesForElement(Toolkit *ic, const char *xmlId)
    mapping.getTimesForElement = VerovioModule.cwrap("vrvToolkit_getTimesForElement", "string", ["number", "string"]);

    // char *getMIDIValuesForElement(Toolkit *ic, const char *xmlId)
    mapping.getMIDIValuesForElement = VerovioModule.cwrap("vrvToolkit_getMIDIValuesForElement", "string", ["number", "string"]);

    // char *getVersion(Toolkit *ic)
    mapping.getVersion = VerovioModule.cwrap("vrvToolkit_getVersion", "string", ["number"]);

    // bool loadData(Toolkit *ic, const char *data)
    mapping.loadData = VerovioModule.cwrap("vrvToolkit_loadData", "number", ["number", "string"]);

    // bool loadZipDataBase64(Toolkit *ic, const char *data)
    mapping.loadZipDataBase64 = VerovioModule.cwrap("vrvToolkit_loadZipDataBase64", "number", ["number", "string"]);

    // bool loadZipDataBuffer(Toolkit *ic, const unsigned char *data, int length)
    mapping.loadZipDataBuffer = VerovioModule.cwrap("vrvToolkit_loadZipDataBuffer", "number", ["number", "number", "number"]);

    // void redoLayout(Toolkit *ic)
    mapping.redoLayout = VerovioModule.cwrap("vrvToolkit_redoLayout", null, ["number", "string"]);

    // void redoPagePitchPosLayout(Toolkit *ic)
    mapping.redoPagePitchPosLayout = VerovioModule.cwrap("vrvToolkit_redoPagePitchPosLayout", null, ["number"]);

    // char *renderData(Toolkit *ic, const char *data, const char *options)
    mapping.renderData = VerovioModule.cwrap("vrvToolkit_renderData", "string", ["number", "string", "string"]);

    // char *renderToExpansionMap(Toolkit *ic)
    mapping.renderToExpansionMap = VerovioModule.cwrap("vrvToolkit_renderToExpansionMap", "string", ["number"]);

    // char *renderToMIDI(Toolkit *ic)
    mapping.renderToMIDI = VerovioModule.cwrap("vrvToolkit_renderToMIDI", "string", ["number"]);

    // char *renderToPAE(Toolkit *ic)
    mapping.renderToPAE = VerovioModule.cwrap("vrvToolkit_renderToPAE", "string", ["number"]);

    // char *renderToSvg(Toolkit *ic, int pageNo, int xmlDeclaration)
    mapping.renderToSVG = VerovioModule.cwrap("vrvToolkit_renderToSVG", "string", ["number", "number", "number"]);

    // char *renderToTimemap(Toolkit *ic)
    mapping.renderToTimemap = VerovioModule.cwrap("vrvToolkit_renderToTimemap", "string", ["number", "string"]);

    // void resetOptions(Toolkit *ic)
    mapping.resetOptions = VerovioModule.cwrap("vrvToolkit_resetOptions", null, ["number"]);

    // void resetXmlIdSeed(Toolkit *ic, int seed) 
    mapping.resetXmlIdSeed = VerovioModule.cwrap("vrvToolkit_resetXmlIdSeed", null, ["number", "number"]);

    // bool select(Toolkit *ic, const char *options) 
    mapping.select = VerovioModule.cwrap("vrvToolkit_select", "number", ["number", "string"]);

    // void setOptions(Toolkit *ic, const char *options) 
    mapping.setOptions = VerovioModule.cwrap("vrvToolkit_setOptions", null, ["number", "string"]);

    // char *validatePAE(Toolkit *ic, const char *options)
    mapping.validatePAE = VerovioModule.cwrap("vrvToolkit_validatePAE", "string", ["number", "string"]);

    return mapping[method];
}

class VerovioToolkit {

    constructor(VerovioModule) {
        this.VerovioModule = VerovioModule;
        if (!this.VerovioModule) {
            throw new Error("VerovioToolkit needs VerovioModule passed as argument to the constructor.");
        }
        this.proxy = createEmscriptenProxy(this.VerovioModule);
        this.ptr = this.proxy.constructor();
        VerovioToolkit.instances.push(this);
    }

    destroy() {
        VerovioToolkit.instances.splice(VerovioToolkit.instances.findIndex(i => i.ptr === this.ptr), 1);
        this.proxy.destructor(this.ptr);
    }

    edit(editorAction) {
        return this.proxy.edit(this.ptr, JSON.stringify(editorAction));
    }

    editInfo() {
        return JSON.parse(this.proxy.editInfo(this.ptr));
    }

    getAvailableOptions() {
        return JSON.parse(this.proxy.getAvailableOptions(this.ptr));
    }

    getDefaultOptions() {
        return JSON.parse(this.proxy.getDefaultOptions(this.ptr));
    }

    getDescriptiveFeatures(options) {
        return JSON.parse(this.proxy.getDescriptiveFeatures(this.ptr, JSON.stringify(options)));
    }

    getElementAttr(xmlId) {
        return JSON.parse(this.proxy.getElementAttr(this.ptr, xmlId));
    }

    getElementsAtTime(millisec) {
        return JSON.parse(this.proxy.getElementsAtTime(this.ptr, millisec));
    }

    getExpansionIdsForElement(xmlId) {
        return JSON.parse(this.proxy.getExpansionIdsForElement(this.ptr, xmlId));
    }

    getHumdrum() {
        return this.proxy.getHumdrum(this.ptr);
    }

    convertHumdrumToHumdrum(data) {
        return this.proxy.convertHumdrumToHumdrum(this.ptr, data);
    }

    convertHumdrumToMIDI(data) {
        return this.proxy.convertHumdrumToMIDI(this.ptr, data);
    }

    convertMEIToHumdrum(data) {
        return this.proxy.convertMEIToHumdrum(this.ptr, data);
    }

    getLog() {
        return this.proxy.getLog(this.ptr);
    }

    getMEI(options = {}) {
        return this.proxy.getMEI(this.ptr, JSON.stringify(options));
    }

    getMIDIValuesForElement(xmlId) {
        return JSON.parse(this.proxy.getMIDIValuesForElement(this.ptr, xmlId));
    }

    getNotatedIdForElement(xmlId) {
        return this.proxy.getNotatedIdForElement(this.ptr, xmlId);
    }

    getOptions(defaultValues) {
        if (defaultValues === true) {
            console.warn( "This function (with 'true' parameter) is deprecated. Use getDefaultOptions() instead." );
            return JSON.parse(this.proxy.getDefaultOptions(this.ptr));    
        }
        else if (defaultValues === false) {
            console.warn( "This function (with 'false' parameter) is deprecated. Use getOptions() instead." );
            return JSON.parse(this.proxy.getOptions(this.ptr));    
        }
        else {
            return JSON.parse(this.proxy.getOptions(this.ptr));  
        }
    }

    getPageCount() {
        return this.proxy.getPageCount(this.ptr);
    }

    getPageWithElement(xmlId) {
        return this.proxy.getPageWithElement(this.ptr, xmlId);
    }

    getTimeForElement(xmlId) {
        return this.proxy.getTimeForElement(this.ptr, xmlId);
    }

    getTimesForElement(xmlId) {
        return JSON.parse(this.proxy.getTimesForElement(this.ptr, xmlId));
    }

    getVersion() {
        return this.proxy.getVersion(this.ptr);
    }

    loadData(data) {
        return this.proxy.loadData(this.ptr, data);
    }

    loadZipDataBase64(data) {
        return this.proxy.loadZipDataBase64(this.ptr, data);
    }

    loadZipDataBuffer(data) {
        if (!(data instanceof ArrayBuffer)) {
            console.error("Parameter for loadZipDataBuffer has to be of type ArrayBuffer");
            return false;
        }
        var dataArray = new Uint8Array(data);
        var dataSize = dataArray.length * dataArray.BYTES_PER_ELEMENT;
        var dataPtr = this.VerovioModule._malloc(dataSize);
        this.VerovioModule.HEAPU8.set(dataArray, dataPtr);
        var res = this.proxy.loadZipDataBuffer(this.ptr, dataPtr, dataSize);
        this.VerovioModule._free(dataPtr);
        return res;
    }

    redoLayout(options = {}) {
        this.proxy.redoLayout(this.ptr, JSON.stringify(options));
    }

    redoPagePitchPosLayout() {
        this.proxy.redoPagePitchPosLayout(this.ptr);
    }

    renderData(data, options) {
        return this.proxy.renderData(this.ptr, data, JSON.stringify(options));
    }

    renderToExpansionMap() {
        return JSON.parse(this.proxy.renderToExpansionMap(this.ptr));
    }

    renderToMIDI() {
        return this.proxy.renderToMIDI(this.ptr);
    }

    renderToPAE() {
        return this.proxy.renderToPAE(this.ptr);
    }

    renderToSVG(pageNo = 1, xmlDeclaration = false) {
        return this.proxy.renderToSVG(this.ptr, pageNo, xmlDeclaration);
    }

    renderToTimemap(options = {}) {
        return JSON.parse(this.proxy.renderToTimemap(this.ptr, JSON.stringify(options)));
    }

    resetOptions() {
        this.proxy.resetOptions(this.ptr);
    }

    resetXmlIdSeed(seed) {
        return this.proxy.resetXmlIdSeed(this.ptr, seed);
    }

    select(selection) {
        return this.proxy.select(this.ptr, JSON.stringify(selection));
    }

    setOptions(options) {
        options = this.preprocessOptions(options);
        return this.proxy.setOptions(this.ptr, JSON.stringify(options));
    }

    validatePAE(data) {
        if (data instanceof Object) {
            data = JSON.stringify(data);
        }
        return JSON.parse(this.proxy.validatePAE(this.ptr, data));
    }

    preprocessOptions(options) {
        // Nothing to do if we do not have 'fontAddCustom' set
        if (!options.hasOwnProperty('fontAddCustom')) {
            return options;
        }
        const files = options['fontAddCustom'];
        let filesInBase64 = [];
        // Get all the files and convert them to a base64 string if necessary
        for ( const file of files ) {
            // The file is already passed as base64 string - nothing to do
            if (!/^https?:\/\//.test( file )) {
                filesInBase64.push( file );
                continue;
            }
            const request = new XMLHttpRequest();
            request.open("GET", file, false); // `false` makes the request synchronous
            request.send(null);

            if (request.status === 200) {
                filesInBase64.push(request.responseText);
            }
            else {
                console.error(`${file} could not be retrieved`);
            }
        }
        options["fontAddCustom"] = filesInBase64;
        return options;
    }
}

// A pointer to the object - only one instance can be created for now
VerovioToolkit.instances = [];


// If the window object is defined (if we are not within a WebWorker)...
if ((typeof window !== "undefined") && (window.addEventListener)) {
    // Add a listener that will delete the object (if necessary) when the page is closed
    window.addEventListener("unload", () => {
        VerovioToolkit.instances.forEach((instance) => {
            instance.destroy();
        });
    });
}

const LOG_OFF = 0;
const LOG_ERROR = 1;
const LOG_WARNING = 2;
const LOG_INFO = 3;
const LOG_DEBUG = 4;

function enableLog(level, VerovioModule) {
    return VerovioModule.cwrap("enableLog", null, ["number"])(level);
}

function enableLogToBuffer(value, VerovioModule) {
    return VerovioModule.cwrap("enableLogToBuffer", null, ["number"])(value);
}

exports.LOG_DEBUG = LOG_DEBUG;
exports.LOG_ERROR = LOG_ERROR;
exports.LOG_INFO = LOG_INFO;
exports.LOG_OFF = LOG_OFF;
exports.LOG_WARNING = LOG_WARNING;
exports.VerovioToolkit = VerovioToolkit;
exports.enableLog = enableLog;
exports.enableLogToBuffer = enableLogToBuffer;
