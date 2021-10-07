/////////////////////////////////////////////////////////////////////////////
// Name:        section.h
// Author:      Laurent Pugin
// Created:     24/08/2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SECTION_H__
#define __VRV_SECTION_H__

#include "atts_shared.h"
#include "atts_visual.h"
#include "systemboundary.h"
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
class Section : public SystemElement, public SystemElementStartInterface, public AttNNumberLike, public AttSectionVis {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Section();
    virtual ~Section();
    virtual Object *Clone() const { return new Section(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Section"; }
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
    ///@{
    int ConvertToPageBased(FunctorParams *functorParams) override;
    int ConvertToPageBasedEnd(FunctorParams *functorParams) override;
    ///@}

    /**
     * See Object::ConvertToUnCastOffMensural
     */
    int ConvertToUnCastOffMensural(FunctorParams *functorParams) override;

    /**
     * See Object::PrepareBoundaries
     */
    int PrepareBoundaries(FunctorParams *functorParams) override;

    /**
     * See Object::ResetDrawing
     */
    int ResetDrawing(FunctorParams *functorParams) override;

private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif
