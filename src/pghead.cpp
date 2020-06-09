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

#include "lb.h"
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

PgHead::~PgHead() {}

void PgHead::Reset()
{
    RunningElement::Reset();
}

bool PgHead::GenerateFromMEIHeader(pugi::xml_document &header)
{
    pugi::xpath_node node;
    pugi::xpath_node_set nodeSet;
    data_FONTSIZE fs;

    // title
    nodeSet = header.select_nodes("//fileDesc/titleStmt/title[text()]");
    if (!nodeSet.empty()) {
        Rend *titleRend = new Rend();
        titleRend->SetHalign(HORIZONTALALIGNMENT_center);
        titleRend->SetValign(VERTICALALIGNMENT_middle);
        titleRend->SetLabel("title");
        for (auto titleNode : nodeSet) {
            Rend *rend = new Rend();
            if (titleNode == nodeSet.first()) {
                fs.SetTerm(FONTSIZETERM_x_large);
                rend->SetFontsize(fs);
            }
            else {
                titleRend->AddChild(new Lb());
                fs.SetTerm(FONTSIZETERM_small);
                rend->SetFontsize(fs);
            }
            Text *text = new Text();
            text->SetText(UTF8to16(titleNode.node().text().as_string()));
            rend->SetLang(titleNode.node().attribute("xml:lang").as_string());
            rend->AddChild(text);
            titleRend->AddChild(rend);
        }
        this->AddChild(titleRend);
    }

    nodeSet = header.select_nodes(
        "//fileDesc/titleStmt/respStmt/persName[contains('lyricist translator composer harmonizer arranger', @role)]");
    for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it) {
        node = *it;
        Rend *personRend = new Rend();
        std::string role = node.node().attribute("role").as_string();
        if (role == "lyricist" || role == "translator") {
            personRend->SetHalign(HORIZONTALALIGNMENT_left);
        }
        else {
            // composer, harmonizer, and arranger get placed on the right side.
            personRend->SetHalign(HORIZONTALALIGNMENT_right);
        }
        personRend->SetValign(VERTICALALIGNMENT_bottom);
        personRend->SetLabel(role);
        Text *personText = new Text();
        personText->SetText(UTF8to16(node.node().text().as_string()));
        personRend->SetLang(node.node().attribute("xml:lang").as_string());
        personRend->AddChild(personText);
        this->AddChild(personRend);
    }

    return true;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

} // namespace vrv
