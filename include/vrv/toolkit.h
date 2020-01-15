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
    bool SaveFile(const std::string &filename);

    /**
     * @name Getter and setter for options as JSON string
     */
    ///@{
    std::string GetOptions(bool defaultValues) const;
    std::string GetAvailableOptions() const;
    bool SetOptions(const std::string &json_options);
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
     * Get all the pages unless a page number (1-based) is specified
     */
    std::string GetMEI(int pageNo = 0, bool scoreBased = true);

    /**
     * Return element attributes as a JSON string
     */
    std::string GetElementAttr(const std::string &xmlId);

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
     * The SetFormat with FileFormat does not perform any validation
     */
    ///@{
    bool SetFormat(std::string const &informat);
    void SetFormat(FileFormat format) { m_format = format; }
    int GetFormat() { return m_format; }
    ///@}

    /**
     * @name Get the output file format (defined as FileFormat)
     * The SetOutputFormat with FileFormat does not perform any validation
     */
    ///@{
    bool SetOutputFormat(std::string const &outformat);
    void SetOutputFormat(FileFormat format) { m_outformat = format; }
    int GetOutputFormat() { return m_outformat; }
    ///@}

    /**
     * @name Identify the input file type for auto loading of input data
     */
    ///@{
    FileFormat IdentifyInputFormat(const std::string &data);
    ///@}

    /**
     * @name Set and get the xPath query for selecting <app> (if any)
     */
    ///@{
    void SetScoreBasedMei(bool scoreBasedMei) { m_scoreBasedMei = scoreBasedMei; }
    bool GetScoreBasedMei() { return m_scoreBasedMei; }
    ///@}

    /**
     * @name Get the pages for a loaded file
     * The SetFormat with FileFormat does not perform any validation
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

public:
    //
private:
    Doc m_doc;
    View m_view;
    int m_scale;
    FileFormat m_format;
    FileFormat m_outformat;
    bool m_scoreBasedMei;

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
