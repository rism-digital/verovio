/////////////////////////////////////////////////////////////////////////////
// Name:        choice.h
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CHOICE_H__
#define __VRV_CHOICE_H__

#include "atts_shared.h"
#include "editorial.h"

namespace vrv {

//----------------------------------------------------------------------------
// Choice
//----------------------------------------------------------------------------

class Choice : public EditorialElement {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Choice();
    Choice(EditorialLevel level);
    virtual Object *Clone() const { return new Choice(*this); }
    virtual ~Choice();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Choice"; }
    virtual ClassId GetClassId() const { return CHOICE; }
    ///@}

    /** Getter for level **/
    EditorialLevel GetLevel() { return m_level; }

    /**
     * Add children to a apparatus.
     */
    virtual bool IsSupportedChild(Object *object);

protected:
    /** We store the level of the <choice> for integrity check */
    EditorialLevel m_level;

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
