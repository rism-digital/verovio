/////////////////////////////////////////////////////////////////////////////
// Name:        ossia.h
// Author:      Klaus Rettinghaus
// Created:     2025
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_OSSIA_H__
#define __VRV_OSSIA_H__

#include "atts_cmn.h"
#include "atts_shared.h"
#include "object.h"
#include "staffgrp.h"

namespace vrv {

class StaffDef;

//----------------------------------------------------------------------------
// Ossia
//----------------------------------------------------------------------------

/**
 * This class represents an ossia.
 */
class Ossia : public Object, public AttTyped {

public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Ossia();
    virtual ~Ossia();
    Object *Clone() const override { return new Ossia(*this); };
    void Reset() override;
    std::string GetClassName() const override { return "ossia"; }
    ///@}

    /**
     * Overriding CloneReset() method to be called after copy / assignment calls.
     */
    void CloneReset() override;

    /**
     * @name Setter, getter and re-setter of the drawing staffDefs / staffGrp
     */
    ///@{
    void AddDrawingStaffDef(StaffDef *drawingStaffDef);
    void ResetDrawingStaffGrp();
    StaffGrp *GetDrawingStaffGrp() { return &m_drawingStaffGrp; }
    ///@}

    /**
     * @name Methods based on type to be replaced by MEI attributes if possible
     */
    ///@{
    bool HasShowScoreDef() const;
    data_BOOLEAN GetShowScoreDef() const;
    bool HasShowBarLines() const;
    data_BOOLEAN GetShowBarLines() const;
    ///@}

    /**
     * @name Setter and getters
     */
    ///@{
    bool IsFirst() const { return (m_isFirst); }
    void SetFirst(bool isFirst) { m_isFirst = isFirst; }
    bool IsLast() const { return (m_isLast); }
    void SetLast(bool isLast) { m_isLast = isLast; }
    ///@}

    /**
     * Return true if the ossia element as more than one oStaff
     */
    bool HasMultipleOStaves() const;

    /**
     * Show scoreDef at drawing depending on the number of staves and the attribute values
     */
    bool DrawScoreDef() const;

    /**
     * Retrieve the original staff corresponding to the ossia staff
     */
    const Staff *GetOriginalStaffForOssia(const Staff *ossia) const;

    /**
     * @name Setter and getter of the clef and keySig alignment
     */
    ///@{
    void SetClefAlignment(Alignment *clefAlignment) { m_clefAlignment = clefAlignment; }
    void SetKeySigAlignment(Alignment *keySigAlignment) { m_keySigAlignment = keySigAlignment; }
    void ResetAlignments();
    ///@}

    /**
     * Return the ossia scoreDef shift using the clef and keySig alignments
     */
    int GetScoreDefShift() const;

    /**
     * Methods for adding allowed content.
     */
    bool IsSupportedChild(ClassId classId) override;

    /**
     * @name Fill the maps with ossia staves above and below
     */
    ///@{
    void GetStavesAbove(MapOfOssiaStaffNs &map) const;
    void GetStavesBelow(MapOfOssiaStaffNs &map) const;
    ///@}

    /**
     * @name Return the top and bottom oStaff (NULL if none)
     */
    ///@{
    const Staff *GetDrawingTopOStaff() const;
    const Staff *GetDrawingBottopOStaff() const;
    ///@}

    /**
     * Return the staff `@n` of oStaff in the order they appear
     */
    std::vector<int> GetOStaffNs() const;

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
    /**
     * Internal method for getting staves above or below.
     */
    void GetStaves(MapOfOssiaStaffNs &map, ListOfConstObjects &staves) const;

public:
    //
protected:
    //
private:
    /** The ossia staffGrp used for drawing */
    StaffGrp m_drawingStaffGrp;
    /** A flag indicating that the ossia is the first of a series */
    bool m_isFirst;
    /** A flag indicating that the ossia is the last of a series */
    bool m_isLast;
    /** The clef and keySig alignments for access to the shift */
    Alignment *m_clefAlignment;
    Alignment *m_keySigAlignment;
};

} // namespace vrv

#endif
