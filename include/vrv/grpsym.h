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
    virtual Object *Clone() const { return new GrpSym(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "GrpSym"; }
    virtual ClassId GetClassId() const { return GRPSYM; }
    ///@}

    /**
     * @name Setter and getter for startDef/endDef
     */
    ///@{
    void SetStartDef(StaffDef *start);
    StaffDef *GetStartDef() const { return m_startDef; };
    void SetEndDef(StaffDef *end);
    StaffDef *GetEndDef() const { return m_endDef; };
    ///@}

    /**
     * @name Get the X and Y drawing position
     */
    ///@{
    virtual int GetDrawingX() const;
    virtual int GetDrawingY() const;
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::PrepareGroupSymbols
     */
    virtual int PrepareGroupSymbols(FunctorParams *functorParams);

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
