/////////////////////////////////////////////////////////////////////////////
// Name:        miscfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_MISCFUNCTOR_H__
#define __VRV_MISCFUNCTOR_H__

#include "functor.h"

namespace vrv {

class LyricElement;

//----------------------------------------------------------------------------
// ApplyPPUFactorFunctor
//----------------------------------------------------------------------------

/**
 * This class applies the Pixel Per Unit factor of the page to its elements.
 */
class ApplyPPUFactorFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    ApplyPPUFactorFunctor(Page *page = NULL);
    virtual ~ApplyPPUFactorFunctor() = default;
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
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitPage(Page *page) override;
    FunctorCode VisitStaff(Staff *staff) override;
    FunctorCode VisitSurface(Surface *surface) override;
    FunctorCode VisitSystem(System *system) override;
    FunctorCode VisitZone(Zone *zone) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The current page
    Page *m_page;
};

//----------------------------------------------------------------------------
// GetAlignmentLeftRightFunctor
//----------------------------------------------------------------------------

/**
 * This class retrieves the minimum left and maximum right for an alignment.
 */
class GetAlignmentLeftRightFunctor : public ConstFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    GetAlignmentLeftRightFunctor();
    virtual ~GetAlignmentLeftRightFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Set the classes to exclude
     */
    void ExcludeClasses(const std::vector<ClassId> &excludeClasses) { m_excludeClasses = excludeClasses; }

    /*
     * Get the minimum left and maximum right
     */
    ///@{
    int GetMinLeft() const { return m_minLeft; }
    int GetMaxRight() const { return m_maxRight; }
    ///@}

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitObject(const Object *object) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The min left and right
    int m_minLeft;
    int m_maxRight;
    // The classes which are ignored
    std::vector<ClassId> m_excludeClasses;
};

//----------------------------------------------------------------------------
// InitProcessingListsFunctor
//----------------------------------------------------------------------------

/**
 * This class builds trees of ints with staff/layer and staff/layer/verse/volta-track numbers.
 */
class InitProcessingListsFunctor : public ConstFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    InitProcessingListsFunctor();
    virtual ~InitProcessingListsFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Getter for the int trees
     */
    ///@{
    const IntTree &GetLayerTree()
    {
        this->PrepareLyricElementTracks();
        return m_layerTree;
    }
    const IntTree &GetVerseTree();
    ///@}

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitLayer(const Layer *layer) override;
    FunctorCode VisitRefrain(const Refrain *refrain) override;
    FunctorCode VisitVerse(const Verse *verse) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    void PrepareLyricElementTracks();
    void CollectLyricElement(const LyricElement *lyricElement);

    // The IntTree for staff/layer
    IntTree m_layerTree;
    // The IntTree for staff/layer/verse/volta-track. Track 0 is direct verse content.
    IntTree m_verseTree;
    // Lyric elements collected before stable drawing and processing slots are assigned.
    std::vector<const LyricElement *> m_lyricElements;
    bool m_lyricElementTracksPrepared = false;
    // Stable volta track numbers, allocated in document order for each staff/layer/lyric group.
    std::map<std::tuple<int, int, int, std::string>, int> m_voltaTracks;
    std::map<std::tuple<int, int, int>, int> m_nextVoltaTrack;
    // Lyric elements grouped together so a direct-syllable track is reflected at every note.
    std::map<std::tuple<int, int, int>, std::vector<const LyricElement *>> m_lyricElementGroups;
    std::set<std::tuple<int, int, int>> m_directSylTrackGroups;
};

//----------------------------------------------------------------------------
// ReorderByXPosFunctor
//----------------------------------------------------------------------------

/**
 * This class reorders elements by x-position.
 */
class ReorderByXPosFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    ReorderByXPosFunctor();
    virtual ~ReorderByXPosFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

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
    //
};

} // namespace vrv

#endif // __VRV_MISCFUNCTOR_H__
