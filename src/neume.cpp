/////////////////////////////////////////////////////////////////////////////
// Name:        neume.cpp
// Author:      Andrew Tran, Juliette Regimbal
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "neume.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "artic.h"
#include "comparison.h"
#include "doc.h"
#include "editorial.h"
#include "elementpart.h"
#include "functor.h"
#include "glyph.h"
#include "layer.h"
#include "nc.h"
#include "slur.h"
#include "smufl.h"
#include "staff.h"
#include "syl.h"
#include "tie.h"
#include "verse.h"
#include "vrv.h"

namespace vrv {

const std::map<std::string, NeumeGroup> Neume::s_neumes
    = { { "", PUNCTUM }, { "u", PES }, { "d", CLIVIS }, { "uu", SCANDICUS }, { "dd", CLIMACUS }, { "ud", TORCULUS },
          { "du", PORRECTUS }, { "ddd", CLIMACUS }, { "ddu", CLIMACUS_RESUPINUS }, { "udu", TORCULUS_RESUPINUS },
          { "dud", PORRECTUS_FLEXUS }, { "udd", PES_SUBPUNCTIS }, { "uud", SCANDICUS_FLEXUS },
          { "uudd", SCANDICUS_SUBPUNCTIS }, { "dudd", PORRECTUS_SUBPUNCTIS }, { "sd", PRESSUS } };

//----------------------------------------------------------------------------
// Neume
//----------------------------------------------------------------------------

static const ClassRegistrar<Neume> s_factory("neume", NEUME);

Neume::Neume() : LayerElement(NEUME), ObjectListInterface(), AttColor()
{
    this->RegisterAttClass(ATT_COLOR);
    this->Reset();
}

Neume::~Neume() {}

void Neume::Reset()
{
    LayerElement::Reset();
    this->ResetColor();
}

bool Neume::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ NC };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else {
        return false;
    }
}

int Neume::GetPosition(const LayerElement *element) const
{
    this->GetList();
    return this->GetListIndex(element);
}

int Neume::GetLigatureCount(int position)
{
    int ligCount = 0;
    this->GetList();
    for (int pos = 0; pos <= position; pos++) {
        Object *posObj = this->GetChild(pos);
        if (posObj != NULL) {
            Nc *posNc = dynamic_cast<Nc *>(posObj);
            assert(posNc);
            if (posNc->GetLigated() == BOOLEAN_true) { // first part of the ligature
                ligCount += 1;
            }
        }
    }
    return ligCount;
}

bool Neume::IsLastInNeume(const LayerElement *element) const
{
    const int size = this->GetListSize();
    int position = this->GetPosition(element);

    // This method should be called only if the note is part of a neume
    assert(position != -1);
    // this is the last one
    if (position == (size - 1)) return true;
    return false;
}

NeumeGroup Neume::GetNeumeGroup() const
{
    ListOfConstObjects children = this->FindAllDescendantsByType(NC);

    auto iter = children.begin();
    const Nc *previous = dynamic_cast<const Nc *>(*iter);
    if (previous == NULL) return NEUME_ERROR;
    ++iter;

    std::string key = "";

    for (; iter != children.end(); ++iter) {
        const Nc *current = vrv_cast<const Nc *>(*iter);
        assert(current);

        int pitchDifference = current->PitchDifferenceTo(previous);
        if (pitchDifference > 0) {
            key += "u";
        }
        else if (pitchDifference < 0) {
            key += "d";
        }
        else {
            key += "s";
        }
        previous = current;
    }

    if (s_neumes.contains(key)) {
        return s_neumes.at(key);
    }
    else {
        return NEUME_ERROR;
    }
}

std::vector<int> Neume::GetPitchDifferences() const
{
    std::vector<int> pitchDifferences;
    ListOfConstObjects ncChildren = this->FindAllDescendantsByType(NC);

    pitchDifferences.reserve(ncChildren.size() - 1);

    // Iterate through children and calculate pitch differences
    auto iter = ncChildren.begin();
    const Nc *previous = dynamic_cast<const Nc *>(*iter);
    if (previous == NULL) return pitchDifferences;
    ++iter;

    for (; iter != ncChildren.end(); ++iter) {
        const Nc *current = vrv_cast<const Nc *>(*iter);
        assert(current);
        pitchDifferences.push_back(current->PitchDifferenceTo(previous));
        previous = current;
    }
    return pitchDifferences;
}

bool Neume::GenerateChildMelodic()
{
    ListOfObjects children = this->FindAllDescendantsByType(NC);

    // Get the first neume component of the neume
    auto iter = children.begin();
    Nc *head = dynamic_cast<Nc *>(*iter);
    if (head == NULL) return false;
    ++iter;

    // Iterate on second to last neume component and add intm value
    for (; iter != children.end(); ++iter) {
        Nc *current = vrv_cast<Nc *>(*iter);
        assert(current);
        std::string intmValue;

        int pitchDifference = current->PitchDifferenceTo(head);
        if (pitchDifference > 0) {
            intmValue = "u";
        }
        else if (pitchDifference < 0) {
            intmValue = "d";
        }
        else {
            intmValue = "s";
        }

        current->SetIntm(intmValue);
        head = current;
    }

    return true;
}

PitchInterface *Neume::GetHighestPitch()
{
    ListOfObjects pitchChildren;
    InterfaceComparison ic(INTERFACE_PITCH);
    this->FindAllDescendantsByComparison(&pitchChildren, &ic);

    auto it = pitchChildren.begin();
    PitchInterface *max = (*it)->GetPitchInterface();
    if (!max) return NULL;
    for (it++; it != pitchChildren.end(); it++) {
        PitchInterface *pi = vrv_cast<PitchInterface *>((*it)->GetPitchInterface());
        assert(pi);
        if (pi->PitchDifferenceTo(max) > 0) {
            max = pi;
        }
    }
    return max;
}

PitchInterface *Neume::GetLowestPitch()
{
    ListOfObjects pitchChildren;
    InterfaceComparison ic(INTERFACE_PITCH);
    this->FindAllDescendantsByComparison(&pitchChildren, &ic);

    auto it = pitchChildren.begin();
    PitchInterface *min = (*it)->GetPitchInterface();
    if (!min) return NULL;
    for (it++; it != pitchChildren.end(); it++) {
        PitchInterface *pi = vrv_cast<PitchInterface *>((*it)->GetPitchInterface());
        assert(pi);
        if (pi->PitchDifferenceTo(min) < 0) {
            min = pi;
        }
    }
    return min;
}

FunctorCode Neume::Accept(Functor &functor)
{
    return functor.VisitNeume(this);
}

FunctorCode Neume::Accept(ConstFunctor &functor) const
{
    return functor.VisitNeume(this);
}

FunctorCode Neume::AcceptEnd(Functor &functor)
{
    return functor.VisitNeumeEnd(this);
}

FunctorCode Neume::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitNeumeEnd(this);
}

} // namespace vrv
