/////////////////////////////////////////////////////////////////////////////
// Name:        fig.h
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FIG_H__
#define __VRV_FIG_H__

#include "areaposinterface.h"
#include "textelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Fig (figure)
//----------------------------------------------------------------------------

/**
 * This class models the MEI <fig> element.
 */
class Fig : public TextElement, public AreaPosInterface {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    Fig();
    virtual ~Fig();
    virtual Object *Clone() const { return new Fig(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Fig"; }
    virtual ClassId GetClassId() const { return FIG; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual AreaPosInterface *GetAreaPosInterface() { return dynamic_cast<AreaPosInterface *>(this); }
    ///@}

    /**
     * Add an element (svg) to an fig.
     * Only supported elements will be actually added to the child list.
     */
    virtual bool IsSupportedChild(Object *object);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::AlignVertically
     */
    ///@{
    virtual int AlignVertically(FunctorParams *functorParams);
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
