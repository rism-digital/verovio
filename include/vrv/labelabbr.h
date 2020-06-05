/////////////////////////////////////////////////////////////////////////////
// Name:        labelabbr.h
// Author:      Klaus Rettinghaus
// Created:     07/03/2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_LABELABBR_H__
#define __VRV_LABELABBR_H__

#include "object.h"

namespace vrv {

//----------------------------------------------------------------------------
// LabelAbbr
//----------------------------------------------------------------------------

/**
 * This class models the MEI <labelAbbr> element.
 */
class LabelAbbr : public Object, public TextListInterface {

public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    LabelAbbr();
    virtual ~LabelAbbr();
    virtual Object *Clone() const { return new LabelAbbr(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "LabelAbbr"; }
    virtual ClassId GetClassId() const { return LABELABBR; }
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    virtual bool IsSupportedChild(Object *object);
    ///@}

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
