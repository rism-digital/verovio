/////////////////////////////////////////////////////////////////////////////
// Name:        custos.h
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CUSTOS_H__
#define __VRV_CUSTOS_H__

#include "accid.h"
#include "atts_analytical.h"
#include "atts_shared.h"
#include "layerelement.h"
#include "pitchinterface.h"
#include "positioninterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Custos
//----------------------------------------------------------------------------

class Custos : public LayerElement, public PitchInterface, public PositionInterface, public AttColor, public AttExtSym {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Custos();
    virtual ~Custos();
    Object *Clone() const override { return new Custos(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "Custos"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    PitchInterface *GetPitchInterface() override { return vrv_cast<PitchInterface *>(this); }
    const PitchInterface *GetPitchInterface() const override { return vrv_cast<const PitchInterface *>(this); }
    ///@}

    /** Override the method since alignment is required */
    bool HasToBeAligned() const override { return true; }

    /**
     * Add an accid to a custos.
     */
    bool IsSupportedChild(Object *object) override;

    /**
     * Return a SMuFL code for the custos
     */
    char32_t GetCustosGlyph(const data_NOTATIONTYPE notationType) const;

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ResetData
     */
    int ResetData(FunctorParams *functorParams) override;

    /**
     * See Object::ResetHorizontalAlignment
     */
    int ResetHorizontalAlignment(FunctorParams *functorParams) override;

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
