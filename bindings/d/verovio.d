module verovio;

import core.sys.windows.windows;
import core.runtime;
import std.conv : to;
import std.string : toStringz;

/// Verovio Toolkit
class VrvToolkit
{
    private void* _tkPtr;

    /**
    * Constructor
    */
    this()
    {
        _tkPtr = vrvToolkit_constructor();
        assert(_tkPtr !is null);
    }

    /**
    * Constructor
    * Params:
    * 	resourcePath = A string with the resource path
    */
    this(string resourcePath)
    {
        _tkPtr = vrvToolkit_constructorResourcePath(resourcePath.toStringz);
    }

    /**
    * Destructor
    */
    ~this()
    {
        vrvToolkit_destructor(_tkPtr);
    }

    /**
    * Edit the MEI data.
    * Params:
    * 	editorAction = The editor actions as a stringified JSON object
    * Returns: True if the edit action was successfully applied
    */
    bool edit(string editorAction)
    {
        return vrvToolkit_edit(_tkPtr, editorAction.toStringz);
    }

    /**
    * Return the editor status
    * Returns: The editor status as a string
    */
    string editInfo()
    {
        return to!string(vrvToolkit_editInfo(_tkPtr));
    }

    /**
    * Return all available options grouped by category.
    *
    * For each option, returns the type, the default value,
    * and the minimum and maximum value (when available).
    * Returns: A stringified JSON object
    */
    string getAvailableOptions()
    {
        return to!string(vrvToolkit_getAvailableOptions(_tkPtr));
    }

    /**
    * Return a dictionary of all the options with their default value.
    * Returns: A stringified JSON object
    */
    string getDefaultOptions()
    {
        return to!string(vrvToolkit_getDefaultOptions(_tkPtr));
    }

    /**
    * Return descriptive features as a JSON string.
    *
    * The features are tailored for implementing incipit search.
    * Params:
    * 	jsonOptions = A stringified JSON object with the feature extraction options
    * Returns: A stringified JSON object with the requested features
    */
    string getDescriptiveFeatures(string jsonOptions)
    {
        return to!string(vrvToolkit_getDescriptiveFeatures(_tkPtr, jsonOptions.toStringz));
    }

    /**
    * Return element attributes as a JSON string.
    *
    * The attributes returned include the ones not supported by Verovio.
    * Params:
    * 	xmlId = the ID (@xml:id) of the element being looked for
    * Returns: A stringified JSON object with all attributes
    */
    string getElementAttr(string xmlId)
    {
        return to!string(vrvToolkit_getElementAttr(_tkPtr, xmlId.toStringz));
    }

    /**
    * Return array of IDs of elements being currently played.
    * Params:
    * 	millisec = The time in milliseconds
    * Returns: A stringified JSON object with the page and notes being played
    */
    string getElementsAtTime(int millisec)
    {
        return to!string(vrvToolkit_getElementsAtTime(_tkPtr, millisec));
    }

    /**
    * Return a vector of ID strings of all elements (the notated and the expanded) for a given element.
    * Params:
    * 	xmlId = the ID (@xml:id) of the element being looked for
    * Returns: A stringified JSON object with all IDs
    */
    string getExpansionIdsForElement(string xmlId)
    {
        return to!string(vrvToolkit_getExpansionIdsForElement(_tkPtr, xmlId.toStringz));
    }

    /**
    * Get the humdrum buffer.
    * Returns: The humdrum buffer as a string
    */
    string getHumdrum()
    {
        return to!string(vrvToolkit_getHumdrum(_tkPtr));
    }

    /**
    * Write the humdrum buffer to the file.
    * Params:
    * 	filename = The output filename
    * Returns: True if the file was successfully written
    */
    bool getHumdrumFile(string filename)
    {
        return vrvToolkit_getHumdrumFile(_tkPtr, filename.toStringz);
    }

    /**
    * Return the ID of the Toolkit instance.
    * Returns: The ID as as string
    */
    string getID()
    {
        return to!string(vrvToolkit_getID(_tkPtr));
    }

    /**
    * Filter Humdrum data.
    * Params:
    * 	humdrumData =
    * Returns: The Humdrum data as a string
    */
    string convertHumdrumToHumdrum(string humdrumData)
    {
        return to!string(vrvToolkit_convertHumdrumToHumdrum(_tkPtr, humdrumData.toStringz));
    }

    /**
    * Convert Humdrum data to MIDI.
    * Params:
    * 	humdrumData =
    * Returns: The MIDI file as a base64-encoded string
    */
    string convertHumdrumToMIDI(string humdrumData)
    {
        return to!string(vrvToolkit_convertHumdrumToMIDI(_tkPtr, humdrumData.toStringz));
    }

    /**
    * Convert MEI data into Humdrum data.
    * Params:
    * 	meiData =
    * Returns: The Humdrum data as a string
    */
    string convertMEIToHumdrum(string meiData)
    {
        return to!string(vrvToolkit_convertMEIToHumdrum(_tkPtr, meiData.toStringz));
    }

    /**
    * Get the log content for the latest operation.
    * Returns: The log content as a string
    */
    string getLog()
    {
        return to!string(vrvToolkit_getLog(_tkPtr));
    }

    /**
    * Get the MEI as a string.
    * Params:
    * 	jsonOptions = A stringified JSON object with the output options; pageNo: integer; (1-based), all pages if none (or 0) specified; scoreBased: true or false; true by default; basic: true or false; false by default; removeIds: true or false; false by default - remove all @xml:id not used in the data;
    * Returns: 
    */
    string getMEI(string jsonOptions = "")
    {
        return to!string(vrvToolkit_getMEI(_tkPtr, jsonOptions.toStringz));
    }

    /**
    * Return MIDI values of the element with the ID (@xml:id)
    *
    * RenderToMIDI() must be called prior to using this method.
    * Params:
    * 	xmlId = the ID (@xml:id) of the element being looked for
    * Returns: A stringified JSON object with the MIDI values
    */
    string getMIDIValuesForElement(string xmlId)
    {
        return to!string(vrvToolkit_getMIDIValuesForElement(_tkPtr, xmlId.toStringz));
    }

    /**
    * Return the ID string of the notated (the original) element.
    * Params:
    * 	xmlId = the ID (@xml:id) of the element being looked for
    * Returns: An ID string
    */
    string getNotatedIdForElement(string xmlId)
    {
        return to!string(vrvToolkit_getNotatedIdForElement(_tkPtr, xmlId.toStringz));
    }

    /**
    * Return a dictionary of all the options with their current value.
    * Returns: A stringified JSON object
    */
    string getOptions()
    {
        return to!string(vrvToolkit_getOptions(_tkPtr));
    }

    /**
    * Get all usage for all option categories as string.
    * Returns:
    */
    string getOptionUsageString()
    {
        return to!string(vrvToolkit_getOptionUsageString(_tkPtr));
    }

    /**
    * Return the number of pages in the loaded document.
    * 
    * The number of pages depends one the page size and if encoded layout was taken into account or not.
    * Returns: The number of pages
    */
    int getPageCount()
    {
        return vrvToolkit_getPageCount(_tkPtr);
    }

    /**
    * Return the page on which the element is the ID (@xml:id) is rendered.
    * 
    * This takes into account the current layout options.
    * Params:
    * 	xmlId = the ID (@xml:id) of the element being looked for
    * Returns: the page number (1-based) where the element is (0 if not found)
    */
    int getPageWithElement(string xmlId)
    {
        return vrvToolkit_getPageWithElement(_tkPtr, xmlId.toStringz);
    }

    /**
    * Get the resource path for the Toolkit instance.
    * Returns: A string with the resource path
    */
    string getResourcePath()
    {
        return to!string(vrvToolkit_getResourcePath(_tkPtr));
    }

    /**
    * Get the scale option.
    * Returns: the scale option as integer
    */
    int getScale()
    {
        return vrvToolkit_getScale(_tkPtr);
    }

    /**
    * Return the time at which the element is the ID (@xml:id) is played.
    * 
    * RenderToMIDI() must be called prior to using this method.
    * Params:
    * 	xmlId = the ID (@xml:id) of the element being looked for
    * Returns: The time in milliseconds
    */
    double getTimeForElement(string xmlId)
    {
        return vrvToolkit_getTimeForElement(_tkPtr, xmlId.toStringz);
    }

    /**
    * Return a JSON object string with the following key values for a given note.
    * 
    * Return scoreTimeOnset, scoreTimeOffset, scoreTimeTiedDuration, realTimeOnsetMilliseconds, realTimeOffsetMilliseconds, realTimeTiedDurationMilliseconds.
    * Params:
    * 	xmlId = the ID (@xml:id) of the element being looked for
    * Returns: A stringified JSON object with the values
    */
    string getTimesForElement(string xmlId)
    {
        return to!string(vrvToolkit_getTimesForElement(_tkPtr, xmlId.toStringz));
    }

    /**
    * Return the version number.
    * Returns: the version number as a string
    */
    string getVersion()
    {
        return to!string(vrvToolkit_getVersion(_tkPtr));
    }

    /**
    * Load a string data with the type previously specified in the options.
    * 
    * By default, the methods try to auto-detect the type.
    * Params:
    * 	data = A string with the data (e.g., MEI data) to be loaded
    * Returns: True if the data was successfully loaded
    */
    bool loadData(string data)
    {
        return vrvToolkit_loadData(_tkPtr, data.toStringz);
    }

    /**
    * Load a file from the file system.
    * 
    * Previously convert UTF16 files to UTF8 or extract files from MusicXML compressed files.
    * Params:
    * 	filename = The filename to be loaded
    * Returns: True if the file was successfully loaded
    */
    bool loadFile(string filename)
    {
        return vrvToolkit_loadFile(_tkPtr, filename.toStringz);
    }

    /**
    * Load a MusicXML compressed file passed as base64 encoded string.
    * Params:
    * 	data = A ZIP file as a base64 encoded string
    * Returns: True if the data was successfully loaded
    */
    bool loadZipDataBase64(string data)
    {
        return vrvToolkit_loadZipDataBase64(_tkPtr, data.toStringz);
    }

    /**
    * Load a MusicXML compressed file passed as a buffer of bytes.
    * Params:
    * 	data = A ZIP file as a buffer of bytes
    * Returns: True if the data was successfully loaded
    */
    bool loadZipDataBuffer(ubyte[] data)
    {
        return vrvToolkit_loadZipDataBuffer(_tkPtr, &data[0], cast(int) data.length);
    }

    /**
    * Redo the layout of the loaded data.
    *
    * This can be called once the rendering option were changed, for example with a new page (sceen) height or a new zoom level.
    * Params:
    * 	jsonOptions = A stringified JSON object with the action options resetCache: true or false; true by default;
    */
    void redoLayout(string jsonOptions = "")
    {
        vrvToolkit_redoLayout(_tkPtr, jsonOptions.toStringz);
    }

    /**
    * Redo the layout of the pitch postitions of the current drawing page.
    *
    * Only the note vertical positions are recalculated with this method. RedoLayout() needs to be called for a full recalculation.
    */
    void redoPagePitchPosLayout()
    {
        vrvToolkit_redoPagePitchPosLayout(_tkPtr);
    }

    /**
    * Render the first page of the data to SVG.
    *
    * This method is a wrapper for setting options, loading data and rendering the first page. It will return an empty string if the options cannot be set or the data cannot be loaded.
    * Params:
    * 	data        = A string with the data (e.g., MEI data) to be loaded
    *   jsonOptions = A stringified JSON objects with the output options
    * Returns: True if the data was successfully loaded
    */
    string renderData(string data, string jsonOptions)
    {
        return to!string(vrvToolkit_renderData(_tkPtr, data.toStringz, jsonOptions.toStringz));
    }

    /**
    * Render a document’s expansionMap, if existing.
    * Returns: The expansionMap as a string
    */
    string renderToExpansionMap()
    {
        return to!string(vrvToolkit_renderToExpansionMap(_tkPtr));
    }

    /**
    * Render a document’s expansionMap and save it to a file.
    * Params:
    * 	filename = 	The output filename
    * Returns: True if the file was successfully written
    */
    bool renderToExpansionMapFile(string filename)
    {
        return vrvToolkit_renderToExpansionMapFile(_tkPtr, filename.toStringz);
    }

    /**
    * Render the document to MIDI.
    * Returns: A MIDI file as a base64 encoded string
    */
    string renderToMIDI()
    {
        // Does the method has an argument?
        return to!string(vrvToolkit_renderToMIDI(_tkPtr));
    }

    /**
    * Render a document to MIDI and save it to the file.
    * Params:
    * 	filename = 	The output filename
    * Returns: True if the file was successfully written
    */
    bool renderToMIDIFile(string filename)
    {
        return vrvToolkit_renderToMIDIFile(_tkPtr, filename.toStringz);
    }

    /**
    * Render a document to Plaine & Easie code.
    *
    * Only the top staff / layer is exported.
    * Returns: The PAE as a string
    */
    string renderToPAE()
    {
        return to!string(vrvToolkit_renderToPAE(_tkPtr));
    }

    /**
    * Render a document to Plaine & Easie code and save it to the file.
    *
    * Only the top staff / layer is exported.
    * Params:
    * 	filename = The output filename
    * Returns: True if the file was successfully written
    */
    bool renderToPAEFile(string filename)
    {
        return vrvToolkit_renderToPAEFile(_tkPtr, filename.toStringz);
    }

    /**
    * Render a page to SVG.
    * Params:
    * 	pageNo         = The page to render (1-based)
    *   xmlDeclaration = True for including the xml declaration in the SVG output
    * Returns: The SVG page as a string
    */
    string renderToSVG(int pageNo = 1, bool xmlDeclaration = false)
    {
        return to!string(vrvToolkit_renderToSVG(_tkPtr, pageNo, xmlDeclaration));
    }

    /**
    * Render a page to SVG and save it to the file.
    * Params:
    * 	filename = The output filename
    *   pageNo   = The page to render (1-based)
    * Returns: True if the file was successfully written
    */
    bool renderToSVGFile(string filename, int pageNo = 1)
    {
        return vrvToolkit_renderToSVGFile(_tkPtr, filename.toStringz, pageNo);
    }

    /**
    * Render a document to a timemap.
    * Params:
    * 	jsonOptions = A stringified JSON objects with the timemap options
    * Returns: The timemap as a string
    */
    string renderToTimemap(string jsonOptions = "")
    {
        return to!string(vrvToolkit_renderToTimemap(_tkPtr, jsonOptions.toStringz));
    }

    /**
    * Render a document to timemap and save it to the file.
    * Params:
    * 	filename    = The output filename
    *   jsonOptions = A stringified JSON objects with the timemap options
    * Returns: True if the file was successfully written
    */
    bool renderToTimemapFile(string filename, string jsonOptions = "")
    {
        return vrvToolkit_renderToTimemapFile(_tkPtr, filename.toStringz, jsonOptions.toStringz);
    }

    /**
    * Reset all options to default values.
    */
    void resetOptions()
    {
        vrvToolkit_resetOptions(_tkPtr);
    }

    /**
    * Reset the seed used to generate MEI @xml:id attribute values.
    *
    * Passing 0 will seed the @xml:id generator with a random (time-based) seed value. This method will have no effect if the xml-id-checksum option is set.
    * Params:
    * 	seed = The seed value for generating the @xml:id values (0 for a time-based random seed)
    */
    void resetXmlIdSeed(int seed)
    {
        vrvToolkit_resetXmlIdSeed(_tkPtr, seed);
    }

    /**
    * Get the MEI and save it to the file.
    * Params:
    * 	filename    = The output filename
    *   jsonOptions = A stringified JSON object with the output options
    * Returns: True if the file was successfully written
    */
    bool saveFile(string filename, string jsonOptions = "")
    {
        return vrvToolkit_saveFile(_tkPtr, filename.toStringz, jsonOptions.toStringz);
    }

    /**
    * Set the value for a selection.
    *
    * The selection will be applied only when some data is loaded or the layout is redone. The selection can be reset (cancelled) by passing an empty string or an empty JSON object. A selection across multiple mdivs is not possible.
    * Params:
    * 	selection = The selection as a stringified JSON object
    * Returns: True if the selection was successfully parsed or reset
    */
    bool select(string selection)
    {
        return vrvToolkit_select(_tkPtr, selection.toStringz);
    }

    /**
    * Set the input from option.
    * Params:
    * 	inputFrom = the input from value as string
    * Returns: True if the option was successfully set
    */
    bool setInputFrom(string inputFrom)
    {
        return vrvToolkit_setInputFrom(_tkPtr, inputFrom.toStringz);
    }

    /**
    * Set option values.
    *
    * The name of each option to be set is to be given as JSON key.
    * Params:
    * 	jsonOptions = A stringified JSON objects with the output options
    * Returns: True if the options were successfully set
    */
    bool setOptions(string jsonOptions)
    {
        return vrvToolkit_setOptions(_tkPtr, jsonOptions.toStringz);
    }

    /**
    * Set the output to option.
    * Params:
    * 	outputTo = the output to value as string
    * Returns: True if the option was successfully set
    */
    bool setOutputTo(string outputTo)
    {
        return vrvToolkit_setOutputTo(_tkPtr, outputTo.toStringz);
    }

    /**
    * Set the resource path for the Toolkit instance.
    *
    * This method needs to be called if the constructor had initFont=false or if the resource path needs to be changed.
    * Params:
    * 	path = The path to the resource directory
    * Returns: True if the resources was successfully loaded
    */
    bool setResourcePath(string path)
    {
        return vrvToolkit_setResourcePath(_tkPtr, path.toStringz);
    }

    /**
    * Set the scale option.
    * Params:
    * 	scale = the scale value as integer
    * Returns: True if the option was successfully set
    */
    bool setScale(int scale)
    {
        return vrvToolkit_setScale(_tkPtr, scale);
    }

    /**
    * Validate the Plaine & Easie code passed in the string data.
    *
    * A single JSON object is returned when there is a global input error. When reading the input succeeds, validation is grouped by input keys. The methods always returns errors in PAE pedantic mode. No data remains loaded after the validation.
    * Params:
    * 	data = A string with the data in JSON or with PAE @ keys
    * Returns: A stringified JSON object with the validation warnings or errors
    */
    string validatePAE(string data)
    {
        return to!string(vrvToolkit_validatePAE(_tkPtr, data.toStringz));
    }

    /**
    * Validate the Plaine & Easie code from a file.
    *
    * The method calls validatePAE.
    * Params:
    * 	filename = The filename to be validated
    * Returns: A stringified JSON object with the validation warnings or errors
    */
    string validatePAEFile(string filename)
    {
        return to!string(vrvToolkit_validatePAEFile(_tkPtr, filename.toStringz));
    }
}

private extern (C) nothrow @nogc
{
    alias fa_enableLog = void function(bool value);
    alias fa_enableLogToBuffer = void function(bool value);
    alias fa_vrvToolkit_constructor = void* function();
    alias fa_vrvToolkit_constructorResourcePath = void* function(const(char)* resourcePath);
    alias fa_vrvToolkit_destructor = void function(void* tkPtr);
    alias fa_vrvToolkit_edit = bool function(void* tkPtr, const(char)* editorAction);
    alias fa_vrvToolkit_editInfo = const(char)* function(void* tkPtr);
    alias fa_vrvToolkit_getAvailableOptions = const(char)* function(void* tkPtr);
    alias fa_vrvToolkit_getDefaultOptions = const(char)* function(void* tkPtr);
    alias fa_vrvToolkit_getDescriptiveFeatures = const(char)* function(void* tkPtr, const(char)* options);
    alias fa_vrvToolkit_getElementAttr = const(char)* function(void* tkPtr, const(char)* xmlId);
    alias fa_vrvToolkit_getElementsAtTime = const(char)* function(void* tkPtr, int millisec);
    alias fa_vrvToolkit_getExpansionIdsForElement = const(char)* function(void* tkPtr, const(char)* xmlId);
    alias fa_vrvToolkit_getHumdrum = const(char)* function(void* tkPtr);
    alias fa_vrvToolkit_getHumdrumFile = bool function(void* tkPtr, const(char)* filename);
    alias fa_vrvToolkit_getID = const(char)* function(void* tkPtr);
    alias fa_vrvToolkit_convertHumdrumToHumdrum = const(char)* function(void* tkPtr, const(char)* humdrumData);
    alias fa_vrvToolkit_convertHumdrumToMIDI = const(char)* function(void* tkPtr, const(char)* humdrumData);
    alias fa_vrvToolkit_convertMEIToHumdrum = const(char)* function(void* tkPtr, const(char)* meiData);
    alias fa_vrvToolkit_getLog = const(char)* function(void* tkPtr);
    alias fa_vrvToolkit_getMEI = const(char)* function(void* tkPtr, const(char)* options);
    alias fa_vrvToolkit_getMIDIValuesForElement = const(char)* function(void* tkPtr, const(char)* xmlId);
    alias fa_vrvToolkit_getNotatedIdForElement = const(char)* function(void* tkPtr, const(char)* xmlId);
    alias fa_vrvToolkit_getOptions = const(char)* function(void* tkPtr);
    alias fa_vrvToolkit_getOptionUsageString = const(char)* function(void* tkPtr);
    alias fa_vrvToolkit_getPageCount = int function(void* tkPtr);
    alias fa_vrvToolkit_getPageWithElement = int function(void* tkPtr, const(char)* xmlId);
    alias fa_vrvToolkit_getResourcePath = const(char)* function(void* tkPtr);
    alias fa_vrvToolkit_getScale = int function(void* tkPtr);
    alias fa_vrvToolkit_getTimeForElement = double function(void* tkPtr, const(char)* xmlId);
    alias fa_vrvToolkit_getTimesForElement = const(char)* function(void* tkPtr, const(char)* xmlId);
    alias fa_vrvToolkit_getVersion = const(char)* function(void* tkPtr);
    alias fa_vrvToolkit_loadData = bool function(void* tkPtr, const(char)* data);
    alias fa_vrvToolkit_loadFile = bool function(void* tkPtr, const(char)* filename);
    alias fa_vrvToolkit_loadZipDataBase64 = bool function(void* tkPtr, const(char)* data);
    alias fa_vrvToolkit_loadZipDataBuffer = bool function(void* tkPtr, const(ubyte)* data, int length);
    alias fa_vrvToolkit_redoLayout = void function(void* tkPtr, const(char)* c_options);
    alias fa_vrvToolkit_redoPagePitchPosLayout = void function(void* tkPtr);
    alias fa_vrvToolkit_renderData = const(char)* function(void* tkPtr, const(char)* data, const(
            char)* options);
    alias fa_vrvToolkit_renderToExpansionMap = const(char)* function(void* tkPtr);
    alias fa_vrvToolkit_renderToExpansionMapFile = bool function(void* tkPtr, const(char)* filename);
    alias fa_vrvToolkit_renderToMIDI = const(char)* function(void* tkPtr);
    alias fa_vrvToolkit_renderToMIDIFile = bool function(void* tkPtr, const(char)* filename);
    alias fa_vrvToolkit_renderToPAE = const(char)* function(void* tkPtr);
    alias fa_vrvToolkit_renderToPAEFile = bool function(void* tkPtr, const(char)* filename);
    alias fa_vrvToolkit_renderToSVG = const(char)* function(void* tkPtr, int page_no, bool xmlDeclaration);
    alias fa_vrvToolkit_renderToSVGFile = bool function(void* tkPtr, const(char)* filename, int pageNo);
    alias fa_vrvToolkit_renderToTimemap = const(char)* function(void* tkPtr, const(char)* c_options);
    alias fa_vrvToolkit_renderToTimemapFile = bool function(void* tkPtr, const(char)* filename, const(
            char)* c_options);
    alias fa_vrvToolkit_resetOptions = void function(void* tkPtr);
    alias fa_vrvToolkit_resetXmlIdSeed = void function(void* tkPtr, int seed);
    alias fa_vrvToolkit_saveFile = bool function(void* tkPtr, const(char)* filename, const(char)* c_options);
    alias fa_vrvToolkit_select = bool function(void* tkPtr, const(char)* selection);
    alias fa_vrvToolkit_setInputFrom = bool function(void* tkPtr, const(char)* inputFrom);
    alias fa_vrvToolkit_setOptions = bool function(void* tkPtr, const(char)* options);
    alias fa_vrvToolkit_setOutputTo = bool function(void* tkPtr, const(char)* outputTo);
    alias fa_vrvToolkit_setResourcePath = bool function(void* tkPtr, const(char)* path);
    alias fa_vrvToolkit_setScale = bool function(void* tkPtr, int scale);
    alias fa_vrvToolkit_validatePAE = const(char)* function(void* tkPtr, const(char)* data);
    alias fa_vrvToolkit_validatePAEFile = const(char)* function(void* tkPtr, const(char)* filename);
}

__gshared fa_enableLog enableLog;
__gshared fa_enableLogToBuffer enableLogToBuffer;

private
{
    __gshared fa_vrvToolkit_constructor vrvToolkit_constructor;
    __gshared fa_vrvToolkit_constructorResourcePath vrvToolkit_constructorResourcePath;
    __gshared fa_vrvToolkit_destructor vrvToolkit_destructor;
    __gshared fa_vrvToolkit_edit vrvToolkit_edit;
    __gshared fa_vrvToolkit_editInfo vrvToolkit_editInfo;
    __gshared fa_vrvToolkit_getAvailableOptions vrvToolkit_getAvailableOptions;
    __gshared fa_vrvToolkit_getDefaultOptions vrvToolkit_getDefaultOptions;
    __gshared fa_vrvToolkit_getDescriptiveFeatures vrvToolkit_getDescriptiveFeatures;
    __gshared fa_vrvToolkit_getElementAttr vrvToolkit_getElementAttr;
    __gshared fa_vrvToolkit_getElementsAtTime vrvToolkit_getElementsAtTime;
    __gshared fa_vrvToolkit_getExpansionIdsForElement vrvToolkit_getExpansionIdsForElement;
    __gshared fa_vrvToolkit_getHumdrum vrvToolkit_getHumdrum;
    __gshared fa_vrvToolkit_getHumdrumFile vrvToolkit_getHumdrumFile;
    __gshared fa_vrvToolkit_getID vrvToolkit_getID;
    __gshared fa_vrvToolkit_convertHumdrumToHumdrum vrvToolkit_convertHumdrumToHumdrum;
    __gshared fa_vrvToolkit_convertHumdrumToMIDI vrvToolkit_convertHumdrumToMIDI;
    __gshared fa_vrvToolkit_convertMEIToHumdrum vrvToolkit_convertMEIToHumdrum;
    __gshared fa_vrvToolkit_getLog vrvToolkit_getLog;
    __gshared fa_vrvToolkit_getMEI vrvToolkit_getMEI;
    __gshared fa_vrvToolkit_getMIDIValuesForElement vrvToolkit_getMIDIValuesForElement;
    __gshared fa_vrvToolkit_getNotatedIdForElement vrvToolkit_getNotatedIdForElement;
    __gshared fa_vrvToolkit_getOptions vrvToolkit_getOptions;
    __gshared fa_vrvToolkit_getOptionUsageString vrvToolkit_getOptionUsageString;
    __gshared fa_vrvToolkit_getPageCount vrvToolkit_getPageCount;
    __gshared fa_vrvToolkit_getPageWithElement vrvToolkit_getPageWithElement;
    __gshared fa_vrvToolkit_getResourcePath vrvToolkit_getResourcePath;
    __gshared fa_vrvToolkit_getScale vrvToolkit_getScale;
    __gshared fa_vrvToolkit_getTimeForElement vrvToolkit_getTimeForElement;
    __gshared fa_vrvToolkit_getTimesForElement vrvToolkit_getTimesForElement;
    __gshared fa_vrvToolkit_getVersion vrvToolkit_getVersion;
    __gshared fa_vrvToolkit_loadData vrvToolkit_loadData;
    __gshared fa_vrvToolkit_loadFile vrvToolkit_loadFile;
    __gshared fa_vrvToolkit_loadZipDataBase64 vrvToolkit_loadZipDataBase64;
    __gshared fa_vrvToolkit_loadZipDataBuffer vrvToolkit_loadZipDataBuffer;
    __gshared fa_vrvToolkit_redoLayout vrvToolkit_redoLayout;
    __gshared fa_vrvToolkit_redoPagePitchPosLayout vrvToolkit_redoPagePitchPosLayout;
    __gshared fa_vrvToolkit_renderData vrvToolkit_renderData;
    __gshared fa_vrvToolkit_renderToExpansionMap vrvToolkit_renderToExpansionMap;
    __gshared fa_vrvToolkit_renderToExpansionMapFile vrvToolkit_renderToExpansionMapFile;
    __gshared fa_vrvToolkit_renderToMIDI vrvToolkit_renderToMIDI;
    __gshared fa_vrvToolkit_renderToMIDIFile vrvToolkit_renderToMIDIFile;
    __gshared fa_vrvToolkit_renderToPAE vrvToolkit_renderToPAE;
    __gshared fa_vrvToolkit_renderToPAEFile vrvToolkit_renderToPAEFile;
    __gshared fa_vrvToolkit_renderToSVG vrvToolkit_renderToSVG;
    __gshared fa_vrvToolkit_renderToSVGFile vrvToolkit_renderToSVGFile;
    __gshared fa_vrvToolkit_renderToTimemap vrvToolkit_renderToTimemap;
    __gshared fa_vrvToolkit_renderToTimemapFile vrvToolkit_renderToTimemapFile;
    __gshared fa_vrvToolkit_resetOptions vrvToolkit_resetOptions;
    __gshared fa_vrvToolkit_resetXmlIdSeed vrvToolkit_resetXmlIdSeed;
    __gshared fa_vrvToolkit_saveFile vrvToolkit_saveFile;
    __gshared fa_vrvToolkit_select vrvToolkit_select;
    __gshared fa_vrvToolkit_setInputFrom vrvToolkit_setInputFrom;
    __gshared fa_vrvToolkit_setOptions vrvToolkit_setOptions;
    __gshared fa_vrvToolkit_setOutputTo vrvToolkit_setOutputTo;
    __gshared fa_vrvToolkit_setResourcePath vrvToolkit_setResourcePath;
    __gshared fa_vrvToolkit_setScale vrvToolkit_setScale;
    __gshared fa_vrvToolkit_validatePAE vrvToolkit_validatePAE;
    __gshared fa_vrvToolkit_validatePAEFile vrvToolkit_validatePAEFile;
}

static this()
{
    void* lib;

    version (Windows)
    {
        lib = Runtime.loadLibrary("verovio.dll");
    }
    else
    {
        lib = Runtime.loadLibrary("libverovio.so");
    }

    assert(lib !is null, "Library not valid");

    enableLog = cast(fa_enableLog) GetProcAddress(lib, "enableLog");
    enableLogToBuffer = cast(fa_enableLogToBuffer) GetProcAddress(lib, "enableLogToBuffer");
    vrvToolkit_constructor = cast(fa_vrvToolkit_constructor) GetProcAddress(lib, "vrvToolkit_constructor");
    vrvToolkit_constructorResourcePath = cast(fa_vrvToolkit_constructorResourcePath) GetProcAddress(lib, "vrvToolkit_constructorResourcePath");
    vrvToolkit_destructor = cast(fa_vrvToolkit_destructor) GetProcAddress(lib, "vrvToolkit_destructor");
    vrvToolkit_edit = cast(fa_vrvToolkit_edit) GetProcAddress(lib, "vrvToolkit_edit");
    vrvToolkit_editInfo = cast(fa_vrvToolkit_editInfo) GetProcAddress(lib, "vrvToolkit_editInfo");
    vrvToolkit_getAvailableOptions = cast(fa_vrvToolkit_getAvailableOptions) GetProcAddress(lib, "vrvToolkit_getAvailableOptions");
    vrvToolkit_getDefaultOptions = cast(fa_vrvToolkit_getDefaultOptions) GetProcAddress(lib, "vrvToolkit_getDefaultOptions");
    vrvToolkit_getDescriptiveFeatures = cast(fa_vrvToolkit_getDescriptiveFeatures) GetProcAddress(lib, "vrvToolkit_getDescriptiveFeatures");
    vrvToolkit_getElementAttr = cast(fa_vrvToolkit_getElementAttr) GetProcAddress(lib, "vrvToolkit_getElementAttr");
    vrvToolkit_getElementsAtTime = cast(fa_vrvToolkit_getElementsAtTime) GetProcAddress(lib, "vrvToolkit_getElementsAtTime");
    vrvToolkit_getExpansionIdsForElement = cast(fa_vrvToolkit_getExpansionIdsForElement) GetProcAddress(lib, "vrvToolkit_getExpansionIdsForElement");
    vrvToolkit_getHumdrum = cast(fa_vrvToolkit_getHumdrum) GetProcAddress(lib, "vrvToolkit_getHumdrum");
    vrvToolkit_getHumdrumFile = cast(fa_vrvToolkit_getHumdrumFile) GetProcAddress(lib, "vrvToolkit_getHumdrumFile");
    vrvToolkit_getID = cast(fa_vrvToolkit_getID) GetProcAddress(lib, "vrvToolkit_getID");
    vrvToolkit_convertHumdrumToHumdrum = cast(fa_vrvToolkit_convertHumdrumToHumdrum) GetProcAddress(lib, "vrvToolkit_convertHumdrumToHumdrum");
    vrvToolkit_convertHumdrumToMIDI = cast(fa_vrvToolkit_convertHumdrumToMIDI) GetProcAddress(lib, "vrvToolkit_convertHumdrumToMIDI");
    vrvToolkit_convertMEIToHumdrum = cast(fa_vrvToolkit_convertMEIToHumdrum) GetProcAddress(lib, "vrvToolkit_convertMEIToHumdrum");
    vrvToolkit_getLog = cast(fa_vrvToolkit_getLog) GetProcAddress(lib, "vrvToolkit_getLog");
    vrvToolkit_getMEI = cast(fa_vrvToolkit_getMEI) GetProcAddress(lib, "vrvToolkit_getMEI");
    vrvToolkit_getMIDIValuesForElement = cast(fa_vrvToolkit_getMIDIValuesForElement) GetProcAddress(lib, "vrvToolkit_getMIDIValuesForElement");
    vrvToolkit_getNotatedIdForElement = cast(fa_vrvToolkit_getNotatedIdForElement) GetProcAddress(lib, "vrvToolkit_getNotatedIdForElement");
    vrvToolkit_getOptions = cast(fa_vrvToolkit_getOptions) GetProcAddress(lib, "vrvToolkit_getOptions");
    vrvToolkit_getOptionUsageString = cast(fa_vrvToolkit_getOptionUsageString) GetProcAddress(lib, "vrvToolkit_getOptionUsageString");
    vrvToolkit_getPageCount = cast(fa_vrvToolkit_getPageCount) GetProcAddress(lib, "vrvToolkit_getPageCount");
    vrvToolkit_getPageWithElement = cast(fa_vrvToolkit_getPageWithElement) GetProcAddress(lib, "vrvToolkit_getPageWithElement");
    vrvToolkit_getResourcePath = cast(fa_vrvToolkit_getResourcePath) GetProcAddress(lib, "vrvToolkit_getResourcePath");
    vrvToolkit_getScale = cast(fa_vrvToolkit_getScale) GetProcAddress(lib, "vrvToolkit_getScale");
    vrvToolkit_getTimeForElement = cast(fa_vrvToolkit_getTimeForElement) GetProcAddress(lib, "vrvToolkit_getTimeForElement");
    vrvToolkit_getTimesForElement = cast(fa_vrvToolkit_getTimesForElement) GetProcAddress(lib, "vrvToolkit_getTimesForElement");
    vrvToolkit_getVersion = cast(fa_vrvToolkit_getVersion) GetProcAddress(lib, "vrvToolkit_getVersion");
    vrvToolkit_loadData = cast(fa_vrvToolkit_loadData) GetProcAddress(lib, "vrvToolkit_loadData");
    vrvToolkit_loadFile = cast(fa_vrvToolkit_loadFile) GetProcAddress(lib, "vrvToolkit_loadFile");
    vrvToolkit_loadZipDataBase64 = cast(fa_vrvToolkit_loadZipDataBase64) GetProcAddress(lib, "vrvToolkit_loadZipDataBase64");
    vrvToolkit_loadZipDataBuffer = cast(fa_vrvToolkit_loadZipDataBuffer) GetProcAddress(lib, "vrvToolkit_loadZipDataBuffer");
    vrvToolkit_redoLayout = cast(fa_vrvToolkit_redoLayout) GetProcAddress(lib, "vrvToolkit_redoLayout");
    vrvToolkit_redoPagePitchPosLayout = cast(fa_vrvToolkit_redoPagePitchPosLayout) GetProcAddress(lib, "vrvToolkit_redoPagePitchPosLayout");
    vrvToolkit_renderData = cast(fa_vrvToolkit_renderData) GetProcAddress(lib, "vrvToolkit_renderData");
    vrvToolkit_renderToExpansionMap = cast(fa_vrvToolkit_renderToExpansionMap) GetProcAddress(lib, "vrvToolkit_renderToExpansionMap");
    vrvToolkit_renderToExpansionMapFile = cast(fa_vrvToolkit_renderToExpansionMapFile) GetProcAddress(lib, "vrvToolkit_renderToExpansionMapFile");
    vrvToolkit_renderToMIDI = cast(fa_vrvToolkit_renderToMIDI) GetProcAddress(lib, "vrvToolkit_renderToMIDI");
    vrvToolkit_renderToMIDIFile = cast(fa_vrvToolkit_renderToMIDIFile) GetProcAddress(lib, "vrvToolkit_renderToMIDIFile");
    vrvToolkit_renderToPAE = cast(fa_vrvToolkit_renderToPAE) GetProcAddress(lib, "vrvToolkit_renderToPAE");
    vrvToolkit_renderToPAEFile = cast(fa_vrvToolkit_renderToPAEFile) GetProcAddress(lib, "vrvToolkit_renderToPAEFile");
    vrvToolkit_renderToSVG = cast(fa_vrvToolkit_renderToSVG) GetProcAddress(lib, "vrvToolkit_renderToSVG");
    vrvToolkit_renderToSVGFile = cast(fa_vrvToolkit_renderToSVGFile) GetProcAddress(lib, "vrvToolkit_renderToSVGFile");
    vrvToolkit_renderToTimemap = cast(fa_vrvToolkit_renderToTimemap) GetProcAddress(lib, "vrvToolkit_renderToTimemap");
    vrvToolkit_renderToTimemapFile = cast(fa_vrvToolkit_renderToTimemapFile) GetProcAddress(lib, "vrvToolkit_renderToTimemapFile");
    vrvToolkit_resetOptions = cast(fa_vrvToolkit_resetOptions) GetProcAddress(lib, "vrvToolkit_resetOptions");
    vrvToolkit_resetXmlIdSeed = cast(fa_vrvToolkit_resetXmlIdSeed) GetProcAddress(lib, "vrvToolkit_resetXmlIdSeed");
    vrvToolkit_saveFile = cast(fa_vrvToolkit_saveFile) GetProcAddress(lib, "vrvToolkit_saveFile");
    vrvToolkit_select = cast(fa_vrvToolkit_select) GetProcAddress(lib, "vrvToolkit_select");
    vrvToolkit_setInputFrom = cast(fa_vrvToolkit_setInputFrom) GetProcAddress(lib, "vrvToolkit_setInputFrom");
    vrvToolkit_setOptions = cast(fa_vrvToolkit_setOptions) GetProcAddress(lib, "vrvToolkit_setOptions");
    vrvToolkit_setOutputTo = cast(fa_vrvToolkit_setOutputTo) GetProcAddress(lib, "vrvToolkit_setOutputTo");
    vrvToolkit_setResourcePath = cast(fa_vrvToolkit_setResourcePath) GetProcAddress(lib, "vrvToolkit_setResourcePath");
    vrvToolkit_setScale = cast(fa_vrvToolkit_setScale) GetProcAddress(lib, "vrvToolkit_setScale");
    vrvToolkit_validatePAE = cast(fa_vrvToolkit_validatePAE) GetProcAddress(lib, "vrvToolkit_validatePAE");
    vrvToolkit_validatePAEFile = cast(fa_vrvToolkit_validatePAEFile) GetProcAddress(lib, "vrvToolkit_validatePAEFile");
}
