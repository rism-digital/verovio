/////////////////////////////////////////////////////////////////////////////
// Name:        dir.h
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_DIR_H__
#define __VRV_DIR_H__

#include "floatingelement.h"
#include "textdirinterface.h"
#include "timeinterface.h"

namespace vrv {

class TextElement;

//----------------------------------------------------------------------------
// Dir
//----------------------------------------------------------------------------

class Dir : public FloatingElement, public TextListInterface, public TextDirInterface, public TimeSpanningInterface {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Dir();
    virtual ~Dir();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Dir"; };
    virtual ClassId Is() const { return DIR; };
    ///@}

    virtual TextDirInterface *GetTextDirInterface() { return dynamic_cast<TextDirInterface *>(this); }
    virtual TimePointInterface *GetTimePointInterface() { return dynamic_cast<TimePointInterface *>(this); }
    virtual TimeSpanningInterface *GetTimeSpanningInterface() { return dynamic_cast<TimeSpanningInterface *>(this); }

    /**
    * Add an element (text, rend. etc.) to a dynam.
    * Only supported elements will be actually added to the child list.
    */
    void AddTextElement(TextElement *element);

    //----------//
    // Functors //
    //----------//

    /**
     * Align the content of a staff vertically.
     * See Object::AlignVertically
     */
    virtual int AlignVertically(ArrayPtrVoid *params);

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
