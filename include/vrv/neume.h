/////////////////////////////////////////////////////////////////////////////
// Name:        neume.h
// Author:      Andrew Tran
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_NEUME_H__
#define __VRV_NEUME_H__

#include <assert.h>

//----------------------------------------------------------------------------

#include "accid.h"
#include "atts_mensural.h"
#include "atts_shared.h"
#include "beam.h"
#include "chord.h"
#include "durationinterface.h"
#include "layerelement.h"
#include "pitchinterface.h"

namespace vrv {

class Accid;
class Chord;
class Slur;
class Tie;
class Verse;
class Neume;

//----------------------------------------------------------------------------
// Note
//----------------------------------------------------------------------------

enum NeumeGroup {
    NEUME_ERROR = 0,
    PUNCTUM,
    CLIVIS,
    PES,
    PRESSUS,
    CLIMACUS,
    PORRECTUS,
    SCANDICUS,
    TORCULUS,
    SCANDICUS_FLEXUS,
    PORRECTUS_FLEXUS,
    TORCULUS_RESUPINUS,
    CLIMACUS_RESUPINUS,
    PES_SUBPUNCTIS,
    PORRECTUS_SUBPUNCTIS,
    SCANDICUS_SUBPUNCTIS
};

/**
 * This class models the MEI <neume> element.
 */

class Neume : public LayerElement, public ObjectListInterface, public AttColor {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Neume();
    virtual ~Neume();
    virtual void Reset();
    virtual Object *Clone() const { return new Neume(*this); }
    virtual std::string GetClassName() const { return "Neume"; }
    virtual ClassId GetClassId() const { return NEUME; }
    ///@}

    /**
     * Add an element (a note or a rest) to a syllable.
     * Only syl or neume will be added.
     */
    virtual bool IsSupportedChild(Object *object);

    virtual int GetPosition(LayerElement *element);
    virtual bool IsLastInNeume(LayerElement *element);

    bool GenerateChildMelodic();

    NeumeGroup GetNeumeGroup();

    std::vector<int> GetPitchDifferences();

    PitchInterface *GetHighestPitch();
    PitchInterface *GetLowestPitch();

private:
    //
public:
    /**
     * String keys come from the contours of neume groupings as defined in MEI4
     */
    static std::map<std::string, NeumeGroup> s_neumes;

    static std::string NeumeGroupToString(NeumeGroup group);

private:
};
} // namespace vrv

#endif
