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
              public AttArticulationGes,
              public AttColor,
              public AttEnclosingChars,
              public AttExtSymAuth,
              public AttExtSymNames,
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
    std::string GetClassName() const override { return "artic"; }
    ///@}

    /** Override the method since alignment is required */
    bool HasToBeAligned() const override { return true; }

    /** Override the method since it is align to the staff */
    bool IsRelativeToStaff() const override { return true; }

    data_ARTICULATION GetArticFirst() const;

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

    /**
     * Getter and setter for the drawing place
     */
    ///@{
    data_STAFFREL GetDrawingPlace() const { return m_drawingPlace; }
    void SetDrawingPlace(data_STAFFREL drawingPlace) { m_drawingPlace = drawingPlace; }
    ///@}

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
     * Interface for class functor visitation
     */
    ///@{
    FunctorCode Accept(Functor &functor) override;
    FunctorCode Accept(ConstFunctor &functor) const override;
    FunctorCode AcceptEnd(Functor &functor) override;
    FunctorCode AcceptEnd(ConstFunctor &functor) const override;
    ///@}

private:
    bool IsInsideArtic(data_ARTICULATION artic) const;

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
