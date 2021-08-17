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
#include "bracketspan.h"
#include "breath.h"
#include "btrem.h"
#include "caesura.h"
#include "choice.h"
#include "chord.h"
#include "clef.h"
#include "corr.h"
#include "course.h"
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
#include "expansionmap.h"
#include "f.h"
#include "fb.h"
#include "fermata.h"
#include "fig.h"
#include "fing.h"
#include "ftrem.h"
#include "functorparams.h"
#include "gliss.h"
#include "gracegrp.h"
#include "grpsym.h"
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
#include "lv.h"
#include "mdiv.h"
#include "measure.h"
#include "mensur.h"
#include "metersig.h"
#include "metersiggrp.h"
#include "mnum.h"
#include "mordent.h"
#include "mrest.h"
#include "mrpt.h"
#include "mrpt2.h"
#include "mspace.h"
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
#include "phrase.h"
#include "pitchinflection.h"
#include "plica.h"
#include "proport.h"
#include "rdg.h"
#include "ref.h"
#include "reg.h"
#include "reh.h"
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
#include "systemboundary.h"
#include "tabdursym.h"
#include "tabgrp.h"
#include "tempo.h"
#include "text.h"
#include "tie.h"
#include "trill.h"
#include "tuning.h"
#include "tuplet.h"
#include "turn.h"
#include "unclear.h"
#include "verse.h"
#include "vrv.h"
#include "zone.h"

namespace vrv {

const std::vector<std::string> MEIInput::s_editorialElementNames = { "abbr", "add", "app", "annot", "choice", "corr",
    "damage", "del", "expan", "orig", "ref", "reg", "restore", "sic", "subst", "supplied", "unclear" };

//----------------------------------------------------------------------------
// MEIOutput
//----------------------------------------------------------------------------

MEIOutput::MEIOutput(Doc *doc) : Output(doc)
{
    m_page = -1;
    m_indent = 5;
    m_scoreBasedMEI = false;
    m_removeIds = false;
}

MEIOutput::~MEIOutput() {}

bool MEIOutput::Export()
{

    if (m_removeIds) {
        FindAllReferencedObjectsParams findAllReferencedObjectsParams(&m_referredObjects);
        Functor findAllReferencedObjects(&Object::FindAllReferencedObjects);
        m_doc->Process(&findAllReferencedObjects, &findAllReferencedObjectsParams);
        m_referredObjects.unique();
    }

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
            m_doc->ConvertToCastOffMensuralDoc(false);

            // this starts the call of all the functors
            m_doc->Save(this);

            // Redo the mensural segment cast of if necessary
            m_doc->ConvertToCastOffMensuralDoc(true);
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
                m_doc->GetCurrentScoreDef()->Save(this);
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
        if (m_doc->GetOptions()->m_outputFormatRaw.GetValue()) {
            output_flags |= pugi::format_raw;
        }

        std::string indent = (m_indent == -1) ? "\t" : std::string(m_indent, ' ');
        meiDoc.save(m_streamStringOutput, indent.c_str(), output_flags);
    }
    catch (char *str) {
        LogError("%s", str);
        return false;
    }

    return true;
}

std::string MEIOutput::GetOutput(int page)
{
    m_page = page;
    this->Export();
    m_page = -1;

    return m_streamStringOutput.str();
}

bool MEIOutput::WriteObject(Object *object)
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
        // else {
        //    m_currentNode = m_currentNode.append_child("score");
        //}
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
    else if (object->Is(GRPSYM)) {
        if (!object->IsAttribute()) m_currentNode = m_currentNode.append_child("grpSym");
        WriteGrpSym(m_currentNode, dynamic_cast<GrpSym *>(object));
    }
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
    else if (object->Is(METERSIGGRP)) {
        m_currentNode = m_currentNode.append_child("meterSigGrp");
        WriteMeterSigGrp(m_currentNode, dynamic_cast<MeterSigGrp *>(object));
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
    else if (object->Is(TUNING)) {
        m_currentNode = m_currentNode.append_child("tuning");
        WriteTuning(m_currentNode, dynamic_cast<Tuning *>(object));
    }
    else if (object->Is(COURSE)) {
        m_currentNode = m_currentNode.append_child("course");
        WriteCourse(m_currentNode, dynamic_cast<Course *>(object));
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
    else if (object->Is(ANCHOREDTEXT)) {
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
    else if (object->Is(CAESURA)) {
        m_currentNode = m_currentNode.append_child("caesura");
        WriteCaesura(m_currentNode, dynamic_cast<Caesura *>(object));
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
    else if (object->Is(FING)) {
        m_currentNode = m_currentNode.append_child("fing");
        WriteFing(m_currentNode, dynamic_cast<Fing *>(object));
    }
    else if (object->Is(HAIRPIN)) {
        m_currentNode = m_currentNode.append_child("hairpin");
        WriteHairpin(m_currentNode, dynamic_cast<Hairpin *>(object));
    }
    else if (object->Is(HARM)) {
        m_currentNode = m_currentNode.append_child("harm");
        WriteHarm(m_currentNode, dynamic_cast<Harm *>(object));
    }
    else if (object->Is(LV)) {
        m_currentNode = m_currentNode.append_child("lv");
        WriteLv(m_currentNode, dynamic_cast<Lv *>(object));
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
    else if (object->Is(PHRASE)) {
        m_currentNode = m_currentNode.append_child("phrase");
        WritePhrase(m_currentNode, dynamic_cast<Phrase *>(object));
    }

    else if (object->Is(PITCHINFLECTION)) {
        m_currentNode = m_currentNode.append_child("pitchInfection");
        WritePitchInflection(m_currentNode, dynamic_cast<PitchInflection *>(object));
    }
    else if (object->Is(REH)) {
        m_currentNode = m_currentNode.append_child("reh");
        WriteReh(m_currentNode, dynamic_cast<Reh *>(object));
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
    else if (object->Is(GLISS)) {
        m_currentNode = m_currentNode.append_child("gliss");
        WriteGliss(m_currentNode, dynamic_cast<Gliss *>(object));
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
    else if (object->Is(MSPACE)) {
        m_currentNode = m_currentNode.append_child("mSpace");
        WriteMSpace(m_currentNode, dynamic_cast<MSpace *>(object));
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
    else if (object->Is(PLICA)) {
        m_currentNode = m_currentNode.append_child("plica");
        WritePlica(m_currentNode, dynamic_cast<Plica *>(object));
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
    else if (object->Is(TABDURSYM)) {
        m_currentNode = m_currentNode.append_child("tabDurSym");
        WriteTabDurSym(m_currentNode, dynamic_cast<TabDurSym *>(object));
    }
    else if (object->Is(TABGRP)) {
        m_currentNode = m_currentNode.append_child("tabGrp");
        WriteTabGrp(m_currentNode, dynamic_cast<TabGrp *>(object));
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

    // SystemElementEnd - nothing to add - only
    else if (object->Is(SYSTEM_ELEMENT_END)) {
        if (m_scoreBasedMEI) {
            // LogDebug("No piling '%s'", object->GetClassName().c_str());
            return true;
        }
        else {
            m_currentNode = m_currentNode.append_child("systemElementEnd");
            WriteSystemElementEnd(m_currentNode, dynamic_cast<SystemElementEnd *>(object));
        }
    }
    // SystemElementEnd - nothing to add - only
    else if (object->Is(PAGE_ELEMENT_END)) {
        if (m_scoreBasedMEI) {
            // LogDebug("No piling '%s'", object->GetClassName().c_str());
            return true;
        }
        else {
            m_currentNode = m_currentNode.append_child("pageElementEnd");
            // WritePageElementEnd(m_currentNode, dynamic_cast<PageElementEnd *>(object));
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
        // m_doc->GetCurrentScoreDef()->Save(this);
    }
    else if (object->Is(SCORE)) {
        // First save the main scoreDef
        m_doc->GetCurrentScoreDef()->Save(this);
    }

    WriteUnsupportedAttr(m_currentNode, object);

    return true;
}

bool MEIOutput::WriteObjectEnd(Object *object)
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
    else if (m_scoreBasedMEI && (object->Is(PAGES))) {
        return true;
    }

    if (object->HasClosingComment()) {
        m_currentNode.append_child(pugi::node_comment).set_value(object->GetClosingComment().c_str());
    }

    m_nodeStack.pop_back();
    m_currentNode = m_nodeStack.back();

    return true;
}

std::string MEIOutput::UuidToMeiStr(Object *element)
{
    std::string out = element->GetUuid();
    // LogDebug("uuid: %s", out.c_str());
    return out;
}

void MEIOutput::WriteXmlId(pugi::xml_node currentNode, Object *object)
{
    if (m_removeIds) {
        ListOfObjects::iterator it = std::find(m_referredObjects.begin(), m_referredObjects.end(), object);
        if (it == m_referredObjects.end()) return;
        m_referredObjects.erase(it);
    }
    currentNode.append_attribute("xml:id") = UuidToMeiStr(object).c_str();
}

bool MEIOutput::WriteDoc(Doc *doc)
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

void MEIOutput::WriteMdiv(pugi::xml_node currentNode, Mdiv *mdiv)
{
    assert(mdiv);

    WriteXmlId(currentNode, mdiv);
    mdiv->WriteLabelled(currentNode);
    mdiv->WriteNNumberLike(currentNode);
}

void MEIOutput::WritePages(pugi::xml_node currentNode, Pages *pages)
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

void MEIOutput::WriteScore(pugi::xml_node currentNode, Score *score)
{
    assert(score);

    WriteXmlId(currentNode, score);
    score->WriteLabelled(currentNode);
    score->WriteNNumberLike(currentNode);
}

void MEIOutput::WritePage(pugi::xml_node currentNode, Page *page)
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

void MEIOutput::WritePageElement(pugi::xml_node currentNode, PageElement *pageElement)
{
    assert(pageElement);

    WriteXmlId(currentNode, pageElement);
    pageElement->WriteTyped(currentNode);
}

void MEIOutput::WritePageElementEnd(pugi::xml_node currentNode, PageElementEnd *elementEnd)
{
    assert(elementEnd && elementEnd->GetStart());

    WritePageElement(currentNode, elementEnd);
    currentNode.append_attribute("startid") = UuidToMeiStr(elementEnd->GetStart()).c_str();
    std::string meiElementName = elementEnd->GetStart()->GetClassName();
    std::transform(meiElementName.begin(), meiElementName.begin() + 1, meiElementName.begin(), ::tolower);
    currentNode.append_attribute("type") = meiElementName.c_str();
}

void MEIOutput::WriteSystem(pugi::xml_node currentNode, System *system)
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

void MEIOutput::WriteSystemElement(pugi::xml_node currentNode, SystemElement *systemElement)
{
    assert(systemElement);

    WriteXmlId(currentNode, systemElement);
    systemElement->WriteTyped(currentNode);
}

void MEIOutput::WriteSystemElementEnd(pugi::xml_node currentNode, SystemElementEnd *elementEnd)
{
    assert(elementEnd && elementEnd->GetStart());

    WriteSystemElement(currentNode, elementEnd);
    currentNode.append_attribute("startid") = UuidToMeiStr(elementEnd->GetStart()).c_str();
    std::string meiElementName = elementEnd->GetStart()->GetClassName();
    std::transform(meiElementName.begin(), meiElementName.begin() + 1, meiElementName.begin(), ::tolower);
    currentNode.append_attribute("type") = meiElementName.c_str();
}

void MEIOutput::WriteSection(pugi::xml_node currentNode, Section *section)
{
    assert(section);

    WriteSystemElement(currentNode, section);
    section->WriteNNumberLike(currentNode);
    section->WriteSectionVis(currentNode);
}

void MEIOutput::WriteEnding(pugi::xml_node currentNode, Ending *ending)
{
    assert(ending);

    WriteSystemElement(currentNode, ending);
    ending->WriteLineRend(currentNode);
    ending->WriteNNumberLike(currentNode);
}

void MEIOutput::WriteExpansion(pugi::xml_node currentNode, Expansion *expansion)
{
    assert(expansion);

    WritePlistInterface(currentNode, expansion);
    WriteSystemElement(currentNode, expansion);
}

void MEIOutput::WritePb(pugi::xml_node currentNode, Pb *pb)
{
    assert(pb);

    WriteSystemElement(currentNode, pb);
    pb->WriteNNumberLike(currentNode);
}

void MEIOutput::WriteSb(pugi::xml_node currentNode, Sb *sb)
{
    assert(sb);

    WriteSystemElement(currentNode, sb);
    sb->WriteNNumberLike(currentNode);
}

void MEIOutput::WriteScoreDefElement(pugi::xml_node currentNode, ScoreDefElement *scoreDefElement)
{
    assert(scoreDefElement);

    WriteXmlId(currentNode, scoreDefElement);
    scoreDefElement->WriteMeasureNumbers(currentNode);
    scoreDefElement->WriteSpacing(currentNode);
    scoreDefElement->WriteSystems(currentNode);
    scoreDefElement->WriteTyped(currentNode);
}

void MEIOutput::WriteScoreDef(pugi::xml_node currentNode, ScoreDef *scoreDef)
{
    assert(scoreDef);

    WriteScoreDefElement(currentNode, scoreDef);
    WriteScoreDefInterface(currentNode, scoreDef);
    scoreDef->WriteDistances(currentNode);
    scoreDef->WriteEndings(currentNode);
    scoreDef->WriteOptimization(currentNode);
    scoreDef->WriteTimeBase(currentNode);
}

void MEIOutput::WriteRunningElement(pugi::xml_node currentNode, RunningElement *runningElement)
{
    assert(runningElement);

    WriteXmlId(currentNode, runningElement);
    runningElement->WriteHorizontalAlign(currentNode);
    runningElement->WriteTyped(currentNode);
}

void MEIOutput::WriteGrpSym(pugi::xml_node currentNode, GrpSym *grpSym)
{
    assert(grpSym);

    // Only write att.symbol if representing an attribute
    if (grpSym->IsAttribute()) {
        grpSym->WriteStaffGroupingSym(currentNode);
        return;
    }

    WriteXmlId(currentNode, grpSym);
    grpSym->WriteColor(currentNode);
    grpSym->WriteGrpSymLog(currentNode);
    grpSym->WriteStaffGroupingSym(currentNode);
    grpSym->WriteStartId(currentNode);
    grpSym->WriteStartEndId(currentNode);
}

void MEIOutput::WritePgFoot(pugi::xml_node currentNode, PgFoot *pgFoot)
{
    assert(pgFoot);

    WriteRunningElement(currentNode, pgFoot);
}

void MEIOutput::WritePgFoot2(pugi::xml_node currentNode, PgFoot2 *pgFoot2)
{
    assert(pgFoot2);

    WriteRunningElement(currentNode, pgFoot2);
}

void MEIOutput::WritePgHead(pugi::xml_node currentNode, PgHead *pgHead)
{
    assert(pgHead);

    WriteRunningElement(currentNode, pgHead);
}

void MEIOutput::WritePgHead2(pugi::xml_node currentNode, PgHead2 *pgHead2)
{
    assert(pgHead2);

    WriteRunningElement(currentNode, pgHead2);
}

void MEIOutput::WriteStaffGrp(pugi::xml_node currentNode, StaffGrp *staffGrp)
{
    assert(staffGrp);

    WriteXmlId(currentNode, staffGrp);
    staffGrp->WriteBasic(currentNode);
    staffGrp->WriteLabelled(currentNode);
    staffGrp->WriteNNumberLike(currentNode);
    staffGrp->WriteStaffGroupingSym(currentNode);
    staffGrp->WriteStaffGrpVis(currentNode);
    staffGrp->WriteTyped(currentNode);
}

void MEIOutput::WriteStaffDef(pugi::xml_node currentNode, StaffDef *staffDef)
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

void MEIOutput::WriteInstrDef(pugi::xml_node currentNode, InstrDef *instrDef)
{
    assert(instrDef);

    WriteXmlId(currentNode, instrDef);
    instrDef->WriteChannelized(currentNode);
    instrDef->WriteLabelled(currentNode);
    instrDef->WriteMidiInstrument(currentNode);
    instrDef->WriteNNumberLike(currentNode);
}

void MEIOutput::WriteLabel(pugi::xml_node currentNode, Label *label)
{
    assert(label);

    WriteXmlId(currentNode, label);
}

void MEIOutput::WriteLabelAbbr(pugi::xml_node currentNode, LabelAbbr *labelAbbr)
{
    assert(labelAbbr);

    WriteXmlId(currentNode, labelAbbr);
}

void MEIOutput::WriteTuning(pugi::xml_node currentNode, Tuning *tuning)
{
    assert(tuning);

    WriteXmlId(currentNode, tuning);
    tuning->WriteCourseLog(currentNode);
}

void MEIOutput::WriteCourse(pugi::xml_node currentNode, Course *course)
{
    assert(course);

    WriteXmlId(currentNode, course);
    course->WriteNNumberLike(currentNode);
}

void MEIOutput::WriteMeasure(pugi::xml_node currentNode, Measure *measure)
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

void MEIOutput::WriteMeterSigGrp(pugi::xml_node currentNode, MeterSigGrp *meterSigGrp)
{
    assert(meterSigGrp);

    WriteXmlId(currentNode, meterSigGrp);
    WriteLinkingInterface(currentNode, meterSigGrp);
    meterSigGrp->WriteBasic(currentNode);
    meterSigGrp->WriteLabelled(currentNode);
    meterSigGrp->WriteMeterSigGrpLog(currentNode);
    meterSigGrp->WriteTyped(currentNode);
}

void MEIOutput::WriteFb(pugi::xml_node currentNode, Fb *fb)
{
    assert(fb);

    WriteXmlId(currentNode, fb);
}

void MEIOutput::WriteControlElement(pugi::xml_node currentNode, ControlElement *controlElement)
{
    assert(controlElement);

    WriteXmlId(currentNode, controlElement);
    WriteLinkingInterface(currentNode, controlElement);
    controlElement->WriteLabelled(currentNode);
    controlElement->WriteTyped(currentNode);
}

void MEIOutput::WriteAnchoredText(pugi::xml_node currentNode, AnchoredText *anchoredText)
{
    assert(anchoredText);

    WriteControlElement(currentNode, anchoredText);
    WriteTextDirInterface(currentNode, anchoredText);
}

void MEIOutput::WriteArpeg(pugi::xml_node currentNode, Arpeg *arpeg)
{
    assert(arpeg);

    WriteControlElement(currentNode, arpeg);
    WritePlistInterface(currentNode, arpeg);
    WriteTimePointInterface(currentNode, arpeg);
    arpeg->WriteArpegLog(currentNode);
    arpeg->WriteArpegVis(currentNode);
    arpeg->WriteColor(currentNode);
}

void MEIOutput::WriteBracketSpan(pugi::xml_node currentNode, BracketSpan *bracketSpan)
{
    assert(bracketSpan);

    WriteControlElement(currentNode, bracketSpan);
    WriteTimeSpanningInterface(currentNode, bracketSpan);
    bracketSpan->WriteBracketSpanLog(currentNode);
    bracketSpan->WriteColor(currentNode);
    bracketSpan->WriteLineRend(currentNode);
    bracketSpan->WriteLineRendBase(currentNode);
}

void MEIOutput::WriteBreath(pugi::xml_node currentNode, Breath *breath)
{
    assert(breath);

    WriteControlElement(currentNode, breath);
    WriteTimePointInterface(currentNode, breath);
    breath->WriteColor(currentNode);
    breath->WritePlacementRelStaff(currentNode);
}

void MEIOutput::WriteCaesura(pugi::xml_node currentNode, Caesura *caesura)
{
    assert(caesura);

    WriteControlElement(currentNode, caesura);
    WriteTimePointInterface(currentNode, caesura);
    caesura->WriteColor(currentNode);
    caesura->WritePlacementRelStaff(currentNode);
}

void MEIOutput::WriteDir(pugi::xml_node currentNode, Dir *dir)
{
    assert(dir);

    WriteControlElement(currentNode, dir);
    WriteTextDirInterface(currentNode, dir);
    WriteTimeSpanningInterface(currentNode, dir);
    dir->WriteLang(currentNode);
    dir->WriteLineRendBase(currentNode);
    dir->WriteExtender(currentNode);
    dir->WriteVerticalGroup(currentNode);
}

void MEIOutput::WriteDynam(pugi::xml_node currentNode, Dynam *dynam)
{
    assert(dynam);

    WriteControlElement(currentNode, dynam);
    WriteTextDirInterface(currentNode, dynam);
    WriteTimeSpanningInterface(currentNode, dynam);
    dynam->WriteExtender(currentNode);
    dynam->WriteLineRendBase(currentNode);
    dynam->WriteMidiValue(currentNode);
    dynam->WriteMidiValue2(currentNode);
    dynam->WriteVerticalGroup(currentNode);
}

void MEIOutput::WriteFermata(pugi::xml_node currentNode, Fermata *fermata)
{
    assert(fermata);

    WriteControlElement(currentNode, fermata);
    WriteTimePointInterface(currentNode, fermata);
    fermata->WriteColor(currentNode);
    fermata->WriteEnclosingChars(currentNode);
    fermata->WriteExtSym(currentNode);
    fermata->WriteFermataVis(currentNode);
    fermata->WritePlacementRelStaff(currentNode);
}

void MEIOutput::WriteFing(pugi::xml_node currentNode, Fing *fing)
{
    assert(fing);

    WriteControlElement(currentNode, fing);
    WriteTextDirInterface(currentNode, fing);
    WriteTimePointInterface(currentNode, fing);
    fing->WriteNNumberLike(currentNode);
}

void MEIOutput::WriteGliss(pugi::xml_node currentNode, Gliss *gliss)
{
    assert(gliss);

    WriteControlElement(currentNode, gliss);
    WriteTimeSpanningInterface(currentNode, gliss);
    gliss->WriteColor(currentNode);
    gliss->WriteLineRend(currentNode);
    gliss->WriteLineRendBase(currentNode);
    gliss->WriteNNumberLike(currentNode);
}

void MEIOutput::WriteHairpin(pugi::xml_node currentNode, Hairpin *hairpin)
{
    assert(hairpin);

    WriteControlElement(currentNode, hairpin);
    WriteTimeSpanningInterface(currentNode, hairpin);
    hairpin->WriteColor(currentNode);
    hairpin->WriteHairpinLog(currentNode);
    hairpin->WriteHairpinVis(currentNode);
    hairpin->WritePlacementRelStaff(currentNode);
    hairpin->WriteVerticalGroup(currentNode);
}

void MEIOutput::WriteHarm(pugi::xml_node currentNode, Harm *harm)
{
    assert(harm);

    WriteControlElement(currentNode, harm);
    WriteTextDirInterface(currentNode, harm);
    WriteTimeSpanningInterface(currentNode, harm);
    harm->WriteLang(currentNode);
    harm->WriteNNumberLike(currentNode);
}

void MEIOutput::WriteLv(pugi::xml_node currentNode, Lv *lv)
{
    assert(lv);

    WriteControlElement(currentNode, lv);
    WriteTimeSpanningInterface(currentNode, lv);
    lv->WriteColor(currentNode);
    lv->WriteCurvature(currentNode);
    lv->WriteCurveRend(currentNode);
}

void MEIOutput::WriteMNum(pugi::xml_node currentNode, MNum *mNum)
{
    assert(mNum);

    WriteControlElement(currentNode, mNum);
    WriteTextDirInterface(currentNode, mNum);
    WriteTimePointInterface(currentNode, mNum);
    mNum->WriteColor(currentNode);
    mNum->WriteLang(currentNode);
    mNum->WriteTypography(currentNode);
}

void MEIOutput::WriteMordent(pugi::xml_node currentNode, Mordent *mordent)
{
    assert(mordent);

    WriteControlElement(currentNode, mordent);
    WriteTimePointInterface(currentNode, mordent);
    mordent->WriteColor(currentNode);
    mordent->WriteExtSym(currentNode);
    mordent->WriteOrnamentAccid(currentNode);
    mordent->WritePlacementRelStaff(currentNode);
    mordent->WriteMordentLog(currentNode);
}

void MEIOutput::WriteOctave(pugi::xml_node currentNode, Octave *octave)
{
    assert(octave);

    WriteControlElement(currentNode, octave);
    WriteTimeSpanningInterface(currentNode, octave);
    octave->WriteColor(currentNode);
    octave->WriteExtender(currentNode);
    octave->WriteLineRend(currentNode);
    octave->WriteLineRendBase(currentNode);
    octave->WriteNNumberLike(currentNode);
    octave->WriteOctaveDisplacement(currentNode);
}

void MEIOutput::WritePedal(pugi::xml_node currentNode, Pedal *pedal)
{
    assert(pedal);

    WriteControlElement(currentNode, pedal);
    WriteTimeSpanningInterface(currentNode, pedal);
    pedal->WriteColor(currentNode);
    pedal->WriteExtSym(currentNode);
    pedal->WritePedalLog(currentNode);
    pedal->WritePedalVis(currentNode);
    pedal->WritePlacementRelStaff(currentNode);
    pedal->WriteVerticalGroup(currentNode);
}

void MEIOutput::WritePhrase(pugi::xml_node currentNode, Phrase *phrase)
{
    assert(phrase);

    WriteSlur(currentNode, phrase);
}

void MEIOutput::WritePitchInflection(pugi::xml_node currentNode, PitchInflection *pitchInflection)
{
    assert(pitchInflection);

    WriteControlElement(currentNode, pitchInflection);
    WriteTimeSpanningInterface(currentNode, pitchInflection);
}

void MEIOutput::WriteReh(pugi::xml_node currentNode, Reh *reh)
{
    assert(reh);

    WriteControlElement(currentNode, reh);
    WriteTextDirInterface(currentNode, reh);
    WriteTimePointInterface(currentNode, reh);
    reh->WriteColor(currentNode);
    reh->WriteLang(currentNode);
    reh->WriteVerticalGroup(currentNode);
}

void MEIOutput::WriteSlur(pugi::xml_node currentNode, Slur *slur)
{
    assert(slur);

    WriteControlElement(currentNode, slur);
    WriteTimeSpanningInterface(currentNode, slur);
    slur->WriteColor(currentNode);
    slur->WriteCurvature(currentNode);
    slur->WriteCurveRend(currentNode);
}

void MEIOutput::WriteStaff(pugi::xml_node currentNode, Staff *staff)
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

void MEIOutput::WriteTempo(pugi::xml_node currentNode, Tempo *tempo)
{
    assert(tempo);

    WriteControlElement(currentNode, tempo);
    WriteTextDirInterface(currentNode, tempo);
    WriteTimePointInterface(currentNode, tempo);
    tempo->WriteLang(currentNode);
    tempo->WriteMidiTempo(currentNode);
    tempo->WriteMmTempo(currentNode);
}

void MEIOutput::WriteTie(pugi::xml_node currentNode, Tie *tie)
{
    assert(tie);

    WriteControlElement(currentNode, tie);
    WriteTimeSpanningInterface(currentNode, tie);
    tie->WriteColor(currentNode);
    tie->WriteCurvature(currentNode);
    tie->WriteCurveRend(currentNode);
}

void MEIOutput::WriteTrill(pugi::xml_node currentNode, Trill *trill)
{
    assert(trill);

    WriteControlElement(currentNode, trill);
    WriteTimeSpanningInterface(currentNode, trill);
    trill->WriteColor(currentNode);
    trill->WriteExtender(currentNode);
    trill->WriteExtSym(currentNode);
    trill->WriteLineRend(currentNode);
    trill->WriteNNumberLike(currentNode);
    trill->WriteOrnamentAccid(currentNode);
    trill->WritePlacementRelStaff(currentNode);
}

void MEIOutput::WriteTurn(pugi::xml_node currentNode, Turn *turn)
{
    assert(turn);

    WriteControlElement(currentNode, turn);
    WriteTimePointInterface(currentNode, turn);
    turn->WriteColor(currentNode);
    turn->WriteExtSym(currentNode);
    turn->WriteOrnamentAccid(currentNode);
    turn->WritePlacementRelStaff(currentNode);
    turn->WriteTurnLog(currentNode);
}

void MEIOutput::WriteLayer(pugi::xml_node currentNode, Layer *layer)
{
    assert(layer);

    WriteXmlId(currentNode, layer);
    layer->WriteNInteger(currentNode);
    layer->WriteTyped(currentNode);
    layer->WriteVisibility(currentNode);
}

void MEIOutput::WriteLayerElement(pugi::xml_node currentNode, LayerElement *element)
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

void MEIOutput::WriteAccid(pugi::xml_node currentNode, Accid *accid)
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
    accid->WriteExtSym(currentNode);
}

void MEIOutput::WriteArtic(pugi::xml_node currentNode, Artic *artic)
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
    artic->WriteEnclosingChars(currentNode);
    artic->WriteExtSym(currentNode);
    artic->WritePlacementRelEvent(currentNode);
}

void MEIOutput::WriteBarLine(pugi::xml_node currentNode, BarLine *barLine)
{
    assert(barLine);

    WriteLayerElement(currentNode, barLine);
    barLine->WriteBarLineLog(currentNode);
    barLine->WriteColor(currentNode);
    barLine->WriteNNumberLike(currentNode);
    barLine->WriteVisibility(currentNode);
}

void MEIOutput::WriteBeam(pugi::xml_node currentNode, Beam *beam)
{
    assert(beam);

    WriteLayerElement(currentNode, beam);
    beam->WriteColor(currentNode);
    beam->WriteBeamedWith(currentNode);
    beam->WriteBeamRend(currentNode);
}

void MEIOutput::WriteBeatRpt(pugi::xml_node currentNode, BeatRpt *beatRpt)
{
    assert(beatRpt);

    WriteLayerElement(currentNode, beatRpt);
    beatRpt->WriteColor(currentNode);
    beatRpt->WriteBeatRptVis(currentNode);
}

void MEIOutput::WriteBTrem(pugi::xml_node currentNode, BTrem *bTrem)
{
    assert(bTrem);

    WriteLayerElement(currentNode, bTrem);
    bTrem->WriteBTremLog(currentNode);
    bTrem->WriteTremMeasured(currentNode);
}

void MEIOutput::WriteChord(pugi::xml_node currentNode, Chord *chord)
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

void MEIOutput::WriteClef(pugi::xml_node currentNode, Clef *clef)
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
        cleffingVis.SetClefVisible(clef->GetVisible());
        cleffingVis.WriteCleffingVis(currentNode);
        return;
    }

    WriteLayerElement(currentNode, clef);
    WriteFacsimileInterface(currentNode, clef);
    clef->WriteClefShape(currentNode);
    clef->WriteColor(currentNode);
    clef->WriteExtSym(currentNode);
    clef->WriteLineLoc(currentNode);
    clef->WriteOctaveDisplacement(currentNode);
    clef->WriteVisibility(currentNode);
}

void MEIOutput::WriteCustos(pugi::xml_node currentNode, Custos *custos)
{
    assert(custos);

    WriteFacsimileInterface(currentNode, custos);
    WritePitchInterface(currentNode, custos);
    WritePositionInterface(currentNode, custos);
    WriteLayerElement(currentNode, custos);
    custos->WriteColor(currentNode);
}

void MEIOutput::WriteDot(pugi::xml_node currentNode, Dot *dot)
{
    assert(dot);

    WriteLayerElement(currentNode, dot);
    WritePositionInterface(currentNode, dot);
    dot->WriteColor(currentNode);
    dot->WriteDotLog(currentNode);
}

void MEIOutput::WriteFTrem(pugi::xml_node currentNode, FTrem *fTrem)
{
    assert(fTrem);

    WriteLayerElement(currentNode, fTrem);
    fTrem->WriteFTremVis(currentNode);
    fTrem->WriteTremMeasured(currentNode);
}

void MEIOutput::WriteGraceGrp(pugi::xml_node currentNode, GraceGrp *graceGrp)
{
    assert(graceGrp);

    WriteLayerElement(currentNode, graceGrp);
    graceGrp->WriteColor(currentNode);
    graceGrp->WriteGraced(currentNode);
    graceGrp->WriteGraceGrpLog(currentNode);
}

void MEIOutput::WriteHalfmRpt(pugi::xml_node currentNode, HalfmRpt *halfmRpt)
{
    assert(halfmRpt);

    WriteLayerElement(currentNode, halfmRpt);
    halfmRpt->WriteColor(currentNode);
}

void MEIOutput::WriteKeyAccid(pugi::xml_node currentNode, KeyAccid *keyAccid)
{
    assert(keyAccid);

    WriteLayerElement(currentNode, keyAccid);
    WritePitchInterface(currentNode, keyAccid);
    keyAccid->WriteAccidental(currentNode);
    keyAccid->WriteColor(currentNode);
    keyAccid->WriteEnclosingChars(currentNode);
}

void MEIOutput::WriteKeySig(pugi::xml_node currentNode, KeySig *keySig)
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

void MEIOutput::WriteLigature(pugi::xml_node currentNode, Ligature *ligature)
{
    assert(ligature);

    WriteLayerElement(currentNode, ligature);
    ligature->WriteLigatureVis(currentNode);
}

void MEIOutput::WriteMensur(pugi::xml_node currentNode, Mensur *mensur)
{
    assert(mensur);

    if (mensur->IsAttribute()) {
        AttMensuralLog mensuralLog;

        mensuralLog.SetProportNum(mensur->GetNum());
        mensuralLog.SetProportNumbase(mensur->GetNumbase());
        mensuralLog.WriteMensuralLog(currentNode);
        AttMensuralShared mensuralShared;
        mensuralShared.SetModusmaior(mensur->GetModusmaior());
        mensuralShared.SetModusminor(mensur->GetModusminor());
        mensuralShared.SetProlatio(mensur->GetProlatio());
        mensuralShared.SetTempus(mensur->GetTempus());
        mensuralShared.WriteMensuralShared(currentNode);
        AttMensuralVis mensuralVis;
        mensuralVis.SetMensurDot(mensur->GetDot());
        mensuralVis.SetMensurColor(mensur->GetColor());
        mensuralVis.SetMensurOrient(mensur->GetOrient());
        mensuralVis.SetMensurSign(mensur->GetSign());
        mensuralVis.SetMensurSlash(mensur->GetSlash());
        mensuralVis.WriteMensuralVis(currentNode);
        return;
    }

    WriteLayerElement(currentNode, mensur);
    mensur->WriteColor(currentNode);
    mensur->WriteCue(currentNode);
    mensur->WriteDurationRatio(currentNode);
    mensur->WriteMensuralShared(currentNode);
    mensur->WriteMensurVis(currentNode);
    mensur->WriteSlashCount(currentNode);
    mensur->WriteStaffLoc(currentNode);
}

void MEIOutput::WriteMeterSig(pugi::xml_node currentNode, MeterSig *meterSig)
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

void MEIOutput::WriteMRest(pugi::xml_node currentNode, MRest *mRest)
{
    assert(mRest);

    WriteLayerElement(currentNode, mRest);
    WritePositionInterface(currentNode, mRest);
    mRest->WriteColor(currentNode);
    mRest->WriteCue(currentNode);
    mRest->WriteFermataPresent(currentNode);
    mRest->WriteVisibility(currentNode);
}

void MEIOutput::WriteMRpt(pugi::xml_node currentNode, MRpt *mRpt)
{
    assert(mRpt);

    WriteLayerElement(currentNode, mRpt);
    mRpt->WriteColor(currentNode);
}

void MEIOutput::WriteMRpt2(pugi::xml_node currentNode, MRpt2 *mRpt2)
{
    assert(mRpt2);

    WriteLayerElement(currentNode, mRpt2);
    mRpt2->WriteColor(currentNode);
}

void MEIOutput::WriteMSpace(pugi::xml_node currentNode, MSpace *mSpace)
{
    assert(mSpace);

    WriteLayerElement(currentNode, mSpace);
}

void MEIOutput::WriteMultiRest(pugi::xml_node currentNode, MultiRest *multiRest)
{
    assert(multiRest);

    WriteLayerElement(currentNode, multiRest);
    WritePositionInterface(currentNode, multiRest);
    multiRest->WriteColor(currentNode);
    multiRest->WriteMultiRestVis(currentNode);
    multiRest->WriteNumbered(currentNode);
    multiRest->WriteWidth(currentNode);
}

void MEIOutput::WriteMultiRpt(pugi::xml_node currentNode, MultiRpt *multiRpt)
{
    assert(multiRpt);

    WriteLayerElement(currentNode, multiRpt);
    multiRpt->WriteNumbered(currentNode);
}

void MEIOutput::WriteNc(pugi::xml_node currentNode, Nc *nc)
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

void MEIOutput::WriteNeume(pugi::xml_node currentNode, Neume *neume)
{
    assert(neume);

    WriteLayerElement(currentNode, neume);
    WriteFacsimileInterface(currentNode, neume);
    neume->WriteColor(currentNode);
}

void MEIOutput::WriteNote(pugi::xml_node currentNode, Note *note)
{
    assert(note);

    WriteLayerElement(currentNode, note);
    WriteDurationInterface(currentNode, note);
    WritePitchInterface(currentNode, note);
    WritePositionInterface(currentNode, note);
    note->WriteColor(currentNode);
    note->WriteColoration(currentNode);
    note->WriteCue(currentNode);
    note->WriteExtSym(currentNode);
    note->WriteGraced(currentNode);
    note->WriteMidiVelocity(currentNode);
    note->WriteNoteGesTab(currentNode);
    note->WriteNoteHeads(currentNode);
    note->WriteNoteVisMensural(currentNode);
    note->WriteStems(currentNode);
    note->WriteStemsCmn(currentNode);
    note->WriteTiePresent(currentNode);
    note->WriteVisibility(currentNode);
}

void MEIOutput::WriteRest(pugi::xml_node currentNode, Rest *rest)
{
    assert(rest);

    WriteLayerElement(currentNode, rest);
    WriteDurationInterface(currentNode, rest);
    WritePositionInterface(currentNode, rest);
    rest->WriteColor(currentNode);
    rest->WriteCue(currentNode);
    rest->WriteExtSym(currentNode);
    rest->WriteRestVisMensural(currentNode);
}

void MEIOutput::WritePlica(pugi::xml_node currentNode, Plica *plica)
{
    assert(plica);

    WriteLayerElement(currentNode, plica);
    plica->WritePlicaVis(currentNode);
}

void MEIOutput::WriteProport(pugi::xml_node currentNode, Proport *proport)
{
    assert(proport);

    WriteLayerElement(currentNode, proport);
}

void MEIOutput::WriteSpace(pugi::xml_node currentNode, Space *space)
{
    assert(space);

    WriteLayerElement(currentNode, space);
    WriteDurationInterface(currentNode, space);
}

void MEIOutput::WriteTabDurSym(pugi::xml_node currentNode, TabDurSym *tabDurSym)
{
    assert(tabDurSym);

    WriteLayerElement(currentNode, tabDurSym);
    tabDurSym->WriteNNumberLike(currentNode);
}

void MEIOutput::WriteTabGrp(pugi::xml_node currentNode, TabGrp *tabGrp)
{
    assert(tabGrp);

    WriteLayerElement(currentNode, tabGrp);
    WriteDurationInterface(currentNode, tabGrp);
}

void MEIOutput::WriteTuplet(pugi::xml_node currentNode, Tuplet *tuplet)
{
    assert(tuplet);

    WriteLayerElement(currentNode, tuplet);
    tuplet->WriteColor(currentNode);
    tuplet->WriteDurationRatio(currentNode);
    tuplet->WriteNumberPlacement(currentNode);
    tuplet->WriteTupletVis(currentNode);
}

void MEIOutput::WriteVerse(pugi::xml_node currentNode, Verse *verse)
{
    assert(verse);

    WriteLayerElement(currentNode, verse);
    verse->WriteColor(currentNode);
    verse->WriteLang(currentNode);
    verse->WriteNInteger(currentNode);
    verse->WriteTypography(currentNode);
}

void MEIOutput::WriteSyl(pugi::xml_node currentNode, Syl *syl)
{
    assert(syl);

    WriteLayerElement(currentNode, syl);
    WriteFacsimileInterface(currentNode, syl);
    syl->WriteLang(currentNode);
    syl->WriteTypography(currentNode);
    syl->WriteSylLog(currentNode);
}

void MEIOutput::WriteSyllable(pugi::xml_node currentNode, Syllable *syllable)
{
    assert(syllable);

    WriteLayerElement(currentNode, syllable);
    syllable->WriteColor(currentNode);
    syllable->WriteSlashCount(currentNode);
}

void MEIOutput::WriteFacsimile(pugi::xml_node currentNode, Facsimile *facsimile)
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

void MEIOutput::WriteSurface(pugi::xml_node currentNode, Surface *surface)
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

void MEIOutput::WriteZone(pugi::xml_node currentNode, Zone *zone)
{
    assert(zone);
    WriteXmlId(currentNode, zone);
    zone->WriteCoordinated(currentNode);
    zone->WriteTyped(currentNode);
}

void MEIOutput::WriteTextElement(pugi::xml_node currentNode, TextElement *textElement)
{
    assert(textElement);

    WriteXmlId(currentNode, textElement);
    textElement->WriteLabelled(currentNode);
    textElement->WriteTyped(currentNode);
}

void MEIOutput::WriteF(pugi::xml_node currentNode, F *f)
{
    assert(f);

    WriteTextElement(currentNode, f);
    WriteTimeSpanningInterface(currentNode, f);
    f->WriteExtender(currentNode);
}

void MEIOutput::WriteFig(pugi::xml_node currentNode, Fig *fig)
{
    assert(fig);

    WriteTextElement(currentNode, fig);
    WriteAreaPosInterface(currentNode, fig);
}

void MEIOutput::WriteLb(pugi::xml_node currentNode, Lb *lb)
{
    assert(lb);

    WriteTextElement(currentNode, lb);
}

void MEIOutput::WriteNum(pugi::xml_node currentNode, Num *num)
{
    assert(num);

    WriteTextElement(currentNode, num);
}

void MEIOutput::WriteRend(pugi::xml_node currentNode, Rend *rend)
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

void MEIOutput::WriteSvg(pugi::xml_node currentNode, Svg *svg)
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

void MEIOutput::WriteText(pugi::xml_node element, Text *text)
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

void MEIOutput::WriteAreaPosInterface(pugi::xml_node element, AreaPosInterface *interface)
{
    assert(interface);

    interface->WriteHorizontalAlign(element);
    interface->WriteVerticalAlign(element);
}

void MEIOutput::WriteDurationInterface(pugi::xml_node element, DurationInterface *interface)
{
    assert(interface);

    interface->WriteAugmentDots(element);
    interface->WriteBeamSecondary(element);
    interface->WriteDurationGestural(element);
    interface->WriteDurationLogical(element);
    interface->WriteDurationQuality(element);
    interface->WriteDurationRatio(element);
    interface->WriteFermataPresent(element);
    interface->WriteStaffIdent(element);
}

void MEIOutput::WriteLinkingInterface(pugi::xml_node element, LinkingInterface *interface)
{
    assert(interface);

    interface->WriteLinking(element);
}

void MEIOutput::WriteFacsimileInterface(pugi::xml_node element, FacsimileInterface *interface)
{
    assert(interface);

    interface->WriteFacsimile(element);
}

void MEIOutput::WritePitchInterface(pugi::xml_node element, PitchInterface *interface)
{
    assert(interface);

    interface->WriteNoteGes(element);
    interface->WriteOctave(element);
    interface->WritePitch(element);
}

void MEIOutput::WritePlistInterface(pugi::xml_node element, PlistInterface *interface)
{
    assert(interface);

    interface->WritePlist(element);
}

void MEIOutput::WritePositionInterface(pugi::xml_node element, PositionInterface *interface)
{
    assert(interface);

    interface->WriteStaffLoc(element);
    interface->WriteStaffLocPitched(element);
}

void MEIOutput::WriteScoreDefInterface(pugi::xml_node element, ScoreDefInterface *interface)
{
    assert(interface);

    interface->WriteLyricStyle(element);
    interface->WriteMidiTempo(element);
    interface->WriteMultinumMeasures(element);
}

void MEIOutput::WriteTextDirInterface(pugi::xml_node element, TextDirInterface *interface)
{
    assert(interface);

    interface->WritePlacementRelStaff(element);
}

void MEIOutput::WriteTimePointInterface(pugi::xml_node element, TimePointInterface *interface)
{
    assert(interface);

    interface->WriteStaffIdent(element);
    interface->WriteStartId(element);
    interface->WriteTimestampLogical(element);
}

void MEIOutput::WriteTimeSpanningInterface(pugi::xml_node element, TimeSpanningInterface *interface)
{
    assert(interface);

    WriteTimePointInterface(element, interface);
    interface->WriteStartEndId(element);
    interface->WriteTimestamp2Logical(element);
}

void MEIOutput::WriteUnsupportedAttr(pugi::xml_node element, Object *object)
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

void MEIOutput::WriteEditorialElement(pugi::xml_node currentNode, EditorialElement *element)
{
    assert(element);

    WriteXmlId(currentNode, element);
    element->WriteLabelled(currentNode);
    element->WriteTyped(currentNode);
}

void MEIOutput::WriteAbbr(pugi::xml_node currentNode, Abbr *abbr)
{
    assert(abbr);

    WriteEditorialElement(currentNode, abbr);
    abbr->WriteSource(currentNode);
}

void MEIOutput::WriteAdd(pugi::xml_node currentNode, Add *add)
{
    assert(add);

    WriteEditorialElement(currentNode, add);
    add->WriteSource(currentNode);
}

void MEIOutput::WriteAnnot(pugi::xml_node currentNode, Annot *annot)
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

void MEIOutput::WriteApp(pugi::xml_node currentNode, App *app)
{
    assert(app);

    WriteEditorialElement(currentNode, app);
}

void MEIOutput::WriteChoice(pugi::xml_node currentNode, Choice *choice)
{
    assert(choice);

    WriteEditorialElement(currentNode, choice);
}

void MEIOutput::WriteCorr(pugi::xml_node currentNode, Corr *corr)
{
    assert(corr);

    WriteEditorialElement(currentNode, corr);
    corr->WriteSource(currentNode);
}

void MEIOutput::WriteDamage(pugi::xml_node currentNode, Damage *damage)
{
    assert(damage);

    WriteEditorialElement(currentNode, damage);
    damage->WriteSource(currentNode);
}

void MEIOutput::WriteDel(pugi::xml_node currentNode, Del *del)
{
    assert(del);

    WriteEditorialElement(currentNode, del);
    del->WriteSource(currentNode);
}

void MEIOutput::WriteExpan(pugi::xml_node currentNode, Expan *expan)
{
    assert(expan);

    WriteEditorialElement(currentNode, expan);
    expan->WriteSource(currentNode);
}

void MEIOutput::WriteLem(pugi::xml_node currentNode, Lem *lem)
{
    assert(lem);

    WriteEditorialElement(currentNode, lem);
    lem->WriteSource(currentNode);
}

void MEIOutput::WriteOrig(pugi::xml_node currentNode, Orig *orig)
{
    assert(orig);

    WriteEditorialElement(currentNode, orig);
    orig->WriteSource(currentNode);
}

void MEIOutput::WriteRdg(pugi::xml_node currentNode, Rdg *rdg)
{
    assert(rdg);

    WriteEditorialElement(currentNode, rdg);
    rdg->WriteSource(currentNode);
}

void MEIOutput::WriteRef(pugi::xml_node currentNode, Ref *ref)
{
    assert(ref);

    WriteEditorialElement(currentNode, ref);
}

void MEIOutput::WriteReg(pugi::xml_node currentNode, Reg *reg)
{
    assert(reg);

    WriteEditorialElement(currentNode, reg);
    reg->WriteSource(currentNode);
}

void MEIOutput::WriteRestore(pugi::xml_node currentNode, Restore *restore)
{
    assert(restore);

    WriteEditorialElement(currentNode, restore);
    restore->WriteSource(currentNode);

    return;
}

void MEIOutput::WriteSic(pugi::xml_node currentNode, Sic *sic)
{
    assert(sic);

    WriteEditorialElement(currentNode, sic);
    sic->WriteSource(currentNode);
}

void MEIOutput::WriteSubst(pugi::xml_node currentNode, Subst *subst)
{
    assert(subst);

    WriteEditorialElement(currentNode, subst);
}

void MEIOutput::WriteSupplied(pugi::xml_node currentNode, Supplied *supplied)
{
    assert(supplied);

    WriteEditorialElement(currentNode, supplied);
    supplied->WriteSource(currentNode);
}

void MEIOutput::WriteUnclear(pugi::xml_node currentNode, Unclear *unclear)
{
    assert(unclear);

    WriteEditorialElement(currentNode, unclear);
    unclear->WriteSource(currentNode);
}

std::wstring MEIOutput::EscapeSMuFL(std::wstring data)
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

std::string MEIOutput::DocTypeToStr(DocType type)
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
// MEIInput
//----------------------------------------------------------------------------

MEIInput::MEIInput(Doc *doc) : Input(doc)
{
    m_hasScoreDef = false;
    m_readingScoreBased = false;
    m_version = MEI_UNDEFINED;
}

MEIInput::~MEIInput() {}

bool MEIInput::Import(const std::string &mei)
{
    try {
        m_doc->Reset();
        m_doc->SetType(Raw);
        pugi::xml_document doc;
        doc.load_string(mei.c_str(), (pugi::parse_comments | pugi::parse_default) & ~pugi::parse_eol);
        pugi::xml_node root = doc.first_child();
        return ReadDoc(root);
    }
    catch (char *str) {
        LogError("%s", str);
        return false;
    }
}

bool MEIInput::IsAllowed(std::string element, Object *filterParent)
{
    if (!filterParent || (element == "")) {
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
        else if (element == "verse") {
            return true;
        }
        else {
            return false;
        }
    }
    // filter for custos
    else if (filterParent->Is(CUSTOS)) {
        if (element == "accid") {
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
        if (element == "dot") {
            return true;
        }
        else if (element == "note") {
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
        else if (element == "plica") {
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
    // filter for tabGrp
    else if (filterParent->Is(TABGRP)) {
        if (element == "tabDurSym") {
            return true;
        }
        if (element == "note") {
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
        else if (element == "bTrem") {
            return true;
        }
        else if (element == "chord") {
            return true;
        }
        else if (element == "clef") {
            return true;
        }
        else if (element == "fTrem") {
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
    // filter for tunning
    else if (filterParent->Is(TUNING)) {
        if (element == "course") {
            return true;
        }
        else {
            return false;
        }
    }
    // filter for verse
    else if (filterParent->Is(VERSE)) {
        if (element == "label") {
            return true;
        }
        else if (element == "labelAbbr") {
            return true;
        }
        else if (element == "syl") {
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

bool MEIInput::ReadDoc(pugi::xml_node root)
{
    pugi::xml_node current;
    bool success = true;
    m_readingScoreBased = false;

    if (root.empty()) {
        LogError("The tree of the MEI data cannot be parsed (no root found)");
        return false;
    }

    if (root.attribute("meiversion")) {
        std::string version = std::string(root.attribute("meiversion").value());
        if (version == "5.0.0-dev") {
            m_version = MEI_5_0_0_dev;
        }
        else if (version == "4.0.1") {
            m_version = MEI_4_0_1;
        }
        else if (version == "4.0.0") {
            m_version = MEI_4_0_0;
        }
        else if (version == "3.0.0") {
            m_version = MEI_3_0_0;
        }
        else if (version == "2013") {
            m_version = MEI_2013;
        }
    }
    else {
        // default to MEI 5
        LogWarning("No meiversion found, falling back to MEI5 (dev)");
        m_version = MEI_5_0_0_dev;
    }

    // only try to handle meiHead if we have a full MEI document
    if (std::string(root.name()) == "mei") {
        current = root.child("meiHead");
        if (current.empty()) {
            LogWarning("No header found in the MEI data, trying to proceed...");
        }
        else {
            m_doc->m_header.reset();
            // copy the complete header into the master document
            m_doc->m_header.append_copy(current);
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
    // Give priority to mdiv-all - maybe we could give a warning
    if (!m_doc->GetOptions()->m_mdivAll.GetValue() && !xPathQuery.empty()) {
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

    const bool allMdivVisible = m_doc->GetOptions()->m_mdivAll.GetValue();
    success = ReadMdivChildren(m_doc, body, allMdivVisible);

    if (success) {
        m_doc->ExpandExpansions();
    }

    if (success && m_readingScoreBased) {
        m_doc->ConvertToPageBasedDoc();
        m_doc->ConvertMarkupDoc(!m_doc->GetOptions()->m_preserveAnalyticalMarkup.GetValue());
    }

    if (success && !m_hasScoreDef) {
        LogWarning("No scoreDef provided, trying to generate one...");
        success = m_doc->GenerateDocumentScoreDef();
    }

    return success;
}

bool MEIInput::ReadMdiv(Object *parent, pugi::xml_node mdiv, bool isVisible)
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

bool MEIInput::ReadMdivChildren(Object *parent, pugi::xml_node parentNode, bool isVisible)
{
    assert(dynamic_cast<Doc *>(parent) || dynamic_cast<Mdiv *>(parent));

    pugi::xml_node current;
    bool success = true;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        // We make the mdiv visible if already set or if matching the desired selection
        bool makeVisible = (isVisible || (m_selectedMdiv == current));
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
        // xml comment
        else if (std::string(current.name()) == "") {
            success = ReadXMLComment(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <mdiv>", current.name());
        }
    }

    return success;
}

bool MEIInput::ReadPages(Object *parent, pugi::xml_node pages)
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
    m_hasLayoutInformation = true;

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
        // xml comment
        else if (std::string(current.name()) == "") {
            success = ReadXMLComment(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <pages>", current.name());
        }
    }

    ReadUnsupportedAttr(pages, vrvPages);
    return success;
}

bool MEIInput::ReadScore(Object *parent, pugi::xml_node score)
{
    Score *vrvScore = new Score();
    SetMeiUuid(score, vrvScore);

    vrvScore->ReadLabelled(score);
    vrvScore->ReadNNumberLike(score);

    parent->AddChild(vrvScore);

    // This is a score-based MEI file
    m_readingScoreBased = true;

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
        else if (elementName == "sb") {
            success = ReadSb(vrvScore, current);
        }
        else if (elementName == "pb") {
            success = ReadPb(vrvScore, current);
        }
        // xml comment
        else if (std::string(current.name()) == "") {
            success = ReadXMLComment(parent, current);
        }
        else {
            LogWarning("Element <%s> within <score> is not supported and will be ignored ", elementName.c_str());
        }
    }

    ReadUnsupportedAttr(score, vrvScore);
    return success;
}

bool MEIInput::ReadSection(Object *parent, pugi::xml_node section)
{
    Section *vrvSection = new Section();
    SetMeiUuid(section, vrvSection);

    vrvSection->ReadNNumberLike(section);
    vrvSection->ReadSectionVis(section);

    parent->AddChild(vrvSection);
    ReadUnsupportedAttr(section, vrvSection);
    if (m_readingScoreBased) {
        return ReadSectionChildren(vrvSection, section);
    }
    else {
        return ReadSystemChildren(vrvSection, section);
    }
}

bool MEIInput::ReadSectionChildren(Object *parent, pugi::xml_node parentNode)
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
        // xml comment
        else if (std::string(current.name()) == "") {
            success = ReadXMLComment(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <section>", current.name());
        }
    }
    return success;
}

bool MEIInput::ReadSystemElement(pugi::xml_node element, SystemElement *object)
{
    SetMeiUuid(element, object);
    object->ReadTyped(element);

    return true;
}

bool MEIInput::ReadEnding(Object *parent, pugi::xml_node ending)
{
    Ending *vrvEnding = new Ending();
    ReadSystemElement(ending, vrvEnding);

    vrvEnding->ReadLineRend(ending);
    vrvEnding->ReadNNumberLike(ending);

    parent->AddChild(vrvEnding);
    ReadUnsupportedAttr(ending, vrvEnding);
    if (m_readingScoreBased) {
        return ReadSectionChildren(vrvEnding, ending);
    }
    return true;
}

bool MEIInput::ReadExpansion(Object *parent, pugi::xml_node expansion)
{
    Expansion *vrvExpansion = new Expansion();
    ReadSystemElement(expansion, vrvExpansion);
    ReadPlistInterface(expansion, vrvExpansion);

    parent->AddChild(vrvExpansion);
    ReadUnsupportedAttr(expansion, vrvExpansion);
    if (m_readingScoreBased) {
        return ReadSectionChildren(vrvExpansion, expansion);
    }
    return true;
}

bool MEIInput::ReadPb(Object *parent, pugi::xml_node pb)
{
    m_hasLayoutInformation = true;

    Pb *vrvPb = new Pb();
    ReadSystemElement(pb, vrvPb);

    vrvPb->ReadNNumberLike(pb);

    parent->AddChild(vrvPb);
    ReadUnsupportedAttr(pb, vrvPb);
    return true;
}

bool MEIInput::ReadSb(Object *parent, pugi::xml_node sb)
{
    Sb *vrvSb = new Sb();
    ReadSystemElement(sb, vrvSb);

    vrvSb->ReadNNumberLike(sb);

    parent->AddChild(vrvSb);
    ReadUnsupportedAttr(sb, vrvSb);
    return true;
}

bool MEIInput::ReadPage(Object *parent, pugi::xml_node page)
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

bool MEIInput::ReadPageChildren(Object *parent, pugi::xml_node parentNode)
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
        // xml comment
        else if (std::string(current.name()) == "") {
            ReadXMLComment(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <page>", current.name());
        }
    }

    return true;
}

bool MEIInput::ReadSystem(Object *parent, pugi::xml_node system)
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
    if (system.attribute("uly") && (m_doc->GetType() == Transcription)) {
        vrvSystem->m_yAbs = atoi(system.attribute("uly").value()) * DEFINITION_FACTOR;
        system.remove_attribute("uly");
    }

    parent->AddChild(vrvSystem);
    ReadUnsupportedAttr(system, vrvSystem);
    return ReadSystemChildren(vrvSystem, system);
}

bool MEIInput::ReadSystemChildren(Object *parent, pugi::xml_node parentNode)
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
        // elementEnd
        else if (std::string(current.name()) == "systemElementEnd") {
            success = ReadSystemElementEnd(parent, current);
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
                    System *system = vrv_cast<System *>(parent);
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
        // xml comment
        else if (std::string(current.name()) == "") {
            success = ReadXMLComment(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <system>", current.name());
        }
    }
    return success;
}

bool MEIInput::ReadSystemElementEnd(Object *parent, pugi::xml_node elementEnd)
{
    assert(dynamic_cast<System *>(parent));

    std::string startUuid;
    Object *start = NULL;
    if (elementEnd.attribute("startid")) {
        std::string startUuid = elementEnd.attribute("startid").value();
        start = m_doc->FindDescendantByUuid(startUuid);
    }
    if (!start) {
        LogError("Could not find start element <%s> for systemElementEnd", startUuid.c_str());
        return false;
    }

    SystemElementEnd *vrvElementEnd = new SystemElementEnd(start);
    ReadSystemElement(elementEnd, vrvElementEnd);

    parent->AddChild(vrvElementEnd);
    return true;
}

bool MEIInput::ReadScoreDefElement(pugi::xml_node element, ScoreDefElement *object)
{
    SetMeiUuid(element, object);
    object->ReadMeasureNumbers(element);
    object->ReadSpacing(element);
    object->ReadSystems(element);
    object->ReadTyped(element);

    AttCleffingLog cleffingLog;
    cleffingLog.ReadCleffingLog(element);
    AttCleffingVis cleffingVis;
    cleffingVis.ReadCleffingVis(element);
    if (cleffingLog.HasClefShape()) {
        Clef *vrvClef = new Clef();
        vrvClef->IsAttribute(true);
        vrvClef->SetShape(cleffingLog.GetClefShape());
        vrvClef->SetLine(cleffingLog.GetClefLine());
        vrvClef->SetDis(cleffingLog.GetClefDis());
        vrvClef->SetDisPlace(cleffingLog.GetClefDisPlace());
        vrvClef->SetColor(cleffingVis.GetClefColor());
        vrvClef->SetVisible(cleffingVis.GetClefVisible());
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
        || mensuralLog.HasProportNumbase() || mensuralVis.HasMensurSign()) {
        Mensur *vrvMensur = new Mensur();
        vrvMensur->IsAttribute(true);
        //
        vrvMensur->SetDot(mensuralVis.GetMensurDot());
        vrvMensur->SetNum(mensuralLog.GetProportNum());
        vrvMensur->SetNumbase(mensuralLog.GetProportNumbase());
        vrvMensur->SetSign(mensuralVis.GetMensurSign());
        vrvMensur->SetSlash(mensuralVis.GetMensurSlash());
        //
        vrvMensur->SetModusmaior(mensuralShared.GetModusmaior());
        vrvMensur->SetModusminor(mensuralShared.GetModusminor());
        vrvMensur->SetProlatio(mensuralShared.GetProlatio());
        vrvMensur->SetTempus(mensuralShared.GetTempus());
        //
        vrvMensur->SetColor(mensuralVis.GetMensurColor());
        vrvMensur->SetOrient(mensuralVis.GetMensurOrient());

        if (m_version < MEI_5_0_0_dev) {
            UpgradeMensurTo_5_0_0(element, vrvMensur);
        }

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

bool MEIInput::ReadScoreDef(Object *parent, pugi::xml_node scoreDef)
{
    assert(dynamic_cast<Pages *>(parent) || dynamic_cast<Score *>(parent) || dynamic_cast<Section *>(parent)
        || dynamic_cast<System *>(parent) || dynamic_cast<Ending *>(parent)
        || dynamic_cast<EditorialElement *>(parent));
    // assert(dynamic_cast<Pages *>(parent));

    ScoreDef *vrvScoreDef;
    // We have not reached the first scoreDef and we have to use if for the doc
    if (parent->Is(SCORE)) {
        Score *score = vrv_cast<Score *>(parent);
        assert(score);
        vrvScoreDef = score->GetScoreDef();
        m_hasScoreDef = true;
    }
    else {
        vrvScoreDef = new ScoreDef();
        parent->AddChild(vrvScoreDef);
    }
    ReadScoreDefElement(scoreDef, vrvScoreDef);

    if (m_version < MEI_4_0_0) {
        UpgradeScoreDefElementTo_4_0_0(scoreDef, vrvScoreDef);
    }

    ReadScoreDefInterface(scoreDef, vrvScoreDef);
    vrvScoreDef->ReadDistances(scoreDef);
    vrvScoreDef->ReadEndings(scoreDef);
    vrvScoreDef->ReadOptimization(scoreDef);
    vrvScoreDef->ReadTimeBase(scoreDef);

    ReadUnsupportedAttr(scoreDef, vrvScoreDef);
    return ReadScoreDefChildren(vrvScoreDef, scoreDef);
}

bool MEIInput::ReadScoreDefChildren(Object *parent, pugi::xml_node parentNode)
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
        else if (std::string(current.name()) == "grpSym") {
            success = ReadGrpSym(parent, current);
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
        else if (std::string(current.name()) == "meterSigGrp") {
            success = ReadMeterSigGrp(parent, current);
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
        // xml comment
        else if (std::string(current.name()) == "") {
            success = ReadXMLComment(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <scoreDef>", current.name());
        }
    }
    return success;
}

bool MEIInput::ReadStaffGrp(Object *parent, pugi::xml_node staffGrp)
{
    assert(dynamic_cast<ScoreDef *>(parent) || dynamic_cast<StaffGrp *>(parent));

    StaffGrp *vrvStaffGrp = new StaffGrp();
    SetMeiUuid(staffGrp, vrvStaffGrp);

    if (m_version < MEI_4_0_0) {
        UpgradeStaffGrpTo_4_0_0(staffGrp, vrvStaffGrp);
    }

    vrvStaffGrp->ReadBasic(staffGrp);
    vrvStaffGrp->ReadLabelled(staffGrp);
    vrvStaffGrp->ReadNNumberLike(staffGrp);
    AttStaffGroupingSym groupingSym;
    groupingSym.ReadStaffGroupingSym(staffGrp);
    if (groupingSym.HasSymbol()) {
        GrpSym *vrvGrpSym = new GrpSym();
        vrvGrpSym->IsAttribute(true);
        vrvGrpSym->SetSymbol(groupingSym.GetSymbol());
        vrvStaffGrp->AddChild(vrvGrpSym);
    }
    vrvStaffGrp->ReadStaffGrpVis(staffGrp);
    vrvStaffGrp->ReadTyped(staffGrp);

    parent->AddChild(vrvStaffGrp);
    ReadUnsupportedAttr(staffGrp, vrvStaffGrp);
    return ReadStaffGrpChildren(vrvStaffGrp, staffGrp);
}

bool MEIInput::ReadStaffGrpChildren(Object *parent, pugi::xml_node parentNode)
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
        else if (std::string(current.name()) == "grpSym") {
            success = ReadGrpSym(parent, current);
        }
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
        // xml comment
        else if (std::string(current.name()) == "") {
            success = ReadXMLComment(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <staffGrp>", current.name());
        }
    }
    return success;
}

bool MEIInput::ReadRunningElement(pugi::xml_node element, RunningElement *object)
{
    SetMeiUuid(element, object);
    object->ReadHorizontalAlign(element);
    object->ReadTyped(element);

    return true;
}

bool MEIInput::ReadGrpSym(Object *parent, pugi::xml_node grpSym)
{
    GrpSym *vrvGrpSym = new GrpSym();
    SetMeiUuid(grpSym, vrvGrpSym);

    vrvGrpSym->ReadColor(grpSym);
    vrvGrpSym->ReadGrpSymLog(grpSym);
    vrvGrpSym->ReadStaffGroupingSym(grpSym);
    vrvGrpSym->ReadStartId(grpSym);
    vrvGrpSym->ReadStartEndId(grpSym);

    if (parent->Is(SCOREDEF)) {
        if (!vrvGrpSym->HasLevel() || !vrvGrpSym->HasStartid() || !vrvGrpSym->HasEndid()) {
            LogWarning("<%s>' nested under <scoreDef> must have @level, @startId and @endId attributes", grpSym.name());
            delete vrvGrpSym;
            return true;
        }
    }

    parent->AddChild(vrvGrpSym);
    ReadUnsupportedAttr(grpSym, vrvGrpSym);
    return true;
}

bool MEIInput::ReadPgFoot(Object *parent, pugi::xml_node pgFoot)
{
    PgFoot *vrvPgFoot = new PgFoot();
    ReadRunningElement(pgFoot, vrvPgFoot);

    parent->AddChild(vrvPgFoot);
    ReadUnsupportedAttr(pgFoot, vrvPgFoot);
    return ReadRunningChildren(vrvPgFoot, pgFoot, vrvPgFoot);
}

bool MEIInput::ReadPgFoot2(Object *parent, pugi::xml_node pgFoot2)
{
    PgFoot2 *vrvPgFoot2 = new PgFoot2();
    ReadRunningElement(pgFoot2, vrvPgFoot2);

    parent->AddChild(vrvPgFoot2);
    ReadUnsupportedAttr(pgFoot2, vrvPgFoot2);
    return ReadRunningChildren(vrvPgFoot2, pgFoot2, vrvPgFoot2);
}

bool MEIInput::ReadPgHead(Object *parent, pugi::xml_node pgHead)
{
    PgHead *vrvPgHead = new PgHead();
    ReadRunningElement(pgHead, vrvPgHead);

    parent->AddChild(vrvPgHead);
    ReadUnsupportedAttr(pgHead, vrvPgHead);
    return ReadRunningChildren(vrvPgHead, pgHead, vrvPgHead);
}

bool MEIInput::ReadPgHead2(Object *parent, pugi::xml_node pgHead2)
{
    PgHead2 *vrvPgHead2 = new PgHead2();
    ReadRunningElement(pgHead2, vrvPgHead2);

    parent->AddChild(vrvPgHead2);
    ReadUnsupportedAttr(pgHead2, vrvPgHead2);
    return ReadRunningChildren(vrvPgHead2, pgHead2, vrvPgHead2);
}

bool MEIInput::ReadRunningChildren(Object *parent, pugi::xml_node parentNode, Object *filter)
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
        // xml comment
        else if (elementName == "") {
            success = ReadXMLComment(parent, xmlElement);
        }
        // unknown
        else {
            LogWarning("Element <%s> is unknown and will be ignored", xmlElement.name());
        }
        i++;
    }
    return success;
}

bool MEIInput::ReadStaffDef(Object *parent, pugi::xml_node staffDef)
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

bool MEIInput::ReadStaffDefChildren(Object *parent, pugi::xml_node parentNode)
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
        else if (std::string(current.name()) == "meterSigGrp") {
            success = ReadMeterSigGrp(parent, current);
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
        else if (std::string(current.name()) == "tuning") {
            success = ReadTuning(parent, current);
        }
        // xml comment
        else if (std::string(current.name()) == "") {
            success = ReadXMLComment(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <staffGrp>", current.name());
        }
    }
    return success;
}

bool MEIInput::ReadTuning(Object *parent, pugi::xml_node tuning)
{
    assert(dynamic_cast<StaffDef *>(parent) || dynamic_cast<EditorialElement *>(parent));

    Tuning *vrvTuning = new Tuning();
    SetMeiUuid(tuning, vrvTuning);

    parent->AddChild(vrvTuning);
    vrvTuning->ReadCourseLog(tuning);

    ReadUnsupportedAttr(tuning, vrvTuning);
    return ReadTuningChildren(vrvTuning, tuning);
}

bool MEIInput::ReadTuningChildren(Object *parent, pugi::xml_node parentNode)
{
    assert(dynamic_cast<Tuning *>(parent) || dynamic_cast<EditorialElement *>(parent));

    bool success = true;
    pugi::xml_node current;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        if (!success) break;
        // content
        else if (std::string(current.name()) == "course") {
            success = ReadCourse(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <staffGrp>", current.name());
        }
    }
    return success;
}

bool MEIInput::ReadCourse(Object *parent, pugi::xml_node course)
{
    assert(dynamic_cast<Tuning *>(parent) || dynamic_cast<EditorialElement *>(parent));

    Course *vrvCourse = new Course();
    SetMeiUuid(course, vrvCourse);

    parent->AddChild(vrvCourse);
    vrvCourse->ReadNNumberLike(course);

    ReadUnsupportedAttr(course, vrvCourse);

    return true;
}

bool MEIInput::ReadInstrDef(Object *parent, pugi::xml_node instrDef)
{
    InstrDef *vrvInstrDef = new InstrDef();
    SetMeiUuid(instrDef, vrvInstrDef);

    if (m_version < MEI_4_0_0) {
        if (instrDef.attribute("midi.volume")) {
            const float midiValue = instrDef.attribute("midi.volume").as_float();
            instrDef.attribute("midi.volume").set_value(StringFormat("%.2f%%", midiValue / 127 * 100).c_str());
        }
    }

    parent->AddChild(vrvInstrDef);
    vrvInstrDef->ReadChannelized(instrDef);
    vrvInstrDef->ReadLabelled(instrDef);
    vrvInstrDef->ReadMidiInstrument(instrDef);
    vrvInstrDef->ReadNNumberLike(instrDef);

    return true;
}

bool MEIInput::ReadLabel(Object *parent, pugi::xml_node label)
{
    Label *vrvLabel = new Label();
    SetMeiUuid(label, vrvLabel);

    parent->AddChild(vrvLabel);
    ReadUnsupportedAttr(label, vrvLabel);
    return ReadTextChildren(vrvLabel, label, vrvLabel);
}

bool MEIInput::ReadLabelAbbr(Object *parent, pugi::xml_node labelAbbr)
{
    LabelAbbr *vrvLabelAbbr = new LabelAbbr();
    SetMeiUuid(labelAbbr, vrvLabelAbbr);

    parent->AddChild(vrvLabelAbbr);
    ReadUnsupportedAttr(labelAbbr, vrvLabelAbbr);
    return ReadTextChildren(vrvLabelAbbr, labelAbbr, vrvLabelAbbr);
}

bool MEIInput::ReadMeasure(Object *parent, pugi::xml_node measure)
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

    if (measure.attribute("ulx") && measure.attribute("lrx") && (m_doc->GetType() == Transcription)) {
        vrvMeasure->m_xAbs = atoi(measure.attribute("ulx").value()) * DEFINITION_FACTOR;
        vrvMeasure->m_xAbs2 = atoi(measure.attribute("lrx").value()) * DEFINITION_FACTOR;
        measure.remove_attribute("ulx");
        measure.remove_attribute("lrx");
    }

    parent->AddChild(vrvMeasure);
    ReadUnsupportedAttr(measure, vrvMeasure);
    return ReadMeasureChildren(vrvMeasure, measure);
}

bool MEIInput::ReadMeasureChildren(Object *parent, pugi::xml_node parentNode)
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
        else if (std::string(current.name()) == "beamSpan") {
            if (!ReadBeamSpanAsBeam(dynamic_cast<Measure *>(parent), current)) {
                LogWarning("<beamSpan> is not readable as <beam> and will be ignored");
            }
        }
        else if (std::string(current.name()) == "bracketSpan") {
            success = ReadBracketSpan(parent, current);
        }
        else if (std::string(current.name()) == "breath") {
            success = ReadBreath(parent, current);
        }
        else if (std::string(current.name()) == "caesura") {
            success = ReadCaesura(parent, current);
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
        else if (std::string(current.name()) == "fing") {
            success = ReadFing(parent, current);
        }
        else if (std::string(current.name()) == "gliss") {
            success = ReadGliss(parent, current);
        }
        else if (std::string(current.name()) == "hairpin") {
            success = ReadHairpin(parent, current);
        }
        else if (std::string(current.name()) == "harm") {
            success = ReadHarm(parent, current);
        }
        else if (std::string(current.name()) == "lv") {
            success = ReadLv(parent, current);
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
        else if (std::string(current.name()) == "phrase") {
            success = ReadPhrase(parent, current);
        }
        else if (std::string(current.name()) == "pitchInflection") {
            success = ReadPitchInflection(parent, current);
        }
        else if (std::string(current.name()) == "reh") {
            success = ReadReh(parent, current);
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
        // xml comment
        else if (std::string(current.name()) == "") {
            success = ReadXMLComment(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <measure>", current.name());
        }
    }
    return success;
}

bool MEIInput::ReadMeterSigGrp(Object *parent, pugi::xml_node meterSigGrp)
{
    assert(dynamic_cast<ScoreDef *>(parent) || dynamic_cast<StaffDef *>(parent) || dynamic_cast<Layer *>(parent));

    MeterSigGrp *vrvMeterSigGrp = new MeterSigGrp();
    SetMeiUuid(meterSigGrp, vrvMeterSigGrp);
    ReadLinkingInterface(meterSigGrp, vrvMeterSigGrp);
    vrvMeterSigGrp->ReadBasic(meterSigGrp);
    vrvMeterSigGrp->ReadLabelled(meterSigGrp);
    vrvMeterSigGrp->ReadMeterSigGrpLog(meterSigGrp);
    vrvMeterSigGrp->ReadTyped(meterSigGrp);

    parent->AddChild(vrvMeterSigGrp);
    ReadUnsupportedAttr(meterSigGrp, vrvMeterSigGrp);
    return ReadMeterSigGrpChildren(vrvMeterSigGrp, meterSigGrp);
}

bool MEIInput::ReadMeterSigGrpChildren(Object *parent, pugi::xml_node parentNode)
{
    assert(dynamic_cast<MeterSigGrp *>(parent));

    bool success = true;
    pugi::xml_node current;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        if (!success) break;
        // content
        else if (std::string(current.name()) == "meterSig") {
            success = ReadMeterSig(parent, current);
        }
        // xml comment
        else if (std::string(current.name()) == "") {
            success = ReadXMLComment(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <meterSigGrp>", current.name());
        }
    }
    return success;
}

bool MEIInput::ReadControlElement(pugi::xml_node element, ControlElement *object)
{
    SetMeiUuid(element, object);
    ReadLinkingInterface(element, object);
    object->ReadLabelled(element);
    object->ReadTyped(element);

    return true;
}

bool MEIInput::ReadAnchoredText(Object *parent, pugi::xml_node anchoredText)
{
    AnchoredText *vrvAnchoredText = new AnchoredText();
    ReadControlElement(anchoredText, vrvAnchoredText);

    ReadTextDirInterface(anchoredText, vrvAnchoredText);

    ReadUnsupportedAttr(anchoredText, vrvAnchoredText);
    parent->AddChild(vrvAnchoredText);
    return ReadTextChildren(vrvAnchoredText, anchoredText, vrvAnchoredText);
}

bool MEIInput::ReadArpeg(Object *parent, pugi::xml_node arpeg)
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

bool MEIInput::ReadBracketSpan(Object *parent, pugi::xml_node bracketSpan)
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

bool MEIInput::ReadBreath(Object *parent, pugi::xml_node breath)
{
    Breath *vrvBreath = new Breath();
    ReadControlElement(breath, vrvBreath);

    ReadTimePointInterface(breath, vrvBreath);
    vrvBreath->ReadColor(breath);
    vrvBreath->ReadPlacementRelStaff(breath);

    parent->AddChild(vrvBreath);
    ReadUnsupportedAttr(breath, vrvBreath);
    return true;
}

bool MEIInput::ReadCaesura(Object *parent, pugi::xml_node caesura)
{
    Caesura *vrvCaesura = new Caesura();
    ReadControlElement(caesura, vrvCaesura);

    ReadTimePointInterface(caesura, vrvCaesura);
    vrvCaesura->ReadColor(caesura);
    vrvCaesura->ReadPlacementRelStaff(caesura);

    parent->AddChild(vrvCaesura);
    ReadUnsupportedAttr(caesura, vrvCaesura);
    return true;
}

bool MEIInput::ReadDir(Object *parent, pugi::xml_node dir)
{
    Dir *vrvDir = new Dir();
    ReadControlElement(dir, vrvDir);

    ReadTextDirInterface(dir, vrvDir);
    ReadTimeSpanningInterface(dir, vrvDir);
    vrvDir->ReadLang(dir);
    vrvDir->ReadLineRendBase(dir);
    vrvDir->ReadExtender(dir);
    vrvDir->ReadVerticalGroup(dir);

    parent->AddChild(vrvDir);
    ReadUnsupportedAttr(dir, vrvDir);
    return ReadTextChildren(vrvDir, dir, vrvDir);
}

bool MEIInput::ReadDynam(Object *parent, pugi::xml_node dynam)
{
    Dynam *vrvDynam = new Dynam();
    ReadControlElement(dynam, vrvDynam);

    ReadTextDirInterface(dynam, vrvDynam);
    ReadTimeSpanningInterface(dynam, vrvDynam);
    vrvDynam->ReadExtender(dynam);
    vrvDynam->ReadLineRendBase(dynam);
    vrvDynam->ReadMidiValue(dynam);
    vrvDynam->ReadMidiValue2(dynam);
    vrvDynam->ReadVerticalGroup(dynam);

    parent->AddChild(vrvDynam);
    ReadUnsupportedAttr(dynam, vrvDynam);
    return ReadTextChildren(vrvDynam, dynam, vrvDynam);
}

bool MEIInput::ReadFermata(Object *parent, pugi::xml_node fermata)
{
    Fermata *vrvFermata = new Fermata();
    ReadControlElement(fermata, vrvFermata);

    ReadTimePointInterface(fermata, vrvFermata);
    vrvFermata->ReadColor(fermata);
    vrvFermata->ReadEnclosingChars(fermata);
    vrvFermata->ReadExtSym(fermata);
    vrvFermata->ReadFermataVis(fermata);
    vrvFermata->ReadPlacementRelStaff(fermata);

    parent->AddChild(vrvFermata);
    ReadUnsupportedAttr(fermata, vrvFermata);
    return true;
}

bool MEIInput::ReadFing(Object *parent, pugi::xml_node fing)
{
    Fing *vrvFing = new Fing();
    ReadControlElement(fing, vrvFing);

    ReadTextDirInterface(fing, vrvFing);
    ReadTimePointInterface(fing, vrvFing);
    vrvFing->ReadNNumberLike(fing);

    parent->AddChild(vrvFing);
    ReadUnsupportedAttr(fing, vrvFing);
    return ReadTextChildren(vrvFing, fing, vrvFing);
}

bool MEIInput::ReadGliss(Object *parent, pugi::xml_node gliss)
{
    Gliss *vrvGliss = new Gliss();
    ReadControlElement(gliss, vrvGliss);

    ReadTimeSpanningInterface(gliss, vrvGliss);
    vrvGliss->ReadColor(gliss);
    vrvGliss->ReadLineRend(gliss);
    vrvGliss->ReadLineRendBase(gliss);
    vrvGliss->ReadNNumberLike(gliss);

    parent->AddChild(vrvGliss);
    ReadUnsupportedAttr(gliss, vrvGliss);
    return true;
}

bool MEIInput::ReadHairpin(Object *parent, pugi::xml_node hairpin)
{
    Hairpin *vrvHairpin = new Hairpin();
    ReadControlElement(hairpin, vrvHairpin);

    ReadTimeSpanningInterface(hairpin, vrvHairpin);
    vrvHairpin->ReadColor(hairpin);
    vrvHairpin->ReadHairpinLog(hairpin);
    vrvHairpin->ReadHairpinVis(hairpin);
    vrvHairpin->ReadPlacementRelStaff(hairpin);
    vrvHairpin->ReadVerticalGroup(hairpin);

    parent->AddChild(vrvHairpin);
    ReadUnsupportedAttr(hairpin, vrvHairpin);
    return true;
}

bool MEIInput::ReadHarm(Object *parent, pugi::xml_node harm)
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

bool MEIInput::ReadLv(Object *parent, pugi::xml_node lv)
{
    Lv *vrvLv = new Lv();
    ReadControlElement(lv, vrvLv);

    ReadTimeSpanningInterface(lv, vrvLv);
    vrvLv->ReadColor(lv);
    vrvLv->ReadCurvature(lv);
    vrvLv->ReadCurveRend(lv);

    parent->AddChild(vrvLv);
    ReadUnsupportedAttr(lv, vrvLv);
    return true;
}

bool MEIInput::ReadMNum(Object *parent, pugi::xml_node mNum)
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

bool MEIInput::ReadMordent(Object *parent, pugi::xml_node mordent)
{
    Mordent *vrvMordent = new Mordent();
    ReadControlElement(mordent, vrvMordent);

    if (m_version < MEI_4_0_0) {
        UpgradeMordentTo_4_0_0(mordent, vrvMordent);
    }

    ReadTimePointInterface(mordent, vrvMordent);
    vrvMordent->ReadColor(mordent);
    vrvMordent->ReadExtSym(mordent);
    vrvMordent->ReadOrnamentAccid(mordent);
    vrvMordent->ReadPlacementRelStaff(mordent);
    vrvMordent->ReadMordentLog(mordent);

    parent->AddChild(vrvMordent);
    ReadUnsupportedAttr(mordent, vrvMordent);
    return true;
}

bool MEIInput::ReadOctave(Object *parent, pugi::xml_node octave)
{
    Octave *vrvOctave = new Octave();
    ReadControlElement(octave, vrvOctave);

    ReadTimeSpanningInterface(octave, vrvOctave);
    vrvOctave->ReadColor(octave);
    vrvOctave->ReadExtender(octave);
    vrvOctave->ReadLineRend(octave);
    vrvOctave->ReadLineRendBase(octave);
    vrvOctave->ReadNNumberLike(octave);
    vrvOctave->ReadOctaveDisplacement(octave);

    parent->AddChild(vrvOctave);
    ReadUnsupportedAttr(octave, vrvOctave);
    return true;
}

bool MEIInput::ReadPedal(Object *parent, pugi::xml_node pedal)
{
    Pedal *vrvPedal = new Pedal();
    ReadControlElement(pedal, vrvPedal);

    ReadTimeSpanningInterface(pedal, vrvPedal);
    vrvPedal->ReadColor(pedal);
    vrvPedal->ReadExtSym(pedal);
    vrvPedal->ReadPedalLog(pedal);
    vrvPedal->ReadPedalVis(pedal);
    vrvPedal->ReadPlacementRelStaff(pedal);
    vrvPedal->ReadVerticalGroup(pedal);

    parent->AddChild(vrvPedal);
    ReadUnsupportedAttr(pedal, vrvPedal);
    return true;
}

bool MEIInput::ReadPhrase(Object *parent, pugi::xml_node phrase)
{
    Phrase *vrvPhrase = new Phrase();
    ReadControlElement(phrase, vrvPhrase);

    ReadTimeSpanningInterface(phrase, vrvPhrase);
    vrvPhrase->ReadColor(phrase);
    vrvPhrase->ReadCurvature(phrase);
    vrvPhrase->ReadCurveRend(phrase);

    parent->AddChild(vrvPhrase);
    ReadUnsupportedAttr(phrase, vrvPhrase);
    return true;
}

bool MEIInput::ReadPitchInflection(Object *parent, pugi::xml_node pitchInflection)
{
    PitchInflection *vrvPitchInflection = new PitchInflection();
    ReadControlElement(pitchInflection, vrvPitchInflection);

    ReadTimeSpanningInterface(pitchInflection, vrvPitchInflection);

    parent->AddChild(vrvPitchInflection);
    ReadUnsupportedAttr(pitchInflection, vrvPitchInflection);
    return true;
}

bool MEIInput::ReadReh(Object *parent, pugi::xml_node reh)
{
    Reh *vrvReh = new Reh();
    ReadControlElement(reh, vrvReh);

    ReadTextDirInterface(reh, vrvReh);
    ReadTimePointInterface(reh, vrvReh);
    vrvReh->ReadColor(reh);
    vrvReh->ReadLang(reh);
    vrvReh->ReadVerticalGroup(reh);

    parent->AddChild(vrvReh);
    ReadUnsupportedAttr(reh, vrvReh);
    return ReadTextChildren(vrvReh, reh, vrvReh);
}

bool MEIInput::ReadSlur(Object *parent, pugi::xml_node slur)
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

bool MEIInput::ReadTempo(Object *parent, pugi::xml_node tempo)
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

bool MEIInput::ReadTie(Object *parent, pugi::xml_node tie)
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

bool MEIInput::ReadTrill(Object *parent, pugi::xml_node trill)
{
    Trill *vrvTrill = new Trill();
    ReadControlElement(trill, vrvTrill);

    ReadTimeSpanningInterface(trill, vrvTrill);
    vrvTrill->ReadColor(trill);
    vrvTrill->ReadExtender(trill);
    vrvTrill->ReadExtSym(trill);
    vrvTrill->ReadLineRend(trill);
    vrvTrill->ReadNNumberLike(trill);
    vrvTrill->ReadOrnamentAccid(trill);
    vrvTrill->ReadPlacementRelStaff(trill);

    parent->AddChild(vrvTrill);
    ReadUnsupportedAttr(trill, vrvTrill);
    return true;
}

bool MEIInput::ReadTurn(Object *parent, pugi::xml_node turn)
{
    Turn *vrvTurn = new Turn();
    ReadControlElement(turn, vrvTurn);

    if (m_version < MEI_4_0_0) {
        UpgradeTurnTo_4_0_0(turn, vrvTurn);
    }

    ReadTimePointInterface(turn, vrvTurn);
    vrvTurn->ReadColor(turn);
    vrvTurn->ReadExtSym(turn);
    vrvTurn->ReadOrnamentAccid(turn);
    vrvTurn->ReadPlacementRelStaff(turn);
    vrvTurn->ReadTurnLog(turn);

    parent->AddChild(vrvTurn);
    ReadUnsupportedAttr(turn, vrvTurn);
    return true;
}

bool MEIInput::ReadFb(Object *parent, pugi::xml_node fb)
{
    Fb *vrvFb = new Fb();
    SetMeiUuid(fb, vrvFb);

    parent->AddChild(vrvFb);
    ReadUnsupportedAttr(fb, vrvFb);
    return ReadFbChildren(vrvFb, fb);
}

bool MEIInput::ReadFbChildren(Object *parent, pugi::xml_node parentNode)
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
        // xml comment
        else if (std::string(current.name()) == "") {
            success = ReadXMLComment(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <staff>", current.name());
        }
    }
    return success;
}

bool MEIInput::ReadStaff(Object *parent, pugi::xml_node staff)
{
    Staff *vrvStaff = new Staff();
    SetMeiUuid(staff, vrvStaff);

    vrvStaff->ReadFacsimile(staff);
    vrvStaff->ReadNInteger(staff);
    vrvStaff->ReadTyped(staff);
    vrvStaff->ReadVisibility(staff);

    if (staff.attribute("uly") && (m_doc->GetType() == Transcription)) {
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

bool MEIInput::ReadStaffChildren(Object *parent, pugi::xml_node parentNode)
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
        // xml comment
        else if (std::string(current.name()) == "") {
            success = ReadXMLComment(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <staff>", current.name());
        }
    }
    return success;
}

bool MEIInput::ReadLayer(Object *parent, pugi::xml_node layer)
{
    Layer *vrvLayer = new Layer();
    SetMeiUuid(layer, vrvLayer);

    vrvLayer->ReadNInteger(layer);
    vrvLayer->ReadTyped(layer);
    vrvLayer->ReadVisibility(layer);

    if (!vrvLayer->HasN()) {
        LogWarning("Missing @n on <layer>, filled by order");
    }
    else if (vrvLayer->GetN() == 0) {
        LogWarning("Value @n='0' on <layer> might yield unpredictable results");
    }

    parent->AddChild(vrvLayer);
    ReadUnsupportedAttr(layer, vrvLayer);
    return ReadLayerChildren(vrvLayer, layer);
}

bool MEIInput::ReadLayerChildren(Object *parent, pugi::xml_node parentNode, Object *filter)
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
        else if (elementName == "label") {
            success = ReadLabel(parent, xmlElement);
        }
        else if (elementName == "labelAbbr") {
            success = ReadLabelAbbr(parent, xmlElement);
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
        else if (elementName == "meterSigGrp") {
            success = ReadMeterSigGrp(parent, xmlElement);
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
        else if (elementName == "mSpace") {
            success = ReadMSpace(parent, xmlElement);
        }
        else if (elementName == "multiRest") {
            success = ReadMultiRest(parent, xmlElement);
        }
        else if (elementName == "multiRpt") {
            success = ReadMultiRpt(parent, xmlElement);
        }
        else if (elementName == "plica") {
            success = ReadPlica(parent, xmlElement);
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
        else if (elementName == "tabDurSym") {
            success = ReadTabDurSym(parent, xmlElement);
        }
        else if (elementName == "tabGrp") {
            success = ReadTabGrp(parent, xmlElement);
        }
        else if (elementName == "tuplet") {
            success = ReadTuplet(parent, xmlElement);
        }
        else if (elementName == "verse") {
            success = ReadVerse(parent, xmlElement);
        }
        // xml comment
        else if (elementName == "") {
            success = ReadXMLComment(parent, xmlElement);
        }
        // unknown
        else {
            LogWarning("Element <%s> is unknown and will be ignored", xmlElement.name());
        }
    }
    return success;
}

bool MEIInput::ReadLayerElement(pugi::xml_node element, LayerElement *object)
{
    if (element.attribute("ulx") && (m_doc->GetType() == Transcription)) {
        object->m_xAbs = atoi(element.attribute("ulx").value()) * DEFINITION_FACTOR;
        element.remove_attribute("ulx");
    }

    SetMeiUuid(element, object);
    ReadLinkingInterface(element, object);
    object->ReadLabelled(element);
    object->ReadTyped(element);

    return true;
}

bool MEIInput::ReadAccid(Object *parent, pugi::xml_node accid)
{
    Accid *vrvAccid = new Accid();
    ReadLayerElement(accid, vrvAccid);

    ReadPositionInterface(accid, vrvAccid);
    vrvAccid->ReadAccidental(accid);
    vrvAccid->ReadAccidentalGestural(accid);
    vrvAccid->ReadAccidLog(accid);
    vrvAccid->ReadColor(accid);
    vrvAccid->ReadEnclosingChars(accid);
    vrvAccid->ReadExtSym(accid);

    parent->AddChild(vrvAccid);
    ReadUnsupportedAttr(accid, vrvAccid);
    return true;
}

bool MEIInput::ReadArtic(Object *parent, pugi::xml_node artic)
{
    Artic *vrvArtic = new Artic();
    ReadLayerElement(artic, vrvArtic);

    vrvArtic->ReadArticulation(artic);
    vrvArtic->ReadColor(artic);
    vrvArtic->ReadEnclosingChars(artic);
    vrvArtic->ReadExtSym(artic);
    vrvArtic->ReadPlacementRelEvent(artic);

    if (vrvArtic->GetArtic().size() > 1) {
        m_doc->SetMarkup(MARKUP_ARTIC_MULTIVAL);
    }

    parent->AddChild(vrvArtic);
    ReadUnsupportedAttr(artic, vrvArtic);
    return true;
}

bool MEIInput::ReadBarLine(Object *parent, pugi::xml_node barLine)
{
    BarLine *vrvBarLine = new BarLine();
    ReadLayerElement(barLine, vrvBarLine);

    vrvBarLine->ReadBarLineLog(barLine);
    vrvBarLine->ReadColor(barLine);
    vrvBarLine->ReadNNumberLike(barLine);
    vrvBarLine->ReadVisibility(barLine);

    parent->AddChild(vrvBarLine);
    ReadUnsupportedAttr(barLine, vrvBarLine);
    return true;
}

bool MEIInput::ReadBeam(Object *parent, pugi::xml_node beam)
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

bool MEIInput::ReadBeatRpt(Object *parent, pugi::xml_node beatRpt)
{
    BeatRpt *vrvBeatRpt = new BeatRpt();
    ReadLayerElement(beatRpt, vrvBeatRpt);

    vrvBeatRpt->ReadColor(beatRpt);
    vrvBeatRpt->ReadBeatRptVis(beatRpt);

    if (m_version < MEI_4_0_0) {
        UpgradeBeatRptTo_4_0_0(beatRpt, vrvBeatRpt);
    }

    parent->AddChild(vrvBeatRpt);
    ReadUnsupportedAttr(beatRpt, vrvBeatRpt);
    return true;
}

bool MEIInput::ReadBTrem(Object *parent, pugi::xml_node bTrem)
{
    BTrem *vrvBTrem = new BTrem();
    ReadLayerElement(bTrem, vrvBTrem);

    vrvBTrem->ReadBTremLog(bTrem);
    vrvBTrem->ReadTremMeasured(bTrem);

    parent->AddChild(vrvBTrem);
    ReadUnsupportedAttr(bTrem, vrvBTrem);
    return ReadLayerChildren(vrvBTrem, bTrem, vrvBTrem);
}

bool MEIInput::ReadChord(Object *parent, pugi::xml_node chord)
{
    Chord *vrvChord = new Chord();
    ReadLayerElement(chord, vrvChord);

    if (m_version < MEI_4_0_0) {
        if (chord.attribute("size")) {
            chord.remove_attribute("size");
            chord.append_attribute("cue").set_value("true");
        }
    }

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
        m_doc->SetMarkup(MARKUP_ANALYTICAL_TIE);
    }

    parent->AddChild(vrvChord);
    ReadUnsupportedAttr(chord, vrvChord);
    return ReadLayerChildren(vrvChord, chord, vrvChord);
}

bool MEIInput::ReadClef(Object *parent, pugi::xml_node clef)
{
    Clef *vrvClef = new Clef();
    ReadLayerElement(clef, vrvClef);
    ReadFacsimileInterface(clef, vrvClef);

    vrvClef->ReadClefShape(clef);
    vrvClef->ReadColor(clef);
    vrvClef->ReadExtSym(clef);
    vrvClef->ReadLineLoc(clef);
    vrvClef->ReadOctaveDisplacement(clef);
    vrvClef->ReadVisibility(clef);

    parent->AddChild(vrvClef);
    ReadUnsupportedAttr(clef, vrvClef);
    return true;
}

void MEIInput::ReadAccidAttr(pugi::xml_node node, Object *object)
{
    AttAccidental accidental;
    accidental.ReadAccidental(node);
    AttAccidentalGestural accidentalGestural;
    accidentalGestural.ReadAccidentalGestural(node);
    if (accidental.HasAccid() || accidentalGestural.HasAccidGes()) {
        Accid *vrvAccid = new Accid();
        vrvAccid->IsAttribute(true);
        vrvAccid->SetAccid(accidental.GetAccid());
        vrvAccid->SetAccidGes(accidentalGestural.GetAccidGes());
        object->AddChild(vrvAccid);
    }
}

bool MEIInput::ReadCustos(Object *parent, pugi::xml_node custos)
{
    Custos *vrvCustos = new Custos();
    ReadLayerElement(custos, vrvCustos);

    ReadFacsimileInterface(custos, vrvCustos);
    ReadPitchInterface(custos, vrvCustos);
    ReadPositionInterface(custos, vrvCustos);
    vrvCustos->ReadColor(custos);

    ReadAccidAttr(custos, vrvCustos);

    parent->AddChild(vrvCustos);
    ReadUnsupportedAttr(custos, vrvCustos);
    return ReadLayerChildren(vrvCustos, custos, vrvCustos);
}

bool MEIInput::ReadDot(Object *parent, pugi::xml_node dot)
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

bool MEIInput::ReadFTrem(Object *parent, pugi::xml_node fTrem)
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

bool MEIInput::ReadGraceGrp(Object *parent, pugi::xml_node graceGrp)
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

bool MEIInput::ReadHalfmRpt(Object *parent, pugi::xml_node halfmRpt)
{
    HalfmRpt *vrvHalfmRpt = new HalfmRpt();
    ReadLayerElement(halfmRpt, vrvHalfmRpt);

    vrvHalfmRpt->ReadColor(halfmRpt);

    parent->AddChild(vrvHalfmRpt);
    ReadUnsupportedAttr(halfmRpt, vrvHalfmRpt);
    return true;
}

bool MEIInput::ReadKeyAccid(Object *parent, pugi::xml_node keyAccid)
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

bool MEIInput::ReadKeySig(Object *parent, pugi::xml_node keySig)
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

bool MEIInput::ReadLigature(Object *parent, pugi::xml_node ligature)
{
    Ligature *vrvLigature = new Ligature();
    SetMeiUuid(ligature, vrvLigature);

    vrvLigature->ReadLigatureVis(ligature);

    parent->AddChild(vrvLigature);
    ReadUnsupportedAttr(ligature, vrvLigature);
    return ReadLayerChildren(vrvLigature, ligature, vrvLigature);
}

bool MEIInput::ReadMensur(Object *parent, pugi::xml_node mensur)
{
    Mensur *vrvMensur = new Mensur();
    ReadLayerElement(mensur, vrvMensur);

    if (m_version < MEI_4_0_0) {
        if (mensur.attribute("size")) {
            mensur.remove_attribute("size");
            mensur.append_attribute("cue").set_value("true");
        }
    }

    vrvMensur->ReadColor(mensur);
    vrvMensur->ReadCue(mensur);
    vrvMensur->ReadDurationRatio(mensur);
    vrvMensur->ReadMensuralShared(mensur);
    vrvMensur->ReadMensurVis(mensur);
    vrvMensur->ReadSlashCount(mensur);
    vrvMensur->ReadStaffLoc(mensur);

    if (m_version < MEI_5_0_0_dev) {
        UpgradeMensurTo_5_0_0(mensur, vrvMensur);
    }

    parent->AddChild(vrvMensur);
    ReadUnsupportedAttr(mensur, vrvMensur);
    return true;
}

bool MEIInput::ReadMeterSig(Object *parent, pugi::xml_node meterSig)
{
    MeterSig *vrvMeterSig = new MeterSig();
    ReadLayerElement(meterSig, vrvMeterSig);

    vrvMeterSig->ReadMeterSigLog(meterSig);
    vrvMeterSig->ReadMeterSigVis(meterSig);

    parent->AddChild(vrvMeterSig);
    ReadUnsupportedAttr(meterSig, vrvMeterSig);
    return true;
}

bool MEIInput::ReadMRest(Object *parent, pugi::xml_node mRest)
{
    MRest *vrvMRest = new MRest();
    ReadLayerElement(mRest, vrvMRest);
    ReadPositionInterface(mRest, vrvMRest);

    if (m_version < MEI_4_0_0) {
        if (mRest.attribute("size")) {
            mRest.remove_attribute("size");
            mRest.append_attribute("cue").set_value("true");
        }
    }

    vrvMRest->ReadColor(mRest);
    vrvMRest->ReadCue(mRest);
    vrvMRest->ReadFermataPresent(mRest);
    vrvMRest->ReadVisibility(mRest);

    if (vrvMRest->HasFermata()) {
        m_doc->SetMarkup(MARKUP_ANALYTICAL_FERMATA);
    }

    if (m_version < MEI_4_0_0) {
        // as mRest has no durationInterface we simply delete dur.ges on upgrade
        if (mRest.attribute("dur.ges")) mRest.remove_attribute("dur.ges");
    }

    parent->AddChild(vrvMRest);
    ReadUnsupportedAttr(mRest, vrvMRest);
    return true;
}

bool MEIInput::ReadMRpt(Object *parent, pugi::xml_node mRpt)
{
    MRpt *vrvMRpt = new MRpt();
    ReadLayerElement(mRpt, vrvMRpt);

    vrvMRpt->ReadColor(mRpt);

    parent->AddChild(vrvMRpt);
    ReadUnsupportedAttr(mRpt, vrvMRpt);
    return true;
}

bool MEIInput::ReadMRpt2(Object *parent, pugi::xml_node mRpt2)
{
    MRpt2 *vrvMRpt2 = new MRpt2();
    ReadLayerElement(mRpt2, vrvMRpt2);

    vrvMRpt2->ReadColor(mRpt2);

    parent->AddChild(vrvMRpt2);
    ReadUnsupportedAttr(mRpt2, vrvMRpt2);
    return true;
}

bool MEIInput::ReadMSpace(Object *parent, pugi::xml_node mSpace)
{
    MSpace *vrvMSpace = new MSpace();
    ReadLayerElement(mSpace, vrvMSpace);

    parent->AddChild(vrvMSpace);
    ReadUnsupportedAttr(mSpace, vrvMSpace);
    return true;
}

bool MEIInput::ReadMultiRest(Object *parent, pugi::xml_node multiRest)
{
    MultiRest *vrvMultiRest = new MultiRest();
    ReadLayerElement(multiRest, vrvMultiRest);

    ReadPositionInterface(multiRest, vrvMultiRest);
    vrvMultiRest->ReadColor(multiRest);
    vrvMultiRest->ReadMultiRestVis(multiRest);
    vrvMultiRest->ReadNumbered(multiRest);
    vrvMultiRest->ReadWidth(multiRest);

    parent->AddChild(vrvMultiRest);
    ReadUnsupportedAttr(multiRest, vrvMultiRest);
    return true;
}

bool MEIInput::ReadMultiRpt(Object *parent, pugi::xml_node multiRpt)
{
    MultiRpt *vrvMultiRpt = new MultiRpt();
    ReadLayerElement(multiRpt, vrvMultiRpt);

    vrvMultiRpt->ReadNumbered(multiRpt);

    parent->AddChild(vrvMultiRpt);
    ReadUnsupportedAttr(multiRpt, vrvMultiRpt);
    return true;
}

bool MEIInput::ReadNc(Object *parent, pugi::xml_node nc)
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

bool MEIInput::ReadNeume(Object *parent, pugi::xml_node neume)
{
    Neume *vrvNeume = new Neume();
    ReadLayerElement(neume, vrvNeume);
    ReadFacsimileInterface(neume, vrvNeume);

    vrvNeume->ReadColor(neume);

    parent->AddChild(vrvNeume);
    return ReadLayerChildren(vrvNeume, neume, vrvNeume);
}

bool MEIInput::ReadNote(Object *parent, pugi::xml_node note)
{
    Note *vrvNote = new Note();
    ReadLayerElement(note, vrvNote);

    if (m_version < MEI_4_0_0) {
        if (note.attribute("size")) {
            note.remove_attribute("size");
            note.append_attribute("cue").set_value("true");
        }
    }

    ReadDurationInterface(note, vrvNote);
    ReadPitchInterface(note, vrvNote);
    ReadPositionInterface(note, vrvNote);
    vrvNote->ReadColor(note);
    vrvNote->ReadColoration(note);
    vrvNote->ReadCue(note);
    vrvNote->ReadExtSym(note);
    vrvNote->ReadGraced(note);
    vrvNote->ReadMidiVelocity(note);
    vrvNote->ReadNoteGesTab(note);
    vrvNote->ReadNoteHeads(note);
    vrvNote->ReadNoteVisMensural(note);
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
        if (artic.GetArtic().size() > 1) {
            m_doc->SetMarkup(MARKUP_ARTIC_MULTIVAL);
        }
        vrvNote->AddChild(vrvArtic);
    }

    ReadAccidAttr(note, vrvNote);

    if (vrvNote->HasTie()) {
        m_doc->SetMarkup(MARKUP_ANALYTICAL_TIE);
    }

    parent->AddChild(vrvNote);
    ReadUnsupportedAttr(note, vrvNote);
    return ReadLayerChildren(vrvNote, note, vrvNote);
}

bool MEIInput::ReadRest(Object *parent, pugi::xml_node rest)
{
    Rest *vrvRest = new Rest();
    ReadLayerElement(rest, vrvRest);

    if (m_version < MEI_4_0_0) {
        if (rest.attribute("size")) {
            rest.remove_attribute("size");
            rest.append_attribute("cue").set_value("true");
        }
    }

    ReadDurationInterface(rest, vrvRest);
    ReadPositionInterface(rest, vrvRest);
    vrvRest->ReadColor(rest);
    vrvRest->ReadCue(rest);
    vrvRest->ReadExtSym(rest);
    vrvRest->ReadRestVisMensural(rest);

    parent->AddChild(vrvRest);
    ReadUnsupportedAttr(rest, vrvRest);
    return ReadLayerChildren(vrvRest, rest, vrvRest);
}

bool MEIInput::ReadPlica(Object *parent, pugi::xml_node plica)
{
    Plica *vrvPlica = new Plica();
    ReadLayerElement(plica, vrvPlica);

    vrvPlica->ReadPlicaVis(plica);

    parent->AddChild(vrvPlica);
    ReadUnsupportedAttr(plica, vrvPlica);
    return true;
}

bool MEIInput::ReadProport(Object *parent, pugi::xml_node proport)
{
    Proport *vrvProport = new Proport();
    ReadLayerElement(proport, vrvProport);

    vrvProport->ReadDurationRatio(proport);

    parent->AddChild(vrvProport);
    ReadUnsupportedAttr(proport, vrvProport);
    return true;
}

bool MEIInput::ReadSpace(Object *parent, pugi::xml_node space)
{
    Space *vrvSpace = new Space();
    ReadLayerElement(space, vrvSpace);

    ReadDurationInterface(space, vrvSpace);

    parent->AddChild(vrvSpace);
    ReadUnsupportedAttr(space, vrvSpace);
    return true;
}

bool MEIInput::ReadSyl(Object *parent, pugi::xml_node syl)
{
    Syl *vrvSyl = new Syl();
    ReadLayerElement(syl, vrvSyl);

    vrvSyl->ReadLang(syl);
    vrvSyl->ReadTypography(syl);
    vrvSyl->ReadSylLog(syl);

    ReadFacsimileInterface(syl, vrvSyl);

    parent->AddChild(vrvSyl);
    ReadUnsupportedAttr(syl, vrvSyl);
    return ReadTextChildren(vrvSyl, syl, vrvSyl);
}

bool MEIInput::ReadSyllable(Object *parent, pugi::xml_node syllable)
{
    Syllable *vrvSyllable = new Syllable();
    ReadLayerElement(syllable, vrvSyllable);

    vrvSyllable->ReadColor(syllable);
    vrvSyllable->ReadSlashCount(syllable);

    parent->AddChild(vrvSyllable);
    return ReadLayerChildren(vrvSyllable, syllable, vrvSyllable);
}

bool MEIInput::ReadTabDurSym(Object *parent, pugi::xml_node tabRhyhtm)
{
    TabDurSym *vrvTabDurSym = new TabDurSym();
    ReadLayerElement(tabRhyhtm, vrvTabDurSym);

    vrvTabDurSym->ReadNNumberLike(tabRhyhtm);

    parent->AddChild(vrvTabDurSym);
    ReadUnsupportedAttr(tabRhyhtm, vrvTabDurSym);
    return true;
}

bool MEIInput::ReadTabGrp(Object *parent, pugi::xml_node tabGrp)
{
    TabGrp *vrvTabGrp = new TabGrp();
    ReadLayerElement(tabGrp, vrvTabGrp);

    ReadDurationInterface(tabGrp, vrvTabGrp);

    parent->AddChild(vrvTabGrp);
    ReadUnsupportedAttr(tabGrp, vrvTabGrp);
    return ReadLayerChildren(vrvTabGrp, tabGrp, vrvTabGrp);
}

bool MEIInput::ReadTuplet(Object *parent, pugi::xml_node tuplet)
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

bool MEIInput::ReadVerse(Object *parent, pugi::xml_node verse)
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

bool MEIInput::ReadTextChildren(Object *parent, pugi::xml_node parentNode, Object *filter)
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
        // xml comment
        else if (elementName == "") {
            success = ReadXMLComment(parent, xmlElement);
        }
        // unknown
        else {
            LogWarning("Element <%s> is unknown and will be ignored", xmlElement.name());
        }
        i++;
    }
    return success;
}

bool MEIInput::ReadTextElement(pugi::xml_node element, TextElement *object)
{
    SetMeiUuid(element, object);
    object->ReadLabelled(element);
    object->ReadTyped(element);

    return true;
}

bool MEIInput::ReadF(Object *parent, pugi::xml_node f)
{
    F *vrvF = new F();
    ReadTextElement(f, vrvF);

    ReadTimeSpanningInterface(f, vrvF);
    vrvF->ReadExtender(f);

    parent->AddChild(vrvF);
    ReadUnsupportedAttr(f, vrvF);
    return ReadTextChildren(vrvF, f, vrvF);
}

bool MEIInput::ReadFig(Object *parent, pugi::xml_node fig)
{
    Fig *vrvFig = new Fig();
    ReadTextElement(fig, vrvFig);

    ReadAreaPosInterface(fig, vrvFig);

    parent->AddChild(vrvFig);
    ReadUnsupportedAttr(fig, vrvFig);
    return ReadTextChildren(vrvFig, fig, vrvFig);
}

bool MEIInput::ReadLb(Object *parent, pugi::xml_node lb)
{
    Lb *vrvLb = new Lb();
    ReadTextElement(lb, vrvLb);

    parent->AddChild(vrvLb);
    ReadUnsupportedAttr(lb, vrvLb);
    return true;
}

bool MEIInput::ReadNum(Object *parent, pugi::xml_node num)
{
    Num *vrvNum = new Num();
    ReadTextElement(num, vrvNum);

    parent->AddChild(vrvNum);
    ReadUnsupportedAttr(num, vrvNum);
    return ReadTextChildren(vrvNum, num, vrvNum);
}

bool MEIInput::ReadRend(Object *parent, pugi::xml_node rend)
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

bool MEIInput::ReadSvg(Object *parent, pugi::xml_node svg)
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

bool MEIInput::ReadText(Object *parent, pugi::xml_node text, bool trimLeft, bool trimRight)
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

bool MEIInput::ReadAreaPosInterface(pugi::xml_node element, AreaPosInterface *interface)
{
    interface->ReadHorizontalAlign(element);
    interface->ReadVerticalAlign(element);
    return true;
}

bool MEIInput::ReadDurationInterface(pugi::xml_node element, DurationInterface *interface)
{
    if (m_version < MEI_4_0_0) {
        UpgradeDurGesTo_4_0_0(element, interface);
    }

    interface->ReadAugmentDots(element);
    interface->ReadBeamSecondary(element);
    interface->ReadDurationGestural(element);
    interface->ReadDurationLogical(element);
    interface->ReadDurationQuality(element);
    interface->ReadDurationRatio(element);
    interface->ReadFermataPresent(element);
    interface->ReadStaffIdent(element);

    if (interface->HasFermata()) {
        m_doc->SetMarkup(MARKUP_ANALYTICAL_FERMATA);
    }

    return true;
}

bool MEIInput::ReadLinkingInterface(pugi::xml_node element, LinkingInterface *interface)
{
    interface->ReadLinking(element);
    return true;
}

bool MEIInput::ReadFacsimileInterface(pugi::xml_node element, FacsimileInterface *interface)
{
    interface->ReadFacsimile(element);
    return true;
}

bool MEIInput::ReadPitchInterface(pugi::xml_node element, PitchInterface *interface)
{
    interface->ReadNoteGes(element);
    interface->ReadOctave(element);
    interface->ReadPitch(element);
    return true;
}

bool MEIInput::ReadPlistInterface(pugi::xml_node element, PlistInterface *interface)
{
    interface->ReadPlist(element);
    return true;
}

bool MEIInput::ReadPositionInterface(pugi::xml_node element, PositionInterface *interface)
{
    interface->ReadStaffLoc(element);
    interface->ReadStaffLocPitched(element);
    return true;
}

bool MEIInput::ReadScoreDefInterface(pugi::xml_node element, ScoreDefInterface *interface)
{
    interface->ReadLyricStyle(element);
    interface->ReadMidiTempo(element);
    interface->ReadMultinumMeasures(element);
    return true;
}

bool MEIInput::ReadTextDirInterface(pugi::xml_node element, TextDirInterface *interface)
{
    interface->ReadPlacementRelStaff(element);
    return true;
}

bool MEIInput::ReadTimePointInterface(pugi::xml_node element, TimePointInterface *interface)
{
    interface->ReadStaffIdent(element);
    interface->ReadStartId(element);
    interface->ReadTimestampLogical(element);
    return true;
}

bool MEIInput::ReadTimeSpanningInterface(pugi::xml_node element, TimeSpanningInterface *interface)
{
    ReadTimePointInterface(element, interface);
    interface->ReadStartEndId(element);
    interface->ReadTimestamp2Logical(element);
    return true;
}

void MEIInput::ReadUnsupportedAttr(pugi::xml_node element, Object *object)
{
    for (pugi::xml_attribute_iterator ait = element.attributes_begin(); ait != element.attributes_end(); ++ait) {
        object->m_unsupported.push_back(std::make_pair(ait->name(), ait->value()));
    }
}

bool MEIInput::ReadEditorialElement(Object *parent, pugi::xml_node current, EditorialLevel level, Object *filter)
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
        assert(false); // this should never happen, MEIInput::s_editorialElementNames should be updated
        return false;
    }
}

bool MEIInput::ReadEditorialElement(pugi::xml_node element, EditorialElement *object)
{
    SetMeiUuid(element, object);

    object->ReadLabelled(element);
    object->ReadTyped(element);

    return true;
}

bool MEIInput::ReadAbbr(Object *parent, pugi::xml_node abbr, EditorialLevel level, Object *filter)
{
    Abbr *vrvAbbr = new Abbr();
    ReadEditorialElement(abbr, vrvAbbr);

    vrvAbbr->ReadSource(abbr);

    parent->AddChild(vrvAbbr);
    ReadUnsupportedAttr(abbr, vrvAbbr);
    return ReadEditorialChildren(vrvAbbr, abbr, level, filter);
}

bool MEIInput::ReadAdd(Object *parent, pugi::xml_node add, EditorialLevel level, Object *filter)
{
    Add *vrvAdd = new Add();
    ReadEditorialElement(add, vrvAdd);

    vrvAdd->ReadSource(add);

    parent->AddChild(vrvAdd);
    ReadUnsupportedAttr(add, vrvAdd);
    return ReadEditorialChildren(vrvAdd, add, level, filter);
}

bool MEIInput::ReadAnnot(Object *parent, pugi::xml_node annot)
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
    // for Annot we do not load children because they preserved in Annot::m_content
    return true;
}

bool MEIInput::ReadApp(Object *parent, pugi::xml_node app, EditorialLevel level, Object *filter)
{
    if (!m_hasScoreDef) {
        LogError("<app> before any <scoreDef> is not supported");
        return false;
    }
    App *vrvApp = new App(level);
    ReadEditorialElement(app, vrvApp);

    parent->AddChild(vrvApp);
    ReadUnsupportedAttr(app, vrvApp);
    return ReadAppChildren(vrvApp, app, level, filter);
}

bool MEIInput::ReadAppChildren(Object *parent, pugi::xml_node parentNode, EditorialLevel level, Object *filter)
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
        // xml comment
        else if (std::string(current.name()) == "") {
            success = ReadXMLComment(parent, current);
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

bool MEIInput::ReadChoice(Object *parent, pugi::xml_node choice, EditorialLevel level, Object *filter)
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

bool MEIInput::ReadChoiceChildren(Object *parent, pugi::xml_node parentNode, EditorialLevel level, Object *filter)
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
        // xml comment
        else if (std::string(current.name()) == "") {
            success = ReadXMLComment(parent, current);
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

bool MEIInput::ReadCorr(Object *parent, pugi::xml_node corr, EditorialLevel level, Object *filter)
{
    Corr *vrvCorr = new Corr();
    ReadEditorialElement(corr, vrvCorr);

    vrvCorr->ReadSource(corr);

    parent->AddChild(vrvCorr);
    ReadUnsupportedAttr(corr, vrvCorr);
    return ReadEditorialChildren(vrvCorr, corr, level, filter);
}

bool MEIInput::ReadDamage(Object *parent, pugi::xml_node damage, EditorialLevel level, Object *filter)
{
    Damage *vrvDamage = new Damage();
    ReadEditorialElement(damage, vrvDamage);

    vrvDamage->ReadSource(damage);

    parent->AddChild(vrvDamage);
    ReadUnsupportedAttr(damage, vrvDamage);
    return ReadEditorialChildren(vrvDamage, damage, level, filter);
}

bool MEIInput::ReadDel(Object *parent, pugi::xml_node del, EditorialLevel level, Object *filter)
{
    Del *vrvDel = new Del();
    ReadEditorialElement(del, vrvDel);

    vrvDel->ReadSource(del);

    parent->AddChild(vrvDel);
    ReadUnsupportedAttr(del, vrvDel);
    return ReadEditorialChildren(vrvDel, del, level, filter);
}

bool MEIInput::ReadExpan(Object *parent, pugi::xml_node expan, EditorialLevel level, Object *filter)
{
    Expan *vrvExpan = new Expan();
    ReadEditorialElement(expan, vrvExpan);

    vrvExpan->ReadSource(expan);

    parent->AddChild(vrvExpan);
    ReadUnsupportedAttr(expan, vrvExpan);
    return ReadEditorialChildren(vrvExpan, expan, level, filter);
}

bool MEIInput::ReadLem(Object *parent, pugi::xml_node lem, EditorialLevel level, Object *filter)
{
    assert(dynamic_cast<App *>(parent));

    Lem *vrvLem = new Lem();
    // By default make them all hidden. MEIInput::ReadAppChildren will make one visible.
    vrvLem->m_visibility = Hidden;
    ReadEditorialElement(lem, vrvLem);

    vrvLem->ReadSource(lem);

    parent->AddChild(vrvLem);
    ReadUnsupportedAttr(lem, vrvLem);
    return ReadEditorialChildren(vrvLem, lem, level, filter);
}

bool MEIInput::ReadOrig(Object *parent, pugi::xml_node orig, EditorialLevel level, Object *filter)
{
    Orig *vrvOrig = new Orig();
    ReadEditorialElement(orig, vrvOrig);

    vrvOrig->ReadSource(orig);

    parent->AddChild(vrvOrig);
    ReadUnsupportedAttr(orig, vrvOrig);
    return ReadEditorialChildren(vrvOrig, orig, level, filter);
}

bool MEIInput::ReadRdg(Object *parent, pugi::xml_node rdg, EditorialLevel level, Object *filter)
{
    assert(dynamic_cast<App *>(parent));

    Rdg *vrvRdg = new Rdg();
    // By default make them all hidden. MEIInput::ReadAppChildren will make one visible.
    vrvRdg->m_visibility = Hidden;
    ReadEditorialElement(rdg, vrvRdg);

    vrvRdg->ReadSource(rdg);

    parent->AddChild(vrvRdg);
    ReadUnsupportedAttr(rdg, vrvRdg);
    return ReadEditorialChildren(vrvRdg, rdg, level, filter);
}

bool MEIInput::ReadRef(Object *parent, pugi::xml_node ref, EditorialLevel level, Object *filter)
{
    Ref *vrvRef = new Ref();
    ReadEditorialElement(ref, vrvRef);

    parent->AddChild(vrvRef);
    ReadUnsupportedAttr(ref, vrvRef);
    return ReadEditorialChildren(vrvRef, ref, level, filter);
}

bool MEIInput::ReadReg(Object *parent, pugi::xml_node reg, EditorialLevel level, Object *filter)
{
    Reg *vrvReg = new Reg();
    ReadEditorialElement(reg, vrvReg);

    vrvReg->ReadSource(reg);

    parent->AddChild(vrvReg);
    ReadUnsupportedAttr(reg, vrvReg);
    return ReadEditorialChildren(vrvReg, reg, level, filter);
}

bool MEIInput::ReadRestore(Object *parent, pugi::xml_node restore, EditorialLevel level, Object *filter)
{
    Restore *vrvRestore = new Restore();
    ReadEditorialElement(restore, vrvRestore);

    vrvRestore->ReadSource(restore);

    parent->AddChild(vrvRestore);
    ReadUnsupportedAttr(restore, vrvRestore);
    return ReadEditorialChildren(vrvRestore, restore, level, filter);
}

bool MEIInput::ReadSic(Object *parent, pugi::xml_node sic, EditorialLevel level, Object *filter)
{
    Sic *vrvSic = new Sic();
    ReadEditorialElement(sic, vrvSic);

    vrvSic->ReadSource(sic);

    parent->AddChild(vrvSic);
    ReadUnsupportedAttr(sic, vrvSic);
    return ReadEditorialChildren(vrvSic, sic, level, filter);
}

bool MEIInput::ReadSubst(Object *parent, pugi::xml_node subst, EditorialLevel level, Object *filter)
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

bool MEIInput::ReadSubstChildren(Object *parent, pugi::xml_node parentNode, EditorialLevel level, Object *filter)
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
        // xml comment
        else if (std::string(current.name()) == "") {
            success = ReadXMLComment(parent, current);
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

bool MEIInput::ReadSupplied(Object *parent, pugi::xml_node supplied, EditorialLevel level, Object *filter)
{
    Supplied *vrvSupplied = new Supplied();
    ReadEditorialElement(supplied, vrvSupplied);

    vrvSupplied->ReadSource(supplied);

    parent->AddChild(vrvSupplied);
    ReadUnsupportedAttr(supplied, vrvSupplied);
    return ReadEditorialChildren(vrvSupplied, supplied, level, filter);
}

bool MEIInput::ReadUnclear(Object *parent, pugi::xml_node unclear, EditorialLevel level, Object *filter)
{
    Unclear *vrvUnclear = new Unclear();
    ReadEditorialElement(unclear, vrvUnclear);

    vrvUnclear->ReadSource(unclear);

    parent->AddChild(vrvUnclear);
    ReadUnsupportedAttr(unclear, vrvUnclear);
    return ReadEditorialChildren(vrvUnclear, unclear, level, filter);
}

bool MEIInput::ReadEditorialChildren(Object *parent, pugi::xml_node parentNode, EditorialLevel level, Object *filter)
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

bool MEIInput::ReadBeamSpanAsBeam(Measure *measure, pugi::xml_node beamSpan)
{
    if (!measure) {
        LogWarning("Cannot read <beamSpan> within editorial markup");
        return false;
    }

    Beam *beam = new Beam();
    SetMeiUuid(beamSpan, beam);

    LayerElement *start = NULL;
    LayerElement *end = NULL;

    // att.labelled
    if (beamSpan.attribute("label")) {
        beam->SetLabel(beamSpan.attribute("label").value());
    }

    // att.typed
    if (beamSpan.attribute("type")) {
        beam->SetType(beamSpan.attribute("type").value());
    }
    else {
        beam->SetType("beamSpan");
    }

    // att.beam.vis
    if (beamSpan.attribute("color")) {
        beam->SetColor(beamSpan.attribute("color").value());
    }

    // position (pitch)
    if (beamSpan.attribute("startid")) {
        std::string refId = ExtractUuidFragment(beamSpan.attribute("startid").value());
        start = dynamic_cast<LayerElement *>(measure->FindDescendantByUuid(refId));
        if (!start) {
            LogWarning("Element with @startid '%s' not found when trying to read the <beamSpan>", refId.c_str());
        }
    }
    if (beamSpan.attribute("endid")) {
        std::string refId = ExtractUuidFragment(beamSpan.attribute("endid").value());
        end = dynamic_cast<LayerElement *>(measure->FindDescendantByUuid(refId));
        if (!end) {
            LogWarning("Element with @endid '%s' not found when trying to read the <beamSpan>", refId.c_str());
        }
    }
    if (!start || !end) {
        delete beam;
        return false;
    }

    LayerElement *startChild = dynamic_cast<LayerElement *>(start->GetLastAncestorNot(LAYER));
    LayerElement *endChild = dynamic_cast<LayerElement *>(end->GetLastAncestorNot(LAYER));

    if (!startChild || !endChild || (startChild->GetParent() != endChild->GetParent())) {
        LogWarning("Start and end elements for <beamSpan> '%s' not in the same layer", beam->GetUuid().c_str());
        delete beam;
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
        if (element) beam->AddChild(element);
    }
    beam->SetParent(parentLayer);
    parentLayer->InsertChild(beam, startIdx);

    return true;
}

bool MEIInput::ReadTupletSpanAsTuplet(Measure *measure, pugi::xml_node tupletSpan)
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

    // att.labelled
    if (tupletSpan.attribute("label")) {
        tuplet->SetLabel(tupletSpan.attribute("label").value());
    }

    // att.typed
    if (tupletSpan.attribute("type")) {
        tuplet->SetType(tupletSpan.attribute("type").value());
    }
    else {
        tuplet->SetType("tupletSpan");
    }

    // att.duration.ratio
    if (tupletSpan.attribute("num")) {
        tuplet->SetNum(atoi(tupletSpan.attribute("num").value()));
    }
    if (tupletSpan.attribute("numbase")) {
        tuplet->SetNumbase(atoi(tupletSpan.attribute("numbase").value()));
    }

    // att.tuplet.vis
    if (tupletSpan.attribute("bracket.place")) {
        tuplet->SetBracketPlace(converter.StrToStaffrelBasic(tupletSpan.attribute("bracket.place").value()));
    }
    if (tupletSpan.attribute("bracket.visible")) {
        tuplet->SetBracketVisible(converter.StrToBoolean(tupletSpan.attribute("bracket.visible").value()));
    }
    if (tupletSpan.attribute("num.format")) {
        tuplet->SetNumFormat(converter.StrToTupletVisNumformat(tupletSpan.attribute("num.format").value()));
    }
    if (tupletSpan.attribute("color")) {
        tuplet->SetColor(tupletSpan.attribute("color").value());
    }
    if (tupletSpan.attribute("num.place")) {
        tuplet->SetNumPlace(converter.StrToStaffrelBasic(tupletSpan.attribute("num.place").value()));
    }
    if (tupletSpan.attribute("num.visible")) {
        tuplet->SetNumVisible(converter.StrToBoolean(tupletSpan.attribute("num.visible").value()));
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

void MEIInput::SetMeiUuid(pugi::xml_node element, Object *object)
{
    if (!m_comment.empty()) {
        object->SetComment(m_comment);
        m_comment.clear();
    }

    if (!element.attribute("xml:id")) {
        return;
    }

    object->SetUuid(element.attribute("xml:id").value());
    element.remove_attribute("xml:id");
}

DocType MEIInput::StrToDocType(std::string type)
{
    if (type == "raw") return Raw;
    if (type == "rendering") return Rendering;
    if (type == "transcription") return Transcription;
    if (type == "facsimile") return Facs;
    LogWarning("Unknown layout type '%s'", type.c_str());
    return Raw;
}

std::wstring MEIInput::LeftTrim(std::wstring str)
{
    std::wstring::size_type pos = 0;
    while (pos < str.size() && iswspace(str[pos])) pos++;
    str.erase(0, pos);
    return str;
}

std::wstring MEIInput::RightTrim(std::wstring str)
{
    std::wstring::size_type pos = str.size();
    while (pos > 0 && iswspace(str[pos - 1])) pos--;
    str.erase(pos);
    return str;
}

bool MEIInput::ReadXMLComment(Object *object, pugi::xml_node element)
{
    assert(object);

    if (element.next_sibling()) {
        m_comment = element.value();
    }
    else {
        object->SetClosingComment(element.value());
    }
    return true;
}

bool MEIInput::IsEditorialElementName(std::string elementName)
{
    auto i = std::find(MEIInput::s_editorialElementNames.begin(), MEIInput::s_editorialElementNames.end(), elementName);
    if (i != MEIInput::s_editorialElementNames.end()) return true;
    return false;
}

void MEIInput::UpgradeBeatRptTo_4_0_0(pugi::xml_node beatRpt, BeatRpt *vrvBeatRpt)
{
    std::string value;
    if (beatRpt.attribute("rend")) {
        value = beatRpt.attribute("rend").value();
        beatRpt.remove_attribute("rend");
    }
    else if (beatRpt.attribute("form")) {
        value = beatRpt.attribute("form").value();
        beatRpt.remove_attribute("form");
    }
    if (value.empty()) return;
    if ((value == "4") || (value == "8")) {
        vrvBeatRpt->SetSlash(BEATRPT_REND_1);
    }
    else if (value == "16") {
        vrvBeatRpt->SetSlash(BEATRPT_REND_2);
    }
    else if (value == "32") {
        vrvBeatRpt->SetSlash(BEATRPT_REND_3);
    }
    else if (value == "64") {
        vrvBeatRpt->SetSlash(BEATRPT_REND_4);
    }
    else if (value == "128") {
        vrvBeatRpt->SetSlash(BEATRPT_REND_5);
    }
    else if (value == "mixed") {
        vrvBeatRpt->SetSlash(BEATRPT_REND_mixed);
    }
}

void MEIInput::UpgradeDurGesTo_4_0_0(pugi::xml_node element, DurationInterface *interface)
{
    if (element.attribute("dur.ges")) {
        std::string durGes = element.attribute("dur.ges").as_string();
        if (durGes.back() == 'p') {
            interface->SetDurPpq(std::atoi(durGes.c_str()));
        }
        else if (durGes.back() == 'r') {
            durGes.pop_back();
            interface->SetDurRecip(durGes.c_str());
        }
        else if (durGes.back() == 's') {
            durGes.pop_back();
            interface->SetDurReal(std::atof(durGes.c_str()));
        }
        element.remove_attribute("dur.ges");
    }
}

void MEIInput::UpgradeFTremTo_4_0_0(pugi::xml_node fTrem, FTrem *vrvFTrem)
{
    if (fTrem.attribute("slash")) {
        vrvFTrem->SetBeams(vrvFTrem->AttFTremVis::StrToInt(fTrem.attribute("slash").value()));
        fTrem.remove_attribute("slash");
    }
}

void MEIInput::UpgradeMensurTo_5_0_0(pugi::xml_node mensur, Mensur *vrvMensur)
{
    if (vrvMensur->HasTempus() && !vrvMensur->HasSign()) {
        vrvMensur->SetSign((vrvMensur->GetTempus() == TEMPUS_3) ? MENSURATIONSIGN_O : MENSURATIONSIGN_C);
    }
    if (vrvMensur->HasProlatio() && !vrvMensur->HasDot()) {
        vrvMensur->SetDot((vrvMensur->GetProlatio() == PROLATIO_3) ? BOOLEAN_true : BOOLEAN_false);
    }
}

void MEIInput::UpgradeMordentTo_4_0_0(pugi::xml_node mordent, Mordent *vrvMordent)
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

void MEIInput::UpgradeScoreDefElementTo_4_0_0(pugi::xml_node scoreDefElement, ScoreDefElement *vrvScoreDefElement)
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

void MEIInput::UpgradeStaffDefTo_4_0_0(pugi::xml_node staffDef, StaffDef *vrvStaffDef)
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

void MEIInput::UpgradeStaffGrpTo_4_0_0(pugi::xml_node staffGrp, StaffGrp *vrvStaffGrp)
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

void MEIInput::UpgradeTurnTo_4_0_0(pugi::xml_node turn, Turn *vrvTurn)
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

void MEIInput::UpgradeMeasureTo_3_0_0(Measure *measure, System *system)
{
    assert(measure);
    assert(system);
    assert(!measure->IsMeasuredMusic());

    if (system->m_yAbs == VRV_UNSET) return;
    if (system->m_systemRightMar == VRV_UNSET) return;
    if (system->m_systemRightMar == VRV_UNSET) return;

    Page *page = vrv_cast<Page *>(system->GetFirstAncestor(PAGE));
    assert(page);
    measure->m_xAbs = system->m_systemLeftMar;
    measure->m_xAbs2 = page->m_pageWidth - system->m_systemRightMar;
}

void MEIInput::UpgradePageTo_3_0_0(Page *page, Doc *doc)
{
    assert(page);
    assert(doc);

    // Once we have the GetPPU in Page through LibMEI, call this from Doc::SetDrawingPage and
    // use m_unit instead of DEFAULT_UNIT - For the upgraded call Page->SetPPU(12.5);

    page->m_PPUFactor = (25.0 / 2.0 / doc->GetOptions()->m_unit.GetDefault());
    // LogDebug("PPUFactor: %f", m_PPUFactor);
}

bool MEIInput::ReadSurface(Facsimile *parent, pugi::xml_node surface)
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
            LogWarning("Unsupported element <%s> in <surface>", child.name());
        }
    }
    parent->AddChild(vrvSurface);
    return true;
}

bool MEIInput::ReadZone(Surface *parent, pugi::xml_node zone)
{
    assert(parent);
    Zone *vrvZone = new Zone();
    SetMeiUuid(zone, vrvZone);
    vrvZone->ReadCoordinated(zone);
    vrvZone->ReadTyped(zone);
    parent->AddChild(vrvZone);
    return true;
}

bool MEIInput::ReadFacsimile(Doc *doc, pugi::xml_node facsimile)
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
            LogWarning("Unsupported element <%s> in <facsimile>", child.name());
        }
    }
    doc->SetFacsimile(vrvFacsimile);
    return true;
}

} // namespace vrv
