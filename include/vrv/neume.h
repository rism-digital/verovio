/////////////////////////////////////////////////////////////////////////////
// Name:        neume.h
// Author:      Andrew Tran
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_NEUME_H__
#define __VRV_NEUME_H__

#include <cassert>

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
    void Reset() override;
    Object *Clone() const override { return new Neume(*this); }
    std::string GetClassName() const override { return "neume"; }
    ///@}

    /**
     * Add an element (a note or a rest) to a syllable.
     * Only syl or neume will be added.
     */
    bool IsSupportedChild(ClassId classId) override;

    int GetLigatureCount(int position);
    bool IsLastInNeume(const LayerElement *element) const;

    bool GenerateChildMelodic();

    NeumeGroup GetNeumeGroup() const;

    std::vector<int> GetPitchDifferences() const;

    PitchInterface *GetHighestPitch();
    PitchInterface *GetLowestPitch();

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
    int GetPosition(const LayerElement *element) const;

public:
    //----------------//
    // Static members //
    //----------------//

    /**
     * String keys come from the contours of neume groupings as defined in MEI4
     */
    static const std::map<std::string, NeumeGroup> s_neumes;

    static std::string NeumeGroupToString(NeumeGroup group);

private:
};
} // namespace vrv

#endif
