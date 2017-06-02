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
#include "systemelement.h"

namespace vrv {

class Section;

//----------------------------------------------------------------------------
// Section
//----------------------------------------------------------------------------

/**
 * This class represents a MEI section.
 * It can be both a container (in score-based MEI) and a boundary (in page-based MEI)
 */
class Section : public SystemElement, public BoundaryStartInterface, public AttNNumberLike {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Section();
    virtual ~Section();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Section"; }
    virtual ClassId GetClassId() const { return SECTION; }
    ///@}

    /**
     * Method for adding allowed content
     */
    virtual void AddChild(Object *object);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ConvertToPageBased
     */
    ///@{
    virtual int ConvertToPageBased(FunctorParams *functorParams);
    virtual int ConvertToPageBasedEnd(FunctorParams *functorParams);
    ///@}

    /**
     * See Object::PrepareBoundaries
     */
    virtual int PrepareBoundaries(FunctorParams *functorParams);

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

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
class Pb : public SystemElement, public AttLabelled, public AttNNumberLike {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Pb();
    virtual ~Pb();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Pb"; }
    virtual ClassId GetClassId() const { return PB; }
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::CastOffEncoding
     */
    virtual int CastOffEncoding(FunctorParams *functorParams);

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
class Sb : public SystemElement, public AttLabelled, public AttNNumberLike {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Sb();
    virtual ~Sb();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Sb"; }
    virtual ClassId GetClassId() const { return SB; }
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::CastOffEncoding
     */
    virtual int CastOffEncoding(FunctorParams *functorParams);

private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif
