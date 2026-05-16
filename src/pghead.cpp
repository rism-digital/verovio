/////////////////////////////////////////////////////////////////////////////
// Name:        pghead.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "pghead.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "doc.h"
#include "functor.h"
#include "lb.h"
#include "rend.h"
#include "text.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// PgHead
//----------------------------------------------------------------------------

static const ClassRegistrar<PgHead> s_factory("pgHead", PGHEAD);

PgHead::PgHead() : RunningElement(PGHEAD)
{
    this->Reset();
}

PgHead::~PgHead() {}

void PgHead::Reset()
{
    RunningElement::Reset();
}

int PgHead::GetTotalHeight(const Doc *doc) const
{
    assert(doc);

    int height = this->GetContentHeight();
    if (height > 0) {
        const int unit = doc->GetDrawingUnit(100);
        height += doc->GetOptions()->m_bottomMarginPgHead.GetValue() * unit;
    }
    return height;
}

bool PgHead::GenerateFromMEIHeader(const pugi::xml_document &header)
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
            text->SetText(UTF8to32(titleNode.node().text().as_string()));
            rend->SetLang(titleNode.node().attribute("xml:lang").as_string());
            rend->AddChild(text);
            titleRend->AddChild(rend);
        }
        this->AddChild(titleRend);
    }

    nodeSet
        = header.select_nodes("//fileDesc/titleStmt/composer|arranger|lyricist|respStmt/persName[contains('lyricist "
                              "translator composer harmonizer arranger', @role)]");

    for (auto node : nodeSet) {
        Rend *personRend = new Rend();
        std::string role = node.node().attribute("role").as_string();
        std::string name = node.node().name();
        if (name == "lyricist" || role == "lyricist" || role == "translator") {
            personRend->SetHalign(HORIZONTALALIGNMENT_left);
        }
        else {
            // composer, harmonizer, and arranger get placed on the right side.
            personRend->SetHalign(HORIZONTALALIGNMENT_right);
        }
        personRend->SetValign(VERTICALALIGNMENT_bottom);
        personRend->SetLabel(role);
        Text *personText = new Text();
        personText->SetText(UTF8to32(node.node().text().as_string()));
        personRend->SetLang(node.node().attribute("xml:lang").as_string());
        personRend->AddChild(personText);
        this->AddChild(personRend);
    }

    return true;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

FunctorCode PgHead::Accept(Functor &functor)
{
    return functor.VisitPgHead(this);
}

FunctorCode PgHead::Accept(ConstFunctor &functor) const
{
    return functor.VisitPgHead(this);
}

FunctorCode PgHead::AcceptEnd(Functor &functor)
{
    return functor.VisitPgHeadEnd(this);
}

FunctorCode PgHead::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitPgHeadEnd(this);
}

} // namespace vrv
