/***************************************************************************************************************************/
// Proxy the exported c++ methods
verovio.vrvToolkit = verovio.vrvToolkit || {};

// Constructor and destructor
// Toolkit *constructor()
verovio.vrvToolkit.constructor = Module.cwrap( 'vrvToolkit_constructor', 'number', [] );

// void destructor(Toolkit *ic)
verovio.vrvToolkit.destructor = Module.cwrap( 'vrvToolkit_destructor', null, ['number'] );

// bool edit(Toolkit *ic, const char *editorAction) 
verovio.vrvToolkit.edit = Module.cwrap( 'vrvToolkit_edit', 'number', ['number', 'string'] );

// char *editInfo(Toolkit *ic)
verovio.vrvToolkit.editInfo = Module.cwrap( 'vrvToolkit_editInfo', 'string', ['number'] );

// char *getAvailableOptions(Toolkit *ic)
verovio.vrvToolkit.getAvailableOptions = Module.cwrap( 'vrvToolkit_getAvailableOptions', 'string', ['number'] );

// char *getDescriptiveFeatures(Toolkit *ic, const char *options)
verovio.vrvToolkit.getDescriptiveFeatures = Module.cwrap( 'vrvToolkit_getDescriptiveFeatures', 'string', ['number', 'string'] );

// char *getElementAttr(Toolkit *ic, const char *xmlId)
verovio.vrvToolkit.getElementAttr = Module.cwrap( 'vrvToolkit_getElementAttr', 'string', ['number', 'string'] );

// char *getElementsAtTime(Toolkit *ic, int time)
verovio.vrvToolkit.getElementsAtTime = Module.cwrap( 'vrvToolkit_getElementsAtTime', 'string', ['number', 'number'] );

// char *vrvToolkit_getExpansionIdsForElement(Toolkit *tk, const char *xmlId);
verovio.vrvToolkit.getExpansionIdsForElement = Module.cwrap( 'vrvToolkit_getExpansionIdsForElement', 'string', ['number', 'string'] );

// char *getHumdrum(Toolkit *ic)
verovio.vrvToolkit.getHumdrum = Module.cwrap( 'vrvToolkit_getHumdrum', 'string' );

// char *convertMEIToHumdrum(Toolkit *ic, const char *meiData)
verovio.vrvToolkit.convertMEIToHumdrum = Module.cwrap( 'vrvToolkit_convertMEIToHumdrum', 'string', ['number', 'string'] );

// char *convertHumdrumToHumdrum(Toolkit *ic, const char *humdrumData)
verovio.vrvToolkit.convertHumdrumToHumdrum = Module.cwrap( 'vrvToolkit_convertHumdrumToHumdrum', 'string', ['number', 'string'] );

// char *getLog(Toolkit *ic)
verovio.vrvToolkit.getLog = Module.cwrap( 'vrvToolkit_getLog', 'string', ['number'] );

// char *getMEI(Toolkit *ic, const char *options)
verovio.vrvToolkit.getMEI = Module.cwrap( 'vrvToolkit_getMEI', 'string', ['number', 'string'] );

// char *vrvToolkit_getNotatedIdForElement(Toolkit *tk, const char *xmlId);
verovio.vrvToolkit.getNotatedIdForElement = Module.cwrap( 'vrvToolkit_getNotatedIdForElement', 'string', ['number', 'string'] );

// char *getOptions(Toolkit *ic, int defaultValues)
verovio.vrvToolkit.getOptions = Module.cwrap( 'vrvToolkit_getOptions', 'string', ['number', 'number'] );

// int getPageCount(Toolkit *ic)
verovio.vrvToolkit.getPageCount = Module.cwrap( 'vrvToolkit_getPageCount', 'number', ['number'] );

// int getPageWithElement(Toolkit *ic, const char *xmlId)
verovio.vrvToolkit.getPageWithElement = Module.cwrap( 'vrvToolkit_getPageWithElement', 'number', ['number', 'string'] );

// double getTimeForElement(Toolkit *ic, const char *xmlId)
verovio.vrvToolkit.getTimeForElement = Module.cwrap( 'vrvToolkit_getTimeForElement', 'number', ['number', 'string'] );

// char *getTimesForElement(Toolkit *ic, const char *xmlId)
verovio.vrvToolkit.getTimesForElement = Module.cwrap( 'vrvToolkit_getTimesForElement', 'string', ['number', 'string'] );

// char *getMIDIValuesForElement(Toolkit *ic, const char *xmlId)
verovio.vrvToolkit.getMIDIValuesForElement = Module.cwrap( 'vrvToolkit_getMIDIValuesForElement', 'string', ['number', 'string'] );

// char *getVersion(Toolkit *ic)
verovio.vrvToolkit.getVersion = Module.cwrap( 'vrvToolkit_getVersion', 'string', ['number'] );

// bool loadData(Toolkit *ic, const char *data)
verovio.vrvToolkit.loadData = Module.cwrap( 'vrvToolkit_loadData', 'number', ['number', 'string'] );

// bool loadZipDataBase64(Toolkit *ic, const char *data)
verovio.vrvToolkit.loadZipDataBase64 = Module.cwrap( 'vrvToolkit_loadZipDataBase64', 'number', ['number', 'string'] );

// bool loadZipDataBuffer(Toolkit *ic, const unsigned char *data, int length)
verovio.vrvToolkit.loadZipDataBuffer = Module.cwrap( 'vrvToolkit_loadZipDataBuffer', 'number', ['number', 'number', 'number'] );

// void redoLayout(Toolkit *ic)
verovio.vrvToolkit.redoLayout = Module.cwrap( 'vrvToolkit_redoLayout', null, ['number'] );

// void redoPagePitchPosLayout(Toolkit *ic)
verovio.vrvToolkit.redoPagePitchPosLayout = Module.cwrap( 'vrvToolkit_redoPagePitchPosLayout', null, ['number'] );

// char *renderData(Toolkit *ic, const char *data, const char *options)
verovio.vrvToolkit.renderData = Module.cwrap( 'vrvToolkit_renderData', 'string', ['number', 'string', 'string'] );

// char *renderToMidi(Toolkit *ic, const char *rendering_options)
verovio.vrvToolkit.renderToMIDI = Module.cwrap( 'vrvToolkit_renderToMIDI', 'string', ['number', 'string'] );

// char *renderToPAE(Toolkit *ic)
verovio.vrvToolkit.renderToPAE = Module.cwrap( 'vrvToolkit_renderToPAE', 'string' );

// char *renderToSvg(Toolkit *ic, int pageNo, const char *rendering_options)
verovio.vrvToolkit.renderToSVG = Module.cwrap( 'vrvToolkit_renderToSVG', 'string', ['number', 'number', 'string'] );

// char *renderToTimemap(Toolkit *ic)
verovio.vrvToolkit.renderToTimemap = Module.cwrap( 'vrvToolkit_renderToTimemap', 'string', ['number'] );

// void resetXmlIdSeed(Toolkit *ic, int seed) 
verovio.vrvToolkit.resetXmlIdSeed = Module.cwrap( 'vrvToolkit_resetXmlIdSeed', null, ['number', 'number'] );

// void setOptions(Toolkit *ic, const char *options) 
verovio.vrvToolkit.setOptions = Module.cwrap( 'vrvToolkit_setOptions', null, ['number', 'string'] );

// A pointer to the object - only one instance can be created for now
verovio.instances = [];

/***************************************************************************************************************************/

verovio.toolkit = function ()
{
    this.ptr = verovio.vrvToolkit.constructor();
    console.debug( "Creating toolkit instance" );
    verovio.instances.push( this.ptr );
}

verovio.toolkit.prototype.destroy = function ()
{
    verovio.instances.splice( verovio.instances.indexOf( this.ptr ), 1 );
    console.debug( "Deleting toolkit instance" );
    verovio.vrvToolkit.destructor( this.ptr );
};

verovio.toolkit.prototype.edit = function ( editorAction )
{
    return verovio.vrvToolkit.edit( this.ptr, JSON.stringify( editorAction ) );
};

verovio.toolkit.prototype.editInfo = function ()
{
    return JSON.parse( verovio.vrvToolkit.editInfo( this.ptr ) );
};

verovio.toolkit.prototype.getAvailableOptions = function ()
{
    return JSON.parse( verovio.vrvToolkit.getAvailableOptions( this.ptr ) );
};

verovio.toolkit.prototype.getDescriptiveFeatures = function ( options )
{
    return JSON.parse( verovio.vrvToolkit.getDescriptiveFeatures( this.ptr, JSON.stringify( options ) ) );
};

verovio.toolkit.prototype.getElementAttr = function ( xmlId )
{
    return JSON.parse( verovio.vrvToolkit.getElementAttr( this.ptr, xmlId ) );
};

verovio.toolkit.prototype.getElementsAtTime = function ( millisec )
{
    return JSON.parse( verovio.vrvToolkit.getElementsAtTime( this.ptr, millisec ) );
};

verovio.toolkit.prototype.getExpansionIdsForElement = function ( xmlId )
{
    return JSON.parse( verovio.vrvToolkit.getExpansionIdsForElement( this.ptr, xmlId ) );
};

verovio.toolkit.prototype.getHumdrum = function ()
{
    return verovio.vrvToolkit.getHumdrum( this.ptr );
};

verovio.toolkit.prototype.convertHumdrumToHumdrum = function ( data )
{
    return verovio.vrvToolkit.convertHumdrumToHumdrum( this.ptr, data );
};

verovio.toolkit.prototype.convertMEIToHumdrum = function ( data )
{
    return verovio.vrvToolkit.convertMEIToHumdrum( this.ptr, data );
};

verovio.toolkit.prototype.getLog = function ()
{
    return verovio.vrvToolkit.getLog( this.ptr );
};

verovio.toolkit.prototype.getMEI = function ( param1, scoreBased )
{
    if ( typeof param1 === 'undefined' )
    {
        return verovio.vrvToolkit.getMEI( this.ptr, JSON.stringify( {} ) );
    }
    else if ( param1 instanceof Object )
    {
        return verovio.vrvToolkit.getMEI( this.ptr, JSON.stringify( param1 ) );
    }
    else
    {
        options = { "pageNo": param1, "scoreBased": scoreBased };
        console.warn( "Deprecated getMEI() arguments, use JSON object instead. Adjusted input:", options );
        return verovio.vrvToolkit.getMEI( this.ptr, JSON.stringify( options ) );
    }
};

verovio.toolkit.prototype.getMIDIValuesForElement = function ( xmlId )
{
    return JSON.parse( verovio.vrvToolkit.getMIDIValuesForElement( this.ptr, xmlId ) );
};

verovio.toolkit.prototype.getNotatedIdForElement = function ( xmlId )
{
    return verovio.vrvToolkit.getNotatedIdForElement( this.ptr, xmlId );
};

verovio.toolkit.prototype.getOptions = function ( defaultValues )
{
    return JSON.parse( verovio.vrvToolkit.getOptions( this.ptr, defaultValues ) );
};

verovio.toolkit.prototype.getPageCount = function ()
{
    return verovio.vrvToolkit.getPageCount( this.ptr );
};

verovio.toolkit.prototype.getPageWithElement = function ( xmlId )
{
    return verovio.vrvToolkit.getPageWithElement( this.ptr, xmlId );
};

verovio.toolkit.prototype.getTimeForElement = function ( xmlId )
{
    return verovio.vrvToolkit.getTimeForElement( this.ptr, xmlId );
};

verovio.toolkit.prototype.getTimesForElement = function ( xmlId )
{
    return JSON.parse( verovio.vrvToolkit.getTimesForElement( this.ptr, xmlId ) );
};

verovio.toolkit.prototype.getVersion = function ()
{
    return verovio.vrvToolkit.getVersion( this.ptr );
};

verovio.toolkit.prototype.loadData = function ( data )
{
    return verovio.vrvToolkit.loadData( this.ptr, data );
};

verovio.toolkit.prototype.loadZipDataBase64 = function ( data )
{
    return verovio.vrvToolkit.loadZipDataBase64( this.ptr, data );
};

verovio.toolkit.prototype.loadZipDataBuffer = function ( data )
{
    if ( !(data instanceof ArrayBuffer ) )
    {
        console.error( "Parameter for loadZipDataBuffer has to be of type ArrayBuffer" );
        return false;
    }
    var dataArray = new Uint8Array( data ); 
    var dataSize = dataArray.length * dataArray.BYTES_PER_ELEMENT;
    var dataPtr = Module._malloc( dataSize );
    Module.HEAPU8.set( dataArray, dataPtr );
    var res = verovio.vrvToolkit.loadZipDataBuffer( this.ptr, dataPtr, dataSize );
    Module._free( dataPtr );
    return res;
};

verovio.toolkit.prototype.redoLayout = function ()
{
    verovio.vrvToolkit.redoLayout( this.ptr );
}

verovio.toolkit.prototype.redoPagePitchPosLayout = function ()
{
    verovio.vrvToolkit.redoPagePitchPosLayout( this.ptr );
}

verovio.toolkit.prototype.renderData = function ( data, options )
{
    return verovio.vrvToolkit.renderData( this.ptr, data, JSON.stringify( options ) );
};

verovio.toolkit.prototype.renderPage = function ( pageNo, options )
{
    console.warn( "Method renderPage is deprecated; use renderToSVG instead" );
    return verovio.vrvToolkit.renderToSVG( this.ptr, pageNo, JSON.stringify( options ) );
};

verovio.toolkit.prototype.renderToMIDI = function ( options )
{
    return verovio.vrvToolkit.renderToMIDI( this.ptr, JSON.stringify( options ) );
};

verovio.toolkit.prototype.renderToMidi = function ( options )
{
    console.warn( "Method renderToMidi is deprecated; use renderToMIDI instead" );
    return verovio.vrvToolkit.renderToMIDI( this.ptr, JSON.stringify( options ) );
};

verovio.toolkit.prototype.renderToPAE = function ()
{
    return verovio.vrvToolkit.renderToPAE( this.ptr );
};

verovio.toolkit.prototype.renderToSVG = function ( pageNo, options )
{
    return verovio.vrvToolkit.renderToSVG( this.ptr, pageNo, JSON.stringify( options ) );
};

verovio.toolkit.prototype.renderToTimemap = function ()
{
    return JSON.parse( verovio.vrvToolkit.renderToTimemap( this.ptr ) );
};

verovio.toolkit.prototype.resetXmlIdSeed = function ( seed )
{
    return verovio.vrvToolkit.resetXmlIdSeed( this.ptr, seed );
};

verovio.toolkit.prototype.setOptions = function ( options )
{
    verovio.vrvToolkit.setOptions( this.ptr, JSON.stringify( options ) );
};

/***************************************************************************************************************************/

// If the window object is defined (if we are not within a WebWorker)...
if ( ( typeof window !== "undefined" ) && ( window.addEventListener ) )
{
    // Add a listener that will delete the object (if necessary) when the page is closed
    window.addEventListener( "unload", function ()
    {
        for ( var i = 0; i < verovio.instances.length; i++ )
        {
            verovio.vrvToolkit.destructor( verovio.instances[i] );
        }
    } );
}
