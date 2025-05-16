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

/**
 * This class models the MEI <arpeg> element.
 */
class Arpeg : public ControlElement,
              public PlistInterface,
              public TimePointInterface,
              public AttArpegLog,
              public AttArpegVis,
              public AttEnclosingChars {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Arpeg();
    virtual ~Arpeg();
    Object *Clone() const override { return new Arpeg(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "arpeg"; }
    ///@}

    /**
     * @name Get the X drawing position (relative to the top note)
     */
    ///@{
    int GetDrawingX() const override;
    ///@}

    /**
     * Get all notes of the arpeggio
     */
    ///@{
    std::set<Note *> GetNotes();
    std::set<const Note *> GetNotes() const;
    ///@}

    /**
     * Set the top and bottom note of the arpeg.
     * Pointers will be NULL if resolving fails (e.g., pointing to one single note)
     */
    void GetDrawingTopBottomNotes(Note *&top, Note *&bottom);

    /**
     * Get cross staff of the front element if all elements of arpeggio are cross-staff
     */
    ///@{
    Staff *GetCrossStaff();
    const Staff *GetCrossStaff() const;
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    PlistInterface *GetPlistInterface() override { return vrv_cast<PlistInterface *>(this); }
    const PlistInterface *GetPlistInterface() const override { return vrv_cast<const PlistInterface *>(this); }
    TimePointInterface *GetTimePointInterface() override { return vrv_cast<TimePointInterface *>(this); }
    const TimePointInterface *GetTimePointInterface() const override
    {
        return vrv_cast<const TimePointInterface *>(this);
    }
    ////@}

    /**
     * @name Get and set the X drawing relative position
     */
    ///@{
    int GetDrawingXRel() const { return m_drawingXRel; }
    virtual void SetDrawingXRel(int drawingXRel);
    void CacheXRel(bool restore = false);
    ///@}

    /**
     * Custom method for @plist validation
     */
    bool IsValidRef(const Object *ref) const override;

    //----------//
    // Functors //
    //----------//

    /**
     * Interface for class functor visitation
     */
    ///@{
    FunctorCode Accept(Functor &functor) override;
    FunctorCode Accept(ConstFunctor &functor) const override;
    FunctorCode AcceptEnd(Functor &functor) override;
    FunctorCode AcceptEnd(ConstFunctor &functor) const override;
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
     * Arpeg are positioned according to the top note through the FloatingPositioner
     * (See View::DrawArpeg that sets the FloatingPositioner)
     */
    int m_drawingXRel;

    /**
     * The cached value for m_drawingXRel for caching horizontal layout
     */
    int m_cachedXRel;
};

} // namespace vrv

#endif
