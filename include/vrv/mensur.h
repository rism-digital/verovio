/////////////////////////////////////////////////////////////////////////////
// Name:        mensur.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_MENSUR_H__
#define __VRV_MENSUR_H__

#include "atts_mensural.h"
#include "atts_shared.h"
#include "atts_visual.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Mensur
//----------------------------------------------------------------------------

class ScoreDefInterface;

/**
 * This class models the MEI <mensur> element.
 */
class Mensur : public LayerElement,
               public AttColor,
               public AttCue,
               public AttDurationRatio,
               public AttMensuralShared,
               public AttMensurLog,
               public AttMensurVis,
               public AttSlashCount,
               public AttStaffLoc {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes.
     */
    ///@{
    Mensur();
    Mensur(const ScoreDefInterface *mensurAttr);
    void Init();
    virtual ~Mensur();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Mensur"; }
    virtual ClassId GetClassId() const { return MENSUR; }
    virtual Object *Clone() const { return new Mensur(*this); }
    ///@}

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::FindSpaceInReferenceAlignments
     */
    virtual int FindSpaceInReferenceAlignments(FunctorParams *functorParams);

private:
    //
public:
    /**
     * Static member for setting a value from a controller.
     * Used for example in SetValue
     */
    static int s_num;
    /**
     * Static member for setting a value from a controller.
     * Used for examle in SetValue.
     */
    static int s_numBase;

private:
};

} // namespace vrv

#endif
