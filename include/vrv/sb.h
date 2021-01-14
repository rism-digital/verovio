/////////////////////////////////////////////////////////////////////////////
// Name:        sb.h
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SB_H__
#define __VRV_SB_H__

#include "atts_shared.h"
#include "systemelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Sb
//----------------------------------------------------------------------------

/**
 * This class represents a MEI sb in score-based MEI.
 * In page-based MEI, it remains as it is. Actual systems are represented by System objects.
 */
class Sb : public SystemElement, public AttNNumberLike {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Sb();
    virtual ~Sb();
    virtual Object *Clone() const { return new Sb(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Sb"; }
    virtual ClassId GetClassId() const { return SB; }
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::CastOffEncoding
     */
    virtual int CastOffEncoding(FunctorParams *functorParams);

private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif
