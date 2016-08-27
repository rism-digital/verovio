/////////////////////////////////////////////////////////////////////////////
// Name:        dynam.h
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_DYNAM_H__
#define __VRV_DYNAM_H__

#include "measureelement.h"
#include "textdirinterface.h"
#include "timeinterface.h"

namespace vrv {

class TextElement;

//----------------------------------------------------------------------------
// Dynam
//----------------------------------------------------------------------------

class Dynam : public MeasureElement, public TextListInterface, public TextDirInterface, public TimeSpanningInterface {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Dynam();
    virtual ~Dynam();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Dynam"; }
    virtual ClassId Is() const { return DYNAM; }
    ///@}

    virtual TextDirInterface *GetTextDirInterface() { return dynamic_cast<TextDirInterface *>(this); }
    virtual TimePointInterface *GetTimePointInterface() { return dynamic_cast<TimePointInterface *>(this); }
    virtual TimeSpanningInterface *GetTimeSpanningInterface() { return dynamic_cast<TimeSpanningInterface *>(this); }

    /**
    * Add an element (text, rend. etc.) to a dynam.
    * Only supported elements will be actually added to the child list.
    */
    virtual void AddChild(Object *object);

    /**
     * Return true if the dynam text is only composed of f, p, r, z, etc. letters (e.g. sfz)
     */
    bool IsSymbolOnly();

    std::wstring GetSymbolStr() const;

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
    /** A cached version of the symbol str instanciated by IsSymbolOnly() */
    std::wstring m_symbolStr;
};

} // namespace vrv

#endif
