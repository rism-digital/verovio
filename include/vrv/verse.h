/////////////////////////////////////////////////////////////////////////////
// Name:        verse.h
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_VERSE_H__
#define __VRV_VERSE_H__

#include "atts_shared.h"
#include "layerelement.h"

namespace vrv {

class LabelAbbr;
class Syl;

//----------------------------------------------------------------------------
// Verse
//----------------------------------------------------------------------------

class Verse : public LayerElement, public AttColor, public AttLang, public AttNInteger, public AttTypography {
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
    std::string GetClassName() const override { return "Verse"; }
    ///@}

    /**
     * Add an element (a syl) to a verse.
     * Only Syl elements will be actually added to the verse.
     */
    bool IsSupportedChild(Object *object) override;

    /**
     * @name Getter for the labelAbbr
     */
    ///@{
    LabelAbbr *GetDrawingLabelAbbr() { return m_drawingLabelAbbr; }
    const LabelAbbr *GetDrawingLabelAbbr() const { return m_drawingLabelAbbr; }
    ///@}

    /**
     * Calculate the adjustment according to the overlap and the free space available before.
     * Will move the verse accordingly.
     * Called from Verse::AdjustSylSpacing and System::AdjustSylSpacingEnd
     */
    int AdjustPosition(int &overlap, int freeSpace, const Doc *doc);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::AlignVertically
     */
    int AlignVertically(FunctorParams *functorParams) override;

    /**
     * See Object::AdjustSylSpacing
     */
    int AdjustSylSpacing(FunctorParams *functorParams) override;

    /**
     * See Object::InitProcessingLists
     */
    int InitProcessingLists(FunctorParams *functorParams) override;

    /**
     * See Object::GenerateMIDI
     */
    int GenerateMIDI(FunctorParams *) override;

    /**
     * See Object::ResetData
     */
    int ResetData(FunctorParams *functorParams) override;

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
