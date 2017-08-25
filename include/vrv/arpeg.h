/////////////////////////////////////////////////////////////////////////////
// Name:        arpeg.h
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ARPEG_H__
#define __VRV_ARPEG_H__

#include "atts_cmn.h"
#include "atts_visual.h"
#include "controlelement.h"
#include "plistinterface.h"
#include "timeinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Arpeg
//----------------------------------------------------------------------------

class Arpeg : public ControlElement,
              public PlistInterface,
              public TimePointInterface,
              public AttArpegLog,
              public AttArpegVis,
              public AttColor {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Arpeg();
    virtual ~Arpeg();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Arpeg"; }
    virtual ClassId GetClassId() const { return ARPEG; }
    ///@}
                  
    /**
     * @name Get the X drawing position (relative to the top note)
     */
    ///@{
    virtual int GetDrawingX() const;
    ///@}
                  
    Note* GetDrawingTopNote();
    Note *GetDrawingBottomNote();
    void GetDrawingTopBottomNotes(Note *& top, Note *& bottom);

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual PlistInterface *GetPlistInterface() { return dynamic_cast<PlistInterface *>(this); }
    virtual TimePointInterface *GetTimePointInterface() { return dynamic_cast<TimePointInterface *>(this); }
    ////@}
                  
                  
    /**
     * @name Get and set the X drawing relative position
     */
    ///@{
    int GetDrawingXRel() const { return m_drawingXRel; }
    virtual void SetDrawingXRel(int drawingXRel);
    ///@}

    /**
     * Custom method for @plist validation
     */
    virtual bool IsValidRef(Object *ref);

    //----------//
    // Functors //
    //----------//
                  
    /**
     * See Object::ResetHorizontalAlignment
     */
    virtual int ResetHorizontalAlignment(FunctorParams *functorParams);
     
    /**
     * See Object::AdjustArpeg
     */
    ///@{
    virtual int AdjustArpeg(FunctorParams *functorParams);
    ///@}
                  

protected:
    //
private:
    //
public:
    //
private:
    /**
     * The X drawing relative position of the object.
     * Arpeg are positionned according to the top note through the FloatingPositioner
     * (See View::DrawArpeg that sets the FloatingPositioner)
     */
    int m_drawingXRel;
};

} // namespace vrv

#endif
