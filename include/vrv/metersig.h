/////////////////////////////////////////////////////////////////////////////
// Name:        metersig.h
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_METERSIG_H__
#define __VRV_METERSIG_H__

#include "atts_externalsymbols.h"
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
class MeterSig : public LayerElement,
                 public AttColor,
                 public AttEnclosingChars,
                 public AttExtSymNames,
                 public AttMeterSigLog,
                 public AttMeterSigVis,
                 public AttTypography,
                 public AttVisibility {
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
    std::string GetClassName() const override { return "meterSig"; }
    ///@}

    /** Override the method since alignment is required */
    bool HasToBeAligned() const override { return true; }

    /** Override the method since check is required */
    bool IsScoreDefElement() const override { return (this->GetParent() && this->GetFirstAncestor(SCOREDEF)); }

    /** Evaluate additive meter counts */
    int GetTotalCount() const;

    /** Return the implicit unit according to the sym (if any, return 0 otherwise) */
    int GetSymImplicitUnit() const;

    /**
     * Return the unit (int) as data_DURATION (up to 32).
     * Return DURATION_4 if no match.
     */
    data_DURATION GetUnitAsDur() const;

    /** Retrieves the symbol glyph */
    char32_t GetSymbolGlyph() const;

    /** Retrieve parentheses from the enclose attribute */
    std::pair<char32_t, char32_t> GetEnclosingGlyphs(bool smallGlpyh) const;

    //----------//
    // Functors //
    //----------//

    /**
     * Interface for class functor visitation
     */
    ///@{
    FunctorCode Accept(Functor &functor) override;
    FunctorCode Accept(ConstFunctor &functor) const override;
    FunctorCode AcceptEnd(Functor &functor) override;
    FunctorCode AcceptEnd(ConstFunctor &functor) const override;
    ///@}

private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif
