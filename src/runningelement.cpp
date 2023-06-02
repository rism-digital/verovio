/////////////////////////////////////////////////////////////////////////////
// Name:        runningelement.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "runningelement.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "doc.h"
#include "editorial.h"
#include "fig.h"
#include "functor.h"
#include "num.h"
#include "page.h"
#include "rend.h"
#include "staff.h"
#include "svg.h"
#include "text.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// RunningElement
//----------------------------------------------------------------------------

RunningElement::RunningElement() : TextLayoutElement(RUNNING_ELEMENT, "re"), AttHorizontalAlign()
{
    this->RegisterAttClass(ATT_HORIZONTALALIGN);

    this->Reset();
}

RunningElement::RunningElement(ClassId classId) : TextLayoutElement(classId, "re"), AttHorizontalAlign()
{
    this->RegisterAttClass(ATT_HORIZONTALALIGN);

    this->Reset();
}

RunningElement::RunningElement(ClassId classId, const std::string &classIdStr)
    : TextLayoutElement(classId, classIdStr), AttHorizontalAlign()
{
    this->RegisterAttClass(ATT_HORIZONTALALIGN);

    this->Reset();
}

RunningElement::~RunningElement() {}

void RunningElement::Reset()
{
    TextLayoutElement::Reset();
    this->ResetHorizontalAlign();

    m_isGenerated = false;

    m_drawingPage = NULL;
    m_drawingYRel = 0;

    this->ResetDrawingScaling();
}

int RunningElement::GetDrawingX() const
{
    if (!m_drawingPage) return 0;

    /*
    if (this->GetHalign() == HORIZONTALALIGNMENT_left) {
        return 0;
    }
    else if (this->GetHalign() == HORIZONTALALIGNMENT_center) {
        return m_drawingPage->GetContentWidth() / 2;
    }
    else if (this->GetHalign() == HORIZONTALALIGNMENT_right) {
        return m_drawingPage->GetContentWidth();
    }
    */

    return 0;
}

int RunningElement::GetDrawingY() const
{
    m_cachedDrawingY = 0;
    return m_drawingYRel;
}

void RunningElement::SetDrawingYRel(int drawingYRel)
{
    this->ResetCachedDrawingY();
    m_drawingYRel = drawingYRel;
}

int RunningElement::GetWidth() const
{
    if (!m_drawingPage) return 0;

    Doc *doc = vrv_cast<Doc *>(m_drawingPage->GetFirstAncestor(DOC));
    if (!doc) return 0;

    return (doc->m_drawingPageContentWidth);
    // return m_drawingPage->GetContentWidth();
}

void RunningElement::SetDrawingPage(Page *page)
{

    this->ResetList(this);

    this->ResetCachedDrawingX();
    m_drawingPage = page;

    if (page) {
        this->SetCurrentPageNum(page);
    }
}

int RunningElement::GetAlignmentPos(data_HORIZONTALALIGNMENT h, data_VERTICALALIGNMENT v) const
{
    int pos = 0;
    switch (h) {
        case (HORIZONTALALIGNMENT_left): break;
        case (HORIZONTALALIGNMENT_center): pos += POSITION_CENTER; break;
        case (HORIZONTALALIGNMENT_right): pos += POSITION_RIGHT; break;
        default: pos += POSITION_LEFT; break;
    }
    switch (v) {
        case (VERTICALALIGNMENT_top): break;
        case (VERTICALALIGNMENT_middle): pos += POSITION_MIDDLE; break;
        case (VERTICALALIGNMENT_bottom): pos += POSITION_BOTTOM; break;
        default: pos += POSITION_MIDDLE; break;
    }
    return pos;
}

void RunningElement::SetCurrentPageNum(const Page *currentPage)
{
    assert(currentPage);

    int currentNum = currentPage->GetIdx() + 1;

    Num *num = vrv_cast<Num *>(this->FindDescendantByType(NUM));
    if (!num || (num->GetLabel() != "page")) return;

    Text *text = vrv_cast<Text *>(num->FindDescendantByType(TEXT));
    if (!text || (text->GetText() != U"#")) return;

    Text *currentText = num->GetCurrentText();
    assert(currentText);

    currentText->SetText(UTF8to32(StringFormat("%d", currentNum)));
}

void RunningElement::LoadFooter(const Doc *doc)
{
    Fig *fig = new Fig();
    Svg *svg = new Svg();

    const Resources &resources = doc->GetResources();
    const std::string footerPath = resources.GetPath() + "/footer.svg";
    pugi::xml_document footerDoc;
    footerDoc.load_file(footerPath.c_str());
    svg->Set(footerDoc.first_child());
    fig->AddChild(svg);
    fig->SetHalign(HORIZONTALALIGNMENT_center);
    fig->SetValign(VERTICALALIGNMENT_bottom);
    this->AddChild(fig);
}

void RunningElement::AddPageNum(data_HORIZONTALALIGNMENT halign, data_VERTICALALIGNMENT valign)
{
    Rend *rend = new Rend();
    data_FONTSIZE fs;
    fs.SetTerm(FONTSIZETERM_small);
    rend->SetFontsize(fs);
    rend->SetHalign(halign);
    rend->SetValign(valign);
    Text *dash1 = new Text();
    dash1->SetText(U"– ");
    Num *num = new Num();
    num->SetLabel("page");
    Text *text = new Text();
    text->SetText(U"#");
    Text *dash2 = new Text();
    dash2->SetText(U" –");

    num->AddChild(text);
    rend->AddChild(dash1);
    rend->AddChild(num);
    rend->AddChild(dash2);
    this->AddChild(rend);
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

FunctorCode RunningElement::Accept(Functor &functor)
{
    return functor.VisitRunningElement(this);
}

FunctorCode RunningElement::Accept(ConstFunctor &functor) const
{
    return functor.VisitRunningElement(this);
}

FunctorCode RunningElement::AcceptEnd(Functor &functor)
{
    return functor.VisitRunningElementEnd(this);
}

FunctorCode RunningElement::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitRunningElementEnd(this);
}

} // namespace vrv
