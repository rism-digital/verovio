/////////////////////////////////////////////////////////////////////////////
// Name:        accid.h
// Author:      Laurent Pugin
// Created:     201X
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ACCID_H__
#define __VRV_ACCID_H__

#include "layerelement.h"
#include "positioninterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Accid
//----------------------------------------------------------------------------

class Accid : public LayerElement,
              public PositionInterface,
              public AttAccidental,
              public AttAccidentalPerformed,
              public AttAccidLog,
              public AttColor,
              public AttEnclosingchars {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Accid();
    virtual ~Accid();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Accid"; }
    virtual ClassId GetClassId() const { return ACCID; }
    ///@}

    virtual PositionInterface *GetPositionInterface() { return dynamic_cast<PositionInterface *>(this); }

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

    /**
     * Retrieve SMuFL string for the accidental.
     * This will include brackets
     */
    std::wstring GetSmuflString(Doc *doc);

    //----------//
    // Functors //
    //----------//

private:
    //
public:
    /**
     * Indicates if is cue size for accid object created for @accid.
     * See Note::PreparePointersByLayer and View::DrawAccid
     */
    bool m_drawingCueSize;

private:
};

} // namespace vrv

#endif
