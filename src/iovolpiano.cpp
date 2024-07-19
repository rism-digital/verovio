/////////////////////////////////////////////////////////////////////////////
// Name:        iovolpiano.cpp
// Author:      Laurent Pugin
// Created:     2024
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "iovolpiano.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <fstream>
#include <sstream>
#include <string>

//----------------------------------------------------------------------------

#include "clef.h"
#include "doc.h"
#include "keysig.h"
#include "layer.h"
#include "mdiv.h"
#include "measure.h"
#include "note.h"
#include "score.h"
#include "section.h"
#include "staff.h"
#include "staffdef.h"
#include "staffgrp.h"
#include "vrv.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// VolpianoInput
//----------------------------------------------------------------------------

VolpianoInput::VolpianoInput(Doc *doc) : Input(doc) {}

VolpianoInput::~VolpianoInput() {}

//////////////////////////////////////////////////////////////////////////

bool VolpianoInput::Import(const std::string &volpiano)
{
    m_doc->Reset();
    m_doc->SetType(Raw);
    // The mDiv
    Mdiv *mdiv = new Mdiv();
    mdiv->m_visibility = Visible;
    m_doc->AddChild(mdiv);
    // The score
    Score *score = new Score();
    mdiv->AddChild(score);
    // the section
    Section *section = new Section();
    score->AddChild(section);

    Staff *staff = new Staff(1);
    Measure *measure = new Measure(MEASURED, 1);
    Layer *layer = new Layer();
    layer->SetN(1);

    staff->AddChild(layer);
    measure->AddChild(staff);
    section->AddChild(measure);

    static std::map<char, std::pair<data_PITCHNAME, int>> notes = { { '9', { PITCHNAME_g, 3 } },
        { 'a', { PITCHNAME_a, 3 } }, { 'b', { PITCHNAME_b, 3 } }, { 'c', { PITCHNAME_c, 4 } },
        { 'd', { PITCHNAME_d, 4 } }, { 'e', { PITCHNAME_e, 4 } }, { 'f', { PITCHNAME_f, 4 } },
        { 'g', { PITCHNAME_g, 4 } }, { 'h', { PITCHNAME_a, 4 } }, { 'j', { PITCHNAME_b, 4 } },
        { 'k', { PITCHNAME_c, 5 } }, { 'l', { PITCHNAME_d, 5 } }, { 'm', { PITCHNAME_e, 5 } },
        { 'n', { PITCHNAME_f, 5 } }, { 'o', { PITCHNAME_g, 5 } }, { 'p', { PITCHNAME_a, 5 } },
        { 'q', { PITCHNAME_b, 5 } }, { 'r', { PITCHNAME_c, 6 } }, { 's', { PITCHNAME_d, 6 } } };

    static std::map<char, char> liquescents = { { ')', '9' }, { 'A', 'a' }, { 'B', 'b' }, { 'C', 'c' }, { 'D', 'd' },
        { 'E', 'e' }, { 'F', 'f' }, { 'G', 'g' }, { 'H', 'h' }, { 'J', 'j' }, { 'K', 'k' }, { 'L', 'l' }, { 'M', 'm' },
        { 'N', 'n' }, { 'O', 'o' }, { 'P', 'p' }, { 'Q', 'q' }, { 'R', 'r' }, { 'S', 's' } };

    for (char ch : volpiano) {
        if (notes.contains(ch)) {
            auto [pitch, oct] = notes.at(ch);
        }
        else if (liquescents.contains(ch)) {
            auto [pitch, oct] = notes.at(liquescents.at(ch));
        }
    }

    // add minimal scoreDef
    StaffGrp *staffGrp = new StaffGrp();
    StaffDef *staffDef = new StaffDef();
    staffDef->SetN(1);
    staffGrp->AddChild(staffDef);
    m_doc->GetFirstScoreDef()->AddChild(staffGrp);

    m_doc->ConvertToPageBasedDoc();

    return true;
}

} // namespace vrv
