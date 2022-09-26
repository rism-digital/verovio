/////////////////////////////////////////////////////////////////////////////
// Name:        fermata.h
// Author:      Klaus Rettinghaus
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FERMATA_H__
#define __VRV_FERMATA_H__

#include "atts_cmn.h"
#include "atts_externalsymbols.h"
#include "atts_visual.h"
#include "controlelement.h"
#include "timeinterface.h"

namespace vrv {

class ConvertMarkupAnalyticalParams;

//----------------------------------------------------------------------------
// Fermata
//----------------------------------------------------------------------------

/**
 * This class models the MEI <fermata> element.
 */
class Fermata : public ControlElement,
                public TimePointInterface,
                public AttColor,
                public AttEnclosingChars,
                public AttExtSym,
                public AttFermataVis,
                public AttPlacementRelStaff {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Fermata();
    virtual ~Fermata();
    Object *Clone() const override { return new Fermata(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "Fermata"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    TimePointInterface *GetTimePointInterface() override { return vrv_cast<TimePointInterface *>(this); }
    const TimePointInterface *GetTimePointInterface() const override
    {
        return vrv_cast<const TimePointInterface *>(this);
    }
    ///@}

    /**
     * Helper for converting markup (from Note, Chord, Rest, MRest)
     */
    void ConvertFromAnalyticalMarkup(
        AttFermataPresent *fermataPresent, const std::string &id, ConvertMarkupAnalyticalParams *params);

    /**
     * Get the SMuFL glyph for the fermata based on type, shape or glyph.num
     */
    char32_t GetFermataGlyph() const;

    /**
     * Retrieve parentheses / brackets from the enclose attribute
     */
    std::pair<char32_t, char32_t> GetEnclosingGlyphs() const;

    //----------------//
    // Static methods //
    //----------------//

    /**
     * Retrieves the vertical alignment for the fermata SMuFL code
     */
    static data_VERTICALALIGNMENT GetVerticalAlignment(char32_t code);

    //----------//
    // Functors //
    //----------//

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
