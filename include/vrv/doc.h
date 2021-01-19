/////////////////////////////////////////////////////////////////////////////
// Name:        doc.h
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_DOC_H__
#define __VRV_DOC_H__

#include "devicecontextbase.h"
#include "expansionmap.h"
#include "facsimile.h"
#include "options.h"
#include "scoredef.h"

namespace smf {
class MidiFile;
}

namespace vrv {

class CastOffPagesParams;
class FontInfo;
class Glyph;
class Pages;
class Page;
class Score;

enum DocType { Raw = 0, Rendering, Transcription, Facs };

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
    virtual bool IsSupportedChild(Object *object);

    /**
     * Clear the content of the document.
     */
    virtual void Reset();

    /**
     * Refreshes the views from Doc.
     */
    virtual void Refresh();

    /**
     * Getter for the options
     */
    Options *GetOptions() const { return m_options; }
    void SetOptions(Options *options) { (*m_options) = *options; };

    /**
     * Generate a document scoreDef when none is provided.
     * This only looks at the content first system of the document.
     */
    bool GenerateDocumentScoreDef();

    /**
     * Generate a document pgFoot if none is provided
     */
    bool GenerateFooter();

    /**
     * Generate a document pgHead from the MEI header if none is provided
     */
    bool GenerateHeader();

    /**
     * Generate measure numbers from measure attributes
     */
    bool GenerateMeasureNumbers();

    /**
     * Getter and setter for the DocType.
     * The setter resets the document.
     */
    DocType GetType() const { return m_type; }
    void SetType(DocType type);

    /**
     * Check if the document has a page with the specified value
     */
    bool HasPage(int pageIdx);

    /**
     * Get the Score in the visible Mdiv.
     * Will find it only when having read a score-based MEI file
     */
    Score *GetScore();

    /**
     * Get the Pages in the visible Mdiv.
     * Will find it only when having read a pages-based MEI file,
     * or when a file was converted to page-based MEI.
     */
    Pages *GetPages();

    /**
     * Get the total page count
     */
    int GetPageCount();

    /**
     * Return true if the MIDI generation is already done
     */
    bool GetMidiExportDone() const;

    /**
     * @name Get the height or width for a glyph taking into account the staff and grace sizes
     */
    ///@{
    int GetGlyphHeight(wchar_t code, int staffSize, bool graceSize) const;
    int GetGlyphWidth(wchar_t code, int staffSize, bool graceSize) const;
    int GetGlyphDescender(wchar_t code, int staffSize, bool graceSize) const;
    int GetGlyphAdvX(wchar_t code, int staffSize, bool graceSize) const;
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
    int GetTextGlyphAdvX(wchar_t code, FontInfo *font, bool graceSize) const;
    int GetTextGlyphDescender(wchar_t code, FontInfo *font, bool graceSize) const;
    int GetTextLineHeight(FontInfo *font, bool graceSize) const;
    int GetTextXHeight(FontInfo *font, bool graceSize) const;
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
     * @name Getters for the object margins (left and right).
     * The margins are given in x * MEI UNIT
     */
    ///@{
    double GetLeftMargin(const ClassId classId) const;
    double GetRightMargin(const ClassId classId) const;
    double GetLeftPosition() const;
    double GetBottomMargin(const ClassId classId) const;
    double GetTopMargin(const ClassId classId) const;
    ///@}

    /**
     * Prepare the MIDI timemap for MIDI and timemap file export.
     * Run trough all the layers and fill the score-time and performance timing variables.
     */
    void CalculateMidiTimemap();

    /**
     * Check to see if the MIDI timemap has already been calculated.  This needs to return
     * true before ExportMIDI() or ExportTimemap() can export anything (These two functions
     * will automatically run CalculateMidiTimemap() if HasMidiTimemap() return false.
     */
    bool HasMidiTimemap();

    /**
     * Export the document to a MIDI file.
     * Run trough all the layers and fill the midi file content.
     */
    void ExportMIDI(smf::MidiFile *midiFile);

    /**
     * Extract a timemap from the document to a JSON string.
     * Run trough all the layers and fill the timemap file content.
     */
    bool ExportTimemap(std::string &output);
    void PrepareJsonTimemap(std::string &output, std::map<double, double> &realTimeToScoreTime,
        std::map<double, std::vector<std::string> > &realTimeToOnElements,
        std::map<double, std::vector<std::string> > &realTimeToOffElements, std::map<double, int> &realTimeToTempo);

    /**
     * Set the initial scoreDef of each page.
     * This is necessary for integrating changes that occur within a page.
     * It uses the MusObject::SetPageScoreDef functor method for parsing the file.
     * This will be done only if m_currentScoreDefDone is false or force is true.
     */
    void SetCurrentScoreDefDoc(bool force = false);

    /**
     * Check whether we need to optimize score based on the condense option
     */
    bool IsOptimizationNeeded();

    /**
     * Optimize the scoreDef once the document is cast-off.
     */
    void OptimizeScoreDefDoc();

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
     * Casts off the entire document, using the document's line breaks,
     * but adding its own page breaks.
     */
    void CastOffLineDoc();

    /**
     * Casts off the entire document, with options for obeying breaks.
     * @param useSb - true to use the sb from the document.
     * @param usePg - true to use the pb from the document.
     */
    void CastOffDocBase(bool useSb, bool usePb);

    /**
     * Casts off the running elements (headers and footer)
     * Called from Doc::CastOffDoc
     * The doc needs to be empty, the methods adds two empty pages to calculate the
     * size of the header and footer of the page one and two.
     * Calcultated sizes are set in the CastOffPagesParams object.
     */
    void CastOffRunningElements(CastOffPagesParams *params);

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
     * Convert mensural MEI into cast-off (measure) segments looking at the barLine objects.
     * Segment positions occur where a barLine is set on all staves.
     */
    void ConvertToCastOffMensuralDoc();

    /**
     * Reverse of ConvertToCastOffMensuralDoc()
     */
    void ConvertToUnCastOffMensuralDoc();

    /**
     * Convert scoreDef / staffDef attributes (clef.*, key.*, meter.*, etc.) to corresponding elements
     * By default, the element are used only for the rendering and not preserved in the MEI output
     * Permanent conversion discard analytical markup and elements will be preserved in the MEI output.
     */
    void ConvertScoreDefMarkupDoc(bool permanent = false);

    /**
     * Convert analytical encoding (@fermata, @tie) to correpsonding elements
     * By default, the element are used only for the rendering and not preserved in the MEI output
     * Permanent conversion discard analytical markup and elements will be preserved in the MEI output.
     */
    void ConvertMarkupDoc(bool permanent = false);

    /**
     * Transpose the content of the doc.
     */
    void TransposeDoc();

    /**
     * Convert encoded <expansion> before rendering
     */
    void ExpandExpansions();

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

    /**
     * Setter for markup flag. See corresponding enum in vrvdef.h
     * Set when reading the file to indicate what markup conversion needs to be applied.
     * See Doc::ConvertMarkupDoc
     */
    void SetMarkup(int markup) { m_markup |= markup; }

    /**
     * @name Setter for and getter for mensural only flag
     */
    ///@{
    void SetMensuralMusicOnly(bool isMensuralMusicOnly) { m_isMensuralMusicOnly = isMensuralMusicOnly; }
    bool IsMensuralMusicOnly() const { return m_isMensuralMusicOnly; }
    ///@}

    /**
     * @name Setter and getter for facsimile
     */
    ///@{
    void SetFacsimile(Facsimile *facsimile) { m_facsimile = facsimile; }
    Facsimile *GetFacsimile() { return m_facsimile; }
    bool HasFacsimile() const { return m_facsimile != NULL; }
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::PrepareLyricsEnd
     */
    virtual int PrepareLyricsEnd(FunctorParams *functorParams);

    /**
     * See Object::PrepareTimestampsEnd
     */
    virtual int PrepareTimestampsEnd(FunctorParams *functorParams);

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
     * A copy of the header tree stored as pugi::xml_document
     */
    pugi::xml_document m_front;

    /**
     * A copy of the header tree stored as pugi::xml_document
     */
    pugi::xml_document m_back;

    /**
     * Holds the top scoreDef.
     * In a standard MEI file, this is the <scoreDef> encoded before the first <section>.
     */
    ScoreDef m_mdivScoreDef;

    /** The current page height */
    int m_drawingPageHeight;
    /** The current page width */
    int m_drawingPageWidth;
    /** The current page content height (without margings) */
    int m_drawingPageContentHeight;
    /** The current page content width (without margins) */
    int m_drawingPageContentWidth;
    /** The current page bottom margin */
    int m_drawingPageMarginBottom;
    /** The current page left margin */
    int m_drawingPageMarginLeft;
    /** The current page right margin */
    int m_drawingPageMarginRight;
    /** The current page top margin */
    int m_drawingPageMarginTop;
    /** the current beam minimal slope */
    float m_drawingBeamMinSlope;
    /** the current beam maximal slope */
    float m_drawingBeamMaxSlope;

    /**
     * Record notation type for document.
     * (This should be improved by storing a vector of all notation types of the document for cases mixing notations)
     */
    data_NOTATIONTYPE m_notationType;

    /** An expansion map that contains  */
    ExpansionMap m_expansionMap;

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
    Options *m_options;

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
     * A flag to indicate that the MIDI timemap has been calculated.  The
     * timemap needs to be prepared before MIDI files or timemap JSON files
     * are generated. Value is 0.0 when no timemap has been generated.
     */
    double m_MIDITimemapTempo;

    /**
     * A flag to indicate whereas the document contains analytical markup to be converted.
     * This is currently limited to @fermata and @tie. Other attribute markup (@accid and @artic)
     * is converted during the import in MEIInput.
     */
    int m_markup;

    /**
     * A flag to indicate whereas to document contains only mensural music.
     * Mensural only music will be converted to cast-off segments by Doc::ConvertToCastOffMensuralDoc
     */
    bool m_isMensuralMusicOnly;

    /** Page width (MEI scoredef@page.width) - currently not saved */
    int m_pageWidth;
    /** Page height (MEI scoredef@page.height) - currently not saved */
    int m_pageHeight;
    /** Page bottom margin (MEI scoredef@page.botmar) - currently not saved */
    int m_pageMarginBottom;
    /** Page left margin (MEI scoredef@page.leftmar) - currently not saved */
    int m_pageMarginLeft;
    /** Page right margin (MEI scoredef@page.rightmar) - currently not saved */
    int m_pageMarginRight;
    /** Page top margin (MEI scoredef@page.topmar) - currently not saved */
    int m_pageMarginTop;

    /** Facsimile information */
    Facsimile *m_facsimile = NULL;
};

} // namespace vrv

#endif
