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
class MeterSig : public LayerElement, public AttEnclosingChars, public AttMeterSigLog, public AttMeterSigVis {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes.
     */
    ///@{
    MeterSig();
    virtual ~MeterSig();
    Object *Clone() const override { return new MeterSig(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "MeterSig"; }
    ///@}

    /** Override the method since alignment is required */
    bool HasToBeAligned() const override { return true; }

    /** Override the method since check is required */
    bool IsScoreDefElement() const override { return (this->GetParent() && this->GetFirstAncestor(SCOREDEF)); }

    /** Evaluate additive meter counts */
    int GetTotalCount() const;

    /** Retrieves the symbol glyph */
    char32_t GetSymbolGlyph() const;

    /** Retrieve parentheses from the enclose attribute */
    std::pair<char32_t, char32_t> GetEnclosingGlyphs(bool smallGlpyh) const;

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::LayerCountInTimeSpan
     */
    int LayerCountInTimeSpan(FunctorParams *functorParams) const override;

private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif
