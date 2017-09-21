/////////////////////////////////////////////////////////////////////////////
// Name:        runningelement.h
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_RUNNING_ELEMENT_H__
#define __VRV_RUNNING_ELEMENT_H__

#include "atts_shared.h"
#include "object.h"

namespace vrv {

class Page;
class StaffAlignment;
    
//----------------------------------------------------------------------------
// RunningElement
//----------------------------------------------------------------------------

/**
 * This class represents running elements (headers and footers).
 * It is not an abstract class but should not be instanciated directly.
 */
class RunningElement : public Object, public AttTyped {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method resets all attribute classes
     */
    ///@{
    RunningElement();
    RunningElement(std::string classid);
    virtual ~RunningElement();
    virtual void Reset();
    virtual ClassId GetClassId() const { return RUNNING_ELEMENT; }
    ///@}
    
    /**
     * Disable cloning of the running elements (for now?).
     * It does not make sense you carry copying the running element acrosse the systems.
     */
    virtual Object *Clone() const { return NULL; }
    
    /*
     * @name Setter and getter for the current drawing page
     */
    ///@{
    void SetDrawingPage(Page *page) { m_drawingPage = page; }
    Page *GetDrawingPage() { return m_drawingPage; }
    ///@}
    
    /*
     * @name Setter and getter for the current staff alignment
     */
    ///@{
    void SetStaffAlignment(StaffAlignment *staffAlignment) { m_staffAlignment = staffAlignment; }
    StaffAlignment *GetStaffAlignment() { return m_staffAlignment; }
    ///@}

    //----------//
    // Functors //
    //----------//

private:
    //
public:
    //
private:
    /**
     * The page we are drawing
     */
    Page *m_drawingPage;
    
    /**
     * The staff alignment we are darwing
     */
    StaffAlignment *m_staffAlignment;
};

} // namespace vrv

#endif
