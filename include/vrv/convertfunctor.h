/////////////////////////////////////////////////////////////////////////////
// Name:        convertfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CONVERTFUNCTOR_H__
#define __VRV_CONVERTFUNCTOR_H__

#include "functor.h"

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
    ConvertToCastOffMensuralFunctor(Doc *doc, System *targetSystem, const IntTree *layerTree);
    virtual ~ConvertToCastOffMensuralFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Setter for staff @n
     */
    ///@{
    void AddStaffN(int staffN) { m_staffNs.push_back(staffN); }
    void ClearStaffNs() { m_staffNs.clear(); }
    ///@}

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitBarLine(BarLine *barLine) override;
    FunctorCode VisitLayer(Layer *layer) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitObject(Object *object) override;
    FunctorCode VisitScoreDef(ScoreDef *scoreDef) override;
    FunctorCode VisitStaff(Staff *staff) override;
    FunctorCode VisitSyllable(Syllable *syllable) override;
    FunctorCode VisitSystemElement(SystemElement *systemElement) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The staff @n for finding splitting bar lines
    std::vector<int> m_staffNs;
    // The content layer from which we are copying the elements
    Layer *m_contentLayer;
    // The target system, measure, staff & layer
    System *m_targetSystem;
    Measure *m_targetMeasure;
    Staff *m_targetStaff;
    Layer *m_targetLayer;
    // A sub-system (e.g., section) to add measure segments
    System *m_targetSubSystem;
    // A counter for segments in the sub-system (section)
    int m_segmentIdx;
    // The total number of segments (previous sections)
    int m_segmentTotal;
    // An IntTree for processing by layer
    const IntTree *m_layerTree;
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

} // namespace vrv

#endif // __VRV_CONVERTFUNCTOR_H__
