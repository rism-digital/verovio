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
#include "beam.h"
#include "chord.h"
#include "custos.h"
#include "dir.h"
#include "dot.h"
#include "dynam.h"
#include "editorial.h"
#include "hairpin.h"
#include "keysig.h"
#include "layer.h"
#include "measure.h"
#include "mensur.h"
#include "metersig.h"
#include "mrest.h"
#include "multirest.h"
#include "note.h"
#include "page.h"
#include "proport.h"
#include "rest.h"
#include "rpt.h"
#include "slur.h"
#include "space.h"
#include "staff.h"
#include "syl.h"
#include "system.h"
#include "tempo.h"
#include "text.h"
#include "tie.h"
#include "tuplet.h"
#include "verse.h"
#include "vrv.h"

namespace vrv {

#define EDIT_NAMES 14
std::string MeiInput::s_editorialElementNames[] = { "abbr", "add", "app", "annot", "corr", "damage", "del", "expan",
    "orig", "reg", "restore", "sic", "supplied", "unclear" }; // update EDIT_NAMES (above) accordingly

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
            decl.append_attribute("href") = "http://music-encoding.org/schema/2.1.1/mei-all.rng";
            decl.append_attribute("type") = "application/xml";
            decl.append_attribute("schematypens") = "http://relaxng.org/ns/structure/1.0";

            // schematron processing instruction
            decl = meiDoc.append_child(pugi::node_declaration);
            decl.set_name("xml-model");
            decl.append_attribute("href") = "http://music-encoding.org/schema/2.1.1/mei-all.rng";
            decl.append_attribute("type") = "application/xml";
            decl.append_attribute("schematypens") = "http://purl.oclc.org/dsdl/schematron";

            m_mei = meiDoc.append_child("mei");
            m_mei.append_attribute("xmlns") = "http://www.music-encoding.org/ns/mei";
            m_mei.append_attribute("meiversion") = "2013";

            // this starts the call of all the functors
            m_doc->Save(this);
        }
        else {
            if (m_page >= m_doc->GetPageCount()) {
                LogError("Page %d does not exist", m_page);
                return false;
            }
            Page *page = dynamic_cast<Page *>(m_doc->m_children.at(m_page));
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
    // Containers and scoreDef related
    if (object->Is() == DOC) {
        WriteMeiDoc(dynamic_cast<Doc *>(object));
        m_nodeStack.push_back(m_currentNode);
        return true;
    }

    if (object->Is() == PAGE) {
        if (!m_scoreBasedMEI) {
            m_currentNode = m_currentNode.append_child("page");
            WriteMeiPage(m_currentNode, dynamic_cast<Page *>(object));
        }
        // Here we could add a <pb> element
    }
    else if (object->Is() == SYSTEM) {
        if (!m_scoreBasedMEI) {
            m_currentNode = m_currentNode.append_child("system");
            WriteMeiSystem(m_currentNode, dynamic_cast<System *>(object));
        }
        // Here we could add a <sb> element (but not for the first system of the page?)
    }
    else if (object->Is() == SCOREDEF) {
        m_currentNode = m_currentNode.append_child("scoreDef");
        WriteMeiScoreDef(m_currentNode, dynamic_cast<ScoreDef *>(object));
    }
    else if (object->Is() == STAFFGRP) {
        m_currentNode = m_currentNode.append_child("staffGrp");
        WriteMeiStaffGrp(m_currentNode, dynamic_cast<StaffGrp *>(object));
    }
    else if (object->Is() == STAFFDEF) {
        m_currentNode = m_currentNode.append_child("staffDef");
        WriteMeiStaffDef(m_currentNode, dynamic_cast<StaffDef *>(object));
    }
    else if (object->Is() == MEASURE) {
        m_currentNode = m_currentNode.append_child("measure");
        WriteMeiMeasure(m_currentNode, dynamic_cast<Measure *>(object));
    }
    else if (object->Is() == STAFF) {
        m_currentNode = m_currentNode.append_child("staff");
        WriteMeiStaff(m_currentNode, dynamic_cast<Staff *>(object));
    }
    else if (object->Is() == LAYER) {
        m_currentNode = m_currentNode.append_child("layer");
        WriteMeiLayer(m_currentNode, dynamic_cast<Layer *>(object));
    }

    // Measure elements
    else if (object->Is() == ANCHORED_TEXT) {
        m_currentNode = m_currentNode.append_child("anchoredText");
        WriteMeiAnchoredText(m_currentNode, dynamic_cast<AnchoredText *>(object));
    }
    else if (object->Is() == DIR) {
        m_currentNode = m_currentNode.append_child("dir");
        WriteMeiDir(m_currentNode, dynamic_cast<Dir *>(object));
    }
    else if (object->Is() == DYNAM) {
        m_currentNode = m_currentNode.append_child("dynam");
        WriteMeiDynam(m_currentNode, dynamic_cast<Dynam *>(object));
    }
    else if (object->Is() == HAIRPIN) {
        m_currentNode = m_currentNode.append_child("hairpin");
        WriteMeiHairpin(m_currentNode, dynamic_cast<Hairpin *>(object));
    }
    else if (object->Is() == SLUR) {
        m_currentNode = m_currentNode.append_child("slur");
        WriteMeiSlur(m_currentNode, dynamic_cast<Slur *>(object));
    }
    else if (object->Is() == TEMPO) {
        m_currentNode = m_currentNode.append_child("tempo");
        WriteMeiTempo(m_currentNode, dynamic_cast<Tempo *>(object));
    }
    else if (object->Is() == TIE) {
        m_currentNode = m_currentNode.append_child("tie");
        WriteMeiTie(m_currentNode, dynamic_cast<Tie *>(object));
    }

    // Layer elements
    else if (object->Is() == ACCID) {
        m_currentNode = m_currentNode.append_child("accid");
        WriteMeiAccid(m_currentNode, dynamic_cast<Accid *>(object));
    }
    else if (object->Is() == BARLINE) {
        m_currentNode = m_currentNode.append_child("barLine");
        WriteMeiBarLine(m_currentNode, dynamic_cast<BarLine *>(object));
    }
    else if (object->Is() == BEAM) {
        m_currentNode = m_currentNode.append_child("beam");
        WriteMeiBeam(m_currentNode, dynamic_cast<Beam *>(object));
    }
    else if (object->Is() == BEATRPT) {
        m_currentNode = m_currentNode.append_child("beatRpt");
        WriteMeiBeatRpt(m_currentNode, dynamic_cast<BeatRpt *>(object));
    }
    else if (object->Is() == BTREM) {
        m_currentNode = m_currentNode.append_child("bTrem");
        WriteMeiBTrem(m_currentNode, dynamic_cast<BTrem *>(object));
    }
    else if (object->Is() == CHORD) {
        m_currentNode = m_currentNode.append_child("chord");
        WriteMeiChord(m_currentNode, dynamic_cast<Chord *>(object));
    }
    else if (object->Is() == CLEF) {
        m_currentNode = m_currentNode.append_child("clef");
        WriteMeiClef(m_currentNode, dynamic_cast<Clef *>(object));
    }
    else if (object->Is() == CUSTOS) {
        m_currentNode = m_currentNode.append_child("custos");
        WriteMeiCustos(m_currentNode, dynamic_cast<Custos *>(object));
    }
    else if (object->Is() == DOT) {
        m_currentNode = m_currentNode.append_child("dot");
        WriteMeiDot(m_currentNode, dynamic_cast<Dot *>(object));
    }
    else if (object->Is() == FTREM) {
        m_currentNode = m_currentNode.append_child("fTrem");
        WriteMeiFTrem(m_currentNode, dynamic_cast<FTrem *>(object));
    }
    else if (object->Is() == KEYSIG) {
        m_currentNode = m_currentNode.append_child("keySig");
        WriteMeiKeySig(m_currentNode, dynamic_cast<KeySig *>(object));
    }
    else if (object->Is() == MENSUR) {
        m_currentNode = m_currentNode.append_child("mensur");
        WriteMeiMensur(m_currentNode, dynamic_cast<Mensur *>(object));
    }
    else if (object->Is() == METERSIG) {
        m_currentNode = m_currentNode.append_child("meterSig");
        WriteMeiMeterSig(m_currentNode, dynamic_cast<MeterSig *>(object));
    }
    else if (object->Is() == MREST) {
        m_currentNode = m_currentNode.append_child("mRest");
        WriteMeiMRest(m_currentNode, dynamic_cast<MRest *>(object));
    }
    else if (object->Is() == MRPT) {
        m_currentNode = m_currentNode.append_child("mRpt");
        WriteMeiMRpt(m_currentNode, dynamic_cast<MRpt *>(object));
    }
    else if (object->Is() == MRPT2) {
        m_currentNode = m_currentNode.append_child("mRpt2");
        WriteMeiMRpt2(m_currentNode, dynamic_cast<MRpt2 *>(object));
    }
    else if (object->Is() == MULTIREST) {
        m_currentNode = m_currentNode.append_child("multiRest");
        WriteMeiMultiRest(m_currentNode, dynamic_cast<MultiRest *>(object));
    }
    else if (object->Is() == MULTIRPT) {
        m_currentNode = m_currentNode.append_child("multiRpt");
        WriteMeiMultiRpt(m_currentNode, dynamic_cast<MultiRpt *>(object));
    }
    else if (object->Is() == NOTE) {
        m_currentNode = m_currentNode.append_child("note");
        WriteMeiNote(m_currentNode, dynamic_cast<Note *>(object));
    }
    else if (object->Is() == PROPORT) {
        m_currentNode = m_currentNode.append_child("proport");
        WriteMeiProport(m_currentNode, dynamic_cast<Proport *>(object));
    }
    else if (object->Is() == REST) {
        m_currentNode = m_currentNode.append_child("rest");
        WriteMeiRest(m_currentNode, dynamic_cast<Rest *>(object));
    }
    else if (object->Is() == SPACE) {
        m_currentNode = m_currentNode.append_child("space");
        WriteMeiSpace(m_currentNode, dynamic_cast<Space *>(object));
    }
    else if (object->Is() == SYL) {
        m_currentNode = m_currentNode.append_child("syl");
        WriteMeiSyl(m_currentNode, dynamic_cast<Syl *>(object));
    }
    else if (object->Is() == TUPLET) {
        m_currentNode = m_currentNode.append_child("tuplet");
        WriteMeiTuplet(m_currentNode, dynamic_cast<Tuplet *>(object));
    }
    else if (object->Is() == VERSE) {
        m_currentNode = m_currentNode.append_child("verse");
        WriteMeiVerse(m_currentNode, dynamic_cast<Verse *>(object));
    }

    // Text elements
    else if (object->Is() == REND) {
        m_currentNode = m_currentNode.append_child("rend");
        WriteMeiRend(m_currentNode, dynamic_cast<Rend *>(object));
    }
    else if (object->Is() == TEXT) {
        WriteMeiText(m_currentNode, dynamic_cast<Text *>(object));
    }

    // Editorial markup
    else if (object->Is() == ABBR) {
        m_currentNode = m_currentNode.append_child("abbr");
        WriteMeiAbbr(m_currentNode, dynamic_cast<Abbr *>(object));
    }
    else if (object->Is() == ADD) {
        m_currentNode = m_currentNode.append_child("add");
        WriteMeiAdd(m_currentNode, dynamic_cast<Add *>(object));
    }
    else if (object->Is() == ANNOT) {
        m_currentNode = m_currentNode.append_child("annot");
        WriteMeiAnnot(m_currentNode, dynamic_cast<Annot *>(object));
    }
    else if (object->Is() == APP) {
        m_currentNode = m_currentNode.append_child("app");
        WriteMeiApp(m_currentNode, dynamic_cast<App *>(object));
    }
    else if (object->Is() == CORR) {
        m_currentNode = m_currentNode.append_child("corr");
        WriteMeiCorr(m_currentNode, dynamic_cast<Corr *>(object));
    }
    else if (object->Is() == DAMAGE) {
        m_currentNode = m_currentNode.append_child("damage");
        WriteMeiDamage(m_currentNode, dynamic_cast<Damage *>(object));
    }
    else if (object->Is() == DEL) {
        m_currentNode = m_currentNode.append_child("del");
        WriteMeiDel(m_currentNode, dynamic_cast<Del *>(object));
    }
    else if (object->Is() == EXPAN) {
        m_currentNode = m_currentNode.append_child("epxan");
        WriteMeiExpan(m_currentNode, dynamic_cast<Expan *>(object));
    }
    else if (object->Is() == LEM) {
        m_currentNode = m_currentNode.append_child("lem");
        WriteMeiLem(m_currentNode, dynamic_cast<Lem *>(object));
    }
    else if (object->Is() == ORIG) {
        m_currentNode = m_currentNode.append_child("orig");
        WriteMeiOrig(m_currentNode, dynamic_cast<Orig *>(object));
    }
    else if (object->Is() == RDG) {
        m_currentNode = m_currentNode.append_child("rdg");
        WriteMeiRdg(m_currentNode, dynamic_cast<Rdg *>(object));
    }
    else if (object->Is() == REG) {
        m_currentNode = m_currentNode.append_child("reg");
        WriteMeiReg(m_currentNode, dynamic_cast<Reg *>(object));
    }
    else if (object->Is() == RESTORE) {
        m_currentNode = m_currentNode.append_child("restore");
        WriteMeiRestore(m_currentNode, dynamic_cast<Restore *>(object));
    }
    else if (object->Is() == SIC) {
        m_currentNode = m_currentNode.append_child("sic");
        WriteMeiSic(m_currentNode, dynamic_cast<Sic *>(object));
    }
    else if (object->Is() == SUPPLIED) {
        m_currentNode = m_currentNode.append_child("supplied");
        WriteMeiSupplied(m_currentNode, dynamic_cast<Supplied *>(object));
    }
    else if (object->Is() == UNCLEAR) {
        m_currentNode = m_currentNode.append_child("unclear");
        WriteMeiUnclear(m_currentNode, dynamic_cast<Unclear *>(object));
    }

    else {
        // Missing output method for the class
        LogError("Output method missing for '%s'", object->GetClassName().c_str());
        assert(false); // let's make it stop because this should not happen
    }

    // LogDebug("Current: %s ", m_currentNode.value());
    m_nodeStack.push_back(m_currentNode);

    return true;
}

bool MeiOutput::WriteObjectEnd(Object *object)
{
    m_nodeStack.pop_back();
    m_currentNode = m_nodeStack.back();

    return true;
}

std::string MeiOutput::UuidToMeiStr(Object *element)
{
    std::string out = element->GetUuid();
    std::transform(out.begin(), out.end(), out.begin(), ::tolower);
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

    pugi::xml_node meiHead = m_mei.append_child("meiHead");

    if (m_doc->m_header.first_child()) {
        for (pugi::xml_node child = m_doc->m_header.first_child(); child; child = child.next_sibling()) {
            meiHead.append_copy(child);
        }
    }
    else {
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
        m_currentNode = m_currentNode.append_child("section");
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
        currentNode.append_attribute("uly") = StringFormat("%d", system->m_yAbs).c_str();
    }
}

void MeiOutput::WriteMeiScoreDef(pugi::xml_node currentNode, ScoreDef *scoreDef)
{
    assert(scoreDef);

    WriteXmlId(currentNode, scoreDef);
    WriteScoreDefInterface(currentNode, scoreDef);
}

void MeiOutput::WriteMeiStaffGrp(pugi::xml_node currentNode, StaffGrp *staffGrp)
{
    assert(staffGrp);

    WriteXmlId(currentNode, staffGrp);
    staffGrp->WriteCommon(currentNode);
    staffGrp->WriteCommonPart(currentNode);
    staffGrp->WriteLabelsAddl(currentNode);
    staffGrp->WriteStaffgroupingsym(currentNode);
    staffGrp->WriteStaffGrpVis(currentNode);
}

void MeiOutput::WriteMeiStaffDef(pugi::xml_node currentNode, StaffDef *staffDef)
{
    assert(staffDef);

    WriteXmlId(currentNode, staffDef);
    WriteScoreDefInterface(currentNode, staffDef);
    staffDef->WriteCommon(currentNode);
    staffDef->WriteCommonPart(currentNode);
    staffDef->WriteLabelsAddl(currentNode);
    staffDef->WriteNotationtype(currentNode);
    staffDef->WriteScalable(currentNode);
    staffDef->WriteStaffDefVis(currentNode);
}

void MeiOutput::WriteMeiMeasure(pugi::xml_node currentNode, Measure *measure)
{
    assert(measure);

    WriteXmlId(currentNode, measure);
    measure->WriteCommon(currentNode);
    measure->WriteMeasureLog(currentNode);
    measure->WritePointing(currentNode);
}

void MeiOutput::WriteMeiAnchoredText(pugi::xml_node currentNode, AnchoredText *anchoredText)
{
    assert(anchoredText);

    WriteXmlId(currentNode, anchoredText);
    WriteTextDirInterface(currentNode, anchoredText);
}

void MeiOutput::WriteMeiDir(pugi::xml_node currentNode, Dir *dir)
{
    assert(dir);

    WriteXmlId(currentNode, dir);
    WriteTextDirInterface(currentNode, dir);
    WriteTimeSpanningInterface(currentNode, dir);
};

void MeiOutput::WriteMeiDynam(pugi::xml_node currentNode, Dynam *dynam)
{
    assert(dynam);

    WriteXmlId(currentNode, dynam);
    WriteTextDirInterface(currentNode, dynam);
    WriteTimeSpanningInterface(currentNode, dynam);
};

void MeiOutput::WriteMeiHairpin(pugi::xml_node currentNode, Hairpin *hairpin)
{
    assert(hairpin);

    WriteXmlId(currentNode, hairpin);
    WriteTimeSpanningInterface(currentNode, hairpin);
    hairpin->WriteHairpinLog(currentNode);
    hairpin->WritePlacement(currentNode);
};

void MeiOutput::WriteMeiSlur(pugi::xml_node currentNode, Slur *slur)
{
    assert(slur);

    WriteXmlId(currentNode, slur);
    WriteTimeSpanningInterface(currentNode, slur);
    slur->WriteCurvature(currentNode);
};

void MeiOutput::WriteMeiStaff(pugi::xml_node currentNode, Staff *staff)
{
    assert(staff);

    WriteXmlId(currentNode, staff);
    staff->WriteCommon(currentNode);
    // y position
    if (staff->m_yAbs != VRV_UNSET) {
        currentNode.append_attribute("uly") = StringFormat("%d", staff->m_yAbs).c_str();
    }
}

void MeiOutput::WriteMeiTempo(pugi::xml_node currentNode, Tempo *tempo)
{
    assert(tempo);

    WriteXmlId(currentNode, tempo);
    WriteTextDirInterface(currentNode, tempo);
    WriteTimePointInterface(currentNode, tempo);
}

void MeiOutput::WriteMeiTie(pugi::xml_node currentNode, Tie *tie)
{
    assert(tie);

    WriteXmlId(currentNode, tie);
    WriteTimeSpanningInterface(currentNode, tie);
};

void MeiOutput::WriteMeiLayer(pugi::xml_node currentNode, Layer *layer)
{
    assert(layer);

    WriteXmlId(currentNode, layer);
    layer->WriteCommon(currentNode);
}

void MeiOutput::WriteLayerElement(pugi::xml_node currentNode, LayerElement *element)
{
    assert(element);

    WriteXmlId(currentNode, element);
    if (element->m_xAbs != VRV_UNSET) {
        currentNode.append_attribute("ulx") = StringFormat("%d", element->m_xAbs).c_str();
    }
}

void MeiOutput::WriteMeiAccid(pugi::xml_node currentNode, Accid *accid)
{
    assert(accid);

    WriteLayerElement(currentNode, accid);
    WritePositionInterface(currentNode, accid);
    accid->WriteAccidental(currentNode);
    accid->WriteAccidLog(currentNode);
}

void MeiOutput::WriteMeiBarLine(pugi::xml_node currentNode, BarLine *barLine)
{
    assert(barLine);

    WriteLayerElement(currentNode, barLine);
    barLine->WriteBarLineLog(currentNode);
}

void MeiOutput::WriteMeiBeam(pugi::xml_node currentNode, Beam *beam)
{
    assert(beam);

    WriteLayerElement(currentNode, beam);
}

void MeiOutput::WriteMeiBeatRpt(pugi::xml_node currentNode, BeatRpt *beatRpt)
{
    assert(beatRpt);

    WriteLayerElement(currentNode, beatRpt);
    beatRpt->WriteBeatRptVis(currentNode);
}

void MeiOutput::WriteMeiBTrem(pugi::xml_node currentNode, BTrem *bTrem)
{
    assert(bTrem);

    WriteLayerElement(currentNode, bTrem);
}

void MeiOutput::WriteMeiChord(pugi::xml_node currentNode, Chord *chord)
{
    assert(chord);

    WriteLayerElement(currentNode, chord);
    WriteDurationInterface(currentNode, chord);
    chord->WriteCommon(currentNode);
    chord->WriteStems(currentNode);
    chord->WriteStemsCmn(currentNode);
    chord->WriteTiepresent(currentNode);
}

void MeiOutput::WriteMeiClef(pugi::xml_node currentNode, Clef *clef)
{
    assert(clef);

    WriteLayerElement(currentNode, clef);
    clef->WriteClefshape(currentNode);
    clef->WriteLineloc(currentNode);
    clef->WriteOctavedisplacement(currentNode);
}

void MeiOutput::WriteMeiCustos(pugi::xml_node currentNode, Custos *custos)
{
    assert(custos);

    WriteLayerElement(currentNode, custos);
    WritePositionInterface(currentNode, custos);
}

void MeiOutput::WriteMeiDot(pugi::xml_node currentNode, Dot *dot)
{
    assert(dot);

    WriteLayerElement(currentNode, dot);
    WritePositionInterface(currentNode, dot);
}

void MeiOutput::WriteMeiFTrem(pugi::xml_node currentNode, FTrem *fTrem)
{
    assert(fTrem);

    WriteLayerElement(currentNode, fTrem);
    fTrem->WriteSlashcount(currentNode);
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
    mensur->WriteDurationRatio(currentNode);
    mensur->WriteMensuralShared(currentNode);
    mensur->WriteMensurLog(currentNode);
    mensur->WriteMensurVis(currentNode);
    mensur->WriteSlashcount(currentNode);
}

void MeiOutput::WriteMeiMeterSig(pugi::xml_node currentNode, MeterSig *meterSig)
{
    assert(meterSig);

    WriteLayerElement(currentNode, meterSig);
    meterSig->WriteMeterSigLog(currentNode);
}

void MeiOutput::WriteMeiMRest(pugi::xml_node currentNode, MRest *mRest)
{
    assert(mRest);

    WriteLayerElement(currentNode, mRest);
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
    note->WriteAccidentalPerformed(currentNode);
    note->WriteColoration(currentNode);
    note->WriteGraced(currentNode);
    note->WriteNoteLogMensural(currentNode);
    note->WriteStems(currentNode);
    note->WriteStemsCmn(currentNode);
    note->WriteTiepresent(currentNode);
}

void MeiOutput::WriteMeiRest(pugi::xml_node currentNode, Rest *rest)
{
    assert(rest);

    WriteLayerElement(currentNode, rest);
    WriteDurationInterface(currentNode, rest);
    WritePositionInterface(currentNode, rest);
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
    tuplet->WriteDurationRatio(currentNode);
    tuplet->WriteNumberplacement(currentNode);
    tuplet->WriteTupletVis(currentNode);
}

void MeiOutput::WriteMeiVerse(pugi::xml_node currentNode, Verse *verse)
{
    assert(verse);

    WriteLayerElement(currentNode, verse);
    verse->WriteCommon(currentNode);
}

void MeiOutput::WriteMeiSyl(pugi::xml_node currentNode, Syl *syl)
{
    assert(syl);

    WriteLayerElement(currentNode, syl);
    syl->WriteTypography(currentNode);
    syl->WriteSylLog(currentNode);
}

void MeiOutput::WriteMeiRend(pugi::xml_node currentNode, Rend *rend)
{
    assert(rend);

    WriteXmlId(currentNode, rend);
    rend->WriteCommon(currentNode);
    rend->WriteTypography(currentNode);
}

void MeiOutput::WriteMeiText(pugi::xml_node element, Text *text)
{
    if (!text->GetText().empty()) {
        pugi::xml_node nodechild = element.append_child(pugi::node_pcdata);
        // nodechild.text() =  UTF16to8(EscapeSMuFL(text->GetText()).c_str()).c_str();
        nodechild.text() = UTF16to8(text->GetText()).c_str();
    }
}

void MeiOutput::WriteDurationInterface(pugi::xml_node element, vrv::DurationInterface *interface)
{
    assert(interface);

    interface->WriteAugmentdots(element);
    interface->WriteBeamsecondary(element);
    interface->WriteDurationMusical(element);
    interface->WriteDurationPerformed(element);
    interface->WriteDurationRatio(element);
    interface->WriteFermatapresent(element);
    interface->WriteStaffident(element);
}

void MeiOutput::WritePitchInterface(pugi::xml_node element, PitchInterface *interface)
{
    assert(interface);

    interface->WriteAccidental(element);
    interface->WriteOctave(element);
    interface->WritePitch(element);
}

void MeiOutput::WritePositionInterface(pugi::xml_node element, PositionInterface *interface)
{
    assert(interface);

    interface->WriteStafflocPitched(element);
}

void MeiOutput::WriteScoreDefInterface(pugi::xml_node element, ScoreDefInterface *interface)
{
    assert(interface);

    interface->WriteCleffingLog(element);
    interface->WriteKeySigDefaultLog(element);
    interface->WriteKeySigDefaultVis(element);
    interface->WriteMensuralLog(element);
    interface->WriteMensuralShared(element);
    interface->WriteMeterSigDefaultLog(element);
    interface->WriteMeterSigDefaultVis(element);
    interface->WriteMultinummeasures(element);
}

void MeiOutput::WriteTextDirInterface(pugi::xml_node element, TextDirInterface *interface)
{
    assert(interface);

    interface->WriteCommon(element);
    interface->WritePlacement(element);
}

void MeiOutput::WriteTimePointInterface(pugi::xml_node element, TimePointInterface *interface)
{
    assert(interface);

    interface->WriteStaffident(element);
    interface->WriteStartid(element);
    interface->WriteTimestampMusical(element);
}

void MeiOutput::WriteTimeSpanningInterface(pugi::xml_node element, TimeSpanningInterface *interface)
{
    assert(interface);

    WriteTimePointInterface(element, interface);
    interface->WriteStartendid(element);
    interface->WriteTimestamp2Musical(element);
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
    element->WriteCommon(currentNode);
    element->WriteCommonPart(currentNode);
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
    m_page = NULL;
    m_system = NULL;
    //
    m_hasScoreDef = false;
}

MeiInput::~MeiInput()
{
}

bool MeiInput::ImportFile()
{
    try {
        m_doc->Reset(Raw);
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

bool MeiInput::ImportString(const std::string mei)
{
    try {
        m_doc->Reset(Raw);
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
        return true;
    }
    // filter for bTrem
    else if (filterParent->Is() == BTREM) {
        if (element == "chord") {
            return true;
        }
        else if (element == "note") {
            return true;
        }
        else {
            return false;
        }
    }
    // filter for fTrem
    else if (filterParent->Is() == FTREM) {
        if (element == "chord") {
            return true;
        }
        else if (element == "note") {
            return true;
        }
        else {
            return false;
        }
    }
    // filter for notes
    else if (filterParent->Is() == NOTE) {
        if (element == "accid") {
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
    // filter for Verse
    else if (filterParent->Is() == VERSE) {
        if (element == "syl") {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        LogDebug("Unknow filter for '%s'", filterParent->GetClassName().c_str());
        return true;
    }
}

bool MeiInput::ReadMei(pugi::xml_node root)
{
    pugi::xml_node current;
    bool success = true;

    if (!root.empty() && (current = root.child("meiHead"))) {
        ReadMeiHeader(current);
    }
    // music
    pugi::xml_node music;
    pugi::xml_node body;
    pugi::xml_node mdiv;
    pugi::xml_node pages;
    if (!root.empty()) {
        music = root.child("music");
    }
    if (!music.empty()) {
        body = music.child("body");
    }
    if (!body.empty()) {
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
            m_doc->Reset(type);
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
        m_page = new Page();
        m_system = new System();
        m_page->AddSystem(m_system);
        m_doc->AddPage(m_page);
        pugi::xml_node current;
        for (current = mdiv.first_child(); current; current = current.next_sibling()) {
            if (!success) break;
            success = ReadScoreBasedMei(current);
        }
    }
    return success;
}

bool MeiInput::ReadMeiHeader(pugi::xml_node meiHead)
{
    m_doc->m_header.reset();
    // copy all the nodes inside into the master document
    for (pugi::xml_node child = meiHead.first_child(); child; child = child.next_sibling()) {
        m_doc->m_header.append_copy(child);
    }
    return true;
}

bool MeiInput::ReadMeiPage(pugi::xml_node page)
{
    Page *vrvPage = new Page();
    SetMeiUuid(page, vrvPage);

    if (page.attribute("page.height")) {
        vrvPage->m_pageHeight = atoi(page.attribute("page.height").value());
    }
    if (page.attribute("page.width")) {
        vrvPage->m_pageWidth = atoi(page.attribute("page.width").value());
    }
    if (page.attribute("page.leftmar")) {
        vrvPage->m_pageLeftMar = atoi(page.attribute("page.leftmar").value());
    }
    if (page.attribute("page.rightmar")) {
        vrvPage->m_pageRightMar = atoi(page.attribute("page.rightmar").value());
    }
    if (page.attribute("page.topmar")) {
        vrvPage->m_pageTopMar = atoi(page.attribute("page.topmar").value());
    }
    if (page.attribute("surface")) {
        vrvPage->m_surface = page.attribute("surface").value();
    }

    m_doc->AddPage(vrvPage);
    return ReadMeiPageChildren(vrvPage, page);
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

    if (system.attribute("system.leftmar")) {
        vrvSystem->m_systemLeftMar = atoi(system.attribute("system.leftmar").value());
    }
    if (system.attribute("system.rightmar")) {
        vrvSystem->m_systemRightMar = atoi(system.attribute("system.rightmar").value());
    }
    if (system.attribute("uly")) {
        vrvSystem->m_yAbs = atoi(system.attribute("uly").value()) * DEFINITON_FACTOR;
    }

    // This could be moved to an AddSystem method for consistency with AddLayerElement
    if (parent->Is() == PAGE) {
        Page *page = dynamic_cast<Page *>(parent);
        assert(page);
        page->AddSystem(vrvSystem);
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
            success = ReadMeiEditorialElement(parent, current, EDITORIAL_SYSTEM);
        }
        // content
        else if (std::string(current.name()) == "scoreDef") {
            // we should not have scoredef with unmeasured music within a system... (?)
            assert(!unmeasured);
            ReadMeiScoreDef(parent, current);
        }
        // unmeasured music
        else if (parentNode.child("staff")) {
            if (!unmeasured) {
                if (parent->Is() == SYSTEM) {
                    System *system = dynamic_cast<System *>(parent);
                    assert(system);
                    unmeasured = new Measure(false);
                    system->AddMeasure(unmeasured);
                }
                else {
                    LogError("Unmeasured music within editorial markup is currently not supported");
                    return false;
                }
            }
            success = ReadMeiStaff(unmeasured, current);
        }
        else if (parentNode.child("measure")) {
            // we should not mix measured and unmeasured music within a system...
            assert(!unmeasured);
            if (parent->IsEditorialElement()) {
                if (!m_ignoreLayoutInformation) {
                    LogError("Cannot have <measure> within editorial markup unless layout information \
                             is ignored and continous layout is chosen (try with --no-layout)");
                    return false;
                }
                else {
                    m_hasMeasureWithinEditMarkup = true;
                }
            }
            success = ReadMeiMeasure(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <system>", current.name());
        }
    }
    return success;
}

bool MeiInput::ReadMeiScoreDef(Object *parent, pugi::xml_node scoreDef)
{
    assert(dynamic_cast<System *>(parent) || dynamic_cast<EditorialElement *>(parent));

    ScoreDef *vrvScoreDef;
    if (!m_hasScoreDef) {
        vrvScoreDef = &m_doc->m_scoreDef;
    }
    else {
        vrvScoreDef = new ScoreDef();
    }
    SetMeiUuid(scoreDef, vrvScoreDef);

    ReadScoreDefInterface(scoreDef, vrvScoreDef);

    AddScoreDef(parent, vrvScoreDef);
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

    vrvStaffGrp->ReadCommon(staffGrp);
    vrvStaffGrp->ReadCommonPart(staffGrp);
    vrvStaffGrp->ReadLabelsAddl(staffGrp);
    vrvStaffGrp->ReadStaffGrpVis(staffGrp);
    vrvStaffGrp->ReadStaffgroupingsym(staffGrp);

    AddStaffGrp(parent, vrvStaffGrp);
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
    SetMeiUuid(staffDef, vrvStaffDef);

    vrvStaffDef->ReadCommon(staffDef);
    vrvStaffDef->ReadCommonPart(staffDef);
    vrvStaffDef->ReadLabelsAddl(staffDef);
    vrvStaffDef->ReadNotationtype(staffDef);
    vrvStaffDef->ReadScalable(staffDef);
    vrvStaffDef->ReadStaffDefVis(staffDef);

    if (!vrvStaffDef->HasN()) {
        LogWarning("No @n on <staffDef> might yield unpredictable results");
    }

    ReadScoreDefInterface(staffDef, vrvStaffDef);

    // This could be moved to an AddMeasure method for consistency with AddLayerElement
    if (parent->Is() == STAFFGRP) {
        StaffGrp *staffGrp = dynamic_cast<StaffGrp *>(parent);
        assert(staffGrp);
        staffGrp->AddStaffDef(vrvStaffDef);
    }
    else if (parent->IsEditorialElement()) {
        EditorialElement *element = dynamic_cast<EditorialElement *>(parent);
        assert(element);
        element->AddStaffDef(vrvStaffDef);
    }
    return true;
}

bool MeiInput::ReadMeiMeasure(Object *parent, pugi::xml_node measure)
{
    Measure *vrvMeasure = new Measure();
    SetMeiUuid(measure, vrvMeasure);

    vrvMeasure->ReadCommon(measure);
    vrvMeasure->ReadMeasureLog(measure);
    vrvMeasure->ReadPointing(measure);

    // This could be moved to an AddMeasure method for consistency with AddLayerElement
    if (parent->Is() == SYSTEM) {
        System *system = dynamic_cast<System *>(parent);
        assert(system);
        system->AddMeasure(vrvMeasure);
    }
    else if (parent->IsEditorialElement()) {
        EditorialElement *element = dynamic_cast<EditorialElement *>(parent);
        assert(element);
        element->AddMeasure(vrvMeasure);
    }
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
        else if (std::string(current.name()) == "dir") {
            success = ReadMeiDir(parent, current);
        }
        else if (std::string(current.name()) == "dynam") {
            success = ReadMeiDynam(parent, current);
        }
        else if (std::string(current.name()) == "hairpin") {
            success = ReadMeiHairpin(parent, current);
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
        else if (std::string(current.name()) == "tupletSpan") {
            if (!ReadTupletSpanAsTuplet(dynamic_cast<Measure *>(parent), current)) {
                LogWarning("<tupletSpan> not readable as <tuplet> and ignored");
            }
        }
        else {
            LogWarning("Unsupported '<%s>' within <measure>", current.name());
        }
    }
    return success;
}

bool MeiInput::ReadMeiAnchoredText(Object *parent, pugi::xml_node anchoredText)
{
    AnchoredText *vrvAnchoredText = new AnchoredText();
    SetMeiUuid(anchoredText, vrvAnchoredText);

    ReadTextDirInterface(anchoredText, vrvAnchoredText);

    AddFloatingElement(parent, vrvAnchoredText);
    return ReadMeiTextChildren(vrvAnchoredText, anchoredText);
}

bool MeiInput::ReadMeiDir(Object *parent, pugi::xml_node dir)
{
    Dir *vrvDir = new Dir();
    SetMeiUuid(dir, vrvDir);

    ReadTextDirInterface(dir, vrvDir);
    ReadTimeSpanningInterface(dir, vrvDir);

    AddFloatingElement(parent, vrvDir);
    return ReadMeiTextChildren(vrvDir, dir);
}

bool MeiInput::ReadMeiDynam(Object *parent, pugi::xml_node dynam)
{
    Dynam *vrvDynam = new Dynam();
    SetMeiUuid(dynam, vrvDynam);

    ReadTextDirInterface(dynam, vrvDynam);
    ReadTimeSpanningInterface(dynam, vrvDynam);

    AddFloatingElement(parent, vrvDynam);
    return ReadMeiTextChildren(vrvDynam, dynam);
}

bool MeiInput::ReadMeiHairpin(Object *parent, pugi::xml_node hairpin)
{
    Hairpin *vrvHairpin = new Hairpin();
    SetMeiUuid(hairpin, vrvHairpin);

    ReadTimeSpanningInterface(hairpin, vrvHairpin);
    vrvHairpin->ReadHairpinLog(hairpin);
    vrvHairpin->ReadPlacement(hairpin);

    AddFloatingElement(parent, vrvHairpin);
    return true;
}

bool MeiInput::ReadMeiSlur(Object *parent, pugi::xml_node slur)
{
    Slur *vrvSlur = new Slur();
    SetMeiUuid(slur, vrvSlur);

    ReadTimeSpanningInterface(slur, vrvSlur);
    vrvSlur->ReadCurvature(slur);

    AddFloatingElement(parent, vrvSlur);
    return true;
}

bool MeiInput::ReadMeiTempo(Object *parent, pugi::xml_node tempo)
{
    Tempo *vrvTempo = new Tempo();
    SetMeiUuid(tempo, vrvTempo);

    ReadTextDirInterface(tempo, vrvTempo);
    ReadTimePointInterface(tempo, vrvTempo);

    AddFloatingElement(parent, vrvTempo);
    return ReadMeiTextChildren(vrvTempo, tempo);
}

bool MeiInput::ReadMeiTie(Object *parent, pugi::xml_node tie)
{
    Tie *vrvTie = new Tie();
    SetMeiUuid(tie, vrvTie);

    ReadTimeSpanningInterface(tie, vrvTie);
    vrvTie->ReadCurvature(tie);

    AddFloatingElement(parent, vrvTie);
    return true;
}

bool MeiInput::ReadMeiStaff(Object *parent, pugi::xml_node staff)
{
    Staff *vrvStaff = new Staff();
    SetMeiUuid(staff, vrvStaff);

    vrvStaff->ReadCommon(staff);

    if (staff.attribute("uly")) {
        vrvStaff->m_yAbs = atoi(staff.attribute("uly").value()) * DEFINITON_FACTOR;
    }

    if (!vrvStaff->HasN()) {
        LogWarning("No @n on <staff> might yield unpredictable results");
    }

    // This could be moved to an AddLayer method for consistency with AddLayerElement
    if (parent->Is() == MEASURE) {
        Measure *measure = dynamic_cast<Measure *>(parent);
        assert(measure);
        measure->AddStaff(vrvStaff);
    }
    else if (parent->IsEditorialElement()) {
        EditorialElement *element = dynamic_cast<EditorialElement *>(parent);
        assert(element);
        element->AddStaff(vrvStaff);
    }
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

    vrvLayer->ReadCommon(layer);

    if (!vrvLayer->HasN()) {
        LogWarning("No @n on <layer> might yield unpredictable results");
    }

    // This could be moved to an AddLayer method for consistency with AddLayerElement
    if (parent->Is() == STAFF) {
        Staff *staff = dynamic_cast<Staff *>(parent);
        assert(staff);
        staff->AddLayer(vrvLayer);
    }
    else if (parent->IsEditorialElement()) {
        EditorialElement *element = dynamic_cast<EditorialElement *>(parent);
        assert(element);
        element->AddLayer(vrvLayer);
    }
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
        if (!IsAllowed(elementName, filter)) {
            LogDebug("Element <%s> within %s ignored", xmlElement.name(), filter->GetClassName().c_str());
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
            LogDebug("Element %s ignored", xmlElement.name());
        }
    }
    return success;
}

bool MeiInput::ReadLayerElement(pugi::xml_node element, LayerElement *object)
{
    if (element.attribute("ulx")) {
        object->m_xAbs = atoi(element.attribute("ulx").value()) * DEFINITON_FACTOR;
    }

    SetMeiUuid(element, object);

    return true;
}

bool MeiInput::ReadMeiAccid(Object *parent, pugi::xml_node accid)
{
    Accid *vrvAccid = new Accid();
    ReadLayerElement(accid, vrvAccid);

    ReadPositionInterface(accid, vrvAccid);
    vrvAccid->ReadAccidental(accid);
    vrvAccid->ReadAccidLog(accid);

    AddLayerElement(parent, vrvAccid);
    return true;
}

bool MeiInput::ReadMeiBarLine(Object *parent, pugi::xml_node barLine)
{
    BarLine *vrvBarLine = new BarLine();
    ReadLayerElement(barLine, vrvBarLine);

    vrvBarLine->ReadBarLineLog(barLine);

    AddLayerElement(parent, vrvBarLine);
    return true;
}

bool MeiInput::ReadMeiBeam(Object *parent, pugi::xml_node beam)
{
    Beam *vrvBeam = new Beam();
    ReadLayerElement(beam, vrvBeam);

    AddLayerElement(parent, vrvBeam);

    ReadMeiLayerChildren(vrvBeam, beam);

    if (vrvBeam->GetNoteCount() == 1) {
        LogWarning("<beam> with only one note");
    }
    return true;
}

bool MeiInput::ReadMeiBeatRpt(Object *parent, pugi::xml_node beatRpt)
{
    BeatRpt *vrvBeatRpt = new BeatRpt();
    ReadLayerElement(beatRpt, vrvBeatRpt);

    vrvBeatRpt->ReadBeatRptVis(beatRpt);

    AddLayerElement(parent, vrvBeatRpt);
    return true;
}

bool MeiInput::ReadMeiBTrem(Object *parent, pugi::xml_node bTrem)
{
    BTrem *vrvBTrem = new BTrem();
    ReadLayerElement(bTrem, vrvBTrem);

    AddLayerElement(parent, vrvBTrem);
    return ReadMeiLayerChildren(vrvBTrem, bTrem, vrvBTrem);
}

bool MeiInput::ReadMeiChord(Object *parent, pugi::xml_node chord)
{
    Chord *vrvChord = new Chord();
    SetMeiUuid(chord, vrvChord);

    ReadDurationInterface(chord, vrvChord);
    vrvChord->ReadCommon(chord);
    vrvChord->ReadStems(chord);
    vrvChord->ReadStemsCmn(chord);
    vrvChord->ReadTiepresent(chord);

    AddLayerElement(parent, vrvChord);
    return ReadMeiLayerChildren(vrvChord, chord);
}

bool MeiInput::ReadMeiClef(Object *parent, pugi::xml_node clef)
{
    Clef *vrvClef = new Clef();
    ReadLayerElement(clef, vrvClef);

    vrvClef->ReadClefshape(clef);
    vrvClef->ReadLineloc(clef);
    vrvClef->ReadOctavedisplacement(clef);

    AddLayerElement(parent, vrvClef);
    return true;
}

bool MeiInput::ReadMeiCustos(Object *parent, pugi::xml_node custos)
{
    Custos *vrvCustos = new Custos();
    ReadLayerElement(custos, vrvCustos);

    ReadPositionInterface(custos, vrvCustos);

    AddLayerElement(parent, vrvCustos);
    return true;
}

bool MeiInput::ReadMeiDot(Object *parent, pugi::xml_node dot)
{
    Dot *vrvDot = new Dot();
    ReadLayerElement(dot, vrvDot);

    ReadPositionInterface(dot, vrvDot);

    AddLayerElement(parent, vrvDot);
    return true;
}

bool MeiInput::ReadMeiFTrem(Object *parent, pugi::xml_node fTrem)
{
    FTrem *vrvFTrem = new FTrem();
    ReadLayerElement(fTrem, vrvFTrem);

    vrvFTrem->ReadSlashcount(fTrem);

    AddLayerElement(parent, vrvFTrem);
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

    AddLayerElement(parent, vrvKeySig);
    return true;
}

bool MeiInput::ReadMeiMensur(Object *parent, pugi::xml_node mensur)
{
    Mensur *vrvMensur = new Mensur();
    ReadLayerElement(mensur, vrvMensur);

    vrvMensur->ReadDurationRatio(mensur);
    vrvMensur->ReadMensuralShared(mensur);
    vrvMensur->ReadMensurLog(mensur);
    vrvMensur->ReadMensurVis(mensur);
    vrvMensur->ReadSlashcount(mensur);

    AddLayerElement(parent, vrvMensur);
    return true;
}

bool MeiInput::ReadMeiMeterSig(Object *parent, pugi::xml_node meterSig)
{
    MeterSig *vrvMeterSig = new MeterSig();
    ReadLayerElement(meterSig, vrvMeterSig);

    vrvMeterSig->ReadMeterSigLog(meterSig);

    AddLayerElement(parent, vrvMeterSig);
    return true;
}

bool MeiInput::ReadMeiMRest(Object *parent, pugi::xml_node mRest)
{
    MRest *vrvMRest = new MRest();
    ReadLayerElement(mRest, vrvMRest);

    AddLayerElement(parent, vrvMRest);
    return true;
}

bool MeiInput::ReadMeiMRpt(Object *parent, pugi::xml_node mRpt)
{
    MRpt *vrvMRpt = new MRpt();
    ReadLayerElement(mRpt, vrvMRpt);

    AddLayerElement(parent, vrvMRpt);
    return true;
}

bool MeiInput::ReadMeiMRpt2(Object *parent, pugi::xml_node mRpt2)
{
    MRpt2 *vrvMRpt2 = new MRpt2();
    ReadLayerElement(mRpt2, vrvMRpt2);

    AddLayerElement(parent, vrvMRpt2);
    return true;
}

bool MeiInput::ReadMeiMultiRest(Object *parent, pugi::xml_node multiRest)
{
    MultiRest *vrvMultiRest = new MultiRest();
    ReadLayerElement(multiRest, vrvMultiRest);

    vrvMultiRest->ReadNumbered(multiRest);

    AddLayerElement(parent, vrvMultiRest);
    return true;
}

bool MeiInput::ReadMeiMultiRpt(Object *parent, pugi::xml_node multiRpt)
{
    MultiRpt *vrvMultiRpt = new MultiRpt();
    ReadLayerElement(multiRpt, vrvMultiRpt);

    vrvMultiRpt->ReadNumbered(multiRpt);

    AddLayerElement(parent, vrvMultiRpt);
    return true;
}

bool MeiInput::ReadMeiNote(Object *parent, pugi::xml_node note)
{
    Note *vrvNote = new Note();
    ReadLayerElement(note, vrvNote);

    ReadDurationInterface(note, vrvNote);
    ReadPitchInterface(note, vrvNote);
    vrvNote->ReadAccidentalPerformed(note);
    vrvNote->ReadColoration(note);
    vrvNote->ReadGraced(note);
    vrvNote->ReadNoteLogMensural(note);
    vrvNote->ReadStems(note);
    vrvNote->ReadStemsCmn(note);
    vrvNote->ReadTiepresent(note);

    AddLayerElement(parent, vrvNote);
    return ReadMeiLayerChildren(vrvNote, note, vrvNote);
}

bool MeiInput::ReadMeiRest(Object *parent, pugi::xml_node rest)
{
    Rest *vrvRest = new Rest();
    ReadLayerElement(rest, vrvRest);

    ReadDurationInterface(rest, vrvRest);
    ReadPositionInterface(rest, vrvRest);

    AddLayerElement(parent, vrvRest);
    return true;
}

bool MeiInput::ReadMeiProport(Object *parent, pugi::xml_node proport)
{
    Proport *vrvProport = new Proport();
    ReadLayerElement(proport, vrvProport);

    vrvProport->ReadDurationRatio(proport);

    AddLayerElement(parent, vrvProport);
    return true;
}

bool MeiInput::ReadMeiSpace(Object *parent, pugi::xml_node space)
{
    Space *vrvSpace = new Space();
    ReadLayerElement(space, vrvSpace);

    ReadDurationInterface(space, vrvSpace);

    AddLayerElement(parent, vrvSpace);
    return true;
}

bool MeiInput::ReadMeiSyl(Object *parent, pugi::xml_node syl)
{
    Syl *vrvSyl = new Syl();
    ReadLayerElement(syl, vrvSyl);

    vrvSyl->ReadTypography(syl);
    vrvSyl->ReadSylLog(syl);

    AddLayerElement(parent, vrvSyl);
    return ReadMeiTextChildren(vrvSyl, syl);
}

bool MeiInput::ReadMeiTuplet(Object *parent, pugi::xml_node tuplet)
{
    Tuplet *vrvTuplet = new Tuplet();
    ReadLayerElement(tuplet, vrvTuplet);

    vrvTuplet->ReadDurationRatio(tuplet);
    vrvTuplet->ReadNumberplacement(tuplet);
    vrvTuplet->ReadTupletVis(tuplet);

    AddLayerElement(parent, vrvTuplet);

    bool success = ReadMeiLayerChildren(vrvTuplet, tuplet);

    // Not very precise: looks at the children count, will not be correct with one beam or an editorial single child,
    // for example
    if (vrvTuplet->GetNoteCount() == 1) {
        LogWarning("<tuplet> with only one note");
    }
    return success;
}

bool MeiInput::ReadMeiVerse(Object *parent, pugi::xml_node verse)
{
    Verse *vrvVerse = new Verse();
    ReadLayerElement(verse, vrvVerse);

    vrvVerse->ReadCommon(verse);

    AddLayerElement(parent, vrvVerse);
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
        if (!IsAllowed(elementName, filter)) {
            LogDebug("Element <%s> within %s ignored", xmlElement.name(), filter->GetClassName().c_str());
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
        // unknown
        else {
            LogDebug("Element %s ignored", xmlElement.name());
        }
        i++;
    }
    return success;
}

bool MeiInput::ReadMeiRend(Object *parent, pugi::xml_node rend)
{
    Rend *vrvRend = new Rend();

    vrvRend->ReadCommon(rend);
    vrvRend->ReadTypography(rend);

    AddTextElement(parent, vrvRend);
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

    AddTextElement(parent, vrvText);
    return true;
}

bool MeiInput::ReadDurationInterface(pugi::xml_node element, DurationInterface *interface)
{
    interface->ReadAugmentdots(element);
    interface->ReadBeamsecondary(element);
    interface->ReadDurationMusical(element);
    interface->ReadDurationPerformed(element);
    interface->ReadDurationRatio(element);
    interface->ReadFermatapresent(element);
    interface->ReadStaffident(element);
    return true;
}

bool MeiInput::ReadPitchInterface(pugi::xml_node element, PitchInterface *interface)
{
    interface->ReadAccidental(element);
    interface->ReadOctave(element);
    interface->ReadPitch(element);
    return true;
}

bool MeiInput::ReadPositionInterface(pugi::xml_node element, PositionInterface *interface)
{
    interface->ReadStafflocPitched(element);
    return true;
}

bool MeiInput::ReadScoreDefInterface(pugi::xml_node element, ScoreDefInterface *interface)
{
    interface->ReadCleffingLog(element);
    interface->ReadKeySigDefaultLog(element);
    interface->ReadKeySigDefaultVis(element);
    interface->ReadMensuralLog(element);
    interface->ReadMensuralShared(element);
    interface->ReadMeterSigDefaultLog(element);
    interface->ReadMeterSigDefaultVis(element);
    interface->ReadMultinummeasures(element);
    return true;
}

bool MeiInput::ReadTextDirInterface(pugi::xml_node element, TextDirInterface *interface)
{
    interface->ReadCommon(element);
    interface->ReadPlacement(element);
    return true;
}

bool MeiInput::ReadTimePointInterface(pugi::xml_node element, TimePointInterface *interface)
{
    interface->ReadStaffident(element);
    interface->ReadStartid(element);
    interface->ReadTimestampMusical(element);
    return true;
}

bool MeiInput::ReadTimeSpanningInterface(pugi::xml_node element, TimeSpanningInterface *interface)
{
    ReadTimePointInterface(element, interface);
    interface->ReadStartendid(element);
    interface->ReadTimestamp2Musical(element);
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

    object->ReadCommon(element);
    object->ReadCommonPart(element);

    return true;
}

bool MeiInput::ReadMeiAbbr(Object *parent, pugi::xml_node abbr, EditorialLevel level, Object *filter)
{
    Abbr *vrvAbbr = new Abbr();
    ReadEditorialElement(abbr, vrvAbbr);

    vrvAbbr->ReadSource(abbr);

    parent->AddEditorialElement(vrvAbbr);
    return ReadMeiEditorialChildren(vrvAbbr, abbr, level, filter);
}

bool MeiInput::ReadMeiAdd(Object *parent, pugi::xml_node add, EditorialLevel level, Object *filter)
{
    Add *vrvAdd = new Add();
    ReadEditorialElement(add, vrvAdd);

    vrvAdd->ReadSource(add);

    parent->AddEditorialElement(vrvAdd);
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

    parent->AddEditorialElement(vrvAnnot);
    return true;
}

bool MeiInput::ReadMeiApp(Object *parent, pugi::xml_node app, EditorialLevel level, Object *filter)
{
    if (!m_hasScoreDef) {
        LogError("<app> before any <scoreDef> is not supported");
        return false;
    }
    App *vrvApp = new App(level);
    ReadEditorialElement(app, vrvApp);

    parent->AddEditorialElement(vrvApp);

    return ReadMeiAppChildren(vrvApp, app, level, filter);
}

bool MeiInput::ReadMeiAppChildren(Object *parent, pugi::xml_node parentNode, EditorialLevel level, Object *filter)
{
    assert(dynamic_cast<App *>(parent));

    // Check if one child node matches the m_appXPathQuery
    pugi::xml_node selectedLemOrRdg;
    if (m_appXPathQuery.length() > 0) {
        pugi::xpath_node selection = parentNode.select_single_node(m_appXPathQuery.c_str());
        if (selection) selectedLemOrRdg = selection.node();
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
            EditorialElement *last = dynamic_cast<EditorialElement *>(parent->m_children.back());
            if (last) {
                last->m_visibility = Visible;
                hasXPathSelected = true;
            }
        }
    }

    // If no child was made visible through the xpath selection, make the first one visible
    if (!hasXPathSelected) {
        EditorialElement *first = dynamic_cast<EditorialElement *>(parent->m_children.front());
        if (first) {
            first->m_visibility = Visible;
        }
        else {
            LogWarning("Could not make one <rdg> or <lem> visible");
        }
    }
    return success;
}

bool MeiInput::ReadMeiCorr(Object *parent, pugi::xml_node corr, EditorialLevel level, Object *filter)
{
    Corr *vrvCorr = new Corr();
    ReadEditorialElement(corr, vrvCorr);

    vrvCorr->ReadSource(corr);

    parent->AddEditorialElement(vrvCorr);
    return ReadMeiEditorialChildren(vrvCorr, corr, level, filter);
}

bool MeiInput::ReadMeiDamage(Object *parent, pugi::xml_node damage, EditorialLevel level, Object *filter)
{
    Damage *vrvDamage = new Damage();
    ReadEditorialElement(damage, vrvDamage);

    vrvDamage->ReadSource(damage);

    parent->AddEditorialElement(vrvDamage);
    return ReadMeiEditorialChildren(vrvDamage, damage, level, filter);
}

bool MeiInput::ReadMeiDel(Object *parent, pugi::xml_node del, EditorialLevel level, Object *filter)
{
    Del *vrvDel = new Del();
    ReadEditorialElement(del, vrvDel);

    vrvDel->ReadSource(del);

    parent->AddEditorialElement(vrvDel);
    return ReadMeiEditorialChildren(vrvDel, del, level, filter);
}

bool MeiInput::ReadMeiExpan(Object *parent, pugi::xml_node expan, EditorialLevel level, Object *filter)
{
    Expan *vrvExpan = new Expan();
    ReadEditorialElement(expan, vrvExpan);

    vrvExpan->ReadSource(expan);

    parent->AddEditorialElement(vrvExpan);
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

    (dynamic_cast<App *>(parent))->AddLemOrRdg(vrvLem);
    return ReadMeiEditorialChildren(vrvLem, lem, level, filter);
}

bool MeiInput::ReadMeiOrig(Object *parent, pugi::xml_node orig, EditorialLevel level, Object *filter)
{
    Orig *vrvOrig = new Orig();
    ReadEditorialElement(orig, vrvOrig);

    vrvOrig->ReadSource(orig);

    parent->AddEditorialElement(vrvOrig);
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

    (dynamic_cast<App *>(parent))->AddLemOrRdg(vrvRdg);
    return ReadMeiEditorialChildren(vrvRdg, rdg, level, filter);
}

bool MeiInput::ReadMeiReg(Object *parent, pugi::xml_node reg, EditorialLevel level, Object *filter)
{
    Reg *vrvReg = new Reg();
    ReadEditorialElement(reg, vrvReg);

    vrvReg->ReadSource(reg);

    parent->AddEditorialElement(vrvReg);
    return ReadMeiEditorialChildren(vrvReg, reg, level, filter);
}

bool MeiInput::ReadMeiRestore(Object *parent, pugi::xml_node restore, EditorialLevel level, Object *filter)
{
    Restore *vrvRestore = new Restore();
    ReadEditorialElement(restore, vrvRestore);

    vrvRestore->ReadSource(restore);

    parent->AddEditorialElement(vrvRestore);
    return ReadMeiEditorialChildren(vrvRestore, restore, level, filter);
}

bool MeiInput::ReadMeiSic(Object *parent, pugi::xml_node sic, EditorialLevel level, Object *filter)
{
    Sic *vrvSic = new Sic();
    ReadEditorialElement(sic, vrvSic);

    vrvSic->ReadSource(sic);

    parent->AddEditorialElement(vrvSic);
    return ReadMeiEditorialChildren(vrvSic, sic, level, filter);
}

bool MeiInput::ReadMeiSupplied(Object *parent, pugi::xml_node supplied, EditorialLevel level, Object *filter)
{
    Supplied *vrvSupplied = new Supplied();
    ReadEditorialElement(supplied, vrvSupplied);

    vrvSupplied->ReadSource(supplied);

    parent->AddEditorialElement(vrvSupplied);
    return ReadMeiEditorialChildren(vrvSupplied, supplied, level, filter);
}

bool MeiInput::ReadMeiUnclear(Object *parent, pugi::xml_node unclear, EditorialLevel level, Object *filter)
{
    Unclear *vrvUnclear = new Unclear();
    ReadEditorialElement(unclear, vrvUnclear);

    vrvUnclear->ReadSource(unclear);

    parent->AddEditorialElement(vrvUnclear);
    return ReadMeiEditorialChildren(vrvUnclear, unclear, level, filter);
}

bool MeiInput::ReadMeiEditorialChildren(Object *parent, pugi::xml_node parentNode, EditorialLevel level, Object *filter)
{
    assert(dynamic_cast<EditorialElement *>(parent));

    if (level == EDITORIAL_SYSTEM) {
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
    else {
        return false;
    }
}

void MeiInput::AddScoreDef(Object *parent, ScoreDef *scoreDef)
{
    if (!m_hasScoreDef) {
        m_hasScoreDef = true;
    }
    else if (parent->IsEditorialElement()) {
        EditorialElement *element = dynamic_cast<EditorialElement *>(parent);
        assert(element);
        element->AddScoreDef(scoreDef);
    }
    else if (parent->Is() == SYSTEM) {
        System *system = dynamic_cast<System *>(parent);
        assert(system);
        system->AddScoreDef(scoreDef);
    }
    else {
        LogWarning("'%s' not supported within '%s'", scoreDef->GetClassName().c_str(), parent->GetClassName().c_str());
        delete scoreDef;
    }
}

void MeiInput::AddStaffGrp(Object *parent, StaffGrp *staffGrp)
{
    if (parent->IsEditorialElement()) {
        EditorialElement *element = dynamic_cast<EditorialElement *>(parent);
        assert(element);
        element->AddStaffGrp(staffGrp);
    }
    else if (parent->Is() == SCOREDEF) {
        (dynamic_cast<ScoreDef *>(parent))->AddStaffGrp(staffGrp);
    }
    else if (parent->Is() == STAFFGRP) {
        (dynamic_cast<StaffGrp *>(parent))->AddStaffGrp(staffGrp);
    }
    else {
        LogWarning("'%s' not supported within '%s'", staffGrp->GetClassName().c_str(), parent->GetClassName().c_str());
        delete staffGrp;
    }
}

void MeiInput::AddLayerElement(Object *parent, LayerElement *element)
{
    if (parent->IsEditorialElement()) {
        EditorialElement *editorialElement = dynamic_cast<EditorialElement *>(parent);
        assert(editorialElement);
        editorialElement->AddLayerElement(element);
    }
    else if (parent->Is() == LAYER) {
        Layer *layer = dynamic_cast<Layer *>(parent);
        assert(layer);
        layer->AddLayerElement(element);
    }
    // LayerElements
    else if (parent->Is() == BEAM) {
        Beam *beam = dynamic_cast<Beam *>(parent);
        assert(beam);
        beam->AddLayerElement(element);
    }
    else if (parent->Is() == BTREM) {
        BTrem *bTrem = dynamic_cast<BTrem *>(parent);
        assert(bTrem);
        bTrem->AddLayerElement(element);
    }
    else if (parent->Is() == CHORD) {
        Chord *chord = dynamic_cast<Chord *>(parent);
        assert(chord);
        chord->AddLayerElement(element);
    }
    else if (parent->Is() == NOTE) {
        Note *note = dynamic_cast<Note *>(parent);
        assert(note);
        note->AddLayerElement(element);
    }
    else if (parent->Is() == FTREM) {
        FTrem *fTrem = dynamic_cast<FTrem *>(parent);
        assert(fTrem);
        fTrem->AddLayerElement(element);
    }
    else if (parent->Is() == TUPLET) {
        Tuplet *tuplet = dynamic_cast<Tuplet *>(parent);
        assert(tuplet);
        tuplet->AddLayerElement(element);
    }
    else if (parent->Is() == VERSE) {
        Verse *verse = dynamic_cast<Verse *>(parent);
        assert(verse);
        verse->AddLayerElement(element);
    }
    else {
        LogWarning("'%s' not supported within '%s'", element->GetClassName().c_str(), parent->GetClassName().c_str());
        delete element;
    }
}

void MeiInput::AddFloatingElement(Object *parent, FloatingElement *element)
{
    if (parent->IsEditorialElement()) {
        EditorialElement *editorialElement = dynamic_cast<EditorialElement *>(parent);
        assert(editorialElement);
        editorialElement->AddFloatingElement(element);
    }
    else if (parent->Is() == MEASURE) {
        Measure *measure = dynamic_cast<Measure *>(parent);
        assert(measure);
        measure->AddFloatingElement(element);
    }
    else {
        LogWarning("'%s' not supported within '%s'", element->GetClassName().c_str(), parent->GetClassName().c_str());
        delete element;
    }
}

void MeiInput::AddTextElement(Object *parent, TextElement *element)
{
    if (parent->IsEditorialElement()) {
        EditorialElement *editorialElement = dynamic_cast<EditorialElement *>(parent);
        assert(editorialElement);
        editorialElement->AddTextElement(element);
    }
    else if (parent->Is() == ANCHORED_TEXT) {
        AnchoredText *anchoredText = dynamic_cast<AnchoredText *>(parent);
        assert(anchoredText);
        anchoredText->AddTextElement(element);
    }
    else if (parent->Is() == DIR) {
        Dir *dir = dynamic_cast<Dir *>(parent);
        assert(dir);
        dir->AddTextElement(element);
    }
    else if (parent->Is() == DYNAM) {
        Dynam *dynam = dynamic_cast<Dynam *>(parent);
        assert(dynam);
        dynam->AddTextElement(element);
    }
    else if (parent->Is() == REND) {
        Rend *rend = dynamic_cast<Rend *>(parent);
        assert(rend);
        rend->AddTextElement(element);
    }
    else if (parent->Is() == SYL) {
        Syl *syl = dynamic_cast<Syl *>(parent);
        assert(syl);
        syl->AddTextElement(element);
    }
    else if (parent->Is() == TEMPO) {
        Tempo *tempo = dynamic_cast<Tempo *>(parent);
        assert(tempo);
        tempo->AddTextElement(element);
    }
    else {
        LogWarning("'%s' not supported within '%s'", element->GetClassName().c_str(), parent->GetClassName().c_str());
        delete element;
    }
}

bool MeiInput::ReadScoreBasedMei(pugi::xml_node element)
{
    bool success = true;
    // editorial
    if ((std::string(element.name()) == "app")) {
        success = ReadMeiApp(m_system, element, EDITORIAL_SYSTEM);
    }
    else if ((std::string(element.name()) == "annot")) {
        success = ReadMeiAnnot(m_system, element);
    }
    else if ((std::string(element.name()) == "supplied")) {
        success = ReadMeiSupplied(m_system, element, EDITORIAL_SYSTEM);
    }
    // content
    else if (std::string(element.name()) == "measure") {
        // This is the call that will put us back on the page-based reading loop
        success = ReadMeiMeasure(m_system, element);
    }
    else if (std::string(element.name()) == "pb") {
        if ((m_system->GetChildCount() > 0) && !m_ignoreLayoutInformation) {
            // LogDebug("pb");
            this->m_hasLayoutInformation = true;
            m_page = new Page();
            m_system = new System();
            m_page->AddSystem(m_system);
            m_doc->AddPage(m_page);
        }
    }
    else if (std::string(element.name()) == "sb") {
        if ((m_page->GetSystemCount() > 0) && !m_ignoreLayoutInformation) {
            // LogDebug("pb");
            this->m_hasLayoutInformation = true;
            m_system = new System();
            m_page->AddSystem(m_system);
        }
    }
    else if (std::string(element.name()) == "score") {
        pugi::xml_node current;
        for (current = element.first_child(); current; current = current.next_sibling()) {
            if (!success) break;
            success = ReadScoreBasedMei(current);
        }
    }
    else if ((std::string(element.name()) == "scoreDef")) {
        success = ReadMeiScoreDef(m_system, element);
    }
    else if (std::string(element.name()) == "section") {
        pugi::xml_node current;
        for (current = element.first_child(); current; current = current.next_sibling()) {
            if (!success) break;
            // This will happen with unmeasured music - ReadMeiSystemChildren will read take over the loop
            // This means that <pb> and <sb> (for example) will not be read
            else if ((std::string(current.name()) == "staff")) {
                success = ReadMeiSystemChildren(m_system, element);
                break;
            }
            success = ReadScoreBasedMei(current);
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

    if (!startChild || !endChild || (startChild->m_parent != endChild->m_parent)) {
        LogWarning("Start and end elements for <tupletSpan> '%s' not in the same layer", tuplet->GetUuid().c_str());
        delete tuplet;
        return false;
    }

    Layer *parentLayer = dynamic_cast<Layer *>(startChild->m_parent);
    assert(parentLayer);

    int startIdx = startChild->GetIdx();
    int endIdx = endChild->GetIdx();
    // LogDebug("%d %d %s!", startIdx, endIdx, start->GetUuid().c_str());
    int i;
    for (i = endIdx; i >= startIdx; i--) {
        LayerElement *element = dynamic_cast<LayerElement *>(parentLayer->DetachChild(i));
        if (element) tuplet->AddLayerElement(element);
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
    int i;
    for (i = 0; i < EDIT_NAMES; i++) {
        if (std::string(MeiInput::s_editorialElementNames[i]) == elementName) return true;
    }
    return false;
}

} // namespace vrv
