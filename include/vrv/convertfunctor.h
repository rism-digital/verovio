/////////////////////////////////////////////////////////////////////////////
// Name:        convertfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CONVERTFUNCTOR_H__
#define __VRV_CONVERTFUNCTOR_H__

#include "functor.h"

//----------------------------------------------------------------------------

#include "alignfunctor.h"

namespace vrv {

//----------------------------------------------------------------------------
// ConvertToPageBasedFunctor
//----------------------------------------------------------------------------

/**
 * This class converts all top-level containers (section, endings) and editorial elements to milestone elements.
 */
class ConvertToPageBasedFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    ConvertToPageBasedFunctor(Page *page);
    virtual ~ConvertToPageBasedFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitDiv(Div *div) override;
    FunctorCode VisitEditorialElement(EditorialElement *editorialElement) override;
    FunctorCode VisitEditorialElementEnd(EditorialElement *editorialElement) override;
    FunctorCode VisitEnding(Ending *ending) override;
    FunctorCode VisitEndingEnd(Ending *ending) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitMdiv(Mdiv *mdiv) override;
    FunctorCode VisitMdivEnd(Mdiv *mdiv) override;
    FunctorCode VisitScore(Score *score) override;
    FunctorCode VisitScoreEnd(Score *score) override;
    FunctorCode VisitScoreDef(ScoreDef *scoreDef) override;
    FunctorCode VisitSection(Section *section) override;
    FunctorCode VisitSectionEnd(Section *section) override;
    FunctorCode VisitSystemElement(SystemElement *systemElement) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The system we are moving the content to
    System *m_currentSystem;
    // The page being processed
    Page *m_page;
};

//----------------------------------------------------------------------------
// ConvertToCastOffMensuralFunctor
//----------------------------------------------------------------------------

/**
 * This class converts mensural MEI into cast-off (measure) segments looking at the barLine objects.
 * Segment positions occur where a barLine is set on all staves.
 */
class ConvertToCastOffMensuralFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    ConvertToCastOffMensuralFunctor(Doc *doc, System *targetSystem);
    virtual ~ConvertToCastOffMensuralFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitLayer(Layer *layer) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitObject(Object *object) override;
    FunctorCode VisitScoreDef(ScoreDef *scoreDef) override;
    FunctorCode VisitStaff(Staff *staff) override;
    FunctorCode VisitSystemElement(SystemElement *systemElement) override;
    ///@}

protected:
    //
private:
    /** Check if the alignment is a valid breakpoint */
    bool IsValidBreakPoint(const Alignment *alignment, const int nbLayers);
    /** Create the staff and layer when a new segment starts */
    void InitSegment(Object *object);

public:
    //
private:
    /** The list of segments (i.e., measures) we are going to create */
    std::list<Measure *> m_segments;
    /** The current segment, reset at for every staff/layer */
    std::list<Measure *>::iterator m_currentSegment;
    /** The list of break points (one less than the segments) */
    std::list<const Alignment *> m_breakPoints;
    /** The current breakpoint, reset for every staff/layer */
    std::list<const Alignment *>::const_iterator m_currentBreakPoint;
    // The content layer from which we are copying the elements
    Staff *m_contentStaff;
    Layer *m_contentLayer;
    // The target system, measure, staff & layer
    System *m_targetSystem;
    Staff *m_targetStaff;
    Layer *m_targetLayer;
};

//----------------------------------------------------------------------------
// ConvertToUnCastOffMensuralFunctor
//----------------------------------------------------------------------------

/**
 * This class converts cast-off (measure) mensural segments MEI into mensural.
 */
class ConvertToUnCastOffMensuralFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    ConvertToUnCastOffMensuralFunctor();
    virtual ~ConvertToUnCastOffMensuralFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Getter and setter for various properties
     */
    ///@{
    void ResetContent();
    void TrackSegmentsToDelete(bool trackSegments) { m_trackSegmentsToDelete = trackSegments; }
    const ArrayOfObjects &GetSegmentsToDelete() const { return m_segmentsToDelete; }
    ///@}

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitLayer(Layer *layer) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitSection(Section *section) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The content/target measure and layer => NULL at the beginning of a section
    Measure *m_contentMeasure;
    Layer *m_contentLayer;
    // Indicates if we keep a reference of the measure segments to delete at the end
    bool m_trackSegmentsToDelete;
    // Measure segments to delete at the end (fill in the first pass only)
    ArrayOfObjects m_segmentsToDelete;
};

//----------------------------------------------------------------------------
// ConvertToCmnFunctor
//----------------------------------------------------------------------------

/**
 * This class converts mensural MEI into cast-off (measure) segments looking at the barLine objects.
 * Segment positions occur where a barLine is set on all staves.
 */
class ConvertToCmnFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    ConvertToCmnFunctor(Doc *doc, System *targetSystem, Score *score);
    virtual ~ConvertToCmnFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitChord(Chord *chord) override;
    FunctorCode VisitLayer(Layer *layer) override;
    FunctorCode VisitLayerElement(LayerElement *object) override;
    FunctorCode VisitLigature(Ligature *ligature) override;
    FunctorCode VisitLigatureEnd(Ligature *ligature) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitMeasureEnd(Measure *measure) override;
    FunctorCode VisitNote(Note *note) override;
    FunctorCode VisitRest(Rest *rest) override;
    FunctorCode VisitScoreDef(ScoreDef *scoreDef) override;
    FunctorCode VisitStaff(Staff *staff) override;
    FunctorCode VisitStaffEnd(Staff *staff) override;
    FunctorCode VisitSystemElement(SystemElement *systemElement) override;
    FunctorCode VisitSystemEnd(System *system) override;
    ///@}

protected:
    //
private:
    /** Check if the alignment correspond to a global mensur change */
    bool IsGlobalMensur(const Alignment *alignment, const int nbLayers, Mensur &mensur);
    /** */
    Fraction CalcMeasureDuration(const Mensur &mensur);
    /** Convert the objects with a DurationInterface into corresponding CMN notes or rests */
    void ConvertDurationInterface(DurationInterface *interface, ClassId classId);
    /** Split DurationInterface objects into CMN ones - including over several measures with a recursive call */
    void SplitDurationInterface(ClassId classId, data_DURATION elementDur, Fraction time, Fraction duration);
    /** Convert accid (mensural) into cmn accids - when not the first note, added as `@accid.ges` */
    void ConvertAccid(Note *cmnNote, const Accid *accid, bool &isFirstNote);
    /** Convert clef (mensural) into cmn clef */
    void ConvertClef(Clef *cmnClef, const Clef *clef);
    /** Convert mensur to dir */
    void ConvertMensur(const Mensur *mensur);

    /** Internal class for holding information about the mensur considered for meter signature (changes) */
    class MensurInfo {
    public:
        Mensur m_mensur;
        Fraction m_time;
    };

    /** Internal class for holding infromation about the measures (time, duration) */
    class MeasureInfo {
    public:
        MeasureInfo(const Fraction &time, const Fraction &duration) : m_time(time), m_duration(duration)
        {
            m_measure = NULL;
        }

    public:
        Measure *m_measure;
        Fraction m_time;
        Fraction m_duration;
    };

    /** Internal class for holding information about the CMN notes to be generated in the process */
    class CmnDuration {
    public:
        CmnDuration(data_DURATION duration, int dots, int num = 1, int numbase = 1)
            : m_duration(duration), m_dots(dots), m_num(num), m_numbase(numbase)
        {
        }

    public:
        data_DURATION m_duration;
        int m_dots;
        int m_num;
        int m_numbase;
    };

    /**
     * Split a mensural duration into corresponding CMN durations - which can be more than one.
     */
    void SplitDurationIntoCmn(
        data_DURATION elementDur, Fraction duration, const Mensur *mensur, std::list<CmnDuration> &cmnDurations);

public:
    //
private:
    /** The score with the main scoreDef */
    Score *m_score;
    /** The list of segments (i.e., measures) we are going to create */
    std::vector<MeasureInfo> m_measures;
    /** The current segment, reset at for every staff/layer */
    std::vector<MeasureInfo>::iterator m_currentMeasure;
    /** The list of break points (one less than the measures) */
    std::vector<Layer *> m_layers;
    /** The current breakpoint, reset for every staff/layer */
    std::vector<Layer *>::iterator m_currentLayer;
    /** A list with the clef for each staff */
    std::list<Clef *> m_clefs;
    /** The first clef encountered in the layer */
    Clef *m_layerClef;
    /** The target system, measure, staff & layer */
    System *m_targetSystem;
    /** The current Mensur and Proport */
    AlignMeterParams m_currentParams;
    /** List of duration element potentially splitted across measures */
    ListOfObjects m_durationElements;
    /** Bracket span for ligature, which also acts as a flag */
    BracketSpan *m_ligature;
    /** Bracket span for coloration, which also acts as a flag */
    BracketSpan *m_coloration;
    /** Proportion tuplet */
    Tuplet *m_proportTuplet;
    /** The current staffN */
    Staff *m_currentStaff;
    /** The current startid (empty string means beginning of the measure and tstamp 0.0) */
    std::string m_startid;
    /** The number of CMN measures generated */
    int m_n;
};

//----------------------------------------------------------------------------
// ConvertMarkupAnalyticalFunctor
//----------------------------------------------------------------------------

/**
 * This class converts analytical markup (\@fermata, \@tie) to elements.
 */
class ConvertMarkupAnalyticalFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    ConvertMarkupAnalyticalFunctor(bool permanent);
    virtual ~ConvertMarkupAnalyticalFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Get the notes with unresolved open ties
     */
    const std::vector<Note *> &GetCurrentNotes() const { return m_currentNotes; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitChord(Chord *chord) override;
    FunctorCode VisitChordEnd(Chord *chord) override;
    FunctorCode VisitMeasureEnd(Measure *measure) override;
    FunctorCode VisitMRest(MRest *mRest) override;
    FunctorCode VisitNote(Note *note) override;
    FunctorCode VisitRest(Rest *rest) override;
    ///@}

protected:
    //
private:
    // Helper for converting markup
    void ConvertToFermata(Fermata *fermata, AttFermataPresent *fermataPresent, const std::string &id);

public:
    //
private:
    // The current notes with open ties
    std::vector<Note *> m_currentNotes;
    // The current chord (if in a chord)
    Chord *m_currentChord;
    // Control events to be added to the measure (at its end)
    ArrayOfObjects m_controlEvents;
    // Indicates whether the conversion is permanent
    bool m_permanent;
};

//----------------------------------------------------------------------------
// ConvertMarkupArticFunctor
//----------------------------------------------------------------------------

/**
 * This class converts markup of artic@artic multi value into distinct artic elements.
 */
class ConvertMarkupArticFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    ConvertMarkupArticFunctor();
    virtual ~ConvertMarkupArticFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitArtic(Artic *artic) override;
    FunctorCode VisitLayerEnd(Layer *layer) override;
    ///@}

protected:
    //
private:
    /**
     * Split the multi-valued artic attributes into distinct artic elements.
     */
    void SplitMultival(Artic *artic) const;

public:
    //
private:
    // The articulations that need to be converted
    std::vector<Artic *> m_articsToConvert;
};

//----------------------------------------------------------------------------
// ConvertMarkupScoreDefFunctor
//----------------------------------------------------------------------------

/**
 * This class moves scoreDef clef, keySig, meterSig and mensur to staffDef.
 * When a staffDef already has one, it is not replaced.
 */
class ConvertMarkupScoreDefFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    ConvertMarkupScoreDefFunctor(Doc *doc);
    virtual ~ConvertMarkupScoreDefFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitScore(Score *score) override;
    FunctorCode VisitScoreDefElement(ScoreDefElement *scoreDefElement) override;
    FunctorCode VisitScoreDefElementEnd(ScoreDefElement *scoreDefElement) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The scoreDef we are moving the content from
    ScoreDefElement *m_currentScoreDef;
};

//----------------------------------------------------------------------------
// ConvertToMensuralViewFunctor
//----------------------------------------------------------------------------

/**
 * This class moves scoreDef clef, keySig, meterSig and mensur to staffDef.
 * When a staffDef already has one, it is not replaced.
 */
class ConvertToMensuralViewFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    ConvertToMensuralViewFunctor(Doc *doc);
    virtual ~ConvertToMensuralViewFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitEditorialElement(EditorialElement *editorialElement) override;
    FunctorCode VisitLayer(Layer *layer) override;
    FunctorCode VisitLayerEnd(Layer *layer) override;
    FunctorCode VisitLayerElement(LayerElement *layerElement) override;
    FunctorCode VisitLayerElementEnd(LayerElement *layerElement) override;
    FunctorCode VisitLigature(Ligature *ligature) override;
    FunctorCode VisitLigatureEnd(Ligature *ligature) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    /** The layer view we are flattening the content to */
    Layer *m_viewLayer;
    /** The stack of current objects */
    ListOfObjects m_stack;
};

} // namespace vrv

#endif // __VRV_CONVERTFUNCTOR_H__
