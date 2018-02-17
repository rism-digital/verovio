/////////////////////////////////////////////////////////////////////////////
// Name:        mnum.h
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_MNUM_H__
#define __VRV_MNUM_H__

#include "controlelement.h"
#include "textdirinterface.h"
#include "timeinterface.h"

namespace vrv {

class TextElement;

//----------------------------------------------------------------------------
// MNum
//----------------------------------------------------------------------------

class MNum : public ControlElement,
            public TextListInterface,
            public TextDirInterface,
            public TimeSpanningInterface,
            public AttColor,
            public AttLang {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    MNum();
    virtual ~MNum();
    virtual void Reset();
    virtual std::string GetClassName() const { return "MNum"; }
    virtual ClassId GetClassId() const { return MNUM; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual TextDirInterface *GetTextDirInterface() { return dynamic_cast<TextDirInterface *>(this); }
    virtual TimePointInterface *GetTimePointInterface() { return dynamic_cast<TimePointInterface *>(this); }
    virtual TimeSpanningInterface *GetTimeSpanningInterface() { return dynamic_cast<TimeSpanningInterface *>(this); }
    ///@}

    /**
     * Add an element (text, rend. etc.) to a dynam.
     * Only supported elements will be actually added to the child list.
     */
    virtual void AddChild(Object *object);

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
