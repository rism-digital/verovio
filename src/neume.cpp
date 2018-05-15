/////////////////////////////////////////////////////////////////////////////
// Name:        neume.cpp
// Author:      Andrew Tran
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

//----------------------------------------------------------------------------
// Neume
//----------------------------------------------------------------------------

Neume::Neume()
    : LayerElement("neume-"),
      ObjectListInterface(),
      AttColor()
{
    RegisterAttClass(ATT_COLOR);
    Reset();
}

Neume::~Neume()
{

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

void Neume::Reset()
{
    LayerElement::Reset();
    ResetColor();
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

    // This method should be called only if the note is part of a beam
    assert(position != -1);
    // this is the last one
    if (position == (size - 1)) return true;
    return false;
}

<<<<<<< HEAD
bool Neume::GenerateChildMelodic()
{
    ArrayOfObjects children;
    AttComparison ac(NC);
    this->FindAllChildByAttComparison(&children, &ac);

    auto iter = children.begin();
    Nc *head = dynamic_cast<Nc *>(*iter);
    if (head == nullptr) return false;
    iter++;

    for (; iter != children.end(); iter++)
    {
        Nc *current = dynamic_cast<Nc *>(*iter);
        assert(current);
        std::string intmValue;

        int pitchDifference = current->PitchDifferenceTo(head);
        if (pitchDifference > 0)
        {
            intmValue = "u";
        }
        else if (pitchDifference < 0)
        {
            intmValue = "d";
        }
        else
        {
            intmValue = "s";
        }

        current->SetIntm(intmValue);
        head = current;
    }

    return true;
}

=======
std::map<std::string, NeumeGroup> Neume::InitNeumeGroups() {
    std::map<std::string, NeumeGroup> map;
    map["u"] = PES;
    map["d"] = CLIVIS;
    map["uu"] = SCANDICUS;
    map["dd"] = CLIMACUS;
    map["ud"] = TORCULUS;
    map["du"] = PORRECTUS;
    map["ddd"] = CLIMACUS;
    return map;
}

std::map<std::string, NeumeGroup> Neume::neumes = Neume::InitNeumeGroups();

>>>>>>> feature/neumegroup
} // namespace vrv
