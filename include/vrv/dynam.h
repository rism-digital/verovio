/////////////////////////////////////////////////////////////////////////////
// Name:        dynam.h
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_DYNAM_H__
#define __VRV_DYNAM_H__

#include "floatingelement.h"
#include "textdirinterface.h"
#include "timeinterface.h"

namespace vrv {

class TextElement;

//----------------------------------------------------------------------------
// Dynam
//----------------------------------------------------------------------------

class Dynam : public FloatingElement, public TextDirInterface, public TimeSpanningInterface {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Dynam();
    virtual ~Dynam();
    virtual void Reset();
    virtual std::string GetClassName() { return "Dynam"; };
    virtual ClassId Is() { return DYNAM; };
    ///@}

    virtual TextDirInterface *GetTextDirInterface() { return vrv_cast2(TextDirInterface *)(this); }
    virtual TimePointInterface *GetTimePointInterface() { return vrv_cast2(TimePointInterface *)(this); }
    virtual TimeSpanningInterface *GetTimeSpanningInterface() { return vrv_cast2(TimeSpanningInterface *)(this); }

    /**
    * Add an element (text, rend. etc.) to a dynam.
    * Only supported elements will be actually added to the child list.
    */
    void AddTextElement(TextElement *element);

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
