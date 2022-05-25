/////////////////////////////////////////////////////////////////////////////
// Name:        tabgrp.h
// Author:      Laurent Pugin
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TABGRP_H__
#define __VRV_TABGRP_H__

#include "durationinterface.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// TabGrp
//----------------------------------------------------------------------------

/**
 * This class models the MEI <tabGrp> element.
 */
class TabGrp : public LayerElement, public ObjectListInterface, public DurationInterface {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    TabGrp();
    virtual ~TabGrp();
    void Reset() override;
    std::string GetClassName() const override { return "TabGrp"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    DurationInterface *GetDurationInterface() override { return vrv_cast<DurationInterface *>(this); }
    const DurationInterface *GetDurationInterface() const override { return vrv_cast<const DurationInterface *>(this); }
    ///@}

    /**
     * Add an element to a element.
     */
    bool IsSupportedChild(Object *object) override;

    /**
     * Return the top or bottom note or their Y position
     */
    ///@{
    Note *GetTopNote();
    const Note *GetTopNote() const;
    Note *GetBottomNote();
    const Note *GetBottomNote() const;
    int GetYTop() const;
    int GetYBottom() const;
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::InitOnsetOffsetEnd
     */
    virtual int InitOnsetOffsetEnd(FunctorParams *functorParams) override;

    /**
     * See Object::CalcStem
     */
    int CalcStem(FunctorParams *functorParams) override;

protected:
    /**
     * Filter the flat list and keep only Note elements.
     */
    void FilterList(ListOfConstObjects &childList) const override;

private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif
