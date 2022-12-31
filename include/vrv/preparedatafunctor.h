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
    FunctorCode VisitChord(Chord *chord) override;
    FunctorCode VisitFloatingObject(FloatingObject *floatingObject) override;
    FunctorCode VisitKeySig(KeySig *keySig) override;
    FunctorCode VisitRunningElement(RunningElement *runningElement) override;
    FunctorCode VisitScore(Score *score) override;
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
class PrepareCueSizeFunctor : public MutableFunctor {
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
class PrepareCrossStaffFunctor : public MutableFunctor {
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
class PrepareAltSymFunctor : public MutableFunctor {
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
class PrepareFacsimileFunctor : public MutableFunctor {
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
class PrepareLinkingFunctor : public MutableFunctor {
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
     * Getter and setter for the fill list flag
     */
    ///@{
    bool FillList() const { return m_fillList; }
    void FillList(bool fillList) { m_fillList = fillList; }
    ///@}

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
    // Indicates the current mode: fill vs process
    bool m_fillList;
};

//----------------------------------------------------------------------------
// PreparePlistFunctor
//----------------------------------------------------------------------------

/**
 * This class matches elements of @plist.
 */
class PreparePlistFunctor : public MutableFunctor {
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
     * Getter and setter for the fill list flag
     */
    ///@{
    bool FillList() const { return m_fillList; }
    void FillList(bool fillList) { m_fillList = fillList; }
    ///@}

    /*
     * Getter and modifier for the interface / id tuples
     */
    ///@{
    const ArrayOfPlistInterfaceIDTuples &GetInterfaceIDTuples() const { return m_interfaceIDTuples; }
    void InsertInterfaceIDTuple(const std::string &elementID, PlistInterface *interface);
    void ClearInterfaceIDTuples() { m_interfaceIDTuples.clear(); }
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
    // Holds the interface / id tuples to match
    ArrayOfPlistInterfaceIDTuples m_interfaceIDTuples;
    // Indicates the current mode: fill vs process
    bool m_fillList;
};

//----------------------------------------------------------------------------
// PrepareDurationFunctor
//----------------------------------------------------------------------------

/**
 * This class extracts the default duration from scoredef/staffdef.
 */
class PrepareDurationFunctor : public MutableFunctor {
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
class PrepareTimePointingFunctor : public MutableFunctor {
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
 * If fillList is set to false, only the remaining elements will be matched.
 * This is used when processing a second time in the other direction.
 */
class PrepareTimeSpanningFunctor : public MutableFunctor {
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
     * Getter and setter for the fill list flag
     */
    ///@{
    bool FillList() const { return m_fillList; }
    void FillList(bool fillList) { m_fillList = fillList; }
    ///@}

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
    ListOfSpanningInterOwnerPairs m_timeSpanningInterfaces;
    // Indicates the current mode: fill vs process
    bool m_fillList;
};

//----------------------------------------------------------------------------
// PrepareTimestampsFunctor
//----------------------------------------------------------------------------

/**
 * This class matches start and end for TimeSpanningInterface elements with tstamp(2) attributes.
 * It is performed only on TimeSpanningInterface elements without @startid (or @endid).
 * It adds to the start (and end) measure a TimeStampAttr to the Measure::m_tstamps.
 */
class PrepareTimestampsFunctor : public MutableFunctor {
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
    const ListOfSpanningInterClassIdPairs& GetInterfaceIDPairs() const { return m_timeSpanningInterfaces; }
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

} // namespace vrv

#endif // __VRV_PREPAREDATAFUNCTOR_H__
