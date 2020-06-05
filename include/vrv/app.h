/////////////////////////////////////////////////////////////////////////////
// Name:        app.h
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_APP_H__
#define __VRV_APP_H__

#include "editorial.h"

namespace vrv {

//----------------------------------------------------------------------------
// App
//----------------------------------------------------------------------------

class App : public EditorialElement {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    App();
    App(EditorialLevel level);
    virtual ~App();
    virtual Object *Clone() const { return new App(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "App"; }
    virtual ClassId GetClassId() const { return APP; }
    ///@}

    /** Getter for level **/
    EditorialLevel GetLevel() { return m_level; }

    /**
     * Add children to a apparatus.
     */
    virtual bool IsSupportedChild(Object *object);

protected:
    /** We store the level of the <app> for integrity check */
    EditorialLevel m_level;

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
