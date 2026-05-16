export const createEmscriptenProxy = (VerovioModule) => {
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
