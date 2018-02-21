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
    pugi::xpath_node_set::const_iterator iter;
    data_FONTSIZE fs;

    // Title
    nodeSet = header.select_nodes("//fileDesc/titleStmt/title");
    if (!nodeSet.empty()) {
        Rend *titleRend = new Rend();
        titleRend->SetHalign(HORIZONTALALIGNMENT_center);
        titleRend->SetValign(VERTICALALIGNMENT_middle);
        for (iter = nodeSet.begin(); iter != nodeSet.end(); iter++) {
            Rend *rend = new Rend();
            if (iter == nodeSet.begin()) {
                fs.SetTerm(FONTSIZETERM_x_large);
                rend->SetFontsize(fs);
            }
            else {
                titleRend->AddChild(new Lb());
                fs.SetTerm(FONTSIZETERM_small);
                rend->SetFontsize(fs);
            }
            Text *text = new Text();
            text->SetText(UTF8to16(iter->node().text().as_string()));
            rend->AddChild(text);
            titleRend->AddChild(rend);
        }
        this->AddChild(titleRend);
    }

    // Composer
    node = header.select_single_node("//fileDesc/titleStmt/respStmt/persName[@role=\"composer\"]");
    if (node) {
        Rend *composerRend = new Rend();
        composerRend->SetHalign(HORIZONTALALIGNMENT_right);
        composerRend->SetValign(VERTICALALIGNMENT_bottom);
        Text *composerText = new Text();
        composerText->SetText(UTF8to16(node.node().text().as_string()));
        composerRend->AddChild(composerText);
        this->AddChild(composerRend);
    }

    // Lyricist
    node = header.select_single_node("//fileDesc/titleStmt/respStmt/persName[@role=\"lyricist\"]");
    if (node) {
        Rend *lyricistRend = new Rend();
        lyricistRend->SetHalign(HORIZONTALALIGNMENT_left);
        lyricistRend->SetValign(VERTICALALIGNMENT_bottom);
        Text *lyricistText = new Text();
        lyricistText->SetText(UTF8to16(node.node().text().as_string()));
        lyricistRend->AddChild(lyricistText);
        this->AddChild(lyricistRend);
    }

    return true;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

} // namespace vrv
