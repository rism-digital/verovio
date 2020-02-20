import Module from './VerovioEmscriptenModule';

const VerovioEmscriptenAdapter = {};

// Proxy the exported c++ methods
VerovioEmscriptenAdapter.vrvToolkit = VerovioEmscriptenAdapter.vrvToolkit || {};

// Constructor and destructor
// Toolkit *constructor()
VerovioEmscriptenAdapter.vrvToolkit.constructor = Module.cwrap('vrvToolkit_constructor', 'number', []);

// void destructor(Toolkit *ic)
VerovioEmscriptenAdapter.vrvToolkit.destructor = Module.cwrap('vrvToolkit_destructor', null, ['number']);

// bool edit(Toolkit *ic, const char *editorAction)
VerovioEmscriptenAdapter.vrvToolkit.edit = Module.cwrap('vrvToolkit_edit', 'number', ['number', 'string']);

// char *editInfo(Toolkit *ic)
VerovioEmscriptenAdapter.vrvToolkit.editInfo = Module.cwrap('vrvToolkit_editInfo', 'string', ['number']);

// char *getAvailableOptions(Toolkit *ic)
VerovioEmscriptenAdapter.vrvToolkit.getAvailableOptions = Module.cwrap('vrvToolkit_getAvailableOptions', 'string', ['number']);

// char *getElementAttr(Toolkit *ic, const char *xmlId)
VerovioEmscriptenAdapter.vrvToolkit.getElementAttr = Module.cwrap('vrvToolkit_getElementAttr', 'string', ['number', 'string']);

// char *getElementsAtTime(Toolkit *ic, int time)
VerovioEmscriptenAdapter.vrvToolkit.getElementsAtTime = Module.cwrap('vrvToolkit_getElementsAtTime', 'string', ['number', 'number']);

// char *vrvToolkit_getExpansionIdsForElement(Toolkit *tk, const char *xmlId);
VerovioEmscriptenAdapter.vrvToolkit.getExpansionIdsForElement = Module.cwrap('vrvToolkit_getExpansionIdsForElement', 'string', ['number', 'string']);

// char *getHumdrum(Toolkit *ic)
VerovioEmscriptenAdapter.vrvToolkit.getHumdrum = Module.cwrap('vrvToolkit_getHumdrum', 'string');

// char *getLog(Toolkit *ic)
VerovioEmscriptenAdapter.vrvToolkit.getLog = Module.cwrap('vrvToolkit_getLog', 'string', ['number']);

// char *getMEI(Toolkit *ic, int pageNo, int scoreBased)
VerovioEmscriptenAdapter.vrvToolkit.getMEI = Module.cwrap('vrvToolkit_getMEI', 'string', ['number', 'number', 'number']);

// char *vrvToolkit_getNotatedIdForElement(Toolkit *tk, const char *xmlId);
VerovioEmscriptenAdapter.vrvToolkit.getNotatedIdForElement = Module.cwrap('vrvToolkit_getNotatedIdForElement', 'string', ['number', 'string']);

// char *getOptions(Toolkit *ic, int defaultValues)
VerovioEmscriptenAdapter.vrvToolkit.getOptions = Module.cwrap('vrvToolkit_getOptions', 'string', ['number', 'number']);

// int getPageCount(Toolkit *ic)
VerovioEmscriptenAdapter.vrvToolkit.getPageCount = Module.cwrap('vrvToolkit_getPageCount', 'number', ['number']);

// int getPageWithElement(Toolkit *ic, const char *xmlId)
VerovioEmscriptenAdapter.vrvToolkit.getPageWithElement = Module.cwrap('vrvToolkit_getPageWithElement', 'number', ['number', 'string']);

// double getTimeForElement(Toolkit *ic, const char *xmlId)
VerovioEmscriptenAdapter.vrvToolkit.getTimeForElement = Module.cwrap('vrvToolkit_getTimeForElement', 'number', ['number', 'string']);

// char *getMIDIValuesForElement(Toolkit *ic, const char *xmlId)
VerovioEmscriptenAdapter.vrvToolkit.getMIDIValuesForElement = Module.cwrap('vrvToolkit_getMIDIValuesForElement', 'string', ['number', 'string']);

// char *getVersion(Toolkit *ic)
VerovioEmscriptenAdapter.vrvToolkit.getVersion = Module.cwrap('vrvToolkit_getVersion', 'string', ['number']);

// bool loadData(Toolkit *ic, const char *data)
VerovioEmscriptenAdapter.vrvToolkit.loadData = Module.cwrap('vrvToolkit_loadData', 'number', ['number', 'string']);

// void redoLayout(Toolkit *ic)
VerovioEmscriptenAdapter.vrvToolkit.redoLayout = Module.cwrap('vrvToolkit_redoLayout', null, ['number']);

// void redoPagePitchPosLayout(Toolkit *ic)
VerovioEmscriptenAdapter.vrvToolkit.redoPagePitchPosLayout = Module.cwrap('vrvToolkit_redoPagePitchPosLayout', null, ['number']);

// char *renderData(Toolkit *ic, const char *data, const char *options)
VerovioEmscriptenAdapter.vrvToolkit.renderData = Module.cwrap('vrvToolkit_renderData', 'string', ['number', 'string', 'string']);

// char *renderToMidi(Toolkit *ic, const char *rendering_options)
VerovioEmscriptenAdapter.vrvToolkit.renderToMIDI = Module.cwrap('vrvToolkit_renderToMIDI', 'string', ['number', 'string']);

// char *renderToSvg(Toolkit *ic, int pageNo, const char *rendering_options)
VerovioEmscriptenAdapter.vrvToolkit.renderToSVG = Module.cwrap('vrvToolkit_renderToSVG', 'string', ['number', 'number', 'string']);

// char *renderToTimemap(Toolkit *ic)
VerovioEmscriptenAdapter.vrvToolkit.renderToTimemap = Module.cwrap('vrvToolkit_renderToTimemap', 'string', ['number']);

// void setOptions(Toolkit *ic, const char *options)
VerovioEmscriptenAdapter.vrvToolkit.setOptions = Module.cwrap('vrvToolkit_setOptions', null, ['number', 'string']);

// A pointer to the object - only one instance can be created for now
VerovioEmscriptenAdapter.instances = [];

export default VerovioEmscriptenAdapter;
