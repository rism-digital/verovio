/////////////////////////////////////////////////////////////////////////////
// Name:        pages.h
// Author:      Laurent Pugin
// Created:     2018/02/15
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_PAGES_H__
#define __VRV_PAGES_H__

#include "atts_shared.h"
#include "object.h"

namespace vrv {

class Score;

//----------------------------------------------------------------------------
// Pages
//----------------------------------------------------------------------------

/**
 * This class represent a <pages> in page-based MEI.
 */
class Pages : public Object, public AttLabelled, public AttNNumberLike {

public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Pages();
    virtual ~Pages();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Pages"; }
    virtual ClassId GetClassId() const { return PAGES; }
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    virtual bool IsSupportedChild(Object *object);
    ///@}

    /**
     *
     */
    void ConvertFrom(Score *score);

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
