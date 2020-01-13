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

#include "abbr.h"
#include "accid.h"
#include "add.h"
#include "anchoredtext.h"
#include "annot.h"
#include "app.h"
#include "arpeg.h"
#include "artic.h"
#include "beam.h"
#include "beatrpt.h"
#include "boundary.h"
#include "bracketspan.h"
#include "breath.h"
#include "btrem.h"
#include "choice.h"
#include "chord.h"
#include "clef.h"
#include "corr.h"
#include "custos.h"
#include "damage.h"
#include "del.h"
#include "dir.h"
#include "dot.h"
#include "dynam.h"
#include "editorial.h"
#include "ending.h"
#include "expan.h"
#include "expansion.h"
#include "f.h"
#include "fb.h"
#include "fermata.h"
#include "fig.h"
#include "ftrem.h"
#include "functorparams.h"
#include "gracegrp.h"
#include "hairpin.h"
#include "halfmrpt.h"
#include "harm.h"
#include "instrdef.h"
#include "keyaccid.h"
#include "keysig.h"
#include "label.h"
#include "labelabbr.h"
#include "layer.h"
#include "lb.h"
#include "lem.h"
#include "ligature.h"
#include "mdiv.h"
#include "measure.h"
#include "mensur.h"
#include "metersig.h"
#include "mnum.h"
#include "mordent.h"
#include "mrest.h"
#include "mrpt.h"
#include "mrpt2.h"
#include "multirest.h"
#include "multirpt.h"
#include "nc.h"
#include "neume.h"
#include "note.h"
#include "num.h"
#include "octave.h"
#include "orig.h"
#include "page.h"
#include "pages.h"
#include "pb.h"
#include "pedal.h"
#include "pgfoot.h"
#include "pgfoot2.h"
#include "pghead.h"
#include "pghead2.h"
#include "proport.h"
#include "rdg.h"
#include "ref.h"
#include "reg.h"
#include "rend.h"
#include "rest.h"
#include "restore.h"
#include "sb.h"
#include "score.h"
#include "section.h"
#include "sic.h"
#include "slur.h"
#include "space.h"
#include "staff.h"
#include "staffdef.h"
#include "staffgrp.h"
#include "subst.h"
#include "supplied.h"
#include "surface.h"
#include "svg.h"
#include "syl.h"
#include "syllable.h"
#include "system.h"
#include "tempo.h"
#include "text.h"
#include "tie.h"
#include "trill.h"
#include "tuplet.h"
#include "turn.h"
#include "unclear.h"
#include "verse.h"
#include "vrv.h"
#include "zone.h"

namespace vrv {

std::vector<std::string> MeiInput::s_editorialElementNames = { "abbr", "add", "app", "annot", "choice", "corr",
    "damage", "del", "expan", "orig", "ref", "reg", "restore", "sic", "subst", "supplied", "unclear" };

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

MeiOutput::~MeiOutput() {}

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
            decl.append_attribute("href") = "https://music-encoding.org/schema/4.0.0/mei-all.rng";
            decl.append_attribute("type") = "application/xml";
            decl.append_attribute("schematypens") = "http://relaxng.org/ns/structure/1.0";

            // schematron processing instruction
            decl = meiDoc.append_child(pugi::node_declaration);
            decl.set_name("xml-model");
            decl.append_attribute("href") = "https://music-encoding.org/schema/4.0.0/mei-all.rng";
            decl.append_attribute("type") = "application/xml";
            decl.append_attribute("schematypens") = "http://purl.oclc.org/dsdl/schematron";

            m_mei = meiDoc.append_child("mei");
            m_mei.append_attribute("xmlns") = "http://www.music-encoding.org/ns/mei";
            m_mei.append_attribute("meiversion") = "4.0.0";

            // If the document is mensural, we have to undo the mensural (segments) cast off
            m_doc->ConvertToUnCastOffMensuralDoc();

            // this starts the call of all the functors
            m_doc->Save(this);

            // Redo the mensural segment cast of if necessary
            m_doc->ConvertToCastOffMensuralDoc();
        }
        else {
            if (m_doc->IsMensuralMusicOnly()) {
                LogError("MEI output by page is not possible for mensural music");
                return false;
            }
            if (m_page >= m_doc->GetPageCount()) {
                LogError("Page %d does not exist", m_page);
                return false;
            }
            Pages *pages = m_doc->GetPages();
            assert(pages);
            Page *page = dynamic_cast<Page *>(pages->GetChild(m_page));
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

        unsigned int output_flags = pugi::format_default;
        if (m_doc->GetOptions()->m_outputSmuflXmlEntities.GetValue()) {
            output_flags |= pugi::format_no_escapes;
        }

        if (m_writeToStreamString) {
            meiDoc.save(m_streamStringOutput, "    ", output_flags);
        }
        else {
            meiDoc.save_file(m_filename.c_str(), "    ", output_flags);
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
        WriteDoc(dynamic_cast<Doc *>(object));
        m_nodeStack.push_back(m_currentNode);
        return true;
    }

    if (object->Is(MDIV)) {
        m_currentNode = m_currentNode.append_child("mdiv");
        WriteMdiv(m_currentNode, dynamic_cast<Mdiv *>(object));
    }
    else if (object->Is(PAGES)) {
        if (!m_scoreBasedMEI) {
            m_currentNode = m_currentNode.append_child("pages");
        }
        else {
            m_currentNode = m_currentNode.append_child("score");
        }
        WritePages(m_currentNode, dynamic_cast<Pages *>(object));
    }
    else if (object->Is(SCORE)) {
        m_currentNode = m_currentNode.append_child("score");
        WriteScore(m_currentNode, dynamic_cast<Score *>(object));
    }

    // Page and content
    else if (object->Is(PAGE)) {
        if (!m_scoreBasedMEI) {
            m_currentNode = m_currentNode.append_child("page");
            WritePage(m_currentNode, dynamic_cast<Page *>(object));
        }
        else {
            return true;
        }
    }
    else if (object->Is(SYSTEM)) {
        if (!m_scoreBasedMEI) {
            m_currentNode = m_currentNode.append_child("system");
            WriteSystem(m_currentNode, dynamic_cast<System *>(object));
        }
        else {
            return true;
        }
    }

    // System boundaries
    else if (object->Is(ENDING)) {
        m_currentNode = m_currentNode.append_child("ending");
        WriteEnding(m_currentNode, dynamic_cast<Ending *>(object));
    }
    else if (object->Is(EXPANSION)) {
        m_currentNode = m_currentNode.append_child("expansion");
        WriteExpansion(m_currentNode, dynamic_cast<Expansion *>(object));
    }
    else if (object->Is(PB)) {
        m_currentNode = m_currentNode.append_child("pb");
        WritePb(m_currentNode, dynamic_cast<Pb *>(object));
    }
    else if (object->Is(SB)) {
        m_currentNode = m_currentNode.append_child("sb");
        WriteSb(m_currentNode, dynamic_cast<Sb *>(object));
    }
    else if (object->Is(SECTION)) {
        m_currentNode = m_currentNode.append_child("section");
        WriteSection(m_currentNode, dynamic_cast<Section *>(object));
    }

    // ScoreDef related
    else if (object->Is(INSTRDEF)) {
        m_currentNode = m_currentNode.append_child("instrDef");
        WriteInstrDef(m_currentNode, dynamic_cast<InstrDef *>(object));
    }
    else if (object->Is(LABEL)) {
        m_currentNode = m_currentNode.append_child("label");
        WriteLabel(m_currentNode, dynamic_cast<Label *>(object));
    }
    else if (object->Is(LABELABBR)) {
        m_currentNode = m_currentNode.append_child("labelAbbr");
        WriteLabelAbbr(m_currentNode, dynamic_cast<LabelAbbr *>(object));
    }
    else if (object->Is(SCOREDEF)) {
        m_currentNode = m_currentNode.append_child("scoreDef");
        WriteScoreDef(m_currentNode, dynamic_cast<ScoreDef *>(object));
    }
    else if (object->Is(PGFOOT)) {
        m_currentNode = m_currentNode.append_child("pgFoot");
        WritePgFoot(m_currentNode, dynamic_cast<PgFoot *>(object));
    }
    else if (object->Is(PGFOOT2)) {
        m_currentNode = m_currentNode.append_child("pgFoot2");
        WritePgFoot2(m_currentNode, dynamic_cast<PgFoot2 *>(object));
    }
    else if (object->Is(PGHEAD)) {
        m_currentNode = m_currentNode.append_child("pgHead");
        WritePgHead(m_currentNode, dynamic_cast<PgHead *>(object));
    }
    else if (object->Is(PGHEAD2)) {
        m_currentNode = m_currentNode.append_child("pgHead2");
        WritePgHead2(m_currentNode, dynamic_cast<PgHead2 *>(object));
    }
    else if (object->Is(STAFFGRP)) {
        m_currentNode = m_currentNode.append_child("staffGrp");
        WriteStaffGrp(m_currentNode, dynamic_cast<StaffGrp *>(object));
    }
    else if (object->Is(STAFFDEF)) {
        m_currentNode = m_currentNode.append_child("staffDef");
        WriteStaffDef(m_currentNode, dynamic_cast<StaffDef *>(object));
    }
    else if (object->Is(MEASURE)) {
        m_currentNode = m_currentNode.append_child("measure");
        WriteMeasure(m_currentNode, dynamic_cast<Measure *>(object));
    }
    else if (object->Is(STAFF)) {
        m_currentNode = m_currentNode.append_child("staff");
        WriteStaff(m_currentNode, dynamic_cast<Staff *>(object));
    }
    else if (object->Is(LAYER)) {
        m_currentNode = m_currentNode.append_child("layer");
        WriteLayer(m_currentNode, dynamic_cast<Layer *>(object));
    }

    // Measure elements
    else if (object->Is(ANCHORED_TEXT)) {
        m_currentNode = m_currentNode.append_child("anchoredText");
        WriteAnchoredText(m_currentNode, dynamic_cast<AnchoredText *>(object));
    }
    else if (object->Is(ARPEG)) {
        m_currentNode = m_currentNode.append_child("arpeg");
        WriteArpeg(m_currentNode, dynamic_cast<Arpeg *>(object));
    }
    else if (object->Is(BRACKETSPAN)) {
        m_currentNode = m_currentNode.append_child("bracketSpan");
        WriteBracketSpan(m_currentNode, dynamic_cast<BracketSpan *>(object));
    }
    else if (object->Is(BREATH)) {
        m_currentNode = m_currentNode.append_child("breath");
        WriteBreath(m_currentNode, dynamic_cast<Breath *>(object));
    }
    else if (object->Is(DIR)) {
        m_currentNode = m_currentNode.append_child("dir");
        WriteDir(m_currentNode, dynamic_cast<Dir *>(object));
    }
    else if (object->Is(DYNAM)) {
        m_currentNode = m_currentNode.append_child("dynam");
        WriteDynam(m_currentNode, dynamic_cast<Dynam *>(object));
    }
    else if (object->Is(FERMATA)) {
        if (!object->IsAttribute()) {
            m_currentNode = m_currentNode.append_child("fermata");
            WriteFermata(m_currentNode, dynamic_cast<Fermata *>(object));
        }
    }
    else if (object->Is(HAIRPIN)) {
        m_currentNode = m_currentNode.append_child("hairpin");
        WriteHairpin(m_currentNode, dynamic_cast<Hairpin *>(object));
    }
    else if (object->Is(HARM)) {
        m_currentNode = m_currentNode.append_child("harm");
        WriteHarm(m_currentNode, dynamic_cast<Harm *>(object));
    }
    else if (object->Is(MNUM)) {
        m_currentNode = m_currentNode.append_child("mNum");
        WriteMNum(m_currentNode, dynamic_cast<MNum *>(object));
    }
    else if (object->Is(MORDENT)) {
        m_currentNode = m_currentNode.append_child("mordent");
        WriteMordent(m_currentNode, dynamic_cast<Mordent *>(object));
    }
    else if (object->Is(OCTAVE)) {
        m_currentNode = m_currentNode.append_child("octave");
        WriteOctave(m_currentNode, dynamic_cast<Octave *>(object));
    }
    else if (object->Is(PEDAL)) {
        m_currentNode = m_currentNode.append_child("pedal");
        WritePedal(m_currentNode, dynamic_cast<Pedal *>(object));
    }
    else if (object->Is(SLUR)) {
        m_currentNode = m_currentNode.append_child("slur");
        WriteSlur(m_currentNode, dynamic_cast<Slur *>(object));
    }
    else if (object->Is(TEMPO)) {
        m_currentNode = m_currentNode.append_child("tempo");
        WriteTempo(m_currentNode, dynamic_cast<Tempo *>(object));
    }
    else if (object->Is(TIE)) {
        if (!object->IsAttribute()) {
            m_currentNode = m_currentNode.append_child("tie");
            WriteTie(m_currentNode, dynamic_cast<Tie *>(object));
        }
    }
    else if (object->Is(TRILL)) {
        m_currentNode = m_currentNode.append_child("trill");
        WriteTrill(m_currentNode, dynamic_cast<Trill *>(object));
    }
    else if (object->Is(TURN)) {
        m_currentNode = m_currentNode.append_child("turn");
        WriteTurn(m_currentNode, dynamic_cast<Turn *>(object));
    }

    // Layer elements
    else if (object->Is(ACCID)) {
        // Do not add a node for object representing an attribute
        if (!object->IsAttribute()) m_currentNode = m_currentNode.append_child("accid");
        WriteAccid(m_currentNode, dynamic_cast<Accid *>(object));
    }
    else if (object->Is(ARTIC)) {
        // Do not add a node for object representing an attribute
        if (!object->IsAttribute()) m_currentNode = m_currentNode.append_child("artic");
        WriteArtic(m_currentNode, dynamic_cast<Artic *>(object));
    }
    else if (object->Is(BARLINE)) {
        m_currentNode = m_currentNode.append_child("barLine");
        WriteBarLine(m_currentNode, dynamic_cast<BarLine *>(object));
    }
    else if (object->Is(BEAM)) {
        m_currentNode = m_currentNode.append_child("beam");
        WriteBeam(m_currentNode, dynamic_cast<Beam *>(object));
    }
    else if (object->Is(BEATRPT)) {
        m_currentNode = m_currentNode.append_child("beatRpt");
        WriteBeatRpt(m_currentNode, dynamic_cast<BeatRpt *>(object));
    }
    else if (object->Is(BTREM)) {
        m_currentNode = m_currentNode.append_child("bTrem");
        WriteBTrem(m_currentNode, dynamic_cast<BTrem *>(object));
    }
    else if (object->Is(CHORD)) {
        m_currentNode = m_currentNode.append_child("chord");
        WriteChord(m_currentNode, dynamic_cast<Chord *>(object));
    }
    else if (object->Is(CLEF)) {
        if (!object->IsAttribute()) m_currentNode = m_currentNode.append_child("clef");
        WriteClef(m_currentNode, dynamic_cast<Clef *>(object));
    }
    else if (object->Is(CUSTOS)) {
        m_currentNode = m_currentNode.append_child("custos");
        WriteCustos(m_currentNode, dynamic_cast<Custos *>(object));
    }
    else if (object->Is(DOT)) {
        m_currentNode = m_currentNode.append_child("dot");
        WriteDot(m_currentNode, dynamic_cast<Dot *>(object));
    }
    else if (object->Is(FTREM)) {
        m_currentNode = m_currentNode.append_child("fTrem");
        WriteFTrem(m_currentNode, dynamic_cast<FTrem *>(object));
    }
    else if (object->Is(GRACEGRP)) {
        m_currentNode = m_currentNode.append_child("graceGrp");
        WriteGraceGrp(m_currentNode, dynamic_cast<GraceGrp *>(object));
    }
    else if (object->Is(HALFMRPT)) {
        m_currentNode = m_currentNode.append_child("halfmRpt");
        WriteHalfmRpt(m_currentNode, dynamic_cast<HalfmRpt *>(object));
    }
    else if (object->Is(KEYACCID)) {
        m_currentNode = m_currentNode.append_child("keyAccid");
        WriteKeyAccid(m_currentNode, dynamic_cast<KeyAccid *>(object));
    }
    else if (object->Is(KEYSIG)) {
        if (!object->IsAttribute()) m_currentNode = m_currentNode.append_child("keySig");
        WriteKeySig(m_currentNode, dynamic_cast<KeySig *>(object));
    }
    else if (object->Is(LIGATURE)) {
        m_currentNode = m_currentNode.append_child("ligature");
        WriteLigature(m_currentNode, dynamic_cast<Ligature *>(object));
    }
    else if (object->Is(MENSUR)) {
        if (!object->IsAttribute()) m_currentNode = m_currentNode.append_child("mensur");
        WriteMensur(m_currentNode, dynamic_cast<Mensur *>(object));
    }
    else if (object->Is(METERSIG)) {
        if (!object->IsAttribute()) m_currentNode = m_currentNode.append_child("meterSig");
        WriteMeterSig(m_currentNode, dynamic_cast<MeterSig *>(object));
    }
    else if (object->Is(MREST)) {
        m_currentNode = m_currentNode.append_child("mRest");
        WriteMRest(m_currentNode, dynamic_cast<MRest *>(object));
    }
    else if (object->Is(MRPT)) {
        m_currentNode = m_currentNode.append_child("mRpt");
        WriteMRpt(m_currentNode, dynamic_cast<MRpt *>(object));
    }
    else if (object->Is(MRPT2)) {
        m_currentNode = m_currentNode.append_child("mRpt2");
        WriteMRpt2(m_currentNode, dynamic_cast<MRpt2 *>(object));
    }
    else if (object->Is(MULTIREST)) {
        m_currentNode = m_currentNode.append_child("multiRest");
        WriteMultiRest(m_currentNode, dynamic_cast<MultiRest *>(object));
    }
    else if (object->Is(MULTIRPT)) {
        m_currentNode = m_currentNode.append_child("multiRpt");
        WriteMultiRpt(m_currentNode, dynamic_cast<MultiRpt *>(object));
    }
    else if (object->Is(NC)) {
        m_currentNode = m_currentNode.append_child("nc");
        WriteNc(m_currentNode, dynamic_cast<Nc *>(object));
    }
    else if (object->Is(NEUME)) {
        m_currentNode = m_currentNode.append_child("neume");
        WriteNeume(m_currentNode, dynamic_cast<Neume *>(object));
    }
    else if (object->Is(NOTE)) {
        m_currentNode = m_currentNode.append_child("note");
        WriteNote(m_currentNode, dynamic_cast<Note *>(object));
    }
    else if (object->Is(PROPORT)) {
        m_currentNode = m_currentNode.append_child("proport");
        WriteProport(m_currentNode, dynamic_cast<Proport *>(object));
    }
    else if (object->Is(REST)) {
        m_currentNode = m_currentNode.append_child("rest");
        WriteRest(m_currentNode, dynamic_cast<Rest *>(object));
    }
    else if (object->Is(SPACE)) {
        m_currentNode = m_currentNode.append_child("space");
        WriteSpace(m_currentNode, dynamic_cast<Space *>(object));
    }
    else if (object->Is(SYL)) {
        m_currentNode = m_currentNode.append_child("syl");
        WriteSyl(m_currentNode, dynamic_cast<Syl *>(object));
    }
    else if (object->Is(SYLLABLE)) {
        m_currentNode = m_currentNode.append_child("syllable");
        WriteSyllable(m_currentNode, dynamic_cast<Syllable *>(object));
    }
    else if (object->Is(TUPLET)) {
        m_currentNode = m_currentNode.append_child("tuplet");
        WriteTuplet(m_currentNode, dynamic_cast<Tuplet *>(object));
    }
    else if (object->Is(VERSE)) {
        m_currentNode = m_currentNode.append_child("verse");
        WriteVerse(m_currentNode, dynamic_cast<Verse *>(object));
    }

    // Text elements
    else if (object->Is(FIG)) {
        m_currentNode = m_currentNode.append_child("fig");
        WriteFig(m_currentNode, dynamic_cast<Fig *>(object));
    }
    else if (object->Is(FIGURE)) {
        m_currentNode = m_currentNode.append_child("f");
        WriteF(m_currentNode, dynamic_cast<F *>(object));
    }
    else if (object->Is(FB)) {
        m_currentNode = m_currentNode.append_child("fb");
        WriteFb(m_currentNode, dynamic_cast<Fb *>(object));
    }
    else if (object->Is(LB)) {
        m_currentNode = m_currentNode.append_child("lb");
        WriteLb(m_currentNode, dynamic_cast<Lb *>(object));
    }
    else if (object->Is(NUM)) {
        m_currentNode = m_currentNode.append_child("num");
        WriteNum(m_currentNode, dynamic_cast<Num *>(object));
    }
    else if (object->Is(REND)) {
        m_currentNode = m_currentNode.append_child("rend");
        WriteRend(m_currentNode, dynamic_cast<Rend *>(object));
    }
    else if (object->Is(SVG)) {
        m_currentNode = m_currentNode.append_child("svg");
        WriteSvg(m_currentNode, dynamic_cast<Svg *>(object));
    }
    else if (object->Is(TEXT)) {
        WriteText(m_currentNode, dynamic_cast<Text *>(object));
    }

    // Editorial markup
    else if (object->Is(ABBR)) {
        m_currentNode = m_currentNode.append_child("abbr");
        WriteAbbr(m_currentNode, dynamic_cast<Abbr *>(object));
    }
    else if (object->Is(ADD)) {
        m_currentNode = m_currentNode.append_child("add");
        WriteAdd(m_currentNode, dynamic_cast<Add *>(object));
    }
    else if (object->Is(ANNOT)) {
        m_currentNode = m_currentNode.append_child("annot");
        WriteAnnot(m_currentNode, dynamic_cast<Annot *>(object));
    }
    else if (object->Is(APP)) {
        m_currentNode = m_currentNode.append_child("app");
        WriteApp(m_currentNode, dynamic_cast<App *>(object));
    }
    else if (object->Is(CHOICE)) {
        m_currentNode = m_currentNode.append_child("choice");
        WriteChoice(m_currentNode, dynamic_cast<Choice *>(object));
    }
    else if (object->Is(CORR)) {
        m_currentNode = m_currentNode.append_child("corr");
        WriteCorr(m_currentNode, dynamic_cast<Corr *>(object));
    }
    else if (object->Is(DAMAGE)) {
        m_currentNode = m_currentNode.append_child("damage");
        WriteDamage(m_currentNode, dynamic_cast<Damage *>(object));
    }
    else if (object->Is(DEL)) {
        m_currentNode = m_currentNode.append_child("del");
        WriteDel(m_currentNode, dynamic_cast<Del *>(object));
    }
    else if (object->Is(EXPAN)) {
        m_currentNode = m_currentNode.append_child("epxan");
        WriteExpan(m_currentNode, dynamic_cast<Expan *>(object));
    }
    else if (object->Is(LEM)) {
        m_currentNode = m_currentNode.append_child("lem");
        WriteLem(m_currentNode, dynamic_cast<Lem *>(object));
    }
    else if (object->Is(ORIG)) {
        m_currentNode = m_currentNode.append_child("orig");
        WriteOrig(m_currentNode, dynamic_cast<Orig *>(object));
    }
    else if (object->Is(RDG)) {
        m_currentNode = m_currentNode.append_child("rdg");
        WriteRdg(m_currentNode, dynamic_cast<Rdg *>(object));
    }
    else if (object->Is(REF)) {
        m_currentNode = m_currentNode.append_child("ref");
        WriteRef(m_currentNode, dynamic_cast<Ref *>(object));
    }
    else if (object->Is(REG)) {
        m_currentNode = m_currentNode.append_child("reg");
        WriteReg(m_currentNode, dynamic_cast<Reg *>(object));
    }
    else if (object->Is(RESTORE)) {
        m_currentNode = m_currentNode.append_child("restore");
        WriteRestore(m_currentNode, dynamic_cast<Restore *>(object));
    }
    else if (object->Is(SIC)) {
        m_currentNode = m_currentNode.append_child("sic");
        WriteSic(m_currentNode, dynamic_cast<Sic *>(object));
    }
    else if (object->Is(SUBST)) {
        m_currentNode = m_currentNode.append_child("subst");
        WriteSubst(m_currentNode, dynamic_cast<Subst *>(object));
    }
    else if (object->Is(SUPPLIED)) {
        m_currentNode = m_currentNode.append_child("supplied");
        WriteSupplied(m_currentNode, dynamic_cast<Supplied *>(object));
    }
    else if (object->Is(UNCLEAR)) {
        m_currentNode = m_currentNode.append_child("unclear");
        WriteUnclear(m_currentNode, dynamic_cast<Unclear *>(object));
    }

    // BoundaryEnd - nothing to add - only
    else if (object->Is(BOUNDARY_END)) {
        if (m_scoreBasedMEI) {
            // LogDebug("No piling '%s'", object->GetClassName().c_str());
            return true;
        }
        else {
            m_currentNode = m_currentNode.append_child("boundaryEnd");
            WriteBoundaryEnd(m_currentNode, dynamic_cast<BoundaryEnd *>(object));
        }
    }

    else {
        // Missing output method for the class
        LogError("Output method missing for '%s'", object->GetClassName().c_str());
        assert(false); // let's make it stop because this should not happen
    }

    // Object representing an attribute have no node to push
    if (!object->IsAttribute()) m_nodeStack.push_back(m_currentNode);

    if (object->Is(PAGES) && (dynamic_cast<Pages *>(object) == m_doc->GetPages())) {
        // First save the main scoreDef
        m_doc->m_scoreDef.Save(this);
    }
    else if (object->Is(SCORE) && (dynamic_cast<Score *>(object) == m_doc->GetScore())) {
        // First save the main scoreDef
        m_doc->m_scoreDef.Save(this);
    }

    WriteUnsupportedAttr(m_currentNode, object);

    return true;
}

bool MeiOutput::WriteObjectEnd(Object *object)
{
    if (m_scoreBasedMEI && object->IsBoundaryElement()) {
        return true;
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

bool MeiOutput::WriteDoc(Doc *doc)
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
    Facsimile *facs = doc->GetFacsimile();
    if ((facs != NULL) && (facs->GetChildCount() > 0)) {
        pugi::xml_node facsimile = music.append_child("facsimile");
        WriteFacsimile(facsimile, facs);
        m_nodeStack.push_back(facsimile);
    }

    if (m_doc->m_front.first_child()) {
        music.append_copy(m_doc->m_front.first_child());
    }

    m_currentNode = music.append_child("body");
    m_nodeStack.push_back(m_currentNode);

    if (m_doc->m_back.first_child()) {
        music.append_copy(m_doc->m_back.first_child());
    }

    /*
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
     */

    return true;
}

void MeiOutput::WriteMdiv(pugi::xml_node currentNode, Mdiv *mdiv)
{
    assert(mdiv);

    WriteXmlId(currentNode, mdiv);
    mdiv->WriteLabelled(currentNode);
    mdiv->WriteNNumberLike(currentNode);
}

void MeiOutput::WritePages(pugi::xml_node currentNode, Pages *pages)
{
    assert(pages);

    if (!m_scoreBasedMEI) {
        m_currentNode.append_attribute("type") = DocTypeToStr(m_doc->GetType()).c_str();
        m_currentNode.append_child(pugi::node_comment).set_value("Coordinates in MEI axis direction");
    }

    WriteXmlId(currentNode, pages);
    pages->WriteLabelled(currentNode);
    pages->WriteNNumberLike(currentNode);
}

void MeiOutput::WriteScore(pugi::xml_node currentNode, Score *score)
{
    assert(score);

    WriteXmlId(currentNode, score);
    score->WriteLabelled(currentNode);
    score->WriteNNumberLike(currentNode);
}

void MeiOutput::WritePage(pugi::xml_node currentNode, Page *page)
{
    assert(page);

    WriteXmlId(currentNode, page);
    // size and margins but only if any - we rely on page.height only to check this
    if (page->m_pageHeight != -1) {
        currentNode.append_attribute("page.width") = StringFormat("%d", page->m_pageWidth / DEFINITION_FACTOR).c_str();
        currentNode.append_attribute("page.height")
            = StringFormat("%d", page->m_pageHeight / DEFINITION_FACTOR).c_str();
        currentNode.append_attribute("page.leftmar")
            = StringFormat("%d", page->m_pageMarginLeft / DEFINITION_FACTOR).c_str();
        currentNode.append_attribute("page.rightmar")
            = StringFormat("%d", page->m_pageMarginRight / DEFINITION_FACTOR).c_str();
        currentNode.append_attribute("page.rightmar")
            = StringFormat("%d", page->m_pageMarginRight / DEFINITION_FACTOR).c_str();
    }
    if (!page->m_surface.empty()) {
        currentNode.append_attribute("surface") = page->m_surface.c_str();
    }
    if (page->m_PPUFactor != 1.0) {
        currentNode.append_attribute("ppu") = StringFormat("%f", page->m_PPUFactor).c_str();
    }
}

void MeiOutput::WriteSystem(pugi::xml_node currentNode, System *system)
{
    assert(system);

    WriteXmlId(currentNode, system);
    // margins
    currentNode.append_attribute("system.leftmar")
        = StringFormat("%d", system->m_systemLeftMar / DEFINITION_FACTOR).c_str();
    currentNode.append_attribute("system.rightmar")
        = StringFormat("%d", system->m_systemRightMar / DEFINITION_FACTOR).c_str();
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

void MeiOutput::WriteBoundaryEnd(pugi::xml_node currentNode, BoundaryEnd *boundaryEnd)
{
    assert(boundaryEnd && boundaryEnd->GetStart());

    WriteSystemElement(currentNode, boundaryEnd);
    currentNode.append_attribute("startid") = UuidToMeiStr(boundaryEnd->GetStart()).c_str();
    std::string meiElementName = boundaryEnd->GetStart()->GetClassName();
    std::transform(meiElementName.begin(), meiElementName.begin() + 1, meiElementName.begin(), ::tolower);
    currentNode.append_attribute("type") = meiElementName.c_str();
}

void MeiOutput::WriteSection(pugi::xml_node currentNode, Section *section)
{
    assert(section);

    WriteSystemElement(currentNode, section);
    section->WriteNNumberLike(currentNode);
}

void MeiOutput::WriteEnding(pugi::xml_node currentNode, Ending *ending)
{
    assert(ending);

    WriteSystemElement(currentNode, ending);
    ending->WriteLineRend(currentNode);
    ending->WriteNNumberLike(currentNode);
}

void MeiOutput::WriteExpansion(pugi::xml_node currentNode, Expansion *expansion)
{
    assert(expansion);

    WritePlistInterface(currentNode, expansion);
    WriteSystemElement(currentNode, expansion);
}

void MeiOutput::WritePb(pugi::xml_node currentNode, Pb *pb)
{
    assert(pb);

    WriteSystemElement(currentNode, pb);
    pb->WriteNNumberLike(currentNode);
}

void MeiOutput::WriteSb(pugi::xml_node currentNode, Sb *sb)
{
    assert(sb);

    WriteSystemElement(currentNode, sb);
    sb->WriteNNumberLike(currentNode);
}

void MeiOutput::WriteScoreDefElement(pugi::xml_node currentNode, ScoreDefElement *scoreDefElement)
{
    assert(scoreDefElement);

    WriteXmlId(currentNode, scoreDefElement);
    scoreDefElement->WriteMeasureNumbers(currentNode);
    scoreDefElement->WriteTyped(currentNode);
}

void MeiOutput::WriteScoreDef(pugi::xml_node currentNode, ScoreDef *scoreDef)
{
    assert(scoreDef);

    WriteScoreDefElement(currentNode, scoreDef);
    WriteScoreDefInterface(currentNode, scoreDef);
    scoreDef->WriteEndings(currentNode);
    scoreDef->WriteOptimization(currentNode);
}

void MeiOutput::WriteRunningElement(pugi::xml_node currentNode, RunningElement *runningElement)
{
    assert(runningElement);

    WriteXmlId(currentNode, runningElement);
    runningElement->WriteHorizontalAlign(currentNode);
    runningElement->WriteTyped(currentNode);
}

void MeiOutput::WritePgFoot(pugi::xml_node currentNode, PgFoot *pgFoot)
{
    assert(pgFoot);

    WriteRunningElement(currentNode, pgFoot);
}

void MeiOutput::WritePgFoot2(pugi::xml_node currentNode, PgFoot2 *pgFoot2)
{
    assert(pgFoot2);

    WriteRunningElement(currentNode, pgFoot2);
}

void MeiOutput::WritePgHead(pugi::xml_node currentNode, PgHead *pgHead)
{
    assert(pgHead);

    WriteRunningElement(currentNode, pgHead);
}

void MeiOutput::WritePgHead2(pugi::xml_node currentNode, PgHead2 *pgHead2)
{
    assert(pgHead2);

    WriteRunningElement(currentNode, pgHead2);
}

void MeiOutput::WriteStaffGrp(pugi::xml_node currentNode, StaffGrp *staffGrp)
{
    assert(staffGrp);

    WriteXmlId(currentNode, staffGrp);
    staffGrp->WriteBasic(currentNode);
    staffGrp->WriteLabelled(currentNode);
    staffGrp->WriteStaffGroupingSym(currentNode);
    staffGrp->WriteStaffGrpVis(currentNode);
    staffGrp->WriteTyped(currentNode);
}

void MeiOutput::WriteStaffDef(pugi::xml_node currentNode, StaffDef *staffDef)
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
    staffDef->WriteStaffDefVis(currentNode);
    staffDef->WriteTimeBase(currentNode);
    staffDef->WriteTransposition(currentNode);
}

void MeiOutput::WriteInstrDef(pugi::xml_node currentNode, InstrDef *instrDef)
{
    assert(instrDef);

    WriteXmlId(currentNode, instrDef);
    instrDef->WriteChannelized(currentNode);
    instrDef->WriteLabelled(currentNode);
    instrDef->WriteMidiInstrument(currentNode);
    instrDef->WriteNNumberLike(currentNode);
}

void MeiOutput::WriteLabel(pugi::xml_node currentNode, Label *label)
{
    assert(label);

    WriteXmlId(currentNode, label);
}

void MeiOutput::WriteLabelAbbr(pugi::xml_node currentNode, LabelAbbr *labelAbbr)
{
    assert(labelAbbr);

    WriteXmlId(currentNode, labelAbbr);
}

void MeiOutput::WriteMeasure(pugi::xml_node currentNode, Measure *measure)
{
    assert(measure);

    WriteXmlId(currentNode, measure);
    measure->WriteBarring(currentNode);
    measure->WriteMeasureLog(currentNode);
    measure->WriteMeterConformanceBar(currentNode);
    measure->WriteNNumberLike(currentNode);
    measure->WritePointing(currentNode);
    measure->WriteTyped(currentNode);
    if ((measure->m_xAbs != VRV_UNSET) && (measure->m_xAbs2 != VRV_UNSET)) {
        currentNode.append_attribute("ulx") = StringFormat("%d", measure->m_xAbs / DEFINITION_FACTOR).c_str();
        currentNode.append_attribute("lrx") = StringFormat("%d", measure->m_xAbs2 / DEFINITION_FACTOR).c_str();
    }
}

void MeiOutput::WriteFb(pugi::xml_node currentNode, Fb *fb)
{
    assert(fb);

    WriteXmlId(currentNode, fb);
}

void MeiOutput::WriteControlElement(pugi::xml_node currentNode, ControlElement *controlElement)
{
    assert(controlElement);

    WriteXmlId(currentNode, controlElement);
    WriteLinkingInterface(currentNode, controlElement);
    controlElement->WriteLabelled(currentNode);
    controlElement->WriteTyped(currentNode);
}

void MeiOutput::WriteAnchoredText(pugi::xml_node currentNode, AnchoredText *anchoredText)
{
    assert(anchoredText);

    WriteControlElement(currentNode, anchoredText);
    WriteTextDirInterface(currentNode, anchoredText);
}

void MeiOutput::WriteArpeg(pugi::xml_node currentNode, Arpeg *arpeg)
{
    assert(arpeg);

    WriteControlElement(currentNode, arpeg);
    WritePlistInterface(currentNode, arpeg);
    WriteTimePointInterface(currentNode, arpeg);
    arpeg->WriteArpegLog(currentNode);
    arpeg->WriteArpegVis(currentNode);
    arpeg->WriteColor(currentNode);
}

void MeiOutput::WriteBracketSpan(pugi::xml_node currentNode, BracketSpan *bracketSpan)
{
    assert(bracketSpan);

    WriteControlElement(currentNode, bracketSpan);
    WriteTimeSpanningInterface(currentNode, bracketSpan);
    bracketSpan->WriteBracketSpanLog(currentNode);
    bracketSpan->WriteColor(currentNode);
    bracketSpan->WriteLineRend(currentNode);
    bracketSpan->WriteLineRendBase(currentNode);
}

void MeiOutput::WriteBreath(pugi::xml_node currentNode, Breath *breath)
{
    assert(breath);

    WriteControlElement(currentNode, breath);
    WriteTimePointInterface(currentNode, breath);
    breath->WriteColor(currentNode);
    breath->WritePlacement(currentNode);
}

void MeiOutput::WriteDir(pugi::xml_node currentNode, Dir *dir)
{
    assert(dir);

    WriteControlElement(currentNode, dir);
    WriteTextDirInterface(currentNode, dir);
    WriteTimeSpanningInterface(currentNode, dir);
    dir->WriteLang(currentNode);
    dir->WriteExtender(currentNode);
    dir->WriteVerticalGroup(currentNode);
}

void MeiOutput::WriteDynam(pugi::xml_node currentNode, Dynam *dynam)
{
    assert(dynam);

    WriteControlElement(currentNode, dynam);
    WriteTextDirInterface(currentNode, dynam);
    WriteTimeSpanningInterface(currentNode, dynam);
    dynam->WriteExtender(currentNode);
    dynam->WriteVerticalGroup(currentNode);
}

void MeiOutput::WriteFermata(pugi::xml_node currentNode, Fermata *fermata)
{
    assert(fermata);

    WriteControlElement(currentNode, fermata);
    WriteTimePointInterface(currentNode, fermata);
    fermata->WriteColor(currentNode);
    fermata->WriteFermataVis(currentNode);
    fermata->WritePlacement(currentNode);
}

void MeiOutput::WriteHairpin(pugi::xml_node currentNode, Hairpin *hairpin)
{
    assert(hairpin);

    WriteControlElement(currentNode, hairpin);
    WriteTimeSpanningInterface(currentNode, hairpin);
    hairpin->WriteColor(currentNode);
    hairpin->WriteHairpinLog(currentNode);
    hairpin->WritePlacement(currentNode);
    hairpin->WriteVerticalGroup(currentNode);
}

void MeiOutput::WriteHarm(pugi::xml_node currentNode, Harm *harm)
{
    assert(harm);

    WriteControlElement(currentNode, harm);
    WriteTextDirInterface(currentNode, harm);
    WriteTimeSpanningInterface(currentNode, harm);
    harm->WriteLang(currentNode);
    harm->WriteNNumberLike(currentNode);
}

void MeiOutput::WriteMNum(pugi::xml_node currentNode, MNum *mNum)
{
    assert(mNum);

    WriteControlElement(currentNode, mNum);
    WriteTextDirInterface(currentNode, mNum);
    WriteTimePointInterface(currentNode, mNum);
    mNum->WriteColor(currentNode);
    mNum->WriteLang(currentNode);
    mNum->WriteTypography(currentNode);
}

void MeiOutput::WriteMordent(pugi::xml_node currentNode, Mordent *mordent)
{
    assert(mordent);

    WriteControlElement(currentNode, mordent);
    WriteTimePointInterface(currentNode, mordent);
    mordent->WriteColor(currentNode);
    mordent->WriteOrnamentAccid(currentNode);
    mordent->WritePlacement(currentNode);
    mordent->WriteMordentLog(currentNode);
}

void MeiOutput::WriteOctave(pugi::xml_node currentNode, Octave *octave)
{
    assert(octave);

    WriteControlElement(currentNode, octave);
    WriteTimeSpanningInterface(currentNode, octave);
    octave->WriteColor(currentNode);
    octave->WriteExtender(currentNode);
    octave->WriteLineRend(currentNode);
    octave->WriteLineRendBase(currentNode);
    octave->WriteOctaveDisplacement(currentNode);
}

void MeiOutput::WritePedal(pugi::xml_node currentNode, Pedal *pedal)
{
    assert(pedal);

    WriteControlElement(currentNode, pedal);
    WriteTimePointInterface(currentNode, pedal);
    pedal->WriteColor(currentNode);
    pedal->WritePedalLog(currentNode);
    pedal->WritePlacement(currentNode);
    pedal->WriteVerticalGroup(currentNode);
}

void MeiOutput::WriteSlur(pugi::xml_node currentNode, Slur *slur)
{
    assert(slur);

    WriteControlElement(currentNode, slur);
    WriteTimeSpanningInterface(currentNode, slur);
    slur->WriteColor(currentNode);
    slur->WriteCurvature(currentNode);
    slur->WriteCurveRend(currentNode);
}

void MeiOutput::WriteStaff(pugi::xml_node currentNode, Staff *staff)
{
    assert(staff);

    WriteXmlId(currentNode, staff);
    staff->WriteFacsimile(currentNode);
    staff->WriteNInteger(currentNode);
    staff->WriteTyped(currentNode);
    staff->WriteVisibility(currentNode);

    // y position
    if (staff->m_yAbs != VRV_UNSET) {
        currentNode.append_attribute("uly") = StringFormat("%d", staff->m_yAbs / DEFINITION_FACTOR).c_str();
    }
}

void MeiOutput::WriteTempo(pugi::xml_node currentNode, Tempo *tempo)
{
    assert(tempo);

    WriteControlElement(currentNode, tempo);
    WriteTextDirInterface(currentNode, tempo);
    WriteTimePointInterface(currentNode, tempo);
    tempo->WriteLang(currentNode);
    tempo->WriteMidiTempo(currentNode);
    tempo->WriteMmTempo(currentNode);
}

void MeiOutput::WriteTie(pugi::xml_node currentNode, Tie *tie)
{
    assert(tie);

    WriteControlElement(currentNode, tie);
    WriteTimeSpanningInterface(currentNode, tie);
    tie->WriteColor(currentNode);
    tie->WriteCurvature(currentNode);
    tie->WriteCurveRend(currentNode);
}

void MeiOutput::WriteTrill(pugi::xml_node currentNode, Trill *trill)
{
    assert(trill);

    WriteControlElement(currentNode, trill);
    WriteTimeSpanningInterface(currentNode, trill);
    trill->WriteColor(currentNode);
    trill->WriteOrnamentAccid(currentNode);
    trill->WritePlacement(currentNode);
}

void MeiOutput::WriteTurn(pugi::xml_node currentNode, Turn *turn)
{
    assert(turn);

    WriteControlElement(currentNode, turn);
    WriteTimePointInterface(currentNode, turn);
    turn->WriteColor(currentNode);
    turn->WriteOrnamentAccid(currentNode);
    turn->WritePlacement(currentNode);
    turn->WriteTurnLog(currentNode);
}

void MeiOutput::WriteLayer(pugi::xml_node currentNode, Layer *layer)
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
    WriteLinkingInterface(currentNode, element);
    element->WriteLabelled(currentNode);
    element->WriteTyped(currentNode);
    if (element->m_xAbs != VRV_UNSET) {
        currentNode.attribute("ulx") = StringFormat("%d", element->m_xAbs / DEFINITION_FACTOR).c_str();
    }
}

void MeiOutput::WriteAccid(pugi::xml_node currentNode, Accid *accid)
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

void MeiOutput::WriteArtic(pugi::xml_node currentNode, Artic *artic)
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

void MeiOutput::WriteBarLine(pugi::xml_node currentNode, BarLine *barLine)
{
    assert(barLine);

    WriteLayerElement(currentNode, barLine);
    barLine->WriteBarLineLog(currentNode);
    barLine->WriteColor(currentNode);
    barLine->WriteVisibility(currentNode);
}

void MeiOutput::WriteBeam(pugi::xml_node currentNode, Beam *beam)
{
    assert(beam);

    WriteLayerElement(currentNode, beam);
    beam->WriteColor(currentNode);
    beam->WriteBeamedWith(currentNode);
    beam->WriteBeamRend(currentNode);
}

void MeiOutput::WriteBeatRpt(pugi::xml_node currentNode, BeatRpt *beatRpt)
{
    assert(beatRpt);

    WriteLayerElement(currentNode, beatRpt);
    beatRpt->WriteColor(currentNode);
    beatRpt->WriteBeatRptVis(currentNode);
}

void MeiOutput::WriteBTrem(pugi::xml_node currentNode, BTrem *bTrem)
{
    assert(bTrem);

    WriteLayerElement(currentNode, bTrem);
    bTrem->WriteTremMeasured(currentNode);
}

void MeiOutput::WriteChord(pugi::xml_node currentNode, Chord *chord)
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

void MeiOutput::WriteClef(pugi::xml_node currentNode, Clef *clef)
{
    assert(clef);

    if (clef->IsAttribute()) {
        AttCleffingLog cleffingLog;
        cleffingLog.SetClefShape(clef->GetShape());
        cleffingLog.SetClefLine(clef->GetLine());
        cleffingLog.SetClefDis(clef->GetDis());
        cleffingLog.SetClefDisPlace(clef->GetDisPlace());
        cleffingLog.WriteCleffingLog(currentNode);
        AttCleffingVis cleffingVis;
        cleffingVis.SetClefColor(clef->GetColor());
        cleffingVis.WriteCleffingVis(currentNode);
        return;
    }

    WriteLayerElement(currentNode, clef);
    WriteFacsimileInterface(currentNode, clef);
    clef->WriteClefShape(currentNode);
    clef->WriteColor(currentNode);
    clef->WriteLineLoc(currentNode);
    clef->WriteOctaveDisplacement(currentNode);
}

void MeiOutput::WriteCustos(pugi::xml_node currentNode, Custos *custos)
{
    assert(custos);

    WriteFacsimileInterface(currentNode, custos);
    WritePitchInterface(currentNode, custos);
    WritePositionInterface(currentNode, custos);
    WriteLayerElement(currentNode, custos);
    custos->WriteColor(currentNode);
}

void MeiOutput::WriteDot(pugi::xml_node currentNode, Dot *dot)
{
    assert(dot);

    WriteLayerElement(currentNode, dot);
    WritePositionInterface(currentNode, dot);
    dot->WriteColor(currentNode);
    dot->WriteDotLog(currentNode);
}

void MeiOutput::WriteFTrem(pugi::xml_node currentNode, FTrem *fTrem)
{
    assert(fTrem);

    WriteLayerElement(currentNode, fTrem);
    fTrem->WriteFTremVis(currentNode);
    fTrem->WriteTremMeasured(currentNode);
}

void MeiOutput::WriteGraceGrp(pugi::xml_node currentNode, GraceGrp *graceGrp)
{
    assert(graceGrp);

    WriteLayerElement(currentNode, graceGrp);
    graceGrp->WriteColor(currentNode);
    graceGrp->WriteGraced(currentNode);
    graceGrp->WriteGraceGrpLog(currentNode);
}

void MeiOutput::WriteHalfmRpt(pugi::xml_node currentNode, HalfmRpt *halfmRpt)
{
    assert(halfmRpt);

    WriteLayerElement(currentNode, halfmRpt);
}

void MeiOutput::WriteKeyAccid(pugi::xml_node currentNode, KeyAccid *keyAccid)
{
    assert(keyAccid);

    WriteLayerElement(currentNode, keyAccid);
    WritePitchInterface(currentNode, keyAccid);
    keyAccid->WriteAccidental(currentNode);
    keyAccid->WriteColor(currentNode);
    keyAccid->WriteEnclosingChars(currentNode);
}

void MeiOutput::WriteKeySig(pugi::xml_node currentNode, KeySig *keySig)
{
    assert(keySig);

    if (keySig->IsAttribute()) {
        AttKeySigDefaultAnl attKeySigDefaultAnl;
        // Broken in MEI 4.0.2 - waiting for a fix
        // attKeySigDefaultAnl.SetKeyAccid(keySig->GetAccid());
        attKeySigDefaultAnl.SetKeyMode(keySig->GetMode());
        attKeySigDefaultAnl.SetKeyPname(keySig->GetPname());
        attKeySigDefaultAnl.WriteKeySigDefaultAnl(currentNode);
        AttKeySigDefaultLog attKeySigDefaultLog;
        attKeySigDefaultLog.SetKeySig(keySig->GetSig());
        attKeySigDefaultLog.WriteKeySigDefaultLog(currentNode);
        AttKeySigDefaultVis attKeySigDefaultVis;
        attKeySigDefaultVis.SetKeysigShow(keySig->GetVisible());
        attKeySigDefaultVis.SetKeysigShowchange(keySig->GetSigShowchange());
        attKeySigDefaultVis.WriteKeySigDefaultVis(currentNode);
        return;
    }

    WriteLayerElement(currentNode, keySig);
    keySig->WriteAccidental(currentNode);
    keySig->WritePitch(currentNode);
    keySig->WriteKeySigAnl(currentNode);
    keySig->WriteKeySigLog(currentNode);
    keySig->WriteKeySigVis(currentNode);
    keySig->WriteVisibility(currentNode);
}

void MeiOutput::WriteLigature(pugi::xml_node currentNode, Ligature *ligature)
{
    assert(ligature);

    WriteLayerElement(currentNode, ligature);
    ligature->WriteLigatureLog(currentNode);
}

void MeiOutput::WriteMensur(pugi::xml_node currentNode, Mensur *mensur)
{
    assert(mensur);

    if (mensur->IsAttribute()) {
        AttMensuralLog mensuralLog;
        mensuralLog.SetMensurDot(mensur->GetDot());
        mensuralLog.SetProportNum(mensur->GetNum());
        mensuralLog.SetProportNumbase(mensur->GetNumbase());
        mensuralLog.SetMensurSign(mensur->GetSign());
        mensuralLog.SetMensurSlash(mensur->GetSlash());
        mensuralLog.WriteMensuralLog(currentNode);
        AttMensuralShared mensuralShared;
        mensuralShared.SetModusmaior(mensur->GetModusmaior());
        mensuralShared.SetModusminor(mensur->GetModusminor());
        mensuralShared.SetProlatio(mensur->GetProlatio());
        mensuralShared.SetTempus(mensur->GetTempus());
        mensuralShared.WriteMensuralShared(currentNode);
        AttMensuralVis mensuralVis;
        mensuralVis.SetMensurColor(mensur->GetColor());
        mensuralVis.SetMensurOrient(mensur->GetOrient());
        mensuralVis.WriteMensuralVis(currentNode);
        return;
    }

    WriteLayerElement(currentNode, mensur);
    mensur->WriteColor(currentNode);
    mensur->WriteCue(currentNode);
    mensur->WriteDurationRatio(currentNode);
    mensur->WriteMensuralShared(currentNode);
    mensur->WriteMensurLog(currentNode);
    mensur->WriteMensurVis(currentNode);
    mensur->WriteSlashCount(currentNode);
    mensur->WriteStaffLoc(currentNode);
}

void MeiOutput::WriteMeterSig(pugi::xml_node currentNode, MeterSig *meterSig)
{
    assert(meterSig);

    if (meterSig->IsAttribute()) {
        AttMeterSigDefaultLog meterSigDefaultLog;
        meterSigDefaultLog.SetMeterCount(meterSig->GetCount());
        meterSigDefaultLog.SetMeterSym(meterSig->GetSym());
        meterSigDefaultLog.SetMeterUnit(meterSig->GetUnit());
        meterSigDefaultLog.WriteMeterSigDefaultLog(currentNode);
        AttMeterSigDefaultVis meterSigDefaultVis;
        meterSigDefaultVis.SetMeterForm(meterSig->GetForm());
        meterSigDefaultVis.WriteMeterSigDefaultVis(currentNode);
        return;
    }

    WriteLayerElement(currentNode, meterSig);
    meterSig->WriteMeterSigLog(currentNode);
    meterSig->WriteMeterSigVis(currentNode);
}

void MeiOutput::WriteMRest(pugi::xml_node currentNode, MRest *mRest)
{
    assert(mRest);

    WriteLayerElement(currentNode, mRest);
    WritePositionInterface(currentNode, mRest);
    mRest->WriteCue(currentNode);
    mRest->WriteFermataPresent(currentNode);
    mRest->WriteVisibility(currentNode);
}

void MeiOutput::WriteMRpt(pugi::xml_node currentNode, MRpt *mRpt)
{
    assert(mRpt);

    WriteLayerElement(currentNode, mRpt);
}

void MeiOutput::WriteMRpt2(pugi::xml_node currentNode, MRpt2 *mRpt2)
{
    assert(mRpt2);

    WriteLayerElement(currentNode, mRpt2);
}

void MeiOutput::WriteMultiRest(pugi::xml_node currentNode, MultiRest *multiRest)
{
    assert(multiRest);

    WriteLayerElement(currentNode, multiRest);
    multiRest->WriteMultiRestVis(currentNode);
    multiRest->WriteNumbered(currentNode);
}

void MeiOutput::WriteMultiRpt(pugi::xml_node currentNode, MultiRpt *multiRpt)
{
    assert(multiRpt);

    WriteLayerElement(currentNode, multiRpt);
    multiRpt->WriteNumbered(currentNode);
}

void MeiOutput::WriteNc(pugi::xml_node currentNode, Nc *nc)
{
    assert(nc);

    WriteLayerElement(currentNode, nc);
    WriteDurationInterface(currentNode, nc);
    WriteFacsimileInterface(currentNode, nc);
    WritePitchInterface(currentNode, nc);
    WritePositionInterface(currentNode, nc);
    nc->WriteColor(currentNode);
    nc->WriteIntervalMelodic(currentNode);
    nc->WriteNcForm(currentNode);
}

void MeiOutput::WriteNeume(pugi::xml_node currentNode, Neume *neume)
{
    assert(neume);

    WriteLayerElement(currentNode, neume);
    WriteFacsimileInterface(currentNode, neume);
    neume->WriteColor(currentNode);
}

void MeiOutput::WriteNote(pugi::xml_node currentNode, Note *note)
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
    note->WriteMidiVelocity(currentNode);
    note->WriteNoteAnlMensural(currentNode);
    note->WriteStems(currentNode);
    note->WriteStemsCmn(currentNode);
    note->WriteTiePresent(currentNode);
    note->WriteVisibility(currentNode);
}

void MeiOutput::WriteRest(pugi::xml_node currentNode, Rest *rest)
{
    assert(rest);

    WriteLayerElement(currentNode, rest);
    WriteDurationInterface(currentNode, rest);
    WritePositionInterface(currentNode, rest);
    rest->WriteColor(currentNode);
    rest->WriteCue(currentNode);
    rest->WriteRestVisMensural(currentNode);
}

void MeiOutput::WriteProport(pugi::xml_node currentNode, Proport *proport)
{
    assert(proport);

    WriteLayerElement(currentNode, proport);
}

void MeiOutput::WriteSpace(pugi::xml_node currentNode, Space *space)
{
    assert(space);

    WriteLayerElement(currentNode, space);
    WriteDurationInterface(currentNode, space);
}

void MeiOutput::WriteTuplet(pugi::xml_node currentNode, Tuplet *tuplet)
{
    assert(tuplet);

    WriteLayerElement(currentNode, tuplet);
    tuplet->WriteColor(currentNode);
    tuplet->WriteDurationRatio(currentNode);
    tuplet->WriteNumberPlacement(currentNode);
    tuplet->WriteTupletVis(currentNode);
}

void MeiOutput::WriteVerse(pugi::xml_node currentNode, Verse *verse)
{
    assert(verse);

    WriteLayerElement(currentNode, verse);
    verse->WriteColor(currentNode);
    verse->WriteLang(currentNode);
    verse->WriteNInteger(currentNode);
    verse->WriteTypography(currentNode);
}

void MeiOutput::WriteSyl(pugi::xml_node currentNode, Syl *syl)
{
    assert(syl);

    WriteLayerElement(currentNode, syl);
    syl->WriteLang(currentNode);
    syl->WriteTypography(currentNode);
    syl->WriteSylLog(currentNode);
}

void MeiOutput::WriteSyllable(pugi::xml_node currentNode, Syllable *syllable)
{
    assert(syllable);

    WriteLayerElement(currentNode, syllable);
    syllable->WriteColor(currentNode);
    syllable->WriteSlashCount(currentNode);
}

void MeiOutput::WriteFacsimile(pugi::xml_node currentNode, Facsimile *facsimile)
{
    assert(facsimile);
    WriteXmlId(currentNode, facsimile);

    // Write Surface(s)
    for (Object *child = facsimile->GetFirst(); child != NULL; child = facsimile->GetNext()) {
        if (child->GetClassId() == SURFACE) {
            pugi::xml_node childNode = currentNode.append_child("surface");
            WriteSurface(childNode, dynamic_cast<Surface *>(child));
        }
        else {
            LogWarning("Unable to write child '%s' of facsimile", child->GetClassName().c_str());
        }
    }
}

void MeiOutput::WriteSurface(pugi::xml_node currentNode, Surface *surface)
{
    assert(surface);
    WriteXmlId(currentNode, surface);
    surface->WriteCoordinated(currentNode);
    surface->WriteTyped(currentNode);

    for (Object *child = surface->GetFirst(); child != NULL; child = surface->GetNext()) {
        if (child->GetClassId() == ZONE) {
            pugi::xml_node childNode = currentNode.append_child("zone");
            WriteZone(childNode, dynamic_cast<Zone *>(child));
        }
        else {
            LogWarning("Unable to write child '%s' of surface", child->GetClassName().c_str());
        }
    }
}

void MeiOutput::WriteZone(pugi::xml_node currentNode, Zone *zone)
{
    assert(zone);
    WriteXmlId(currentNode, zone);
    zone->WriteCoordinated(currentNode);
    zone->WriteTyped(currentNode);
}

void MeiOutput::WriteTextElement(pugi::xml_node currentNode, TextElement *textElement)
{
    assert(textElement);

    WriteXmlId(currentNode, textElement);
    textElement->WriteLabelled(currentNode);
    textElement->WriteTyped(currentNode);
}

void MeiOutput::WriteF(pugi::xml_node currentNode, F *f)
{
    assert(f);

    WriteTextElement(currentNode, f);
    WriteTimeSpanningInterface(currentNode, f);
}

void MeiOutput::WriteFig(pugi::xml_node currentNode, Fig *fig)
{
    assert(fig);

    WriteTextElement(currentNode, fig);
    WriteAreaPosInterface(currentNode, fig);
}

void MeiOutput::WriteLb(pugi::xml_node currentNode, Lb *lb)
{
    assert(lb);

    WriteTextElement(currentNode, lb);
}

void MeiOutput::WriteNum(pugi::xml_node currentNode, Num *num)
{
    assert(num);

    WriteTextElement(currentNode, num);
}

void MeiOutput::WriteRend(pugi::xml_node currentNode, Rend *rend)
{
    assert(rend);

    WriteTextElement(currentNode, rend);
    WriteAreaPosInterface(currentNode, rend);
    rend->WriteColor(currentNode);
    rend->WriteLang(currentNode);
    rend->WriteTextRendition(currentNode);
    rend->WriteTypography(currentNode);
    rend->WriteWhitespace(currentNode);
}

void MeiOutput::WriteSvg(pugi::xml_node currentNode, Svg *svg)
{
    assert(svg);

    WriteXmlId(currentNode, svg);

    pugi::xml_node svgNode = svg->Get();
    for (pugi::xml_attribute attr : svgNode.attributes()) {
        currentNode.append_attribute(attr.name()) = attr.value();
    }

    for (pugi::xml_node child : svgNode.children()) {
        currentNode.append_copy(child);
    }
}

void MeiOutput::WriteText(pugi::xml_node element, Text *text)
{
    if (!text->GetText().empty()) {
        pugi::xml_node nodechild = element.append_child(pugi::node_pcdata);
        if (m_doc->GetOptions()->m_outputSmuflXmlEntities.GetValue()) {
            nodechild.text() = UTF16to8(EscapeSMuFL(text->GetText()).c_str()).c_str();
        }
        else {
            nodechild.text() = UTF16to8(text->GetText()).c_str();
        }
    }
}

void MeiOutput::WriteAreaPosInterface(pugi::xml_node element, AreaPosInterface *interface)
{
    assert(interface);

    interface->WriteHorizontalAlign(element);
    interface->WriteVerticalAlign(element);
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

void MeiOutput::WriteLinkingInterface(pugi::xml_node element, LinkingInterface *interface)
{
    assert(interface);

    interface->WriteLinking(element);
}

void MeiOutput::WriteFacsimileInterface(pugi::xml_node element, FacsimileInterface *interface)
{
    assert(interface);

    interface->WriteFacsimile(element);
}

void MeiOutput::WritePitchInterface(pugi::xml_node element, PitchInterface *interface)
{
    assert(interface);

    interface->WriteNoteGes(element);
    interface->WriteOctave(element);
    interface->WritePitch(element);
}

void MeiOutput::WritePlistInterface(pugi::xml_node element, PlistInterface *interface)
{
    assert(interface);

    interface->WritePlist(element);
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

    interface->WriteLyricStyle(element);
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
    for (auto &pair : object->m_unsupported) {
        if (element.attribute(pair.first.c_str())) {
            LogDebug("Attribute '%s' for '%s' is supported", pair.first.c_str(), object->GetClassName().c_str());
        }
        else {
            element.append_attribute(pair.first.c_str()) = pair.second.c_str();
        }
    }
}

void MeiOutput::WriteEditorialElement(pugi::xml_node currentNode, EditorialElement *element)
{
    assert(element);

    WriteXmlId(currentNode, element);
    element->WriteLabelled(currentNode);
    element->WriteTyped(currentNode);
}

void MeiOutput::WriteAbbr(pugi::xml_node currentNode, Abbr *abbr)
{
    assert(abbr);

    WriteEditorialElement(currentNode, abbr);
    abbr->WriteSource(currentNode);
}

void MeiOutput::WriteAdd(pugi::xml_node currentNode, Add *add)
{
    assert(add);

    WriteEditorialElement(currentNode, add);
    add->WriteSource(currentNode);
}

void MeiOutput::WriteAnnot(pugi::xml_node currentNode, Annot *annot)
{
    assert(annot);

    WriteEditorialElement(currentNode, annot);
    annot->WritePlist(currentNode);
    annot->WriteSource(currentNode);
    // special case where we keep the pugi::nodes
    for (pugi::xml_node child = annot->m_content.first_child(); child; child = child.next_sibling()) {
        currentNode.append_copy(child);
    }
}

void MeiOutput::WriteApp(pugi::xml_node currentNode, App *app)
{
    assert(app);

    WriteEditorialElement(currentNode, app);
}

void MeiOutput::WriteChoice(pugi::xml_node currentNode, Choice *choice)
{
    assert(choice);

    WriteEditorialElement(currentNode, choice);
}

void MeiOutput::WriteCorr(pugi::xml_node currentNode, Corr *corr)
{
    assert(corr);

    WriteEditorialElement(currentNode, corr);
    corr->WriteSource(currentNode);
}

void MeiOutput::WriteDamage(pugi::xml_node currentNode, Damage *damage)
{
    assert(damage);

    WriteEditorialElement(currentNode, damage);
    damage->WriteSource(currentNode);
}

void MeiOutput::WriteDel(pugi::xml_node currentNode, Del *del)
{
    assert(del);

    WriteEditorialElement(currentNode, del);
    del->WriteSource(currentNode);
}

void MeiOutput::WriteExpan(pugi::xml_node currentNode, Expan *expan)
{
    assert(expan);

    WriteEditorialElement(currentNode, expan);
    expan->WriteSource(currentNode);
}

void MeiOutput::WriteLem(pugi::xml_node currentNode, Lem *lem)
{
    assert(lem);

    WriteEditorialElement(currentNode, lem);
    lem->WriteSource(currentNode);
}

void MeiOutput::WriteOrig(pugi::xml_node currentNode, Orig *orig)
{
    assert(orig);

    WriteEditorialElement(currentNode, orig);
    orig->WriteSource(currentNode);
}

void MeiOutput::WriteRdg(pugi::xml_node currentNode, Rdg *rdg)
{
    assert(rdg);

    WriteEditorialElement(currentNode, rdg);
    rdg->WriteSource(currentNode);
}

void MeiOutput::WriteRef(pugi::xml_node currentNode, Ref *ref)
{
    assert(ref);

    WriteEditorialElement(currentNode, ref);
}

void MeiOutput::WriteReg(pugi::xml_node currentNode, Reg *reg)
{
    assert(reg);

    WriteEditorialElement(currentNode, reg);
    reg->WriteSource(currentNode);
}

void MeiOutput::WriteRestore(pugi::xml_node currentNode, Restore *restore)
{
    assert(restore);

    WriteEditorialElement(currentNode, restore);
    restore->WriteSource(currentNode);

    return;
}

void MeiOutput::WriteSic(pugi::xml_node currentNode, Sic *sic)
{
    assert(sic);

    WriteEditorialElement(currentNode, sic);
    sic->WriteSource(currentNode);
}

void MeiOutput::WriteSubst(pugi::xml_node currentNode, Subst *subst)
{
    assert(subst);

    WriteEditorialElement(currentNode, subst);
}

void MeiOutput::WriteSupplied(pugi::xml_node currentNode, Supplied *supplied)
{
    assert(supplied);

    WriteEditorialElement(currentNode, supplied);
    supplied->WriteSource(currentNode);
}

void MeiOutput::WriteUnclear(pugi::xml_node currentNode, Unclear *unclear)
{
    assert(unclear);

    WriteEditorialElement(currentNode, unclear);
    unclear->WriteSource(currentNode);
}

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
        case Facs: value = "facsimile"; break;
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
    m_useScoreDefForDoc = false;
    m_readingScoreBased = false;
    m_version = MEI_UNDEFINED;
}

MeiInput::~MeiInput() {}

bool MeiInput::ImportFile()
{
    try {
        m_doc->Reset();
        m_doc->SetType(Raw);
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(m_filename.c_str(), pugi::parse_default & ~pugi::parse_eol);
        if (!result) {
            return false;
        }
        pugi::xml_node root = doc.first_child();
        return ReadDoc(root);
    }
    catch (char *str) {
        LogError("%s", str);
        return false;
    }
}

bool MeiInput::ImportString(const std::string &mei)
{
    try {
        m_doc->Reset();
        m_doc->SetType(Raw);
        pugi::xml_document doc;
        doc.load_string(mei.c_str(), pugi::parse_default & ~pugi::parse_eol);
        pugi::xml_node root = doc.first_child();
        return ReadDoc(root);
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
        // Because of the Clone issue on annot do not support it in label and labelAbbr
        if (filterParent->Is(LABEL) && (element == "annot")) {
            return false;
        }
        else if (filterParent->Is(LABELABBR) && (element == "annot")) {
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
    // filter for dynam
    else if (filterParent->Is(DYNAM)) {
        if (element == "") {
            return true;
        }
        else if (element == "lb") {
            return true;
        }
        else if (element == "rend") {
            return true;
        }
        else {
            return false;
        }
    }
    // filter for dir
    else if (filterParent->Is(DIR)) {
        if (element == "") {
            return true;
        }
        else if (element == "lb") {
            return true;
        }
        else if (element == "rend") {
            return true;
        }
        else {
            return false;
        }
    }
    else if (filterParent->Is(FIG)) {
        if (element == "svg") {
            return true;
        }
        else {
            return false;
        }
    }
    else if (filterParent->Is(FIGURE)) {
        if (element == "") {
            return true;
        }
        else {
            return false;
        }
    }
    else if (filterParent->Is(NUM)) {
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
    // filter for rend
    else if (filterParent->Is(REND)) {
        if (element == "") {
            return true;
        }
        else if (element == "lb") {
            return true;
        }
        else if (element == "num") {
            return true;
        }
        else if (element == "rend") {
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
    // filter for any other control element
    else if (filterParent->IsRunningElement()) {
        if (element == "fig") {
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
        else if (element == "bTrem") {
            return true;
        }
        else if (element == "chord") {
            return true;
        }
        else if (element == "clef") {
            return true;
        }
        else if (element == "graceGrp") {
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
    // filter for graceGrp
    else if (filterParent->Is(GRACEGRP)) {
        if (element == "beam") {
            return true;
        }
        else if (element == "chord") {
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
        else {
            return false;
        }
    }
    // filter for keySig
    else if (filterParent->Is(KEYSIG)) {
        if (element == "keyAccid") {
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
        else if (element == "lb") {
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
        else if (element == "lb") {
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
    // filter for rest
    else if (filterParent->Is(REST)) {
        return false;
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
    else {
        LogDebug("Unknown filter for '%s'", filterParent->GetClassName().c_str());
        return true;
    }
}

bool MeiInput::ReadDoc(pugi::xml_node root)
{
    pugi::xml_node current;
    bool success = true;
    m_readingScoreBased = false;

    if (root.empty()) {
        LogError("The tree of the MEI data cannot be parsed (no root found)");
        return false;
    }

    current = root.child("meiHead");
    if (current.empty()) {
        LogWarning("No header found in the MEI data, trying to proceed...");
    }
    else {
        m_doc->m_header.reset();
        // copy the complete header into the master document
        m_doc->m_header.append_copy(current);
        if (root.attribute("meiversion")) {
            std::string version = std::string(root.attribute("meiversion").value());
            if (version == "4.0.1")
                m_version = MEI_4_0_1;
            else if (version == "4.0.0")
                m_version = MEI_4_0_0;
            else if (version == "3.0.0")
                m_version = MEI_3_0_0;
            else if (version == "2013")
                m_version = MEI_2013;
        }
    }

    // music
    pugi::xml_node music;
    pugi::xml_node front;
    pugi::xml_node body;
    pugi::xml_node facsimile;
    pugi::xml_node pages;
    pugi::xml_node back;

    if (std::string(root.name()) == "music") {
        music = root;
    }
    else {
        music = root.child("music");
    }
    if (music.empty()) {
        LogError("No <music> element found in the MEI data");
        return false;
    }

    facsimile = music.child("facsimile");
    if ((!facsimile.empty()) && (m_doc->GetOptions()->m_useFacsimile.GetValue())) {
        ReadFacsimile(m_doc, facsimile);
        m_doc->SetType(Facs);
        m_doc->m_drawingPageHeight = m_doc->GetFacsimile()->GetMaxY();
        m_doc->m_drawingPageWidth = m_doc->GetFacsimile()->GetMaxX();
    }

    front = music.child("front");
    if (!front.empty()) {
        m_doc->m_front.reset();
        // copy the complete front into the master document
        m_doc->m_front.append_copy(front);
    }

    back = music.child("back");
    if (!back.empty()) {
        m_doc->m_back.reset();
        // copy the complete front into the master document
        m_doc->m_back.append_copy(back);
    }

    body = music.child("body");
    if (body.empty()) {
        LogError("No <body> element found in the MEI data");
        return false;
    }

    // Select the first mdiv by default
    m_selectedMdiv = body.child("mdiv");
    if (m_selectedMdiv.empty()) {
        LogError("No <mdiv> element found in the MEI data");
        return false;
    }

    std::string xPathQuery = m_doc->GetOptions()->m_mdivXPathQuery.GetValue();
    if (!xPathQuery.empty()) {
        pugi::xpath_node selection = body.select_node(xPathQuery.c_str());
        if (selection) {
            m_selectedMdiv = selection.node();
        }
        else {
            LogError("The <mdiv> requested with the xpath query '%s' could not be found", xPathQuery.c_str());
            return false;
        }
    }
    else {
        // Try to select the mdiv above the first score (if any) - if not, we have pages or something is wrong
        pugi::xpath_node scoreMdiv = body.select_node(".//mdiv[count(score)>0]");
        if (scoreMdiv) {
            m_selectedMdiv = scoreMdiv.node();
        }
    }

    if (m_selectedMdiv.select_nodes(".//score").size() > 1) {
        LogError("An <mdiv> with only one <score> descendant must be selected");
        return false;
    }

    if (m_selectedMdiv.select_nodes(".//pages").size() > 1) {
        LogError("An <mdiv> with only one <pages> descendant must be selected");
        return false;
    }

    if ((m_selectedMdiv.select_nodes(".//score").size() > 0) && (m_selectedMdiv.select_nodes(".//pages").size() > 0)) {
        LogError("An <mdiv> with only one <pages> or one <score> descendant must be selected");
        return false;
    }

    success = ReadMdivChildren(m_doc, body, false);

    if (success) {
        m_doc->ConvertScoreDefMarkupDoc();
    }

    if (success && m_readingScoreBased) {
        m_doc->ConvertToPageBasedDoc();
        m_doc->ConvertAnalyticalMarkupDoc();
    }

    if (success && !m_hasScoreDef) {
        LogWarning("No scoreDef provided, trying to generate one...");
        success = m_doc->GenerateDocumentScoreDef();
    }

    if (success && m_doc->GetType() == Facs) {
        Functor setChildZones(&Object::SetChildZones);
        SetChildZonesParams setChildZonesParams(m_doc);
        m_doc->Process(&setChildZones, &setChildZonesParams);
    }

    return success;
}

bool MeiInput::ReadMdiv(Object *parent, pugi::xml_node mdiv, bool isVisible)
{
    Mdiv *vrvMdiv = new Mdiv();
    SetMeiUuid(mdiv, vrvMdiv);

    vrvMdiv->ReadLabelled(mdiv);
    vrvMdiv->ReadNNumberLike(mdiv);

    parent->AddChild(vrvMdiv);

    if (isVisible) {
        vrvMdiv->MakeVisible();
    }

    ReadUnsupportedAttr(mdiv, vrvMdiv);
    return ReadMdivChildren(vrvMdiv, mdiv, isVisible);
}

bool MeiInput::ReadMdivChildren(Object *parent, pugi::xml_node parentNode, bool isVisible)
{
    assert(dynamic_cast<Doc *>(parent) || dynamic_cast<Mdiv *>(parent));

    pugi::xml_node current;
    bool success = true;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        // We make the mdiv visible if already set or if matching the desired selection
        bool makeVisible = (isVisible || (m_selectedMdiv == current));
        m_useScoreDefForDoc = makeVisible;
        if (!success) break;
        if (std::string(current.name()) == "mdiv") {
            success = ReadMdiv(parent, current, makeVisible);
        }
        else if (std::string(current.name()) == "pages") {
            success = ReadPages(parent, current);
            if (parentNode.last_child() != current) {
                LogWarning("Skipping nodes after <pages> element");
            }
            break;
        }
        else if (std::string(current.name()) == "score") {
            success = ReadScore(parent, current);
            if (parentNode.last_child() != current) {
                LogWarning("Skipping nodes after <score> element");
            }
            break;
        }
        else {
            LogWarning("Unsupported '<%s>' within <mdiv>", current.name());
        }
    }

    return success;
}

bool MeiInput::ReadPages(Object *parent, pugi::xml_node pages)
{
    Pages *vrvPages = new Pages();
    SetMeiUuid(pages, vrvPages);

    vrvPages->ReadLabelled(pages);
    vrvPages->ReadNNumberLike(pages);

    parent->AddChild(vrvPages);

    // check if there is a type attribute for the score
    DocType type;
    if (pages.attribute("type")) {
        type = StrToDocType(pages.attribute("type").value());
        m_doc->SetType(type);
        pages.remove_attribute("type");
    }

    // This is a page-based MEI file
    this->m_hasLayoutInformation = true;

    bool success = true;
    // We require to have s <scoreDef> as first child of <score>
    pugi::xml_node scoreDef = pages.first_child();
    if (!scoreDef || (std::string(scoreDef.name()) != "scoreDef")) {
        LogWarning("No <scoreDef> provided, trying to proceed... ");
    }
    else {
        // This actually sets the Doc::m_scoreDef
        success = ReadScoreDef(vrvPages, scoreDef);
    }

    if (!success) return false;

    // No need to have ReadPagesChildren for this...
    pugi::xml_node current;
    for (current = pages.first_child(); current; current = current.next_sibling()) {
        if (!success) break;
        // page
        if (std::string(current.name()) == "page") {
            success = ReadPage(vrvPages, current);
        }
        else if (std::string(current.name()) == "scoreDef") {
            // Skipping scoreDefs, only the first one is possible
            continue;
        }
        else {
            LogWarning("Unsupported '<%s>' within <pages>", current.name());
        }
    }

    ReadUnsupportedAttr(pages, vrvPages);
    return success;
}

bool MeiInput::ReadScore(Object *parent, pugi::xml_node score)
{
    Score *vrvScore = new Score();
    SetMeiUuid(score, vrvScore);

    vrvScore->ReadLabelled(score);
    vrvScore->ReadNNumberLike(score);

    parent->AddChild(vrvScore);

    // This is a score-based MEI file
    this->m_readingScoreBased = true;

    // We require to have s <scoreDef> as first child of <score>
    pugi::xml_node scoreDef = score.first_child();
    if (!scoreDef || (std::string(scoreDef.name()) != "scoreDef")) {
        LogError("A <scoreDef> is required as first child of <score>");
        return false;
    }

    // This actually sets the Doc::m_scoreDef
    bool success = ReadScoreDef(vrvScore, scoreDef);

    if (!success) return false;

    pugi::xml_node current;
    for (current = scoreDef.next_sibling(); current; current = current.next_sibling()) {
        if (!success) break;
        std::string elementName = std::string(current.name());
        // editorial
        if (IsEditorialElementName(current.name())) {
            success = ReadEditorialElement(vrvScore, current, EDITORIAL_TOPLEVEL);
        }
        // content
        else if (elementName == "ending") {
            success = ReadEnding(vrvScore, current);
        }
        else if (elementName == "section") {
            success = ReadSection(vrvScore, current);
        }
        else {
            LogWarning("Element <%s> within <score> is not supported and will be ignored ", elementName.c_str());
        }
    }

    ReadUnsupportedAttr(score, vrvScore);
    return success;
}

bool MeiInput::ReadSection(Object *parent, pugi::xml_node section)
{
    Section *vrvSection = new Section();
    SetMeiUuid(section, vrvSection);

    vrvSection->ReadNNumberLike(section);

    parent->AddChild(vrvSection);
    ReadUnsupportedAttr(section, vrvSection);
    if (m_readingScoreBased)
        return ReadSectionChildren(vrvSection, section);
    else
        return ReadSystemChildren(vrvSection, section);
}

bool MeiInput::ReadSectionChildren(Object *parent, pugi::xml_node parentNode)
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
            success = ReadEditorialElement(parent, current, EDITORIAL_TOPLEVEL);
        }
        // content
        else if (std::string(current.name()) == "ending") {
            // we should not endings with unmeasured music ... (?)
            assert(!unmeasured);
            success = ReadEnding(parent, current);
        }
        else if (std::string(current.name()) == "expansion") {
            success = ReadExpansion(parent, current);
        }
        else if (std::string(current.name()) == "scoreDef") {
            success = ReadScoreDef(parent, current);
        }
        else if (std::string(current.name()) == "section") {
            success = ReadSection(parent, current);
        }
        // pb and sb
        else if (std::string(current.name()) == "pb") {
            success = ReadPb(parent, current);
        }
        else if (std::string(current.name()) == "sb") {
            success = ReadSb(parent, current);
        }
        // unmeasured music
        else if (std::string(current.name()) == "staff") {
            if (!unmeasured) {
                if (parent->Is(SECTION)) {
                    unmeasured = new Measure(false);
                    m_doc->SetMensuralMusicOnly(true);
                    parent->AddChild(unmeasured);
                }
                else {
                    LogError("Unmeasured music within editorial markup is currently not supported");
                    return false;
                }
            }
            success = ReadStaff(unmeasured, current);
        }
        else if (std::string(current.name()) == "measure") {
            // we should not mix measured and unmeasured music within a system...
            assert(!unmeasured);
            // if (parent->IsEditorialElement()) {
            //    m_hasMeasureWithinEditMarkup = true;
            //}
            success = ReadMeasure(parent, current);
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

bool MeiInput::ReadEnding(Object *parent, pugi::xml_node ending)
{
    Ending *vrvEnding = new Ending();
    ReadSystemElement(ending, vrvEnding);

    vrvEnding->ReadLineRend(ending);
    vrvEnding->ReadNNumberLike(ending);

    parent->AddChild(vrvEnding);
    ReadUnsupportedAttr(ending, vrvEnding);
    if (m_readingScoreBased)
        return ReadSectionChildren(vrvEnding, ending);
    else
        return true;
}

bool MeiInput::ReadExpansion(Object *parent, pugi::xml_node expansion)
{
    Expansion *vrvExpansion = new Expansion();
    ReadSystemElement(expansion, vrvExpansion);

    parent->AddChild(vrvExpansion);
    ReadUnsupportedAttr(expansion, vrvExpansion);
    if (m_readingScoreBased)
        return ReadSectionChildren(vrvExpansion, expansion);
    else
        return true;
}

bool MeiInput::ReadPb(Object *parent, pugi::xml_node pb)
{
    this->m_hasLayoutInformation = true;

    Pb *vrvPb = new Pb();
    ReadSystemElement(pb, vrvPb);

    vrvPb->ReadNNumberLike(pb);

    parent->AddChild(vrvPb);
    ReadUnsupportedAttr(pb, vrvPb);
    return true;
}

bool MeiInput::ReadSb(Object *parent, pugi::xml_node sb)
{
    Sb *vrvSb = new Sb();
    ReadSystemElement(sb, vrvSb);

    vrvSb->ReadNNumberLike(sb);

    parent->AddChild(vrvSb);
    ReadUnsupportedAttr(sb, vrvSb);
    return true;
}

bool MeiInput::ReadPage(Object *parent, pugi::xml_node page)
{
    Page *vrvPage = new Page();
    SetMeiUuid(page, vrvPage);

    if ((m_doc->GetType() == Transcription) && (m_version == MEI_2013)) {
        UpgradePageTo_3_0_0(vrvPage, m_doc);
    }

    if (page.attribute("page.height")) {
        vrvPage->m_pageHeight = atoi(page.attribute("page.height").value()) * DEFINITION_FACTOR;
        page.remove_attribute("page.height");
    }
    if (page.attribute("page.width")) {
        vrvPage->m_pageWidth = atoi(page.attribute("page.width").value()) * DEFINITION_FACTOR;
        page.remove_attribute("page.width");
    }
    if (page.attribute("page.botmar")) {
        vrvPage->m_pageMarginBottom = atoi(page.attribute("page.botmar").value()) * DEFINITION_FACTOR;
        page.remove_attribute("page.botmar");
    }
    if (page.attribute("page.leftmar")) {
        vrvPage->m_pageMarginLeft = atoi(page.attribute("page.leftmar").value()) * DEFINITION_FACTOR;
        page.remove_attribute("page.leftmar");
    }
    if (page.attribute("page.rightmar")) {
        vrvPage->m_pageMarginRight = atoi(page.attribute("page.rightmar").value()) * DEFINITION_FACTOR;
        page.remove_attribute("page.rightmar");
    }
    if (page.attribute("page.topmar")) {
        vrvPage->m_pageMarginTop = atoi(page.attribute("page.topmar").value()) * DEFINITION_FACTOR;
        page.remove_attribute("page.topmar");
    }
    if (page.attribute("surface")) {
        vrvPage->m_surface = page.attribute("surface").value();
        page.remove_attribute("surface");
    }
    if (page.attribute("ppu")) {
        vrvPage->m_PPUFactor = atof(page.attribute("ppu").value());
    }

    parent->AddChild(vrvPage);
    bool success = ReadPageChildren(vrvPage, page);

    if (success && (m_doc->GetType() == Transcription) && (vrvPage->GetPPUFactor() != 1.0)) {
        ApplyPPUFactorParams applyPPUFactorParams;
        Functor applyPPUFactor(&Object::ApplyPPUFactor);
        vrvPage->Process(&applyPPUFactor, &applyPPUFactorParams);
    }

    ReadUnsupportedAttr(page, vrvPage);
    return success;
}

bool MeiInput::ReadPageChildren(Object *parent, pugi::xml_node parentNode)
{
    // If we allow <app> between <page> elements
    // assert(dynamic_cast<Page*>(parent) || dynamic_cast<EditorialElement*>(parent));
    assert(dynamic_cast<Page *>(parent));

    pugi::xml_node current;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        if (std::string(current.name()) == "system") {
            ReadSystem(parent, current);
        }
        // can we have scoreDef between system in the current page-based cusotmization? To be checked
        else if (std::string(current.name()) == "scoreDef") {
            ReadScoreDef(parent, current);
        }
        // can we have scoreDef between system in the current page-based cusotmization?
        // To be checked or defined - we would need to add an EDITORIAL_PAGE EditorialLevel
        /*
         else if (std::string(current.name()) == "app") {
         ReadApp(vrvPage, current, EDITORIAL_PAGE);
         }
         */
        else {
            LogWarning("Unsupported '<%s>' within <page>", current.name());
        }
    }

    return true;
}

bool MeiInput::ReadSystem(Object *parent, pugi::xml_node system)
{
    // If we allow <app> between <page> elements
    // assert(dynamic_cast<Page*>(parent) || dynamic_cast<EditorialElement*>(parent));
    assert(dynamic_cast<Page *>(parent));

    System *vrvSystem = new System();
    SetMeiUuid(system, vrvSystem);
    vrvSystem->ReadTyped(system);

    if (system.attribute("system.leftmar")) {
        vrvSystem->m_systemLeftMar = atoi(system.attribute("system.leftmar").value());
        system.remove_attribute("system.leftmar");
    }
    if (system.attribute("system.rightmar")) {
        vrvSystem->m_systemRightMar = atoi(system.attribute("system.rightmar").value());
        system.remove_attribute("system.rightmar");
    }
    if (system.attribute("uly") && (this->m_doc->GetType() == Transcription)) {
        vrvSystem->m_yAbs = atoi(system.attribute("uly").value()) * DEFINITION_FACTOR;
        system.remove_attribute("uly");
    }

    parent->AddChild(vrvSystem);
    ReadUnsupportedAttr(system, vrvSystem);
    return ReadSystemChildren(vrvSystem, system);
}

bool MeiInput::ReadSystemChildren(Object *parent, pugi::xml_node parentNode)
{
    assert(dynamic_cast<System *>(parent) || dynamic_cast<EditorialElement *>(parent));

    bool success = true;
    pugi::xml_node current;
    Measure *unmeasured = NULL;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        if (!success) break;
        // editorial
        else if (IsEditorialElementName(current.name())) {
            success = ReadEditorialElement(parent, current, EDITORIAL_TOPLEVEL);
        }
        // boundaryEnd
        else if (std::string(current.name()) == "boundaryEnd") {
            success = ReadBoundaryEnd(parent, current);
        }
        // content
        else if (std::string(current.name()) == "scoreDef") {
            // we should not have scoredef with unmeasured music within a system... (?)
            assert(!unmeasured);
            ReadScoreDef(parent, current);
        }
        // unmeasured music
        else if (std::string(current.name()) == "staff") {
            if (!unmeasured) {
                if (parent->Is(SYSTEM)) {
                    System *system = dynamic_cast<System *>(parent);
                    assert(system);
                    unmeasured = new Measure(false);
                    m_doc->SetMensuralMusicOnly(true);
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
            success = ReadStaff(unmeasured, current);
        }
        else if (std::string(current.name()) == "measure") {
            // we should not mix measured and unmeasured music within a system...
            assert(!unmeasured);
            success = ReadMeasure(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <system>", current.name());
        }
    }
    return success;
}

bool MeiInput::ReadBoundaryEnd(Object *parent, pugi::xml_node boundaryEnd)
{
    assert(dynamic_cast<System *>(parent));

    std::string startUuid;
    Object *start = NULL;
    if (boundaryEnd.attribute("startid")) {
        std::string startUuid = boundaryEnd.attribute("startid").value();
        start = m_doc->FindDescendantByUuid(startUuid);
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
    object->ReadMeasureNumbers(element);
    object->ReadTyped(element);

    AttCleffingLog cleffingLog;
    cleffingLog.ReadCleffingLog(element);
    AttCleffingVis cleffingVis;
    cleffingVis.ReadCleffingVis(element);
    if (cleffingLog.HasClefLine() && cleffingLog.HasClefShape()) {
        Clef *vrvClef = new Clef();
        vrvClef->IsAttribute(true);
        vrvClef->SetShape(cleffingLog.GetClefShape());
        vrvClef->SetLine(cleffingLog.GetClefLine());
        vrvClef->SetDis(cleffingLog.GetClefDis());
        vrvClef->SetDisPlace(cleffingLog.GetClefDisPlace());
        vrvClef->SetColor(cleffingVis.GetClefColor());
        object->AddChild(vrvClef);
    }

    AttKeySigDefaultAnl keySigDefaultAnl;
    keySigDefaultAnl.ReadKeySigDefaultAnl(element);
    AttKeySigDefaultLog keySigDefaultLog;
    keySigDefaultLog.ReadKeySigDefaultLog(element);
    AttKeySigDefaultVis keySigDefaultVis;
    keySigDefaultVis.ReadKeySigDefaultVis(element);
    if (keySigDefaultAnl.HasKeyAccid() || keySigDefaultAnl.HasKeyMode() || keySigDefaultAnl.HasKeyPname()
        || keySigDefaultLog.HasKeySig() || keySigDefaultVis.HasKeysigShow() || keySigDefaultVis.HasKeysigShowchange()) {
        KeySig *vrvKeySig = new KeySig();
        vrvKeySig->IsAttribute(true);
        // Broken in MEI 4.0.2 - waiting for a fix
        // vrvKeySig->SetAccid(keySigDefaultAnl.GetKeyAccid());
        vrvKeySig->SetMode(keySigDefaultAnl.GetKeyMode());
        vrvKeySig->SetPname(keySigDefaultAnl.GetKeyPname());
        vrvKeySig->SetSig(keySigDefaultLog.GetKeySig());
        vrvKeySig->SetVisible(keySigDefaultVis.GetKeysigShow());
        vrvKeySig->SetSigShowchange(keySigDefaultVis.GetKeysigShowchange());
        object->AddChild(vrvKeySig);
    }

    AttMensuralLog mensuralLog;
    mensuralLog.ReadMensuralLog(element);
    AttMensuralShared mensuralShared;
    mensuralShared.ReadMensuralShared(element);
    AttMensuralVis mensuralVis;
    mensuralVis.ReadMensuralVis(element);
    if (mensuralShared.HasProlatio() || mensuralShared.HasTempus() || mensuralLog.HasProportNum()
        || mensuralLog.HasProportNumbase() || mensuralLog.HasMensurSign()) {
        Mensur *vrvMensur = new Mensur();
        vrvMensur->IsAttribute(true);
        //
        vrvMensur->SetDot(mensuralLog.GetMensurDot());
        vrvMensur->SetNum(mensuralLog.GetProportNum());
        vrvMensur->SetNumbase(mensuralLog.GetProportNumbase());
        vrvMensur->SetSign(mensuralLog.GetMensurSign());
        vrvMensur->SetSlash(mensuralLog.GetMensurSlash());
        //
        vrvMensur->SetModusmaior(mensuralShared.GetModusmaior());
        vrvMensur->SetModusminor(mensuralShared.GetModusminor());
        vrvMensur->SetProlatio(mensuralShared.GetProlatio());
        vrvMensur->SetTempus(mensuralShared.GetTempus());
        //
        vrvMensur->SetColor(mensuralVis.GetMensurColor());
        vrvMensur->SetOrient(mensuralVis.GetMensurOrient());
        object->AddChild(vrvMensur);
    }

    AttMeterSigDefaultLog meterSigDefaultLog;
    meterSigDefaultLog.ReadMeterSigDefaultLog(element);
    AttMeterSigDefaultVis meterSigDefaultVis;
    meterSigDefaultVis.ReadMeterSigDefaultVis(element);
    if (meterSigDefaultLog.HasMeterCount() || meterSigDefaultLog.HasMeterSym() || meterSigDefaultLog.HasMeterUnit()) {
        MeterSig *vrvMeterSig = new MeterSig();
        vrvMeterSig->IsAttribute(true);
        vrvMeterSig->SetCount(meterSigDefaultLog.GetMeterCount());
        vrvMeterSig->SetSym(meterSigDefaultLog.GetMeterSym());
        vrvMeterSig->SetUnit(meterSigDefaultLog.GetMeterUnit());
        vrvMeterSig->SetForm(meterSigDefaultVis.GetMeterForm());
        object->AddChild(vrvMeterSig);
    }

    return true;
}

bool MeiInput::ReadScoreDef(Object *parent, pugi::xml_node scoreDef)
{
    assert(dynamic_cast<Pages *>(parent) || dynamic_cast<Score *>(parent) || dynamic_cast<Section *>(parent)
        || dynamic_cast<System *>(parent) || dynamic_cast<Ending *>(parent)
        || dynamic_cast<EditorialElement *>(parent));
    // assert(dynamic_cast<Pages *>(parent));

    ScoreDef *vrvScoreDef;
    // We have not reached the first scoreDef and we have to use if for the doc
    if (!m_hasScoreDef && m_useScoreDefForDoc) {
        vrvScoreDef = &m_doc->m_scoreDef;
    }
    else {
        vrvScoreDef = new ScoreDef();
    }
    ReadScoreDefElement(scoreDef, vrvScoreDef);

    if (m_version < MEI_4_0_0) {
        UpgradeScoreDefElementTo_4_0_0(scoreDef, vrvScoreDef);
    }

    ReadScoreDefInterface(scoreDef, vrvScoreDef);
    vrvScoreDef->ReadEndings(scoreDef);
    vrvScoreDef->ReadOptimization(scoreDef);

    if (!m_hasScoreDef && m_useScoreDefForDoc) {
        m_hasScoreDef = true;
    }
    else {
        parent->AddChild(vrvScoreDef);
    }
    ReadUnsupportedAttr(scoreDef, vrvScoreDef);
    return ReadScoreDefChildren(vrvScoreDef, scoreDef);
}

bool MeiInput::ReadScoreDefChildren(Object *parent, pugi::xml_node parentNode)
{
    assert(dynamic_cast<ScoreDef *>(parent) || dynamic_cast<EditorialElement *>(parent));

    bool success = true;
    pugi::xml_node current;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        if (!success) break;
        // editorial
        else if (IsEditorialElementName(current.name())) {
            success = ReadEditorialElement(parent, current, EDITORIAL_SCOREDEF);
        }
        // clef, keySig, etc.
        else if (std::string(current.name()) == "clef") {
            success = ReadClef(parent, current);
        }
        else if (std::string(current.name()) == "keySig") {
            success = ReadKeySig(parent, current);
        }
        else if (std::string(current.name()) == "mensur") {
            success = ReadMensur(parent, current);
        }
        else if (std::string(current.name()) == "meterSig") {
            success = ReadMeterSig(parent, current);
        }
        // headers and footers
        else if (std::string(current.name()) == "pgFoot") {
            success = ReadPgFoot(parent, current);
        }
        else if (std::string(current.name()) == "pgFoot2") {
            success = ReadPgFoot2(parent, current);
        }
        else if (std::string(current.name()) == "pgHead") {
            success = ReadPgHead(parent, current);
        }
        else if (std::string(current.name()) == "pgHead2") {
            success = ReadPgHead2(parent, current);
        }
        // content
        else if (std::string(current.name()) == "staffGrp") {
            success = ReadStaffGrp(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <scoreDef>", current.name());
        }
    }
    return success;
}

bool MeiInput::ReadStaffGrp(Object *parent, pugi::xml_node staffGrp)
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
    ReadUnsupportedAttr(staffGrp, vrvStaffGrp);
    return ReadStaffGrpChildren(vrvStaffGrp, staffGrp);
}

bool MeiInput::ReadStaffGrpChildren(Object *parent, pugi::xml_node parentNode)
{
    assert(dynamic_cast<StaffGrp *>(parent) || dynamic_cast<EditorialElement *>(parent));

    bool success = true;
    pugi::xml_node current;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        if (!success) break;
        // editorial
        else if (IsEditorialElementName(current.name())) {
            success = ReadEditorialElement(parent, current, EDITORIAL_STAFFGRP);
        }
        // content
        else if (std::string(current.name()) == "instrDef") {
            success = ReadInstrDef(parent, current);
        }
        else if (std::string(current.name()) == "label") {
            success = ReadLabel(parent, current);
        }
        else if (std::string(current.name()) == "labelAbbr") {
            success = ReadLabelAbbr(parent, current);
        }
        else if (std::string(current.name()) == "staffGrp") {
            success = ReadStaffGrp(parent, current);
        }
        else if (std::string(current.name()) == "staffDef") {
            success = ReadStaffDef(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <staffGrp>", current.name());
        }
    }
    return success;
}

bool MeiInput::ReadRunningElement(pugi::xml_node element, RunningElement *object)
{
    SetMeiUuid(element, object);
    object->ReadHorizontalAlign(element);
    object->ReadTyped(element);

    return true;
}

bool MeiInput::ReadPgFoot(Object *parent, pugi::xml_node pgFoot)
{
    PgFoot *vrvPgFoot = new PgFoot();
    ReadRunningElement(pgFoot, vrvPgFoot);

    parent->AddChild(vrvPgFoot);
    ReadUnsupportedAttr(pgFoot, vrvPgFoot);
    return ReadRunningChildren(vrvPgFoot, pgFoot, vrvPgFoot);
}

bool MeiInput::ReadPgFoot2(Object *parent, pugi::xml_node pgFoot2)
{
    PgFoot2 *vrvPgFoot2 = new PgFoot2();
    ReadRunningElement(pgFoot2, vrvPgFoot2);

    parent->AddChild(vrvPgFoot2);
    ReadUnsupportedAttr(pgFoot2, vrvPgFoot2);
    return ReadRunningChildren(vrvPgFoot2, pgFoot2, vrvPgFoot2);
}

bool MeiInput::ReadPgHead(Object *parent, pugi::xml_node pgHead)
{
    PgHead *vrvPgHead = new PgHead();
    ReadRunningElement(pgHead, vrvPgHead);

    parent->AddChild(vrvPgHead);
    ReadUnsupportedAttr(pgHead, vrvPgHead);
    return ReadRunningChildren(vrvPgHead, pgHead, vrvPgHead);
}

bool MeiInput::ReadPgHead2(Object *parent, pugi::xml_node pgHead2)
{
    PgHead2 *vrvPgHead2 = new PgHead2();
    ReadRunningElement(pgHead2, vrvPgHead2);

    parent->AddChild(vrvPgHead2);
    ReadUnsupportedAttr(pgHead2, vrvPgHead2);
    return ReadRunningChildren(vrvPgHead2, pgHead2, vrvPgHead2);
}

bool MeiInput::ReadRunningChildren(Object *parent, pugi::xml_node parentNode, Object *filter)
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
        if (IsEditorialElementName(xmlElement.name())) {
            success = ReadEditorialElement(parent, xmlElement, EDITORIAL_RUNNING, filter);
        }
        // content
        else if (elementName == "fig") {
            success = ReadFig(parent, xmlElement);
        }
        else if (elementName == "rend") {
            success = ReadRend(parent, xmlElement);
        }
        // unknown
        else {
            LogWarning("Element %s is unknown and will be ignored", xmlElement.name());
        }
        i++;
    }
    return success;
}

bool MeiInput::ReadStaffDef(Object *parent, pugi::xml_node staffDef)
{
    assert(dynamic_cast<StaffGrp *>(parent) || dynamic_cast<EditorialElement *>(parent));

    StaffDef *vrvStaffDef = new StaffDef();
    ReadScoreDefElement(staffDef, vrvStaffDef);

    if (m_version < MEI_4_0_0) {
        UpgradeScoreDefElementTo_4_0_0(staffDef, vrvStaffDef);
        UpgradeStaffDefTo_4_0_0(staffDef, vrvStaffDef);
    }

    vrvStaffDef->ReadDistances(staffDef);
    vrvStaffDef->ReadLabelled(staffDef);
    vrvStaffDef->ReadNInteger(staffDef);
    vrvStaffDef->ReadNotationType(staffDef);
    vrvStaffDef->ReadScalable(staffDef);
    vrvStaffDef->ReadStaffDefLog(staffDef);
    vrvStaffDef->ReadStaffDefVis(staffDef);
    vrvStaffDef->ReadTimeBase(staffDef);
    vrvStaffDef->ReadTransposition(staffDef);

    if (!vrvStaffDef->HasN()) {
        LogWarning("No @n on <staffDef> might yield unpredictable results");
    }

    ReadScoreDefInterface(staffDef, vrvStaffDef);

    m_doc->m_notationType = vrvStaffDef->GetNotationtype();

    parent->AddChild(vrvStaffDef);
    ReadUnsupportedAttr(staffDef, vrvStaffDef);
    return ReadStaffDefChildren(vrvStaffDef, staffDef);
}

bool MeiInput::ReadStaffDefChildren(Object *parent, pugi::xml_node parentNode)
{
    assert(dynamic_cast<StaffDef *>(parent) || dynamic_cast<EditorialElement *>(parent));

    bool success = true;
    pugi::xml_node current;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        if (!success) break;
        // clef, keySig, etc.
        else if (std::string(current.name()) == "clef") {
            success = ReadClef(parent, current);
        }
        else if (std::string(current.name()) == "keySig") {
            success = ReadKeySig(parent, current);
        }
        else if (std::string(current.name()) == "mensur") {
            success = ReadMensur(parent, current);
        }
        else if (std::string(current.name()) == "meterSig") {
            success = ReadMeterSig(parent, current);
        }
        // content
        else if (std::string(current.name()) == "instrDef") {
            success = ReadInstrDef(parent, current);
        }
        else if (std::string(current.name()) == "label") {
            success = ReadLabel(parent, current);
        }
        else if (std::string(current.name()) == "labelAbbr") {
            success = ReadLabelAbbr(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <staffDef>", current.name());
        }
    }
    return success;
}

bool MeiInput::ReadInstrDef(Object *parent, pugi::xml_node instrDef)
{
    InstrDef *vrvInstrDef = new InstrDef();
    SetMeiUuid(instrDef, vrvInstrDef);

    parent->AddChild(vrvInstrDef);
    vrvInstrDef->ReadChannelized(instrDef);
    vrvInstrDef->ReadLabelled(instrDef);
    vrvInstrDef->ReadMidiInstrument(instrDef);
    vrvInstrDef->ReadNNumberLike(instrDef);

    return true;
}

bool MeiInput::ReadLabel(Object *parent, pugi::xml_node label)
{
    Label *vrvLabel = new Label();
    SetMeiUuid(label, vrvLabel);

    parent->AddChild(vrvLabel);
    ReadUnsupportedAttr(label, vrvLabel);
    return ReadTextChildren(vrvLabel, label, vrvLabel);
}

bool MeiInput::ReadLabelAbbr(Object *parent, pugi::xml_node labelAbbr)
{
    LabelAbbr *vrvLabelAbbr = new LabelAbbr();
    SetMeiUuid(labelAbbr, vrvLabelAbbr);

    parent->AddChild(vrvLabelAbbr);
    ReadUnsupportedAttr(labelAbbr, vrvLabelAbbr);
    return ReadTextChildren(vrvLabelAbbr, labelAbbr, vrvLabelAbbr);
}

bool MeiInput::ReadMeasure(Object *parent, pugi::xml_node measure)
{
    Measure *vrvMeasure = new Measure();
    if (m_doc->IsMensuralMusicOnly()) {
        LogWarning("Mixing mensural and non mensural music is not supported. Trying to go ahead...");
        m_doc->SetMensuralMusicOnly(false);
    }
    SetMeiUuid(measure, vrvMeasure);

    vrvMeasure->ReadBarring(measure);
    vrvMeasure->ReadMeasureLog(measure);
    vrvMeasure->ReadMeterConformanceBar(measure);
    vrvMeasure->ReadNNumberLike(measure);
    vrvMeasure->ReadPointing(measure);
    vrvMeasure->ReadTyped(measure);

    if (measure.attribute("ulx") && measure.attribute("lrx") && (this->m_doc->GetType() == Transcription)) {
        vrvMeasure->m_xAbs = atoi(measure.attribute("ulx").value()) * DEFINITION_FACTOR;
        vrvMeasure->m_xAbs2 = atoi(measure.attribute("lrx").value()) * DEFINITION_FACTOR;
        measure.remove_attribute("ulx");
        measure.remove_attribute("lrx");
    }

    parent->AddChild(vrvMeasure);
    ReadUnsupportedAttr(measure, vrvMeasure);
    return ReadMeasureChildren(vrvMeasure, measure);
}

bool MeiInput::ReadMeasureChildren(Object *parent, pugi::xml_node parentNode)
{
    assert(dynamic_cast<Measure *>(parent) || dynamic_cast<EditorialElement *>(parent));

    bool success = true;
    pugi::xml_node current;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        if (!success) break;
        // editorial
        else if (IsEditorialElementName(current.name())) {
            success = ReadEditorialElement(parent, current, EDITORIAL_MEASURE);
        }
        // content
        else if (std::string(current.name()) == "anchoredText") {
            success = ReadAnchoredText(parent, current);
        }
        else if (std::string(current.name()) == "arpeg") {
            success = ReadArpeg(parent, current);
        }
        else if (std::string(current.name()) == "bracketSpan") {
            success = ReadBracketSpan(parent, current);
        }
        else if (std::string(current.name()) == "breath") {
            success = ReadBreath(parent, current);
        }
        else if (std::string(current.name()) == "dir") {
            success = ReadDir(parent, current);
        }
        else if (std::string(current.name()) == "dynam") {
            success = ReadDynam(parent, current);
        }
        else if (std::string(current.name()) == "fermata") {
            success = ReadFermata(parent, current);
        }
        else if (std::string(current.name()) == "hairpin") {
            success = ReadHairpin(parent, current);
        }
        else if (std::string(current.name()) == "harm") {
            success = ReadHarm(parent, current);
        }
        else if (std::string(current.name()) == "mNum") {
            success = ReadMNum(parent, current);
        }
        else if (std::string(current.name()) == "mordent") {
            success = ReadMordent(parent, current);
        }
        else if (std::string(current.name()) == "octave") {
            success = ReadOctave(parent, current);
        }
        else if (std::string(current.name()) == "pedal") {
            success = ReadPedal(parent, current);
        }
        else if (std::string(current.name()) == "slur") {
            success = ReadSlur(parent, current);
        }
        else if (std::string(current.name()) == "staff") {
            success = ReadStaff(parent, current);
        }
        else if (std::string(current.name()) == "tempo") {
            success = ReadTempo(parent, current);
        }
        else if (std::string(current.name()) == "tie") {
            success = ReadTie(parent, current);
        }
        else if (std::string(current.name()) == "trill") {
            success = ReadTrill(parent, current);
        }
        else if (std::string(current.name()) == "turn") {
            success = ReadTurn(parent, current);
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
    ReadLinkingInterface(element, object);
    object->ReadLabelled(element);
    object->ReadTyped(element);

    return true;
}

bool MeiInput::ReadAnchoredText(Object *parent, pugi::xml_node anchoredText)
{
    AnchoredText *vrvAnchoredText = new AnchoredText();
    ReadControlElement(anchoredText, vrvAnchoredText);

    ReadTextDirInterface(anchoredText, vrvAnchoredText);

    ReadUnsupportedAttr(anchoredText, vrvAnchoredText);
    parent->AddChild(vrvAnchoredText);
    return ReadTextChildren(vrvAnchoredText, anchoredText, vrvAnchoredText);
}

bool MeiInput::ReadArpeg(Object *parent, pugi::xml_node arpeg)
{
    Arpeg *vrvArpeg = new Arpeg();
    ReadControlElement(arpeg, vrvArpeg);

    ReadPlistInterface(arpeg, vrvArpeg);
    ReadTimePointInterface(arpeg, vrvArpeg);
    vrvArpeg->ReadArpegLog(arpeg);
    vrvArpeg->ReadArpegVis(arpeg);
    vrvArpeg->ReadColor(arpeg);

    parent->AddChild(vrvArpeg);
    ReadUnsupportedAttr(arpeg, vrvArpeg);
    return true;
}

bool MeiInput::ReadBracketSpan(Object *parent, pugi::xml_node bracketSpan)
{
    BracketSpan *vrvBracketSpan = new BracketSpan();
    ReadControlElement(bracketSpan, vrvBracketSpan);

    ReadTimeSpanningInterface(bracketSpan, vrvBracketSpan);
    vrvBracketSpan->ReadBracketSpanLog(bracketSpan);
    vrvBracketSpan->ReadColor(bracketSpan);
    vrvBracketSpan->ReadLineRend(bracketSpan);
    vrvBracketSpan->ReadLineRendBase(bracketSpan);

    parent->AddChild(vrvBracketSpan);
    ReadUnsupportedAttr(bracketSpan, vrvBracketSpan);
    return true;
}

bool MeiInput::ReadBreath(Object *parent, pugi::xml_node breath)
{
    Breath *vrvBreath = new Breath();
    ReadControlElement(breath, vrvBreath);

    ReadTimePointInterface(breath, vrvBreath);
    vrvBreath->ReadColor(breath);
    vrvBreath->ReadPlacement(breath);

    parent->AddChild(vrvBreath);
    ReadUnsupportedAttr(breath, vrvBreath);
    return true;
}

bool MeiInput::ReadDir(Object *parent, pugi::xml_node dir)
{
    Dir *vrvDir = new Dir();
    ReadControlElement(dir, vrvDir);

    ReadTextDirInterface(dir, vrvDir);
    ReadTimeSpanningInterface(dir, vrvDir);
    vrvDir->ReadLang(dir);
    vrvDir->ReadExtender(dir);
    vrvDir->ReadVerticalGroup(dir);

    parent->AddChild(vrvDir);
    ReadUnsupportedAttr(dir, vrvDir);
    return ReadTextChildren(vrvDir, dir, vrvDir);
}

bool MeiInput::ReadDynam(Object *parent, pugi::xml_node dynam)
{
    Dynam *vrvDynam = new Dynam();
    ReadControlElement(dynam, vrvDynam);

    ReadTextDirInterface(dynam, vrvDynam);
    ReadTimeSpanningInterface(dynam, vrvDynam);
    vrvDynam->ReadExtender(dynam);
    vrvDynam->ReadVerticalGroup(dynam);

    parent->AddChild(vrvDynam);
    ReadUnsupportedAttr(dynam, vrvDynam);
    return ReadTextChildren(vrvDynam, dynam, vrvDynam);
}

bool MeiInput::ReadFermata(Object *parent, pugi::xml_node fermata)
{
    Fermata *vrvFermata = new Fermata();
    ReadControlElement(fermata, vrvFermata);

    ReadTimePointInterface(fermata, vrvFermata);
    vrvFermata->ReadColor(fermata);
    vrvFermata->ReadFermataVis(fermata);
    vrvFermata->ReadPlacement(fermata);

    parent->AddChild(vrvFermata);
    ReadUnsupportedAttr(fermata, vrvFermata);
    return true;
}

bool MeiInput::ReadHairpin(Object *parent, pugi::xml_node hairpin)
{
    Hairpin *vrvHairpin = new Hairpin();
    ReadControlElement(hairpin, vrvHairpin);

    ReadTimeSpanningInterface(hairpin, vrvHairpin);
    vrvHairpin->ReadColor(hairpin);
    vrvHairpin->ReadHairpinLog(hairpin);
    vrvHairpin->ReadPlacement(hairpin);
    vrvHairpin->ReadVerticalGroup(hairpin);

    parent->AddChild(vrvHairpin);
    ReadUnsupportedAttr(hairpin, vrvHairpin);
    return true;
}

bool MeiInput::ReadHarm(Object *parent, pugi::xml_node harm)
{
    Harm *vrvHarm = new Harm();
    ReadControlElement(harm, vrvHarm);

    ReadTextDirInterface(harm, vrvHarm);
    ReadTimeSpanningInterface(harm, vrvHarm);
    vrvHarm->ReadLang(harm);
    vrvHarm->ReadNNumberLike(harm);

    parent->AddChild(vrvHarm);
    ReadUnsupportedAttr(harm, vrvHarm);
    return ReadTextChildren(vrvHarm, harm, vrvHarm);
}

bool MeiInput::ReadMNum(Object *parent, pugi::xml_node mNum)
{
    MNum *vrvMNum = new MNum();
    ReadControlElement(mNum, vrvMNum);

    ReadTextDirInterface(mNum, vrvMNum);
    ReadTimePointInterface(mNum, vrvMNum);
    vrvMNum->ReadColor(mNum);
    vrvMNum->ReadLang(mNum);
    vrvMNum->ReadTypography(mNum);

    parent->AddChild(vrvMNum);
    return ReadTextChildren(vrvMNum, mNum, vrvMNum);
}

bool MeiInput::ReadMordent(Object *parent, pugi::xml_node mordent)
{
    Mordent *vrvMordent = new Mordent();
    ReadControlElement(mordent, vrvMordent);

    if (m_version < MEI_4_0_0) {
        UpgradeMordentTo_4_0_0(mordent, vrvMordent);
    }

    ReadTimePointInterface(mordent, vrvMordent);
    vrvMordent->ReadColor(mordent);
    vrvMordent->ReadOrnamentAccid(mordent);
    vrvMordent->ReadPlacement(mordent);
    vrvMordent->ReadMordentLog(mordent);

    parent->AddChild(vrvMordent);
    ReadUnsupportedAttr(mordent, vrvMordent);
    return true;
}

bool MeiInput::ReadOctave(Object *parent, pugi::xml_node octave)
{
    Octave *vrvOctave = new Octave();
    ReadControlElement(octave, vrvOctave);

    ReadTimeSpanningInterface(octave, vrvOctave);
    vrvOctave->ReadColor(octave);
    vrvOctave->ReadExtender(octave);
    vrvOctave->ReadLineRend(octave);
    vrvOctave->ReadLineRendBase(octave);
    vrvOctave->ReadOctaveDisplacement(octave);

    parent->AddChild(vrvOctave);
    ReadUnsupportedAttr(octave, vrvOctave);
    return true;
}

bool MeiInput::ReadPedal(Object *parent, pugi::xml_node pedal)
{
    Pedal *vrvPedal = new Pedal();
    ReadControlElement(pedal, vrvPedal);

    ReadTimePointInterface(pedal, vrvPedal);
    vrvPedal->ReadColor(pedal);
    vrvPedal->ReadPedalLog(pedal);
    vrvPedal->ReadPlacement(pedal);
    vrvPedal->ReadVerticalGroup(pedal);

    parent->AddChild(vrvPedal);
    ReadUnsupportedAttr(pedal, vrvPedal);
    return true;
}

bool MeiInput::ReadSlur(Object *parent, pugi::xml_node slur)
{
    Slur *vrvSlur = new Slur();
    ReadControlElement(slur, vrvSlur);

    ReadTimeSpanningInterface(slur, vrvSlur);
    vrvSlur->ReadColor(slur);
    vrvSlur->ReadCurvature(slur);
    vrvSlur->ReadCurveRend(slur);

    parent->AddChild(vrvSlur);
    ReadUnsupportedAttr(slur, vrvSlur);
    return true;
}

bool MeiInput::ReadTempo(Object *parent, pugi::xml_node tempo)
{
    Tempo *vrvTempo = new Tempo();
    ReadControlElement(tempo, vrvTempo);

    ReadTextDirInterface(tempo, vrvTempo);
    ReadTimePointInterface(tempo, vrvTempo);
    vrvTempo->ReadLang(tempo);
    vrvTempo->ReadMidiTempo(tempo);
    vrvTempo->ReadMmTempo(tempo);

    parent->AddChild(vrvTempo);
    ReadUnsupportedAttr(tempo, vrvTempo);
    return ReadTextChildren(vrvTempo, tempo, vrvTempo);
}

bool MeiInput::ReadTie(Object *parent, pugi::xml_node tie)
{
    Tie *vrvTie = new Tie();
    ReadControlElement(tie, vrvTie);

    ReadTimeSpanningInterface(tie, vrvTie);
    vrvTie->ReadColor(tie);
    vrvTie->ReadCurvature(tie);
    vrvTie->ReadCurveRend(tie);

    parent->AddChild(vrvTie);
    ReadUnsupportedAttr(tie, vrvTie);
    return true;
}

bool MeiInput::ReadTrill(Object *parent, pugi::xml_node trill)
{
    Trill *vrvTrill = new Trill();
    ReadControlElement(trill, vrvTrill);

    ReadTimeSpanningInterface(trill, vrvTrill);
    vrvTrill->ReadColor(trill);
    vrvTrill->ReadOrnamentAccid(trill);
    vrvTrill->ReadPlacement(trill);

    parent->AddChild(vrvTrill);
    ReadUnsupportedAttr(trill, vrvTrill);
    return true;
}

bool MeiInput::ReadTurn(Object *parent, pugi::xml_node turn)
{
    Turn *vrvTurn = new Turn();
    ReadControlElement(turn, vrvTurn);

    if (m_version < MEI_4_0_0) {
        UpgradeTurnTo_4_0_0(turn, vrvTurn);
    }

    ReadTimePointInterface(turn, vrvTurn);
    vrvTurn->ReadColor(turn);
    vrvTurn->ReadOrnamentAccid(turn);
    vrvTurn->ReadPlacement(turn);
    vrvTurn->ReadTurnLog(turn);

    parent->AddChild(vrvTurn);
    ReadUnsupportedAttr(turn, vrvTurn);
    return true;
}

bool MeiInput::ReadFb(Object *parent, pugi::xml_node fb)
{
    Fb *vrvFb = new Fb();
    SetMeiUuid(fb, vrvFb);

    parent->AddChild(vrvFb);
    ReadUnsupportedAttr(fb, vrvFb);
    return ReadFbChildren(vrvFb, fb);
}

bool MeiInput::ReadFbChildren(Object *parent, pugi::xml_node parentNode)
{
    assert(dynamic_cast<Fb *>(parent) || dynamic_cast<EditorialElement *>(parent));

    bool success = true;
    pugi::xml_node current;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        if (!success) break;
        // editorial
        else if (IsEditorialElementName(current.name())) {
            success = ReadEditorialElement(parent, current, EDITORIAL_FB);
        }
        // content
        else if (std::string(current.name()) == "f") {
            success = ReadF(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <staff>", current.name());
        }
    }
    return success;
}

bool MeiInput::ReadStaff(Object *parent, pugi::xml_node staff)
{
    Staff *vrvStaff = new Staff();
    SetMeiUuid(staff, vrvStaff);

    vrvStaff->ReadFacsimile(staff);
    vrvStaff->ReadNInteger(staff);
    vrvStaff->ReadTyped(staff);
    vrvStaff->ReadVisibility(staff);

    if (staff.attribute("uly") && (this->m_doc->GetType() == Transcription)) {
        vrvStaff->m_yAbs = atoi(staff.attribute("uly").value()) * DEFINITION_FACTOR;
        staff.remove_attribute("uly");
    }

    if (!vrvStaff->HasN() || (vrvStaff->GetN() == 0)) {
        LogWarning("No @n on <staff> or a value of 0 might yield unpredictable results");
    }

    parent->AddChild(vrvStaff);
    ReadUnsupportedAttr(staff, vrvStaff);
    return ReadStaffChildren(vrvStaff, staff);
}

bool MeiInput::ReadStaffChildren(Object *parent, pugi::xml_node parentNode)
{
    assert(dynamic_cast<Staff *>(parent) || dynamic_cast<EditorialElement *>(parent));

    bool success = true;
    pugi::xml_node current;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        if (!success) break;
        // editorial
        else if (IsEditorialElementName(current.name())) {
            success = ReadEditorialElement(parent, current, EDITORIAL_STAFF);
        }
        // content
        else if (std::string(current.name()) == "layer") {
            success = ReadLayer(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <staff>", current.name());
        }
    }
    return success;
}

bool MeiInput::ReadLayer(Object *parent, pugi::xml_node layer)
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
    ReadUnsupportedAttr(layer, vrvLayer);
    return ReadLayerChildren(vrvLayer, layer);
}

bool MeiInput::ReadLayerChildren(Object *parent, pugi::xml_node parentNode, Object *filter)
{
    bool success = true;
    pugi::xml_node xmlElement;
    std::string elementName;
    for (xmlElement = parentNode.first_child(); xmlElement; xmlElement = xmlElement.next_sibling()) {
        if (!success) {
            break;
        }
        elementName = std::string(xmlElement.name());
        // LogDebug("ReadLayerChildren: element <%s>", xmlElement.name());
        if (!IsAllowed(elementName, filter)) {
            std::string meiElementName = filter->GetClassName();
            std::transform(meiElementName.begin(), meiElementName.begin() + 1, meiElementName.begin(), ::tolower);
            LogWarning("Element <%s> within <%s> is not supported and will be ignored ", xmlElement.name(),
                meiElementName.c_str());
            continue;
        }
        // editorial
        else if (IsEditorialElementName(xmlElement.name())) {
            success = ReadEditorialElement(parent, xmlElement, EDITORIAL_LAYER, filter);
        }
        // content
        else if (elementName == "accid") {
            success = ReadAccid(parent, xmlElement);
        }
        else if (elementName == "artic") {
            success = ReadArtic(parent, xmlElement);
        }
        else if (elementName == "barLine") {
            success = ReadBarLine(parent, xmlElement);
        }
        else if (elementName == "beam") {
            success = ReadBeam(parent, xmlElement);
        }
        else if (elementName == "beatRpt") {
            success = ReadBeatRpt(parent, xmlElement);
        }
        else if (elementName == "bTrem") {
            success = ReadBTrem(parent, xmlElement);
        }
        else if (elementName == "chord") {
            success = ReadChord(parent, xmlElement);
        }
        else if (elementName == "clef") {
            success = ReadClef(parent, xmlElement);
        }
        else if (elementName == "custos") {
            success = ReadCustos(parent, xmlElement);
        }
        else if (elementName == "dot") {
            success = ReadDot(parent, xmlElement);
        }
        else if (elementName == "fTrem") {
            success = ReadFTrem(parent, xmlElement);
        }
        else if (elementName == "graceGrp") {
            success = ReadGraceGrp(parent, xmlElement);
        }
        else if (elementName == "halfmRpt") {
            success = ReadHalfmRpt(parent, xmlElement);
        }
        else if (elementName == "keyAccid") {
            success = ReadKeyAccid(parent, xmlElement);
        }
        else if (elementName == "keySig") {
            success = ReadKeySig(parent, xmlElement);
        }
        else if (elementName == "ligature") {
            success = ReadLigature(parent, xmlElement);
        }
        else if (elementName == "mensur") {
            success = ReadMensur(parent, xmlElement);
        }
        else if (elementName == "meterSig") {
            success = ReadMeterSig(parent, xmlElement);
        }
        else if (elementName == "nc") {
            success = ReadNc(parent, xmlElement);
        }
        else if (elementName == "neume") {
            success = ReadNeume(parent, xmlElement);
        }
        else if (elementName == "note") {
            success = ReadNote(parent, xmlElement);
        }
        else if (elementName == "rest") {
            success = ReadRest(parent, xmlElement);
        }
        else if (elementName == "mRest") {
            success = ReadMRest(parent, xmlElement);
        }
        else if (elementName == "mRpt") {
            success = ReadMRpt(parent, xmlElement);
        }
        else if (elementName == "mRpt2") {
            success = ReadMRpt2(parent, xmlElement);
        }
        else if (elementName == "multiRest") {
            success = ReadMultiRest(parent, xmlElement);
        }
        else if (elementName == "multiRpt") {
            success = ReadMultiRpt(parent, xmlElement);
        }
        else if (elementName == "proport") {
            success = ReadProport(parent, xmlElement);
        }
        else if (elementName == "space") {
            success = ReadSpace(parent, xmlElement);
        }
        else if (elementName == "syl") {
            success = ReadSyl(parent, xmlElement);
        }
        else if (elementName == "syllable") {
            success = ReadSyllable(parent, xmlElement);
        }
        else if (elementName == "tuplet") {
            success = ReadTuplet(parent, xmlElement);
        }
        else if (elementName == "verse") {
            success = ReadVerse(parent, xmlElement);
        }
        // unknown
        else {
            LogWarning("Element '%s' is unknown and will be ignored", xmlElement.name());
        }
    }

    // if the current parent is a syllable then we need to make sure that a syl got added
    // if not then add a blank one
    if (strcmp(parentNode.name(), "syllable") == 0) {
        auto testSyl = parent->FindDescendantByType(SYL);
        if (testSyl == NULL) {
            Syl *syl = new Syl();
            parent->AddChild(syl);
        }
    }
    return success;
}

bool MeiInput::ReadLayerElement(pugi::xml_node element, LayerElement *object)
{
    if (element.attribute("ulx") && (this->m_doc->GetType() == Transcription)) {
        object->m_xAbs = atoi(element.attribute("ulx").value()) * DEFINITION_FACTOR;
        element.remove_attribute("ulx");
    } /*else if (element.attribute("facs") && this->m_doc->HasZones() && object->HasAttClass(ATT_COORDINATED)) {
        Zone zone = this->m_doc->GetZone(element.attribute("facs").value());
        AttCoordinated *att = dynamic_cast<AttCoordinated *>(object);
        assert(att);
        LogMessage("%d, %d, %d, %d", zone.ulx, zone.uly, zone.lrx, zone.lry);
        if (zone.ulx > 0) {
            att->SetUlx(zone.ulx);
        }
        if (zone.uly > 0) {
            att->SetUly(zone.uly);
        }
        if (zone.lrx > 0) {
            att->SetLrx(zone.lrx);
        }
        if (zone.lry > 0) {
            att->SetLry(zone.lry);
        }
        LogMessage("Set att_coordinated based on zone %s", zone.xmlId.c_str());
    }*/

    SetMeiUuid(element, object);
    ReadLinkingInterface(element, object);
    object->ReadLabelled(element);
    object->ReadTyped(element);

    return true;
}

bool MeiInput::ReadAccid(Object *parent, pugi::xml_node accid)
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
    ReadUnsupportedAttr(accid, vrvAccid);
    return true;
}

bool MeiInput::ReadArtic(Object *parent, pugi::xml_node artic)
{
    Artic *vrvArtic = new Artic();
    ReadLayerElement(artic, vrvArtic);

    vrvArtic->ReadArticulation(artic);
    vrvArtic->ReadColor(artic);
    vrvArtic->ReadPlacement(artic);

    parent->AddChild(vrvArtic);
    ReadUnsupportedAttr(artic, vrvArtic);
    return true;
}

bool MeiInput::ReadBarLine(Object *parent, pugi::xml_node barLine)
{
    BarLine *vrvBarLine = new BarLine();
    ReadLayerElement(barLine, vrvBarLine);

    vrvBarLine->ReadBarLineLog(barLine);
    vrvBarLine->ReadColor(barLine);
    vrvBarLine->ReadVisibility(barLine);

    parent->AddChild(vrvBarLine);
    ReadUnsupportedAttr(barLine, vrvBarLine);
    return true;
}

bool MeiInput::ReadBeam(Object *parent, pugi::xml_node beam)
{
    Beam *vrvBeam = new Beam();
    ReadLayerElement(beam, vrvBeam);

    vrvBeam->ReadColor(beam);
    vrvBeam->ReadBeamedWith(beam);
    vrvBeam->ReadBeamRend(beam);

    parent->AddChild(vrvBeam);
    ReadUnsupportedAttr(beam, vrvBeam);
    return ReadLayerChildren(vrvBeam, beam, vrvBeam);
}

bool MeiInput::ReadBeatRpt(Object *parent, pugi::xml_node beatRpt)
{
    BeatRpt *vrvBeatRpt = new BeatRpt();
    ReadLayerElement(beatRpt, vrvBeatRpt);

    vrvBeatRpt->ReadColor(beatRpt);
    vrvBeatRpt->ReadBeatRptVis(beatRpt);

    parent->AddChild(vrvBeatRpt);
    ReadUnsupportedAttr(beatRpt, vrvBeatRpt);
    return true;
}

bool MeiInput::ReadBTrem(Object *parent, pugi::xml_node bTrem)
{
    BTrem *vrvBTrem = new BTrem();
    ReadLayerElement(bTrem, vrvBTrem);

    vrvBTrem->ReadTremMeasured(bTrem);

    parent->AddChild(vrvBTrem);
    ReadUnsupportedAttr(bTrem, vrvBTrem);
    return ReadLayerChildren(vrvBTrem, bTrem, vrvBTrem);
}

bool MeiInput::ReadChord(Object *parent, pugi::xml_node chord)
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

    if (vrvChord->HasTie()) {
        m_doc->SetAnalyticalMarkup(true);
    }

    parent->AddChild(vrvChord);
    ReadUnsupportedAttr(chord, vrvChord);
    return ReadLayerChildren(vrvChord, chord, vrvChord);
}

bool MeiInput::ReadClef(Object *parent, pugi::xml_node clef)
{
    Clef *vrvClef = new Clef();
    ReadLayerElement(clef, vrvClef);
    ReadFacsimileInterface(clef, vrvClef);

    vrvClef->ReadClefShape(clef);
    vrvClef->ReadColor(clef);
    vrvClef->ReadLineLoc(clef);
    vrvClef->ReadOctaveDisplacement(clef);

    parent->AddChild(vrvClef);
    ReadUnsupportedAttr(clef, vrvClef);
    return true;
}

bool MeiInput::ReadCustos(Object *parent, pugi::xml_node custos)
{
    Custos *vrvCustos = new Custos();
    ReadLayerElement(custos, vrvCustos);

    ReadFacsimileInterface(custos, vrvCustos);
    ReadPitchInterface(custos, vrvCustos);
    ReadPositionInterface(custos, vrvCustos);
    vrvCustos->ReadColor(custos);

    parent->AddChild(vrvCustos);
    ReadUnsupportedAttr(custos, vrvCustos);
    return true;
}

bool MeiInput::ReadDot(Object *parent, pugi::xml_node dot)
{
    Dot *vrvDot = new Dot();
    ReadLayerElement(dot, vrvDot);

    ReadPositionInterface(dot, vrvDot);
    vrvDot->ReadColor(dot);
    vrvDot->ReadDotLog(dot);

    parent->AddChild(vrvDot);
    ReadUnsupportedAttr(dot, vrvDot);
    return true;
}

bool MeiInput::ReadFTrem(Object *parent, pugi::xml_node fTrem)
{
    FTrem *vrvFTrem = new FTrem();
    ReadLayerElement(fTrem, vrvFTrem);

    if (m_version < MEI_4_0_0) {
        UpgradeFTremTo_4_0_0(fTrem, vrvFTrem);
    }

    vrvFTrem->ReadFTremVis(fTrem);
    vrvFTrem->ReadTremMeasured(fTrem);

    parent->AddChild(vrvFTrem);
    ReadUnsupportedAttr(fTrem, vrvFTrem);
    return ReadLayerChildren(vrvFTrem, fTrem, vrvFTrem);
}

bool MeiInput::ReadGraceGrp(Object *parent, pugi::xml_node graceGrp)
{
    GraceGrp *vrvGraceGrp = new GraceGrp();
    ReadLayerElement(graceGrp, vrvGraceGrp);

    vrvGraceGrp->ReadColor(graceGrp);
    vrvGraceGrp->ReadGraced(graceGrp);
    vrvGraceGrp->ReadGraceGrpLog(graceGrp);

    parent->AddChild(vrvGraceGrp);
    ReadUnsupportedAttr(graceGrp, vrvGraceGrp);
    return ReadLayerChildren(vrvGraceGrp, graceGrp, vrvGraceGrp);
}

bool MeiInput::ReadHalfmRpt(Object *parent, pugi::xml_node halfmRpt)
{
    HalfmRpt *vrvHalfmRpt = new HalfmRpt();
    ReadLayerElement(halfmRpt, vrvHalfmRpt);

    parent->AddChild(vrvHalfmRpt);
    ReadUnsupportedAttr(halfmRpt, vrvHalfmRpt);
    return true;
}

bool MeiInput::ReadKeyAccid(Object *parent, pugi::xml_node keyAccid)
{
    KeyAccid *vrvKeyAccid = new KeyAccid();
    ReadLayerElement(keyAccid, vrvKeyAccid);

    ReadPitchInterface(keyAccid, vrvKeyAccid);
    vrvKeyAccid->ReadAccidental(keyAccid);
    vrvKeyAccid->ReadColor(keyAccid);
    vrvKeyAccid->ReadEnclosingChars(keyAccid);

    parent->AddChild(vrvKeyAccid);
    ReadUnsupportedAttr(keyAccid, vrvKeyAccid);
    return true;
}

bool MeiInput::ReadKeySig(Object *parent, pugi::xml_node keySig)
{
    KeySig *vrvKeySig = new KeySig();
    ReadLayerElement(keySig, vrvKeySig);

    vrvKeySig->ReadAccidental(keySig);
    vrvKeySig->ReadPitch(keySig);
    vrvKeySig->ReadKeySigAnl(keySig);
    vrvKeySig->ReadKeySigLog(keySig);
    vrvKeySig->ReadKeySigVis(keySig);
    vrvKeySig->ReadVisibility(keySig);

    parent->AddChild(vrvKeySig);
    ReadUnsupportedAttr(keySig, vrvKeySig);
    return ReadLayerChildren(vrvKeySig, keySig, vrvKeySig);
}

bool MeiInput::ReadLigature(Object *parent, pugi::xml_node ligature)
{
    Ligature *vrvLigature = new Ligature();
    SetMeiUuid(ligature, vrvLigature);

    vrvLigature->ReadLigatureLog(ligature);

    parent->AddChild(vrvLigature);
    ReadUnsupportedAttr(ligature, vrvLigature);
    return ReadLayerChildren(vrvLigature, ligature, vrvLigature);
}

bool MeiInput::ReadMensur(Object *parent, pugi::xml_node mensur)
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
    vrvMensur->ReadStaffLoc(mensur);

    parent->AddChild(vrvMensur);
    ReadUnsupportedAttr(mensur, vrvMensur);
    return true;
}

bool MeiInput::ReadMeterSig(Object *parent, pugi::xml_node meterSig)
{
    MeterSig *vrvMeterSig = new MeterSig();
    ReadLayerElement(meterSig, vrvMeterSig);

    vrvMeterSig->ReadMeterSigLog(meterSig);
    vrvMeterSig->ReadMeterSigVis(meterSig);

    parent->AddChild(vrvMeterSig);
    ReadUnsupportedAttr(meterSig, vrvMeterSig);
    return true;
}

bool MeiInput::ReadMRest(Object *parent, pugi::xml_node mRest)
{
    MRest *vrvMRest = new MRest();
    ReadLayerElement(mRest, vrvMRest);
    ReadPositionInterface(mRest, vrvMRest);

    vrvMRest->ReadCue(mRest);
    vrvMRest->ReadFermataPresent(mRest);
    vrvMRest->ReadVisibility(mRest);

    if (vrvMRest->HasFermata()) {
        m_doc->SetAnalyticalMarkup(true);
    }

    parent->AddChild(vrvMRest);
    ReadUnsupportedAttr(mRest, vrvMRest);
    return true;
}

bool MeiInput::ReadMRpt(Object *parent, pugi::xml_node mRpt)
{
    MRpt *vrvMRpt = new MRpt();
    ReadLayerElement(mRpt, vrvMRpt);

    parent->AddChild(vrvMRpt);
    ReadUnsupportedAttr(mRpt, vrvMRpt);
    return true;
}

bool MeiInput::ReadMRpt2(Object *parent, pugi::xml_node mRpt2)
{
    MRpt2 *vrvMRpt2 = new MRpt2();
    ReadLayerElement(mRpt2, vrvMRpt2);

    parent->AddChild(vrvMRpt2);
    ReadUnsupportedAttr(mRpt2, vrvMRpt2);
    return true;
}

bool MeiInput::ReadMultiRest(Object *parent, pugi::xml_node multiRest)
{
    MultiRest *vrvMultiRest = new MultiRest();
    ReadLayerElement(multiRest, vrvMultiRest);

    vrvMultiRest->ReadMultiRestVis(multiRest);
    vrvMultiRest->ReadNumbered(multiRest);

    parent->AddChild(vrvMultiRest);
    ReadUnsupportedAttr(multiRest, vrvMultiRest);
    return true;
}

bool MeiInput::ReadMultiRpt(Object *parent, pugi::xml_node multiRpt)
{
    MultiRpt *vrvMultiRpt = new MultiRpt();
    ReadLayerElement(multiRpt, vrvMultiRpt);

    vrvMultiRpt->ReadNumbered(multiRpt);

    parent->AddChild(vrvMultiRpt);
    ReadUnsupportedAttr(multiRpt, vrvMultiRpt);
    return true;
}

bool MeiInput::ReadNc(Object *parent, pugi::xml_node nc)
{
    Nc *vrvNc = new Nc();
    ReadLayerElement(nc, vrvNc);

    ReadDurationInterface(nc, vrvNc);
    ReadFacsimileInterface(nc, vrvNc);
    ReadPitchInterface(nc, vrvNc);
    ReadPositionInterface(nc, vrvNc);
    vrvNc->ReadColor(nc);
    vrvNc->ReadIntervalMelodic(nc);
    vrvNc->ReadNcForm(nc);

    parent->AddChild(vrvNc);
    return ReadLayerChildren(vrvNc, nc, vrvNc);
}

bool MeiInput::ReadNeume(Object *parent, pugi::xml_node neume)
{
    Neume *vrvNeume = new Neume();
    ReadLayerElement(neume, vrvNeume);
    ReadFacsimileInterface(neume, vrvNeume);

    vrvNeume->ReadColor(neume);

    parent->AddChild(vrvNeume);
    return ReadLayerChildren(vrvNeume, neume, vrvNeume);
}

bool MeiInput::ReadNote(Object *parent, pugi::xml_node note)
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
    vrvNote->ReadMidiVelocity(note);
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

    if (vrvNote->HasTie()) {
        m_doc->SetAnalyticalMarkup(true);
    }

    parent->AddChild(vrvNote);
    ReadUnsupportedAttr(note, vrvNote);
    return ReadLayerChildren(vrvNote, note, vrvNote);
}

bool MeiInput::ReadRest(Object *parent, pugi::xml_node rest)
{
    Rest *vrvRest = new Rest();
    ReadLayerElement(rest, vrvRest);

    ReadDurationInterface(rest, vrvRest);
    ReadPositionInterface(rest, vrvRest);
    vrvRest->ReadColor(rest);
    vrvRest->ReadCue(rest);
    vrvRest->ReadRestVisMensural(rest);

    parent->AddChild(vrvRest);
    ReadUnsupportedAttr(rest, vrvRest);
    return ReadLayerChildren(vrvRest, rest, vrvRest);
}

bool MeiInput::ReadProport(Object *parent, pugi::xml_node proport)
{
    Proport *vrvProport = new Proport();
    ReadLayerElement(proport, vrvProport);

    vrvProport->ReadDurationRatio(proport);

    parent->AddChild(vrvProport);
    ReadUnsupportedAttr(proport, vrvProport);
    return true;
}

bool MeiInput::ReadSpace(Object *parent, pugi::xml_node space)
{
    Space *vrvSpace = new Space();
    ReadLayerElement(space, vrvSpace);

    ReadDurationInterface(space, vrvSpace);

    parent->AddChild(vrvSpace);
    ReadUnsupportedAttr(space, vrvSpace);
    return true;
}

bool MeiInput::ReadSyl(Object *parent, pugi::xml_node syl)
{
    Syl *vrvSyl = new Syl();
    ReadLayerElement(syl, vrvSyl);

    vrvSyl->ReadLang(syl);
    vrvSyl->ReadTypography(syl);
    vrvSyl->ReadSylLog(syl);

    parent->AddChild(vrvSyl);
    ReadUnsupportedAttr(syl, vrvSyl);
    return ReadTextChildren(vrvSyl, syl, vrvSyl);
}

bool MeiInput::ReadSyllable(Object *parent, pugi::xml_node syllable)
{
    bool success;
    Syllable *vrvSyllable = new Syllable();
    ReadLayerElement(syllable, vrvSyllable);

    vrvSyllable->ReadColor(syllable);
    vrvSyllable->ReadSlashCount(syllable);

    parent->AddChild(vrvSyllable);

    // read all of the syllables elements
    // and add an empty <syl> if it doesn't have one
    if ((success = ReadLayerChildren(vrvSyllable, syllable, vrvSyllable))) {

        Object *obj = vrvSyllable->FindDescendantByType(SYL);
        Syl *syl = dynamic_cast<Syl *>(obj);

        if (syl == NULL) {
            syl = new Syl();
            vrvSyllable->AddChild(syl);
        }
    }
    return success;
}

bool MeiInput::ReadTuplet(Object *parent, pugi::xml_node tuplet)
{
    Tuplet *vrvTuplet = new Tuplet();
    ReadLayerElement(tuplet, vrvTuplet);

    vrvTuplet->ReadColor(tuplet);
    vrvTuplet->ReadDurationRatio(tuplet);
    vrvTuplet->ReadNumberPlacement(tuplet);
    vrvTuplet->ReadTupletVis(tuplet);

    parent->AddChild(vrvTuplet);
    ReadUnsupportedAttr(tuplet, vrvTuplet);
    return ReadLayerChildren(vrvTuplet, tuplet, vrvTuplet);
}

bool MeiInput::ReadVerse(Object *parent, pugi::xml_node verse)
{
    Verse *vrvVerse = new Verse();
    ReadLayerElement(verse, vrvVerse);

    vrvVerse->ReadColor(verse);
    vrvVerse->ReadLang(verse);
    vrvVerse->ReadNInteger(verse);
    vrvVerse->ReadTypography(verse);

    parent->AddChild(vrvVerse);
    ReadUnsupportedAttr(verse, vrvVerse);
    return ReadLayerChildren(vrvVerse, verse, vrvVerse);
}

bool MeiInput::ReadTextChildren(Object *parent, pugi::xml_node parentNode, Object *filter)
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
            success = ReadEditorialElement(parent, xmlElement, EDITORIAL_TEXT, filter);
        }
        // content
        else if (elementName == "fig") {
            success = ReadFig(parent, xmlElement);
        }
        else if (elementName == "lb") {
            success = ReadLb(parent, xmlElement);
        }
        else if (elementName == "num") {
            success = ReadNum(parent, xmlElement);
        }
        else if (elementName == "rend") {
            success = ReadRend(parent, xmlElement);
        }
        else if (elementName == "svg") {
            success = ReadSvg(parent, xmlElement);
        }
        else if (xmlElement.text()) {
            bool trimLeft = (i == 0);
            bool trimRight = (!xmlElement.next_sibling());
            success = ReadText(parent, xmlElement, trimLeft, trimRight);
        }
        // figured bass
        else if (elementName == "fb") {
            success = ReadFb(parent, xmlElement);
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
    object->ReadLabelled(element);
    object->ReadTyped(element);

    return true;
}

bool MeiInput::ReadF(Object *parent, pugi::xml_node f)
{
    F *vrvF = new F();
    ReadTextElement(f, vrvF);

    ReadTimeSpanningInterface(f, vrvF);

    parent->AddChild(vrvF);
    ReadUnsupportedAttr(f, vrvF);
    return ReadTextChildren(vrvF, f, vrvF);
}

bool MeiInput::ReadFig(Object *parent, pugi::xml_node fig)
{
    Fig *vrvFig = new Fig();
    ReadTextElement(fig, vrvFig);

    ReadAreaPosInterface(fig, vrvFig);

    parent->AddChild(vrvFig);
    ReadUnsupportedAttr(fig, vrvFig);
    return ReadTextChildren(vrvFig, fig, vrvFig);
}

bool MeiInput::ReadLb(Object *parent, pugi::xml_node lb)
{
    Lb *vrvLb = new Lb();
    ReadTextElement(lb, vrvLb);

    parent->AddChild(vrvLb);
    ReadUnsupportedAttr(lb, vrvLb);
    return true;
}

bool MeiInput::ReadNum(Object *parent, pugi::xml_node num)
{
    Num *vrvNum = new Num();
    ReadTextElement(num, vrvNum);

    parent->AddChild(vrvNum);
    ReadUnsupportedAttr(num, vrvNum);
    return ReadTextChildren(vrvNum, num, vrvNum);
}

bool MeiInput::ReadRend(Object *parent, pugi::xml_node rend)
{
    Rend *vrvRend = new Rend();
    ReadTextElement(rend, vrvRend);

    ReadAreaPosInterface(rend, vrvRend);

    vrvRend->ReadColor(rend);
    vrvRend->ReadLang(rend);
    vrvRend->ReadTextRendition(rend);
    vrvRend->ReadTypography(rend);
    vrvRend->ReadWhitespace(rend);

    if (vrvRend->GetFirstAncestor(REND) && (vrvRend->HasHalign() || vrvRend->HasValign())) {
        LogWarning("@halign or @valign in nested <rend> element <rend> %s will be ignored", vrvRend->GetUuid().c_str());
        // Eventually to be added to unsupported attributes?
        vrvRend->SetHalign(HORIZONTALALIGNMENT_NONE);
        vrvRend->SetValign(VERTICALALIGNMENT_NONE);
    }

    parent->AddChild(vrvRend);
    ReadUnsupportedAttr(rend, vrvRend);
    return ReadTextChildren(vrvRend, rend, vrvRend);
}

bool MeiInput::ReadSvg(Object *parent, pugi::xml_node svg)
{
    Svg *vrvSvg = new Svg();
    SetMeiUuid(svg, vrvSvg);

    if (std::string(svg.name()) == "svg") {
        vrvSvg->Set(svg);
    }
    else {
        LogWarning("No svg content found for <fig> %s", parent->GetUuid().c_str());
    }

    parent->AddChild(vrvSvg);
    ReadUnsupportedAttr(svg, vrvSvg);
    return true;
}

bool MeiInput::ReadText(Object *parent, pugi::xml_node text, bool trimLeft, bool trimRight)
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

bool MeiInput::ReadAreaPosInterface(pugi::xml_node element, AreaPosInterface *interface)
{
    interface->ReadHorizontalAlign(element);
    interface->ReadVerticalAlign(element);
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

    if (interface->HasFermata()) {
        m_doc->SetAnalyticalMarkup(true);
    }

    return true;
}

bool MeiInput::ReadLinkingInterface(pugi::xml_node element, LinkingInterface *interface)
{
    interface->ReadLinking(element);
    return true;
}

bool MeiInput::ReadFacsimileInterface(pugi::xml_node element, FacsimileInterface *interface)
{
    interface->ReadFacsimile(element);
    return true;
}

bool MeiInput::ReadPitchInterface(pugi::xml_node element, PitchInterface *interface)
{
    interface->ReadNoteGes(element);
    interface->ReadOctave(element);
    interface->ReadPitch(element);
    return true;
}

bool MeiInput::ReadPlistInterface(pugi::xml_node element, PlistInterface *interface)
{
    interface->ReadPlist(element);
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
    interface->ReadLyricStyle(element);
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

bool MeiInput::ReadEditorialElement(Object *parent, pugi::xml_node current, EditorialLevel level, Object *filter)
{
    if (std::string(current.name()) == "abbr") {
        return ReadAbbr(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "add") {
        return ReadAdd(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "app") {
        return ReadApp(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "annot") {
        return ReadAnnot(parent, current);
    }
    else if (std::string(current.name()) == "choice") {
        return ReadChoice(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "corr") {
        return ReadCorr(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "damage") {
        return ReadDamage(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "del") {
        return ReadDel(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "expan") {
        return ReadExpan(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "orig") {
        return ReadOrig(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "ref") {
        return ReadRef(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "reg") {
        return ReadReg(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "restore") {
        return ReadRestore(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "sic") {
        return ReadSic(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "subst") {
        return ReadSubst(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "supplied") {
        return ReadSupplied(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "unclear") {
        return ReadUnclear(parent, current, level, filter);
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

bool MeiInput::ReadAbbr(Object *parent, pugi::xml_node abbr, EditorialLevel level, Object *filter)
{
    Abbr *vrvAbbr = new Abbr();
    ReadEditorialElement(abbr, vrvAbbr);

    vrvAbbr->ReadSource(abbr);

    parent->AddChild(vrvAbbr);
    ReadUnsupportedAttr(abbr, vrvAbbr);
    return ReadEditorialChildren(vrvAbbr, abbr, level, filter);
}

bool MeiInput::ReadAdd(Object *parent, pugi::xml_node add, EditorialLevel level, Object *filter)
{
    Add *vrvAdd = new Add();
    ReadEditorialElement(add, vrvAdd);

    vrvAdd->ReadSource(add);

    parent->AddChild(vrvAdd);
    ReadUnsupportedAttr(add, vrvAdd);
    return ReadEditorialChildren(vrvAdd, add, level, filter);
}

bool MeiInput::ReadAnnot(Object *parent, pugi::xml_node annot)
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
    ReadUnsupportedAttr(annot, vrvAnnot);
    // For Annot we do not load children because they preserved in Annot::m_content
    return true;
}

bool MeiInput::ReadApp(Object *parent, pugi::xml_node app, EditorialLevel level, Object *filter)
{
    if (!m_hasScoreDef && m_useScoreDefForDoc) {
        LogError("<app> before any <scoreDef> is not supported");
        return false;
    }
    App *vrvApp = new App(level);
    ReadEditorialElement(app, vrvApp);

    parent->AddChild(vrvApp);
    ReadUnsupportedAttr(app, vrvApp);
    return ReadAppChildren(vrvApp, app, level, filter);
}

bool MeiInput::ReadAppChildren(Object *parent, pugi::xml_node parentNode, EditorialLevel level, Object *filter)
{
    assert(dynamic_cast<App *>(parent));

    // Check if one child node matches the m_appXPathQuery
    pugi::xml_node selectedLemOrRdg;
    std::vector<std::string> xPathQueries = m_doc->GetOptions()->m_appXPathQuery.GetValue();
    if (xPathQueries.size() > 0) {
        auto i = std::find_if(xPathQueries.begin(), xPathQueries.end(),
            [parentNode](std::string &query) { return (parentNode.select_node(query.c_str())); });
        if (i != xPathQueries.end()) selectedLemOrRdg = parentNode.select_node(i->c_str()).node();
    }

    bool success = true;
    bool hasXPathSelected = false;
    pugi::xml_node current;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        if (!success) break;
        if (std::string(current.name()) == "lem") {
            success = ReadLem(parent, current, level, filter);
        }
        else if (std::string(current.name()) == "rdg") {
            success = ReadRdg(parent, current, level, filter);
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

bool MeiInput::ReadChoice(Object *parent, pugi::xml_node choice, EditorialLevel level, Object *filter)
{
    if (!m_hasScoreDef) {
        LogError("<choice> before any <scoreDef> is not supported");
        return false;
    }
    Choice *vrvChoice = new Choice(level);
    ReadEditorialElement(choice, vrvChoice);

    parent->AddChild(vrvChoice);
    ReadUnsupportedAttr(choice, vrvChoice);
    return ReadChoiceChildren(vrvChoice, choice, level, filter);
}

bool MeiInput::ReadChoiceChildren(Object *parent, pugi::xml_node parentNode, EditorialLevel level, Object *filter)
{
    assert(dynamic_cast<Choice *>(parent));

    // Check if one child node matches a value in m_choiceXPathQueries
    pugi::xml_node selectedChild;
    std::vector<std::string> xPathQueries = m_doc->GetOptions()->m_choiceXPathQuery.GetValue();
    if (xPathQueries.size() > 0) {
        auto i = std::find_if(xPathQueries.begin(), xPathQueries.end(),
            [parentNode](std::string &query) { return (parentNode.select_node(query.c_str())); });
        if (i != xPathQueries.end()) {
            selectedChild = parentNode.select_node(i->c_str()).node();
        }
    }

    bool success = true;
    bool hasXPathSelected = false;
    pugi::xml_node current;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        if (!success) break;
        if (std::string(current.name()) == "abbr") {
            success = ReadAbbr(parent, current, level, filter);
        }
        else if (std::string(current.name()) == "choice") {
            success = ReadChoice(parent, current, level, filter);
        }
        else if (std::string(current.name()) == "corr") {
            success = ReadCorr(parent, current, level, filter);
        }
        else if (std::string(current.name()) == "expan") {
            success = ReadExpan(parent, current, level, filter);
        }
        else if (std::string(current.name()) == "orig") {
            success = ReadOrig(parent, current, level, filter);
        }
        else if (std::string(current.name()) == "ref") {
            success = ReadRef(parent, current, level, filter);
        }
        else if (std::string(current.name()) == "reg") {
            success = ReadReg(parent, current, level, filter);
        }
        else if (std::string(current.name()) == "sic") {
            success = ReadSic(parent, current, level, filter);
        }
        else if (std::string(current.name()) == "unclear") {
            success = ReadUnclear(parent, current, level, filter);
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

bool MeiInput::ReadCorr(Object *parent, pugi::xml_node corr, EditorialLevel level, Object *filter)
{
    Corr *vrvCorr = new Corr();
    ReadEditorialElement(corr, vrvCorr);

    vrvCorr->ReadSource(corr);

    parent->AddChild(vrvCorr);
    ReadUnsupportedAttr(corr, vrvCorr);
    return ReadEditorialChildren(vrvCorr, corr, level, filter);
}

bool MeiInput::ReadDamage(Object *parent, pugi::xml_node damage, EditorialLevel level, Object *filter)
{
    Damage *vrvDamage = new Damage();
    ReadEditorialElement(damage, vrvDamage);

    vrvDamage->ReadSource(damage);

    parent->AddChild(vrvDamage);
    ReadUnsupportedAttr(damage, vrvDamage);
    return ReadEditorialChildren(vrvDamage, damage, level, filter);
}

bool MeiInput::ReadDel(Object *parent, pugi::xml_node del, EditorialLevel level, Object *filter)
{
    Del *vrvDel = new Del();
    ReadEditorialElement(del, vrvDel);

    vrvDel->ReadSource(del);

    parent->AddChild(vrvDel);
    ReadUnsupportedAttr(del, vrvDel);
    return ReadEditorialChildren(vrvDel, del, level, filter);
}

bool MeiInput::ReadExpan(Object *parent, pugi::xml_node expan, EditorialLevel level, Object *filter)
{
    Expan *vrvExpan = new Expan();
    ReadEditorialElement(expan, vrvExpan);

    vrvExpan->ReadSource(expan);

    parent->AddChild(vrvExpan);
    ReadUnsupportedAttr(expan, vrvExpan);
    return ReadEditorialChildren(vrvExpan, expan, level, filter);
}

bool MeiInput::ReadLem(Object *parent, pugi::xml_node lem, EditorialLevel level, Object *filter)
{
    assert(dynamic_cast<App *>(parent));

    Lem *vrvLem = new Lem();
    // By default make them all hidden. MeiInput::ReadAppChildren will make one visible.
    vrvLem->m_visibility = Hidden;
    ReadEditorialElement(lem, vrvLem);

    vrvLem->ReadSource(lem);

    parent->AddChild(vrvLem);
    ReadUnsupportedAttr(lem, vrvLem);
    return ReadEditorialChildren(vrvLem, lem, level, filter);
}

bool MeiInput::ReadOrig(Object *parent, pugi::xml_node orig, EditorialLevel level, Object *filter)
{
    Orig *vrvOrig = new Orig();
    ReadEditorialElement(orig, vrvOrig);

    vrvOrig->ReadSource(orig);

    parent->AddChild(vrvOrig);
    ReadUnsupportedAttr(orig, vrvOrig);
    return ReadEditorialChildren(vrvOrig, orig, level, filter);
}

bool MeiInput::ReadRdg(Object *parent, pugi::xml_node rdg, EditorialLevel level, Object *filter)
{
    assert(dynamic_cast<App *>(parent));

    Rdg *vrvRdg = new Rdg();
    // By default make them all hidden. MeiInput::ReadAppChildren will make one visible.
    vrvRdg->m_visibility = Hidden;
    ReadEditorialElement(rdg, vrvRdg);

    vrvRdg->ReadSource(rdg);

    parent->AddChild(vrvRdg);
    ReadUnsupportedAttr(rdg, vrvRdg);
    return ReadEditorialChildren(vrvRdg, rdg, level, filter);
}

bool MeiInput::ReadRef(Object *parent, pugi::xml_node ref, EditorialLevel level, Object *filter)
{
    Ref *vrvRef = new Ref();
    ReadEditorialElement(ref, vrvRef);

    parent->AddChild(vrvRef);
    ReadUnsupportedAttr(ref, vrvRef);
    return ReadEditorialChildren(vrvRef, ref, level, filter);
}

bool MeiInput::ReadReg(Object *parent, pugi::xml_node reg, EditorialLevel level, Object *filter)
{
    Reg *vrvReg = new Reg();
    ReadEditorialElement(reg, vrvReg);

    vrvReg->ReadSource(reg);

    parent->AddChild(vrvReg);
    ReadUnsupportedAttr(reg, vrvReg);
    return ReadEditorialChildren(vrvReg, reg, level, filter);
}

bool MeiInput::ReadRestore(Object *parent, pugi::xml_node restore, EditorialLevel level, Object *filter)
{
    Restore *vrvRestore = new Restore();
    ReadEditorialElement(restore, vrvRestore);

    vrvRestore->ReadSource(restore);

    parent->AddChild(vrvRestore);
    ReadUnsupportedAttr(restore, vrvRestore);
    return ReadEditorialChildren(vrvRestore, restore, level, filter);
}

bool MeiInput::ReadSic(Object *parent, pugi::xml_node sic, EditorialLevel level, Object *filter)
{
    Sic *vrvSic = new Sic();
    ReadEditorialElement(sic, vrvSic);

    vrvSic->ReadSource(sic);

    parent->AddChild(vrvSic);
    ReadUnsupportedAttr(sic, vrvSic);
    return ReadEditorialChildren(vrvSic, sic, level, filter);
}

bool MeiInput::ReadSubst(Object *parent, pugi::xml_node subst, EditorialLevel level, Object *filter)
{
    if (!m_hasScoreDef) {
        LogError("<subst> before any <scoreDef> is not supported");
        return false;
    }
    Subst *vrvSubst = new Subst(level);
    ReadEditorialElement(subst, vrvSubst);

    parent->AddChild(vrvSubst);
    ReadUnsupportedAttr(subst, vrvSubst);
    return ReadSubstChildren(vrvSubst, subst, level, filter);
}

bool MeiInput::ReadSubstChildren(Object *parent, pugi::xml_node parentNode, EditorialLevel level, Object *filter)
{
    assert(dynamic_cast<Subst *>(parent));

    // Check if one child node matches a value in m_substXPathQueries
    pugi::xml_node selectedChild;
    std::vector<std::string> xPathQueries = m_doc->GetOptions()->m_substXPathQuery.GetValue();
    if (xPathQueries.size() > 0) {
        auto i = std::find_if(xPathQueries.begin(), xPathQueries.end(),
            [parentNode](std::string &query) { return (parentNode.select_node(query.c_str())); });
        if (i != xPathQueries.end()) {
            selectedChild = parentNode.select_node(i->c_str()).node();
        }
    }

    bool success = true;
    bool hasXPathSelected = false;
    pugi::xml_node current;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        if (!success) break;
        if (std::string(current.name()) == "add") {
            success = ReadAdd(parent, current, level, filter);
        }
        else if (std::string(current.name()) == "del") {
            success = ReadDel(parent, current, level, filter);
        }
        else if (std::string(current.name()) == "subst") {
            success = ReadSubst(parent, current, level, filter);
        }
        else {
            LogWarning("Unsupported '<%s>' within <subst>", current.name());
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
            LogWarning("Could not make one child of <subst> visible");
        }
    }
    return success;
}

bool MeiInput::ReadSupplied(Object *parent, pugi::xml_node supplied, EditorialLevel level, Object *filter)
{
    Supplied *vrvSupplied = new Supplied();
    ReadEditorialElement(supplied, vrvSupplied);

    vrvSupplied->ReadSource(supplied);

    parent->AddChild(vrvSupplied);
    ReadUnsupportedAttr(supplied, vrvSupplied);
    return ReadEditorialChildren(vrvSupplied, supplied, level, filter);
}

bool MeiInput::ReadUnclear(Object *parent, pugi::xml_node unclear, EditorialLevel level, Object *filter)
{
    Unclear *vrvUnclear = new Unclear();
    ReadEditorialElement(unclear, vrvUnclear);

    vrvUnclear->ReadSource(unclear);

    parent->AddChild(vrvUnclear);
    ReadUnsupportedAttr(unclear, vrvUnclear);
    return ReadEditorialChildren(vrvUnclear, unclear, level, filter);
}

bool MeiInput::ReadEditorialChildren(Object *parent, pugi::xml_node parentNode, EditorialLevel level, Object *filter)
{
    assert(dynamic_cast<EditorialElement *>(parent));

    if (level == EDITORIAL_TOPLEVEL) {
        if (m_readingScoreBased)
            return ReadSectionChildren(parent, parentNode);
        else
            return ReadSystemChildren(parent, parentNode);
    }
    else if (level == EDITORIAL_SCOREDEF) {
        return ReadScoreDefChildren(parent, parentNode);
    }
    else if (level == EDITORIAL_STAFFGRP) {
        return ReadStaffGrpChildren(parent, parentNode);
    }
    else if (level == EDITORIAL_MEASURE) {
        return ReadMeasureChildren(parent, parentNode);
    }
    else if (level == EDITORIAL_STAFF) {
        return ReadStaffChildren(parent, parentNode);
    }
    else if (level == EDITORIAL_LAYER) {
        return ReadLayerChildren(parent, parentNode, filter);
    }
    else if (level == EDITORIAL_TEXT) {
        return ReadTextChildren(parent, parentNode, filter);
    }
    else if (level == EDITORIAL_FB) {
        return ReadFbChildren(parent, parentNode);
    }
    else if (level == EDITORIAL_RUNNING) {
        return ReadRunningChildren(parent, parentNode, filter);
    }
    else {
        return false;
    }
}

bool MeiInput::ReadTupletSpanAsTuplet(Measure *measure, pugi::xml_node tupletSpan)
{
    if (!measure) {
        LogWarning("Cannot read <tupletSpan> within editorial markup");
        return false;
    }

    Tuplet *tuplet = new Tuplet();
    SetMeiUuid(tupletSpan, tuplet);

    LayerElement *start = NULL;
    LayerElement *end = NULL;

    AttConverter converter;

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
    if (tupletSpan.attribute("num.visible")) {
        tuplet->SetNumVisible(converter.StrToBoolean(tupletSpan.attribute("num.visible").value()));
    }
    if (tupletSpan.attribute("num.place")) {
        tuplet->SetNumPlace(converter.StrToStaffrelBasic(tupletSpan.attribute("num.place").value()));
    }
    if (tupletSpan.attribute("bracket.visible")) {
        tuplet->SetBracketVisible(converter.StrToBoolean(tupletSpan.attribute("bracket.visible").value()));
    }
    if (tupletSpan.attribute("bracket.place")) {
        tuplet->SetBracketPlace(converter.StrToStaffrelBasic(tupletSpan.attribute("bracket.place").value()));
    }

    // position (pitch)
    if (tupletSpan.attribute("startid")) {
        std::string refId = ExtractUuidFragment(tupletSpan.attribute("startid").value());
        start = dynamic_cast<LayerElement *>(measure->FindDescendantByUuid(refId));
        if (!start) {
            LogWarning("Element with @startid '%s' not found when trying to read the <tupletSpan>", refId.c_str());
        }
    }
    if (tupletSpan.attribute("endid")) {
        std::string refId = ExtractUuidFragment(tupletSpan.attribute("endid").value());
        end = dynamic_cast<LayerElement *>(measure->FindDescendantByUuid(refId));
        if (!end) {
            LogWarning("Element with @endid '%s' not found when trying to read the <tupletSpan>", refId.c_str());
        }
    }
    if (!start || !end) {
        delete tuplet;
        return false;
    }

    LayerElement *startChild = dynamic_cast<LayerElement *>(start->GetLastAncestorNot(LAYER));
    LayerElement *endChild = dynamic_cast<LayerElement *>(end->GetLastAncestorNot(LAYER));

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
    if (type == "facsimile") return Facs;
    LogWarning("Unknown layout type '%s'", type.c_str());
    return Raw;
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

void MeiInput::UpgradeFTremTo_4_0_0(pugi::xml_node fTrem, FTrem *vrvFTrem)
{
    if (fTrem.attribute("slash")) {
        vrvFTrem->SetBeams(vrvFTrem->AttFTremVis::StrToInt(fTrem.attribute("slash").value()));
        fTrem.remove_attribute("slash");
    }
}

void MeiInput::UpgradeMordentTo_4_0_0(pugi::xml_node mordent, Mordent *vrvMordent)
{
    if (mordent.attribute("form")) {
        std::string form = std::string(mordent.attribute("form").value());
        if (form == "norm") {
            vrvMordent->SetForm(mordentLog_FORM_lower);
        }
        else if (form == "inv") {
            vrvMordent->SetForm(mordentLog_FORM_upper);
        }
        else {
            LogWarning("Unsupported value '%s' for att.mordent.log@form (MEI 3.0)", form.c_str());
        }
        mordent.remove_attribute("form");
    }
}

void MeiInput::UpgradeScoreDefElementTo_4_0_0(pugi::xml_node scoreDefElement, ScoreDefElement *vrvScoreDefElement)
{
    KeySig *keySig = dynamic_cast<KeySig *>(vrvScoreDefElement->FindDescendantByType(KEYSIG));
    MeterSig *meterSig = dynamic_cast<MeterSig *>(vrvScoreDefElement->FindDescendantByType(METERSIG));

    if (scoreDefElement.attribute("key.sig.show")) {
        if (keySig) {
            keySig->SetVisible(keySig->AttVisibility::StrToBoolean(scoreDefElement.attribute("key.sig.show").value()));
            scoreDefElement.remove_attribute("key.sig.show");
        }
        else {
            LogWarning("No keySig found when trying to upgrade '@key.sig.show'");
        }
    }
    if (scoreDefElement.attribute("key.sig.showchange")) {
        if (keySig) {
            keySig->SetSigShowchange(
                keySig->AttKeySigVis::StrToBoolean(scoreDefElement.attribute("key.sig.showchange").value()));
            scoreDefElement.remove_attribute("key.sig.showchange");
        }
        else {
            LogWarning("No keySig found when trying to upgrade '@key.sig.showchange'");
        }
    }
    if (scoreDefElement.attribute("meter.rend")) {
        if (meterSig) {
            meterSig->SetForm(
                meterSig->AttMeterSigVis::StrToMeterform(scoreDefElement.attribute("meter.rend").value()));
            scoreDefElement.remove_attribute("meter.rend");
        }
    }
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
    if (staffGrp.attribute("barthru")) {
        vrvStaffGrp->SetBarThru(vrvStaffGrp->AttStaffGrpVis::StrToBoolean(staffGrp.attribute("barthru").value()));
        staffGrp.remove_attribute("barthru");
    }
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

void MeiInput::UpgradeTurnTo_4_0_0(pugi::xml_node turn, Turn *vrvTurn)
{
    if (turn.attribute("form")) {
        std::string form = std::string(turn.attribute("form").value());
        if (form == "inv") {
            vrvTurn->SetForm(turnLog_FORM_lower);
        }
        else if (form == "norm") {
            vrvTurn->SetForm(turnLog_FORM_lower);
        }
        else {
            LogWarning("Unsupported value '%s' for att.turn.log@form (MEI 3.0)", form.c_str());
        }
        turn.remove_attribute("form");
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

    Page *page = dynamic_cast<Page *>(system->GetFirstAncestor(PAGE));
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

    page->m_PPUFactor = (25.0 / 2.0 / doc->GetOptions()->m_unit.GetDefault());
    // LogDebug("PPUFactor: %f", m_PPUFactor);
}

bool MeiInput::ReadSurface(Facsimile *parent, pugi::xml_node surface)
{
    assert(parent);
    Surface *vrvSurface = new Surface();
    SetMeiUuid(surface, vrvSurface);
    vrvSurface->ReadCoordinated(surface);
    vrvSurface->ReadTyped(surface);

    for (pugi::xml_node child = surface.first_child(); child; child = child.next_sibling()) {
        if (strcmp(child.name(), "zone") == 0) {
            ReadZone(vrvSurface, child);
        }
        else {
            LogWarning("Unsupported element '%s' in <surface>", child.name());
        }
    }
    parent->AddChild(vrvSurface);
    return true;
}

bool MeiInput::ReadZone(Surface *parent, pugi::xml_node zone)
{
    assert(parent);
    Zone *vrvZone = new Zone();
    SetMeiUuid(zone, vrvZone);
    vrvZone->ReadCoordinated(zone);
    vrvZone->ReadTyped(zone);
    parent->AddChild(vrvZone);
    return true;
}

bool MeiInput::ReadFacsimile(Doc *doc, pugi::xml_node facsimile)
{
    assert(doc);
    Facsimile *vrvFacsimile = new Facsimile();
    // Read xmlId (if present)
    SetMeiUuid(facsimile, vrvFacsimile);
    // Read children
    for (pugi::xml_node child = facsimile.first_child(); child; child = child.next_sibling()) {
        if (strcmp(child.name(), "surface") == 0) {
            ReadSurface(vrvFacsimile, child);
        }
        else {
            LogWarning("Unsupported element '%s' in <facsimile>", child.name());
        }
    }
    doc->SetFacsimile(vrvFacsimile);
    return true;
}

} // namespace vrv
