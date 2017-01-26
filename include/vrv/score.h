/////////////////////////////////////////////////////////////////////////////
// Name:        score.h
// Author:      Laurent Pugin
// Created:     29/08/2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SCORE_H__
#define __VRV_SCORE_H__

#include "object.h"

namespace vrv {

//----------------------------------------------------------------------------
// Score
//----------------------------------------------------------------------------

/**
 * This class represent a <score> in MEI.
 * It is used only for loading score-based MEI documents before they are
 * converted to page-based MEI.
 */
class Score : public Object {

public:
    // constructors and destructors
    Score();
    virtual ~Score();
    virtual ClassId GetClassId() const { return SCORE; }

    virtual void AddChild(Object *object);

    /*
     * Clear the content of the score.
     */
    virtual void Reset();

    //----------//
    // Functors //
    //----------//

private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif
