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
    virtual Object *Clone() const { return new Fermata(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Fermata"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual TimePointInterface *GetTimePointInterface() { return dynamic_cast<TimePointInterface *>(this); }
    ///@}

    /**
     * Helpler for converting markup (from Note, Chord, Rest, MRest)
     */
    void ConvertFromAnalyticalMarkup(
        AttFermataPresent *fermataPresent, const std::string &uuid, ConvertMarkupAnalyticalParams *params);

    /**
     * Get the SMuFL glyph for the fermata based on type, shape or glyph.num
     */
    wchar_t GetFermataGlyph() const;

    /**
     * Retrieves parentheses / brackets from the enclose attribute
     */
    wchar_t GetEnclosingGlyph(bool beforeFermata) const;

    //----------------//
    // Static methods //
    //----------------//

    /**
     * Retrieves the vertical alignment for the fermata SMuFL code
     */
    static data_VERTICALALIGNMENT GetVerticalAlignment(wchar_t code);

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
