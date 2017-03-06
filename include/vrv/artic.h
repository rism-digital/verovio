/////////////////////////////////////////////////////////////////////////////
// Name:        artic.h
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ARTIC_H__
#define __VRV_ARTIC_H__

#include "atts_shared.h"
#include "layerelement.h"

namespace vrv {

class ArticPart;

//----------------------------------------------------------------------------
// Artic
//----------------------------------------------------------------------------

class Artic : public LayerElement, public AttArticulation, public AttColor, public AttPlacement {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Artic();
    virtual ~Artic();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Artic"; }
    virtual ClassId GetClassId() const { return ARTIC; }
    ///@}

    /**
     * Add an element (an articPart) to a artic.
     */
    virtual void AddChild(Object *object);

    /**
     * Split the articulation content into an array with the values to be displayed inside the staff / slur
     * and the values to be displayed outside.
     * Used by Artic::PrepareArtic that then creates the corresponding ArticPart objects.
     */
    void SplitArtic(std::vector<data_ARTICULATION> *insideSlur, std::vector<data_ARTICULATION> *outsideSlur);

    /**
     * Update the outside ArticPart (place and drawingY)
     * If allowAbove is true it will place the above if the content requires so (even if place below if given)
     */
    void UpdateOutsidePartPosition(int yAbove, int yBelow, data_STAFFREL place, bool allowAbove);

    /**
     * Update the inside ArticPart (place and drawingY)
     */
    void UpdateInsidePartPosition(int yAbove, int yBelow, data_STAFFREL place);

    /**
     * Static method that retrieves the appropriate SMuFL code for a data_ARTICULATION with data_STAFFREL
     */
    static wchar_t GetSmuflCode(data_ARTICULATION artic, data_STAFFREL place);

    /**
     * Static method that retrieves the vertical correctoin for a SMuFL code for with data_STAFFREL.
     * The reason for this is that SMuFL sometimes has the glyph below the line, sometimes above.
     * See bow indications for an example where is is always above
     */
    static bool VerticalCorr(wchar_t code, data_STAFFREL place);

    /**
     * Static method that returns true if the data_ARTICULATION has to be centered between staff lines
     */
    static bool IsCentered(data_ARTICULATION artic);

    /**
     * Return the inside and outside part of an artic if any (NULL otherwiser)
     */
    ///@{
    ArticPart *GetInsidePart();
    ArticPart *GetOutsidePart();
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::AdjustArtic
     */
    virtual int AdjustArtic(FunctorParams *functorParams);

    /**
     * See Object::PrepareArtic
     */
    virtual int PrepareArtic(FunctorParams *functorParams);

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

private:
    //
public:
    /**
     * A static array for storing the articulation that have to be placed outside the staff
     */
    static std::vector<data_ARTICULATION> s_outStaffArtic;
    /**
     * A static array for storing the articulation that have to be place above the staff is possible
     */
    static std::vector<data_ARTICULATION> s_aboveStaffArtic;

private:
};

//----------------------------------------------------------------------------
// ArticPart
//----------------------------------------------------------------------------

/**
 * This class models a sub-part of an artic element and has not direct MEI correspondant.
 */

class ArticPart : public LayerElement, public AttArticulation, public AttColor, public AttPlacement {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    ArticPart(ArticPartType type, Artic *artic);
    virtual ~ArticPart();
    virtual void Reset();
    virtual std::string GetClassName() const { return "ArticPart"; }
    virtual ClassId GetClassId() const { return ARTIC_PART; }
    ///@}

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

    /**
     * @name Set and get the type of the alignment
     */
    ///@{
    void SetType(ArticPartType type) { m_type = type; }
    ArticPartType GetType() const { return m_type; }
    ///@}

    /**
     * Check if the articList contains data_ARTICULATION has to be place above staff.
     */
    bool AlwaysAbove();

    void AddSlurPositioner(FloatingPositioner *positioner, bool start);

    //----------//
    // Functors //
    //----------//

    /**
     * Overwritten version of Save that avoids anything to be written
     */
    virtual int Save(FunctorParams *) { return FUNCTOR_CONTINUE; }
    virtual int SaveEnd(FunctorParams *) { return FUNCTOR_CONTINUE; }

    /**
     * See Object::AdjustArticWithSlurs
     */
    virtual int AdjustArticWithSlurs(FunctorParams *functorParams);

    /**
     * See Object::ResetVerticalAlignment
     */
    virtual int ResetVerticalAlignment(FunctorParams *functorParams);

private:
    //
public:
    //
private:
    /** the type of artic part */
    ArticPartType m_type;

public:
    std::vector<FloatingPositioner *> m_startSlurPositioners;
    std::vector<FloatingPositioner *> m_endSlurPositioners;
};

} // namespace vrv

#endif
