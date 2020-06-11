/////////////////////////////////////////////////////////////////////////////
// Name:        gracegrp.h
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_GRACEGRP_H__
#define __VRV_GRACEGRP_H__

#include "atts_cmn.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// GraceGrp
//----------------------------------------------------------------------------

class GraceGrp : public LayerElement, public AttColor, public AttGraced, public AttGraceGrpLog {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    GraceGrp();
    virtual ~GraceGrp();
    virtual Object *Clone() const { return new GraceGrp(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "GraceGrp"; };
    virtual ClassId GetClassId() const { return GRACEGRP; };
    ///@}

    /**
     * Add childElement to a element.
     */
    virtual bool IsSupportedChild(Object *childElement);

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
