/////////////////////////////////////////////////////////////////////////////
// Name:        iomei.cpp
// Author:      Laurent Pugin
// Created:     2008
// Copyright (c) Laurent Pugin. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "iomei.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <iostream>

//----------------------------------------------------------------------------

#include "accid.h"
#include "anchoredtext.h"
#include "artic.h"
#include "beam.h"
#include "boundary.h"
#include "breath.h"
#include "chord.h"
#include "clef.h"
#include "custos.h"
#include "dir.h"
#include "dot.h"
#include "dynam.h"
#include "editorial.h"
#include "ending.h"
#include "expansion.h"
#include "fb.h"
#include "fermata.h"
#include "functorparams.h"
#include "hairpin.h"
#include "harm.h"
#include "keysig.h"
#include "label.h"
#include "layer.h"
#include "ligature.h"
#include "measure.h"
#include "mensur.h"
#include "metersig.h"
#include "mordent.h"
#include "mrest.h"
#include "multirest.h"
#include "note.h"
#include "octave.h"
#include "page.h"
#include "pedal.h"
#include "proport.h"
#include "rest.h"
#include "rpt.h"
#include "score.h"
#include "section.h"
#include "slur.h"
#include "space.h"
#include "staff.h"
#include "syl.h"
#include "system.h"
#include "tempo.h"
#include "text.h"
#include "tie.h"
#include "trill.h"
#include "tuplet.h"
#include "turn.h"
#include "verse.h"
#include "vrv.h"

namespace vrv {

std::vector<std::string> MeiInput::s_editorialElementNames = { "abbr", "add", "app", "annot", "choice", "corr",
    "damage", "del", "expan", "orig", "reg", "restore", "sic", "supplied", "unclear" };

//----------------------------------------------------------------------------
// MeiOutput
//----------------------------------------------------------------------------

MeiOutput::MeiOutput(Doc *doc, std::string filename) : FileOutputStream(doc)
{
    m_filename = filename;
    m_writeToStreamString = false;
    m_page = -1;
    m_scoreBasedMEI = false;
}

MeiOutput::~MeiOutput()
{
}

bool MeiOutput::ExportFile()
{
    try {
        pugi::xml_document meiDoc;

        if (m_page < 0) {
            pugi::xml_node decl = meiDoc.prepend_child(pugi::node_declaration);
            decl.append_attribute("version") = "1.0";
            decl.append_attribute("encoding") = "UTF-8";

            // schema processing instruction
            decl = meiDoc.append_child(pugi::node_declaration);
            decl.set_name("xml-model");
            decl.append_attribute("href") = "http://music-encoding.org/schema/4.0.0/mei-all.rng";
            decl.append_attribute("type") = "application/xml";
            decl.append_attribute("schematypens") = "http://relaxng.org/ns/structure/1.0";

            // schematron processing instruction
            decl = meiDoc.append_child(pugi::node_declaration);
            decl.set_name("xml-model");
            decl.append_attribute("href") = "http://music-encoding.org/schema/4.0.0/mei-all.rng";
            decl.append_attribute("type") = "application/xml";
            decl.append_attribute("schematypens") = "http://purl.oclc.org/dsdl/schematron";

            m_mei = meiDoc.append_child("mei");
            m_mei.append_attribute("xmlns") = "http://www.music-encoding.org/ns/mei";
            m_mei.append_attribute("meiversion") = "4.0.0";

            // this starts the call of all the functors
            m_doc->Save(this);
        }
        else {
            if (m_page >= m_doc->GetPageCount()) {
                LogError("Page %d does not exist", m_page);
                return false;
            }
            Page *page = dynamic_cast<Page *>(m_doc->GetChild(m_page));
            assert(page);
            if (m_scoreBasedMEI) {
                m_currentNode = meiDoc.append_child("score");
                m_currentNode = m_currentNode.append_child("section");
                m_nodeStack.push_back(m_currentNode);
                // First save the main scoreDef
                m_doc->m_scoreDef.Save(this);
            }
            else {
                m_currentNode = meiDoc.append_child("pages");
            }

            page->Save(this);
        }
        if (m_writeToStreamString) {
            // meiDoc.save(m_streamStringOutput, "    ", pugi::format_default | pugi::format_no_escapes);
            meiDoc.save(m_streamStringOutput, "    ");
        }
        else {
            // meiDoc.save_file(m_filename.c_str(), "    ", pugi::format_default | pugi::format_no_escapes);
            meiDoc.save_file(m_filename.c_str(), "    ");
        }
    }
    catch (char *str) {
        LogError("%s", str);
        return false;
    }

    return true;
}

std::string MeiOutput::GetOutput(int page)
{
    m_writeToStreamString = true;
    m_page = page;
    this->ExportFile();
    m_writeToStreamString = false;
    m_page = -1;

    return m_streamStringOutput.str();
}

bool MeiOutput::WriteObject(Object *object)
{
    if (object->HasComment()) {
        m_currentNode.append_child(pugi::node_comment).set_value(object->GetComment().c_str());
    }

    // Main containers
    if (object->Is(DOC)) {
        WriteMeiDoc(dynamic_cast<Doc *>(object));
        m_nodeStack.push_back(m_currentNode);
        return true;
    }

    if (object->Is(PAGE)) {
        if (!m_scoreBasedMEI) {
            m_currentNode = m_currentNode.append_child("page");
            WriteMeiPage(m_currentNode, dynamic_cast<Page *>(object));
        }
    }
    else if (object->Is(SYSTEM)) {
        if (!m_scoreBasedMEI) {
            m_currentNode = m_currentNode.append_child("system");
            WriteMeiSystem(m_currentNode, dynamic_cast<System *>(object));
        }
    }

    // System boundaries
    else if (object->Is(ENDING)) {
        m_currentNode = m_currentNode.append_child("ending");
        WriteMeiEnding(m_currentNode, dynamic_cast<Ending *>(object));
    }
    else if (object->Is(EXPANSION)) {
        m_currentNode = m_currentNode.append_child("expansion");
        WriteMeiExpansion(m_currentNode, dynamic_cast<Expansion *>(object));
    }
    else if (object->Is(PB)) {
        m_currentNode = m_currentNode.append_child("pb");
        WriteMeiPb(m_currentNode, dynamic_cast<Pb *>(object));
    }
    else if (object->Is(SB)) {
        m_currentNode = m_currentNode.append_child("sb");
        WriteMeiSb(m_currentNode, dynamic_cast<Sb *>(object));
    }
    else if (object->Is(SECTION)) {
        m_currentNode = m_currentNode.append_child("section");
        WriteMeiSection(m_currentNode, dynamic_cast<Section *>(object));
    }

    // ScoreDef related
    else if (object->Is(LABEL)) {
        m_currentNode = m_currentNode.append_child("label");
        WriteMeiLabel(m_currentNode, dynamic_cast<Label *>(object));
    }
    else if (object->Is(LABELABBR)) {
        m_currentNode = m_currentNode.append_child("labelAbbr");
        WriteMeiLabelAbbr(m_currentNode, dynamic_cast<LabelAbbr *>(object));
    }
    else if (object->Is(SCOREDEF)) {
        m_currentNode = m_currentNode.append_child("scoreDef");
        WriteMeiScoreDef(m_currentNode, dynamic_cast<ScoreDef *>(object));
    }
    else if (object->Is(STAFFGRP)) {
        m_currentNode = m_currentNode.append_child("staffGrp");
        WriteMeiStaffGrp(m_currentNode, dynamic_cast<StaffGrp *>(object));
    }
    else if (object->Is(STAFFDEF)) {
        m_currentNode = m_currentNode.append_child("staffDef");
        WriteMeiStaffDef(m_currentNode, dynamic_cast<StaffDef *>(object));
    }
    else if (object->Is(MEASURE)) {
        m_currentNode = m_currentNode.append_child("measure");
        WriteMeiMeasure(m_currentNode, dynamic_cast<Measure *>(object));
    }
    else if (object->Is(STAFF)) {
        m_currentNode = m_currentNode.append_child("staff");
        WriteMeiStaff(m_currentNode, dynamic_cast<Staff *>(object));
    }
    else if (object->Is(LAYER)) {
        m_currentNode = m_currentNode.append_child("layer");
        WriteMeiLayer(m_currentNode, dynamic_cast<Layer *>(object));
    }

    // Measure elements
    else if (object->Is(ANCHORED_TEXT)) {
        m_currentNode = m_currentNode.append_child("anchoredText");
        WriteMeiAnchoredText(m_currentNode, dynamic_cast<AnchoredText *>(object));
    }
    else if (object->Is(BREATH)) {
        m_currentNode = m_currentNode.append_child("breath");
        WriteMeiBreath(m_currentNode, dynamic_cast<Breath *>(object));
    }
    else if (object->Is(DIR)) {
        m_currentNode = m_currentNode.append_child("dir");
        WriteMeiDir(m_currentNode, dynamic_cast<Dir *>(object));
    }
    else if (object->Is(DYNAM)) {
        m_currentNode = m_currentNode.append_child("dynam");
        WriteMeiDynam(m_currentNode, dynamic_cast<Dynam *>(object));
    }
    else if (object->Is(FERMATA)) {
        m_currentNode = m_currentNode.append_child("fermata");
        WriteMeiFermata(m_currentNode, dynamic_cast<Fermata *>(object));
    }
    else if (object->Is(HAIRPIN)) {
        m_currentNode = m_currentNode.append_child("hairpin");
        WriteMeiHairpin(m_currentNode, dynamic_cast<Hairpin *>(object));
    }
    else if (object->Is(HARM)) {
        m_currentNode = m_currentNode.append_child("harm");
        WriteMeiHarm(m_currentNode, dynamic_cast<Harm *>(object));
    }
    else if (object->Is(MORDENT)) {
        m_currentNode = m_currentNode.append_child("mordent");
        WriteMeiMordent(m_currentNode, dynamic_cast<Mordent *>(object));
    }
    else if (object->Is(OCTAVE)) {
        m_currentNode = m_currentNode.append_child("octave");
        WriteMeiOctave(m_currentNode, dynamic_cast<Octave *>(object));
    }
    else if (object->Is(PEDAL)) {
        m_currentNode = m_currentNode.append_child("pedal");
        WriteMeiPedal(m_currentNode, dynamic_cast<Pedal *>(object));
    }
    else if (object->Is(SLUR)) {
        m_currentNode = m_currentNode.append_child("slur");
        WriteMeiSlur(m_currentNode, dynamic_cast<Slur *>(object));
    }
    else if (object->Is(TEMPO)) {
        m_currentNode = m_currentNode.append_child("tempo");
        WriteMeiTempo(m_currentNode, dynamic_cast<Tempo *>(object));
    }
    else if (object->Is(TIE)) {
        m_currentNode = m_currentNode.append_child("tie");
        WriteMeiTie(m_currentNode, dynamic_cast<Tie *>(object));
    }
    else if (object->Is(TRILL)) {
        m_currentNode = m_currentNode.append_child("trill");
        WriteMeiTrill(m_currentNode, dynamic_cast<Trill *>(object));
    }
    else if (object->Is(TURN)) {
        m_currentNode = m_currentNode.append_child("turn");
        WriteMeiTurn(m_currentNode, dynamic_cast<Turn *>(object));
    }

    // Layer elements
    else if (object->Is(ACCID)) {
        // Do not add a node for object representing an attribute
        if (!object->IsAttribute()) m_currentNode = m_currentNode.append_child("accid");
        WriteMeiAccid(m_currentNode, dynamic_cast<Accid *>(object));
    }
    else if (object->Is(ARTIC)) {
        // Do not add a node for object representing an attribute
        if (!object->IsAttribute()) m_currentNode = m_currentNode.append_child("artic");
        WriteMeiArtic(m_currentNode, dynamic_cast<Artic *>(object));
    }
    else if (object->Is(BARLINE)) {
        m_currentNode = m_currentNode.append_child("barLine");
        WriteMeiBarLine(m_currentNode, dynamic_cast<BarLine *>(object));
    }
    else if (object->Is(BEAM)) {
        m_currentNode = m_currentNode.append_child("beam");
        WriteMeiBeam(m_currentNode, dynamic_cast<Beam *>(object));
    }
    else if (object->Is(BEATRPT)) {
        m_currentNode = m_currentNode.append_child("beatRpt");
        WriteMeiBeatRpt(m_currentNode, dynamic_cast<BeatRpt *>(object));
    }
    else if (object->Is(BTREM)) {
        m_currentNode = m_currentNode.append_child("bTrem");
        WriteMeiBTrem(m_currentNode, dynamic_cast<BTrem *>(object));
    }
    else if (object->Is(CHORD)) {
        m_currentNode = m_currentNode.append_child("chord");
        WriteMeiChord(m_currentNode, dynamic_cast<Chord *>(object));
    }
    else if (object->Is(CLEF)) {
        m_currentNode = m_currentNode.append_child("clef");
        WriteMeiClef(m_currentNode, dynamic_cast<Clef *>(object));
    }
    else if (object->Is(CUSTOS)) {
        m_currentNode = m_currentNode.append_child("custos");
        WriteMeiCustos(m_currentNode, dynamic_cast<Custos *>(object));
    }
    else if (object->Is(DOT)) {
        m_currentNode = m_currentNode.append_child("dot");
        WriteMeiDot(m_currentNode, dynamic_cast<Dot *>(object));
    }
    else if (object->Is(FTREM)) {
        m_currentNode = m_currentNode.append_child("fTrem");
        WriteMeiFTrem(m_currentNode, dynamic_cast<FTrem *>(object));
    }
    else if (object->Is(KEYSIG)) {
        m_currentNode = m_currentNode.append_child("keySig");
        WriteMeiKeySig(m_currentNode, dynamic_cast<KeySig *>(object));
    }
    else if (object->Is(LIGATURE)) {
        LogError("WriteMeiLigature not implemented. (MeiOutput::WriteObject)");
        // m_currentNode = m_currentNode.append_child("ligature");
        // WriteMeiLigature(m_currentNode, dynamic_cast<KeySig *>(object));
    }
    else if (object->Is(MENSUR)) {
        m_currentNode = m_currentNode.append_child("mensur");
        WriteMeiMensur(m_currentNode, dynamic_cast<Mensur *>(object));
    }
    else if (object->Is(METERSIG)) {
        m_currentNode = m_currentNode.append_child("meterSig");
        WriteMeiMeterSig(m_currentNode, dynamic_cast<MeterSig *>(object));
    }
    else if (object->Is(MREST)) {
        m_currentNode = m_currentNode.append_child("mRest");
        WriteMeiMRest(m_currentNode, dynamic_cast<MRest *>(object));
    }
    else if (object->Is(MRPT)) {
        m_currentNode = m_currentNode.append_child("mRpt");
        WriteMeiMRpt(m_currentNode, dynamic_cast<MRpt *>(object));
    }
    else if (object->Is(MRPT2)) {
        m_currentNode = m_currentNode.append_child("mRpt2");
        WriteMeiMRpt2(m_currentNode, dynamic_cast<MRpt2 *>(object));
    }
    else if (object->Is(MULTIREST)) {
        m_currentNode = m_currentNode.append_child("multiRest");
        WriteMeiMultiRest(m_currentNode, dynamic_cast<MultiRest *>(object));
    }
    else if (object->Is(MULTIRPT)) {
        m_currentNode = m_currentNode.append_child("multiRpt");
        WriteMeiMultiRpt(m_currentNode, dynamic_cast<MultiRpt *>(object));
    }
    else if (object->Is(NOTE)) {
        m_currentNode = m_currentNode.append_child("note");
        WriteMeiNote(m_currentNode, dynamic_cast<Note *>(object));
    }
    else if (object->Is(PROPORT)) {
        m_currentNode = m_currentNode.append_child("proport");
        WriteMeiProport(m_currentNode, dynamic_cast<Proport *>(object));
    }
    else if (object->Is(REST)) {
        m_currentNode = m_currentNode.append_child("rest");
        WriteMeiRest(m_currentNode, dynamic_cast<Rest *>(object));
    }
    else if (object->Is(SPACE)) {
        m_currentNode = m_currentNode.append_child("space");
        WriteMeiSpace(m_currentNode, dynamic_cast<Space *>(object));
    }
    else if (object->Is(SYL)) {
        m_currentNode = m_currentNode.append_child("syl");
        WriteMeiSyl(m_currentNode, dynamic_cast<Syl *>(object));
    }
    else if (object->Is(TUPLET)) {
        m_currentNode = m_currentNode.append_child("tuplet");
        WriteMeiTuplet(m_currentNode, dynamic_cast<Tuplet *>(object));
    }
    else if (object->Is(VERSE)) {
        m_currentNode = m_currentNode.append_child("verse");
        WriteMeiVerse(m_currentNode, dynamic_cast<Verse *>(object));
    }

    // Text elements
    else if (object->Is(FIGURE)) {
        m_currentNode = m_currentNode.append_child("f");
        WriteMeiF(m_currentNode, dynamic_cast<F *>(object));
    }
    else if (object->Is(FB)) {
        m_currentNode = m_currentNode.append_child("fb");
        WriteMeiFb(m_currentNode, dynamic_cast<Fb *>(object));
    }
    else if (object->Is(REND)) {
        m_currentNode = m_currentNode.append_child("rend");
        WriteMeiRend(m_currentNode, dynamic_cast<Rend *>(object));
    }
    else if (object->Is(TEXT)) {
        WriteMeiText(m_currentNode, dynamic_cast<Text *>(object));
    }

    // Editorial markup
    else if (object->Is(ABBR)) {
        m_currentNode = m_currentNode.append_child("abbr");
        WriteMeiAbbr(m_currentNode, dynamic_cast<Abbr *>(object));
    }
    else if (object->Is(ADD)) {
        m_currentNode = m_currentNode.append_child("add");
        WriteMeiAdd(m_currentNode, dynamic_cast<Add *>(object));
    }
    else if (object->Is(ANNOT)) {
        m_currentNode = m_currentNode.append_child("annot");
        WriteMeiAnnot(m_currentNode, dynamic_cast<Annot *>(object));
    }
    else if (object->Is(APP)) {
        m_currentNode = m_currentNode.append_child("app");
        WriteMeiApp(m_currentNode, dynamic_cast<App *>(object));
    }
    else if (object->Is(CHOICE)) {
        m_currentNode = m_currentNode.append_child("choice");
        WriteMeiChoice(m_currentNode, dynamic_cast<Choice *>(object));
    }
    else if (object->Is(CORR)) {
        m_currentNode = m_currentNode.append_child("corr");
        WriteMeiCorr(m_currentNode, dynamic_cast<Corr *>(object));
    }
    else if (object->Is(DAMAGE)) {
        m_currentNode = m_currentNode.append_child("damage");
        WriteMeiDamage(m_currentNode, dynamic_cast<Damage *>(object));
    }
    else if (object->Is(DEL)) {
        m_currentNode = m_currentNode.append_child("del");
        WriteMeiDel(m_currentNode, dynamic_cast<Del *>(object));
    }
    else if (object->Is(EXPAN)) {
        m_currentNode = m_currentNode.append_child("epxan");
        WriteMeiExpan(m_currentNode, dynamic_cast<Expan *>(object));
    }
    else if (object->Is(LEM)) {
        m_currentNode = m_currentNode.append_child("lem");
        WriteMeiLem(m_currentNode, dynamic_cast<Lem *>(object));
    }
    else if (object->Is(ORIG)) {
        m_currentNode = m_currentNode.append_child("orig");
        WriteMeiOrig(m_currentNode, dynamic_cast<Orig *>(object));
    }
    else if (object->Is(RDG)) {
        m_currentNode = m_currentNode.append_child("rdg");
        WriteMeiRdg(m_currentNode, dynamic_cast<Rdg *>(object));
    }
    else if (object->Is(REG)) {
        m_currentNode = m_currentNode.append_child("reg");
        WriteMeiReg(m_currentNode, dynamic_cast<Reg *>(object));
    }
    else if (object->Is(RESTORE)) {
        m_currentNode = m_currentNode.append_child("restore");
        WriteMeiRestore(m_currentNode, dynamic_cast<Restore *>(object));
    }
    else if (object->Is(SIC)) {
        m_currentNode = m_currentNode.append_child("sic");
        WriteMeiSic(m_currentNode, dynamic_cast<Sic *>(object));
    }
    else if (object->Is(SUPPLIED)) {
        m_currentNode = m_currentNode.append_child("supplied");
        WriteMeiSupplied(m_currentNode, dynamic_cast<Supplied *>(object));
    }
    else if (object->Is(UNCLEAR)) {
        m_currentNode = m_currentNode.append_child("unclear");
        WriteMeiUnclear(m_currentNode, dynamic_cast<Unclear *>(object));
    }

    // BoundaryEnd - nothing to add - only
    else if (object->Is(BOUNDARY_END)) {
        if (m_scoreBasedMEI)
            return true;
        else {
            m_currentNode = m_currentNode.append_child("boundaryEnd");
            WriteMeiBoundaryEnd(m_currentNode, dynamic_cast<BoundaryEnd *>(object));
        }
    }

    else {
        // Missing output method for the class
        LogError("Output method missing for '%s'", object->GetClassName().c_str());
        assert(false); // let's make it stop because this should not happen
    }

    // Object representing an attribute have no node to push
    if (!object->IsAttribute()) m_nodeStack.push_back(m_currentNode);

    return true;
}

bool MeiOutput::WriteObjectEnd(Object *object)
{
    if (m_scoreBasedMEI && object->IsBoundaryElement()) {
        BoundaryStartInterface *interface = dynamic_cast<BoundaryStartInterface *>(object);
        assert(interface);
        if (interface->IsBoundary()) return true;
    }
    // Object representing an attribute have no node to pop
    else if (object->IsAttribute()) {
        return true;
    }
    else if (m_scoreBasedMEI && (object->Is(SYSTEM))) {
        return true;
    }
    else if (m_scoreBasedMEI && (object->Is(PAGE))) {
        return true;
    }
    m_nodeStack.pop_back();
    m_currentNode = m_nodeStack.back();

    return true;
}

std::string MeiOutput::UuidToMeiStr(Object *element)
{
    std::string out = element->GetUuid();
    // LogDebug("uuid: %s", out.c_str());
    return out;
}

void MeiOutput::WriteXmlId(pugi::xml_node currentNode, Object *object)
{
    currentNode.append_attribute("xml:id") = UuidToMeiStr(object).c_str();
}

bool MeiOutput::WriteMeiDoc(Doc *doc)
{
    assert(doc);
    assert(!m_mei.empty());

    // ---- header ----

    if (m_doc->m_header.first_child()) {
        m_mei.append_copy(m_doc->m_header.first_child());
    }
    else {
        pugi::xml_node meiHead = m_mei.append_child("meiHead");
        pugi::xml_node fileDesc = meiHead.append_child("fileDesc");
        pugi::xml_node titleStmt = fileDesc.append_child("titleStmt");
        titleStmt.append_child("title");
        pugi::xml_node pubStmt = fileDesc.append_child("pubStmt");
        pugi::xml_node date = pubStmt.append_child("date");

        pugi::xml_node encodingDesc = meiHead.append_child("encodingDesc");
        pugi::xml_node projectDesc = encodingDesc.append_child("projectDesc");
        pugi::xml_node p1 = projectDesc.append_child("p");
        p1.append_child(pugi::node_pcdata)
            .set_value(StringFormat("Encoded with Verovio version %s", GetVersion().c_str()).c_str());

        // date
        time_t t = time(0); // get time now
        struct tm *now = localtime(&t);
        std::string dateStr = StringFormat("%d-%02d-%02d %02d:%02d:%02d", now->tm_year + 1900, now->tm_mon + 1,
            now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
        date.append_child(pugi::node_pcdata).set_value(dateStr.c_str());
    }

    // ---- music ----

    pugi::xml_node music = m_mei.append_child("music");
    pugi::xml_node body = music.append_child("body");
    pugi::xml_node mdiv = body.append_child("mdiv");

    if (m_scoreBasedMEI) {
        m_currentNode = mdiv.append_child("score");
        m_nodeStack.push_back(m_currentNode);
        // First save the main scoreDef
        m_doc->m_scoreDef.Save(this);
    }
    else {
        // element to place the pages
        m_currentNode = mdiv.append_child("pages");
        m_currentNode.append_attribute("type") = DocTypeToStr(m_doc->GetType()).c_str();
        m_currentNode.append_child(pugi::node_comment).set_value("Coordinates in MEI axis direction");
    }

    return true;
}

void MeiOutput::WriteMeiPage(pugi::xml_node currentNode, Page *page)
{
    assert(page);

    WriteXmlId(currentNode, page);
    // size and margins but only if any - we rely on page.height only to check this
    if (page->m_pageHeight != -1) {
        currentNode.append_attribute("page.width") = StringFormat("%d", page->m_pageWidth).c_str();
        currentNode.append_attribute("page.height") = StringFormat("%d", page->m_pageHeight).c_str();
        currentNode.append_attribute("page.leftmar") = StringFormat("%d", page->m_pageLeftMar).c_str();
        currentNode.append_attribute("page.rightmar") = StringFormat("%d", page->m_pageRightMar).c_str();
        currentNode.append_attribute("page.rightmar") = StringFormat("%d", page->m_pageRightMar).c_str();
    }
    if (!page->m_surface.empty()) {
        currentNode.append_attribute("surface") = page->m_surface.c_str();
    }
}

void MeiOutput::WriteMeiSystem(pugi::xml_node currentNode, System *system)
{
    assert(system);

    WriteXmlId(currentNode, system);
    // margins
    currentNode.append_attribute("system.leftmar") = StringFormat("%d", system->m_systemLeftMar).c_str();
    currentNode.append_attribute("system.rightmar") = StringFormat("%d", system->m_systemRightMar).c_str();
    // y positions
    if (system->m_yAbs != VRV_UNSET) {
        currentNode.append_attribute("uly") = StringFormat("%d", system->m_yAbs / DEFINITION_FACTOR).c_str();
    }
    system->WriteTyped(currentNode);
}

void MeiOutput::WriteSystemElement(pugi::xml_node currentNode, SystemElement *systemElement)
{
    assert(systemElement);

    WriteXmlId(currentNode, systemElement);
    systemElement->WriteTyped(currentNode);
}

void MeiOutput::WriteMeiBoundaryEnd(pugi::xml_node currentNode, BoundaryEnd *boundaryEnd)
{
    assert(boundaryEnd && boundaryEnd->GetStart());

    WriteSystemElement(currentNode, boundaryEnd);
    currentNode.append_attribute("startid") = UuidToMeiStr(boundaryEnd->GetStart()).c_str();
    std::string meiElementName = boundaryEnd->GetStart()->GetClassName();
    std::transform(meiElementName.begin(), meiElementName.begin() + 1, meiElementName.begin(), ::tolower);
    currentNode.append_attribute("type") = meiElementName.c_str();
}

void MeiOutput::WriteMeiSection(pugi::xml_node currentNode, Section *section)
{
    assert(section);

    WriteSystemElement(currentNode, section);
    section->WriteNNumberLike(currentNode);
}

void MeiOutput::WriteMeiEnding(pugi::xml_node currentNode, Ending *ending)
{
    assert(ending);

    WriteSystemElement(currentNode, ending);
    ending->WriteNNumberLike(currentNode);
}

void MeiOutput::WriteMeiExpansion(pugi::xml_node currentNode, Expansion *expansion)
{
    assert(expansion);

    WriteSystemElement(currentNode, expansion);
}

void MeiOutput::WriteMeiPb(pugi::xml_node currentNode, Pb *pb)
{
    assert(pb);

    WriteSystemElement(currentNode, pb);
    pb->WriteNNumberLike(currentNode);
}

void MeiOutput::WriteMeiSb(pugi::xml_node currentNode, Sb *sb)
{
    assert(sb);

    WriteSystemElement(currentNode, sb);
    sb->WriteNNumberLike(currentNode);
}

void MeiOutput::WriteScoreDefElement(pugi::xml_node currentNode, ScoreDefElement *scoreDefElement)
{
    assert(scoreDefElement);

    WriteXmlId(currentNode, scoreDefElement);
    scoreDefElement->WriteTyped(currentNode);
}

void MeiOutput::WriteMeiScoreDef(pugi::xml_node currentNode, ScoreDef *scoreDef)
{
    assert(scoreDef);

    WriteScoreDefElement(currentNode, scoreDef);
    WriteScoreDefInterface(currentNode, scoreDef);
    scoreDef->WriteEndings(currentNode);
}

void MeiOutput::WriteMeiStaffGrp(pugi::xml_node currentNode, StaffGrp *staffGrp)
{
    assert(staffGrp);

    WriteXmlId(currentNode, staffGrp);
    staffGrp->WriteBasic(currentNode);
    staffGrp->WriteLabelled(currentNode);
    staffGrp->WriteStaffGroupingSym(currentNode);
    staffGrp->WriteStaffGrpVis(currentNode);
    staffGrp->WriteTyped(currentNode);
}

void MeiOutput::WriteMeiStaffDef(pugi::xml_node currentNode, StaffDef *staffDef)
{
    assert(staffDef);

    WriteScoreDefElement(currentNode, staffDef);
    WriteScoreDefInterface(currentNode, staffDef);
    staffDef->WriteDistances(currentNode);
    staffDef->WriteLabelled(currentNode);
    staffDef->WriteNInteger(currentNode);
    staffDef->WriteNotationType(currentNode);
    staffDef->WriteScalable(currentNode);
    staffDef->WriteStaffDefLog(currentNode);
    staffDef->WriteTransposition(currentNode);
}

void MeiOutput::WriteMeiLabel(pugi::xml_node currentNode, Label *label)
{
    assert(label);

    WriteXmlId(currentNode, label);
}

void MeiOutput::WriteMeiLabelAbbr(pugi::xml_node currentNode, LabelAbbr *labelAbbr)
{
    assert(labelAbbr);

    WriteXmlId(currentNode, labelAbbr);
}

void MeiOutput::WriteMeiMeasure(pugi::xml_node currentNode, Measure *measure)
{
    assert(measure);

    WriteXmlId(currentNode, measure);
    measure->WriteMeasureLog(currentNode);
    measure->WriteNNumberLike(currentNode);
    measure->WritePointing(currentNode);
    measure->WriteTyped(currentNode);
    if ((measure->m_xAbs != VRV_UNSET) && (measure->m_xAbs2 != VRV_UNSET)) {
        currentNode.append_attribute("ulx") = StringFormat("%d", measure->m_xAbs / DEFINITION_FACTOR).c_str();
        currentNode.append_attribute("lrx") = StringFormat("%d", measure->m_xAbs2 / DEFINITION_FACTOR).c_str();
    }
}

void MeiOutput::WriteMeiFb(pugi::xml_node currentNode, Fb *fb)
{
    assert(fb);

    WriteXmlId(currentNode, fb);
};

void MeiOutput::WriteControlElement(pugi::xml_node currentNode, ControlElement *controlElement)
{
    assert(controlElement);

    WriteXmlId(currentNode, controlElement);
    controlElement->WriteLabelled(currentNode);
    controlElement->WriteTyped(currentNode);
}

void MeiOutput::WriteMeiAnchoredText(pugi::xml_node currentNode, AnchoredText *anchoredText)
{
    assert(anchoredText);

    WriteControlElement(currentNode, anchoredText);
    WriteTextDirInterface(currentNode, anchoredText);
}

void MeiOutput::WriteMeiBreath(pugi::xml_node currentNode, Breath *breath)
{
    assert(breath);

    WriteControlElement(currentNode, breath);
    WriteTimePointInterface(currentNode, breath);
    breath->WriteColor(currentNode);
    breath->WritePlacement(currentNode);
};

void MeiOutput::WriteMeiDir(pugi::xml_node currentNode, Dir *dir)
{
    assert(dir);

    WriteControlElement(currentNode, dir);
    WriteTextDirInterface(currentNode, dir);
    WriteTimeSpanningInterface(currentNode, dir);
    dir->WriteLang(currentNode);
};

void MeiOutput::WriteMeiDynam(pugi::xml_node currentNode, Dynam *dynam)
{
    assert(dynam);

    WriteControlElement(currentNode, dynam);
    WriteTextDirInterface(currentNode, dynam);
    WriteTimeSpanningInterface(currentNode, dynam);
};

void MeiOutput::WriteMeiFermata(pugi::xml_node currentNode, Fermata *fermata)
{
    assert(fermata);

    WriteControlElement(currentNode, fermata);
    WriteTimePointInterface(currentNode, fermata);
    fermata->WriteColor(currentNode);
    fermata->WriteFermataVis(currentNode);
    fermata->WritePlacement(currentNode);
};

void MeiOutput::WriteMeiHairpin(pugi::xml_node currentNode, Hairpin *hairpin)
{
    assert(hairpin);

    WriteControlElement(currentNode, hairpin);
    WriteTimeSpanningInterface(currentNode, hairpin);
    hairpin->WriteColor(currentNode);
    hairpin->WriteHairpinLog(currentNode);
    hairpin->WritePlacement(currentNode);
};

void MeiOutput::WriteMeiHarm(pugi::xml_node currentNode, Harm *harm)
{
    assert(harm);

    WriteControlElement(currentNode, harm);
    WriteTextDirInterface(currentNode, harm);
    WriteTimeSpanningInterface(currentNode, harm);
    harm->WriteLang(currentNode);
};

void MeiOutput::WriteMeiMordent(pugi::xml_node currentNode, Mordent *mordent)
{
    assert(mordent);

    WriteControlElement(currentNode, mordent);
    WriteTimePointInterface(currentNode, mordent);
    mordent->WriteColor(currentNode);
    mordent->WriteOrnamentAccid(currentNode);
    mordent->WritePlacement(currentNode);
    mordent->WriteMordentLog(currentNode);
};

void MeiOutput::WriteMeiOctave(pugi::xml_node currentNode, Octave *octave)
{
    assert(octave);

    WriteControlElement(currentNode, octave);
    WriteTimeSpanningInterface(currentNode, octave);
    octave->WriteColor(currentNode);
    octave->WriteLineRendBase(currentNode);
    octave->WriteOctaveDisplacement(currentNode);
};

void MeiOutput::WriteMeiPedal(pugi::xml_node currentNode, Pedal *pedal)
{
    assert(pedal);

    WriteControlElement(currentNode, pedal);
    WriteTimePointInterface(currentNode, pedal);
    pedal->WriteColor(currentNode);
    pedal->WritePedalLog(currentNode);
    pedal->WritePlacement(currentNode);
};

void MeiOutput::WriteMeiSlur(pugi::xml_node currentNode, Slur *slur)
{
    assert(slur);

    WriteControlElement(currentNode, slur);
    WriteTimeSpanningInterface(currentNode, slur);
    slur->WriteColor(currentNode);
    slur->WriteCurvature(currentNode);
};

void MeiOutput::WriteMeiStaff(pugi::xml_node currentNode, Staff *staff)
{
    assert(staff);

    WriteXmlId(currentNode, staff);
    staff->WriteNInteger(currentNode);
    staff->WriteTyped(currentNode);
    staff->WriteVisibility(currentNode);

    // y position
    if (staff->m_yAbs != VRV_UNSET) {
        currentNode.append_attribute("uly") = StringFormat("%d", staff->m_yAbs / DEFINITION_FACTOR).c_str();
    }
}

void MeiOutput::WriteMeiTempo(pugi::xml_node currentNode, Tempo *tempo)
{
    assert(tempo);

    WriteControlElement(currentNode, tempo);
    WriteTextDirInterface(currentNode, tempo);
    WriteTimePointInterface(currentNode, tempo);
    tempo->WriteLang(currentNode);
    tempo->WriteMidiTempo(currentNode);
    tempo->WriteMmTempo(currentNode);
}

void MeiOutput::WriteMeiTie(pugi::xml_node currentNode, Tie *tie)
{
    assert(tie);

    WriteControlElement(currentNode, tie);
    WriteTimeSpanningInterface(currentNode, tie);
    tie->WriteColor(currentNode);
    tie->WriteCurvature(currentNode);
};

void MeiOutput::WriteMeiTrill(pugi::xml_node currentNode, Trill *trill)
{
    assert(trill);

    WriteControlElement(currentNode, trill);
    WriteTimePointInterface(currentNode, trill);
    trill->WriteColor(currentNode);
    trill->WriteOrnamentAccid(currentNode);
    trill->WritePlacement(currentNode);
};

void MeiOutput::WriteMeiTurn(pugi::xml_node currentNode, Turn *turn)
{
    assert(turn);

    WriteControlElement(currentNode, turn);
    WriteTimePointInterface(currentNode, turn);
    turn->WriteColor(currentNode);
    turn->WriteOrnamentAccid(currentNode);
    turn->WritePlacement(currentNode);
    turn->WriteTurnLog(currentNode);
};

void MeiOutput::WriteMeiLayer(pugi::xml_node currentNode, Layer *layer)
{
    assert(layer);

    WriteXmlId(currentNode, layer);
    layer->WriteNInteger(currentNode);
    layer->WriteTyped(currentNode);
    layer->WriteVisibility(currentNode);
}

void MeiOutput::WriteLayerElement(pugi::xml_node currentNode, LayerElement *element)
{
    assert(element);

    WriteXmlId(currentNode, element);
    element->WriteLabelled(currentNode);
    element->WriteTyped(currentNode);
    if (element->m_xAbs != VRV_UNSET) {
        currentNode.append_attribute("ulx") = StringFormat("%d", element->m_xAbs / DEFINITION_FACTOR).c_str();
    }
}

void MeiOutput::WriteMeiAccid(pugi::xml_node currentNode, Accid *accid)
{
    assert(accid);

    // Only write att.accidental and accidentalPerformed if representing an attribute
    if (accid->IsAttribute()) {
        accid->WriteAccidental(currentNode);
        accid->WriteAccidentalGestural(currentNode);
        return;
    }

    WriteLayerElement(currentNode, accid);
    WritePositionInterface(currentNode, accid);
    accid->WriteAccidental(currentNode);
    accid->WriteAccidentalGestural(currentNode);
    accid->WriteAccidLog(currentNode);
    accid->WriteColor(currentNode);
    accid->WriteEnclosingChars(currentNode);
}

void MeiOutput::WriteMeiArtic(pugi::xml_node currentNode, Artic *artic)
{
    assert(artic);

    // Only write att.articulation if representing an attribute
    if (artic->IsAttribute()) {
        artic->WriteArticulation(currentNode);
        return;
    }

    WriteLayerElement(currentNode, artic);
    artic->WriteArticulation(currentNode);
    artic->WriteColor(currentNode);
    artic->WritePlacement(currentNode);
}

void MeiOutput::WriteMeiBarLine(pugi::xml_node currentNode, BarLine *barLine)
{
    assert(barLine);

    WriteLayerElement(currentNode, barLine);
    barLine->WriteBarLineLog(currentNode);
    barLine->WriteColor(currentNode);
}

void MeiOutput::WriteMeiBeam(pugi::xml_node currentNode, Beam *beam)
{
    assert(beam);

    WriteLayerElement(currentNode, beam);
    beam->WriteColor(currentNode);
}

void MeiOutput::WriteMeiBeatRpt(pugi::xml_node currentNode, BeatRpt *beatRpt)
{
    assert(beatRpt);

    WriteLayerElement(currentNode, beatRpt);
    beatRpt->WriteColor(currentNode);
    beatRpt->WriteBeatRptVis(currentNode);
}

void MeiOutput::WriteMeiBTrem(pugi::xml_node currentNode, BTrem *bTrem)
{
    assert(bTrem);

    WriteLayerElement(currentNode, bTrem);
    bTrem->WriteTremMeasured(currentNode);
}

void MeiOutput::WriteMeiChord(pugi::xml_node currentNode, Chord *chord)
{
    assert(chord);

    WriteLayerElement(currentNode, chord);
    WriteDurationInterface(currentNode, chord);
    chord->WriteColor(currentNode);
    chord->WriteCue(currentNode);
    chord->WriteGraced(currentNode);
    chord->WriteStems(currentNode);
    chord->WriteStemsCmn(currentNode);
    chord->WriteTiePresent(currentNode);
    chord->WriteVisibility(currentNode);
}

void MeiOutput::WriteMeiClef(pugi::xml_node currentNode, Clef *clef)
{
    assert(clef);

    WriteLayerElement(currentNode, clef);
    clef->WriteClefShape(currentNode);
    clef->WriteColor(currentNode);
    clef->WriteLineLoc(currentNode);
    clef->WriteOctaveDisplacement(currentNode);
}

void MeiOutput::WriteMeiCustos(pugi::xml_node currentNode, Custos *custos)
{
    assert(custos);

    WriteLayerElement(currentNode, custos);
    WritePositionInterface(currentNode, custos);
    custos->WriteColor(currentNode);
}

void MeiOutput::WriteMeiDot(pugi::xml_node currentNode, Dot *dot)
{
    assert(dot);

    WriteLayerElement(currentNode, dot);
    WritePositionInterface(currentNode, dot);
    dot->WriteColor(currentNode);
    dot->WriteDotLog(currentNode);
}

void MeiOutput::WriteMeiFTrem(pugi::xml_node currentNode, FTrem *fTrem)
{
    assert(fTrem);

    WriteLayerElement(currentNode, fTrem);
    fTrem->WriteSlashCount(currentNode);
    fTrem->WriteTremMeasured(currentNode);
}

void MeiOutput::WriteMeiKeySig(pugi::xml_node currentNode, KeySig *keySig)
{
    assert(keySig);

    WriteLayerElement(currentNode, keySig);
    keySig->WriteAccidental(currentNode);
    keySig->WritePitch(currentNode);
}

void MeiOutput::WriteMeiMensur(pugi::xml_node currentNode, Mensur *mensur)
{
    assert(mensur);

    WriteLayerElement(currentNode, mensur);
    mensur->WriteColor(currentNode);
    mensur->WriteCue(currentNode);
    mensur->WriteDurationRatio(currentNode);
    mensur->WriteMensuralShared(currentNode);
    mensur->WriteMensurLog(currentNode);
    mensur->WriteMensurVis(currentNode);
    mensur->WriteSlashCount(currentNode);
}

void MeiOutput::WriteMeiMeterSig(pugi::xml_node currentNode, MeterSig *meterSig)
{
    assert(meterSig);

    WriteLayerElement(currentNode, meterSig);
    meterSig->WriteMeterSigLog(currentNode);
    meterSig->WriteMeterSigVis(currentNode);
}

void MeiOutput::WriteMeiMRest(pugi::xml_node currentNode, MRest *mRest)
{
    assert(mRest);

    WriteLayerElement(currentNode, mRest);
    WritePositionInterface(currentNode, mRest);
    mRest->WriteCue(currentNode);
    mRest->WriteFermataPresent(currentNode);
    mRest->WriteVisibility(currentNode);
}

void MeiOutput::WriteMeiMRpt(pugi::xml_node currentNode, MRpt *mRpt)
{
    assert(mRpt);

    WriteLayerElement(currentNode, mRpt);
}

void MeiOutput::WriteMeiMRpt2(pugi::xml_node currentNode, MRpt2 *mRpt2)
{
    assert(mRpt2);

    WriteLayerElement(currentNode, mRpt2);
}

void MeiOutput::WriteMeiMultiRest(pugi::xml_node currentNode, MultiRest *multiRest)
{
    assert(multiRest);

    WriteLayerElement(currentNode, multiRest);
    multiRest->WriteMultiRestVis(currentNode);
    multiRest->WriteNumbered(currentNode);
}

void MeiOutput::WriteMeiMultiRpt(pugi::xml_node currentNode, MultiRpt *multiRpt)
{
    assert(multiRpt);

    WriteLayerElement(currentNode, multiRpt);
    multiRpt->WriteNumbered(currentNode);
}

void MeiOutput::WriteMeiNote(pugi::xml_node currentNode, Note *note)
{
    assert(note);

    WriteLayerElement(currentNode, note);
    WriteDurationInterface(currentNode, note);
    WritePitchInterface(currentNode, note);
    WritePositionInterface(currentNode, note);
    note->WriteColor(currentNode);
    note->WriteColoration(currentNode);
    note->WriteCue(currentNode);
    note->WriteGraced(currentNode);
    note->WriteNoteAnlMensural(currentNode);
    note->WriteStems(currentNode);
    note->WriteStemsCmn(currentNode);
    note->WriteTiePresent(currentNode);
    note->WriteVisibility(currentNode);
}

void MeiOutput::WriteMeiRest(pugi::xml_node currentNode, Rest *rest)
{
    assert(rest);

    WriteLayerElement(currentNode, rest);
    WriteDurationInterface(currentNode, rest);
    WritePositionInterface(currentNode, rest);
    rest->WriteColor(currentNode);
    rest->WriteCue(currentNode);
    rest->WriteRestVisMensural(currentNode);
}

void MeiOutput::WriteMeiProport(pugi::xml_node currentNode, Proport *proport)
{
    assert(proport);

    WriteLayerElement(currentNode, proport);
}

void MeiOutput::WriteMeiSpace(pugi::xml_node currentNode, Space *space)
{
    assert(space);

    WriteLayerElement(currentNode, space);
    WriteDurationInterface(currentNode, space);
}

void MeiOutput::WriteMeiTuplet(pugi::xml_node currentNode, Tuplet *tuplet)
{
    assert(tuplet);

    WriteLayerElement(currentNode, tuplet);
    tuplet->WriteColor(currentNode);
    tuplet->WriteDurationRatio(currentNode);
    tuplet->WriteNumberPlacement(currentNode);
    tuplet->WriteTupletVis(currentNode);
}

void MeiOutput::WriteMeiVerse(pugi::xml_node currentNode, Verse *verse)
{
    assert(verse);

    WriteLayerElement(currentNode, verse);
    verse->WriteColor(currentNode);
    verse->WriteLang(currentNode);
    verse->WriteNInteger(currentNode);
    verse->WriteTypography(currentNode);
}

void MeiOutput::WriteMeiSyl(pugi::xml_node currentNode, Syl *syl)
{
    assert(syl);

    WriteLayerElement(currentNode, syl);
    syl->WriteLang(currentNode);
    syl->WriteTypography(currentNode);
    syl->WriteSylLog(currentNode);
}

void MeiOutput::WriteTextElement(pugi::xml_node currentNode, TextElement *textElement)
{
    assert(textElement);

    WriteXmlId(currentNode, textElement);
    textElement->WriteTyped(currentNode);
}

void MeiOutput::WriteMeiF(pugi::xml_node currentNode, F *figure)
{
    assert(figure);

    WriteTextElement(currentNode, figure);
};

void MeiOutput::WriteMeiRend(pugi::xml_node currentNode, Rend *rend)
{
    assert(rend);

    WriteTextElement(currentNode, rend);
    rend->WriteColor(currentNode);
    rend->WriteHorizontalAlign(currentNode);
    rend->WriteLang(currentNode);
    rend->WriteTypography(currentNode);
    rend->WriteWhitespace(currentNode);
}

void MeiOutput::WriteMeiText(pugi::xml_node element, Text *text)
{
    if (!text->GetText().empty()) {
        pugi::xml_node nodechild = element.append_child(pugi::node_pcdata);
        // nodechild.text() =  UTF16to8(EscapeSMuFL(text->GetText()).c_str()).c_str();
        nodechild.text() = UTF16to8(text->GetText()).c_str();
    }
}

void MeiOutput::WriteDurationInterface(pugi::xml_node element, DurationInterface *interface)
{
    assert(interface);

    interface->WriteAugmentDots(element);
    interface->WriteBeamSecondary(element);
    interface->WriteDurationGestural(element);
    interface->WriteDurationLogical(element);
    interface->WriteDurationRatio(element);
    interface->WriteFermataPresent(element);
    interface->WriteStaffIdent(element);
}

void MeiOutput::WritePitchInterface(pugi::xml_node element, PitchInterface *interface)
{
    assert(interface);

    interface->WriteNoteGes(element);
    interface->WriteOctave(element);
    interface->WritePitch(element);
}

void MeiOutput::WritePositionInterface(pugi::xml_node element, PositionInterface *interface)
{
    assert(interface);

    interface->WriteStaffLoc(element);
    interface->WriteStaffLocPitched(element);
}

void MeiOutput::WriteScoreDefInterface(pugi::xml_node element, ScoreDefInterface *interface)
{
    assert(interface);

    interface->WriteCleffingLog(element);
    interface->WriteCleffingVis(element);
    interface->WriteKeySigDefaultAnl(element);
    interface->WriteKeySigDefaultLog(element);
    interface->WriteKeySigDefaultVis(element);
    interface->WriteLyricStyle(element);
    interface->WriteMensuralLog(element);
    interface->WriteMensuralShared(element);
    interface->WriteMeterSigDefaultLog(element);
    interface->WriteMeterSigDefaultVis(element);
    interface->WriteMidiTempo(element);
    interface->WriteMultinumMeasures(element);
}

void MeiOutput::WriteTextDirInterface(pugi::xml_node element, TextDirInterface *interface)
{
    assert(interface);

    interface->WritePlacement(element);
}

void MeiOutput::WriteTimePointInterface(pugi::xml_node element, TimePointInterface *interface)
{
    assert(interface);

    interface->WriteStaffIdent(element);
    interface->WriteStartId(element);
    interface->WriteTimestampLogical(element);
}

void MeiOutput::WriteTimeSpanningInterface(pugi::xml_node element, TimeSpanningInterface *interface)
{
    assert(interface);

    WriteTimePointInterface(element, interface);
    interface->WriteStartEndId(element);
    interface->WriteTimestamp2Logical(element);
}

void MeiOutput::WriteUnsupportedAttr(pugi::xml_node element, Object *object)
{
    ArrayOfStrAttr::iterator iter;
    for (iter = object->m_unsupported.begin(); iter != object->m_unsupported.end(); iter++) {
        element.append_attribute((*iter).first.c_str()) = (*iter).second.c_str();
    }
}

void MeiOutput::WriteEditorialElement(pugi::xml_node currentNode, EditorialElement *element)
{
    assert(element);

    WriteXmlId(currentNode, element);
    element->WriteLabelled(currentNode);
    element->WriteTyped(currentNode);
}

void MeiOutput::WriteMeiAbbr(pugi::xml_node currentNode, Abbr *abbr)
{
    assert(abbr);

    WriteEditorialElement(currentNode, abbr);
    abbr->WriteSource(currentNode);
};

void MeiOutput::WriteMeiAdd(pugi::xml_node currentNode, Add *add)
{
    assert(add);

    WriteEditorialElement(currentNode, add);
    add->WriteSource(currentNode);
};

void MeiOutput::WriteMeiAnnot(pugi::xml_node currentNode, Annot *annot)
{
    assert(annot);

    WriteEditorialElement(currentNode, annot);
    annot->WritePlist(currentNode);
    annot->WriteSource(currentNode);
    // special case where we keep the pugi::nodes
    for (pugi::xml_node child = annot->m_content.first_child(); child; child = child.next_sibling()) {
        currentNode.append_copy(child);
    }
};

void MeiOutput::WriteMeiApp(pugi::xml_node currentNode, App *app)
{
    assert(app);

    WriteEditorialElement(currentNode, app);
};

void MeiOutput::WriteMeiChoice(pugi::xml_node currentNode, Choice *choice)
{
    assert(choice);

    WriteEditorialElement(currentNode, choice);
};

void MeiOutput::WriteMeiCorr(pugi::xml_node currentNode, Corr *corr)
{
    assert(corr);

    WriteEditorialElement(currentNode, corr);
    corr->WriteSource(currentNode);
};

void MeiOutput::WriteMeiDamage(pugi::xml_node currentNode, Damage *damage)
{
    assert(damage);

    WriteEditorialElement(currentNode, damage);
    damage->WriteSource(currentNode);
};

void MeiOutput::WriteMeiDel(pugi::xml_node currentNode, Del *del)
{
    assert(del);

    WriteEditorialElement(currentNode, del);
    del->WriteSource(currentNode);
};

void MeiOutput::WriteMeiExpan(pugi::xml_node currentNode, Expan *expan)
{
    assert(expan);

    WriteEditorialElement(currentNode, expan);
    expan->WriteSource(currentNode);
};

void MeiOutput::WriteMeiLem(pugi::xml_node currentNode, Lem *lem)
{
    assert(lem);

    WriteEditorialElement(currentNode, lem);
    lem->WriteSource(currentNode);
};

void MeiOutput::WriteMeiOrig(pugi::xml_node currentNode, Orig *orig)
{
    assert(orig);

    WriteEditorialElement(currentNode, orig);
    orig->WriteSource(currentNode);
};

void MeiOutput::WriteMeiRdg(pugi::xml_node currentNode, Rdg *rdg)
{
    assert(rdg);

    WriteEditorialElement(currentNode, rdg);
    rdg->WriteSource(currentNode);
};

void MeiOutput::WriteMeiReg(pugi::xml_node currentNode, Reg *reg)
{
    assert(reg);

    WriteEditorialElement(currentNode, reg);
    reg->WriteSource(currentNode);
};

void MeiOutput::WriteMeiRestore(pugi::xml_node currentNode, Restore *restore)
{
    assert(restore);

    WriteEditorialElement(currentNode, restore);
    restore->WriteSource(currentNode);

    return;
};

void MeiOutput::WriteMeiSic(pugi::xml_node currentNode, Sic *sic)
{
    assert(sic);

    WriteEditorialElement(currentNode, sic);
    sic->WriteSource(currentNode);
};

void MeiOutput::WriteMeiSupplied(pugi::xml_node currentNode, Supplied *supplied)
{
    assert(supplied);

    WriteEditorialElement(currentNode, supplied);
    supplied->WriteSource(currentNode);
};

void MeiOutput::WriteMeiUnclear(pugi::xml_node currentNode, Unclear *unclear)
{
    assert(unclear);

    WriteEditorialElement(currentNode, unclear);
    unclear->WriteSource(currentNode);
};

std::wstring MeiOutput::EscapeSMuFL(std::wstring data)
{
    std::wstring buffer;
    // approximate that we won't have a 1.1 longer string (for optimization)
    buffer.reserve(data.size() * 1.1);
    for (size_t pos = 0; pos != data.size(); ++pos) {
        if (data[pos] == '&') {
            buffer.append(L"&amp;");
        }
        else if (data[pos] == '\"') {
            buffer.append(L"&quot;");
        }
        else if (data[pos] == '\'') {
            buffer.append(L"&apos;");
        }
        else if (data[pos] == '<') {
            buffer.append(L"&lt;");
        }
        else if (data[pos] == '>') {
            buffer.append(L"&gt;");
        }
        // Unicode private area for SMuFL characters
        else if ((data[pos] > 0xE000) && (data[pos] < 0xF8FF)) {
            std::wostringstream ss;
            ss << std::hex << (int)data[pos];
            buffer.append(L"&#x").append(ss.str()).append(L";");
        }
        else
            buffer.append(&data[pos], 1);
    }
    return buffer;
}

std::string MeiOutput::DocTypeToStr(DocType type)
{
    std::string value;
    switch (type) {
        case Raw: value = "raw"; break;
        case Rendering: value = "rendering"; break;
        case Transcription: value = "transcription"; break;
        default:
            LogWarning("Unknown document type '%d'", type);
            value = "";
            break;
    }

    return value;
}

//----------------------------------------------------------------------------
// MeiInput
//----------------------------------------------------------------------------

MeiInput::MeiInput(Doc *doc, std::string filename) : FileInputStream(doc)
{
    m_filename = filename;
    //
    m_hasScoreDef = false;
    m_readingScoreBased = false;
    m_version = MEI_UNDEFINED;
}

MeiInput::~MeiInput()
{
}

bool MeiInput::ImportFile()
{
    try {
        m_doc->SetType(Raw);
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(m_filename.c_str(), pugi::parse_default & ~pugi::parse_eol);
        if (!result) {
            return false;
        }
        pugi::xml_node root = doc.first_child();
        return ReadMei(root);
    }
    catch (char *str) {
        LogError("%s", str);
        return false;
    }
}

bool MeiInput::ImportString(std::string const &mei)
{
    try {
        m_doc->SetType(Raw);
        pugi::xml_document doc;
        doc.load(mei.c_str(), pugi::parse_default & ~pugi::parse_eol);
        pugi::xml_node root = doc.first_child();
        return ReadMei(root);
    }
    catch (char *str) {
        LogError("%s", str);
        return false;
    }
}

bool MeiInput::IsAllowed(std::string element, Object *filterParent)
{
    if (!filterParent) {
        return true;
    }

    // editorial
    if (IsEditorialElementName(element)) {
        if (filterParent->Is(LABEL)) {
            return false;
        }
        else if (filterParent->Is(LABELABBR)) {
            return false;
        }
        else {
            return true;
        }
    }
    // filter for annot
    else if (filterParent->Is(ANNOT)) {
        if (element == "") {
            return true;
        }
        else {
            return false;
        }
    }
    // filter for harm
    else if (filterParent->Is(HARM)) {
        if (element == "") {
            return true;
        }
        else if (element == "rend") {
            return true;
        }
        else if (element == "fb") {
            return true;
        }
        else {
            return false;
        }
    }
    // filter for any other control element
    else if (filterParent->IsControlElement()) {
        if (element == "") {
            return true;
        }
        else if (element == "rend") {
            return true;
        }
        else {
            return false;
        }
    }
    // filter for beam
    else if (filterParent->Is(BEAM)) {
        if (element == "beam") {
            return true;
        }
        else if (element == "chord") {
            return true;
        }
        else if (element == "clef") {
            return true;
        }
        else if (element == "note") {
            return true;
        }
        else if (element == "rest") {
            return true;
        }
        else if (element == "space") {
            return true;
        }
        else if (element == "tuplet") {
            return true;
        }
        else {
            return false;
        }
    }
    // filter for bTrem
    else if (filterParent->Is(BTREM)) {
        if (element == "chord") {
            return true;
        }
        else if (element == "clef") {
            return true;
        }
        else if (element == "note") {
            return true;
        }
        else {
            return false;
        }
    }
    // filter for chord
    else if (filterParent->Is(CHORD)) {
        if (element == "note") {
            return true;
        }
        else if (element == "artic") {
            return true;
        }
        else {
            return false;
        }
    }
    // filter for fTrem
    else if (filterParent->Is(FTREM)) {
        if (element == "chord") {
            return true;
        }
        else if (element == "clef") {
            return true;
        }
        else if (element == "note") {
            return true;
        }
        else {
            return false;
        }
    }
    // filter for label
    else if (filterParent->Is(LABEL)) {
        if (element == "") {
            return true;
        }
        else if (element == "rend") {
            return true;
        }

        else {
            return false;
        }
    }
    // filter for label
    else if (filterParent->Is(LABELABBR)) {
        if (element == "") {
            return true;
        }
        else if (element == "rend") {
            return true;
        }

        else {
            return false;
        }
    }
    // filter for ligature
    else if (filterParent->Is(LIGATURE)) {
        if (element == "note") {
            return true;
        }
        else {
            return false;
        }
    }
    // filter for note
    else if (filterParent->Is(NOTE)) {
        if (element == "accid") {
            return true;
        }
        else if (element == "artic") {
            return true;
        }
        else if (element == "syl") {
            return true;
        }
        else if (element == "verse") {
            return true;
        }
        else {
            return false;
        }
    }
    // filter for tuplet
    else if (filterParent->Is(TUPLET)) {
        if (element == "beam") {
            return true;
        }
        else if (element == "chord") {
            return true;
        }
        else if (element == "clef") {
            return true;
        }
        else if (element == "note") {
            return true;
        }
        else if (element == "rest") {
            return true;
        }
        else if (element == "space") {
            return true;
        }
        else if (element == "tuplet") {
            return true;
        }
        else {
            return false;
        }
    }
    // filter for verse
    else if (filterParent->Is(VERSE)) {
        if (element == "syl") {
            return true;
        }
        else {
            return false;
        }
    }
    // filter for syl
    else if (filterParent->Is(SYL)) {
        if (element == "") {
            return true;
        }
        else if (element == "rend") {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        LogDebug("Unknown filter for '%s'", filterParent->GetClassName().c_str());
        return true;
    }
}

bool MeiInput::ReadMei(pugi::xml_node root)
{
    pugi::xml_node current;
    bool success = true;
    m_readingScoreBased = false;

    if (!root.empty() && (current = root.child("meiHead"))) {
        m_doc->m_header.reset();
        // copy the complete header into the master document
        m_doc->m_header.append_copy(current);
        if (root.attribute("meiversion")) {
            std::string version = std::string(root.attribute("meiversion").value());
            if (version == "4.0.0")
                m_version = MEI_4_0_0;
            else if (version == "3.0.0")
                m_version = MEI_3_0_0;
            else if (version == "2013")
                m_version = MEI_2013;
        }
    }
    // music
    pugi::xml_node music;
    pugi::xml_node body;
    pugi::xml_node mdiv;
    pugi::xml_node pages;
    if (!root.empty()) {
        if (std::string(root.name()) == "music")
            music = root;
        else
            music = root.child("music");
    }
    if (!music.empty()) {
        body = music.child("body");
    }
    if (!body.empty()) {
        if (!m_mdivXPathQuery.empty()) {
            pugi::xpath_node selection = body.select_single_node(m_mdivXPathQuery.c_str());
            if (selection)
                mdiv = selection.node();
            else {
                LogError("the <mdiv> requested with the xpath query '%s' could not be found", m_mdivXPathQuery.c_str());
                return false;
            }
        }
        else
            mdiv = body.child("mdiv");
    }

    if (!mdiv.empty()) {
        pages = mdiv.child("pages");
    }
    if (!pages.empty()) {

        // check if there is a type attribute for the score
        DocType type;
        if (pages.attribute("type")) {
            type = StrToDocType(pages.attribute("type").value());
            m_doc->SetType(type);
        }

        // this is a page-based MEI file, we just loop trough the pages
        if (pages.child("page")) {
            // because we are in a page-based MEI
            this->m_hasLayoutInformation = true;
            for (current = pages.child("page"); current; current = current.next_sibling("page")) {
                if (!success) break;
                success = ReadMeiPage(current);
            }
        }
    }
    else {
        m_readingScoreBased = true;
        Score *score = m_doc->CreateScoreBuffer();
        pugi::xml_node current;
        for (current = mdiv.first_child(); current; current = current.next_sibling()) {
            if (!success) break;
            success = ReadScoreBasedMei(current, score);
        }
        if (success) {
            m_doc->ConvertToPageBasedDoc();
        }
    }

    if (success && !m_hasScoreDef) {
        LogMessage("No scoreDef provided, trying to generate one...");
        success = m_doc->GenerateDocumentScoreDef();
    }

    return success;
}

bool MeiInput::ReadMeiSection(Object *parent, pugi::xml_node section)
{
    Section *vrvSection = new Section();
    SetMeiUuid(section, vrvSection);

    vrvSection->ReadNNumberLike(section);

    parent->AddChild(vrvSection);
    if (m_readingScoreBased)
        return ReadMeiSectionChildren(vrvSection, section);
    else
        return ReadMeiSystemChildren(vrvSection, section);
}

bool MeiInput::ReadMeiSectionChildren(Object *parent, pugi::xml_node parentNode)
{
    assert(dynamic_cast<Section *>(parent) || dynamic_cast<Ending *>(parent) || dynamic_cast<Expansion *>(parent)
        || dynamic_cast<EditorialElement *>(parent));

    bool success = true;
    pugi::xml_node current;
    Measure *unmeasured = NULL;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        if (!success) break;
        // editorial
        else if (IsEditorialElementName(current.name())) {
            success = ReadMeiEditorialElement(parent, current, EDITORIAL_TOPLEVEL);
        }
        // content
        else if (std::string(current.name()) == "ending") {
            // we should not endings with unmeasured music ... (?)
            assert(!unmeasured);
            success = ReadMeiEnding(parent, current);
        }
        else if (std::string(current.name()) == "expansion") {
            success = ReadMeiExpansion(parent, current);
        }
        else if (std::string(current.name()) == "scoreDef") {
            success = ReadMeiScoreDef(parent, current);
        }
        else if (std::string(current.name()) == "section") {
            success = ReadMeiSection(parent, current);
        }
        // pb and sb
        else if (std::string(current.name()) == "pb") {
            success = ReadMeiPb(parent, current);
        }
        else if (std::string(current.name()) == "sb") {
            success = ReadMeiSb(parent, current);
        }
        // unmeasured music
        else if (std::string(current.name()) == "staff") {
            if (!unmeasured) {
                if (parent->Is(SECTION)) {
                    unmeasured = new Measure(false);
                    parent->AddChild(unmeasured);
                }
                else {
                    LogError("Unmeasured music within editorial markup is currently not supported");
                    return false;
                }
            }
            success = ReadMeiStaff(unmeasured, current);
        }
        else if (std::string(current.name()) == "measure") {
            // we should not mix measured and unmeasured music within a system...
            assert(!unmeasured);
            // if (parent->IsEditorialElement()) {
            //    m_hasMeasureWithinEditMarkup = true;
            //}
            success = ReadMeiMeasure(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <section>", current.name());
        }
    }
    return success;
}

bool MeiInput::ReadSystemElement(pugi::xml_node element, SystemElement *object)
{
    SetMeiUuid(element, object);
    object->ReadTyped(element);

    return true;
}

bool MeiInput::ReadMeiEnding(Object *parent, pugi::xml_node ending)
{
    Ending *vrvEnding = new Ending();
    ReadSystemElement(ending, vrvEnding);

    vrvEnding->ReadNNumberLike(ending);

    parent->AddChild(vrvEnding);
    if (m_readingScoreBased)
        return ReadMeiSectionChildren(vrvEnding, ending);
    else
        return true;
}

bool MeiInput::ReadMeiExpansion(Object *parent, pugi::xml_node expansion)
{
    Expansion *vrvExpansion = new Expansion();
    ReadSystemElement(expansion, vrvExpansion);

    parent->AddChild(vrvExpansion);
    if (m_readingScoreBased)
        return ReadMeiSectionChildren(vrvExpansion, expansion);
    else
        return true;
}

bool MeiInput::ReadMeiPb(Object *parent, pugi::xml_node pb)
{
    this->m_hasLayoutInformation = true;

    Pb *vrvPb = new Pb();
    ReadSystemElement(pb, vrvPb);

    vrvPb->ReadNNumberLike(pb);

    parent->AddChild(vrvPb);
    return true;
}

bool MeiInput::ReadMeiSb(Object *parent, pugi::xml_node sb)
{
    Sb *vrvSb = new Sb();
    ReadSystemElement(sb, vrvSb);

    vrvSb->ReadNNumberLike(sb);

    parent->AddChild(vrvSb);
    return true;
}

bool MeiInput::ReadMeiPage(pugi::xml_node page)
{
    Page *vrvPage = new Page();
    SetMeiUuid(page, vrvPage);

    if ((m_doc->GetType() == Transcription) && (m_version == MEI_2013)) {
        UpgradePageTo_3_0_0(vrvPage, m_doc);
    }

    if (page.attribute("page.height")) {
        vrvPage->m_pageHeight = atoi(page.attribute("page.height").value()) * DEFINITION_FACTOR;
    }
    if (page.attribute("page.width")) {
        vrvPage->m_pageWidth = atoi(page.attribute("page.width").value()) * DEFINITION_FACTOR;
    }
    if (page.attribute("page.leftmar")) {
        vrvPage->m_pageLeftMar = atoi(page.attribute("page.leftmar").value()) * DEFINITION_FACTOR;
    }
    if (page.attribute("page.rightmar")) {
        vrvPage->m_pageRightMar = atoi(page.attribute("page.rightmar").value()) * DEFINITION_FACTOR;
    }
    if (page.attribute("page.topmar")) {
        vrvPage->m_pageTopMar = atoi(page.attribute("page.topmar").value()) * DEFINITION_FACTOR;
    }
    if (page.attribute("surface")) {
        vrvPage->m_surface = page.attribute("surface").value();
    }

    m_doc->AddChild(vrvPage);
    bool success = ReadMeiPageChildren(vrvPage, page);

    if (success && (m_doc->GetType() == Transcription) && (vrvPage->GetPPUFactor() != 1.0)) {
        ApplyPPUFactorParams applyPPUFactorParams;
        Functor applyPPUFactor(&Object::ApplyPPUFactor);
        vrvPage->Process(&applyPPUFactor, &applyPPUFactorParams);
    }

    return success;
}

bool MeiInput::ReadMeiPageChildren(Object *parent, pugi::xml_node parentNode)
{
    // If we allow <app> between <page> elements
    // assert(dynamic_cast<Page*>(parent) || dynamic_cast<EditorialElement*>(parent));
    assert(dynamic_cast<Page *>(parent));

    pugi::xml_node current;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        if (std::string(current.name()) == "system") {
            ReadMeiSystem(parent, current);
        }
        // can we have scoreDef between system in the current page-based cusotmization? To be checked
        else if (std::string(current.name()) == "scoreDef") {
            ReadMeiScoreDef(parent, current);
        }
        // can we have scoreDef between system in the current page-based cusotmization?
        // To be checked or defined - we would need to add an EDITORIAL_PAGE EditorialLevel
        /*
         else if (std::string(current.name()) == "app") {
         ReadMeiApp(vrvPage, current, EDITORIAL_PAGE);
         }
         */
        else {
            LogWarning("Unsupported '<%s>' within <page>", current.name());
        }
    }

    return true;
}

bool MeiInput::ReadMeiSystem(Object *parent, pugi::xml_node system)
{
    // If we allow <app> between <page> elements
    // assert(dynamic_cast<Page*>(parent) || dynamic_cast<EditorialElement*>(parent));
    assert(dynamic_cast<Page *>(parent));

    System *vrvSystem = new System();
    SetMeiUuid(system, vrvSystem);
    vrvSystem->ReadTyped(system);

    if (system.attribute("system.leftmar")) {
        vrvSystem->m_systemLeftMar = atoi(system.attribute("system.leftmar").value());
    }
    if (system.attribute("system.rightmar")) {
        vrvSystem->m_systemRightMar = atoi(system.attribute("system.rightmar").value());
    }
    if (system.attribute("uly") && (this->m_doc->GetType() == Transcription)) {
        vrvSystem->m_yAbs = atoi(system.attribute("uly").value()) * DEFINITION_FACTOR;
    }

    // This could be moved to an AddSystem method for consistency with AddLayerElement
    if (parent->Is(PAGE)) {
        Page *page = dynamic_cast<Page *>(parent);
        assert(page);
        page->AddChild(vrvSystem);
    }
    return ReadMeiSystemChildren(vrvSystem, system);
}

bool MeiInput::ReadMeiSystemChildren(Object *parent, pugi::xml_node parentNode)
{
    assert(dynamic_cast<System *>(parent) || dynamic_cast<EditorialElement *>(parent));

    bool success = true;
    pugi::xml_node current;
    Measure *unmeasured = NULL;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        if (!success) break;
        // editorial
        else if (IsEditorialElementName(current.name())) {
            success = ReadMeiEditorialElement(parent, current, EDITORIAL_TOPLEVEL);
        }
        // boundaryEnd
        else if (std::string(current.name()) == "boundaryEnd") {
            success = ReadMeiBoundaryEnd(parent, current);
        }
        // content
        else if (std::string(current.name()) == "scoreDef") {
            // we should not have scoredef with unmeasured music within a system... (?)
            assert(!unmeasured);
            ReadMeiScoreDef(parent, current);
        }
        // unmeasured music
        else if (std::string(current.name()) == "staff") {
            if (!unmeasured) {
                if (parent->Is(SYSTEM)) {
                    System *system = dynamic_cast<System *>(parent);
                    assert(system);
                    unmeasured = new Measure(false);
                    if ((m_doc->GetType() == Transcription) && (m_version == MEI_2013)) {
                        UpgradeMeasureTo_3_0_0(unmeasured, system);
                    }
                    system->AddChild(unmeasured);
                }
                else {
                    LogError("Unmeasured music within editorial markup is currently not supported");
                    return false;
                }
            }
            success = ReadMeiStaff(unmeasured, current);
        }
        else if (std::string(current.name()) == "measure") {
            // we should not mix measured and unmeasured music within a system...
            assert(!unmeasured);
            success = ReadMeiMeasure(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <system>", current.name());
        }
    }
    return success;
}

bool MeiInput::ReadMeiBoundaryEnd(Object *parent, pugi::xml_node boundaryEnd)
{
    assert(dynamic_cast<System *>(parent));

    std::string startUuid;
    Object *start = NULL;
    if (boundaryEnd.attribute("startid")) {
        std::string startUuid = boundaryEnd.attribute("startid").value();
        start = m_doc->FindChildByUuid(startUuid);
    }
    if (!start) {
        LogError("Could not find start element '%s' for boundaryEnd", startUuid.c_str());
        return false;
    }

    BoundaryEnd *vrvBoundaryEnd = new BoundaryEnd(start);
    ReadSystemElement(boundaryEnd, vrvBoundaryEnd);

    parent->AddChild(vrvBoundaryEnd);
    return true;
}

bool MeiInput::ReadScoreDefElement(pugi::xml_node element, ScoreDefElement *object)
{
    SetMeiUuid(element, object);
    object->ReadTyped(element);

    return true;
}

bool MeiInput::ReadMeiScoreDef(Object *parent, pugi::xml_node scoreDef)
{
    assert(dynamic_cast<Score *>(parent) || dynamic_cast<Section *>(parent) || dynamic_cast<System *>(parent)
        || dynamic_cast<EditorialElement *>(parent));

    ScoreDef *vrvScoreDef;
    if (!m_hasScoreDef) {
        vrvScoreDef = &m_doc->m_scoreDef;
    }
    else {
        vrvScoreDef = new ScoreDef();
    }
    ReadScoreDefElement(scoreDef, vrvScoreDef);

    ReadScoreDefInterface(scoreDef, vrvScoreDef);
    vrvScoreDef->ReadEndings(scoreDef);

    if (!m_hasScoreDef) {
        m_hasScoreDef = true;
    }
    else {
        parent->AddChild(vrvScoreDef);
    }
    return ReadMeiScoreDefChildren(vrvScoreDef, scoreDef);
}

bool MeiInput::ReadMeiScoreDefChildren(Object *parent, pugi::xml_node parentNode)
{
    assert(dynamic_cast<ScoreDef *>(parent) || dynamic_cast<EditorialElement *>(parent));

    bool success = true;
    pugi::xml_node current;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        if (!success) break;
        // editorial
        else if (IsEditorialElementName(current.name())) {
            success = ReadMeiEditorialElement(parent, current, EDITORIAL_SCOREDEF);
        }
        // content
        else if (std::string(current.name()) == "staffGrp") {
            success = ReadMeiStaffGrp(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <scoreDef>", current.name());
        }
    }
    return success;
}

bool MeiInput::ReadMeiStaffGrp(Object *parent, pugi::xml_node staffGrp)
{
    assert(dynamic_cast<ScoreDef *>(parent) || dynamic_cast<StaffGrp *>(parent));

    StaffGrp *vrvStaffGrp = new StaffGrp();
    SetMeiUuid(staffGrp, vrvStaffGrp);

    if (m_version < MEI_4_0_0) {
        UpgradeStaffGrpTo_4_0_0(staffGrp, vrvStaffGrp);
    }

    vrvStaffGrp->ReadBasic(staffGrp);
    vrvStaffGrp->ReadLabelled(staffGrp);
    vrvStaffGrp->ReadStaffGroupingSym(staffGrp);
    vrvStaffGrp->ReadStaffGrpVis(staffGrp);
    vrvStaffGrp->ReadTyped(staffGrp);

    parent->AddChild(vrvStaffGrp);
    return ReadMeiStaffGrpChildren(vrvStaffGrp, staffGrp);
}

bool MeiInput::ReadMeiStaffGrpChildren(Object *parent, pugi::xml_node parentNode)
{
    assert(dynamic_cast<StaffGrp *>(parent) || dynamic_cast<EditorialElement *>(parent));

    bool success = true;
    pugi::xml_node current;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        if (!success) break;
        // editorial
        else if (IsEditorialElementName(current.name())) {
            success = ReadMeiEditorialElement(parent, current, EDITORIAL_STAFFGRP);
        }
        // content
        else if (std::string(current.name()) == "label") {
            success = ReadMeiLabel(parent, current);
        }
        else if (std::string(current.name()) == "labelAbbr") {
            success = ReadMeiLabelAbbr(parent, current);
        }
        else if (std::string(current.name()) == "staffGrp") {
            success = ReadMeiStaffGrp(parent, current);
        }
        else if (std::string(current.name()) == "staffDef") {
            success = ReadMeiStaffDef(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <staffGrp>", current.name());
        }
    }
    return success;
}

bool MeiInput::ReadMeiStaffDef(Object *parent, pugi::xml_node staffDef)
{
    assert(dynamic_cast<StaffGrp *>(parent) || dynamic_cast<EditorialElement *>(parent));

    StaffDef *vrvStaffDef = new StaffDef();
    ReadScoreDefElement(staffDef, vrvStaffDef);

    if (m_version < MEI_4_0_0) {
        UpgradeStaffDefTo_4_0_0(staffDef, vrvStaffDef);
    }

    vrvStaffDef->ReadDistances(staffDef);
    vrvStaffDef->ReadLabelled(staffDef);
    vrvStaffDef->ReadNInteger(staffDef);
    vrvStaffDef->ReadNotationType(staffDef);
    vrvStaffDef->ReadScalable(staffDef);
    vrvStaffDef->ReadStaffDefLog(staffDef);
    vrvStaffDef->ReadTransposition(staffDef);

    if (!vrvStaffDef->HasN()) {
        LogWarning("No @n on <staffDef> might yield unpredictable results");
    }

    ReadScoreDefInterface(staffDef, vrvStaffDef);

    parent->AddChild(vrvStaffDef);

    return ReadMeiStaffDefChildren(vrvStaffDef, staffDef);
}

bool MeiInput::ReadMeiStaffDefChildren(Object *parent, pugi::xml_node parentNode)
{
    assert(dynamic_cast<StaffDef *>(parent) || dynamic_cast<EditorialElement *>(parent));

    bool success = true;
    pugi::xml_node current;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        if (!success) break;
        // content
        else if (std::string(current.name()) == "label") {
            success = ReadMeiLabel(parent, current);
        }
        else if (std::string(current.name()) == "labelAbbr") {
            success = ReadMeiLabelAbbr(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <staffGrp>", current.name());
        }
    }
    return success;
}

bool MeiInput::ReadMeiLabel(Object *parent, pugi::xml_node label)
{
    Label *vrvLabel = new Label();
    SetMeiUuid(label, vrvLabel);

    parent->AddChild(vrvLabel);
    return ReadMeiTextChildren(vrvLabel, label, vrvLabel);
}

bool MeiInput::ReadMeiLabelAbbr(Object *parent, pugi::xml_node labelAbbr)
{
    LabelAbbr *vrvLabelAbbr = new LabelAbbr();
    SetMeiUuid(labelAbbr, vrvLabelAbbr);

    parent->AddChild(vrvLabelAbbr);
    return ReadMeiTextChildren(vrvLabelAbbr, labelAbbr, vrvLabelAbbr);
}

bool MeiInput::ReadMeiMeasure(Object *parent, pugi::xml_node measure)
{
    Measure *vrvMeasure = new Measure();
    SetMeiUuid(measure, vrvMeasure);

    vrvMeasure->ReadMeasureLog(measure);
    vrvMeasure->ReadNNumberLike(measure);
    vrvMeasure->ReadPointing(measure);
    vrvMeasure->ReadTyped(measure);

    if (measure.attribute("ulx") && measure.attribute("lrx") && (this->m_doc->GetType() == Transcription)) {
        vrvMeasure->m_xAbs = atoi(measure.attribute("ulx").value()) * DEFINITION_FACTOR;
        vrvMeasure->m_xAbs2 = atoi(measure.attribute("lrx").value()) * DEFINITION_FACTOR;
    }

    // This could be moved to an AddMeasure method for consistency with AddLayerElement
    parent->AddChild(vrvMeasure);
    return ReadMeiMeasureChildren(vrvMeasure, measure);
}

bool MeiInput::ReadMeiMeasureChildren(Object *parent, pugi::xml_node parentNode)
{
    assert(dynamic_cast<Measure *>(parent) || dynamic_cast<EditorialElement *>(parent));

    bool success = true;
    pugi::xml_node current;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        if (!success) break;
        // editorial
        else if (IsEditorialElementName(current.name())) {
            success = ReadMeiEditorialElement(parent, current, EDITORIAL_MEASURE);
        }
        // content
        else if (std::string(current.name()) == "anchoredText") {
            success = ReadMeiAnchoredText(parent, current);
        }
        else if (std::string(current.name()) == "breath") {
            success = ReadMeiBreath(parent, current);
        }
        else if (std::string(current.name()) == "dir") {
            success = ReadMeiDir(parent, current);
        }
        else if (std::string(current.name()) == "dynam") {
            success = ReadMeiDynam(parent, current);
        }
        else if (std::string(current.name()) == "fermata") {
            success = ReadMeiFermata(parent, current);
        }
        else if (std::string(current.name()) == "hairpin") {
            success = ReadMeiHairpin(parent, current);
        }
        else if (std::string(current.name()) == "harm") {
            success = ReadMeiHarm(parent, current);
        }
        else if (std::string(current.name()) == "mordent") {
            success = ReadMeiMordent(parent, current);
        }
        else if (std::string(current.name()) == "octave") {
            success = ReadMeiOctave(parent, current);
        }
        else if (std::string(current.name()) == "pedal") {
            success = ReadMeiPedal(parent, current);
        }
        else if (std::string(current.name()) == "slur") {
            success = ReadMeiSlur(parent, current);
        }
        else if (std::string(current.name()) == "staff") {
            success = ReadMeiStaff(parent, current);
        }
        else if (std::string(current.name()) == "tempo") {
            success = ReadMeiTempo(parent, current);
        }
        else if (std::string(current.name()) == "tie") {
            success = ReadMeiTie(parent, current);
        }
        else if (std::string(current.name()) == "trill") {
            success = ReadMeiTrill(parent, current);
        }
        else if (std::string(current.name()) == "turn") {
            success = ReadMeiTurn(parent, current);
        }
        else if (std::string(current.name()) == "tupletSpan") {
            if (!ReadTupletSpanAsTuplet(dynamic_cast<Measure *>(parent), current)) {
                LogWarning("<tupletSpan> is not readable as <tuplet> and will be ignored");
            }
        }
        else {
            LogWarning("Unsupported '<%s>' within <measure>", current.name());
        }
    }
    return success;
}

bool MeiInput::ReadControlElement(pugi::xml_node element, ControlElement *object)
{
    SetMeiUuid(element, object);
    object->ReadLabelled(element);
    object->ReadTyped(element);

    return true;
}

bool MeiInput::ReadMeiAnchoredText(Object *parent, pugi::xml_node anchoredText)
{
    AnchoredText *vrvAnchoredText = new AnchoredText();
    ReadControlElement(anchoredText, vrvAnchoredText);

    ReadTextDirInterface(anchoredText, vrvAnchoredText);

    parent->AddChild(vrvAnchoredText);
    return ReadMeiTextChildren(vrvAnchoredText, anchoredText, vrvAnchoredText);
}

bool MeiInput::ReadMeiBreath(Object *parent, pugi::xml_node breath)
{
    Breath *vrvBreath = new Breath();
    ReadControlElement(breath, vrvBreath);

    ReadTimePointInterface(breath, vrvBreath);
    vrvBreath->ReadColor(breath);
    vrvBreath->ReadPlacement(breath);

    parent->AddChild(vrvBreath);
    return ReadMeiTextChildren(vrvBreath, breath, vrvBreath);
}

bool MeiInput::ReadMeiDir(Object *parent, pugi::xml_node dir)
{
    Dir *vrvDir = new Dir();
    ReadControlElement(dir, vrvDir);

    ReadTextDirInterface(dir, vrvDir);
    ReadTimeSpanningInterface(dir, vrvDir);
    vrvDir->ReadLang(dir);

    parent->AddChild(vrvDir);
    return ReadMeiTextChildren(vrvDir, dir, vrvDir);
}

bool MeiInput::ReadMeiDynam(Object *parent, pugi::xml_node dynam)
{
    Dynam *vrvDynam = new Dynam();
    ReadControlElement(dynam, vrvDynam);

    ReadTextDirInterface(dynam, vrvDynam);
    ReadTimeSpanningInterface(dynam, vrvDynam);

    parent->AddChild(vrvDynam);
    return ReadMeiTextChildren(vrvDynam, dynam, vrvDynam);
}

bool MeiInput::ReadMeiFermata(Object *parent, pugi::xml_node fermata)
{
    Fermata *vrvFermata = new Fermata();
    ReadControlElement(fermata, vrvFermata);

    ReadTimePointInterface(fermata, vrvFermata);
    vrvFermata->ReadColor(fermata);
    vrvFermata->ReadFermataVis(fermata);
    vrvFermata->ReadPlacement(fermata);

    parent->AddChild(vrvFermata);
    return true;
}

bool MeiInput::ReadMeiHairpin(Object *parent, pugi::xml_node hairpin)
{
    Hairpin *vrvHairpin = new Hairpin();
    ReadControlElement(hairpin, vrvHairpin);

    ReadTimeSpanningInterface(hairpin, vrvHairpin);
    vrvHairpin->ReadColor(hairpin);
    vrvHairpin->ReadHairpinLog(hairpin);
    vrvHairpin->ReadPlacement(hairpin);

    parent->AddChild(vrvHairpin);
    return true;
}

bool MeiInput::ReadMeiHarm(Object *parent, pugi::xml_node harm)
{
    Harm *vrvHarm = new Harm();
    ReadControlElement(harm, vrvHarm);

    ReadTextDirInterface(harm, vrvHarm);
    ReadTimeSpanningInterface(harm, vrvHarm);
    vrvHarm->ReadLang(harm);

    parent->AddChild(vrvHarm);
    return ReadMeiTextChildren(vrvHarm, harm, vrvHarm);
}

bool MeiInput::ReadMeiMordent(Object *parent, pugi::xml_node mordent)
{
    Mordent *vrvMordent = new Mordent();
    ReadControlElement(mordent, vrvMordent);

    ReadTimePointInterface(mordent, vrvMordent);
    vrvMordent->ReadColor(mordent);
    vrvMordent->ReadOrnamentAccid(mordent);
    vrvMordent->ReadPlacement(mordent);
    vrvMordent->ReadMordentLog(mordent);

    parent->AddChild(vrvMordent);
    return true;
}

bool MeiInput::ReadMeiOctave(Object *parent, pugi::xml_node octave)
{
    Octave *vrvOctave = new Octave();
    ReadControlElement(octave, vrvOctave);

    ReadTimeSpanningInterface(octave, vrvOctave);
    vrvOctave->ReadColor(octave);
    vrvOctave->ReadLineRendBase(octave);
    vrvOctave->ReadOctaveDisplacement(octave);

    parent->AddChild(vrvOctave);
    return true;
}

bool MeiInput::ReadMeiPedal(Object *parent, pugi::xml_node pedal)
{
    Pedal *vrvPedal = new Pedal();
    ReadControlElement(pedal, vrvPedal);

    ReadTimePointInterface(pedal, vrvPedal);
    vrvPedal->ReadColor(pedal);
    vrvPedal->ReadPedalLog(pedal);
    vrvPedal->ReadPlacement(pedal);

    parent->AddChild(vrvPedal);
    return true;
}

bool MeiInput::ReadMeiSlur(Object *parent, pugi::xml_node slur)
{
    Slur *vrvSlur = new Slur();
    ReadControlElement(slur, vrvSlur);

    ReadTimeSpanningInterface(slur, vrvSlur);
    vrvSlur->ReadColor(slur);
    vrvSlur->ReadCurvature(slur);

    parent->AddChild(vrvSlur);
    return true;
}

bool MeiInput::ReadMeiTempo(Object *parent, pugi::xml_node tempo)
{
    Tempo *vrvTempo = new Tempo();
    ReadControlElement(tempo, vrvTempo);

    ReadTextDirInterface(tempo, vrvTempo);
    ReadTimePointInterface(tempo, vrvTempo);
    vrvTempo->ReadLang(tempo);
    vrvTempo->ReadMidiTempo(tempo);
    vrvTempo->ReadMmTempo(tempo);

    parent->AddChild(vrvTempo);
    return ReadMeiTextChildren(vrvTempo, tempo, vrvTempo);
}

bool MeiInput::ReadMeiTie(Object *parent, pugi::xml_node tie)
{
    Tie *vrvTie = new Tie();
    ReadControlElement(tie, vrvTie);

    ReadTimeSpanningInterface(tie, vrvTie);
    vrvTie->ReadColor(tie);
    vrvTie->ReadCurvature(tie);

    parent->AddChild(vrvTie);
    return true;
}

bool MeiInput::ReadMeiTrill(Object *parent, pugi::xml_node trill)
{
    Trill *vrvTrill = new Trill();
    ReadControlElement(trill, vrvTrill);

    ReadTimePointInterface(trill, vrvTrill);
    vrvTrill->ReadColor(trill);
    vrvTrill->ReadOrnamentAccid(trill);
    vrvTrill->ReadPlacement(trill);

    parent->AddChild(vrvTrill);
    return true;
}

bool MeiInput::ReadMeiTurn(Object *parent, pugi::xml_node turn)
{
    Turn *vrvTurn = new Turn();
    ReadControlElement(turn, vrvTurn);

    ReadTimePointInterface(turn, vrvTurn);
    vrvTurn->ReadColor(turn);
    vrvTurn->ReadOrnamentAccid(turn);
    vrvTurn->ReadPlacement(turn);
    vrvTurn->ReadTurnLog(turn);

    parent->AddChild(vrvTurn);
    return true;
}

bool MeiInput::ReadMeiFb(Object *parent, pugi::xml_node fb)
{
    Fb *vrvFb = new Fb();
    SetMeiUuid(fb, vrvFb);

    parent->AddChild(vrvFb);
    return ReadMeiFbChildren(vrvFb, fb);
}

bool MeiInput::ReadMeiFbChildren(Object *parent, pugi::xml_node parentNode)
{
    assert(dynamic_cast<Fb *>(parent) || dynamic_cast<EditorialElement *>(parent));

    bool success = true;
    pugi::xml_node current;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        if (!success) break;
        // editorial
        else if (IsEditorialElementName(current.name())) {
            success = ReadMeiEditorialElement(parent, current, EDITORIAL_FB);
        }
        // content
        else if (std::string(current.name()) == "f") {
            success = ReadMeiF(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <staff>", current.name());
        }
    }
    return success;
}

bool MeiInput::ReadMeiStaff(Object *parent, pugi::xml_node staff)
{
    Staff *vrvStaff = new Staff();
    SetMeiUuid(staff, vrvStaff);

    vrvStaff->ReadNInteger(staff);
    vrvStaff->ReadTyped(staff);
    vrvStaff->ReadVisibility(staff);

    if (staff.attribute("uly") && (this->m_doc->GetType() == Transcription)) {
        vrvStaff->m_yAbs = atoi(staff.attribute("uly").value()) * DEFINITION_FACTOR;
    }

    if (!vrvStaff->HasN() || (vrvStaff->GetN() == 0)) {
        LogWarning("No @n on <staff> or a value of 0 might yield unpredictable results");
    }

    parent->AddChild(vrvStaff);
    return ReadMeiStaffChildren(vrvStaff, staff);
}

bool MeiInput::ReadMeiStaffChildren(Object *parent, pugi::xml_node parentNode)
{
    assert(dynamic_cast<Staff *>(parent) || dynamic_cast<EditorialElement *>(parent));

    bool success = true;
    pugi::xml_node current;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        if (!success) break;
        // editorial
        else if (IsEditorialElementName(current.name())) {
            success = ReadMeiEditorialElement(parent, current, EDITORIAL_STAFF);
        }
        // content
        else if (std::string(current.name()) == "layer") {
            success = ReadMeiLayer(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <staff>", current.name());
        }
    }
    return success;
}

bool MeiInput::ReadMeiLayer(Object *parent, pugi::xml_node layer)
{
    Layer *vrvLayer = new Layer();
    SetMeiUuid(layer, vrvLayer);

    vrvLayer->ReadNInteger(layer);
    vrvLayer->ReadTyped(layer);
    vrvLayer->ReadVisibility(layer);

    if (!vrvLayer->HasN()) {
        LogWarning("No @n on <layer>, 1 is set");
    }
    else if (vrvLayer->GetN() == 0) {
        LogWarning("Value @n='1' on <layer> might yield unpredictable results");
    }

    parent->AddChild(vrvLayer);
    return ReadMeiLayerChildren(vrvLayer, layer);
}

bool MeiInput::ReadMeiLayerChildren(Object *parent, pugi::xml_node parentNode, Object *filter)
{
    bool success = true;
    pugi::xml_node xmlElement;
    std::string elementName;
    for (xmlElement = parentNode.first_child(); xmlElement; xmlElement = xmlElement.next_sibling()) {
        if (!success) {
            break;
        }
        elementName = std::string(xmlElement.name());
        // LogDebug("ReadMeiLayerChildren: element <%s>", xmlElement.name());
        if (!IsAllowed(elementName, filter)) {
            std::string meiElementName = filter->GetClassName();
            std::transform(meiElementName.begin(), meiElementName.begin() + 1, meiElementName.begin(), ::tolower);
            LogWarning("Element <%s> within <%s> is not supported and will be ignored ", xmlElement.name(),
                meiElementName.c_str());
            continue;
        }
        // editorial
        else if (IsEditorialElementName(xmlElement.name())) {
            success = ReadMeiEditorialElement(parent, xmlElement, EDITORIAL_LAYER, filter);
        }
        // content
        else if (elementName == "accid") {
            success = ReadMeiAccid(parent, xmlElement);
        }
        else if (elementName == "artic") {
            success = ReadMeiArtic(parent, xmlElement);
        }
        else if (elementName == "barLine") {
            success = ReadMeiBarLine(parent, xmlElement);
        }
        else if (elementName == "beam") {
            success = ReadMeiBeam(parent, xmlElement);
        }
        else if (elementName == "beatRpt") {
            success = ReadMeiBeatRpt(parent, xmlElement);
        }
        else if (elementName == "bTrem") {
            success = ReadMeiBTrem(parent, xmlElement);
        }
        else if (elementName == "chord") {
            success = ReadMeiChord(parent, xmlElement);
        }
        else if (elementName == "clef") {
            success = ReadMeiClef(parent, xmlElement);
        }
        else if (elementName == "custos") {
            success = ReadMeiCustos(parent, xmlElement);
        }
        else if (elementName == "dot") {
            success = ReadMeiDot(parent, xmlElement);
        }
        else if (elementName == "fTrem") {
            success = ReadMeiFTrem(parent, xmlElement);
        }
        else if (elementName == "keySig") {
            success = ReadMeiKeySig(parent, xmlElement);
        }
        else if (elementName == "ligature") {
            success = ReadMeiLigature(parent, xmlElement);
        }
        else if (elementName == "mensur") {
            success = ReadMeiMensur(parent, xmlElement);
        }
        else if (elementName == "meterSig") {
            success = ReadMeiMeterSig(parent, xmlElement);
        }
        else if (elementName == "note") {
            success = ReadMeiNote(parent, xmlElement);
        }
        else if (elementName == "rest") {
            success = ReadMeiRest(parent, xmlElement);
        }
        else if (elementName == "mRest") {
            success = ReadMeiMRest(parent, xmlElement);
        }
        else if (elementName == "mRpt") {
            success = ReadMeiMRpt(parent, xmlElement);
        }
        else if (elementName == "mRpt2") {
            success = ReadMeiMRpt2(parent, xmlElement);
        }
        else if (elementName == "multiRest") {
            success = ReadMeiMultiRest(parent, xmlElement);
        }
        else if (elementName == "multiRpt") {
            success = ReadMeiMultiRpt(parent, xmlElement);
        }
        else if (elementName == "proport") {
            success = ReadMeiProport(parent, xmlElement);
        }
        else if (elementName == "space") {
            success = ReadMeiSpace(parent, xmlElement);
        }
        else if (elementName == "syl") {
            success = ReadMeiSyl(parent, xmlElement);
        }
        else if (elementName == "tuplet") {
            success = ReadMeiTuplet(parent, xmlElement);
        }
        else if (elementName == "verse") {
            success = ReadMeiVerse(parent, xmlElement);
        }
        // unknown
        else {
            LogWarning("Element '%s' is unknown and will be ignored", xmlElement.name());
        }
    }
    return success;
}

bool MeiInput::ReadLayerElement(pugi::xml_node element, LayerElement *object)
{
    if (element.attribute("ulx") && (this->m_doc->GetType() == Transcription)) {
        object->m_xAbs = atoi(element.attribute("ulx").value()) * DEFINITION_FACTOR;
    }

    SetMeiUuid(element, object);
    object->ReadLabelled(element);
    object->ReadTyped(element);

    return true;
}

bool MeiInput::ReadMeiAccid(Object *parent, pugi::xml_node accid)
{
    Accid *vrvAccid = new Accid();
    ReadLayerElement(accid, vrvAccid);

    ReadPositionInterface(accid, vrvAccid);
    vrvAccid->ReadAccidental(accid);
    vrvAccid->ReadAccidentalGestural(accid);
    vrvAccid->ReadAccidLog(accid);
    vrvAccid->ReadColor(accid);
    vrvAccid->ReadEnclosingChars(accid);

    parent->AddChild(vrvAccid);
    return true;
}

bool MeiInput::ReadMeiArtic(Object *parent, pugi::xml_node artic)
{
    Artic *vrvArtic = new Artic();
    ReadLayerElement(artic, vrvArtic);

    vrvArtic->ReadArticulation(artic);
    vrvArtic->ReadColor(artic);
    vrvArtic->ReadPlacement(artic);

    parent->AddChild(vrvArtic);
    return true;
}

bool MeiInput::ReadMeiBarLine(Object *parent, pugi::xml_node barLine)
{
    BarLine *vrvBarLine = new BarLine();
    ReadLayerElement(barLine, vrvBarLine);

    vrvBarLine->ReadBarLineLog(barLine);
    vrvBarLine->ReadColor(barLine);

    parent->AddChild(vrvBarLine);
    return true;
}

bool MeiInput::ReadMeiBeam(Object *parent, pugi::xml_node beam)
{
    Beam *vrvBeam = new Beam();
    ReadLayerElement(beam, vrvBeam);

    vrvBeam->ReadColor(beam);

    parent->AddChild(vrvBeam);

    return ReadMeiLayerChildren(vrvBeam, beam, vrvBeam);
}

bool MeiInput::ReadMeiBeatRpt(Object *parent, pugi::xml_node beatRpt)
{
    BeatRpt *vrvBeatRpt = new BeatRpt();
    ReadLayerElement(beatRpt, vrvBeatRpt);

    vrvBeatRpt->ReadColor(beatRpt);
    vrvBeatRpt->ReadBeatRptVis(beatRpt);

    parent->AddChild(vrvBeatRpt);
    return true;
}

bool MeiInput::ReadMeiBTrem(Object *parent, pugi::xml_node bTrem)
{
    BTrem *vrvBTrem = new BTrem();
    ReadLayerElement(bTrem, vrvBTrem);

    vrvBTrem->ReadTremMeasured(bTrem);

    parent->AddChild(vrvBTrem);
    return ReadMeiLayerChildren(vrvBTrem, bTrem, vrvBTrem);
}

bool MeiInput::ReadMeiChord(Object *parent, pugi::xml_node chord)
{
    Chord *vrvChord = new Chord();
    ReadLayerElement(chord, vrvChord);

    ReadDurationInterface(chord, vrvChord);
    vrvChord->ReadColor(chord);
    vrvChord->ReadCue(chord);
    vrvChord->ReadGraced(chord);
    vrvChord->ReadStems(chord);
    vrvChord->ReadStemsCmn(chord);
    vrvChord->ReadTiePresent(chord);
    vrvChord->ReadVisibility(chord);

    AttArticulation artic;
    artic.ReadArticulation(chord);
    if (artic.HasArtic()) {
        Artic *vrvArtic = new Artic();
        vrvArtic->IsAttribute(true);
        vrvArtic->SetArtic(artic.GetArtic());
        vrvChord->AddChild(vrvArtic);
    }

    parent->AddChild(vrvChord);
    return ReadMeiLayerChildren(vrvChord, chord, vrvChord);
}

bool MeiInput::ReadMeiClef(Object *parent, pugi::xml_node clef)
{
    Clef *vrvClef = new Clef();
    ReadLayerElement(clef, vrvClef);

    vrvClef->ReadClefShape(clef);
    vrvClef->ReadColor(clef);
    vrvClef->ReadLineLoc(clef);
    vrvClef->ReadOctaveDisplacement(clef);

    parent->AddChild(vrvClef);
    return true;
}

bool MeiInput::ReadMeiCustos(Object *parent, pugi::xml_node custos)
{
    Custos *vrvCustos = new Custos();
    ReadLayerElement(custos, vrvCustos);

    ReadPositionInterface(custos, vrvCustos);

    vrvCustos->ReadColor(custos);

    parent->AddChild(vrvCustos);
    return true;
}

bool MeiInput::ReadMeiDot(Object *parent, pugi::xml_node dot)
{
    Dot *vrvDot = new Dot();
    ReadLayerElement(dot, vrvDot);

    ReadPositionInterface(dot, vrvDot);
    vrvDot->ReadColor(dot);
    vrvDot->ReadDotLog(dot);

    parent->AddChild(vrvDot);
    return true;
}

bool MeiInput::ReadMeiFTrem(Object *parent, pugi::xml_node fTrem)
{
    FTrem *vrvFTrem = new FTrem();
    ReadLayerElement(fTrem, vrvFTrem);

    vrvFTrem->ReadSlashCount(fTrem);
    vrvFTrem->ReadTremMeasured(fTrem);

    parent->AddChild(vrvFTrem);
    return ReadMeiLayerChildren(vrvFTrem, fTrem, vrvFTrem);
}

bool MeiInput::ReadMeiKeySig(Object *parent, pugi::xml_node keySig)
{
    KeySig *vrvKeySig = new KeySig();
    ReadLayerElement(keySig, vrvKeySig);

    vrvKeySig->ReadAccidental(keySig);
    vrvKeySig->ReadPitch(keySig);

    // special processing required
    vrvKeySig->ConvertToInternal();

    parent->AddChild(vrvKeySig);
    return true;
}

bool MeiInput::ReadMeiLigature(Object *parent, pugi::xml_node ligature)
{
    Ligature *vrvLigature = new Ligature();
    SetMeiUuid(ligature, vrvLigature);

    ReadDurationInterface(ligature, vrvLigature);
    vrvLigature->ReadStems(ligature);
    vrvLigature->ReadStemsCmn(ligature);
    vrvLigature->ReadTiePresent(ligature);

    parent->AddChild(vrvLigature);
    return ReadMeiLayerChildren(vrvLigature, ligature, vrvLigature);
}

bool MeiInput::ReadMeiMensur(Object *parent, pugi::xml_node mensur)
{
    Mensur *vrvMensur = new Mensur();
    ReadLayerElement(mensur, vrvMensur);

    vrvMensur->ReadColor(mensur);
    vrvMensur->ReadCue(mensur);
    vrvMensur->ReadDurationRatio(mensur);
    vrvMensur->ReadMensuralShared(mensur);
    vrvMensur->ReadMensurLog(mensur);
    vrvMensur->ReadMensurVis(mensur);
    vrvMensur->ReadSlashCount(mensur);

    parent->AddChild(vrvMensur);
    return true;
}

bool MeiInput::ReadMeiMeterSig(Object *parent, pugi::xml_node meterSig)
{
    MeterSig *vrvMeterSig = new MeterSig();
    ReadLayerElement(meterSig, vrvMeterSig);

    vrvMeterSig->ReadMeterSigLog(meterSig);
    vrvMeterSig->ReadMeterSigVis(meterSig);

    parent->AddChild(vrvMeterSig);
    return true;
}

bool MeiInput::ReadMeiMRest(Object *parent, pugi::xml_node mRest)
{
    MRest *vrvMRest = new MRest();
    ReadLayerElement(mRest, vrvMRest);
    ReadPositionInterface(mRest, vrvMRest);

    vrvMRest->ReadCue(mRest);
    vrvMRest->ReadFermataPresent(mRest);
    vrvMRest->ReadVisibility(mRest);

    parent->AddChild(vrvMRest);
    return true;
}

bool MeiInput::ReadMeiMRpt(Object *parent, pugi::xml_node mRpt)
{
    MRpt *vrvMRpt = new MRpt();
    ReadLayerElement(mRpt, vrvMRpt);

    parent->AddChild(vrvMRpt);
    return true;
}

bool MeiInput::ReadMeiMRpt2(Object *parent, pugi::xml_node mRpt2)
{
    MRpt2 *vrvMRpt2 = new MRpt2();
    ReadLayerElement(mRpt2, vrvMRpt2);

    parent->AddChild(vrvMRpt2);
    return true;
}

bool MeiInput::ReadMeiMultiRest(Object *parent, pugi::xml_node multiRest)
{
    MultiRest *vrvMultiRest = new MultiRest();
    ReadLayerElement(multiRest, vrvMultiRest);

    vrvMultiRest->ReadMultiRestVis(multiRest);
    vrvMultiRest->ReadNumbered(multiRest);

    parent->AddChild(vrvMultiRest);
    return true;
}

bool MeiInput::ReadMeiMultiRpt(Object *parent, pugi::xml_node multiRpt)
{
    MultiRpt *vrvMultiRpt = new MultiRpt();
    ReadLayerElement(multiRpt, vrvMultiRpt);

    vrvMultiRpt->ReadNumbered(multiRpt);

    parent->AddChild(vrvMultiRpt);
    return true;
}

bool MeiInput::ReadMeiNote(Object *parent, pugi::xml_node note)
{
    Note *vrvNote = new Note();
    ReadLayerElement(note, vrvNote);

    ReadDurationInterface(note, vrvNote);
    ReadPitchInterface(note, vrvNote);
    ReadPositionInterface(note, vrvNote);
    vrvNote->ReadColor(note);
    vrvNote->ReadColoration(note);
    vrvNote->ReadCue(note);
    vrvNote->ReadGraced(note);
    vrvNote->ReadNoteAnlMensural(note);
    vrvNote->ReadStems(note);
    vrvNote->ReadStemsCmn(note);
    vrvNote->ReadTiePresent(note);
    vrvNote->ReadVisibility(note);

    AttArticulation artic;
    artic.ReadArticulation(note);
    if (artic.HasArtic()) {
        Artic *vrvArtic = new Artic();
        vrvArtic->IsAttribute(true);
        vrvArtic->SetArtic(artic.GetArtic());
        vrvNote->AddChild(vrvArtic);
    }

    AttAccidental accidental;
    accidental.ReadAccidental(note);
    AttAccidentalGestural accidentalGestural;
    accidentalGestural.ReadAccidentalGestural(note);
    if (accidental.HasAccid() || accidentalGestural.HasAccidGes()) {
        Accid *vrvAccid = new Accid();
        vrvAccid->IsAttribute(true);
        vrvAccid->SetAccid(accidental.GetAccid());
        vrvAccid->SetAccidGes(accidentalGestural.GetAccidGes());
        vrvNote->AddChild(vrvAccid);
    }

    parent->AddChild(vrvNote);
    return ReadMeiLayerChildren(vrvNote, note, vrvNote);
}

bool MeiInput::ReadMeiRest(Object *parent, pugi::xml_node rest)
{
    Rest *vrvRest = new Rest();
    ReadLayerElement(rest, vrvRest);

    ReadDurationInterface(rest, vrvRest);
    ReadPositionInterface(rest, vrvRest);
    vrvRest->ReadColor(rest);
    vrvRest->ReadCue(rest);
    vrvRest->ReadRestVisMensural(rest);

    parent->AddChild(vrvRest);
    return true;
}

bool MeiInput::ReadMeiProport(Object *parent, pugi::xml_node proport)
{
    Proport *vrvProport = new Proport();
    ReadLayerElement(proport, vrvProport);

    vrvProport->ReadDurationRatio(proport);

    parent->AddChild(vrvProport);
    return true;
}

bool MeiInput::ReadMeiSpace(Object *parent, pugi::xml_node space)
{
    Space *vrvSpace = new Space();
    ReadLayerElement(space, vrvSpace);

    ReadDurationInterface(space, vrvSpace);

    parent->AddChild(vrvSpace);
    return true;
}

bool MeiInput::ReadMeiSyl(Object *parent, pugi::xml_node syl)
{
    Syl *vrvSyl = new Syl();
    ReadLayerElement(syl, vrvSyl);

    vrvSyl->ReadLang(syl);
    vrvSyl->ReadTypography(syl);
    vrvSyl->ReadSylLog(syl);

    parent->AddChild(vrvSyl);
    return ReadMeiTextChildren(vrvSyl, syl, vrvSyl);
}

bool MeiInput::ReadMeiTuplet(Object *parent, pugi::xml_node tuplet)
{
    Tuplet *vrvTuplet = new Tuplet();
    ReadLayerElement(tuplet, vrvTuplet);

    vrvTuplet->ReadColor(tuplet);
    vrvTuplet->ReadDurationRatio(tuplet);
    vrvTuplet->ReadNumberPlacement(tuplet);
    vrvTuplet->ReadTupletVis(tuplet);

    parent->AddChild(vrvTuplet);
    return ReadMeiLayerChildren(vrvTuplet, tuplet, vrvTuplet);
}

bool MeiInput::ReadMeiVerse(Object *parent, pugi::xml_node verse)
{
    Verse *vrvVerse = new Verse();
    ReadLayerElement(verse, vrvVerse);

    vrvVerse->ReadColor(verse);
    vrvVerse->ReadLang(verse);
    vrvVerse->ReadNInteger(verse);
    vrvVerse->ReadTypography(verse);

    parent->AddChild(vrvVerse);
    return ReadMeiLayerChildren(vrvVerse, verse, vrvVerse);
}

bool MeiInput::ReadMeiTextChildren(Object *parent, pugi::xml_node parentNode, Object *filter)
{
    bool success = true;
    pugi::xml_node xmlElement;
    std::string elementName;
    int i = 0;
    for (xmlElement = parentNode.first_child(); xmlElement; xmlElement = xmlElement.next_sibling()) {
        if (!success) {
            break;
        }
        elementName = std::string(xmlElement.name());
        if (filter && !IsAllowed(elementName, filter)) {
            std::string meiElementName = filter->GetClassName();
            std::transform(meiElementName.begin(), meiElementName.begin() + 1, meiElementName.begin(), ::tolower);
            LogWarning("Element <%s> within <%s> is not supported and will be ignored ", xmlElement.name(),
                meiElementName.c_str());
            continue;
        }
        // editorial
        else if (IsEditorialElementName(xmlElement.name())) {
            success = ReadMeiEditorialElement(parent, xmlElement, EDITORIAL_TEXT, filter);
        }
        // content
        else if (elementName == "rend") {
            success = ReadMeiRend(parent, xmlElement);
        }
        else if (xmlElement.text()) {
            bool trimLeft = (i == 0);
            bool trimRight = (!xmlElement.next_sibling());
            success = ReadMeiText(parent, xmlElement, trimLeft, trimRight);
        }
        // figured bass
        else if (elementName == "fb") {
            success = ReadMeiFb(parent, xmlElement);
        }
        // unknown
        else {
            LogWarning("Element %s is unknown and will be ignored", xmlElement.name());
        }
        i++;
    }
    return success;
}

bool MeiInput::ReadTextElement(pugi::xml_node element, TextElement *object)
{
    SetMeiUuid(element, object);
    object->ReadTyped(element);

    return true;
}

bool MeiInput::ReadMeiF(Object *parent, pugi::xml_node f)
{
    F *vrvF = new F();
    ReadTextElement(f, vrvF);

    parent->AddChild(vrvF);
    return ReadMeiTextChildren(vrvF, f);
}

bool MeiInput::ReadMeiRend(Object *parent, pugi::xml_node rend)
{
    Rend *vrvRend = new Rend();
    ReadTextElement(rend, vrvRend);

    vrvRend->ReadColor(rend);
    vrvRend->ReadHorizontalAlign(rend);
    vrvRend->ReadLang(rend);
    vrvRend->ReadTypography(rend);
    vrvRend->ReadWhitespace(rend);

    parent->AddChild(vrvRend);
    return ReadMeiTextChildren(vrvRend, rend);
}

bool MeiInput::ReadMeiText(Object *parent, pugi::xml_node text, bool trimLeft, bool trimRight)
{
    Text *vrvText = new Text();

    assert(text.text());
    std::wstring str = UTF8to16(text.text().as_string());
    if (trimLeft) str = this->LeftTrim(str);
    if (trimRight) str = this->RightTrim(str);

    vrvText->SetText(str);

    parent->AddChild(vrvText);
    return true;
}

bool MeiInput::ReadDurationInterface(pugi::xml_node element, DurationInterface *interface)
{
    interface->ReadAugmentDots(element);
    interface->ReadBeamSecondary(element);
    interface->ReadDurationGestural(element);
    interface->ReadDurationLogical(element);
    interface->ReadDurationRatio(element);
    interface->ReadFermataPresent(element);
    interface->ReadStaffIdent(element);
    return true;
}

bool MeiInput::ReadPitchInterface(pugi::xml_node element, PitchInterface *interface)
{
    interface->ReadNoteGes(element);
    interface->ReadOctave(element);
    interface->ReadPitch(element);
    return true;
}

bool MeiInput::ReadPositionInterface(pugi::xml_node element, PositionInterface *interface)
{
    interface->ReadStaffLoc(element);
    interface->ReadStaffLocPitched(element);
    return true;
}

bool MeiInput::ReadScoreDefInterface(pugi::xml_node element, ScoreDefInterface *interface)
{
    interface->ReadCleffingLog(element);
    interface->ReadCleffingVis(element);
    interface->ReadKeySigDefaultAnl(element);
    interface->ReadKeySigDefaultLog(element);
    interface->ReadKeySigDefaultVis(element);
    interface->ReadLyricStyle(element);
    interface->ReadMensuralLog(element);
    interface->ReadMensuralShared(element);
    interface->ReadMeterSigDefaultLog(element);
    interface->ReadMeterSigDefaultVis(element);
    interface->ReadMidiTempo(element);
    interface->ReadMultinumMeasures(element);
    return true;
}

bool MeiInput::ReadTextDirInterface(pugi::xml_node element, TextDirInterface *interface)
{
    interface->ReadPlacement(element);
    return true;
}

bool MeiInput::ReadTimePointInterface(pugi::xml_node element, TimePointInterface *interface)
{
    interface->ReadStaffIdent(element);
    interface->ReadStartId(element);
    interface->ReadTimestampLogical(element);
    return true;
}

bool MeiInput::ReadTimeSpanningInterface(pugi::xml_node element, TimeSpanningInterface *interface)
{
    ReadTimePointInterface(element, interface);
    interface->ReadStartEndId(element);
    interface->ReadTimestamp2Logical(element);
    return true;
}

void MeiInput::ReadUnsupportedAttr(pugi::xml_node element, Object *object)
{
    for (pugi::xml_attribute_iterator ait = element.attributes_begin(); ait != element.attributes_end(); ++ait) {
        object->m_unsupported.push_back(std::make_pair(ait->name(), ait->value()));
    }
}

bool MeiInput::ReadMeiEditorialElement(Object *parent, pugi::xml_node current, EditorialLevel level, Object *filter)
{
    if (std::string(current.name()) == "abbr") {
        return ReadMeiAbbr(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "add") {
        return ReadMeiAdd(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "app") {
        return ReadMeiApp(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "annot") {
        return ReadMeiAnnot(parent, current);
    }
    else if (std::string(current.name()) == "choice") {
        return ReadMeiChoice(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "corr") {
        return ReadMeiCorr(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "damage") {
        return ReadMeiDamage(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "del") {
        return ReadMeiDel(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "expan") {
        return ReadMeiExpan(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "orig") {
        return ReadMeiOrig(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "reg") {
        return ReadMeiReg(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "restore") {
        return ReadMeiRestore(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "sic") {
        return ReadMeiSic(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "supplied") {
        return ReadMeiSupplied(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "unclear") {
        return ReadMeiUnclear(parent, current, level, filter);
    }
    else {
        assert(false); // this should never happen, MeiInput::s_editorialElementNames should be updated
        return false;
    }
}

bool MeiInput::ReadEditorialElement(pugi::xml_node element, EditorialElement *object)
{
    SetMeiUuid(element, object);

    object->ReadLabelled(element);
    object->ReadTyped(element);

    return true;
}

bool MeiInput::ReadMeiAbbr(Object *parent, pugi::xml_node abbr, EditorialLevel level, Object *filter)
{
    Abbr *vrvAbbr = new Abbr();
    ReadEditorialElement(abbr, vrvAbbr);

    vrvAbbr->ReadSource(abbr);

    parent->AddChild(vrvAbbr);
    return ReadMeiEditorialChildren(vrvAbbr, abbr, level, filter);
}

bool MeiInput::ReadMeiAdd(Object *parent, pugi::xml_node add, EditorialLevel level, Object *filter)
{
    Add *vrvAdd = new Add();
    ReadEditorialElement(add, vrvAdd);

    vrvAdd->ReadSource(add);

    parent->AddChild(vrvAdd);
    return ReadMeiEditorialChildren(vrvAdd, add, level, filter);
}

bool MeiInput::ReadMeiAnnot(Object *parent, pugi::xml_node annot)
{
    Annot *vrvAnnot = new Annot();
    ReadEditorialElement(annot, vrvAnnot);

    vrvAnnot->ReadPlist(annot);
    vrvAnnot->ReadSource(annot);

    vrvAnnot->m_content.reset();
    // copy all the nodes inside into the document
    for (pugi::xml_node child = annot.first_child(); child; child = child.next_sibling()) {
        vrvAnnot->m_content.append_copy(child);
    }

    parent->AddChild(vrvAnnot);
    return ReadMeiTextChildren(vrvAnnot, annot, vrvAnnot);
}

bool MeiInput::ReadMeiApp(Object *parent, pugi::xml_node app, EditorialLevel level, Object *filter)
{
    if (!m_hasScoreDef) {
        LogError("<app> before any <scoreDef> is not supported");
        return false;
    }
    App *vrvApp = new App(level);
    ReadEditorialElement(app, vrvApp);

    parent->AddChild(vrvApp);

    return ReadMeiAppChildren(vrvApp, app, level, filter);
}

bool MeiInput::ReadMeiAppChildren(Object *parent, pugi::xml_node parentNode, EditorialLevel level, Object *filter)
{
    assert(dynamic_cast<App *>(parent));

    // Check if one child node matches the m_appXPathQuery
    pugi::xml_node selectedLemOrRdg;
    if (m_appXPathQueries.size() > 0) {
        auto i = std::find_if(m_appXPathQueries.begin(), m_appXPathQueries.end(),
            [parentNode](std::string &query) { return (parentNode.select_single_node(query.c_str())); });
        if (i != m_appXPathQueries.end()) selectedLemOrRdg = parentNode.select_single_node(i->c_str()).node();
    }

    bool success = true;
    bool hasXPathSelected = false;
    pugi::xml_node current;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        if (!success) break;
        if (std::string(current.name()) == "lem") {
            success = ReadMeiLem(parent, current, level, filter);
        }
        else if (std::string(current.name()) == "rdg") {
            success = ReadMeiRdg(parent, current, level, filter);
        }
        else {
            LogWarning("Unsupported '<%s>' within <app>", current.name());
        }
        // Now we check if the xpath selection (if any) matches the current node.
        // If yes, make it visible
        if (selectedLemOrRdg == current) {
            EditorialElement *last = dynamic_cast<EditorialElement *>(parent->GetLast());
            if (last) {
                last->m_visibility = Visible;
                hasXPathSelected = true;
            }
        }
    }

    // If no child was made visible through the xpath selection, make the first one visible
    if (!hasXPathSelected) {
        EditorialElement *first = dynamic_cast<EditorialElement *>(parent->GetFirst());
        if (first) {
            first->m_visibility = Visible;
        }
        else {
            LogWarning("Could not make one <rdg> or <lem> visible");
        }
    }
    return success;
}

bool MeiInput::ReadMeiChoice(Object *parent, pugi::xml_node choice, EditorialLevel level, Object *filter)
{
    if (!m_hasScoreDef) {
        LogError("<choice> before any <scoreDef> is not supported");
        return false;
    }
    Choice *vrvChoice = new Choice(level);
    ReadEditorialElement(choice, vrvChoice);

    parent->AddChild(vrvChoice);

    return ReadMeiChoiceChildren(vrvChoice, choice, level, filter);
}

bool MeiInput::ReadMeiChoiceChildren(Object *parent, pugi::xml_node parentNode, EditorialLevel level, Object *filter)
{
    assert(dynamic_cast<Choice *>(parent));

    // Check if one child node matches a value in m_choiceXPathQueries
    pugi::xml_node selectedChild;
    if (m_choiceXPathQueries.size() > 0) {
        auto i = std::find_if(m_choiceXPathQueries.begin(), m_choiceXPathQueries.end(),
            [parentNode](std::string &query) { return (parentNode.select_single_node(query.c_str())); });
        if (i != m_choiceXPathQueries.end()) selectedChild = parentNode.select_single_node(i->c_str()).node();
    }

    bool success = true;
    bool hasXPathSelected = false;
    pugi::xml_node current;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        if (!success) break;
        if (std::string(current.name()) == "abbr") {
            success = ReadMeiAbbr(parent, current, level, filter);
        }
        else if (std::string(current.name()) == "choice") {
            success = ReadMeiChoice(parent, current, level, filter);
        }
        else if (std::string(current.name()) == "corr") {
            success = ReadMeiCorr(parent, current, level, filter);
        }
        else if (std::string(current.name()) == "expan") {
            success = ReadMeiExpan(parent, current, level, filter);
        }
        else if (std::string(current.name()) == "orig") {
            success = ReadMeiOrig(parent, current, level, filter);
        }
        else if (std::string(current.name()) == "reg") {
            success = ReadMeiReg(parent, current, level, filter);
        }
        else if (std::string(current.name()) == "sic") {
            success = ReadMeiSic(parent, current, level, filter);
        }
        else if (std::string(current.name()) == "unclear") {
            success = ReadMeiUnclear(parent, current, level, filter);
        }
        else {
            LogWarning("Unsupported '<%s>' within <choice>", current.name());
        }
        // Now we check if the xpath selection (if any) matches the current node.
        // If yes, make it visible
        EditorialElement *last = dynamic_cast<EditorialElement *>(parent->GetLast());
        if (success && last) {
            if (selectedChild == current) {
                last->m_visibility = Visible;
                hasXPathSelected = true;
            }
            else {
                last->m_visibility = Hidden;
            }
        }
    }

    // If no child was made visible through the xpath selection, make the first one visible
    if (!hasXPathSelected) {
        EditorialElement *first = dynamic_cast<EditorialElement *>(parent->GetFirst());
        if (first) {
            first->m_visibility = Visible;
        }
        else {
            LogWarning("Could not make one child of <choice> visible");
        }
    }
    return success;
}

bool MeiInput::ReadMeiCorr(Object *parent, pugi::xml_node corr, EditorialLevel level, Object *filter)
{
    Corr *vrvCorr = new Corr();
    ReadEditorialElement(corr, vrvCorr);

    vrvCorr->ReadSource(corr);

    parent->AddChild(vrvCorr);
    return ReadMeiEditorialChildren(vrvCorr, corr, level, filter);
}

bool MeiInput::ReadMeiDamage(Object *parent, pugi::xml_node damage, EditorialLevel level, Object *filter)
{
    Damage *vrvDamage = new Damage();
    ReadEditorialElement(damage, vrvDamage);

    vrvDamage->ReadSource(damage);

    parent->AddChild(vrvDamage);
    return ReadMeiEditorialChildren(vrvDamage, damage, level, filter);
}

bool MeiInput::ReadMeiDel(Object *parent, pugi::xml_node del, EditorialLevel level, Object *filter)
{
    Del *vrvDel = new Del();
    ReadEditorialElement(del, vrvDel);

    vrvDel->ReadSource(del);

    parent->AddChild(vrvDel);
    return ReadMeiEditorialChildren(vrvDel, del, level, filter);
}

bool MeiInput::ReadMeiExpan(Object *parent, pugi::xml_node expan, EditorialLevel level, Object *filter)
{
    Expan *vrvExpan = new Expan();
    ReadEditorialElement(expan, vrvExpan);

    vrvExpan->ReadSource(expan);

    parent->AddChild(vrvExpan);
    return ReadMeiEditorialChildren(vrvExpan, expan, level, filter);
}

bool MeiInput::ReadMeiLem(Object *parent, pugi::xml_node lem, EditorialLevel level, Object *filter)
{
    assert(dynamic_cast<App *>(parent));

    Lem *vrvLem = new Lem();
    // By default make them all hidden. MeiInput::ReadMeiAppChildren will make one visible.
    vrvLem->m_visibility = Hidden;
    ReadEditorialElement(lem, vrvLem);

    vrvLem->ReadSource(lem);

    parent->AddChild(vrvLem);
    return ReadMeiEditorialChildren(vrvLem, lem, level, filter);
}

bool MeiInput::ReadMeiOrig(Object *parent, pugi::xml_node orig, EditorialLevel level, Object *filter)
{
    Orig *vrvOrig = new Orig();
    ReadEditorialElement(orig, vrvOrig);

    vrvOrig->ReadSource(orig);

    parent->AddChild(vrvOrig);
    return ReadMeiEditorialChildren(vrvOrig, orig, level, filter);
}

bool MeiInput::ReadMeiRdg(Object *parent, pugi::xml_node rdg, EditorialLevel level, Object *filter)
{
    assert(dynamic_cast<App *>(parent));

    Rdg *vrvRdg = new Rdg();
    // By default make them all hidden. MeiInput::ReadMeiAppChildren will make one visible.
    vrvRdg->m_visibility = Hidden;
    ReadEditorialElement(rdg, vrvRdg);

    vrvRdg->ReadSource(rdg);

    parent->AddChild(vrvRdg);
    return ReadMeiEditorialChildren(vrvRdg, rdg, level, filter);
}

bool MeiInput::ReadMeiReg(Object *parent, pugi::xml_node reg, EditorialLevel level, Object *filter)
{
    Reg *vrvReg = new Reg();
    ReadEditorialElement(reg, vrvReg);

    vrvReg->ReadSource(reg);

    parent->AddChild(vrvReg);
    return ReadMeiEditorialChildren(vrvReg, reg, level, filter);
}

bool MeiInput::ReadMeiRestore(Object *parent, pugi::xml_node restore, EditorialLevel level, Object *filter)
{
    Restore *vrvRestore = new Restore();
    ReadEditorialElement(restore, vrvRestore);

    vrvRestore->ReadSource(restore);

    parent->AddChild(vrvRestore);
    return ReadMeiEditorialChildren(vrvRestore, restore, level, filter);
}

bool MeiInput::ReadMeiSic(Object *parent, pugi::xml_node sic, EditorialLevel level, Object *filter)
{
    Sic *vrvSic = new Sic();
    ReadEditorialElement(sic, vrvSic);

    vrvSic->ReadSource(sic);

    parent->AddChild(vrvSic);
    return ReadMeiEditorialChildren(vrvSic, sic, level, filter);
}

bool MeiInput::ReadMeiSupplied(Object *parent, pugi::xml_node supplied, EditorialLevel level, Object *filter)
{
    Supplied *vrvSupplied = new Supplied();
    ReadEditorialElement(supplied, vrvSupplied);

    vrvSupplied->ReadSource(supplied);

    parent->AddChild(vrvSupplied);
    return ReadMeiEditorialChildren(vrvSupplied, supplied, level, filter);
}

bool MeiInput::ReadMeiUnclear(Object *parent, pugi::xml_node unclear, EditorialLevel level, Object *filter)
{
    Unclear *vrvUnclear = new Unclear();
    ReadEditorialElement(unclear, vrvUnclear);

    vrvUnclear->ReadSource(unclear);

    parent->AddChild(vrvUnclear);
    return ReadMeiEditorialChildren(vrvUnclear, unclear, level, filter);
}

bool MeiInput::ReadMeiEditorialChildren(Object *parent, pugi::xml_node parentNode, EditorialLevel level, Object *filter)
{
    assert(dynamic_cast<EditorialElement *>(parent));

    if (level == EDITORIAL_TOPLEVEL) {
        if (m_readingScoreBased)
            return ReadMeiSectionChildren(parent, parentNode);
        else
            return ReadMeiSystemChildren(parent, parentNode);
    }
    else if (level == EDITORIAL_SCOREDEF) {
        return ReadMeiScoreDefChildren(parent, parentNode);
    }
    else if (level == EDITORIAL_STAFFGRP) {
        return ReadMeiStaffGrpChildren(parent, parentNode);
    }
    else if (level == EDITORIAL_MEASURE) {
        return ReadMeiMeasureChildren(parent, parentNode);
    }
    else if (level == EDITORIAL_STAFF) {
        return ReadMeiStaffChildren(parent, parentNode);
    }
    else if (level == EDITORIAL_LAYER) {
        return ReadMeiLayerChildren(parent, parentNode, filter);
    }
    else if (level == EDITORIAL_TEXT) {
        return ReadMeiTextChildren(parent, parentNode, filter);
    }
    else if (level == EDITORIAL_FB) {
        return ReadMeiFbChildren(parent, parentNode);
    }
    else {
        return false;
    }
}

bool MeiInput::ReadScoreBasedMei(pugi::xml_node element, Score *parent)
{
    bool success = true;
    // nested mdiv
    if (std::string(element.name()) == "mdiv") {
        if (!element.first_child()) {
            LogError("No <mdiv> child found");
            return false;
        }
        success = ReadScoreBasedMei(element.first_child(), parent);
    }
    else if (std::string(element.name()) == "score") {
        pugi::xml_node scoreDef = element.first_child();
        if (!scoreDef || (std::string(scoreDef.name()) != "scoreDef")) {
            LogError("A <scoreDef> is required as first child of <score>");
            return false;
        }
        // This actually sets the Doc::m_scoreDef
        success = ReadMeiScoreDef(parent, scoreDef);
        if (!success) return false;
        pugi::xml_node current;
        for (current = scoreDef.next_sibling(); current; current = current.next_sibling()) {
            if (!success) break;
            std::string elementName = std::string(current.name());

            // editorial
            if (IsEditorialElementName(current.name())) {
                success = ReadMeiEditorialElement(parent, current, EDITORIAL_TOPLEVEL);
            }
            // content
            else if (elementName == "section") {
                success = ReadMeiSection(parent, current);
            }
            else {
                LogWarning("Element <%s> within <score> is not supported and will be ignored ", elementName.c_str());
            }
        }
    }
    else {
        LogWarning("Elements <%s> ignored", element.name());
    }
    return success;
}

bool MeiInput::ReadTupletSpanAsTuplet(Measure *measure, pugi::xml_node tupletSpan)
{
    if (!measure) {
        LogWarning("Cannot read <tupleSpan> within editorial markup");
        return false;
    }

    Tuplet *tuplet = new Tuplet();
    SetMeiUuid(tupletSpan, tuplet);

    LayerElement *start = NULL;
    LayerElement *end = NULL;

    // label
    if (tupletSpan.attribute("label")) {
        tuplet->SetLabel(tupletSpan.attribute("label").value());
    }

    // Read in the numerator and denominator properties
    if (tupletSpan.attribute("num")) {
        tuplet->SetNum(atoi(tupletSpan.attribute("num").value()));
    }
    if (tupletSpan.attribute("numbase")) {
        tuplet->SetNumbase(atoi(tupletSpan.attribute("numbase").value()));
    }

    // position (pitch)
    if (tupletSpan.attribute("startid")) {
        std::string refId = ExtractUuidFragment(tupletSpan.attribute("startid").value());
        start = dynamic_cast<LayerElement *>(measure->FindChildByUuid(refId));
        if (!start) {
            LogWarning("Element with @startid '%s' not found when trying to read the <tupletSpan>", refId.c_str());
        }
    }
    if (tupletSpan.attribute("endid")) {
        std::string refId = ExtractUuidFragment(tupletSpan.attribute("endid").value());
        end = dynamic_cast<LayerElement *>(measure->FindChildByUuid(refId));
        if (!end) {
            LogWarning("Element with @endid '%s' not found when trying to read the <tupletSpan>", refId.c_str());
        }
    }
    if (!start || !end) {
        delete tuplet;
        return false;
    }

    LayerElement *startChild = dynamic_cast<LayerElement *>(start->GetLastParentNot(LAYER));
    LayerElement *endChild = dynamic_cast<LayerElement *>(end->GetLastParentNot(LAYER));

    if (!startChild || !endChild || (startChild->GetParent() != endChild->GetParent())) {
        LogWarning("Start and end elements for <tupletSpan> '%s' not in the same layer", tuplet->GetUuid().c_str());
        delete tuplet;
        return false;
    }

    Layer *parentLayer = dynamic_cast<Layer *>(startChild->GetParent());
    assert(parentLayer);

    int startIdx = startChild->GetIdx();
    int endIdx = endChild->GetIdx();
    // LogDebug("%d %d %s!", startIdx, endIdx, start->GetUuid().c_str());
    int i;
    for (i = endIdx; i >= startIdx; i--) {
        LayerElement *element = dynamic_cast<LayerElement *>(parentLayer->DetachChild(i));
        if (element) tuplet->AddChild(element);
    }
    tuplet->SetParent(parentLayer);
    parentLayer->InsertChild(tuplet, startIdx);

    return true;
}

void MeiInput::SetMeiUuid(pugi::xml_node element, Object *object)
{
    if (!element.attribute("xml:id")) {
        return;
    }

    object->SetUuid(element.attribute("xml:id").value());
    element.remove_attribute("xml:id");
}

DocType MeiInput::StrToDocType(std::string type)
{
    if (type == "raw") return Raw;
    if (type == "rendering") return Rendering;
    if (type == "transcription") return Transcription;
    LogWarning("Unknown layout type '%s'", type.c_str());
    return Raw;
}

std::string MeiInput::ExtractUuidFragment(std::string refUuid)
{
    size_t pos = refUuid.find_last_of("#");
    if ((pos != std::string::npos) && (pos < refUuid.length() - 1)) {
        refUuid = refUuid.substr(pos + 1);
    }
    return refUuid;
}

std::wstring MeiInput::LeftTrim(std::wstring str)
{
    std::wstring::size_type pos = 0;
    while (pos < str.size() && iswspace(str[pos])) pos++;
    str.erase(0, pos);
    return str;
}

std::wstring MeiInput::RightTrim(std::wstring str)
{
    std::wstring::size_type pos = str.size();
    while (pos > 0 && iswspace(str[pos - 1])) pos--;
    str.erase(pos);
    return str;
}

bool MeiInput::IsEditorialElementName(std::string elementName)
{
    auto i = std::find(MeiInput::s_editorialElementNames.begin(), MeiInput::s_editorialElementNames.end(), elementName);
    if (i != MeiInput::s_editorialElementNames.end()) return true;
    return false;
}

void MeiInput::UpgradeStaffDefTo_4_0_0(pugi::xml_node staffDef, StaffDef *vrvStaffDef)
{
    if (staffDef.attribute("label")) {
        Text *text = new Text();
        text->SetText(UTF8to16(staffDef.attribute("label").value()));
        Label *label = new Label();
        label->AddChild(text);
        vrvStaffDef->AddChild(label);
        staffDef.remove_attribute("label");
    }
    if (staffDef.attribute("label.abbr")) {
        Text *text = new Text();
        text->SetText(UTF8to16(staffDef.attribute("label.abbr").value()));
        LabelAbbr *labelAbbr = new LabelAbbr();
        labelAbbr->AddChild(text);
        vrvStaffDef->AddChild(labelAbbr);
        staffDef.remove_attribute("label.abbr");
    }
}

void MeiInput::UpgradeStaffGrpTo_4_0_0(pugi::xml_node staffGrp, StaffGrp *vrvStaffGrp)
{
    if (staffGrp.attribute("label")) {
        Text *text = new Text();
        text->SetText(UTF8to16(staffGrp.attribute("label").value()));
        Label *label = new Label();
        label->AddChild(text);
        vrvStaffGrp->AddChild(label);
        staffGrp.remove_attribute("label");
    }
    if (staffGrp.attribute("label.abbr")) {
        Text *text = new Text();
        text->SetText(UTF8to16(staffGrp.attribute("label.abbr").value()));
        LabelAbbr *labelAbbr = new LabelAbbr();
        labelAbbr->AddChild(text);
        vrvStaffGrp->AddChild(labelAbbr);
        staffGrp.remove_attribute("label.abbr");
    }
}

void MeiInput::UpgradeMeasureTo_3_0_0(Measure *measure, System *system)
{
    assert(measure);
    assert(system);
    assert(!measure->IsMeasuredMusic());

    if (system->m_yAbs == VRV_UNSET) return;
    if (system->m_systemRightMar == VRV_UNSET) return;
    if (system->m_systemRightMar == VRV_UNSET) return;

    Page *page = dynamic_cast<Page *>(system->GetFirstParent(PAGE));
    assert(page);
    measure->m_xAbs = system->m_systemLeftMar;
    measure->m_xAbs2 = page->m_pageWidth - system->m_systemRightMar;
}

void MeiInput::UpgradePageTo_3_0_0(Page *page, Doc *doc)
{
    assert(page);
    assert(doc);

    // Once we have the GetPPU in Page through LibMEI, call this from Doc::SetDrawingPage and
    // use m_unit instead of DEFAULT_UNIT - For the upgraded call Page->SetPPU(12.5);

    page->m_PPUFactor = (25.0 / 2.0 / DEFAULT_UNIT);
    // LogDebug("PPUFactor: %f", m_PPUFactor);
}

} // namespace vrv
