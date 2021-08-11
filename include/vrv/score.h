/////////////////////////////////////////////////////////////////////////////
// Name:        score.h
// Author:      Laurent Pugin
// Created:     29/08/2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SCORE_H__
#define __VRV_SCORE_H__

#include "atts_shared.h"
#include "pageboundary.h"
#include "pageelement.h"
#include "scoredef.h"

namespace vrv {

//----------------------------------------------------------------------------
// Score
//----------------------------------------------------------------------------

/**
 * This class represent a <score> in MEI.
 * It is used only for loading score-based MEI documents before they are
 * converted to page-based MEI.
 */
class Score : public PageElement, public PageElementStartInterface, public AttLabelled, public AttNNumberLike {

public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Score();
    virtual ~Score();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Score"; }
    virtual ClassId GetClassId() const { return SCORE; }
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    virtual bool IsSupportedChild(Object *object);
    ///@}

    /**
     * Getter for the score/scoreDef
     */
    ScoreDef *GetScoreDef() { return &m_scoreDef; }

    /**
     * Helper looking at the parent Doc and set its scoreDef as current one.
     * Called from Object::Process
     */
    void SetScoreDefAsCurrent();

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
     * See Object::UnCastOff
     */
    virtual int UnCastOff(FunctorParams *functorParams);

    /**
     * See Object::ConvertToCastOffMensural
     */
    virtual int ConvertToCastOffMensural(FunctorParams *params);

private:
    /**
     * The score/scoreDef (first child of the score)
     */
    ScoreDef m_scoreDef;

public:
    //
private:
    //
};

} // namespace vrv

#endif
