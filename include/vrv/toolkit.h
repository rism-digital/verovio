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
#include "view.h"

//----------------------------------------------------------------------------

namespace vrv {

class EditorToolkit;

enum FileFormat {
    UNKNOWN = 0,
    AUTO,
    MEI,
    HUMDRUM,
    PAE,
    ABC,
    DARMS,
    MUSICXML,
    MUSICXMLHUM,
    MEIHUM,
    MUSEDATAHUM,
    ESAC,
    MIDI,
    TIMEMAP
};

void SetDefaultResourcePath(const std::string &path);

//----------------------------------------------------------------------------
// Toolkit
//----------------------------------------------------------------------------

class Toolkit {
public:
    /**
     * @name Constructors and destructors
     */
    ///@{
    /** If initFont is set to false, Resources::InitFonts will have to be called explicitely */
    Toolkit(bool initFont = true);
    virtual ~Toolkit();
    ///@}

    /** We just use the doc uuid as uuid */
    std::string GetUuid() { return m_doc.GetUuid(); }

    /** We just use the doc options */
    Options *GetOptions() { return m_options; }

    /**
     * Set the resource path. To be called if the constructor had initFont=false.
     * This needs refactoring.
     */
    bool SetResourcePath(const std::string &path);

    /**
     * Load a file with the specified type.
     */
    bool LoadFile(const std::string &filename);

    /**
     * Load a string data with the specified type.
     */
    bool LoadData(const std::string &data);

    /**
     * Save an MEI file.
     */
    bool SaveFile(const std::string &filename, const std::string &jsonOptions);

    /**
     * @name Getter and setter for options as JSON string
     */
    ///@{
    std::string GetOptions(bool defaultValues) const;
    std::string GetAvailableOptions() const;
    bool SetOptions(const std::string &jsonOptions);
    ///@}

    /**
     * @name Getter and setter for single option
     */
    ///@{
    std::string GetOption(const std::string &option, bool defaultValue = false) const;
    bool SetOption(const std::string &option, const std::string &value);
    ///@}

    /**
     * Parse the editor actions passed as JSON string.
     * Only available for Emscripten-based compiles
     **/
    bool Edit(const std::string &json_editorAction);

    std::string EditInfo();

    /**
     * Concatenates the vrv::logBuffer into a string an returns it.
     * This is used only for Emscripten-based compilation.
     * The vrv::logBuffer is filled by the vrv::LogXXX functions.
     */
    std::string GetLog();

    /**
     * Returns the version number as a string.
     * This is used only for Emscripten-based compilation.
     */
    std::string GetVersion();

    /**
     * Resets the vrv::logBuffer.
     * This is used only for Emscripten-based compilation.
     */
    void ResetLogBuffer();

    /**
     * Render the page to the deviceContext.
     * Page number is 1-based.
     */
    bool RenderToDeviceContext(int pageNo, DeviceContext *deviceContext);

    /**
     * Render the page in SVG and returns it as a string.
     * Page number is 1-based
     */
    std::string RenderToSVG(int pageNo = 1, bool xml_declaration = false);

    /**
     * Render the page in SVG and save it to the file.
     * Page number is 1-based.
     */
    bool RenderToSVGFile(const std::string &filename, int pageNo = 1);

    /**
     * Creates a midi file, opens it, and writes to it.
     * currently generates a dummy midi file.
     */
    bool RenderToMIDIFile(const std::string &filename);

    /**
     * Creates a midi file, opens it, and returns it (base64 encoded).
     */
    std::string RenderToMIDI();

    /**
     * Export the content to a Plaine and Easie file.
     */
    bool RenderToPAEFile(const std::string &filename);

    /**
     * Render the content to Plaine and Easie.
     * Only the top staff / layer is exported.
     */
    std::string RenderToPAE();

    /**
     * Creates a timemap file, and return it as a JSON string.
     */
    std::string RenderToTimemap();
    bool RenderToTimemapFile(const std::string &filename);

    const char *GetHumdrumBuffer();
    void SetHumdrumBuffer(const char *contents);

    bool GetHumdrumFile(const std::string &filename);
    void GetHumdrum(std::ostream &output);
    std::string GetHumdrum();

    /**
     * Returns array of IDs of elements being currently played.
     */
    std::string GetElementsAtTime(int millisec);

    /**
     * Get the MEI as a string.
     * Options (JSON) can be:
     * pageNo: integer; (1-based), all pages if none (or 0) specified
     * scoreBased: true|false; true by default
     * (noXmlIds: true|false; false by default - remove all @xml:id not used in the data - not implemented)
     */
    std::string GetMEI(const std::string &jsonOptions);

    /**
     * Return element attributes as a JSON string
     */
    std::string GetElementAttr(const std::string &xmlId);

    /**
     * Returns the ID string of the notated (the original) element
     */
    std::string GetNotatedIdForElement(const std::string &xmlId);

    /**
     * Returns a vector of ID strings of all elements (the notated and the expanded) for a given element
     */
    std::string GetExpansionIdsForElement(const std::string &xmlId);

    /**
     * Redo the layout of the loaded data.
     * This can be called once the rendering option were changed,
     * For example with a new page (sceen) height or a new zoom level.
     */
    void RedoLayout();

    /**
     * Redo the layout of the pitch postitions of the current drawing page.
     * Only the note vertical positions are recalculated with this method.
     * RedoLayout() needs to be called for a full recalculation.
     */
    void RedoPagePitchPosLayout();

    /**
     * Return the page on which the element is the ID (xml:id) is rendered.
     * This takes into account the current layout options.
     * Returns 0 if no element is found.
     */
    int GetPageWithElement(const std::string &xmlId);

    /**
     * Return the time at which the element is the ID (xml:id) is played.
     * RenderToMidi() must be called prior to using this method.
     * Returns 0 if no element is found.
     */
    int GetTimeForElement(const std::string &xmlId);

    /**
     * Return MIDI values of the element with the ID (xml:id).
     * RenderToMidi() must be called prior to using this method.
     */
    std::string GetMIDIValuesForElement(const std::string &xmlId);

    /**
     * @name Set and get the scale
     */
    ///@{
    bool SetScale(int scale);
    int GetScale() { return m_scale; }
    ///@}

    /**
     * @name Get the input file format (defined as FileFormat)
     * The SetInputFrom with FileFormat does not perform any validation
     */
    ///@{
    bool SetInputFrom(std::string const &inputFrom);
    void SetInputFrom(FileFormat format) { m_inputFrom = format; }
    int GetInputFrom() { return m_inputFrom; }
    ///@}

    /**
     * @name Get the output file format (defined as FileFormat)
     * The SetOutputTo with FileFormat does not perform any validation
     */
    ///@{
    bool SetOutputTo(std::string const &outputTo);
    int GetOutputTo() { return m_outputTo; }
    ///@}

    /**
     * @name Identify the input file type for auto loading of input data
     */
    ///@{
    FileFormat IdentifyInputFrom(const std::string &data);
    ///@}

    /**
     * @name Get the pages for a loaded file
     */
    ///@{
    int GetPageCount();
    ///@}

    /**
     * @name Set and get a std::string into a char * buffer.
     * This is used for returning a string buffer to emscripten.
     * The buffer is freed when reset or in MusController destructor.
     */
    ///@{
    void SetCString(const std::string &data);
    const char *GetCString();
    ///@}

private:
    bool IsUTF16(const std::string &filename);
    bool LoadUTF16File(const std::string &filename);
    void GetClassIds(const std::vector<std::string> &classStrings, std::vector<ClassId> &classIds);

public:
    static std::map<std::string, ClassId> s_MEItoClassIdMap;

private:
    Doc m_doc;
    View m_view;
    int m_scale;
    FileFormat m_inputFrom;
    FileFormat m_outputTo;

    static char *m_humdrumBuffer;

    Options *m_options;

    /**
     * The C buffer string.
     */
    char *m_cString;

    EditorToolkit *m_editorToolkit;
};

} // namespace vrv
#endif
