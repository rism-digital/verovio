/////////////////////////////////////////////////////////////////////////////
// Name:        artic.h
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ARTIC_H__
#define __VRV_ARTIC_H__

#include "atts_externalsymbols.h"
#include "atts_gestural.h"
#include "atts_shared.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Artic
//----------------------------------------------------------------------------

class Artic : public LayerElement,
              public AttArticulation,
              public AttArticulationGestural,
              public AttColor,
              public AttEnclosingChars,
              public AttExtSym,
              public AttPlacementRelEvent {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Artic();
    virtual ~Artic();
    Object *Clone() const override { return new Artic(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "Artic"; }
    ///@}

    /** Override the method since alignment is required */
    bool HasToBeAligned() const override { return true; }

    /** Override the method since it is align to the staff */
    bool IsRelativeToStaff() const override { return true; }

    data_ARTICULATION GetArticFirst() const;

    /**
     * Split the multi-valued artic attributes into distinct artic elements.
     * Applied by ConvertMarkupArtic functor.
     */
    void SplitMultival(Object *parent);

    void GetAllArtics(bool direction, std::vector<Artic *> &artics);

    /**
     * Return the inside and outside part of an artic if any (NULL otherwiser)
     */
    ///@{
    bool IsInsideArtic() const;
    bool IsOutsideArtic() const { return !IsInsideArtic(); };
    ///@}

    /**
     * Check if the articList contains data_ARTICULATION has to be place above staff.
     */
    bool AlwaysAbove() const;

    void AddSlurPositioner(FloatingCurvePositioner *positioner, bool start);

    data_STAFFREL GetDrawingPlace() const { return m_drawingPlace; }

    /**
     * Retrieves the appropriate SMuFL code for a data_ARTICULATION with data_STAFFREL
     */
    char32_t GetArticGlyph(data_ARTICULATION artic, data_STAFFREL place) const;

    /**
     * Retrieve parentheses / brackets from the enclose attribute
     */
    std::pair<char32_t, char32_t> GetEnclosingGlyphs() const;

    //----------------//
    // Static methods //
    //----------------//

    /**
     * Static method that retrieves the vertical correction for a SMuFL code with data_STAFFREL.
     * The reason for this is that SMuFL sometimes has the glyph below the line, sometimes above.
     * See bow indications for an example where is is always above
     */
    static bool VerticalCorr(char32_t code, data_STAFFREL place);

    /**
     * Static method that returns true if the data_ARTICULATION has to be centered between staff lines
     */
    static bool IsCentered(data_ARTICULATION artic);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ConvertMarkupArtic
     */
    int ConvertMarkupArtic(FunctorParams *functorParams) override;

    /**
     * See Object::CalcArtic
     */
    int CalcArtic(FunctorParams *functorParams) override;

    /**
     * See Object::AdjustArtic
     */
    int AdjustArtic(FunctorParams *functorParams) override;

    /**
     * See Object::AdjustArticWithSlurs
     */
    int AdjustArticWithSlurs(FunctorParams *functorParams) override;

    /**
     * See Object::ResetVerticalAlignment
     */
    int ResetVerticalAlignment(FunctorParams *functorParams) override;

    /**
     * See Object::ResetData
     */
    int ResetData(FunctorParams *functorParams) override;

private:
    bool IsInsideArtic(data_ARTICULATION artic) const;
    // Calculate shift for the articulation based on its type and presence of other articulations
    int CalculateHorizontalShift(const Doc *doc, const LayerElement *parent, data_STEMDIRECTION stemDir) const;

public:
    std::vector<FloatingCurvePositioner *> m_startSlurPositioners;
    std::vector<FloatingCurvePositioner *> m_endSlurPositioners;

    /**
     * A static array for storing the articulation that have to be placed outside the staff
     */
    static const std::vector<data_ARTICULATION> s_outStaffArtic;
    /**
     * A static array for storing the articulation that have to be place above the staff is possible
     */
    static const std::vector<data_ARTICULATION> s_aboveStaffArtic;

private:
    //
    data_STAFFREL m_drawingPlace;
};

} // namespace vrv

#endif
