/////////////////////////////////////////////////////////////////////////////
// Name:        artic.h
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ARTIC_H__
#define __VRV_ARTIC_H__

#include "atts_externalsymbols.h"
#include "atts_shared.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Artic
//----------------------------------------------------------------------------

class Artic : public LayerElement,
              public AttArticulation,
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
    virtual Object *Clone() const { return new Artic(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Artic"; }
    virtual ClassId GetClassId() const { return ARTIC; }
    ///@}

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

    /** Override the method since it is align to the staff */
    virtual bool IsRelativeToStaff() const { return true; }

    data_ARTICULATION GetArticFirst() const;

    /**
     * Split the multi-valued artic attributes into distinct artic elements.
     * Applied by ConvertMarkupArtic functor.
     */
    void SplitMultival(Object *parent);

    void GetAllArtics(bool direction, std::vector<Artic *> &artics);

    /**
     * Split the articulation content into an array with the values to be displayed inside the staff / slur
     * and the values to be displayed outside.
     * Used by Artic::PrepareLayerElementParts that then creates the corresponding ArticPart objects.
     */
    void SplitArtic(std::vector<data_ARTICULATION> *insideSlur, std::vector<data_ARTICULATION> *outsideSlur);

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
    bool AlwaysAbove();

    void AddSlurPositioner(FloatingCurvePositioner *positioner, bool start);

    data_STAFFREL GetDrawingPlace() const { return m_drawingPlace; }

    /**
     * Retrieves the appropriate SMuFL code for a data_ARTICULATION with data_STAFFREL
     */
    wchar_t GetArticGlyph(data_ARTICULATION artic, data_STAFFREL place) const;

    /**
     * Retrieves parentheses / brackets from the enclose attribute
     */
    wchar_t GetEnclosingGlyph(bool beforeArtic) const;

    //----------------//
    // Static methods //
    //----------------//

    /**
     * Static method that retrieves the vertical correction for a SMuFL code with data_STAFFREL.
     * The reason for this is that SMuFL sometimes has the glyph below the line, sometimes above.
     * See bow indications for an example where is is always above
     */
    static bool VerticalCorr(wchar_t code, data_STAFFREL place);

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
    virtual int ConvertMarkupArtic(FunctorParams *functorParams);

    /**
     * See Object::CalcArtic
     */
    virtual int CalcArtic(FunctorParams *functorParams);

    /**
     * See Object::AdjustArtic
     */
    virtual int AdjustArtic(FunctorParams *functorParams);

    /**
     * See Object::AdjustArticWithSlurs
     */
    virtual int AdjustArticWithSlurs(FunctorParams *functorParams);

    /**
     * See Object::ResetVerticalAlignment
     */
    virtual int ResetVerticalAlignment(FunctorParams *functorParams);

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

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
