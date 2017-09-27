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
class Staff;
    
//----------------------------------------------------------------------------
// RunningElement
//----------------------------------------------------------------------------

/**
 * This class represents running elements (headers and footers).
 * It is not an abstract class but should not be instanciated directly.
 */
class RunningElement : public Object, public AttHorizontalAlign, public AttTyped {
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
    
    /**
     * @name Get and set the X and Y drawing position
     */
    ///@{
    virtual int GetDrawingX() const;
    virtual int GetDrawingY() const;
    ///@}
    
    int GetWidth() const;
    
    /*
     * @name Setter and getter for the current drawing page
     */
    ///@{
    void SetDrawingPage(Page *page);
    Page *GetDrawingPage() { return m_drawingPage; }
    ///@}
    
    /*
     * @name Setter and getter for the current darwing staff
     */
    ///@{
    void SetDrawingStaff(Staff *staff);
    Staff *GetDrawingStaff() { return m_drawingStaff; }
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
     * The page we are drawing (for the x position)
     */
    Page *m_drawingPage;
    
    /**
     * The staff we are darwing (for the y postion)
     */
    Staff *m_drawingStaff;
};

} // namespace vrv

#endif
