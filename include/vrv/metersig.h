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
    enum class MeterCountSign { None, Slash, Minus, Asterisk, Plus };
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
    int GetTotalCount();

    /**
     * Set/get methods to get operation signa and meter counts as separate values
     */
    ///@{
    std::pair<std::vector<int>, MeterCountSign> GetMeterCounts();
    void SetMeterCounts(const std::vector<int> &counts, MeterSig::MeterCountSign sign);
    ///@}

    /** Retrieves the symbol glyph */
    wchar_t GetSymbolGlyph() const;

    /** Retrieve parentheses from the enclose attribute */
    std::pair<wchar_t, wchar_t> GetEnclosingGlyphs(bool smallGlpyh) const;

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::LayerCountInTimeSpan
     */
    int LayerCountInTimeSpan(FunctorParams *functorParams) override;

private:
    // Helper function to split meter counts and signs within MeterSig
    void SplitMeterCounts();

public:
    //
private:
    std::vector<int> m_meterCount;
    MeterCountSign m_meterCountSign;
};

} // namespace vrv

#endif
