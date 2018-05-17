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

std::map<std::string, NeumeGroup> Neume::s_neumes = { { "u", PES }, { "d", CLIVIS }, { "uu", SCANDICUS },
    { "dd", CLIMACUS }, { "ud", TORCULUS }, { "du", PORRECTUS }, { "ddd", CLIMACUS } };

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

} // namespace vrv
