/////////////////////////////////////////////////////////////////////////////
// Name:        metersig.h
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_METERSIG_H__
#define __VRV_METERSIG_H__

#include "atts_shared.h"
#include "atts_visual.h"
#include "layerelement.h"

namespace vrv {

class ScoreDefInterface;

//----------------------------------------------------------------------------
// MeterSig
//----------------------------------------------------------------------------

/**
 * This class models the MEI <meterSig> element.
 */
class MeterSig : public LayerElement, public AttMeterSigLog, public AttMeterSigVis {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes.
     */
    ///@{
    MeterSig();
    virtual ~MeterSig();
    virtual Object *Clone() const { return new MeterSig(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "MeterSig"; }
    virtual ClassId GetClassId() const { return METERSIG; }
    ///@}

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

    /** Override the method since check is required */
    virtual bool IsScoreDefElement() const { return (this->GetParent() && this->GetFirstAncestor(SCOREDEF)); }

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::LayerCountInTimeSpan
     */
    virtual int LayerCountInTimeSpan(FunctorParams *functorParams);

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
