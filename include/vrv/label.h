/////////////////////////////////////////////////////////////////////////////
// Name:        label.h
// Author:      Laurent Pugin
// Created:     19/06/2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_LABEL_H__
#define __VRV_LABEL_H__

#include "object.h"

namespace vrv {

//----------------------------------------------------------------------------
// Label
//----------------------------------------------------------------------------

/**
 * This class models the MEI <label> element.
 */
class Label : public Object, public TextListInterface {

public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Label();
    virtual ~Label();
    virtual Object *Clone() const { return new Label(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Label"; }
    virtual ClassId GetClassId() const { return LABEL; }
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
