/////////////////////////////////////////////////////////////////////////////
// Name:        grpsym.h
// Author:      Andriy Makarchuk
// Created:     2020
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_GRPSYM_H__
#define __VRV_GRPSYM_H__

#include "atts_mei.h"
#include "atts_shared.h"
#include "object.h"
#include "staffdef.h"

namespace vrv {

//----------------------------------------------------------------------------
// GrpSym
//----------------------------------------------------------------------------

/**
 * This class represents a MEI grpSym.
 */
class GrpSym : public Object,
               public AttColor,
               public AttGrpSymLog,
               public AttStaffGroupingSym,
               public AttStartId,
               public AttStartEndId {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    GrpSym();
    virtual ~GrpSym();
    Object *Clone() const override { return new GrpSym(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "grpSym"; }
    ///@}

    /**
     * @name Setter and getter for startDef/endDef
     */
    ///@{
    void SetStartDef(StaffDef *start);
    StaffDef *GetStartDef() { return m_startDef; }
    const StaffDef *GetStartDef() const { return m_startDef; }
    void SetEndDef(StaffDef *end);
    StaffDef *GetEndDef() { return m_endDef; }
    const StaffDef *GetEndDef() const { return m_endDef; }
    ///@}

    /**
     * @name Get the X and Y drawing position
     */
    ///@{
    int GetDrawingX() const override;
    int GetDrawingY() const override;
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
    //
    StaffDef *m_startDef;
    StaffDef *m_endDef;
};

} // namespace vrv

#endif // __VRV_GRPSYM_H__
