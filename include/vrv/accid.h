/////////////////////////////////////////////////////////////////////////////
// Name:        accid.h
// Author:      Laurent Pugin
// Created:     201X
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ACCID_H__
#define __VRV_ACCID_H__

#include "atts_externalsymbols.h"
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
              public AttAccidentalGes,
              public AttAccidLog,
              public AttColor,
              public AttEnclosingChars,
              public AttExtSymAuth,
              public AttExtSymNames,
              public AttPlacementOnStaff,
              public AttPlacementRelEvent {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Accid();
    virtual ~Accid();
    Object *Clone() const override { return new Accid(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "accid"; }
    ///@}

    /** Override the method since it is align to the staff */
    bool IsRelativeToStaff() const override { return (this->HasLoc() || (this->HasOloc() && this->HasPloc())); }

    /**
     * @name Getter to interfaces
     */
    ///@{
    PositionInterface *GetPositionInterface() override { return vrv_cast<PositionInterface *>(this); }
    const PositionInterface *GetPositionInterface() const override { return vrv_cast<const PositionInterface *>(this); }
    ///@}

    /** Override the method since alignment is required */
    bool HasToBeAligned() const override { return true; }

    /**
     * @name Set and get drawing unison accid
     */
    ///@{
    void SetDrawingUnisonAccid(Accid *drawingUnison) { m_drawingUnison = drawingUnison; }
    Accid *GetDrawingUnisonAccid() { return m_drawingUnison; }
    const Accid *GetDrawingUnisonAccid() const { return m_drawingUnison; }
    ///@}

    /**
     * Retrieve SMuFL string for the accidental.
     * This will include brackets
     */
    std::u32string GetSymbolStr(data_NOTATIONTYPE notationType) const;

    /**
     * Adjust X position of accid in relation to other element
     */
    void AdjustX(LayerElement *element, const Doc *doc, int staffSize, std::vector<Accid *> &leftAccids,
        std::set<Accid *> &adjustedAccids);

    /**
     * Adjust accid position if it's placed above/below staff so that it does not overlap with ledger lines
     */
    void AdjustToLedgerLines(const Doc *doc, LayerElement *element, int staffSize);

    /**
     * @name Set and get same layer alignment
     */
    ///@{
    void IsAlignedWithSameLayer(bool alignWithSameLayer) { m_alignedWithSameLayer = alignWithSameLayer; }
    bool IsAlignedWithSameLayer() const { return m_alignedWithSameLayer; }
    ///@}

    //----------------//
    // Static methods //
    //----------------//

    /**
     * @name Method used for drawing accidentals on ornaments
     */
    static char32_t GetAccidGlyph(data_ACCIDENTAL_WRITTEN accid);

    /**
     * Create the SMuFL string based on various properties
     */
    static std::u32string CreateSymbolStr(data_ACCIDENTAL_WRITTEN accid, data_ENCLOSURE enclosure = ENCLOSURE_NONE,
        data_NOTATIONTYPE notationType = NOTATIONTYPE_NONE, const Resources *resources = NULL, data_HEXNUM glyphNum = 0,
        std::string glyphName = "");

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

private:
    //
public:
    //
private:
    Accid *m_drawingUnison;
    bool m_alignedWithSameLayer;
};

//----------------------------------------------------------------------------
// AccidSpaceSort
//----------------------------------------------------------------------------

/**
 * Sort Object by drawing Y value
 */
class AccidSpaceSort {

public:
    AccidSpaceSort() = default;

    bool operator()(const Accid *first, const Accid *second) const
    {
        if (first->GetDrawingY() == second->GetDrawingY()) {
            // with unissons, natural should always be the last accidental
            return ((first->GetAccid() == ACCIDENTAL_WRITTEN_n) && (second->GetAccid() != ACCIDENTAL_WRITTEN_n));
        }
        else {
            return (first->GetDrawingY() > second->GetDrawingY());
        }
    }
};

//----------------------------------------------------------------------------
// AccidOctaveSort
//----------------------------------------------------------------------------

/**
 * Equivalence of accidentals that are an octave apart
 */
class AccidOctaveSort {

public:
    AccidOctaveSort() = default;

    // Encodes parent ID + accid type + pitch
    std::string GetOctaveID(const Accid *accid) const;

    bool operator()(const Accid *first, const Accid *second) const
    {
        return this->GetOctaveID(first) < this->GetOctaveID(second);
    }
};

} // namespace vrv

#endif
