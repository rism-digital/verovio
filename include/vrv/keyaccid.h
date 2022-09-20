/////////////////////////////////////////////////////////////////////////////
// Name:        keyaccid.h
// Author:      Laurent Pugin
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_KEYACCID_H__
#define __VRV_KEYACCID_H__

#include "atts_externalsymbols.h"
#include "atts_gestural.h"
#include "layerelement.h"
#include "pitchinterface.h"
#include "positioninterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// KeyAccid
//----------------------------------------------------------------------------

/**
 * This class models the MEI <keyAccid> element.
 */
class KeyAccid : public LayerElement,
                 public PitchInterface,
                 public PositionInterface,
                 public AttAccidental,
                 public AttColor,
                 public AttEnclosingChars,
                 public AttExtSym {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    KeyAccid();
    virtual ~KeyAccid();
    Object *Clone() const override { return new KeyAccid(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "KeyAccid"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    PitchInterface *GetPitchInterface() override { return vrv_cast<PitchInterface *>(this); }
    const PitchInterface *GetPitchInterface() const override { return vrv_cast<const PitchInterface *>(this); }
    PositionInterface *GetPositionInterface() override { return vrv_cast<PositionInterface *>(this); }
    const PositionInterface *GetPositionInterface() const override { return vrv_cast<const PositionInterface *>(this); }
    ///@}

    /**
     * Retrieve SMuFL string for the accidental.
     * This will include brackets
     */
    std::u32string GetSymbolStr(data_NOTATIONTYPE notationType) const;

    /**
     * Determine the staff location
     */
    int CalcStaffLoc(Clef *clef, int clefLocOffset) const;

    //----------//
    // Functors //
    //----------//

private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif
