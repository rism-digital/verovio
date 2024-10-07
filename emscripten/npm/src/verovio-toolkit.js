
import { createEmscriptenProxy } from "./emscripten-proxy.js";

async function solve(options) {      
    const res = await fetch(
      `https://raw.githubusercontent.com/lpugin/test-font/main/GoldenAge.zip`,
       {
            method: "GET",
       }
    );
    const data = await res.blob();
    console.log( res );
    console.log( options );
    return options;
}

export class VerovioToolkit {

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
        const filenames = options['fontAddCustom'];
        let filesInBase64 = [];
        // Get all the files and convert them to a base64 string
        for (let i = 0; i < filenames.length; i++ ) {
            const request = new XMLHttpRequest();
            request.open("GET", filenames[i], false); // `false` makes the request synchronous
            request.send(null);

            if (request.status === 200) {
                filesInBase64.push(request.responseText);
            }
            else {
                console.error(`${filenames[i]} could not be retrieved`);
            }
        }
        options["fontAddCustom"] = filesInBase64;
        //console.log( options );
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
