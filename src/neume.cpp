/////////////////////////////////////////////////////////////////////////////
// Name:        neume.cpp
// Author:      Andrew Tran, Juliette Regimbal
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "neume.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "artic.h"
#include "attcomparison.h"
#include "doc.h"
#include "editorial.h"
#include "elementpart.h"
#include "functorparams.h"
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

std::map<std::string, NeumeGroup> Neume::s_neumes = { { "", PUNCTUM }, { "u", PES }, { "d", CLIVIS }, 
    { "uu", SCANDICUS }, { "dd", CLIMACUS }, { "ud", TORCULUS }, { "du", PORRECTUS }, { "ddd", CLIMACUS }, 
    { "ddu", CLIMACUS_RESUPINUS },{ "udu", TORCULUS_RESUPINUS }, { "dud", PORRECTUS_FLEXUS },
    { "udd", PES_SUBPUNCTIS }, { "uud", SCANDICUS_FLEXUS }, { "uudd", SCANDICUS_SUBPUNCTIS },
    { "dudd", PORRECTUS_SUBPUNCTIS }, { "sd", PRESSUS } };

//----------------------------------------------------------------------------
// Neume
//----------------------------------------------------------------------------

Neume::Neume() : LayerElement("neume-"), ObjectListInterface(), AttColor()
{
    RegisterAttClass(ATT_COLOR);
    Reset();
}

Neume::~Neume() {}

void Neume::Reset()
{
    LayerElement::Reset();
    ResetColor();
}

void Neume::AddChild(Object *child)
{
    if (child->Is(NC)) {
        assert(dynamic_cast<Nc *>(child));
    }
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

int Neume::GetPosition(LayerElement *element)
{
    this->GetList(this);
    int position = this->GetListIndex(element);
    return position;
}

bool Neume::IsLastInNeume(LayerElement *element)
{
    int size = (int)this->GetList(this)->size();
    int position = this->GetPosition(element);

    // This method should be called only if the note is part of a neume
    assert(position != -1);
    // this is the last one
    if (position == (size - 1)) return true;
    return false;
}

NeumeGroup Neume::GetNeumeGroup()
{
    ArrayOfObjects children;
    AttComparison ac(NC);
    this->FindAllChildByComparison(&children, &ac);

    auto iter = children.begin();
    Nc *previous = dynamic_cast<Nc *>(*iter);
    if (previous == nullptr) return NEUME_ERROR;
    iter++;
    
    std::string key = "";
    
    for (; iter != children.end(); iter++)
    {
        Nc *current = dynamic_cast<Nc *>(*iter);
        assert(current);
        
        int pitchDifference = current->PitchDifferenceTo(previous);
        if (pitchDifference > 0)
        {
            key += "u";
        }
        else if (pitchDifference < 0)
        {
            key += "d";
        }
        else
        {
            key += "s";
        }
        previous = current;
    }
    return s_neumes[key]; 
}

std::vector<int> Neume::GetPitchDifferences()
{
    std::vector<int> pitchDifferences;
    ArrayOfObjects ncChildren;
    AttComparison ac(NC);
    this->FindAllChildByComparison(&ncChildren, &ac);

    pitchDifferences.reserve(ncChildren.size() - 1);

    // Iterate through children and calculate pitch differences
    auto iter = ncChildren.begin();
    Nc *previous = dynamic_cast<Nc *>(*iter);
    if (previous == nullptr) return pitchDifferences;
    iter ++;

    for (; iter != ncChildren.end(); iter++) {
        Nc *current = dynamic_cast<Nc *>(*iter);
        assert(current);
        pitchDifferences.push_back(current->PitchDifferenceTo(previous));
        previous = current;
    }
    return pitchDifferences;
}

bool Neume::GenerateChildMelodic()
{
    ArrayOfObjects children;
    AttComparison ac(NC);
    this->FindAllChildByComparison(&children, &ac);

    // Get the first neume component of the neume
    auto iter = children.begin();
    Nc *head = dynamic_cast<Nc *>(*iter);
    if (head == nullptr) return false;
    iter++;

    // Iterate on second to last neume component and add intm value
    for (; iter != children.end(); iter++) {
        Nc *current = dynamic_cast<Nc *>(*iter);
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

std::string Neume::NeumeGroupToString(NeumeGroup group)
{
    switch (group)
    {
        case PUNCTUM:
            return "Punctum";
        case CLIVIS:
            return "Clivis";
        case PES:
            return "Pes";
        case PRESSUS:
            return "Pressus";
        case CLIMACUS:
            return "Climacus";
        case PORRECTUS:
            return "Porrectus";
        case SCANDICUS:
            return "Scandicus";
        case TORCULUS:
            return "TORCULUS";
        case SCANDICUS_FLEXUS:
            return "Scandicus flexus";
        case PORRECTUS_FLEXUS:
            return "Porrectus flexus";
        case TORCULUS_RESUPINUS:
            return "Torculus resupinus";
        case CLIMACUS_RESUPINUS:
            return "Climacus resupinus";
        case PES_SUBPUNCTIS:
            return "Pes subpunctis";
        case PORRECTUS_SUBPUNCTIS:
            return "Porrectus subpunctis";
        case SCANDICUS_SUBPUNCTIS:
            return "Scandicus subpunctis"; 
        default:
            return "";
    }
}

} // namespace vrv
