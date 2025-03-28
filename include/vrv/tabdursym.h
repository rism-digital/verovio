/////////////////////////////////////////////////////////////////////////////
// Name:        tabdursym.h
// Author:      Laurent Pugin
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TABDURSYM_H__
#define __VRV_TABDURSYM_H__

#include "atts_shared.h"
#include "atts_stringtab.h"
#include "drawinginterface.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// TabDurSym
//----------------------------------------------------------------------------

/**
 * This class models the MEI <tabDurSym> element.
 */
class TabDurSym : public LayerElement,
                  public StemmedDrawingInterface,
                  public AttNNumberLike,
                  public AttStringtab,
                  public AttVisualOffsetVo {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    TabDurSym();
    virtual ~TabDurSym();
    Object *Clone() const override { return new TabDurSym(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "tabDurSym"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    StemmedDrawingInterface *GetStemmedDrawingInterface() override { return vrv_cast<StemmedDrawingInterface *>(this); }
    const StemmedDrawingInterface *GetStemmedDrawingInterface() const override
    {
        return vrv_cast<const StemmedDrawingInterface *>(this);
    }
    ///@}

    /** Override the method since alignment is required */
    bool HasToBeAligned() const override { return true; }

    /**
     * Add an element to a element.
     */
    bool IsSupportedChild(ClassId classId) override;

    /**
     * Overwritten method for note
     */
    void AddChild(Object *object) override;

    /**
     * Get the stem up / stem down attachment point.
     * If necessary look at the glyph anchor (if any).
     */
    ///@{
    Point GetStemUpSE(const Doc *doc, int staffSize, bool isCueSize) const override;
    Point GetStemDownNW(const Doc *doc, int staffSize, bool isCueSize) const override;
    int CalcStemLenInThirdUnits(const Staff *staff, data_STEMDIRECTION stemDir) const override;
    ///@}

    /**
     * Adjust the drawing y relative value for TabDurSym placed below the staff.
     * The value is originally set in LayerElement::SetAlignmentPosPitch.
     * However, setting it below the staff for a TabDurSym can be done only once the stem
     * direction has been determined. This is why we have this method called from
     * Beam::CalcTabBeamPlace
     */
    void AdjustDrawingYRel(const Staff *staff, const Doc *doc);

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

#endif
