import VerovioEmscriptenAdapter from './VerovioEmscriptenAdapter';

const VerovioToolkit = class {

    constructor() {
        this.ptr = VerovioEmscriptenAdapter.vrvToolkit.constructor();
        console.debug("Creating toolkit instance");
        VerovioEmscriptenAdapter.instances.push(this.ptr);
    }

    destroy() {
        VerovioEmscriptenAdapter.instances.splice(VerovioEmscriptenAdapter.instances.indexOf(this.ptr), 1);
        console.debug("Deleting toolkit instance");
        VerovioEmscriptenAdapter.vrvToolkit.destructor(this.ptr);
    }

    edit(editorAction) {
        return VerovioEmscriptenAdapter.vrvToolkit.edit(this.ptr, JSON.stringify(editorAction));
    }

    editInfo() {
        return JSON.parse(VerovioEmscriptenAdapter.vrvToolkit.editInfo(this.ptr));
    }

    getAvailableOptions() {
        return JSON.parse(VerovioEmscriptenAdapter.vrvToolkit.getAvailableOptions(this.ptr));
    }

    getElementAttr(xmlId) {
        return JSON.parse(VerovioEmscriptenAdapter.vrvToolkit.getElementAttr(this.ptr, xmlId));
    }

    getElementsAtTime(millisec) {
        return JSON.parse(VerovioEmscriptenAdapter.vrvToolkit.getElementsAtTime(this.ptr, millisec));
    }

    getExpansionIdsForElement(xmlId) {
        return JSON.parse(VerovioEmscriptenAdapter.vrvToolkit.getExpansionIdsForElement(this.ptr, xmlId));
    }

    getHumdrum() {
        return VerovioEmscriptenAdapter.vrvToolkit.getHumdrum(this.ptr);
    }

    getLog() {
        return VerovioEmscriptenAdapter.vrvToolkit.getLog(this.ptr);
    }

    getMEI(pageNo, scoreBased) {
        return VerovioEmscriptenAdapter.vrvToolkit.getMEI(this.ptr, pageNo, scoreBased);
    }

    getMIDIValuesForElement(xmlId) {
        return JSON.parse(VerovioEmscriptenAdapter.vrvToolkit.getMIDIValuesForElement(this.ptr, xmlId));
    }

    getNotatedIdForElement(xmlId) {
        return VerovioEmscriptenAdapter.vrvToolkit.getNotatedIdForElement(this.ptr, xmlId);
    }

    getOptions(defaultValues) {
        return JSON.parse(VerovioEmscriptenAdapter.vrvToolkit.getOptions(this.ptr, defaultValues));
    }

    getPageCount() {
        return VerovioEmscriptenAdapter.vrvToolkit.getPageCount(this.ptr);
    }

    getPageWithElement(xmlId) {
        return VerovioEmscriptenAdapter.vrvToolkit.getPageWithElement(this.ptr, xmlId);
    }

    getTimeForElement(xmlId) {
        return VerovioEmscriptenAdapter.vrvToolkit.getTimeForElement(this.ptr, xmlId);
    }

    getVersion() {
        return VerovioEmscriptenAdapter.vrvToolkit.getVersion(this.ptr);
    }

    loadData(data) {
        return VerovioEmscriptenAdapter.vrvToolkit.loadData(this.ptr, data);
    }

    redoLayout() {
        VerovioEmscriptenAdapter.vrvToolkit.redoLayout(this.ptr);
    }

    redoPagePitchPosLayout() {
        VerovioEmscriptenAdapter.vrvToolkit.redoPagePitchPosLayout(this.ptr);
    }

    renderData(data, options) {
        return VerovioEmscriptenAdapter.vrvToolkit.renderData(this.ptr, data, JSON.stringify(options));
    }

    renderPage(pageNo, options) {
        console.warn("Method renderPage is deprecated; use renderToSVG instead");
        return VerovioEmscriptenAdapter.vrvToolkit.renderToSVG(this.ptr, pageNo, JSON.stringify(options));
    }

    renderToMIDI(options) {
        return VerovioEmscriptenAdapter.vrvToolkit.renderToMIDI(this.ptr, JSON.stringify(options));
    }

    renderToMidi(options) {
        console.warn("Method renderToMidi is deprecated; use renderToMIDI instead");
        return VerovioEmscriptenAdapter.vrvToolkit.renderToMIDI(this.ptr, JSON.stringify(options));
    }

    renderToSVG(pageNo, options) {
        return VerovioEmscriptenAdapter.vrvToolkit.renderToSVG(this.ptr, pageNo, JSON.stringify(options));
    }

    renderToTimemap() {
        return JSON.parse(VerovioEmscriptenAdapter.vrvToolkit.renderToTimemap(this.ptr));
    }

    setOptions(options) {
        VerovioEmscriptenAdapter.vrvToolkit.setOptions(this.ptr, JSON.stringify(options));
    }

};

// If the window object is defined (if we are not within a WebWorker)...
if ((typeof window !== "undefined") && (window.addEventListener)) {
    // Add a listener that will delete the object (if necessary) when the page is closed
    window.addEventListener("unload", function () {
        for (var i = 0; i < VerovioEmscriptenAdapter.instances.length; i++) {
            VerovioEmscriptenAdapter.vrvToolkit.destructor(VerovioEmscriptenAdapter.instances[i]);
        }
    });
}

export default VerovioToolkit;
