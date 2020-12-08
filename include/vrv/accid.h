/////////////////////////////////////////////////////////////////////////////
// Name:        accid.h
// Author:      Laurent Pugin
// Created:     201X
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ACCID_H__
#define __VRV_ACCID_H__

#include "atts_gestural.h"
#include "layerelement.h"
#include "positioninterface.h"

namespace vrv {

class AlignmentReference;

//----------------------------------------------------------------------------
// Accid
//----------------------------------------------------------------------------

/**
 * This class models the MEI <accid> element.
 */
class Accid : public LayerElement,
              public PositionInterface,
              public AttAccidental,
              public AttAccidentalGestural,
              public AttAccidLog,
              public AttColor,
              public AttEnclosingChars {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Accid();
    virtual ~Accid();
    virtual Object *Clone() const { return new Accid(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Accid"; }
    virtual ClassId GetClassId() const { return ACCID; }
    ///@}

    virtual PositionInterface *GetPositionInterface() { return dynamic_cast<PositionInterface *>(this); }

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

    /**
     * @name Set and get drawing octave flag
     */
    ///@{
    void SetDrawingOctave(bool isDrawingOctave) { m_isDrawingOctave = isDrawingOctave; }
    bool GetDrawingOctave() const { return m_isDrawingOctave; }
    void SetDrawingOctaveAccid(Accid *drawingOctave) { m_drawingOctave = drawingOctave; }
    Accid *GetDrawingOctaveAccid() const { return m_drawingOctave; }
    ///@}

    /**
     * Retrieve SMuFL string for the accidental.
     * This will include brackets
     */
    std::wstring GetSymbolStr() const;

    bool AdjustX(LayerElement *element, Doc *doc, int staffSize, std::vector<Accid *> &leftAccids);

    /**
     * @name Method used for drawing accidentals on ornaments
     */
    static wchar_t GetAccidGlyph(data_ACCIDENTAL_WRITTEN);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

    /**
     * See Object::ResetHorizontalAlignment
     */
    virtual int ResetHorizontalAlignment(FunctorParams *functorParams);

private:
    //
public:
    //
private:
    Accid *m_drawingOctave;
    bool m_isDrawingOctave;
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
        if (first->GetDrawingY() < second->GetDrawingY()) {
            return true;
        }
        else if (first->GetDrawingY() > second->GetDrawingY()) {
            return false;
        }
        else {
            // with unissons, natural should always be the last accidental (assuming there is a natural)
            if ((first->GetAccid() == ACCIDENTAL_WRITTEN_n) || (second->GetAccid() == ACCIDENTAL_WRITTEN_n)) {
                return (first->GetAccid() != ACCIDENTAL_WRITTEN_n);
            }
            return first->GetDrawingY() < second->GetDrawingY();
        }
    }
};

} // namespace vrv

#endif
