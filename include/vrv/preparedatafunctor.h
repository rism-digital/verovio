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
     * Getter for the fill list flag
     */
    bool FillList() const { return m_fillList; }

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
    // Indicates whether the pairs have to be stacked or not
    bool m_fillList;
};

} // namespace vrv

#endif // __VRV_PREPAREDATAFUNCTOR_H__
