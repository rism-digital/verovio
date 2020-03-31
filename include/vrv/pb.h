/////////////////////////////////////////////////////////////////////////////
// Name:        pb.h
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_PB_H__
#define __VRV_PB_H__

#include "atts_shared.h"
#include "systemelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Pb
//----------------------------------------------------------------------------

/**
 * This class represents a MEI pb in score-based MEI.
 * In page-based MEI, it remains as is as. Actuall pages are represented by Page objects.
 */
class Pb : public SystemElement, public AttNNumberLike {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Pb();
    virtual ~Pb();
    virtual Object *Clone() const { return new Pb(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Pb"; }
    virtual ClassId GetClassId() const { return PB; }
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
