/////////////////////////////////////////////////////////////////////////////
// Name:        verse.h
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_VERSE_H__
#define __VRV_VERSE_H__

#include "lyricelement.h"

namespace vrv {

class LabelAbbr;

//----------------------------------------------------------------------------
// Verse
//----------------------------------------------------------------------------

class Verse : public LyricElement, public AttNInteger {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Verse();
    virtual ~Verse();
    Object *Clone() const override { return new Verse(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "verse"; }
    ///@}

    /**
     * Add an element (a syl) to a verse.
     * Only Syl elements will be actually added to the verse.
     */
    bool IsSupportedChild(ClassId classId) override;

    /**
     * @name Getter and setter for the labelAbbr
     */
    ///@{
    LabelAbbr *GetDrawingLabelAbbr() { return m_drawingLabelAbbr; }
    const LabelAbbr *GetDrawingLabelAbbr() const { return m_drawingLabelAbbr; }
    void SetDrawingLabelAbbr(LabelAbbr *labelAbbr) { m_drawingLabelAbbr = labelAbbr; }
    ///@}

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
    //
public:
    //
private:
    /**
     *  A pointer to the labelAbbr
     */
    LabelAbbr *m_drawingLabelAbbr;
};

} // namespace vrv

#endif
