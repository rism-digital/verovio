/////////////////////////////////////////////////////////////////////////////
// Name:        preparedatafunctor.h
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_PREPAREDATAFUNCTOR_H__
#define __VRV_PREPAREDATAFUNCTOR_H__

#include "functor.h"

namespace vrv {

class Facsimile;
class SystemMilestoneInterface;
class SymbolTable;

//----------------------------------------------------------------------------
// PrepareDataInitializationFunctor
//----------------------------------------------------------------------------

/**
 * This class is responsible for one time member initialization at the very begin.
 */
class PrepareDataInitializationFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    PrepareDataInitializationFunctor(Doc *doc);
    virtual ~PrepareDataInitializationFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitDiv(Div *div) override;
    FunctorCode VisitChord(Chord *chord) override;
    FunctorCode VisitFloatingObject(FloatingObject *floatingObject) override;
    FunctorCode VisitKeySig(KeySig *keySig) override;
    FunctorCode VisitRepeatMark(RepeatMark *repeatMark) override;
    FunctorCode VisitScore(Score *score) override;
    FunctorCode VisitTextLayoutElement(TextLayoutElement *textLayoutElement) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    //
};

//----------------------------------------------------------------------------
// PrepareCueSizeFunctor
//----------------------------------------------------------------------------

/**
 * This class sets the drawing cue size for all layer elements.
 */
class PrepareCueSizeFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    PrepareCueSizeFunctor();
    virtual ~PrepareCueSizeFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitLayerElement(LayerElement *layerElement) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    //
};

//----------------------------------------------------------------------------
// PrepareCrossStaffFunctor
//----------------------------------------------------------------------------

/**
 * This class sets the cross staff and cross layer pointers on layer elements.
 */
class PrepareCrossStaffFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    PrepareCrossStaffFunctor();
    virtual ~PrepareCrossStaffFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitLayerElement(LayerElement *layerElement) override;
    FunctorCode VisitLayerElementEnd(LayerElement *layerElement) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The current measure
    Measure *m_currentMeasure;
    // The current cross staff
    Staff *m_currentCrossStaff;
    // The current cross layer
    Layer *m_currentCrossLayer;
};

//----------------------------------------------------------------------------
// PrepareAltSymFunctor
//----------------------------------------------------------------------------

/**
 * This class matches the @altsym element to the corresponding symbolDef.
 */
class PrepareAltSymFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    PrepareAltSymFunctor();
    virtual ~PrepareAltSymFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Getter for the symbol table
     */
    SymbolTable *GetSymbolTable() const { return m_symbolTable; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitObject(Object *object) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The SymbolTable in the current scoreDef
    SymbolTable *m_symbolTable;
};

//----------------------------------------------------------------------------
// PrepareFacsimileFunctor
//----------------------------------------------------------------------------

/**
 * This class associates layer elements with @facs to the appropriate zone.
 */
class PrepareFacsimileFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    PrepareFacsimileFunctor(Facsimile *facsimile);
    virtual ~PrepareFacsimileFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Getter for the zoneless syllables
     */
    const ListOfObjects &GetZonelessSyls() const { return m_zonelessSyls; }

    /*
     * Getter for the facsimile
     */
    Facsimile *GetFacsimile() const { return m_facsimile; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitObject(Object *object) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The facsimile
    Facsimile *m_facsimile;
    // List of zoneless syllables
    ListOfObjects m_zonelessSyls;
};

//----------------------------------------------------------------------------
// PrepareLinkingFunctor
//----------------------------------------------------------------------------

/**
 * This class matches linking elements (e.g, @next).
 */
class PrepareLinkingFunctor : public Functor, public CollectAndProcess {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    PrepareLinkingFunctor();
    virtual ~PrepareLinkingFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Getter for the interface / id pairs
     */
    ///@{
    const MapOfLinkingInterfaceIDPairs &GetNextIDPairs() const { return m_nextIDPairs; }
    const MapOfLinkingInterfaceIDPairs &GetSameasIDPairs() const { return m_sameasIDPairs; }
    const MapOfNoteIDPairs &GetStemSameasIDPairs() const { return m_stemSameasIDPairs; }
    ///@}

    /*
     * Insert interface / id pairs
     */
    ///@{
    void InsertNextIDPair(const std::string &nextID, LinkingInterface *interface);
    void InsertSameasIDPair(const std::string &sameasID, LinkingInterface *interface);
    ///@}

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitObject(Object *object) override;
    ///@}

protected:
    //
private:
    /**
     * Resolve @stem.sameas links by instanciating Note::m_stemSameas (*Note).
     * Called twice - once to fill id / note pairs and once to resolve the link.
     * The link is bi-directional, which means that both notes have
     * their m_stemSameas pointer instanciated.
     */
    void ResolveStemSameas(Note *note);

public:
    //
private:
    // Holds the interface / id pairs to match for links
    MapOfLinkingInterfaceIDPairs m_nextIDPairs;
    // Holds the interface / id pairs to match for sameas
    MapOfLinkingInterfaceIDPairs m_sameasIDPairs;
    // Holds the note / id pairs to match for stem.sameas
    MapOfNoteIDPairs m_stemSameasIDPairs;
};

//----------------------------------------------------------------------------
// PreparePlistFunctor
//----------------------------------------------------------------------------

/**
 * This class matches elements of @plist.
 */
class PreparePlistFunctor : public Functor, public CollectAndProcess {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    PreparePlistFunctor();
    virtual ~PreparePlistFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Getter and modifier for the interface / id pairs
     */
    ///@{
    const ArrayOfPlistObjectIDPairs &GetInterfaceIDPairs() const { return m_plistObjectIDPairs; }
    void InsertInterfaceObjectIDPair(Object *objectWithPlist, const std::string &elementID);
    ///@}

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitObject(Object *object) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // Holds the object with plist / id pairs to match
    ArrayOfPlistObjectIDPairs m_plistObjectIDPairs;
};

//----------------------------------------------------------------------------
// PrepareDurationFunctor
//----------------------------------------------------------------------------

/**
 * This class extracts the default duration from scoredef/staffdef.
 */
class PrepareDurationFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    PrepareDurationFunctor();
    virtual ~PrepareDurationFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitLayerElement(LayerElement *layerElement) override;
    FunctorCode VisitScore(Score *score) override;
    FunctorCode VisitScoreDef(ScoreDef *scoreDef) override;
    FunctorCode VisitStaffDef(StaffDef *staffDef) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The current scoreDef default duration
    data_DURATION m_durDefault;
    // The current staffDef default durations
    std::map<int, data_DURATION> m_durDefaultForStaffN;
};

//----------------------------------------------------------------------------
// PrepareTimePointingFunctor
//----------------------------------------------------------------------------

/**
 * This class matches the start for TimePointingInterface elements (such as fermata or harm).
 */
class PrepareTimePointingFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    PrepareTimePointingFunctor();
    virtual ~PrepareTimePointingFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Getter and modifier for the interface / id tuples
     */
    ///@{
    void InsertInterfaceIDTuple(ClassId classID, TimePointInterface *interface);
    ///@}

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitF(F *f) override;
    FunctorCode VisitFloatingObject(FloatingObject *floatingObject) override;
    FunctorCode VisitLayerElement(LayerElement *layerElement) override;
    FunctorCode VisitMeasureEnd(Measure *measure) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The interface list that holds the current elements to match
    ListOfPointingInterClassIdPairs m_timePointingInterfaces;
};

//----------------------------------------------------------------------------
// PrepareTimeSpanningFunctor
//----------------------------------------------------------------------------

/**
 * This class matches start and end for TimeSpanningInterface elements (such as tie or slur).
 * If fillMode is set to false, only the remaining elements will be matched.
 * This is used when processing a second time in the other direction.
 */
class PrepareTimeSpanningFunctor : public Functor, public CollectAndProcess {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    PrepareTimeSpanningFunctor();
    virtual ~PrepareTimeSpanningFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Getter and modifier for the interface / owner pairs
     */
    ///@{
    const ListOfSpanningInterOwnerPairs &GetInterfaceOwnerPairs() const { return m_timeSpanningInterfaces; }
    void InsertInterfaceOwnerPair(Object *owner, TimeSpanningInterface *interface);
    ///@}

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitF(F *f) override;
    FunctorCode VisitFloatingObject(FloatingObject *floatingObject) override;
    FunctorCode VisitLayerElement(LayerElement *layerElement) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitMeasureEnd(Measure *measure) override;
    ///@}

protected:
    //
private:
    // Delegates to the pseudo functor of the interface
    FunctorCode CallPseudoFunctor(Object *timeSpanningObject);

public:
    //
private:
    // The interface list that holds the current elements to match
    ListOfSpanningInterOwnerPairs m_timeSpanningInterfaces;
    // Indicates whether we currently traverse a measure
    bool m_insideMeasure;
};

//----------------------------------------------------------------------------
// PrepareTimestampsFunctor
//----------------------------------------------------------------------------

/**
 * This class matches start and end for TimeSpanningInterface elements with tstamp(2) attributes.
 * It is performed only on TimeSpanningInterface elements without @startid (or @endid).
 * It adds to the start (and end) measure a TimeStampAttr to the Measure::m_tstamps.
 */
class PrepareTimestampsFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    PrepareTimestampsFunctor();
    virtual ~PrepareTimestampsFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Getter and modifier for the interface / object lists
     */
    ///@{
    const ListOfSpanningInterClassIdPairs &GetInterfaceIDPairs() const { return m_timeSpanningInterfaces; }
    void InsertInterfaceIDPair(ClassId classID, TimeSpanningInterface *interface);
    void InsertObjectBeatPair(Object *object, const data_MEASUREBEAT &beat);
    ///@}

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitDocEnd(Doc *doc) override;
    FunctorCode VisitF(F *f) override;
    FunctorCode VisitFloatingObject(FloatingObject *floatingObject) override;
    FunctorCode VisitMeasureEnd(Measure *measure) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The interface list that holds the current elements to match
    ListOfSpanningInterClassIdPairs m_timeSpanningInterfaces;
    // The list of tstamp2 elements to attach at the measure end
    ListOfObjectBeatPairs m_tstamps;
};

//----------------------------------------------------------------------------
// PreparePedalsFunctor
//----------------------------------------------------------------------------

/**
 * This class matches down and up pedal lines.
 */
class PreparePedalsFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    PreparePedalsFunctor(Doc *doc);
    virtual ~PreparePedalsFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitMeasureEnd(Measure *measure) override;
    FunctorCode VisitPedal(Pedal *pedal) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The current pedals to be linked / grouped
    std::list<Pedal *> m_pedalLines;
};

//----------------------------------------------------------------------------
// PreparePointersByLayerFunctor
//----------------------------------------------------------------------------

/**
 * This class processes by layer and sets drawing pointers.
 * Set Dot::m_drawingNote for Dot elements in mensural mode
 */
class PreparePointersByLayerFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    PreparePointersByLayerFunctor();
    virtual ~PreparePointersByLayerFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitDot(Dot *dot) override;
    FunctorCode VisitLayerElement(LayerElement *layerElement) override;
    FunctorCode VisitMeasureEnd(Measure *measure) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The current note, rest etc.
    LayerElement *m_currentElement;
    // The last dot
    Dot *m_lastDot;
};

//----------------------------------------------------------------------------
// PrepareLyricsFunctor
//----------------------------------------------------------------------------

/**
 * This class sets wordpos and connector ends.
 * The functor is processed by staff/layer/verse using Filters.
 * At the end, the functor closes opened syl in VisitDocEnd.
 */
class PrepareLyricsFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    PrepareLyricsFunctor();
    virtual ~PrepareLyricsFunctor() = default;
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
    FunctorCode VisitDocEnd(Doc *doc) override;
    FunctorCode VisitNote(Note *note) override;
    FunctorCode VisitSyl(Syl *syl) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The current Syl
    Syl *m_currentSyl;
    // The last Note or Chord
    LayerElement *m_lastNoteOrChord;
    // The penultimate Note or Chord
    LayerElement *m_penultimateNoteOrChord;
};

//----------------------------------------------------------------------------
// PrepareLayerElementPartsFunctor
//----------------------------------------------------------------------------

/**
 * This class adds LayerElement parts (stem, flag, dots, etc).
 */
class PrepareLayerElementPartsFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    PrepareLayerElementPartsFunctor();
    virtual ~PrepareLayerElementPartsFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitChord(Chord *chord) override;
    FunctorCode VisitNote(Note *note) override;
    FunctorCode VisitRest(Rest *rest) override;
    FunctorCode VisitTabDurSym(TabDurSym *tabDurSym) override;
    FunctorCode VisitTuplet(Tuplet *tuplet) override;
    ///@}

protected:
    //
private:
    // Create stem if it does not exist
    Stem *EnsureStemExists(Stem *stem, Object *parent) const;
    // Create dots if they should exist, otherwise remove them
    Dots *ProcessDots(Dots *dots, Object *parent, bool shouldExist) const;
    // Create flag if it should exist, otherwise remove it
    Flag *ProcessFlag(Flag *flag, Object *parent, bool shouldExist) const;

public:
    //
private:
    //
};

//----------------------------------------------------------------------------
// PrepareRptFunctor
//----------------------------------------------------------------------------

/**
 * This class sets mRpt drawing numbers (if required).
 * The functor is processed by staff/layer using Filters.
 */
class PrepareRptFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    PrepareRptFunctor(Doc *doc);
    virtual ~PrepareRptFunctor() = default;
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
    FunctorCode VisitMRpt(MRpt *mRpt) override;
    FunctorCode VisitStaff(Staff *staff) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The current MRpt
    MRpt *m_currentMRpt;
    // The data_BOOLEAN indicating if multiNumber
    data_BOOLEAN m_multiNumber;
};

//----------------------------------------------------------------------------
// PrepareDelayedTurnsFunctor
//----------------------------------------------------------------------------

/**
 * This class sets Turn::m_drawingEndNote for delayed turns.
 * Needs a first pass to fill the map, processed by staff/layer after that.
 */
class PrepareDelayedTurnsFunctor : public Functor, public CollectAndProcess {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    PrepareDelayedTurnsFunctor();
    virtual ~PrepareDelayedTurnsFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Getter for the map of delayed turns
     */
    const std::map<LayerElement *, Turn *> &GetDelayedTurns() const { return m_delayedTurns; }

    /*
     * Reset the current turn and element
     */
    void ResetCurrent();

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitLayerElement(LayerElement *layerElement) override;
    FunctorCode VisitTurn(Turn *turn) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The element to which a turn is pointing to
    LayerElement *m_previousElement;
    // The current chord
    Chord *m_currentChord;
    // The turn to which we want to set a m_drawingEndElement
    Turn *m_currentTurn;
    // A map of the delayed turns and the layer elements they point to
    std::map<LayerElement *, Turn *> m_delayedTurns;
};

//----------------------------------------------------------------------------
// PrepareMilestonesFunctor
//----------------------------------------------------------------------------

/**
 * This class sets the Measure of Ending.
 */
class PrepareMilestonesFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    PrepareMilestonesFunctor();
    virtual ~PrepareMilestonesFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Getter and modifier for the interface list
     */
    ///@{
    const std::vector<SystemMilestoneInterface *> &GetStartMilestones() const { return m_startMilestones; }
    void InsertStartMilestone(SystemMilestoneInterface *interface);
    ///@}

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitEditorialElement(EditorialElement *editorialElement) override;
    FunctorCode VisitEnding(Ending *ending) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitSection(Section *section) override;
    FunctorCode VisitSystemMilestone(SystemMilestoneEnd *systemMilestoneEnd) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The last measure
    Measure *m_lastMeasure;
    // The current ending
    Ending *m_currentEnding;
    // The list of start milestones
    std::vector<SystemMilestoneInterface *> m_startMilestones;
};

//----------------------------------------------------------------------------
// PrepareFloatingGrpsFunctor
//----------------------------------------------------------------------------

/**
 * This class groups FloatingObjects by drawingGrpId.
 * Also chains the Dynam and Hairpin.
 */
class PrepareFloatingGrpsFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    PrepareFloatingGrpsFunctor();
    virtual ~PrepareFloatingGrpsFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitDir(Dir *dir) override;
    FunctorCode VisitDynam(Dynam *dynam) override;
    FunctorCode VisitEnding(Ending *ending) override;
    FunctorCode VisitHairpin(Hairpin *hairpin) override;
    FunctorCode VisitHarm(Harm *harm) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitMeasureEnd(Measure *measure) override;
    FunctorCode VisitPedal(Pedal *pedal) override;
    FunctorCode VisitSystemMilestone(SystemMilestoneEnd *systemMilestoneEnd) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The previous ending
    Ending *m_previousEnding;
    // The dynams in the current measure
    std::vector<Dynam *> m_dynams;
    // The current hairpins to be linked / grouped
    std::vector<Hairpin *> m_hairpins;
    // The map of existing harms (based on @n)
    std::map<std::string, Harm *> m_harms;
};

//----------------------------------------------------------------------------
// PrepareStaffCurrentTimeSpanningFunctor
//----------------------------------------------------------------------------

/**
 * This class goes through all the TimeSpanningInterface elements and sets them for each staff
 * that is covered. At the end, it removes the TimeSpanningInterface element
 * from the list when the last measure is reached.
 */
class PrepareStaffCurrentTimeSpanningFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    PrepareStaffCurrentTimeSpanningFunctor();
    virtual ~PrepareStaffCurrentTimeSpanningFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Getter and modifier for the interface list
     */
    ///@{
    const ArrayOfObjects &GetTimeSpanningElements() const { return m_timeSpanningElements; }
    void InsertTimeSpanningElement(Object *element);
    ///@}

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitF(F *f) override;
    FunctorCode VisitFloatingObject(FloatingObject *floatingObject) override;
    FunctorCode VisitMeasureEnd(Measure *measure) override;
    FunctorCode VisitStaff(Staff *staff) override;
    FunctorCode VisitSyl(Syl *syl) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The currently running TimeSpanningInterface elements
    ArrayOfObjects m_timeSpanningElements;
};

//----------------------------------------------------------------------------
// PrepareRehPositionFunctor
//----------------------------------------------------------------------------

/**
 * This class resolves the Reh time pointing position in case none is set.
 */
class PrepareRehPositionFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    PrepareRehPositionFunctor();
    virtual ~PrepareRehPositionFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitReh(Reh *reh) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    //
};

//----------------------------------------------------------------------------
// PrepareBeamSpanElementsFunctor
//----------------------------------------------------------------------------

/**
 * This class gets the list of referenced elements for the BeamSpan and marks referenced
 * objects as contained in a BeamSpan.
 */
class PrepareBeamSpanElementsFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    PrepareBeamSpanElementsFunctor();
    virtual ~PrepareBeamSpanElementsFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitBeamSpan(BeamSpan *beamSpan) override;
    ///@}

protected:
    //
private:
    // Helper to get the element list for the beamSpan - elements are acquired from all layerElements
    // that are located in between start and end of the beamSpan
    ArrayOfObjects GetBeamSpanElementList(BeamSpan *beamSpan, Layer *layer, const Staff *staff) const;

public:
    //
private:
    //
};

} // namespace vrv

#endif // __VRV_PREPAREDATAFUNCTOR_H__
