/////////////////////////////////////////////////////////////////////////////
// Name:        anchoredtext.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ANCHORED_TEXT_H__
#define __VRV_ANCHORED_TEXT_H__

#include "controlelement.h"
#include "textdirinterface.h"

namespace vrv {

class TextElement;

//----------------------------------------------------------------------------
// AnchoredText
//----------------------------------------------------------------------------

/**
 * This class is an interface for <anchoredText> elements at the measure level
 */
class AnchoredText : public ControlElement, public TextDirInterface {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    AnchoredText();
    virtual ~AnchoredText();
    virtual Object *Clone() const { return new AnchoredText(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "AnchoredText"; }
    virtual ClassId GetClassId() const { return ANCHORED_TEXT; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual TextDirInterface *GetTextDirInterface() { return dynamic_cast<TextDirInterface *>(this); }
    ///@}

    /**
     * Add an element (text, rend. etc.) to a tempo.
     * Only supported elements will be actually added to the child list.
     */
    virtual bool IsSupportedChild(Object *object);

    //----------//
    // Functors //
    //----------//

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
