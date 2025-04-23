/////////////////////////////////////////////////////////////////////////////
// Name:        btrem.cpp
// Author:      Klaus Rettinghaus
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "btrem.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <math.h>

//----------------------------------------------------------------------------

#include "chord.h"
#include "comparison.h"
#include "editorial.h"
#include "functor.h"
#include "layer.h"
#include "note.h"
#include "tuplet.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// BTrem
//----------------------------------------------------------------------------

static const ClassRegistrar<BTrem> s_factory("btrem", BTREM);

BTrem::BTrem() : LayerElement(BTREM), AttNumbered(), AttNumberPlacement(), AttTremForm(), AttTremMeasured()
{
    this->RegisterAttClass(ATT_NUMBERED);
    this->RegisterAttClass(ATT_NUMBERPLACEMENT);
    this->RegisterAttClass(ATT_TREMFORM);
    this->RegisterAttClass(ATT_TREMMEASURED);

    this->Reset();
}

BTrem::~BTrem() {}

void BTrem::Reset()
{
    LayerElement::Reset();
    this->ResetNumbered();
    this->ResetNumberPlacement();
    this->ResetTremForm();
    this->ResetTremMeasured();
}

bool BTrem::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ CHORD, CLEF, NOTE };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else if (Object::IsEditorialElement(classId)) {
        return true;
    }
    else {
        return false;
    }
}

FunctorCode BTrem::Accept(Functor &functor)
{
    return functor.VisitBTrem(this);
}

FunctorCode BTrem::Accept(ConstFunctor &functor) const
{
    return functor.VisitBTrem(this);
}

FunctorCode BTrem::AcceptEnd(Functor &functor)
{
    return functor.VisitBTremEnd(this);
}

FunctorCode BTrem::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitBTremEnd(this);
}

data_DURATION BTrem::CalcIndividualNoteDuration() const
{
    // Check if duration is given by attribute
    if (this->HasUnitdur()) {
        return this->GetUnitdur();
    }

    // Otherwise consider duration and stem modifier of first child chord/note
    data_DURATION childDur = DURATION_NONE;
    data_STEMMODIFIER stemMod = STEMMODIFIER_NONE;
    const Chord *chord = vrv_cast<const Chord *>(this->FindDescendantByType(CHORD));
    if (chord) {
        childDur = chord->GetDur();
        stemMod = chord->GetStemMod();
    }
    else {
        const Note *note = vrv_cast<const Note *>(this->FindDescendantByType(NOTE));
        if (note) {
            childDur = note->GetDur();
            stemMod = note->GetStemMod();
        }
    }

    // Calculate duration from number of slashes
    if ((stemMod >= STEMMODIFIER_1slash) && (stemMod <= STEMMODIFIER_6slash)) {
        if ((childDur >= DURATION_long) && (childDur <= DURATION_1024)) {
            int value = std::max<int>(childDur, DURATION_4);
            value += (stemMod - STEMMODIFIER_1slash + 1);
            value = std::min<int>(value, DURATION_1024);
            assert((value >= DURATION_8) && (value <= DURATION_1024));
            return static_cast<data_DURATION>(value);
        }
    }
    return DURATION_NONE;
}

data_STEMMODIFIER BTrem::GetDrawingStemMod() const
{
    Object *child = const_cast<BTrem *>(this)->FindDescendantByType(CHORD);
    if (!child) {
        child = const_cast<BTrem *>(this)->FindDescendantByType(NOTE);
        if (!child) return STEMMODIFIER_NONE;
    }

    data_STEMMODIFIER stemMod = vrv_cast<LayerElement *>(child)->GetDrawingStemMod();
    if (stemMod != STEMMODIFIER_NONE) return stemMod;

    DurationInterface *duration = child->GetDurationInterface();
    if (!duration) return STEMMODIFIER_NONE;
    const int drawingDur = duration->GetActualDur();

    if (!this->HasUnitdur()) {
        if (drawingDur < DURATION_2) return STEMMODIFIER_3slash;
        return STEMMODIFIER_NONE;
    }
    int slashDur = this->GetUnitdur() - drawingDur;
    if (drawingDur < DURATION_4) slashDur = this->GetUnitdur() - DURATION_4;
    switch (slashDur) {
        case 0: return STEMMODIFIER_NONE;
        case 1: return STEMMODIFIER_1slash;
        case 2: return STEMMODIFIER_2slash;
        case 3: return STEMMODIFIER_3slash;
        case 4: return STEMMODIFIER_4slash;
        case 5: return STEMMODIFIER_5slash;
        case 6: return STEMMODIFIER_6slash;
        default: break;
    }

    return STEMMODIFIER_NONE;
}

} // namespace vrv
