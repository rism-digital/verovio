
import { createEmscriptenProxy } from './emscripten-proxy.js';

export const VerovioToolkit = function(VerovioModule)
{
    this.VerovioModule = VerovioModule;
    if (!this.VerovioModule) {
        throw new Error('VerovioToolkit could not find emscripten module.');
    }
    this.proxy = createEmscriptenProxy(this.VerovioModule);
    this.ptr = this.proxy.constructor();
    console.debug('Creating toolkit instance');
    VerovioToolkit.instances.push(this.ptr);
}

// A pointer to the object - only one instance can be created for now
VerovioToolkit.instances = [];


VerovioToolkit.prototype.destroy = function()
{
    VerovioToolkit.instances.splice(VerovioToolkit.instances.indexOf(this.ptr), 1);
    console.debug('Deleting toolkit instance');
    this.proxy.destructor(sthis.ptr);
};

VerovioToolkit.prototype.edit = function(editorAction)
{
    return this.proxy.edit(this.ptr, JSON.stringify(editorAction));
};

VerovioToolkit.prototype.editInfo = function()
{
    return JSON.parse(this.proxy.editInfo(this.ptr));
};

VerovioToolkit.prototype.getAvailableOptions = function()
{
    return JSON.parse(this.proxy.getAvailableOptions(this.ptr));
};

VerovioToolkit.prototype.getDescriptiveFeatures = function(options)
{
    return JSON.parse(this.proxy.getDescriptiveFeatures(this.ptr, JSON.stringify(options)));
};

VerovioToolkit.prototype.getElementAttr = function(xmlId)
{
    return JSON.parse(this.proxy.getElementAttr(this.ptr, xmlId));
};

VerovioToolkit.prototype.getElementsAtTime = function(millisec)
{
    return JSON.parse(this.proxy.getElementsAtTime(this.ptr, millisec));
};

VerovioToolkit.prototype.getExpansionIdsForElement = function(xmlId)
{
    return JSON.parse(this.proxy.getExpansionIdsForElement(this.ptr, xmlId));
};

VerovioToolkit.prototype.getHumdrum = function()
{
    return this.proxy.getHumdrum(this.ptr);
};

VerovioToolkit.prototype.convertHumdrumToHumdrum = function(data)
{
    return this.proxy.convertHumdrumToHumdrum(this.ptr, data);
};

VerovioToolkit.prototype.convertHumdrumToMIDI = function(data)
{
    return this.proxy.convertHumdrumToMIDI(this.ptr, data);
};

VerovioToolkit.prototype.convertMEIToHumdrum = function(data)
{
    return this.proxy.convertMEIToHumdrum(this.ptr, data);
};

VerovioToolkit.prototype.getLog = function()
{
    return this.proxy.getLog(this.ptr);
};

VerovioToolkit.prototype.getMEI = function(options = {})
{
    return this.proxy.getMEI(this.ptr, JSON.stringify(options));
};

VerovioToolkit.prototype.getMIDIValuesForElement = function(xmlId)
{
    return JSON.parse(this.proxy.getMIDIValuesForElement(this.ptr, xmlId));
};

VerovioToolkit.prototype.getNotatedIdForElement = function(xmlId)
{
    return this.proxy.getNotatedIdForElement(this.ptr, xmlId);
};

VerovioToolkit.prototype.getOptions = function(defaultValues)
{
    return JSON.parse(this.proxy.getOptions(this.ptr, defaultValues));
};

VerovioToolkit.prototype.getPageCount = function()
{
    return this.proxy.getPageCount(this.ptr);
};

VerovioToolkit.prototype.getPageWithElement = function(xmlId)
{
    return this.proxy.getPageWithElement(this.ptr, xmlId);
};

VerovioToolkit.prototype.getTimeForElement = function(xmlId)
{
    return this.proxy.getTimeForElement(this.ptr, xmlId);
};

VerovioToolkit.prototype.getTimesForElement = function(xmlId)
{
    return JSON.parse(this.proxy.getTimesForElement(this.ptr, xmlId));
};

VerovioToolkit.prototype.getVersion = function()
{
    return this.proxy.getVersion(this.ptr);
};

VerovioToolkit.prototype.loadData = function(data)
{
    return this.proxy.loadData(this.ptr, data);
};

VerovioToolkit.prototype.loadZipDataBase64 = function(data)
{
    return this.proxy.loadZipDataBase64(this.ptr, data);
};

VerovioToolkit.prototype.loadZipDataBuffer = function(data)
{
    if (!(data instanceof ArrayBuffer))
    {
        console.error('Parameter for loadZipDataBuffer has to be of type ArrayBuffer');
        return false;
    }
    var dataArray = new Uint8Array(data); 
    var dataSize = dataArray.length * dataArray.BYTES_PER_ELEMENT;
    var dataPtr = Module._malloc(dataSize);
    Module.HEAPU8.set(dataArray, dataPtr);
    var res = this.proxy.loadZipDataBuffer(this.ptr, dataPtr, dataSize);
    Module._free(dataPtr);
    return res;
};

VerovioToolkit.prototype.redoLayout = function(options = {})
{
    this.proxy.redoLayout(this.ptr, JSON.stringify(options));
}

VerovioToolkit.prototype.redoPagePitchPosLayout = function()
{
    this.proxy.redoPagePitchPosLayout(this.ptr);
}

VerovioToolkit.prototype.renderData = function(data, options)
{
    return this.proxy.renderData(this.ptr, data, JSON.stringify(options));
};

VerovioToolkit.prototype.renderPage = function(pageNo, options)
{
    console.warn('Method renderPage is deprecated; use renderToSVG instead');
    return this.proxy.renderToSVG(this.ptr, pageNo, JSON.stringify(options));
};

VerovioToolkit.prototype.renderToMIDI = function(options)
{
    return this.proxy.renderToMIDI(this.ptr, JSON.stringify(options));
};

VerovioToolkit.prototype.renderToMidi = function(options)
{
    console.warn('Method renderToMidi is deprecated; use renderToMIDI instead');
    return this.proxy.renderToMIDI(this.ptr, JSON.stringify(options));
};

VerovioToolkit.prototype.renderToPAE = function()
{
    return this.proxy.renderToPAE(this.ptr);
};

VerovioToolkit.prototype.renderToSVG = function(pageNo = 1, xmlDeclaration = false)
{
    return this.proxy.renderToSVG(this.ptr, pageNo, xmlDeclaration);
};

VerovioToolkit.prototype.renderToTimemap = function(options = {})
{
    return JSON.parse(this.proxy.renderToTimemap(this.ptr, JSON.stringify(options)));
};

VerovioToolkit.prototype.resetOptions = function()
{
    this.proxy.resetOptions(this.ptr);
}

VerovioToolkit.prototype.resetXmlIdSeed = function(seed)
{
    return this.proxy.resetXmlIdSeed(this.ptr, seed);
};

VerovioToolkit.prototype.select = function(selection)
{
    return this.proxy.select(this.ptr, JSON.stringify(selection));
};

VerovioToolkit.prototype.setOptions = function(options)
{
    this.proxy.setOptions(this.ptr, JSON.stringify(options));
};

VerovioToolkit.prototype.validatePAE = function(data)
{
    if (data instanceof Object)
    {
        data = JSON.stringify(data)
    }
    return JSON.parse(this.proxy.validatePAE(this.ptr, data));
};

/***************************************************************************************************************************/

// If the window object is defined (if we are not within a WebWorker)...
if ((typeof window !== 'undefined') && (window.addEventListener))
{
    // Add a listener that will delete the object (if necessary) when the page is closed
    window.addEventListener('unload', function()
    {
        for (var i = 0; i < VerovioToolkit.instances.length; i++)
        {
            this.proxy.destructor(instances[i]);
        }
    });
}
