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
#include "resources.h"
#include "scoredef.h"

namespace smf {
class MidiFile;
}

namespace vrv {

class DocSelection;
class PageRange;
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
    ///@}

    /**
     * Add a page to the document
     */
    bool IsSupportedChild(ClassId classId) override;

    /**
     * Clear the content of the document.
     */
    void Reset() override;

    /**
     *
     */
    void ResetToLoading();

    /**
     * Clear the selection pages.
     */
    void ClearSelectionPages();

    /**
     * Getter for the options
     */
    ///@{
    Options *GetOptions() { return m_options; }
    const Options *GetOptions() const { return m_options; }
    void SetOptions(Options *options) { (*m_options) = *options; }
    ///@}

    /**
     * Getter for the resources
     */
    ///@{
    const Resources &GetResources() const { return m_resources; }
    Resources &GetResourcesForModification() { return m_resources; }
    ///@}

    /**
     * Generate a document scoreDef when none is provided.
     * This only looks at the content first system of the document.
     */
    bool GenerateDocumentScoreDef();

    /**
     * Generate a document pgFoot if none is provided
     */
    void GenerateFooter();

    /**
     * Generate a document pgHead from the MEI header if none is provided
     */
    void GenerateHeader();

    /**
     * Generate measure numbers from measure attributes
     */
    bool GenerateMeasureNumbers();

    /**
     * Generate a minimal MEI header
     */
    void GenerateMEIHeader();

    /**
     * Convert the header to MEI basic by preserving only the fileDesc and its titleStmt
     */
    void ConvertHeaderToMEIBasic();

    /**
     * Getter and setter for the DocType.
     */
    ///@{
    DocType GetType() const { return m_type; }
    void SetType(DocType type);
    bool IsFacs() const { return (m_type == Facs); }
    bool IsRaw() const { return (m_type == Raw); }
    bool IsRendering() const { return (m_type == Rendering); }
    bool IsTranscription() const { return (m_type == Transcription); }
    ///@}

    /**
     * Check if the document has a page with the specified value
     */
    bool HasPage(int pageIdx) const;

    /**
     * Get the Pages in the visible Mdiv.
     * Will find it only when having read a pages-based MEI file,
     * or when a file was converted to page-based MEI.
     */
    ///@{
    Pages *GetPages();
    const Pages *GetPages() const;
    ///@}

    /**
     * Get the total page count
     */
    int GetPageCount() const;

    /**
     * Get the first scoreDef
     */
    ///@{
    ScoreDef *GetFirstScoreDef();
    const ScoreDef *GetFirstScoreDef() const;
    ///@}

    /**
     * Get all visible scores / the first visible score
     * Lazily updates the visible scores, hence not const
     */
    ///@{
    std::list<Score *> GetVisibleScores();
    Score *GetFirstVisibleScore();
    ///@}

    /**
     * Get the corresponding score for a node
     */
    ///@{
    Score *GetCorrespondingScore(const Object *object);
    const Score *GetCorrespondingScore(const Object *object) const;
    // Generic version that does not necessarily rely on precalculated visible scores
    Score *GetCorrespondingScore(const Object *object, const std::list<Score *> &scores);
    const Score *GetCorrespondingScore(const Object *object, const std::list<Score *> &scores) const;
    ///@}

    /**
     * Return true if the MIDI generation is already done
     */
    bool GetMidiExportDone() const;

    /**
     * @name Get the height or width for a glyph taking into account the staff and grace sizes
     */
    ///@{
    int GetGlyphHeight(char32_t code, int staffSize, bool graceSize) const;
    int GetGlyphWidth(char32_t code, int staffSize, bool graceSize) const;
    int GetGlyphLeft(char32_t code, int staffSize, bool graceSize) const;
    int GetGlyphRight(char32_t code, int staffSize, bool graceSize) const;
    int GetGlyphBottom(char32_t code, int staffSize, bool graceSize) const;
    int GetGlyphTop(char32_t code, int staffSize, bool graceSize) const;
    int GetGlyphAdvX(char32_t code, int staffSize, bool graceSize) const;
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
    int GetDrawingLedgerLineExtension(int staffSize, bool graceSize) const;
    int GetDrawingMinimalLedgerLineExtension(int staffSize, bool graceSize) const;
    int GetCueSize(int value) const;
    double GetCueScaling() const;
    ///@}

    Point ConvertFontPoint(const Glyph *glyph, const Point &fontPoint, int staffSize, bool graceSize) const;

    /**
     * @name Get the height or width for a text glyph taking into account the grace size.
     * The staff size must already be taken into account in the FontInfo
     */
    ///@{
    int GetTextGlyphHeight(char32_t code, const FontInfo *font, bool graceSize) const;
    int GetTextGlyphWidth(char32_t code, const FontInfo *font, bool graceSize) const;
    int GetTextGlyphAdvX(char32_t code, const FontInfo *font, bool graceSize) const;
    int GetTextGlyphDescender(char32_t code, const FontInfo *font, bool graceSize) const;
    int GetTextLineHeight(const FontInfo *font, bool graceSize) const;
    int GetTextXHeight(const FontInfo *font, bool graceSize) const;
    ///@}

    /**
     * @name Get the height or width for a glyph taking into account the staff and grace sizes
     * (no const because the size of the member font is changed)
     */
    ///@{
    FontInfo *GetDrawingSmuflFont(int staffSize, bool graceSize);
    FontInfo *GetDrawingLyricFont(int staffSize);
    FontInfo *GetFingeringFont(int staffSize);
    ///@}

    /**
     * Get the ratio between the lyric font size and the music font size.
     * This is used when the music font is used within text.
     */
    double GetMusicToLyricFontSizeRatio() const;

    /**
     * @name Getters for the object margins (left and right).
     * The margins are given in x * MEI UNIT
     */
    ///@{
    double GetLeftMargin(const ClassId classId) const;
    double GetLeftMargin(const Object *object) const;
    double GetRightMargin(const ClassId classId) const;
    double GetRightMargin(const Object *object) const;
    double GetLeftPosition() const;
    double GetBottomMargin(const ClassId classId) const;
    double GetTopMargin(const ClassId classId) const;
    ///@}

    /**
     * Get the default distance from the staff for the object
     * The distance is given in x * MEI UNIT
     */
    data_MEASUREMENTSIGNED GetStaffDistance(const Object *object, int staffIndex, data_STAFFREL staffPosition) const;

    /**
     * Prepare the timemap for MIDI and timemap file export.
     * Run trough all the layers and fill the score-time and performance timing variables.
     */
    void CalculateTimemap();

    /**
     * Check to see if the timemap has already been calculated.  This needs to return
     * true before ExportMIDI() or ExportTimemap() can export anything (These two functions
     * will automatically run CalculateTimemap() if HasTimemap() return false.
     */
    bool HasTimemap() const;

    /**
     * Export the document to a MIDI file.
     * Run trough all the layers and fill the MIDI file content.
     */
    void ExportMIDI(smf::MidiFile *midiFile);

    /**
     * Extract a timemap from the document to a JSON string.
     * Run trough all the layers and fill the timemap file content.
     */
    bool ExportTimemap(std::string &output, bool includeRests, bool includeMeasures, bool useFractions);

    /**
     *  Extract expansionMap from the document to JSON string.
     */
    bool ExportExpansionMap(std::string &output);

    /**
     * Extract music features to JSON string.
     */
    bool ExportFeatures(std::string &output, const std::string &options);

    /**
     * Set the initial scoreDef of each page.
     * This is necessary for integrating changes that occur within a page.
     * It uses the MusObject::SetPageScoreDef functor method for parsing the file.
     * This will be done only if m_currentScoreDefDone is false or force is true.
     */
    void ScoreDefSetCurrentDoc(bool force = false);

    /**
     * Optimize the scoreDef once the document is cast-off.
     */
    void ScoreDefOptimizeDoc();

    /**
     * Set the GrpSym start / end for each System once ScoreDef is set and (if necessary) optimized
     */
    void ScoreDefSetGrpSymDoc();

    /**
     * Prepare the document data.
     * This sets pointers and value and needs to be done after loading and any editing.
     * For example, it sets the approriate values for the lyrics connectors
     */
    void PrepareData();

    /**
     * Casts off the entire document.
     * Starting from a single system, create and fill pages and systems.
     */
    void CastOffDoc();

    /**
     * Casts off the entire document, only using the document's system breaks
     * if they would be close to the end in the normal document.
     */
    void CastOffSmartDoc();

    /**
     * Casts off the entire document, using the document's line breaks,
     * but adding its own page breaks.
     */
    void CastOffLineDoc();

    /**
     * Casts off the entire document, with options for obeying breaks.
     * @param useSb - true to use the sb from the document.
     * @param usePb - true to use the pb from the document.
     * @param smart - true to sometimes use encoded sb and pb.
     */
    void CastOffDocBase(bool useSb, bool usePb, bool smart = false);

    /**
     * Undo the cast off of the entire document.
     * The document will then contain one single page with one single system.
     */
    void UnCastOffDoc(bool resetCache = true);

    /**
     * Cast off of the entire document according to the encoded data (pb and sb).
     * Does not perform any check on the presence and / or validity of such data.
     */
    void CastOffEncodingDoc();

    /**
     * Convert the doc from score-based to page-based MEI.
     * Containers will be converted to systemMilestone / systemMilestoneEnd.
     * Does not perform any check if the data needs or can be converted.
     */
    void ConvertToPageBasedDoc();

    /**
     * Convert mensural MEI into cast-off (measure) segments looking at the barLine objects.
     * Segment positions occur where a barLine is set on all staves.
     * castOff parameters indicates if we perform cast off (true) or un-cast off
     */
    void ConvertToCastOffMensuralDoc(bool castOff);

    /**
     * Convert mensural MEI into CMN measure-based MEI.
     */
    void ConvertToCmnDoc();

    /**
     * Convert analytical encoding (@fermata, @tie) to correpsonding elements
     * By default, the element are used only for the rendering and not preserved in the MEI output
     * Permanent conversion discard analytical markup and elements will be preserved in the MEI output.
     */
    void ConvertMarkupDoc(bool permanent = true);

    /**
     * Calls the scoringUpFunctor and applies it.
     */
    void ScoringUpDoc();

    /*
     * Convert the doc from mensural to a flattened version with no ligatures and the selected editorial markup.
     */
    void ConvertToMensuralViewDoc();

    /**
     * Convert the doc from mensural to CMN.
     */
    void ConvertMensuralToCmnDoc();

    /**
     * Sync the coordinate provided trought <facsimile> to m_drawingFacsX/Y.
     * Call the SyncToFacsimile functor.
     */
    void SyncFromFacsimileDoc();

    /**
     * Sync the coordinate provided in rendering to a <facsimile>.
     * The document must have encoded layout and the option --break encoded must have enabled.
     */
    void SyncToFacsimileDoc();

    /**
     * Transpose the content of the doc.
     */
    void TransposeDoc();

    /**
     * Convert encoded <expansion> before rendering
     */
    void ExpandExpansions();

    /**
     * Set drawing values (page size, etc) when drawing a page.
     * By default, the page size of the document is taken.
     * If a page is given, the size of the page is taken.
     * The withPageRange parameter trigger layout of pages in the appropriate range.
     */
    Page *SetDrawingPage(int pageIdx, bool withPageRange = false);

    /**
     * Update the drawing page sizes when a page is set as drawing page.
     */
    void UpdatePageDrawingSizes();

    /**
     * Reset drawing page to NULL.
     * This might be necessary if we have replaced a page in the document.
     * We need to call this because otherwise looking at the page idx will fail.
     * See Doc::LayOut for an example.
     */
    void ResetDataPage() { m_drawingPage = NULL; }

    /**
     * Getter to the drawPage. Normally, getting the page should
     * be done with Doc::SetDrawingPage. This is only a method for
     * asserting that we currently have the right page.
     */
    ///@{
    Page *GetDrawingPage() { return m_drawingPage; }
    const Page *GetDrawingPage() const { return m_drawingPage; }
    ///@}

    /**
     * Check that the page is the drawing page or that they have no given dimensions
     */
    bool CheckPageSize(const Page *page) const;

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
    void SetMensuralMusicOnly(data_BOOLEAN isMensuralMusicOnly);
    bool IsMensuralMusicOnly() const { return (m_isMensuralMusicOnly == BOOLEAN_true); }
    ///@}

    /**
     * @name Setter for and getter for neume-line flag
     */
    ///@{
    void SetNeumeLines(bool isNeumeLines) { m_isNeumeLines = isNeumeLines; }
    bool IsNeumeLines() const { return m_isNeumeLines; }
    ///@}

    /**
     * @name Setter and getter for facsimile
     */
    ///@{
    void SetFacsimile(Facsimile *facsimile) { m_facsimile = facsimile; }
    Facsimile *GetFacsimile() { return m_facsimile; }
    const Facsimile *GetFacsimile() const { return m_facsimile; }
    bool HasFacsimile() const { return m_facsimile != NULL; }
    ///@}

    /**
     * Return true if the document has been cast off already.
     */
    bool IsCastOff() const { return m_isCastOff; }

    /**
     * @name Methods for managing a selection.
     */
    ///@{
    void InitSelectionDoc(DocSelection &selection, bool resetCache);
    void ResetSelectionDoc(bool resetCache);
    bool HasSelection() const;
    /**
     * Temporarily deactivate and reactivate selection.
     * Used for example to get the complete MEI data.
     * No check and cast-off performed.
     */
    void DeactiveateSelection();
    void ReactivateSelection(bool resetAligners);
    ///@}

    /**
     * Refresh the layout of all pages in the doc.
     */
    void RefreshLayout();

    /**
     * Reset the document focus
     */
    void SetFocus();

    //----------//
    // Functors //
    //----------//

    /**
     * Interface for class functor visitation
     */
    ///@{
    FunctorCode Accept(Functor &functor) override;
    FunctorCode Accept(ConstFunctor &functor) const override;
    FunctorCode AcceptEnd(Functor &functor) override;
    FunctorCode AcceptEnd(ConstFunctor &functor) const override;
    ///@}

private:
    /**
     * Calculates the music font size according to the m_interlDefin reference value.
     */
    int CalcMusicFontSize();

    /**
     * Generate the measure indices
     */
    void PrepareMeasureIndices();

    /**
     * Determine all visible scores
     */
    void CollectVisibleScores();

    /**
     * Reset the document focus
     */
    void ResetFocus();

public:
    Page *m_selectionPreceding;
    Page *m_selectionFollowing;
    std::string m_selectionStart;
    std::string m_selectionEnd;

    /**
     * A page range (owned object) with focus in the document.
     */
    PageRange *m_focusRange;

    /**
     * A copy of the header tree stored as pugi::xml_document
     */
    pugi::xml_document m_header;

    /**
     * A copy of the front tree stored as pugi::xml_document
     */
    pugi::xml_document m_front;

    /**
     * A copy of the back tree stored as pugi::xml_document
     */
    pugi::xml_document m_back;

    /**
     * The music@decls value
     */
    std::string m_musicDecls;

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

    /**
     * The resources (glyph table).
     */
    Resources m_resources;

    /**
     * The list of all visible scores
     * Used in Doc::GetCorrespondingScore to quickly determine the score for an object
     */
    std::list<Score *> m_visibleScores;

    /**
     * A flag indicating if the document has been cast off or not.
     */
    bool m_isCastOff;

    /**
     * A flag indicating the focus status (unset, set, used)
     */
    FocusStatusType m_focusStatus;

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
    /** Brevis width */
    int m_drawingBrevisWidth;

    /** Smufl font size (100 par defaut) */
    int m_drawingSmuflFontSize;
    /** Lyric font size  */
    int m_drawingLyricFontSize;
    /** Fingering font size*/
    int m_fingeringFontSize;
    /** Current music font */
    FontInfo m_drawingSmuflFont;
    /** Current lyric font */
    FontInfo m_drawingLyricFont;
    /** Current fingering font */
    FontInfo m_fingeringFont;

    /**
     * A flag to indicate whether the currentScoreDef has been set or not.
     * If yes, ScoreDefSetCurrentDoc will not parse the document (again) unless
     * the force parameter is set.
     */
    bool m_currentScoreDefDone;

    /**
     * A flag to indicate if the data preparation has been done. If yes,
     * data preparation will be reset before being done again.
     */
    bool m_dataPreparationDone;

    /**
     * A flag to indicate that the timemap has been calculated.  The
     * timemap needs to be prepared before MIDI files or timemap JSON files
     * are generated. Value is 0.0 when no timemap has been generated.
     */
    double m_timemapTempo;

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
    data_BOOLEAN m_isMensuralMusicOnly;

    /**
     * A flag to indicate that the document contains neume lines.
     * This is a special document type where neume lines are encoded with <section type="neon-neume-line">
     */
    bool m_isNeumeLines;

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
    Facsimile *m_facsimile;
};

} // namespace vrv

#endif
