/////////////////////////////////////////////////////////////////////////////
// Name:        toolkit.h
// Author:      Laurent Pugin
// Created:     17/10/2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TOOLKIT_H__
#define __VRV_TOOLKIT_H__

#include <string>

//----------------------------------------------------------------------------

#include "doc.h"
#include "docselection.h"
#include "toolkitdef.h"
#include "view.h"

//----------------------------------------------------------------------------

namespace vrv {

class EditorToolkit;
class RuntimeClock;

/**
 * @defgroup nodoc Public methods that are not listed in the documentation
 */

//----------------------------------------------------------------------------
// Toolkit
//----------------------------------------------------------------------------

class Toolkit {
public:
    /**
     *************************************************************************
     * @name Constructors and destructors
     *************************************************************************
     */
    ///@{
    /**
     * Constructor.
     *
     * @param initFont If set to false, resource path is not initialized and SetResourcePath will have to be called
     * explicitely
     */
    Toolkit(bool initFont = true);
    virtual ~Toolkit();

    ///@}

    /**
     *************************************************************************
     * @name Utility methods
     *************************************************************************
     */
    ///@{

    /**
     * Return the ID of the Toolkit instance.
     *
     * @remark nojs
     *
     * @return The ID as as string
     */
    std::string GetID() { return m_doc.GetID(); }

    /**
     * Get the resource path for the Toolkit instance.
     *
     * @remark nojs
     *
     * @return A string with the resource path
     */
    std::string GetResourcePath() const;

    /**
     * Set the resource path for the Toolkit instance and any extra fonts
     *
     * This method needs to be called if the constructor had initFont=false or if the resource path
     * needs to be changed.
     *
     * @remark nojs
     *
     * @param path The path to the resource directory
     * @return True if the resources was successfully loaded
     */
    bool SetResourcePath(const std::string &path);

    /**
     * Get the log content for the latest operation.
     *
     * @return The log content as a string
     */
    std::string GetLog();

    /**
     * Return the version number.
     *
     * @return the version number as a string
     */
    std::string GetVersion() const;

    /**
     * Reset the seed used to generate MEI \@xml:id attribute values
     *
     * Passing 0 will seed the \@xml:id generator with a random (time-based) seed value.
     * This method will have no effect if the --xml-id-checksum option is set.
     *
     * @param seed The seed value for generating the \@xml:id values (0 for a time-based random seed)
     */
    void ResetXmlIdSeed(int seed);

    ///@}

    /**
     **************************************************************************
     * @name Loading methods
     **************************************************************************
     */
    ///@{

    /**
     * Load a file from the file system.
     *
     * Previously convert UTF16 files to UTF8 or extract files from MusicXML compressed files.
     *
     * @remark nojs
     *
     * @param filename The filename to be loaded
     * @return True if the file was successfully loaded
     */
    bool LoadFile(const std::string &filename);

    /**
     * Load a string data with the type previously specified in the options.
     *
     * By default, the methods try to auto-detect the type.
     *
     * @param data A string with the data (e.g., MEI data) to be loaded
     * @return True if the data was successfully loaded
     */
    bool LoadData(const std::string &data);

    /**
     * Load a MusicXML compressed file passed as base64 encoded string.
     *
     * @param data A ZIP file as a base64 encoded string
     * @return True if the data was successfully loaded
     */
    bool LoadZipDataBase64(const std::string &data);

    /**
     * Load a MusicXML compressed file passed as a buffer of bytes.
     *
     * @param data A ZIP file as a buffer of bytes
     * @param length The size of the data buffer
     * @return True if the data was successfully loaded
     */
    bool LoadZipDataBuffer(const unsigned char *data, int length);

    /**
     * Validate the Plaine & Easie code from a file.
     *
     * The method calls Toolkit::ValidatePAE.
     *
     * @remark nojs
     *
     * @param filename The filename to be validated
     * @return A stringified JSON object with the validation warnings or errors
     */
    std::string ValidatePAEFile(const std::string &filename);

    /**
     * Validate the Plaine & Easie code passed in the string data.
     *
     * A single JSON object is returned when there is a global input error.
     * When reading the input succeeds, validation is grouped by input keys.
     * The methods always returns errors in PAE pedantic mode.
     * No data remains loaded after the validation.
     *
     * @param data A string with the data in JSON or with PAE `@` keys
     * @return A stringified JSON object with the validation warnings or errors
     */
    std::string ValidatePAE(const std::string &data);

    /**
     * Return the number of pages in the loaded document.
     *
     * The number of pages depends one the page size and if encoded layout was taken into account or not.
     *
     * @return The number of pages
     */
    int GetPageCount();

    ///@}

    /**
     **************************************************************************
     * @name Getter and setter for options
     **************************************************************************
     */
    ///@{

    /**
     * Return a dictionary of all the options with their current value.
     *
     * @return A stringified JSON object
     */
    std::string GetOptions() const;

    /**
     * Return a dictionary of all the options with their default value.
     *
     * @return A stringified JSON object
     */
    std::string GetDefaultOptions() const;

    /**
     * Return all available options grouped by category.
     *
     * For each option, returns the type, the default value, and the minimum and maximum value (when available).
     *
     * @return A stringified JSON object
     */
    std::string GetAvailableOptions() const;

    /**
     * Set option values.
     *
     * The name of each option to be set is to be given as JSON key.
     *
     * @param jsonOptions A stringified JSON objects with the output options
     * @return True if the options were successfully set
     */
    bool SetOptions(const std::string &jsonOptions);

    /**
     * Reset all options to default values
     */
    void ResetOptions();

    /**
     * Print formatted option usage for specific category (with max/min/default values) to output stream.
     */
    void PrintOptionUsage(const std::string &category, std::ostream &output) const;

    /**
     * Get all usage for all option categories as string.
     */
    std::string GetOptionUsageString() const;

    /**
     * Set the scale option.
     *
     * @remark nojs
     *
     * @param scale the scale value as integer
     * @return True if the option was successfully set
     */
    bool SetScale(int scale);

    /**
     * Get the scale option.
     *
     * @remark nojs
     *
     * @return the scale option as integer
     */
    int GetScale() { return m_options->m_scale.GetValue(); }

    /**
     * Set the input from option.
     *
     * @remark nojs
     *
     * @param inputFrom the input from value as string
     * @return True if the option was successfully set
     */
    bool SetInputFrom(std::string const &inputFrom);

    /**
     * Set the output to option.
     *
     * @remark nojs
     *
     * @param outputTo the value to output as string
     * @return True if the option was successfully set
     */
    bool SetOutputTo(std::string const &outputTo);

    /**
     * Set the value for a selection.
     *
     * The selection will be applied only when some data is loaded or the layout is redone.
     * The selection can be reset (cancelled) by passing an empty string or an empty JSON object.
     * A selection across multiple mdivs is not possible.
     *
     * @param selection The selection as a stringified JSON object
     * @return True if the selection was successfully parsed or reset
     */
    bool Select(const std::string &selection);

    ///@}

    /**
     **************************************************************************
     * @name Experimental editor methods
     **************************************************************************
     */
    ///@{

    /**
     * Edit the MEI data - experimental code not to rely on.
     *
     * @param editorAction The editor actions as a stringified JSON object
     * @return True if the edit action was successfully applied
     **/
    bool Edit(const std::string &editorAction);

    /**
     * Return the editor status - experimental code not to rely on.
     *
     * @return The editor status as a string
     **/
    std::string EditInfo();

    ///@}

    /**
     **************************************************************************
     * @name Rendering methods
     **************************************************************************
     */
    ///@{

    /**
     * Render the first page of the data to SVG.
     *
     * This method is a wrapper for setting options, loading data and rendering the first page.
     * It will return an empty string if the options cannot be set or the data cannot be loaded.
     *
     * @param data A string with the data (e.g., MEI data) to be loaded
     * @param jsonOptions A stringified JSON objects with the output options
     * @return The SVG first page as a string
     */
    std::string RenderData(const std::string &data, const std::string &jsonOptions);

    /**
     * Render a page to SVG.
     *
     * @param pageNo The page to render (1-based)
     * @param xmlDeclaration True for including the xml declaration in the SVG output
     * @return The SVG page as a string
     */
    std::string RenderToSVG(int pageNo = 1, bool xmlDeclaration = false);

    /**
     * Render a page to SVG and save it to the file.
     *
     * @remark nojs
     *
     * @param filename The output filename
     * @param pageNo The page to render (1-based)
     * @return True if the file was successfully written
     */
    bool RenderToSVGFile(const std::string &filename, int pageNo = 1);

    /**
     * Render the document to MIDI.
     *
     * @return A MIDI file as a base64 encoded string
     */
    std::string RenderToMIDI();

    /**
     * Render a document to MIDI and save it to the file.
     *
     * @remark nojs
     *
     * @param filename The output filename
     * @return True if the file was successfully written
     */
    bool RenderToMIDIFile(const std::string &filename);

    /**
     * Render a document to Plaine & Easie code.
     *
     * Only the top staff / layer is exported.
     *
     * @return The PAE as a string
     */
    std::string RenderToPAE();

    /**
     * Render a document to Plaine & Easie code and save it to the file.
     *
     * Only the top staff / layer is exported.
     *
     * @remark nojs
     *
     * @param filename The output filename
     * @return True if the file was successfully written
     */
    bool RenderToPAEFile(const std::string &filename);

    /**
     * Render a document to a timemap.
     *
     * @param jsonOptions A stringified JSON objects with the timemap options
     * @return The timemap as a string
     */
    std::string RenderToTimemap(const std::string &jsonOptions = "");

    /**
     * Render a document's expansionMap, if existing
     *
     * @return The expansionMap as a string
     */
    std::string RenderToExpansionMap();

    /**
     * Render a document to timemap and save it to the file.
     *
     * @remark nojs
     *
     * @param filename The output filename
     * @param jsonOptions A stringified JSON objects with the timemap options
     * @return True if the file was successfully written
     */
    bool RenderToTimemapFile(const std::string &filename, const std::string &jsonOptions = "");

    /**
     * Render a document's expansionMap and save it to a file.
     *
     * @remark nojs
     *
     * @param filename The output filename
     */
    bool RenderToExpansionMapFile(const std::string &filename);

    //@}

    /**
     **************************************************************************
     * @name Humdrum related methods
     **************************************************************************
     */
    ///@{

    /**
     * Get the humdrum buffer.
     *
     * @return The humdrum buffer as a string
     */
    std::string GetHumdrum();

    /**
     * Convert MEI data into Humdrum data.
     *
     * @return The Humdrum data as a string
     */
    std::string ConvertMEIToHumdrum(const std::string &meiData);

    /**
     * Filter Humdrum data.
     *
     * @return The Humdrum data as a string
     */
    std::string ConvertHumdrumToHumdrum(const std::string &humdrumData);

    /**
     * Convert Humdrum data to MIDI.
     *
     * @return The MIDI file as a base64-encoded string
     */
    std::string ConvertHumdrumToMIDI(const std::string &humdrumData);

    /**
     * Write the humdrum buffer to the file.
     *
     * @remark nojs
     *
     * @param filename The output filename
     * @return True if the file was successfully written
     */
    bool GetHumdrumFile(const std::string &filename);

    ///@}

    /**
     **************************************************************************
     * @name MEI related methods
     **************************************************************************
     */
    ///@{

    /**
     * Get the MEI as a string.
     *
     * @param jsonOptions A stringified JSON object with the output options;
     * pageNo: integer; (1-based), all pages if none (or 0) specified;
     * scoreBased: true or false; true by default;
     * basic: true or false; false by default;
     * removeIds: true or false; false by default - remove all \@xml:id not used in the data;
     */
    std::string GetMEI(const std::string &jsonOptions = "");

    /**
     * Get the MEI and save it to the file.
     *
     * @remark nojs
     *
     * @param filename The output filename
     * @param jsonOptions A stringified JSON object with the output options
     * @return True if the file was successfully written
     */
    bool SaveFile(const std::string &filename, const std::string &jsonOptions = "");

    ///@}

    /**
     **************************************************************************
     * @name Data access methods
     **************************************************************************
     */
    ///@{

    /**
     * Return descriptive features as a JSON string.
     *
     * The features are tailored for implementing incipit search.
     *
     * @param jsonOptions A stringified JSON object with the feature extraction options
     * @return A stringified JSON object with the requested features
     */
    std::string GetDescriptiveFeatures(const std::string &jsonOptions);

    /**
     * Return array of IDs of elements being currently played.
     *
     * @param millisec The time in milliseconds
     * @return A stringified JSON object with the page and notes being played
     */
    std::string GetElementsAtTime(int millisec);

    /**
     * Return the page on which the element is the ID (\@xml:id) is rendered
     *
     * This takes into account the current layout options.
     *
     * @param xmlId the ID (\@xml:id) of the element being looked for
     * @return the page number (1-based) where the element is (0 if not found)
     */
    int GetPageWithElement(const std::string &xmlId);

    /**
     * Return element attributes as a JSON string.
     *
     * The attributes returned include the ones not supported by Verovio.
     *
     * @param xmlId the ID (\@xml:id) of the element being looked for
     * @return A stringified JSON object with all attributes
     */
    std::string GetElementAttr(const std::string &xmlId);

    /**
     * Return the ID string of the notated (the original) element.
     *
     * @param xmlId the ID (\@xml:id) of the element being looked for
     * @return An ID string
     */
    std::string GetNotatedIdForElement(const std::string &xmlId);

    /**
     * Return a vector of ID strings of all elements (the notated and the expanded) for a given element.
     *
     * @param xmlId the ID (\@xml:id) of the element being looked for
     * @return A stringified JSON object with all IDs
     */
    std::string GetExpansionIdsForElement(const std::string &xmlId);

    /**
     * Return the time at which the element is the ID (\@xml:id) is played
     *
     * RenderToMIDI() must be called prior to using this method.
     *
     * @param xmlId the ID (\@xml:id) of the element being looked for
     * @return The time in milliseconds
     */
    int GetTimeForElement(const std::string &xmlId);

    /**
     * Return MIDI values of the element with the ID (\@xml:id)
     *
     * RenderToMIDI() must be called prior to using this method.
     *
     * @param xmlId the ID (\@xml:id) of the element being looked for
     * @return A stringified JSON object with the MIDI values
     */
    std::string GetMIDIValuesForElement(const std::string &xmlId);

    /**
     * Return a JSON object string with the following key values for a given note.
     *
     * Return scoreTimeOnset, scoreTimeOffset, scoreTimeTiedDuration,
     * realTimeOnsetMilliseconds, realTimeOffsetMilliseconds, realTimeTiedDurationMilliseconds.
     *
     * @param xmlId the ID (\@xml:id) of the element being looked for
     * @return A stringified JSON object with the values
     */
    std::string GetTimesForElement(const std::string &xmlId);

    ///@}

    /**
     * @name Layout related methods
     */
    ///@{

    /**
     * Redo the layout of the loaded data.
     *
     * This can be called once the rendering option were changed, for example with a new page (sceen) height or a new
     * zoom level.
     *
     * @param jsonOptions A stringified JSON object with the action options
     * resetCache: true or false; true by default;
     */
    void RedoLayout(const std::string &jsonOptions = "");

    /**
     * Redo the layout of the pitch postitions of the current drawing page.
     *
     * Only the note vertical positions are recalculated with this method.
     * RedoLayout() needs to be called for a full recalculation.
     */
    void RedoPagePitchPosLayout();

    ///@}

    //------------------------------------------------//
    // Public methods not listed in the documentation //
    //------------------------------------------------//

    /**
     * @name Public methods that are not listed in the documentation
     *
     * @internal They are marked with \@ingroup nodoc
     */
    ///@{

    /**
     * Render the page to the deviceContext.
     *
     * Page number is 1-based.
     *
     * @ingroup nodoc
     */
    bool RenderToDeviceContext(int pageNo, DeviceContext *deviceContext);

    /**
     * Return the Options object of the Toolkit instance.
     *
     * @ingroup nodoc
     */
    Options *GetOptionsObj() { return m_options; }

    /**
     * Copy the data to the cstring internal buffer.
     *
     * @ingroup nodoc
     */
    void SetCString(const std::string &data);

    /**
     * Return the content of the cstring internal buffer.
     *
     * Return "[unspecified]" if the buffer has not been allocated.
     *
     * @ingroup nodoc
     */
    const char *GetCString();

    /**
     * Write the Humdrum buffer to the outputstream.
     *
     * @ingroup nodoc
     */
    void GetHumdrum(std::ostream &output);

    /**
     * Copy the data to the humdrum internal buffer.
     *
     * @ingroup nodoc
     */
    void SetHumdrumBuffer(const char *contents);

    /**
     * Return the content of the humdrum internal buffer.
     *
     * Return "[empty]" if the buffer has not been allocated.
     *
     * @ingroup nodoc
     */
    const char *GetHumdrumBuffer();

    /**
     * Clear the Humdrum buffer of any contents.
     *
     * @ingroup nodoc
     */
    void ClearHumdrumBuffer();

    /**
     * Get the input format.
     *
     * @return Input format enumeration
     *
     * @ingroup nodoc
     */
    int GetInputFrom() { return m_inputFrom; }

    /**
     * @ingroup nodoc
     */
    int GetOutputTo() { return m_outputTo; }

    /**
     * Setting the global locale.
     */
    ///@{
    void SetLocale();
    void ResetLocale();
    ///@}

    /**
     * Measuring runtime.
     *
     * @ingroup nodoc
     */
    ///@{
    void InitClock();
    void ResetClock();
    double GetRuntimeInSeconds() const;
    void LogRuntime() const;
    ///@}

    ///@}

protected:
    /**
     * Identify the input file type for auto loading of input data
     */
    FileFormat IdentifyInputFrom(const std::string &data);

    /**
     * Print formatted option usage for specific option to output stream.
     */
    void PrintOptionUsageOutput(const vrv::Option *option, std::ostream &output) const;

    /**
     * Resets the vrv::logBuffer.
     */
    void ResetLogBuffer();

    /**
     * Start capturing std::cerr from an external codebase for redirection to vrv::logBuffer.
     * Only one capture should be active at a given time.  Finish by calling LogRedirectStop.
     */
    void LogRedirectStart();

    /**
     * End capturing std::cerr from an external codebase for redirection to vrv::logBuffer.
     */
    void LogRedirectStop();

    /**
     * Load a string data with or without resetting the log buffer
     */
    bool LoadData(const std::string &data, bool resetLogBuffer);

private:
    bool SetFont(const std::string &fontName);
    bool IsUTF16(const std::string &filename);
    bool LoadUTF16File(const std::string &filename);
    bool IsZip(const std::string &filename);
    bool LoadZipFile(const std::string &filename);
    bool LoadZipData(const std::vector<unsigned char> &bytes);
    void GetClassIds(const std::vector<std::string> &classStrings, std::vector<ClassId> &classIds);

    /**
     * Return a dictionary of all the options
     *
     * @param defaultValues True for getting the default values and false for the current values
     * @return A stringified JSON object
     */
    std::string GetOptions(bool defaultValues) const;

    /**
     * Set the doc to the view and set the editor after having loaded data
     */
    void SetViewAndEditor();

public:
    //
private:
    Doc m_doc;
    DocSelection m_docSelection;
    View m_view;
    FileFormat m_inputFrom;
    FileFormat m_outputTo;

    Options *m_options;

    std::optional<std::locale> m_previousLocale;

    /**
     * The C buffer string.
     */
    char *m_cString;

    /**
     * Temporary capture buffer for redirecting std::cerr to vrv::LogWarning.
     * Used to coordinate between LogRedirectStart()/LogRedirectStop().
     */
    std::stringstream m_cerrCaptured;

    /**
     * Temporary storage of the std::cerr read buffer during LogCapture. NULL when not in use.
     * Used to coordinate between LogRedirectStart()/LogRedirectStop().
     */
    std::streambuf *m_cerrOriginalBuf;

    EditorToolkit *m_editorToolkit;

#ifndef NO_RUNTIME
    /** Measuring runtime */
    RuntimeClock *m_runtimeClock;
#endif

    //----------------//
    // Static members //
    //----------------//

    static char *m_humdrumBuffer;
};

} // namespace vrv
#endif
