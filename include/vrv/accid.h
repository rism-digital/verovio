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

class AlignmentReference;

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
    std::wstring GetSymbolStr() const;

    bool AdjustX(LayerElement *element, Doc *doc, int staffSize, std::vector<Accid *> &leftAccids);

    /**
     * @name Method used for drawing accidentals on ornaments
     */
    static wchar_t GetAccidGlyph(data_ACCIDENTAL_EXPLICIT);

    //----------//
    // Functors //
    //----------//

private:
    //
public:
    //
private:
};

//----------------------------------------------------------------------------
// AccidSpaceSort
//----------------------------------------------------------------------------

/**
 * Sort Object by drawing Y value or by layerN
 */
class AccidSpaceSort {

public:
    AccidSpaceSort() {}

    bool operator()(const Accid *first, const Accid *second) const
    {
        if (first->GetDrawingY() < second->GetDrawingY())
            return true;
        else if (first->GetDrawingY() > second->GetDrawingY())
            return false;
        // with unissons, look at the layer @n
        else
            return (first->GetAlignmentLayerN() < second->GetAlignmentLayerN());
    }
};

} // namespace vrv

#endif
