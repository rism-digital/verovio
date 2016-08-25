/////////////////////////////////////////////////////////////////////////////
// Name:        section.h
// Author:      Laurent Pugin
// Created:     24/08/2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SECTION_H__
#define __VRV_SECTION_H__

#include "atts_shared.h"
#include "boundary.h"
#include "object.h"

namespace vrv {

class Section;

//----------------------------------------------------------------------------
// Section
//----------------------------------------------------------------------------

/**
 * This class represents a MEI section.
 * It can be both a container (in score-based MEI) and a boundary (in page-based MEI)
 */
class Section : public Object, public BoundaryStartInterface, public AttCommon, public AttCommonPart {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Section();
    virtual ~Section();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Section"; };
    virtual ClassId Is() const { return SECTION; };
    ///@}

    /**
     * Method for adding allowed content
     */
    virtual void AddChild(Object *object);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::PrepareBoundaries.
     */
    virtual int PrepareBoundaries(FunctorParams *functorParams);

    /**
     * Reset the drawing values before calling PrepareDrawing after changes.
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

    /**
     * Fill a page by adding systems with the appropriate length.
     * For Section, this means only moving them since their width is not taken into
     * account
     */
    virtual int CastOffSystems(FunctorParams *functorParams);

private:
    //
public:
    //
private:
    //
};

//----------------------------------------------------------------------------
// Pb
//----------------------------------------------------------------------------

/**
 * This class represents a MEI pb in score-based MEI.
 * In page-based MEI, it remains as is as. Actuall pages are represented by Page objects.
 */
class Pb : public Object, public AttCommon, public AttCommonPart {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Pb();
    virtual ~Pb();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Pb"; };
    virtual ClassId Is() const { return PB; };
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * Fill a page by adding systems with the appropriate length.
     * For Pb, this means only moving them since their width is not taken into
     * account
     */
    virtual int CastOffSystems(FunctorParams *functorParams);

private:
    //
public:
    //
private:
    //
};

//----------------------------------------------------------------------------
// Sb
//----------------------------------------------------------------------------

/**
 * This class represents a MEI sb in score-based MEI.
 * In page-based MEI, it remains as is as. Actuall systems are represented by System objects.
 */
class Sb : public Object, public AttCommon, public AttCommonPart {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Sb();
    virtual ~Sb();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Sb"; };
    virtual ClassId Is() const { return SB; };
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * Fill a page by adding systems with the appropriate length.
     * For Sb, this means only moving them since their width is not taken into
     * account
     */
    virtual int CastOffSystems(FunctorParams *functorParams);

private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif
