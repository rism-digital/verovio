/////////////////////////////////////////////////////////////////////////////
// Name:        ending.h
// Author:      Laurent Pugin
// Created:     14/07/2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ENDING_H__
#define __VRV_ENDING_H__

#include "atts_shared.h"
#include "boundary.h"
#include "systemelement.h"

namespace vrv {

class Measure;

//----------------------------------------------------------------------------
// Ending
//----------------------------------------------------------------------------

/**
 * This class represents a MEI ending.
 * It can be both a container (in score-based MEI) and a boundary (in page-based MEI).
 * It inherits from FloatingElement for spanning drawing features.
 */
class Ending : public SystemElement, public BoundaryStartInterface, public AttLineRend, public AttNNumberLike {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Ending();
    virtual ~Ending();
    virtual Object *Clone() const { return new Ending(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Ending"; }
    virtual ClassId GetClassId() const { return ENDING; }
    ///@}

    /**
     * Method for adding allowed content
     */
    virtual bool IsSupportedChild(Object *object);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ConvertToPageBased
     */
    virtual int ConvertToPageBased(FunctorParams *functorParams);
    virtual int ConvertToPageBasedEnd(FunctorParams *functorParams);

    /**
     * See Object::PrepareBoundaries
     */
    virtual int PrepareBoundaries(FunctorParams *functorParams);

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

    /**
     * See Object::CastOffSystems
     */
    virtual int CastOffSystems(FunctorParams *functorParams);

    /**
     * See Object::CastOffEncoding
     */
    virtual int CastOffEncoding(FunctorParams *functorParams);

    /**
     * See Object::PrepareFloatingGrps
     */
    virtual int PrepareFloatingGrps(FunctorParams *functoParams);

private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif
