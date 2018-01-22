/////////////////////////////////////////////////////////////////////////////
// Name:        pghead.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "pghead.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "rend.h"
#include "text.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// PgHead
//----------------------------------------------------------------------------

PgHead::PgHead() : RunningElement("pghead-")
{
    Reset();
}

PgHead::~PgHead()
{
}

void PgHead::Reset()
{
    RunningElement::Reset();
}
    
bool PgHead::GenerateFromMEIHeader(pugi::xml_document &header)
{
    Rend *composerRend = new Rend();
    Text *composerText = new Text();
    std::string cmp = "Frédéric Chopin";
    composerText->SetText(UTF8to16(cmp));
    composerRend->AddChild(composerText);
    this->AddChild(composerRend);
    pugi::xpath_node build_tool = header.select_single_node("//persName[@role=\"composer\"]");
    if (build_tool) {
        std::string composer = build_tool.node().text().as_string();
        LogMessage(composer.c_str());
    }
    
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------
    
} // namespace vrv
