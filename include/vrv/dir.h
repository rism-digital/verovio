/////////////////////////////////////////////////////////////////////////////
// Name:        dir.h
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_DIR_H__
#define __VRV_DIR_H__

#include "controlelement.h"
#include "textdirinterface.h"
#include "timeinterface.h"

namespace vrv {

class TextElement;

//----------------------------------------------------------------------------
// Dir (directive)
//----------------------------------------------------------------------------

/**
 * This class models the MEI <dir> element.
 */
class Dir : public ControlElement,
            public TextListInterface,
            public TextDirInterface,
            public TimeSpanningInterface,
            public AttExtender,
            public AttLang,
            public AttLineRendBase,
            public AttVerticalGroup {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Dir();
    virtual ~Dir();
    virtual Object *Clone() const { return new Dir(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Dir"; }
    virtual ClassId GetClassId() const { return DIR; }
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
     * Add an element (text, rend. etc.) to a dir.
     * Only supported elements will be actually added to the child list.
     */
    virtual bool IsSupportedChild(Object *object);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::PrepareFloatingGrps
     */
    virtual int PrepareFloatingGrps(FunctorParams *);

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
