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
    Object *Clone() const override { return new Dir(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "Dir"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    TextDirInterface *GetTextDirInterface() override { return dynamic_cast<TextDirInterface *>(this); }
    TimePointInterface *GetTimePointInterface() override { return dynamic_cast<TimePointInterface *>(this); }
    TimeSpanningInterface *GetTimeSpanningInterface() override { return dynamic_cast<TimeSpanningInterface *>(this); }
    ///@}

    /**
     * Add an element (text, rend. etc.) to a dir.
     * Only supported elements will be actually added to the child list.
     */
    bool IsSupportedChild(Object *object) override;

    /**
     * See FloatingObject::IsExtenderElement
     */
    virtual bool IsExtenderElement() const { return GetExtender() == BOOLEAN_true; }

    /**
     * Check whether one of the children has hAlign attribute set to `alignment` value
     */
    bool AreChildrenAlignedTo(data_HORIZONTALALIGNMENT alignment) const;

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::PrepareFloatingGrps
     */
    int PrepareFloatingGrps(FunctorParams *functorParams) override;

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
