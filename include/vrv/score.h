/////////////////////////////////////////////////////////////////////////////
// Name:        score.h
// Author:      Laurent Pugin
// Created:     29/08/2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SCORE_H__
#define __VRV_SCORE_H__

#include "atts_shared.h"
#include "options.h"
#include "pageelement.h"
#include "pagemilestone.h"
#include "scoredef.h"

namespace vrv {

class SymbolDef;

//----------------------------------------------------------------------------
// Score
//----------------------------------------------------------------------------

/**
 * This class represent a <score> in MEI.
 * It is used only for loading score-based MEI documents before they are
 * converted to page-based MEI.
 */
class Score : public PageElement, public PageMilestoneInterface, public AttLabelled, public AttNNumberLike {

public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Score();
    Score(bool createScoreDef);
    virtual ~Score();
    void Reset() override;
    std::string GetClassName() const override { return "score"; }
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    bool IsSupportedChild(ClassId classId) override;
    ///@}

    /**
     * Getter for the score/scoreDef
     */
    ///@{
    void SetScoreDefSubtree(Object *subtree, ScoreDef *scoreScoreDef);
    ScoreDef *GetScoreDef() { return m_scoreDef; }
    const ScoreDef *GetScoreDef() const { return m_scoreDef; }
    Object *GetScoreDefSubtree() { return m_scoreDefSubtree; }
    ///@}

    /**
     * Calculate the height of the pgHead/pgHead2 and pgFoot/pgFoot2 (if any)
     * Requires the Doc to have an empty Pages object because it adds temporary pages
     * Called from Doc::CastOffBase
     */
    void CalcRunningElementHeight(Doc *doc);

    /**
     * Check whether we need to optimize score based on the condense option
     */
    bool ScoreDefNeedsOptimization(int optionCondense) const;

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
     * The score/scoreDef (first child of the score).
     * A score can have either a single scoreDef, or a subtree with editorial markup and different scoreDefs.
     * This member holds the selected scoreDef. It is set either in the constructor, or by SetScoreDefSubtree (only
     * once).
     */
    ScoreDef *m_scoreDef;
    /**
     * A complete subtree of the scoreDefs (including editorial markup).
     * The subtree is owned by the Score object. The m_scoreDef member is expected to be part of the subtree.
     */
    Object *m_scoreDefSubtree;

public:
    /**
     * @name Height of headers and footers for the score.
     * Fill by
     */
    ///@{
    int m_drawingPgHeadHeight;
    int m_drawingPgFootHeight;
    int m_drawingPgHead2Height;
    int m_drawingPgFoot2Height;
    ///@}
private:
    //
};

} // namespace vrv

#endif
