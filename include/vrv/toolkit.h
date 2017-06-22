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

#if defined(USE_EMSCRIPTEN) || defined(PYTHON_BINDING)
#include "jsonxx.h"
#endif

#include "checked.h"
#include "unchecked.h"

namespace vrv {

enum FileFormat { UNKNOWN = 0, AUTO, MEI, HUMDRUM, PAE, DARMS, MUSICXML, MUSICXMLHUM, ESAC, MIDI, TIMEMAP };

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
     * Parse the options passed as JSON string.
     * Only available for Emscripten-based compiles
     **/
    bool ParseOptions(const std::string &json_options);

    /**
     * Parse the editor actions passed as JSON string.
     * Only available for Emscripten-based compiles
     **/
    bool Edit(const std::string &json_editorAction);

    /**
     * Concatenates the vrv::logBuffer into a string an returns it.
     * This is used only for Emscripten-based compilation.
     * The vrv::logBuffer is filled by the vrv::LogXXX functions.
     */
    std::string GetLogString();

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
     * Render the page in SVG and returns it as a string.
     * Page number is 1-based
     */
    std::string RenderToSvg(int pageNo = 1, bool xml_declaration = false);

    /**
     * Render the page in SVG and save it to the file.
     * Page number is 1-based.
     */
    bool RenderToSvgFile(const std::string &filename, int pageNo = 1);

    /**
     * Creates a midi file, opens it, and writes to it.
     * currently generates a dummy midi file.
     */
    bool RenderToMidiFile(const std::string &filename);

    /**
     * Creates a midi file, opens it, and returns it (base64 encoded).
     */
    std::string RenderToMidi();

    /**
     * Creates a timemap file, and return it as a JSON string.
     */
    bool RenderToTimemap(std::string &output);
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
    std::string GetMEI(int pageNo = 0, bool scoreBased = false);

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
     * @name Set and get a std::string into a char * buffer.
     * This is used for returning a string buffer to emscripten.
     * The buffer is freed when reset or in MusController destructor.
     */
    ///@{
    void SetCString(const std::string &data);
    const char *GetCString();
    ///@}

    /**
     * @name Set and get the border
     */
    ///@{
    bool SetBorder(int border);
    int GetBorder() { return m_border; }
    ///@}

    /**
     * @name Set and get the scale
     */
    ///@{
    bool SetScale(int scale);
    int GetScale() { return m_scale; }
    ///@}

    /**
     * @name Set and get the page height (in pixels)
     */
    ///@{
    bool SetPageHeight(int h);
    int GetPageHeight() { return m_pageHeight; }
    ///@}

    /**
     * @name Set and get the page width (in pixels)
     */
    ///@{
    bool SetPageWidth(int w);
    int GetPageWidth() { return m_pageWidth; }
    ///@}

    /**
     * @name Set and get the spacing staff and system
     */
    ///@{
    bool SetSpacingStaff(int spacingStaff);
    bool SetSpacingSystem(int spacingSystem);
    int GetSpacingStaff() { return m_spacingStaff; }
    int GetSpacingSystem() { return m_spacingSystem; }
    ///@}

    /**
     * @name Space notes equally and close together (normally for mensural notation)
     */
    ///@{
    bool SetSpacingLinear(float spacingLinear);
    float GetSpacingLinear() { return m_spacingLinear; }
    ///@}

    /**
     * @name Space notes equally and close together (normally for mensural notation)
     */
    ///@{
    bool SetSpacingNonLinear(float spacingNonLinear);
    float GetSpacingNonLinear() { return m_spacingNonLinear; }
    ///@}

    /**
     * @name Ignore all encoded layout information (if any)
     * and output one single page with one single system
     */
    ///@{
    void SetNoLayout(bool l) { m_noLayout = l; }
    int GetNoLayout() { return m_noLayout; }
    ///@}

    /**
     * @name Include type attributes when importing from Humdrum
     */
    ///@{
    void SetHumType(int l) { m_humType = l; }
    int GetHumType() { return m_humType; }
    ///@}

    /**
     * @name Ignore all encoded layout information (if any)
     * and fully recalculate the layout
     */
    ///@{
    void SetIgnoreLayout(bool l) { m_ignoreLayout = l; }
    int GetIgnoreLayout() { return m_ignoreLayout; }
    ///@}

    /**
     * @name Crop the page height to the height of the content
     */
    ///@{
    void SetAdjustPageHeight(bool a) { m_adjustPageHeight = a; }
    int GetAdjustPageHeight() { return m_adjustPageHeight; }
    ///@}

    /**
     * @name Space notes equally and close together (normally for mensural notation)
     */
    ///@{
    void SetEvenNoteSpacing(bool even) { m_evenNoteSpacing = even; }
    int GetEvenNoteSpacing() { return m_evenNoteSpacing; }
    ///@}

    /**
     * @name Do not justify the system (for debugging purposes)
     */
    ///@{
    void SetNoJustification(bool j) { m_noJustification = j; }
    int GetNoJustification() { return m_noJustification; }
    ///@}

    /**
     * @name Do not justify the system (for debugging purposes)
     */
    ///@{
    void SetShowBoundingBoxes(bool b) { m_showBoundingBoxes = b; }
    int GetShowBoundingBoxes() { return m_showBoundingBoxes; }
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
    void SetAppXPathQueries(std::vector<std::string> const &xPathQueries);
    std::vector<std::string> GetAppXPathQuery() { return m_appXPathQueries; }
    ///@}

    /**
     * @name Set and get the xPath queries for selecting <choice> (if any)
     */
    ///@{
    void SetChoiceXPathQueries(std::vector<std::string> const &xPathQueries);
    std::vector<std::string> GetChoiceXPathQueries() { return m_choiceXPathQueries; }
    ///@}

    /**
     * @name Set and get the xPath query for selecting a <mdiv>
     */
    ///@{
    void SetMdivXPathQuery(std::string const &xPathQuery) { m_mdivXPathQuery = xPathQuery; };
    std::string GetMdivXPathQuery() { return m_mdivXPathQuery; };
    ///@}

    /**
     * @name Set and get the xPath query for selecting <app> (if any)
     */
    ///@{
    void SetScoreBasedMei(bool scoreBasedMei) { m_scoreBasedMei = scoreBasedMei; }
    bool GetScoreBasedMei() { return m_scoreBasedMei; }
    ///@}

    /**
     * @name Set a specific font
     */
    ///@{
    bool SetFont(std::string const &font);
    ///@}

    /**
     * @name Get the pages for a loaded file
     * The SetFormat with FileFormat does not perform any validation
     */
    ///@{
    int GetPageCount();
    ///@}

    /**
     * Experimental editor methods
     */
    ///@{
    bool Drag(std::string elementId, int x, int y);
    bool Insert(std::string elementType, std::string startId, std::string endId);
    bool Set(std::string elementId, std::string attrType, std::string attrValue);
    ///@}

private:
    bool IsUTF16(const std::string &filename);
    bool LoadUTF16File(const std::string &filename);

protected:
#ifdef USE_EMSCRIPTEN
    /**
     * Experimental editor methods
     */
    ///@{
    bool ParseDragAction(jsonxx::Object param, std::string *elementId, int *x, int *y);
    bool ParseInsertAction(jsonxx::Object param, std::string *elementType, std::string *startid, std::string *endid);
    bool ParseSetAction(jsonxx::Object param, std::string *elementId, std::string *attrType, std::string *attrValue);
///@}
#endif

public:
    //
private:
    Doc m_doc;
    View m_view;
    int m_scale;
    FileFormat m_format;
    FileFormat m_outformat;

    int m_pageHeight;
    int m_pageWidth;
    int m_border; // to be replace by pageRightMar, pageLeftMar, pageTopMar
    /** given in units **/
    int m_spacingStaff;
    int m_spacingSystem;

    bool m_noLayout;
    bool m_ignoreLayout;
    int m_humType = 0;
    bool m_adjustPageHeight;
    std::vector<std::string> m_appXPathQueries;
    std::vector<std::string> m_choiceXPathQueries;
    std::string m_mdivXPathQuery;
    bool m_scoreBasedMei;
    bool m_evenNoteSpacing;
    float m_spacingLinear;
    float m_spacingNonLinear;
    // for debugging
    bool m_noJustification;
    bool m_showBoundingBoxes;

    static char *m_humdrumBuffer;
    char *m_cString;
};

} // namespace vrv
#endif
