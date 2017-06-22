/////////////////////////////////////////////////////////////////////////////
// Name:        doc.h
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_DOC_H__
#define __VRV_DOC_H__

#include "devicecontextbase.h"
#include "scoredef.h"
#include "style.h"

class MidiFile;

namespace vrv {

class FontInfo;
class Glyph;
class Page;
class Score;

enum DocType { Raw = 0, Rendering, Transcription };

//----------------------------------------------------------------------------
// Doc
//----------------------------------------------------------------------------

/**
 * This class is a hold the data and corresponds to the model of a MVC design pattern.
 */
class Doc : public Object {

public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method resets all attribute classes
     */
    ///@{
    Doc();
    virtual ~Doc();
    virtual ClassId GetClassId() const { return DOC; }
    ///@}

    /**
     * Add a page to the document
     */
    virtual void AddChild(Object *object);

    /*
     * Clear the content of the document.
     */
    virtual void Reset();

    /**
     * Refreshes the views from Doc.
     */
    virtual void Refresh();

    /**
     * Generate a document scoreDef when none is provided.
     * This only looks at the content first system of the document.
     */
    bool GenerateDocumentScoreDef();

    /**
     * Getter and setter for the DocType.
     * The setter resets the document.
     */
    DocType GetType() const { return m_type; }
    void SetType(DocType type);

    /**
     * Check if the document has a page with the specified value
     */
    bool HasPage(int pageIdx) const;

    /**
     * Create a score buffer for loading or creating a scoreBased MEI document.
     * Creating a mdiv buffer clear existing data (but not the header).
     * The score buffer is owned by the Doc.
     * Once the document is created, Doc::ConvertToPagePagedDoc should be called to convert it before rendering.
     */
    Score *CreateScoreBuffer();

    /**
     * Get the total page count
     */
    int GetPageCount() const;

    bool GetMidiExportDone() const;

    /**
     * @name Get the height or width for a glyph taking into account the staff and grace sizes
     */
    ///@{
    int GetGlyphHeight(wchar_t code, int staffSize, bool graceSize) const;
    int GetGlyphWidth(wchar_t code, int staffSize, bool graceSize) const;
    int GetGlyphDescender(wchar_t code, int staffSize, bool graceSize) const;
    int GetDrawingUnit(int staffSize) const;
    int GetDrawingDoubleUnit(int staffSize) const;
    int GetDrawingStaffSize(int staffSize) const;
    int GetDrawingOctaveSize(int staffSize) const;
    int GetDrawingBrevisWidth(int staffSize) const;
    int GetDrawingBarLineWidth(int staffSize) const;
    int GetDrawingStaffLineWidth(int staffSize) const;
    int GetDrawingStemWidth(int staffSize) const;
    int GetDrawingDirHeight(int staffSize, bool withMargin) const;
    int GetDrawingDynamHeight(int staffSize, bool withMargin) const;
    int GetDrawingHairpinSize(int staffSize, bool withMargin) const;
    int GetDrawingBeamWidth(int staffSize, bool graceSize) const;
    int GetDrawingBeamWhiteWidth(int staffSize, bool graceSize) const;
    int GetDrawingLedgerLineLength(int staffSize, bool graceSize) const;
    int GetCueSize(int value) const;
    ///@}

    Point ConvertFontPoint(const Glyph *glyph, const Point &fontPoint, int staffSize, bool graceSize) const;

    /**
     * @name Get the height or width for a text glyph taking into account the grace size.
     * The staff size must already be taken into account in the FontInfo
     */
    ///@{
    int GetTextGlyphHeight(wchar_t code, FontInfo *font, bool graceSize) const;
    int GetTextGlyphWidth(wchar_t code, FontInfo *font, bool graceSize) const;
    int GetTextGlyphDescender(wchar_t code, FontInfo *font, bool graceSize) const;
    ///@}

    /**
     * @name Get the height or width for a glyph taking into account the staff and grace sizes
     * (no const because the size of the member font is changed)
     */
    ///@{
    FontInfo *GetDrawingSmuflFont(int staffSize, bool graceSize);
    FontInfo *GetDrawingLyricFont(int staffSize);
    ///@}

    /**
     * @name Setters for the page dimensions and margins
     */
    ///@{
    void SetPageHeight(int pageHeight);
    void SetPageWidth(int pageWidth);
    void SetPageLeftMar(short pageLeftMar);
    void SetPageRightMar(short pageRightMar);
    void SetPageTopMar(short pageTopMar);
    void SetSpacingStaff(short spacingStaff);
    void SetSpacingSystem(short spacingSystem);
    ///@}

    /**
     * @name Getters for tie and slur parameters
     */
    ///@{
    char GetTieThickness() const { return m_style->m_tieThickness; }
    char GetSlurMinHeight() const { return m_style->m_minSlurHeight; }
    char GetSlurMaxHeight() const { return m_style->m_maxSlurHeight; }
    char GetSlurThickness() const { return m_style->m_slurThickness; }
    ///@}

    /**
     * @name Getters for the page dimensions and margins
     */
    ///@{
    short GetSpacingStaff() const { return m_spacingStaff; }
    short GetSpacingSystem() const { return m_spacingSystem; }
    ///@}

    /**
     * @name Getters for the object margins (left and right).
     * The margins are given in x / PARAM_DENOMINATOR * UNIT
     * With PARAM_DENOMINATOR == 10, a margin of 25 is 2.5 UNIT.
     * These should eventually be set at parameters.
     */
    ///@{
    char GetLeftMargin(const ClassId classId) const;
    char GetRightMargin(const ClassId classId) const;
    char GetLeftPosition() const;
    char GetBottomMargin(const ClassId classId) const;
    char GetTopMargin(const ClassId classId) const;
    ///@}

    /*
     * @name Setter and getter for the justification (x-axis) flag.
     * Justification is enabled by default. It needs to be disabled
     * for drawing the entire document on one single system.
     */
    ///@{
    void SetJustificationX(bool drawingJustifyX) { m_drawingJustifyX = drawingJustifyX; }
    bool GetJustificationX() const { return m_drawingJustifyX; }
    ///@}

    /*
     * @name Setter and getter for the duration-based-spacing flag.
     * Spacing by duration is always used with CMN, and it's enabled by default.
     * It should be disabled (so we get "even" note spacing) for mensural notation.
     */
    ///@{
    void SetEvenSpacing(bool drawingEvenSpacing) { m_drawingEvenSpacing = drawingEvenSpacing; }
    bool GetEvenSpacing() const { return m_drawingEvenSpacing; }
    ///@}

    /*
     * @name Setter and getter for linear and non-linear spacing parameters
     */
    ///@{
    void SetSpacingLinear(double drawingSpacingLinear) { m_drawingSpacingLinear = drawingSpacingLinear; }
    double GetSpacingLinear() const { return m_drawingSpacingLinear; }
    void SetSpacingNonLinear(double drawingSpacingNonLinear) { m_drawingSpacingNonLinear = drawingSpacingNonLinear; }
    double GetSpacingNonLinear() const { return m_drawingSpacingNonLinear; }
    ///@}

    /**
     * Prepare the MIDI timemap for MIDI and timemap file export.
     * Run trough all the layers and fill the score-time and performance timing variables.
     */
    void CalculateMidiTimemap(void);

    /**
     * Check to see if the MIDI timemap has already been calculated.  This needs to return
     * true before ExportMIDI() or ExportTimemap() can export anything (These two functions
     * will automatically run CalculateMidiTimemap() if HasMidiTimemap() return false.
     */
    bool HasMidiTimemap(void);

    /**
     * Export the document to a MIDI file.
     * Run trough all the layers and fill the midi file content.
     */
    void ExportMIDI(MidiFile *midiFile);

    /**
     * Extract a timemap from the document to a JSON string.
     * Run trough all the layers and fill the timemap file content.
     */
    std::string ExportTimemap(void);

    /**
     * Set the initial scoreDef of each page.
     * This is necessary for integrating changes that occur within a page.
     * It uses the MusObject::SetPageScoreDef functor method for parsing the file.
     * This will be done only if m_currentScoreDefDone is false or force is true.
     */
    void CollectScoreDefs(bool force = false);

    /**
     * Prepare the document for drawing.
     * This sets drawing pointers and value and needs to be done after loading and any editing.
     * For example, it sets the approriate values for the lyrics connectors
     */
    void PrepareDrawing();

    /**
     * Casts off the entire document.
     * Starting from a single system, create and fill pages and systems.
     */
    void CastOffDoc();

    /**
     * Undo the cast off of the entire document.
     * The document will then contain one single page with one single system.
     */
    void UnCastOffDoc();

    /**
     * Cast off of the entire document according to the encoded data (pb and sb).
     * Does not perform any check on the presence and / or validity of such data.
     */
    void CastOffEncodingDoc();

    /**
     * Convert the doc from score-based to page-based MEI.
     * Containers will be converted to boundaryStart / boundaryEnd.
     * Does not perform any check if the data needs or can be converted.
     */
    void ConvertToPageBasedDoc();

    /**
     * To be implemented.
     */
    void RefreshViews(){};

    /**
     * Set drawing values (page size, etc) when drawing a page.
     * By default, the page size of the document is taken.
     * If a page is given, the size of the page is taken.
     */
    Page *SetDrawingPage(int pageIdx);

    /**
     * Reset drawing page to NULL.
     * This might be necessary if we have replaced a page in the document.
     * We need to call this because otherwise looking at the page idx will fail.
     * See Doc::LayOut for an example.
     */
    void ResetDrawingPage() { m_drawingPage = NULL; }

    /**
     * Getter to the drawPage. Normally, getting the page should
     * be done with Doc::SetDrawingPage. This is only a method for
     * asserting that currently have the right page.
     */
    Page *GetDrawingPage() const { return m_drawingPage; }

    /**
     * Return the width adjusted to the content of the current drawing page.
     * This includes the appropriate left and right margins.
     */
    int GetAdjustedDrawingPageWidth() const;

    /**
     * Return the height adjusted to the content of the current drawing page.
     * This includes the appropriate top and bottom margin (using top as bottom).
     */
    int GetAdjustedDrawingPageHeight() const;

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::PrepareLyricsEnd
     */
    virtual int PrepareLyricsEnd(FunctorParams *functorParams);

private:
    /**
     * Calculates the music font size according to the m_interlDefin reference value.
     */
    int CalcMusicFontSize();

public:
    /**
     * A copy of the header tree stored as pugi::xml_document
     */
    pugi::xml_document m_header;

    /**
     * Holds the top scoreDef.
     * In a standard MEI file, this is the <scoreDef> encoded before the first <section>.
     */
    ScoreDef m_scoreDef;

    /** The current page height */
    int m_drawingPageHeight;
    /** The current page height */
    int m_drawingPageWidth;
    /** The current page witdth */
    int m_drawingPageLeftMar;
    /** The current page left margin */
    int m_drawingPageRightMar;
    /** The current page right margin */
    int m_drawingPageTopMar;
    /** the current beam minimal slope */
    float m_drawingBeamMinSlope;
    /** the current beam maximal slope */
    float m_drawingBeamMaxSlope;
    /** flag for disabling justification */
    bool m_drawingJustifyX;
    /** flag for disabling spacing by duration */
    bool m_drawingEvenSpacing;
    /** value of the linear spacing factor */
    double m_drawingSpacingLinear;
    /** value of the non linear spacing factor */
    double m_drawingSpacingNonLinear;
    /** minimum measure width */
    int m_drawingMinMeasureWidth;

private:
    /**
     * The type of document indicates how to deal with the layout information.
     * A Transcription document type means that the layout information is included
     * and that no layout algorithm should be applied.
     */
    DocType m_type;

    /**
     * The object with the default values.
     * This could be saved somewhere as preferences (todo).
     */
    Style *m_style;

    /*
     * The following values are set in the Doc::SetDrawingPage.
     * They are all current values to be used when drawing a page in a View and
     * reset for every page. However, most of them are based on the m_staffDefin values
     * and will remain the same. This can be optimized.
     * The pages dimensions and margins are based on the page ones, the document ones or
     * the default in the following order and if available.
     */

    /** The page currently being drawn */
    Page *m_drawingPage;
    /** Half a the space between to staff lines */
    int m_drawingUnit;
    /** Space between to staff lines */
    int m_drawingDoubleUnit;
    /** Height of a five line staff */
    int m_drawingStaffSize;
    /** Height of an octave */
    int m_drawingOctaveSize;
    /** Height of a beam (10 and 6 by default) */
    int m_drawingBeamWidth;
    /** Height of a beam spacing (white) (10 and 6 by default) */
    int m_drawingBeamWhiteWidth;
    /** Ledger line length and normal and grace size */
    int m_drawingLedgerLine;
    /** Brevis width */
    int m_drawingBrevisWidth;

    /** Smufl font size (100 par defaut) */
    int m_drawingSmuflFontSize;
    /** Lyric font size  */
    int m_drawingLyricFontSize;
    /** Current music font */
    FontInfo m_drawingSmuflFont;
    /** Current lyric font */
    FontInfo m_drawingLyricFont;

    /**
     * A flag to indicate whether the currentScoreDef has been set or not.
     * If yes, SetCurrentScoreDef will not parse the document (again) unless
     * the force parameter is set.
     */
    bool m_currentScoreDefDone;

    /**
     * A flag to indicate if the drawing preparation has been done. If yes,
     * drawing preparation will be reset before being done again.
     */
    bool m_drawingPreparationDone;

    /**
     * A flag to indicate if the MIDI export has been done.
     * This is necessary for retrieving notes being played at a certain time.
     */
    bool m_midiExportDone;

    /**
     * A flag to indicate that the MIDI timemap has been calculated.  The
     * timemap needs to be prepared before MIDI files or timemap JSON files
     * are generated.
     */
    bool m_hasMidiTimemap;

    /** Page width (MEI scoredef@page.width) - currently not saved */
    int m_pageWidth;
    /** Page height (MEI scoredef@page.height) - currently not saved */
    int m_pageHeight;
    /** Page left margin (MEI scoredef@page.leftmar) - currently not saved */
    short m_pageLeftMar;
    /** Page right margin (MEI scoredef@page.rightmar) - currently not saved */
    short m_pageRightMar;
    /** Page top margin (MEI scoredef@page.topmar) - currently not saved */
    short m_pageTopMar;
    /** Staff minimal spacing (MEI scoredef@spacing.staff) - currently not saved */
    short m_spacingStaff;
    /** System minimal spacing (MEI scoredef@spacing.system) - currently not saved */
    short m_spacingSystem;

    /**
     * A score buffer for loading or creating a scoreBased MEI.
     */
    Score *m_scoreBuffer;
};

} // namespace vrv

#endif
