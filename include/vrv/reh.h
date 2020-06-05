/////////////////////////////////////////////////////////////////////////////
// Name:        reh.h
// Author:      Klaus Rettinghaus
// Created:     2020
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_REH_H__
#define __VRV_REH_H__

#include "controlelement.h"
#include "textdirinterface.h"
#include "timeinterface.h"

namespace vrv {

class TextElement;

//----------------------------------------------------------------------------
// Reh (rehearsal mark)
//----------------------------------------------------------------------------

/**
 * This class models the MEI <reh> element.
 */
class Reh : public ControlElement,
            public TextDirInterface,
            public TimePointInterface,
            public AttColor,
            public AttLang,
            public AttVerticalGroup {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Reh();
    virtual ~Reh();
    virtual Object *Clone() const { return new Reh(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Reh"; }
    virtual ClassId GetClassId() const { return REH; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual TextDirInterface *GetTextDirInterface() { return dynamic_cast<TextDirInterface *>(this); }
    virtual TimePointInterface *GetTimePointInterface() { return dynamic_cast<TimePointInterface *>(this); }
    ///@}

    /**
     * Add an element (text, rend. etc.) to a reh.
     * Only supported elements will be actually added to the child list.
     */
    virtual bool IsSupportedChild(Object *object);

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
