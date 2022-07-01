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
#include "beamspan.h"
#include "beatrpt.h"
#include "bracketspan.h"
#include "breath.h"
#include "btrem.h"
#include "caesura.h"
#include "choice.h"
#include "chord.h"
#include "clef.h"
#include "comparison.h"
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
#include "layerdef.h"
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
#include "pagemilestone.h"
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
#include "systemmilestone.h"
#include "tabdursym.h"
#include "tabgrp.h"
#include "tempo.h"
#include "text.h"
#include "tie.h"
#include "transposition.h"
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
    m_indent = 5;
    m_scoreBasedMEI = false;
    m_ignoreHeader = false;
    m_removeIds = false;

    this->Reset();
    this->ResetFilter();
}

MEIOutput::~MEIOutput() {}

bool MEIOutput::Export()
{

    if (m_removeIds) {
        FindAllReferencedObjectsParams findAllReferencedObjectsParams(&m_referredObjects);
        // When saving page-based MEI we also want to keep IDs for milestone elements
        findAllReferencedObjectsParams.m_milestoneReferences = this->IsPageBasedMEI();
        Functor findAllReferencedObjects(&Object::FindAllReferencedObjects);
        m_doc->Process(&findAllReferencedObjects, &findAllReferencedObjectsParams);
        m_referredObjects.unique();
    }

    try {
        pugi::xml_document meiDoc;

        if (this->HasFilter()) {
            if (!this->IsScoreBasedMEI()) {
                LogError("MEI output with filter is not possible in page-based MEI");
                return false;
            }
            if (m_doc->IsMensuralMusicOnly()) {
                LogError("MEI output with filter is not possible for mensural music");
                return false;
            }
            if (!this->HasValidFilter()) {
                LogError("Invalid filter, please check the input");
                return false;
            }
        }

        // Saving the entire document
        // * With score-based MEI, all mdivs are saved
        // * With page-based MEI, only visible mdivs are saved
        pugi::xml_node decl = meiDoc.prepend_child(pugi::node_declaration);
        decl.append_attribute("version") = "1.0";
        decl.append_attribute("encoding") = "UTF-8";

        // schema processing instruction
        const std::string schema = this->IsPageBasedMEI() ? "https://www.verovio.org/schema/dev/mei-verovio.rng"
                                                          : "https://music-encoding.org/schema/dev/mei-all.rng";
        decl = meiDoc.append_child(pugi::node_declaration);
        decl.set_name("xml-model");
        decl.append_attribute("href") = schema.c_str();
        decl.append_attribute("type") = "application/xml";
        decl.append_attribute("schematypens") = "http://relaxng.org/ns/structure/1.0";

        // schematron processing instruction - currently not working for page-based MEI
        if (!this->IsPageBasedMEI()) {
            decl = meiDoc.append_child(pugi::node_declaration);
            decl.set_name("xml-model");
            decl.append_attribute("href") = schema.c_str();
            decl.append_attribute("type") = "application/xml";
            decl.append_attribute("schematypens") = "http://purl.oclc.org/dsdl/schematron";
        }

        m_mei = meiDoc.append_child("mei");
        m_mei.append_attribute("xmlns") = "http://www.music-encoding.org/ns/mei";
        m_mei.append_attribute("meiversion") = "5.0.0-dev";

        // If the document is mensural, we have to undo the mensural (segments) cast off
        m_doc->ConvertToCastOffMensuralDoc(false);

        // this starts the call of all the functors
        m_doc->Save(this);

        // Redo the mensural segment cast of if necessary
        m_doc->ConvertToCastOffMensuralDoc(true);
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

std::string MEIOutput::GetOutput()
{
    this->Export();

    std::string output = m_streamStringOutput.str();

    this->Reset();

    return output;
}

bool MEIOutput::WriteObject(Object *object)
{
    if (this->IsScoreBasedMEI() && this->HasFilter()) {
        if (!this->ProcessScoreBasedFilter(object)) {
            return true;
        }
    }
    return this->WriteObjectInternal(object, false);
}

bool MEIOutput::WriteObjectInternal(Object *object, bool useCustomScoreDef)
{
    if (object->HasComment()) {
        m_currentNode.append_child(pugi::node_comment).set_value(object->GetComment().c_str());
    }

    // Main containers
    if (object->Is(DOC)) {
        this->WriteDoc(vrv_cast<Doc *>(object));
        m_nodeStack.push_back(m_currentNode);
        return true;
    }

    if (object->Is(MDIV)) {
        const std::string name = (this->IsPageBasedMEI()) ? "mdivb" : "mdiv";
        m_currentNode = m_currentNode.append_child(name.c_str());
        this->WriteMdiv(m_currentNode, vrv_cast<Mdiv *>(object));
    }
    else if (object->Is(PAGES)) {
        if (this->IsPageBasedMEI()) {
            m_currentNode = m_currentNode.append_child("pages");
            this->WritePages(m_currentNode, vrv_cast<Pages *>(object));
        }
        else {
            return true;
        }
    }
    else if (object->Is(SCORE)) {
        m_currentNode = m_currentNode.append_child("score");
        this->WriteScore(m_currentNode, vrv_cast<Score *>(object));
    }

    // Page and content
    else if (object->Is(PAGE)) {
        if (this->IsPageBasedMEI()) {
            m_currentNode = m_currentNode.append_child("page");
            this->WritePage(m_currentNode, vrv_cast<Page *>(object));
        }
        else {
            return true;
        }
    }
    else if (object->Is(SYSTEM)) {
        if (this->IsPageBasedMEI()) {
            m_currentNode = m_currentNode.append_child("system");
            this->WriteSystem(m_currentNode, vrv_cast<System *>(object));
        }
        else {
            return true;
        }
    }

    // System boundaries
    else if (object->Is(ENDING)) {
        m_currentNode = m_currentNode.append_child("ending");
        this->WriteEnding(m_currentNode, vrv_cast<Ending *>(object));
    }
    else if (object->Is(EXPANSION)) {
        m_currentNode = m_currentNode.append_child("expansion");
        this->WriteExpansion(m_currentNode, vrv_cast<Expansion *>(object));
    }
    else if (object->Is(PB)) {
        if (this->IsScoreBasedMEI()) {
            m_currentNode = m_currentNode.append_child("pb");
            this->WritePb(m_currentNode, vrv_cast<Pb *>(object));
        }
        else {
            return true;
        }
    }
    else if (object->Is(SB)) {
        if (this->IsScoreBasedMEI()) {
            m_currentNode = m_currentNode.append_child("sb");
            this->WriteSb(m_currentNode, vrv_cast<Sb *>(object));
        }
        else {
            return true;
        }
    }
    else if (object->Is(SECTION)) {
        const std::string name = (this->IsPageBasedMEI()) ? "secb" : "section";
        m_currentNode = m_currentNode.append_child(name.c_str());
        this->WriteSection(m_currentNode, vrv_cast<Section *>(object));
    }

    // ScoreDef related
    else if (object->Is(GRPSYM)) {
        if (!object->IsAttribute()) m_currentNode = m_currentNode.append_child("grpSym");
        this->WriteGrpSym(m_currentNode, vrv_cast<GrpSym *>(object));
    }
    else if (object->Is(INSTRDEF)) {
        m_currentNode = m_currentNode.append_child("instrDef");
        this->WriteInstrDef(m_currentNode, vrv_cast<InstrDef *>(object));
    }
    else if (object->Is(LABEL)) {
        m_currentNode = m_currentNode.append_child("label");
        this->WriteLabel(m_currentNode, vrv_cast<Label *>(object));
    }
    else if (object->Is(LABELABBR)) {
        m_currentNode = m_currentNode.append_child("labelAbbr");
        this->WriteLabelAbbr(m_currentNode, vrv_cast<LabelAbbr *>(object));
    }
    else if (object->Is(LAYERDEF)) {
        m_currentNode = m_currentNode.append_child("layerDef");
        this->WriteLayerDef(m_currentNode, vrv_cast<LayerDef *>(object));
    }
    else if (object->Is(METERSIGGRP)) {
        m_currentNode = m_currentNode.append_child("meterSigGrp");
        this->WriteMeterSigGrp(m_currentNode, vrv_cast<MeterSigGrp *>(object));
    }
    else if (object->Is(SCOREDEF)) {
        m_currentNode = m_currentNode.append_child("scoreDef");
        this->WriteScoreDef(m_currentNode, vrv_cast<ScoreDef *>(object));
    }
    else if (object->Is(PGFOOT)) {
        m_currentNode = m_currentNode.append_child("pgFoot");
        this->WritePgFoot(m_currentNode, vrv_cast<PgFoot *>(object));
    }
    else if (object->Is(PGFOOT2)) {
        m_currentNode = m_currentNode.append_child("pgFoot2");
        this->WritePgFoot2(m_currentNode, vrv_cast<PgFoot2 *>(object));
    }
    else if (object->Is(PGHEAD)) {
        m_currentNode = m_currentNode.append_child("pgHead");
        this->WritePgHead(m_currentNode, vrv_cast<PgHead *>(object));
    }
    else if (object->Is(PGHEAD2)) {
        m_currentNode = m_currentNode.append_child("pgHead2");
        this->WritePgHead2(m_currentNode, vrv_cast<PgHead2 *>(object));
    }
    else if (object->Is(STAFFGRP)) {
        m_currentNode = m_currentNode.append_child("staffGrp");
        this->WriteStaffGrp(m_currentNode, vrv_cast<StaffGrp *>(object));
    }
    else if (object->Is(STAFFDEF)) {
        m_currentNode = m_currentNode.append_child("staffDef");
        this->WriteStaffDef(m_currentNode, vrv_cast<StaffDef *>(object));
    }
    else if (object->Is(TUNING)) {
        m_currentNode = m_currentNode.append_child("tuning");
        this->WriteTuning(m_currentNode, vrv_cast<Tuning *>(object));
    }
    else if (object->Is(COURSE)) {
        m_currentNode = m_currentNode.append_child("course");
        this->WriteCourse(m_currentNode, vrv_cast<Course *>(object));
    }
    else if (object->Is(MEASURE)) {
        m_currentNode = m_currentNode.append_child("measure");
        this->WriteMeasure(m_currentNode, vrv_cast<Measure *>(object));
    }
    else if (object->Is(STAFF)) {
        m_currentNode = m_currentNode.append_child("staff");
        this->WriteStaff(m_currentNode, vrv_cast<Staff *>(object));
    }
    else if (object->Is(LAYER)) {
        m_currentNode = m_currentNode.append_child("layer");
        this->WriteLayer(m_currentNode, vrv_cast<Layer *>(object));
    }

    // Measure elements
    else if (object->Is(ANCHOREDTEXT)) {
        m_currentNode = m_currentNode.append_child("anchoredText");
        this->WriteAnchoredText(m_currentNode, vrv_cast<AnchoredText *>(object));
    }
    else if (object->Is(ARPEG)) {
        m_currentNode = m_currentNode.append_child("arpeg");
        this->WriteArpeg(m_currentNode, vrv_cast<Arpeg *>(object));
    }
    else if (object->Is(BEAMSPAN)) {
        m_currentNode = m_currentNode.append_child("beamSpan");
        WriteBeamSpan(m_currentNode, dynamic_cast<BeamSpan *>(object));
    }
    else if (object->Is(BRACKETSPAN)) {
        m_currentNode = m_currentNode.append_child("bracketSpan");
        this->WriteBracketSpan(m_currentNode, vrv_cast<BracketSpan *>(object));
    }
    else if (object->Is(BREATH)) {
        m_currentNode = m_currentNode.append_child("breath");
        this->WriteBreath(m_currentNode, vrv_cast<Breath *>(object));
    }
    else if (object->Is(CAESURA)) {
        m_currentNode = m_currentNode.append_child("caesura");
        this->WriteCaesura(m_currentNode, vrv_cast<Caesura *>(object));
    }
    else if (object->Is(DIR)) {
        m_currentNode = m_currentNode.append_child("dir");
        this->WriteDir(m_currentNode, vrv_cast<Dir *>(object));
    }
    else if (object->Is(DYNAM)) {
        m_currentNode = m_currentNode.append_child("dynam");
        this->WriteDynam(m_currentNode, vrv_cast<Dynam *>(object));
    }
    else if (object->Is(FERMATA)) {
        if (!object->IsAttribute()) {
            m_currentNode = m_currentNode.append_child("fermata");
            this->WriteFermata(m_currentNode, vrv_cast<Fermata *>(object));
        }
    }
    else if (object->Is(FING)) {
        m_currentNode = m_currentNode.append_child("fing");
        this->WriteFing(m_currentNode, vrv_cast<Fing *>(object));
    }
    else if (object->Is(HAIRPIN)) {
        m_currentNode = m_currentNode.append_child("hairpin");
        this->WriteHairpin(m_currentNode, vrv_cast<Hairpin *>(object));
    }
    else if (object->Is(HARM)) {
        m_currentNode = m_currentNode.append_child("harm");
        this->WriteHarm(m_currentNode, vrv_cast<Harm *>(object));
    }
    else if (object->Is(LV)) {
        m_currentNode = m_currentNode.append_child("lv");
        this->WriteLv(m_currentNode, vrv_cast<Lv *>(object));
    }
    else if (object->Is(MNUM)) {
        m_currentNode = m_currentNode.append_child("mNum");
        this->WriteMNum(m_currentNode, vrv_cast<MNum *>(object));
    }
    else if (object->Is(MORDENT)) {
        m_currentNode = m_currentNode.append_child("mordent");
        this->WriteMordent(m_currentNode, vrv_cast<Mordent *>(object));
    }
    else if (object->Is(OCTAVE)) {
        m_currentNode = m_currentNode.append_child("octave");
        this->WriteOctave(m_currentNode, vrv_cast<Octave *>(object));
    }
    else if (object->Is(PEDAL)) {
        m_currentNode = m_currentNode.append_child("pedal");
        this->WritePedal(m_currentNode, vrv_cast<Pedal *>(object));
    }
    else if (object->Is(PHRASE)) {
        m_currentNode = m_currentNode.append_child("phrase");
        this->WritePhrase(m_currentNode, vrv_cast<Phrase *>(object));
    }

    else if (object->Is(PITCHINFLECTION)) {
        m_currentNode = m_currentNode.append_child("pitchInfection");
        this->WritePitchInflection(m_currentNode, vrv_cast<PitchInflection *>(object));
    }
    else if (object->Is(REH)) {
        m_currentNode = m_currentNode.append_child("reh");
        this->WriteReh(m_currentNode, vrv_cast<Reh *>(object));
    }
    else if (object->Is(SLUR)) {
        m_currentNode = m_currentNode.append_child("slur");
        this->WriteSlur(m_currentNode, vrv_cast<Slur *>(object));
    }
    else if (object->Is(TEMPO)) {
        m_currentNode = m_currentNode.append_child("tempo");
        this->WriteTempo(m_currentNode, vrv_cast<Tempo *>(object));
    }
    else if (object->Is(TIE)) {
        if (!object->IsAttribute()) {
            m_currentNode = m_currentNode.append_child("tie");
            this->WriteTie(m_currentNode, vrv_cast<Tie *>(object));
        }
    }
    else if (object->Is(TRILL)) {
        m_currentNode = m_currentNode.append_child("trill");
        this->WriteTrill(m_currentNode, vrv_cast<Trill *>(object));
    }
    else if (object->Is(TURN)) {
        m_currentNode = m_currentNode.append_child("turn");
        this->WriteTurn(m_currentNode, vrv_cast<Turn *>(object));
    }

    // Layer elements
    else if (object->Is(ACCID)) {
        // Do not add a node for object representing an attribute
        if (!object->IsAttribute()) m_currentNode = m_currentNode.append_child("accid");
        this->WriteAccid(m_currentNode, vrv_cast<Accid *>(object));
    }
    else if (object->Is(ARTIC)) {
        // Do not add a node for object representing an attribute
        if (!object->IsAttribute()) m_currentNode = m_currentNode.append_child("artic");
        this->WriteArtic(m_currentNode, vrv_cast<Artic *>(object));
    }
    else if (object->Is(BARLINE)) {
        m_currentNode = m_currentNode.append_child("barLine");
        this->WriteBarLine(m_currentNode, vrv_cast<BarLine *>(object));
    }
    else if (object->Is(BEAM)) {
        m_currentNode = m_currentNode.append_child("beam");
        this->WriteBeam(m_currentNode, vrv_cast<Beam *>(object));
    }
    else if (object->Is(BEATRPT)) {
        m_currentNode = m_currentNode.append_child("beatRpt");
        this->WriteBeatRpt(m_currentNode, vrv_cast<BeatRpt *>(object));
    }
    else if (object->Is(BTREM)) {
        m_currentNode = m_currentNode.append_child("bTrem");
        this->WriteBTrem(m_currentNode, vrv_cast<BTrem *>(object));
    }
    else if (object->Is(CHORD)) {
        m_currentNode = m_currentNode.append_child("chord");
        this->WriteChord(m_currentNode, vrv_cast<Chord *>(object));
    }
    else if (object->Is(CLEF)) {
        if (!object->IsAttribute()) m_currentNode = m_currentNode.append_child("clef");
        this->WriteClef(m_currentNode, vrv_cast<Clef *>(object));
    }
    else if (object->Is(CUSTOS)) {
        m_currentNode = m_currentNode.append_child("custos");
        this->WriteCustos(m_currentNode, vrv_cast<Custos *>(object));
    }
    else if (object->Is(DOT)) {
        m_currentNode = m_currentNode.append_child("dot");
        this->WriteDot(m_currentNode, vrv_cast<Dot *>(object));
    }
    else if (object->Is(FTREM)) {
        m_currentNode = m_currentNode.append_child("fTrem");
        this->WriteFTrem(m_currentNode, vrv_cast<FTrem *>(object));
    }
    else if (object->Is(GLISS)) {
        m_currentNode = m_currentNode.append_child("gliss");
        this->WriteGliss(m_currentNode, vrv_cast<Gliss *>(object));
    }
    else if (object->Is(GRACEGRP)) {
        m_currentNode = m_currentNode.append_child("graceGrp");
        this->WriteGraceGrp(m_currentNode, vrv_cast<GraceGrp *>(object));
    }
    else if (object->Is(HALFMRPT)) {
        m_currentNode = m_currentNode.append_child("halfmRpt");
        this->WriteHalfmRpt(m_currentNode, vrv_cast<HalfmRpt *>(object));
    }
    else if (object->Is(KEYACCID)) {
        if (!object->IsAttribute()) {
            m_currentNode = m_currentNode.append_child("keyAccid");
            this->WriteKeyAccid(m_currentNode, vrv_cast<KeyAccid *>(object));
        }
    }
    else if (object->Is(KEYSIG)) {
        if (!object->IsAttribute()) m_currentNode = m_currentNode.append_child("keySig");
        this->WriteKeySig(m_currentNode, vrv_cast<KeySig *>(object));
    }
    else if (object->Is(LIGATURE)) {
        m_currentNode = m_currentNode.append_child("ligature");
        this->WriteLigature(m_currentNode, vrv_cast<Ligature *>(object));
    }
    else if (object->Is(MENSUR)) {
        if (!object->IsAttribute()) m_currentNode = m_currentNode.append_child("mensur");
        this->WriteMensur(m_currentNode, vrv_cast<Mensur *>(object));
    }
    else if (object->Is(METERSIG)) {
        if (!object->IsAttribute()) m_currentNode = m_currentNode.append_child("meterSig");
        this->WriteMeterSig(m_currentNode, vrv_cast<MeterSig *>(object));
    }
    else if (object->Is(MREST)) {
        m_currentNode = m_currentNode.append_child("mRest");
        this->WriteMRest(m_currentNode, vrv_cast<MRest *>(object));
    }
    else if (object->Is(MRPT)) {
        m_currentNode = m_currentNode.append_child("mRpt");
        this->WriteMRpt(m_currentNode, vrv_cast<MRpt *>(object));
    }
    else if (object->Is(MRPT2)) {
        m_currentNode = m_currentNode.append_child("mRpt2");
        this->WriteMRpt2(m_currentNode, vrv_cast<MRpt2 *>(object));
    }
    else if (object->Is(MSPACE)) {
        m_currentNode = m_currentNode.append_child("mSpace");
        this->WriteMSpace(m_currentNode, vrv_cast<MSpace *>(object));
    }
    else if (object->Is(MULTIREST)) {
        m_currentNode = m_currentNode.append_child("multiRest");
        this->WriteMultiRest(m_currentNode, vrv_cast<MultiRest *>(object));
    }
    else if (object->Is(MULTIRPT)) {
        m_currentNode = m_currentNode.append_child("multiRpt");
        this->WriteMultiRpt(m_currentNode, vrv_cast<MultiRpt *>(object));
    }
    else if (object->Is(NC)) {
        m_currentNode = m_currentNode.append_child("nc");
        this->WriteNc(m_currentNode, vrv_cast<Nc *>(object));
    }
    else if (object->Is(NEUME)) {
        m_currentNode = m_currentNode.append_child("neume");
        this->WriteNeume(m_currentNode, vrv_cast<Neume *>(object));
    }
    else if (object->Is(NOTE)) {
        m_currentNode = m_currentNode.append_child("note");
        this->WriteNote(m_currentNode, vrv_cast<Note *>(object));
    }
    else if (object->Is(PLICA)) {
        m_currentNode = m_currentNode.append_child("plica");
        this->WritePlica(m_currentNode, vrv_cast<Plica *>(object));
    }
    else if (object->Is(PROPORT)) {
        m_currentNode = m_currentNode.append_child("proport");
        this->WriteProport(m_currentNode, vrv_cast<Proport *>(object));
    }
    else if (object->Is(REST)) {
        m_currentNode = m_currentNode.append_child("rest");
        this->WriteRest(m_currentNode, vrv_cast<Rest *>(object));
    }
    else if (object->Is(SPACE)) {
        m_currentNode = m_currentNode.append_child("space");
        this->WriteSpace(m_currentNode, vrv_cast<Space *>(object));
    }
    else if (object->Is(SYL)) {
        m_currentNode = m_currentNode.append_child("syl");
        this->WriteSyl(m_currentNode, vrv_cast<Syl *>(object));
    }
    else if (object->Is(SYLLABLE)) {
        m_currentNode = m_currentNode.append_child("syllable");
        this->WriteSyllable(m_currentNode, vrv_cast<Syllable *>(object));
    }
    else if (object->Is(TABDURSYM)) {
        m_currentNode = m_currentNode.append_child("tabDurSym");
        this->WriteTabDurSym(m_currentNode, vrv_cast<TabDurSym *>(object));
    }
    else if (object->Is(TABGRP)) {
        m_currentNode = m_currentNode.append_child("tabGrp");
        this->WriteTabGrp(m_currentNode, vrv_cast<TabGrp *>(object));
    }
    else if (object->Is(TUPLET)) {
        m_currentNode = m_currentNode.append_child("tuplet");
        this->WriteTuplet(m_currentNode, vrv_cast<Tuplet *>(object));
    }
    else if (object->Is(VERSE)) {
        m_currentNode = m_currentNode.append_child("verse");
        this->WriteVerse(m_currentNode, vrv_cast<Verse *>(object));
    }

    // Text elements
    else if (object->Is(FIG)) {
        m_currentNode = m_currentNode.append_child("fig");
        this->WriteFig(m_currentNode, vrv_cast<Fig *>(object));
    }
    else if (object->Is(FIGURE)) {
        m_currentNode = m_currentNode.append_child("f");
        this->WriteF(m_currentNode, vrv_cast<F *>(object));
    }
    else if (object->Is(FB)) {
        m_currentNode = m_currentNode.append_child("fb");
        this->WriteFb(m_currentNode, vrv_cast<Fb *>(object));
    }
    else if (object->Is(LB)) {
        m_currentNode = m_currentNode.append_child("lb");
        this->WriteLb(m_currentNode, vrv_cast<Lb *>(object));
    }
    else if (object->Is(NUM)) {
        m_currentNode = m_currentNode.append_child("num");
        this->WriteNum(m_currentNode, vrv_cast<Num *>(object));
    }
    else if (object->Is(REND)) {
        m_currentNode = m_currentNode.append_child("rend");
        this->WriteRend(m_currentNode, vrv_cast<Rend *>(object));
    }
    else if (object->Is(SVG)) {
        m_currentNode = m_currentNode.append_child("svg");
        this->WriteSvg(m_currentNode, vrv_cast<Svg *>(object));
    }
    else if (object->Is(TEXT)) {
        this->WriteText(m_currentNode, vrv_cast<Text *>(object));
    }

    // Editorial markup
    else if (object->Is(ABBR)) {
        m_currentNode = m_currentNode.append_child("abbr");
        this->WriteAbbr(m_currentNode, vrv_cast<Abbr *>(object));
    }
    else if (object->Is(ADD)) {
        m_currentNode = m_currentNode.append_child("add");
        this->WriteAdd(m_currentNode, vrv_cast<Add *>(object));
    }
    else if (object->Is(ANNOT)) {
        m_currentNode = m_currentNode.append_child("annot");
        this->WriteAnnot(m_currentNode, vrv_cast<Annot *>(object));
    }
    else if (object->Is(APP)) {
        m_currentNode = m_currentNode.append_child("app");
        this->WriteApp(m_currentNode, vrv_cast<App *>(object));
    }
    else if (object->Is(CHOICE)) {
        m_currentNode = m_currentNode.append_child("choice");
        this->WriteChoice(m_currentNode, vrv_cast<Choice *>(object));
    }
    else if (object->Is(CORR)) {
        m_currentNode = m_currentNode.append_child("corr");
        this->WriteCorr(m_currentNode, vrv_cast<Corr *>(object));
    }
    else if (object->Is(DAMAGE)) {
        m_currentNode = m_currentNode.append_child("damage");
        this->WriteDamage(m_currentNode, vrv_cast<Damage *>(object));
    }
    else if (object->Is(DEL)) {
        m_currentNode = m_currentNode.append_child("del");
        this->WriteDel(m_currentNode, vrv_cast<Del *>(object));
    }
    else if (object->Is(EXPAN)) {
        m_currentNode = m_currentNode.append_child("epxan");
        this->WriteExpan(m_currentNode, vrv_cast<Expan *>(object));
    }
    else if (object->Is(LEM)) {
        m_currentNode = m_currentNode.append_child("lem");
        this->WriteLem(m_currentNode, vrv_cast<Lem *>(object));
    }
    else if (object->Is(ORIG)) {
        m_currentNode = m_currentNode.append_child("orig");
        this->WriteOrig(m_currentNode, vrv_cast<Orig *>(object));
    }
    else if (object->Is(RDG)) {
        m_currentNode = m_currentNode.append_child("rdg");
        this->WriteRdg(m_currentNode, vrv_cast<Rdg *>(object));
    }
    else if (object->Is(REF)) {
        m_currentNode = m_currentNode.append_child("ref");
        this->WriteRef(m_currentNode, vrv_cast<Ref *>(object));
    }
    else if (object->Is(REG)) {
        m_currentNode = m_currentNode.append_child("reg");
        this->WriteReg(m_currentNode, vrv_cast<Reg *>(object));
    }
    else if (object->Is(RESTORE)) {
        m_currentNode = m_currentNode.append_child("restore");
        this->WriteRestore(m_currentNode, vrv_cast<Restore *>(object));
    }
    else if (object->Is(SIC)) {
        m_currentNode = m_currentNode.append_child("sic");
        this->WriteSic(m_currentNode, vrv_cast<Sic *>(object));
    }
    else if (object->Is(SUBST)) {
        m_currentNode = m_currentNode.append_child("subst");
        this->WriteSubst(m_currentNode, vrv_cast<Subst *>(object));
    }
    else if (object->Is(SUPPLIED)) {
        m_currentNode = m_currentNode.append_child("supplied");
        this->WriteSupplied(m_currentNode, vrv_cast<Supplied *>(object));
    }
    else if (object->Is(UNCLEAR)) {
        m_currentNode = m_currentNode.append_child("unclear");
        this->WriteUnclear(m_currentNode, vrv_cast<Unclear *>(object));
    }

    // SystemMilestoneEnd - nothing to add - only
    else if (object->Is(SYSTEM_MILESTONE_END)) {
        if (this->IsPageBasedMEI()) {
            m_currentNode = m_currentNode.append_child("milestoneEnd");
            this->WriteSystemMilestoneEnd(m_currentNode, vrv_cast<SystemMilestoneEnd *>(object));
        }
        else {
            return true;
        }
    }
    // PageMilestoneEnd - nothing to add - only
    else if (object->Is(PAGE_MILESTONE_END)) {
        if (this->IsPageBasedMEI()) {
            m_currentNode = m_currentNode.append_child("milestoneEnd");
            this->WritePageMilestoneEnd(m_currentNode, vrv_cast<PageMilestoneEnd *>(object));
        }
        else {
            return true;
        }
    }

    else {
        // Missing output method for the class
        LogError("Output method missing for '%s'", object->GetClassName().c_str());
        assert(false); // let's make it stop because this should not happen
    }

    // Object representing an attribute have no node to push
    if (!object->IsAttribute()) m_nodeStack.push_back(m_currentNode);

    if (object->Is(SCORE)) {
        if (useCustomScoreDef) {
            this->WriteCustomScoreDef();
        }
        else {
            // Save the main scoreDef
            m_doc->GetCurrentScoreDef()->Save(this);
        }
    }

    this->WriteUnsupportedAttr(m_currentNode, object);

    return true;
}

bool MEIOutput::WriteObjectEnd(Object *object)
{
    if (this->IsScoreBasedMEI()) {
        // In score-based MEI, page, pages and system are not written.
        if (object->Is({ PAGE, PAGES, SYSTEM })) {
            return true;
        }

        // Merging boundaries into one xml element
        if (object->IsMilestoneElement()) {
            m_boundaries.push(object->GetMilestoneEnd());
            return true;
        }
        if (object->Is({ PAGE_MILESTONE_END, SYSTEM_MILESTONE_END })) {
            assert(!m_boundaries.empty() && (m_boundaries.top() == object));

            m_boundaries.pop();
        }
    }
    else {
        // In page-based MEI, pb and sb are not written.
        if (object->Is({ PB, SB })) {
            return true;
        }
    }

    if (this->IsScoreBasedMEI() && this->HasFilter()) {
        if (!this->ProcessScoreBasedFilterEnd(object)) {
            return true;
        }
    }

    return this->WriteObjectInternalEnd(object);
}

bool MEIOutput::WriteObjectInternalEnd(Object *object)
{
    // Object representing an attribute have no node to pop
    if (object->IsAttribute()) {
        return true;
    }

    if (object->HasClosingComment()) {
        m_currentNode.append_child(pugi::node_comment).set_value(object->GetClosingComment().c_str());
    }

    m_nodeStack.pop_back();
    m_currentNode = m_nodeStack.back();

    return true;
}

bool MEIOutput::HasFilter() const
{
    return m_hasFilter;
}

void MEIOutput::SetFirstPage(int page)
{
    m_firstPage = page;
    m_hasFilter = true;
}

void MEIOutput::SetLastPage(int page)
{
    m_lastPage = page;
    m_hasFilter = true;
}

void MEIOutput::SetFirstMeasure(const std::string &id)
{
    m_firstMeasureID = id;
    m_hasFilter = true;
}

void MEIOutput::SetLastMeasure(const std::string &id)
{
    m_lastMeasureID = id;
    m_hasFilter = true;
}

void MEIOutput::SetMdiv(const std::string &id)
{
    m_mdivID = id;
    m_hasFilter = true;
}

void MEIOutput::ResetFilter()
{
    m_hasFilter = false;
    m_firstPage = 1;
    m_lastPage = m_doc->GetPageCount();
    m_firstMeasureID = "";
    m_lastMeasureID = "";
    m_mdivID = "";
}

void MEIOutput::Reset()
{
    m_filterMatchLocation = MatchLocation::Before;
    m_firstFilterMatch = NULL;
    m_currentPage = 0;
    m_measureFilterMatchLocation = RangeMatchLocation::BeforeStart;
    m_mdivFilterMatchLocation = MatchLocation::Before;

    m_streamStringOutput.str("");
    m_streamStringOutput.clear();
}

bool MEIOutput::HasValidFilter() const
{
    // Verify page filter
    if ((m_firstPage < 1) || (m_lastPage > m_doc->GetPageCount()) || (m_firstPage > m_lastPage)) {
        return false;
    }

    // Verify measure filter
    Object *firstMeasure = NULL;
    if (!m_firstMeasureID.empty()) {
        firstMeasure = m_doc->FindDescendantByID(m_firstMeasureID);
        if (!firstMeasure || !firstMeasure->Is(MEASURE)) return false;
    }
    Object *lastMeasure = NULL;
    if (!m_lastMeasureID.empty()) {
        lastMeasure = m_doc->FindDescendantByID(m_lastMeasureID);
        if (!lastMeasure || !lastMeasure->Is(MEASURE)) return false;
    }
    if (firstMeasure && lastMeasure && (firstMeasure != lastMeasure)) {
        if (!Object::IsPreOrdered(firstMeasure, lastMeasure)) {
            return false;
        }
    }

    // Verify mdiv filter
    if (!m_mdivID.empty()) {
        Object *mdiv = m_doc->FindDescendantByID(m_mdivID);
        if (!mdiv || !mdiv->Is(MDIV)) return false;
    }

    return true;
}

bool MEIOutput::IsMatchingFilter() const
{
    if (!this->HasFilter()) return true;

    // Check page filter
    if ((m_currentPage < m_firstPage) || (m_currentPage > m_lastPage)) return false;

    // Check measure filter
    if ((m_measureFilterMatchLocation == RangeMatchLocation::BeforeStart)
        || (m_measureFilterMatchLocation == RangeMatchLocation::AfterEnd)) {
        return false;
    }

    // Check mdiv filter
    if (m_mdivFilterMatchLocation != MatchLocation::Here) return false;

    return true;
}

void MEIOutput::UpdateFilter(Object *object)
{
    this->UpdatePageFilter(object);
    this->UpdateMeasureFilter(object);
    this->UpdateMdivFilter(object);
}

void MEIOutput::UpdatePageFilter(Object *object)
{
    // Update page
    if (object->Is(PAGE)) {
        ++m_currentPage;
    }
}

void MEIOutput::UpdateMeasureFilter(Object *object)
{
    // Update measure range
    if (m_firstMeasureID.empty() && (m_measureFilterMatchLocation == RangeMatchLocation::BeforeStart)) {
        m_measureFilterMatchLocation = RangeMatchLocation::BetweenStartEnd;
    }
    if (object->Is(MEASURE)) {
        switch (m_measureFilterMatchLocation) {
            case RangeMatchLocation::BeforeStart:
                if (!m_firstMeasureID.empty() && (object->GetID() == m_firstMeasureID)) {
                    m_measureFilterMatchLocation = RangeMatchLocation::AtStart;
                }
                break;
            case RangeMatchLocation::AtStart:
                if (m_lastMeasureID.empty()) {
                    m_measureFilterMatchLocation = RangeMatchLocation::BetweenStartEnd;
                }
                else if (object->GetID() == m_lastMeasureID) {
                    m_measureFilterMatchLocation = RangeMatchLocation::AtEnd;
                }
                else if (m_firstMeasureID == m_lastMeasureID) {
                    m_measureFilterMatchLocation = RangeMatchLocation::AfterEnd;
                }
                else {
                    m_measureFilterMatchLocation = RangeMatchLocation::BetweenStartEnd;
                }
                break;
            case RangeMatchLocation::BetweenStartEnd:
                if (!m_lastMeasureID.empty() && (object->GetID() == m_lastMeasureID)) {
                    m_measureFilterMatchLocation = RangeMatchLocation::AtEnd;
                }
                break;
            case RangeMatchLocation::AtEnd: m_measureFilterMatchLocation = RangeMatchLocation::AfterEnd; break;
            default: break;
        }
    }
}

void MEIOutput::UpdateMdivFilter(Object *object)
{
    // Update mdiv
    if (m_mdivID.empty() && (m_mdivFilterMatchLocation == MatchLocation::Before)) {
        m_mdivFilterMatchLocation = MatchLocation::Here;
    }
    if (object->Is(MDIV)) {
        switch (m_mdivFilterMatchLocation) {
            case MatchLocation::Before:
                if (!m_mdivID.empty() && (object->GetID() == m_mdivID)) {
                    m_mdivFilterMatchLocation = MatchLocation::Here;
                }
                break;
            case MatchLocation::Here:
                // Mdivs can contain mdivs as children
                // => have to check whether we are still in the subtree of the filtered mdiv
                if (!m_mdivID.empty()) {
                    const bool noneMatchingStackElements = std::none_of(m_objectStack.cbegin(), m_objectStack.cend(),
                        [this](Object *object) { return (object->GetID() == m_mdivID); });
                    if (noneMatchingStackElements) {
                        m_mdivFilterMatchLocation = MatchLocation::After;
                    }
                }
                break;
            default: break;
        }
    }
}

bool MEIOutput::ProcessScoreBasedFilter(Object *object)
{
    // Update current page etc.
    this->UpdateFilter(object);

    if (this->IsMatchingFilter()) {
        // Transition Before => Here
        if (m_filterMatchLocation == MatchLocation::Before) {
            m_filterMatchLocation = MatchLocation::Here;
            m_firstFilterMatch = object;
            this->WriteStackedObjects();
        }
    }
    else {
        // Transition Here => After
        if (m_filterMatchLocation == MatchLocation::Here) {
            m_filterMatchLocation = MatchLocation::After;
            this->WriteStackedObjectsEnd();
        }
    }

    if (!object->Is({ PAGES, PAGE, SYSTEM, SYSTEM_MILESTONE_END, PAGE_MILESTONE_END })) {
        m_objectStack.push_back(object);
    }

    return (m_filterMatchLocation == MatchLocation::Here);
}

bool MEIOutput::ProcessScoreBasedFilterEnd(Object *object)
{
    // Pop current object or merged boundary from stack
    if (!m_objectStack.empty()) {
        m_objectStack.pop_back();
    }

    return (m_filterMatchLocation == MatchLocation::Here);
}

void MEIOutput::WriteStackedObjects()
{
    // Write the objects from the stack
    std::for_each(m_objectStack.cbegin(), m_objectStack.cend(),
        [this](Object *object) { this->WriteObjectInternal(object, true); });
}

void MEIOutput::WriteStackedObjectsEnd()
{
    // Write the objects from the stack in reverse order
    std::for_each(m_objectStack.crbegin(), m_objectStack.crend(),
        [this](Object *object) { this->WriteObjectInternalEnd(object); });
}

void MEIOutput::WriteCustomScoreDef()
{
    // Determine the first measure with respect to the first filter match
    Measure *measure = NULL;
    if (m_firstFilterMatch) {
        if (m_firstFilterMatch->Is(MEASURE)) {
            measure = vrv_cast<Measure *>(m_firstFilterMatch);
        }
        else {
            measure = vrv_cast<Measure *>(m_firstFilterMatch->FindDescendantByType(MEASURE));
        }
    }

    // Detect the scoredef which is used as reference
    ScoreDef *refScoreDef = NULL;
    if (measure) {
        refScoreDef = measure->GetDrawingScoreDef();
        if (!refScoreDef) {
            // Use the system scoredef as fallback
            System *system = vrv_cast<System *>(measure->GetFirstAncestor(SYSTEM));
            if (system) refScoreDef = system->GetDrawingScoreDef();
        }
    }

    if (measure && refScoreDef) {
        // Create a copy of the reference scoredef and adjust it to keep track of clef changes, key signature changes,
        // etc.
        ScoreDef *scoreDef = vrv_cast<ScoreDef *>(refScoreDef->Clone());
        ListOfObjects staffDefs = scoreDef->FindAllDescendantsByType(STAFFDEF);
        for (Object *staffDef : staffDefs) {
            this->AdjustStaffDef(vrv_cast<StaffDef *>(staffDef), measure);
        }

        // Check if labels should be drawn
        bool drawLabels = false;
        System *system = vrv_cast<System *>(measure->GetFirstAncestor(SYSTEM));
        if (system && system->GetDrawingScoreDef()) {
            drawLabels = system->GetDrawingScoreDef()->DrawLabels();
        }
        if (!drawLabels) {
            // If not, replace labels by abbreviation or delete them
            ListOfObjects labels = scoreDef->FindAllDescendantsByType(LABEL);
            for (Object *label : labels) {
                if (!this->AdjustLabel(vrv_cast<Label *>(label))) {
                    label->GetParent()->DeleteChild(label);
                }
            }
        }

        // Save the adjusted score def and delete it afterwards
        scoreDef->Save(this);
        delete scoreDef;
    }
    else {
        m_doc->GetCurrentScoreDef()->Save(this);
    }
}

void MEIOutput::AdjustStaffDef(StaffDef *staffDef, Measure *measure)
{
    assert(staffDef);
    assert(measure);

    // Retrieve the first layer of the corresponding staff: this stores the current
    // drawing elements for clef, key signature, etc.
    AttNIntegerComparison matchN(STAFF, staffDef->GetN());
    Staff *staff = vrv_cast<Staff *>(measure->FindDescendantByComparison(&matchN, 1));
    if (!staff) return;
    Layer *layer = vrv_cast<Layer *>(staff->FindDescendantByType(LAYER));
    if (!layer || !layer->HasStaffDef()) return;
    // Replace any element (clef, keysig, metersig, ...) by its current drawing element
    if (layer->GetStaffDefClef()) {
        Object *clef = staffDef->GetChild(0, CLEF);
        if (clef) staffDef->DeleteChild(clef);
        staffDef->AddChild(layer->GetStaffDefClef()->Clone());
    }
    if (layer->GetStaffDefKeySig()) {
        Object *keySig = staffDef->GetChild(0, KEYSIG);
        if (keySig) staffDef->DeleteChild(keySig);
        staffDef->AddChild(layer->GetStaffDefKeySig()->Clone());
    }
    if (layer->GetStaffDefMensur()) {
        Object *mensur = staffDef->GetChild(0, MENSUR);
        if (mensur) staffDef->DeleteChild(mensur);
        staffDef->AddChild(layer->GetStaffDefMensur()->Clone());
    }
    if (layer->GetStaffDefMeterSigGrp()) {
        Object *meterSigGrp = staffDef->GetChild(0, METERSIGGRP);
        if (meterSigGrp) {
            staffDef->DeleteChild(meterSigGrp);
        }
        else {
            Object *meterSig = staffDef->GetChild(0, METERSIG);
            if (meterSig) staffDef->DeleteChild(meterSig);
        }
        staffDef->AddChild(layer->GetStaffDefMeterSigGrp()->Clone());
    }
    if (layer->GetStaffDefMeterSig()) {
        Object *meterSig = staffDef->GetChild(0, METERSIG);
        if (meterSig) {
            staffDef->DeleteChild(meterSig);
        }
        else {
            Object *meterSigGrp = staffDef->GetChild(0, METERSIGGRP);
            if (meterSigGrp) staffDef->DeleteChild(meterSigGrp);
        }
        staffDef->AddChild(layer->GetStaffDefMeterSig()->Clone());
    }
}

bool MEIOutput::AdjustLabel(Label *label)
{
    assert(label);
    assert(label->GetParent());

    // Check if there is a label abbreviation sibling
    LabelAbbr *abbr = vrv_cast<LabelAbbr *>(label->GetParent()->GetChild(0, LABELABBR));

    // If so, use its text
    if (abbr && (label->GetChildCount(TEXT) == 1)) {
        Text *text = vrv_cast<Text *>(label->GetChild(0, TEXT));
        if (text) {
            text->SetText(abbr->GetText(abbr));
            return true;
        }
    }
    return false;
}

std::string MEIOutput::IDToMeiStr(Object *element)
{
    std::string out = element->GetID();
    // LogDebug("id: %s", out.c_str());
    return out;
}

void MEIOutput::WriteXmlId(pugi::xml_node currentNode, Object *object)
{
    if (m_removeIds) {
        ListOfObjects::iterator it = std::find(m_referredObjects.begin(), m_referredObjects.end(), object);
        if (it == m_referredObjects.end()) return;
        m_referredObjects.erase(it);
    }
    currentNode.append_attribute("xml:id") = IDToMeiStr(object).c_str();
}

bool MEIOutput::WriteDoc(Doc *doc)
{
    assert(doc);
    assert(!m_mei.empty());

    // ---- header ----
    if (!m_ignoreHeader && m_doc->m_header.first_child()) {
        if (!m_doc->GetOptions()->m_transpose.GetValue().empty())
            this->WriteRevisionDesc(m_doc->m_header.first_child());
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

        // revisionDesc
        if (!m_doc->GetOptions()->m_transpose.GetValue().empty()) this->WriteRevisionDesc(meiHead);
    }

    // ---- music ----

    pugi::xml_node music = m_mei.append_child("music");
    Facsimile *facs = doc->GetFacsimile();
    if ((facs != NULL) && (facs->GetChildCount() > 0)) {
        pugi::xml_node facsimile = music.append_child("facsimile");
        this->WriteFacsimile(facsimile, facs);
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

    return true;
}

void MEIOutput::WriteRevisionDesc(pugi::xml_node meiHead)
{
    // Use transposer to convert interval from options to semitones
    const std::string transposition = m_doc->GetOptions()->m_transpose.GetValue();
    Transposer transposer;
    transposer.SetBase600();
    int value = 0;
    std::string keyTonic = "";
    if (transposer.IsValidIntervalName(transposition)) {
        const int interval = transposer.GetInterval(transposition);
        int diatonic = 0;
        int chromatic = 0;
        transposer.IntervalToDiatonicChromatic(diatonic, chromatic, interval);
        value = chromatic;
    }
    else if (transposer.IsValidSemitones(transposition)) {
        value = stoi(transposition);
    }
    else if (transposer.IsValidKeyTonic(transposition)) {
        // Prepare message for the KeyTonic transposition
        TransPitch pitch;
        transposer.GetKeyTonic(transposition, pitch);
        std::string direction = "";
        if (!pitch.m_oct) {
            direction.assign("closest ");
        }
        else if (pitch.m_oct < 0) {
            direction.append(std::to_string(std::abs(pitch.m_oct)) + " next lower ");
        }
        else if (pitch.m_oct > 0) {
            direction.append(std::to_string(std::abs(pitch.m_oct)) + " next higher ");
        }
        keyTonic.append("Transposed to ");
        keyTonic.append(direction);
        keyTonic.append(pitch.GetSimplePitchString());
        keyTonic.append(" by Verovio");
    }

    if (!value && keyTonic.empty()) return;

    // Prepare revisionDest and change element to append
    pugi::xml_node revisionDesc = meiHead.child("revisionDesc");
    if (revisionDesc.empty()) revisionDesc = meiHead.append_child("revisionDesc");
    pugi::xml_node change = revisionDesc.append_child("change");
    // add isodate
    const time_t t = time(0); // get time now
    struct tm *now = localtime(&t);
    std::string dateStr = StringFormat("%d-%02d-%02dT%02d:%02d:%02d", now->tm_year + 1900, now->tm_mon + 1,
        now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
    change.append_attribute("isodate").set_value(dateStr.c_str());
    pugi::xml_node changeDesc = change.append_child("changeDesc");
    pugi::xml_node p1 = changeDesc.append_child("p");

    // Prepare change string
    if (!keyTonic.empty()) {
        p1.text().set(keyTonic.c_str());
    }
    else {
        std::stringstream ss;
        ss << "Transposed";
        if (value > 0) {
            ss << " up " << value << " semitones by Verovio.";
        }
        else {
            ss << " down " << std::abs(value) << " semitones by Verovio.";
        }
        p1.text().set(ss.str().c_str());
    }
}

void MEIOutput::WriteMdiv(pugi::xml_node currentNode, Mdiv *mdiv)
{
    assert(mdiv);

    this->WriteXmlId(currentNode, mdiv);
    mdiv->WriteLabelled(currentNode);
    mdiv->WriteNNumberLike(currentNode);
}

void MEIOutput::WritePages(pugi::xml_node currentNode, Pages *pages)
{
    assert(pages);

    if (this->IsPageBasedMEI()) {
        m_currentNode.append_attribute("type") = DocTypeToStr(m_doc->GetType()).c_str();
        m_currentNode.append_child(pugi::node_comment).set_value("Coordinates in MEI axis direction");
    }

    this->WriteXmlId(currentNode, pages);
    pages->WriteLabelled(currentNode);
    pages->WriteNNumberLike(currentNode);
}

void MEIOutput::WriteScore(pugi::xml_node currentNode, Score *score)
{
    assert(score);

    this->WriteXmlId(currentNode, score);
    score->WriteLabelled(currentNode);
    score->WriteNNumberLike(currentNode);
}

void MEIOutput::WritePage(pugi::xml_node currentNode, Page *page)
{
    assert(page);

    this->WriteXmlId(currentNode, page);
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

    this->WriteXmlId(currentNode, pageElement);
    pageElement->WriteTyped(currentNode);
}

void MEIOutput::WritePageMilestoneEnd(pugi::xml_node currentNode, PageMilestoneEnd *milestoneEnd)
{
    assert(milestoneEnd && milestoneEnd->GetStart());

    this->WritePageElement(currentNode, milestoneEnd);
    currentNode.append_attribute("startid") = ("#" + IDToMeiStr(milestoneEnd->GetStart())).c_str();
    std::string meiElementName = milestoneEnd->GetStart()->GetClassName();
    std::transform(meiElementName.begin(), meiElementName.begin() + 1, meiElementName.begin(), ::tolower);
    currentNode.append_attribute("type") = meiElementName.c_str();
}

void MEIOutput::WriteSystem(pugi::xml_node currentNode, System *system)
{
    assert(system);

    this->WriteXmlId(currentNode, system);
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

    this->WriteXmlId(currentNode, systemElement);
    systemElement->WriteTyped(currentNode);
}

void MEIOutput::WriteSystemMilestoneEnd(pugi::xml_node currentNode, SystemMilestoneEnd *milestoneEnd)
{
    assert(milestoneEnd && milestoneEnd->GetStart());

    this->WriteSystemElement(currentNode, milestoneEnd);
    currentNode.append_attribute("startid") = ("#" + IDToMeiStr(milestoneEnd->GetStart())).c_str();
    std::string meiElementName = milestoneEnd->GetStart()->GetClassName();
    std::transform(meiElementName.begin(), meiElementName.begin() + 1, meiElementName.begin(), ::tolower);
    currentNode.append_attribute("type") = meiElementName.c_str();
}

void MEIOutput::WriteSection(pugi::xml_node currentNode, Section *section)
{
    assert(section);

    this->WriteSystemElement(currentNode, section);
    section->WriteNNumberLike(currentNode);
    section->WriteSectionVis(currentNode);
}

void MEIOutput::WriteEnding(pugi::xml_node currentNode, Ending *ending)
{
    assert(ending);

    this->WriteSystemElement(currentNode, ending);
    ending->WriteLineRend(currentNode);
    ending->WriteNNumberLike(currentNode);
}

void MEIOutput::WriteExpansion(pugi::xml_node currentNode, Expansion *expansion)
{
    assert(expansion);

    this->WritePlistInterface(currentNode, expansion);
    this->WriteSystemElement(currentNode, expansion);
}

void MEIOutput::WritePb(pugi::xml_node currentNode, Pb *pb)
{
    assert(pb);

    this->WriteSystemElement(currentNode, pb);
    pb->WriteNNumberLike(currentNode);
}

void MEIOutput::WriteSb(pugi::xml_node currentNode, Sb *sb)
{
    assert(sb);

    this->WriteSystemElement(currentNode, sb);
    sb->WriteNNumberLike(currentNode);
}

void MEIOutput::WriteScoreDefElement(pugi::xml_node currentNode, ScoreDefElement *scoreDefElement)
{
    assert(scoreDefElement);

    this->WriteXmlId(currentNode, scoreDefElement);
    scoreDefElement->WriteMeasureNumbers(currentNode);
    scoreDefElement->WriteSpacing(currentNode);
    scoreDefElement->WriteSystems(currentNode);
    scoreDefElement->WriteTyped(currentNode);
}

void MEIOutput::WriteScoreDef(pugi::xml_node currentNode, ScoreDef *scoreDef)
{
    assert(scoreDef);

    this->WriteScoreDefElement(currentNode, scoreDef);
    this->WriteScoreDefInterface(currentNode, scoreDef);
    scoreDef->WriteDistances(currentNode);
    scoreDef->WriteEndings(currentNode);
    scoreDef->WriteOptimization(currentNode);
    scoreDef->WriteTimeBase(currentNode);
}

void MEIOutput::WriteRunningElement(pugi::xml_node currentNode, RunningElement *runningElement)
{
    assert(runningElement);

    this->WriteXmlId(currentNode, runningElement);
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

    this->WriteXmlId(currentNode, grpSym);
    grpSym->WriteColor(currentNode);
    grpSym->WriteGrpSymLog(currentNode);
    grpSym->WriteStaffGroupingSym(currentNode);
    grpSym->WriteStartId(currentNode);
    grpSym->WriteStartEndId(currentNode);
}

void MEIOutput::WritePgFoot(pugi::xml_node currentNode, PgFoot *pgFoot)
{
    assert(pgFoot);

    this->WriteRunningElement(currentNode, pgFoot);
}

void MEIOutput::WritePgFoot2(pugi::xml_node currentNode, PgFoot2 *pgFoot2)
{
    assert(pgFoot2);

    this->WriteRunningElement(currentNode, pgFoot2);
}

void MEIOutput::WritePgHead(pugi::xml_node currentNode, PgHead *pgHead)
{
    assert(pgHead);

    this->WriteRunningElement(currentNode, pgHead);
}

void MEIOutput::WritePgHead2(pugi::xml_node currentNode, PgHead2 *pgHead2)
{
    assert(pgHead2);

    this->WriteRunningElement(currentNode, pgHead2);
}

void MEIOutput::WriteStaffGrp(pugi::xml_node currentNode, StaffGrp *staffGrp)
{
    assert(staffGrp);

    this->WriteXmlId(currentNode, staffGrp);
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

    this->WriteScoreDefElement(currentNode, staffDef);
    this->WriteScoreDefInterface(currentNode, staffDef);
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

    this->WriteXmlId(currentNode, instrDef);
    instrDef->WriteChannelized(currentNode);
    instrDef->WriteLabelled(currentNode);
    instrDef->WriteMidiInstrument(currentNode);
    instrDef->WriteNNumberLike(currentNode);
}

void MEIOutput::WriteLabel(pugi::xml_node currentNode, Label *label)
{
    assert(label);

    this->WriteXmlId(currentNode, label);
}

void MEIOutput::WriteLabelAbbr(pugi::xml_node currentNode, LabelAbbr *labelAbbr)
{
    assert(labelAbbr);

    this->WriteXmlId(currentNode, labelAbbr);
}

void MEIOutput::WriteLayerDef(pugi::xml_node currentNode, LayerDef *layerDef)
{
    assert(layerDef);

    layerDef->WriteLabelled(currentNode);
    layerDef->WriteNInteger(currentNode);
    layerDef->WriteTyped(currentNode);

    this->WriteXmlId(currentNode, layerDef);
}

void MEIOutput::WriteTuning(pugi::xml_node currentNode, Tuning *tuning)
{
    assert(tuning);

    this->WriteXmlId(currentNode, tuning);
    tuning->WriteCourseLog(currentNode);
}

void MEIOutput::WriteCourse(pugi::xml_node currentNode, Course *course)
{
    assert(course);

    this->WriteXmlId(currentNode, course);
    course->WriteAccidental(currentNode);
    course->WriteNNumberLike(currentNode);
    course->WriteOctave(currentNode);
    course->WritePitch(currentNode);
}

void MEIOutput::WriteMeasure(pugi::xml_node currentNode, Measure *measure)
{
    assert(measure);

    this->WriteXmlId(currentNode, measure);
    measure->WriteBarring(currentNode);
    measure->WriteMeasureLog(currentNode);
    measure->WriteMeterConformanceBar(currentNode);
    measure->WriteNNumberLike(currentNode);
    measure->WritePointing(currentNode);
    measure->WriteTyped(currentNode);
    // For now we copy the adjusted value of coord.x1 and coord.x2 to xAbs and xAbs2 respectively
    if ((measure->m_xAbs != VRV_UNSET) && (measure->m_xAbs2 != VRV_UNSET)) {
        measure->SetCoordX1(measure->m_xAbs / DEFINITION_FACTOR);
        measure->SetCoordX2(measure->m_xAbs2 / DEFINITION_FACTOR);
        measure->WriteCoordX1(currentNode);
        measure->WriteCoordX2(currentNode);
    }
}

void MEIOutput::WriteMeterSigGrp(pugi::xml_node currentNode, MeterSigGrp *meterSigGrp)
{
    assert(meterSigGrp);

    this->WriteXmlId(currentNode, meterSigGrp);
    this->WriteLinkingInterface(currentNode, meterSigGrp);
    meterSigGrp->WriteBasic(currentNode);
    meterSigGrp->WriteLabelled(currentNode);
    meterSigGrp->WriteMeterSigGrpLog(currentNode);
    meterSigGrp->WriteTyped(currentNode);
}

void MEIOutput::WriteFb(pugi::xml_node currentNode, Fb *fb)
{
    assert(fb);

    this->WriteXmlId(currentNode, fb);
}

void MEIOutput::WriteControlElement(pugi::xml_node currentNode, ControlElement *controlElement)
{
    assert(controlElement);

    this->WriteXmlId(currentNode, controlElement);
    this->WriteLinkingInterface(currentNode, controlElement);
    controlElement->WriteLabelled(currentNode);
    controlElement->WriteTyped(currentNode);
}

void MEIOutput::WriteAnchoredText(pugi::xml_node currentNode, AnchoredText *anchoredText)
{
    assert(anchoredText);

    this->WriteControlElement(currentNode, anchoredText);
    this->WriteTextDirInterface(currentNode, anchoredText);
}

void MEIOutput::WriteArpeg(pugi::xml_node currentNode, Arpeg *arpeg)
{
    assert(arpeg);

    this->WriteControlElement(currentNode, arpeg);
    this->WritePlistInterface(currentNode, arpeg);
    this->WriteTimePointInterface(currentNode, arpeg);
    arpeg->WriteArpegLog(currentNode);
    arpeg->WriteArpegVis(currentNode);
    arpeg->WriteColor(currentNode);
    arpeg->WriteEnclosingChars(currentNode);
}

void MEIOutput::WriteBeamSpan(pugi::xml_node currentNode, BeamSpan *beamSpan)
{
    assert(beamSpan);

    WriteControlElement(currentNode, beamSpan);
    WritePlistInterface(currentNode, beamSpan);
    WriteTimeSpanningInterface(currentNode, beamSpan);
    beamSpan->WriteBeamedWith(currentNode);
    beamSpan->WriteBeamRend(currentNode);
    beamSpan->WriteColor(currentNode);
}

void MEIOutput::WriteBracketSpan(pugi::xml_node currentNode, BracketSpan *bracketSpan)
{
    assert(bracketSpan);

    this->WriteControlElement(currentNode, bracketSpan);
    this->WriteTimeSpanningInterface(currentNode, bracketSpan);
    bracketSpan->WriteBracketSpanLog(currentNode);
    bracketSpan->WriteColor(currentNode);
    bracketSpan->WriteLineRend(currentNode);
    bracketSpan->WriteLineRendBase(currentNode);
}

void MEIOutput::WriteBreath(pugi::xml_node currentNode, Breath *breath)
{
    assert(breath);

    this->WriteControlElement(currentNode, breath);
    this->WriteTimePointInterface(currentNode, breath);
    breath->WriteColor(currentNode);
    breath->WritePlacementRelStaff(currentNode);
}

void MEIOutput::WriteCaesura(pugi::xml_node currentNode, Caesura *caesura)
{
    assert(caesura);

    this->WriteControlElement(currentNode, caesura);
    this->WriteTimePointInterface(currentNode, caesura);
    caesura->WriteColor(currentNode);
    caesura->WritePlacementRelStaff(currentNode);
}

void MEIOutput::WriteDir(pugi::xml_node currentNode, Dir *dir)
{
    assert(dir);

    this->WriteControlElement(currentNode, dir);
    this->WriteTextDirInterface(currentNode, dir);
    this->WriteTimeSpanningInterface(currentNode, dir);
    dir->WriteLang(currentNode);
    dir->WriteLineRendBase(currentNode);
    dir->WriteExtender(currentNode);
    dir->WriteVerticalGroup(currentNode);
}

void MEIOutput::WriteDynam(pugi::xml_node currentNode, Dynam *dynam)
{
    assert(dynam);

    this->WriteControlElement(currentNode, dynam);
    this->WriteTextDirInterface(currentNode, dynam);
    this->WriteTimeSpanningInterface(currentNode, dynam);
    dynam->WriteEnclosingChars(currentNode);
    dynam->WriteExtender(currentNode);
    dynam->WriteLineRendBase(currentNode);
    dynam->WriteMidiValue(currentNode);
    dynam->WriteMidiValue2(currentNode);
    dynam->WriteVerticalGroup(currentNode);
}

void MEIOutput::WriteFermata(pugi::xml_node currentNode, Fermata *fermata)
{
    assert(fermata);

    this->WriteControlElement(currentNode, fermata);
    this->WriteTimePointInterface(currentNode, fermata);
    fermata->WriteColor(currentNode);
    fermata->WriteEnclosingChars(currentNode);
    fermata->WriteExtSym(currentNode);
    fermata->WriteFermataVis(currentNode);
    fermata->WritePlacementRelStaff(currentNode);
}

void MEIOutput::WriteFing(pugi::xml_node currentNode, Fing *fing)
{
    assert(fing);

    this->WriteControlElement(currentNode, fing);
    this->WriteTextDirInterface(currentNode, fing);
    this->WriteTimePointInterface(currentNode, fing);
    fing->WriteNNumberLike(currentNode);
}

void MEIOutput::WriteGliss(pugi::xml_node currentNode, Gliss *gliss)
{
    assert(gliss);

    this->WriteControlElement(currentNode, gliss);
    this->WriteTimeSpanningInterface(currentNode, gliss);
    gliss->WriteColor(currentNode);
    gliss->WriteLineRend(currentNode);
    gliss->WriteLineRendBase(currentNode);
    gliss->WriteNNumberLike(currentNode);
}

void MEIOutput::WriteHairpin(pugi::xml_node currentNode, Hairpin *hairpin)
{
    assert(hairpin);

    this->WriteControlElement(currentNode, hairpin);
    this->WriteTimeSpanningInterface(currentNode, hairpin);
    hairpin->WriteColor(currentNode);
    hairpin->WriteHairpinLog(currentNode);
    hairpin->WriteHairpinVis(currentNode);
    hairpin->WritePlacementRelStaff(currentNode);
    hairpin->WriteVerticalGroup(currentNode);
}

void MEIOutput::WriteHarm(pugi::xml_node currentNode, Harm *harm)
{
    assert(harm);

    this->WriteControlElement(currentNode, harm);
    this->WriteTextDirInterface(currentNode, harm);
    this->WriteTimeSpanningInterface(currentNode, harm);
    harm->WriteLang(currentNode);
    harm->WriteNNumberLike(currentNode);
}

void MEIOutput::WriteLv(pugi::xml_node currentNode, Lv *lv)
{
    assert(lv);

    this->WriteControlElement(currentNode, lv);
    this->WriteTimeSpanningInterface(currentNode, lv);
    lv->WriteColor(currentNode);
    lv->WriteCurvature(currentNode);
    lv->WriteCurveRend(currentNode);
}

void MEIOutput::WriteMNum(pugi::xml_node currentNode, MNum *mNum)
{
    assert(mNum);

    this->WriteControlElement(currentNode, mNum);
    this->WriteTextDirInterface(currentNode, mNum);
    this->WriteTimePointInterface(currentNode, mNum);
    mNum->WriteColor(currentNode);
    mNum->WriteLang(currentNode);
    mNum->WriteTypography(currentNode);
}

void MEIOutput::WriteMordent(pugi::xml_node currentNode, Mordent *mordent)
{
    assert(mordent);

    this->WriteControlElement(currentNode, mordent);
    this->WriteTimePointInterface(currentNode, mordent);
    mordent->WriteColor(currentNode);
    mordent->WriteExtSym(currentNode);
    mordent->WriteOrnamentAccid(currentNode);
    mordent->WritePlacementRelStaff(currentNode);
    mordent->WriteMordentLog(currentNode);
}

void MEIOutput::WriteOctave(pugi::xml_node currentNode, Octave *octave)
{
    assert(octave);

    this->WriteControlElement(currentNode, octave);
    this->WriteTimeSpanningInterface(currentNode, octave);
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

    this->WriteControlElement(currentNode, pedal);
    this->WriteTimeSpanningInterface(currentNode, pedal);
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

    this->WriteSlur(currentNode, phrase);
}

void MEIOutput::WritePitchInflection(pugi::xml_node currentNode, PitchInflection *pitchInflection)
{
    assert(pitchInflection);

    this->WriteControlElement(currentNode, pitchInflection);
    this->WriteTimeSpanningInterface(currentNode, pitchInflection);
}

void MEIOutput::WriteReh(pugi::xml_node currentNode, Reh *reh)
{
    assert(reh);

    this->WriteControlElement(currentNode, reh);
    this->WriteTextDirInterface(currentNode, reh);
    this->WriteTimePointInterface(currentNode, reh);
    reh->WriteColor(currentNode);
    reh->WriteLang(currentNode);
    reh->WriteVerticalGroup(currentNode);
}

void MEIOutput::WriteSlur(pugi::xml_node currentNode, Slur *slur)
{
    assert(slur);

    this->WriteControlElement(currentNode, slur);
    this->WriteTimeSpanningInterface(currentNode, slur);
    slur->WriteColor(currentNode);
    slur->WriteCurvature(currentNode);
    slur->WriteCurveRend(currentNode);
    slur->WriteLayerIdent(currentNode);
}

void MEIOutput::WriteStaff(pugi::xml_node currentNode, Staff *staff)
{
    assert(staff);

    this->WriteXmlId(currentNode, staff);
    staff->WriteFacsimile(currentNode);
    staff->WriteNInteger(currentNode);
    staff->WriteTyped(currentNode);
    staff->WriteVisibility(currentNode);

    // y position
    if (staff->m_yAbs != VRV_UNSET) {
        staff->SetCoordY1(staff->m_yAbs / DEFINITION_FACTOR);
        staff->WriteCoordY1(currentNode);
    }
}

void MEIOutput::WriteTempo(pugi::xml_node currentNode, Tempo *tempo)
{
    assert(tempo);

    this->WriteControlElement(currentNode, tempo);
    this->WriteTextDirInterface(currentNode, tempo);
    this->WriteTimePointInterface(currentNode, tempo);
    tempo->WriteLang(currentNode);
    tempo->WriteMidiTempo(currentNode);
    tempo->WriteMmTempo(currentNode);
}

void MEIOutput::WriteTie(pugi::xml_node currentNode, Tie *tie)
{
    assert(tie);

    this->WriteControlElement(currentNode, tie);
    this->WriteTimeSpanningInterface(currentNode, tie);
    tie->WriteColor(currentNode);
    tie->WriteCurvature(currentNode);
    tie->WriteCurveRend(currentNode);
}

void MEIOutput::WriteTrill(pugi::xml_node currentNode, Trill *trill)
{
    assert(trill);

    this->WriteControlElement(currentNode, trill);
    this->WriteTimeSpanningInterface(currentNode, trill);
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

    this->WriteControlElement(currentNode, turn);
    this->WriteTimePointInterface(currentNode, turn);
    turn->WriteColor(currentNode);
    turn->WriteExtSym(currentNode);
    turn->WriteOrnamentAccid(currentNode);
    turn->WritePlacementRelStaff(currentNode);
    turn->WriteTurnLog(currentNode);
}

void MEIOutput::WriteLayer(pugi::xml_node currentNode, Layer *layer)
{
    assert(layer);

    this->WriteXmlId(currentNode, layer);
    layer->WriteCue(currentNode);
    layer->WriteNInteger(currentNode);
    layer->WriteTyped(currentNode);
    layer->WriteVisibility(currentNode);
}

void MEIOutput::WriteLayerElement(pugi::xml_node currentNode, LayerElement *element)
{
    assert(element);

    this->WriteXmlId(currentNode, element);
    this->WriteLinkingInterface(currentNode, element);
    element->WriteLabelled(currentNode);
    element->WriteTyped(currentNode);
    if (element->m_xAbs != VRV_UNSET) {
        element->SetCoordX1(element->m_xAbs / DEFINITION_FACTOR);
        element->WriteCoordX1(currentNode);
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

    this->WriteLayerElement(currentNode, accid);
    this->WritePositionInterface(currentNode, accid);
    accid->WriteAccidental(currentNode);
    accid->WriteAccidentalGestural(currentNode);
    accid->WriteAccidLog(currentNode);
    accid->WriteColor(currentNode);
    accid->WriteEnclosingChars(currentNode);
    accid->WriteExtSym(currentNode);
    accid->WritePlacementOnStaff(currentNode);
    accid->WritePlacementRelEvent(currentNode);
}

void MEIOutput::WriteArtic(pugi::xml_node currentNode, Artic *artic)
{
    assert(artic);

    // Only write att.articulation if representing an attribute
    if (artic->IsAttribute()) {
        artic->WriteArticulation(currentNode);
        return;
    }

    this->WriteLayerElement(currentNode, artic);
    artic->WriteArticulation(currentNode);
    artic->WriteArticulationGestural(currentNode);
    artic->WriteColor(currentNode);
    artic->WriteEnclosingChars(currentNode);
    artic->WriteExtSym(currentNode);
    artic->WritePlacementRelEvent(currentNode);
}

void MEIOutput::WriteBarLine(pugi::xml_node currentNode, BarLine *barLine)
{
    assert(barLine);

    this->WriteLayerElement(currentNode, barLine);
    barLine->WriteBarLineLog(currentNode);
    barLine->WriteColor(currentNode);
    barLine->WriteNNumberLike(currentNode);
    barLine->WriteVisibility(currentNode);
}

void MEIOutput::WriteBeam(pugi::xml_node currentNode, Beam *beam)
{
    assert(beam);

    this->WriteLayerElement(currentNode, beam);
    beam->WriteBeamedWith(currentNode);
    beam->WriteBeamRend(currentNode);
    beam->WriteColor(currentNode);
    beam->WriteCue(currentNode);
}

void MEIOutput::WriteBeatRpt(pugi::xml_node currentNode, BeatRpt *beatRpt)
{
    assert(beatRpt);

    this->WriteLayerElement(currentNode, beatRpt);
    beatRpt->WriteColor(currentNode);
    beatRpt->WriteBeatRptLog(currentNode);
    beatRpt->WriteBeatRptVis(currentNode);
}

void MEIOutput::WriteBTrem(pugi::xml_node currentNode, BTrem *bTrem)
{
    assert(bTrem);

    this->WriteLayerElement(currentNode, bTrem);
    bTrem->WriteBTremLog(currentNode);
    bTrem->WriteNumbered(currentNode);
    bTrem->WriteNumberPlacement(currentNode);
    bTrem->WriteTremMeasured(currentNode);
}

void MEIOutput::WriteChord(pugi::xml_node currentNode, Chord *chord)
{
    assert(chord);

    this->WriteLayerElement(currentNode, chord);
    this->WriteDurationInterface(currentNode, chord);
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

    this->WriteLayerElement(currentNode, clef);
    this->WriteFacsimileInterface(currentNode, clef);
    clef->WriteClefShape(currentNode);
    clef->WriteColor(currentNode);
    clef->WriteEnclosingChars(currentNode);
    clef->WriteExtSym(currentNode);
    clef->WriteLineLoc(currentNode);
    clef->WriteOctaveDisplacement(currentNode);
    clef->WriteStaffIdent(currentNode);
    clef->WriteVisibility(currentNode);
}

void MEIOutput::WriteCustos(pugi::xml_node currentNode, Custos *custos)
{
    assert(custos);

    this->WriteFacsimileInterface(currentNode, custos);
    this->WritePitchInterface(currentNode, custos);
    this->WritePositionInterface(currentNode, custos);
    this->WriteLayerElement(currentNode, custos);
    custos->WriteColor(currentNode);
    custos->WriteExtSym(currentNode);
}

void MEIOutput::WriteDot(pugi::xml_node currentNode, Dot *dot)
{
    assert(dot);

    this->WriteLayerElement(currentNode, dot);
    this->WritePositionInterface(currentNode, dot);
    dot->WriteColor(currentNode);
    dot->WriteDotLog(currentNode);
}

void MEIOutput::WriteFTrem(pugi::xml_node currentNode, FTrem *fTrem)
{
    assert(fTrem);

    this->WriteLayerElement(currentNode, fTrem);
    fTrem->WriteFTremVis(currentNode);
    fTrem->WriteTremMeasured(currentNode);
}

void MEIOutput::WriteGraceGrp(pugi::xml_node currentNode, GraceGrp *graceGrp)
{
    assert(graceGrp);

    this->WriteLayerElement(currentNode, graceGrp);
    graceGrp->WriteColor(currentNode);
    graceGrp->WriteGraced(currentNode);
    graceGrp->WriteGraceGrpLog(currentNode);
}

void MEIOutput::WriteHalfmRpt(pugi::xml_node currentNode, HalfmRpt *halfmRpt)
{
    assert(halfmRpt);

    this->WriteLayerElement(currentNode, halfmRpt);
    halfmRpt->WriteColor(currentNode);
}

void MEIOutput::WriteKeyAccid(pugi::xml_node currentNode, KeyAccid *keyAccid)
{
    assert(keyAccid);

    this->WriteLayerElement(currentNode, keyAccid);
    this->WritePitchInterface(currentNode, keyAccid);
    this->WritePositionInterface(currentNode, keyAccid);
    keyAccid->WriteAccidental(currentNode);
    keyAccid->WriteColor(currentNode);
    keyAccid->WriteEnclosingChars(currentNode);
    keyAccid->WriteExtSym(currentNode);
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

    this->WriteLayerElement(currentNode, keySig);
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

    this->WriteLayerElement(currentNode, ligature);
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

    this->WriteLayerElement(currentNode, mensur);
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

    this->WriteLayerElement(currentNode, meterSig);
    meterSig->WriteEnclosingChars(currentNode);
    meterSig->WriteMeterSigLog(currentNode);
    meterSig->WriteMeterSigVis(currentNode);
}

void MEIOutput::WriteMRest(pugi::xml_node currentNode, MRest *mRest)
{
    assert(mRest);

    this->WriteLayerElement(currentNode, mRest);
    this->WritePositionInterface(currentNode, mRest);
    mRest->WriteColor(currentNode);
    mRest->WriteCue(currentNode);
    mRest->WriteFermataPresent(currentNode);
    mRest->WriteVisibility(currentNode);
}

void MEIOutput::WriteMRpt(pugi::xml_node currentNode, MRpt *mRpt)
{
    assert(mRpt);

    this->WriteLayerElement(currentNode, mRpt);
    mRpt->WriteColor(currentNode);
    mRpt->WriteNumbered(currentNode);
    mRpt->WriteNumberPlacement(currentNode);
}

void MEIOutput::WriteMRpt2(pugi::xml_node currentNode, MRpt2 *mRpt2)
{
    assert(mRpt2);

    this->WriteLayerElement(currentNode, mRpt2);
    mRpt2->WriteColor(currentNode);
}

void MEIOutput::WriteMSpace(pugi::xml_node currentNode, MSpace *mSpace)
{
    assert(mSpace);

    this->WriteLayerElement(currentNode, mSpace);
}

void MEIOutput::WriteMultiRest(pugi::xml_node currentNode, MultiRest *multiRest)
{
    assert(multiRest);

    this->WriteLayerElement(currentNode, multiRest);
    this->WritePositionInterface(currentNode, multiRest);
    multiRest->WriteColor(currentNode);
    multiRest->WriteMultiRestVis(currentNode);
    multiRest->WriteNumbered(currentNode);
    multiRest->WriteNumberPlacement(currentNode);
    multiRest->WriteWidth(currentNode);
}

void MEIOutput::WriteMultiRpt(pugi::xml_node currentNode, MultiRpt *multiRpt)
{
    assert(multiRpt);

    this->WriteLayerElement(currentNode, multiRpt);
    multiRpt->WriteNumbered(currentNode);
}

void MEIOutput::WriteNc(pugi::xml_node currentNode, Nc *nc)
{
    assert(nc);

    this->WriteLayerElement(currentNode, nc);
    this->WriteDurationInterface(currentNode, nc);
    this->WriteFacsimileInterface(currentNode, nc);
    this->WritePitchInterface(currentNode, nc);
    this->WritePositionInterface(currentNode, nc);
    nc->WriteColor(currentNode);
    nc->WriteIntervalMelodic(currentNode);
    nc->WriteNcForm(currentNode);
}

void MEIOutput::WriteNeume(pugi::xml_node currentNode, Neume *neume)
{
    assert(neume);

    this->WriteLayerElement(currentNode, neume);
    this->WriteFacsimileInterface(currentNode, neume);
    neume->WriteColor(currentNode);
}

void MEIOutput::WriteNote(pugi::xml_node currentNode, Note *note)
{
    assert(note);

    this->WriteLayerElement(currentNode, note);
    this->WriteDurationInterface(currentNode, note);
    this->WritePitchInterface(currentNode, note);
    this->WritePositionInterface(currentNode, note);
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

    this->WriteLayerElement(currentNode, rest);
    this->WriteDurationInterface(currentNode, rest);
    this->WritePositionInterface(currentNode, rest);
    rest->WriteColor(currentNode);
    rest->WriteCue(currentNode);
    rest->WriteExtSym(currentNode);
    rest->WriteRestVisMensural(currentNode);
}

void MEIOutput::WritePlica(pugi::xml_node currentNode, Plica *plica)
{
    assert(plica);

    this->WriteLayerElement(currentNode, plica);
    plica->WritePlicaVis(currentNode);
}

void MEIOutput::WriteProport(pugi::xml_node currentNode, Proport *proport)
{
    assert(proport);

    this->WriteLayerElement(currentNode, proport);
}

void MEIOutput::WriteSpace(pugi::xml_node currentNode, Space *space)
{
    assert(space);

    this->WriteLayerElement(currentNode, space);
    this->WriteDurationInterface(currentNode, space);
}

void MEIOutput::WriteTabDurSym(pugi::xml_node currentNode, TabDurSym *tabDurSym)
{
    assert(tabDurSym);

    this->WriteLayerElement(currentNode, tabDurSym);
    tabDurSym->WriteNNumberLike(currentNode);
}

void MEIOutput::WriteTabGrp(pugi::xml_node currentNode, TabGrp *tabGrp)
{
    assert(tabGrp);

    this->WriteLayerElement(currentNode, tabGrp);
    this->WriteDurationInterface(currentNode, tabGrp);
}

void MEIOutput::WriteTuplet(pugi::xml_node currentNode, Tuplet *tuplet)
{
    assert(tuplet);

    this->WriteLayerElement(currentNode, tuplet);
    tuplet->WriteColor(currentNode);
    tuplet->WriteDurationRatio(currentNode);
    tuplet->WriteNumberPlacement(currentNode);
    tuplet->WriteTupletVis(currentNode);
}

void MEIOutput::WriteVerse(pugi::xml_node currentNode, Verse *verse)
{
    assert(verse);

    this->WriteLayerElement(currentNode, verse);
    verse->WriteColor(currentNode);
    verse->WriteLang(currentNode);
    verse->WriteNInteger(currentNode);
    verse->WriteTypography(currentNode);
}

void MEIOutput::WriteSyl(pugi::xml_node currentNode, Syl *syl)
{
    assert(syl);

    this->WriteLayerElement(currentNode, syl);
    this->WriteFacsimileInterface(currentNode, syl);
    syl->WriteLang(currentNode);
    syl->WriteTypography(currentNode);
    syl->WriteSylLog(currentNode);
}

void MEIOutput::WriteSyllable(pugi::xml_node currentNode, Syllable *syllable)
{
    assert(syllable);

    this->WriteLayerElement(currentNode, syllable);
    syllable->WriteColor(currentNode);
    syllable->WriteSlashCount(currentNode);
}

void MEIOutput::WriteFacsimile(pugi::xml_node currentNode, Facsimile *facsimile)
{
    assert(facsimile);
    this->WriteXmlId(currentNode, facsimile);

    // Write Surface(s)
    for (Object *child = facsimile->GetFirst(); child != NULL; child = facsimile->GetNext()) {
        if (child->GetClassId() == SURFACE) {
            pugi::xml_node childNode = currentNode.append_child("surface");
            this->WriteSurface(childNode, dynamic_cast<Surface *>(child));
        }
        else {
            LogWarning("Unable to write child '%s' of facsimile", child->GetClassName().c_str());
        }
    }
}

void MEIOutput::WriteSurface(pugi::xml_node currentNode, Surface *surface)
{
    assert(surface);
    this->WriteXmlId(currentNode, surface);
    surface->WriteCoordinated(currentNode);
    surface->WriteTyped(currentNode);

    for (Object *child = surface->GetFirst(); child != NULL; child = surface->GetNext()) {
        if (child->GetClassId() == ZONE) {
            pugi::xml_node childNode = currentNode.append_child("zone");
            this->WriteZone(childNode, dynamic_cast<Zone *>(child));
        }
        else {
            LogWarning("Unable to write child '%s' of surface", child->GetClassName().c_str());
        }
    }
}

void MEIOutput::WriteZone(pugi::xml_node currentNode, Zone *zone)
{
    assert(zone);
    this->WriteXmlId(currentNode, zone);
    zone->WriteCoordinated(currentNode);
    zone->WriteTyped(currentNode);
}

void MEIOutput::WriteTextElement(pugi::xml_node currentNode, TextElement *textElement)
{
    assert(textElement);

    this->WriteXmlId(currentNode, textElement);
    textElement->WriteLabelled(currentNode);
    textElement->WriteTyped(currentNode);
}

void MEIOutput::WriteF(pugi::xml_node currentNode, F *f)
{
    assert(f);

    this->WriteTextElement(currentNode, f);
    this->WriteTimeSpanningInterface(currentNode, f);
    f->WriteExtender(currentNode);
}

void MEIOutput::WriteFig(pugi::xml_node currentNode, Fig *fig)
{
    assert(fig);

    this->WriteTextElement(currentNode, fig);
    this->WriteAreaPosInterface(currentNode, fig);
}

void MEIOutput::WriteLb(pugi::xml_node currentNode, Lb *lb)
{
    assert(lb);

    this->WriteTextElement(currentNode, lb);
}

void MEIOutput::WriteNum(pugi::xml_node currentNode, Num *num)
{
    assert(num);

    this->WriteTextElement(currentNode, num);
}

void MEIOutput::WriteRend(pugi::xml_node currentNode, Rend *rend)
{
    assert(rend);

    this->WriteTextElement(currentNode, rend);
    this->WriteAreaPosInterface(currentNode, rend);
    rend->WriteColor(currentNode);
    rend->WriteLang(currentNode);
    rend->WriteTextRendition(currentNode);
    rend->WriteTypography(currentNode);
    rend->WriteWhitespace(currentNode);
}

void MEIOutput::WriteSvg(pugi::xml_node currentNode, Svg *svg)
{
    assert(svg);

    this->WriteXmlId(currentNode, svg);

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

    interface->WriteDurationDefault(element);
    interface->WriteLyricStyle(element);
    interface->WriteMidiTempo(element);
    interface->WriteMultinumMeasures(element);
    interface->WritePianoPedals(element);
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

    this->WriteTimePointInterface(element, interface);
    interface->WriteStartEndId(element);
    interface->WriteTimestamp2Logical(element);
}

void MEIOutput::WriteUnsupportedAttr(pugi::xml_node element, Object *object)
{
    for (auto &pair : object->m_unsupported) {
        if (element.attribute(pair.first.c_str())) {
            LogDebug("Attribute '%s' for '%s' is not supported", pair.first.c_str(), object->GetClassName().c_str());
        }
        else {
            element.append_attribute(pair.first.c_str()) = pair.second.c_str();
        }
    }
}

void MEIOutput::WriteEditorialElement(pugi::xml_node currentNode, EditorialElement *element)
{
    assert(element);

    this->WriteXmlId(currentNode, element);
    element->WriteLabelled(currentNode);
    element->WriteTyped(currentNode);
}

void MEIOutput::WriteAbbr(pugi::xml_node currentNode, Abbr *abbr)
{
    assert(abbr);

    this->WriteEditorialElement(currentNode, abbr);
    abbr->WriteSource(currentNode);
}

void MEIOutput::WriteAdd(pugi::xml_node currentNode, Add *add)
{
    assert(add);

    this->WriteEditorialElement(currentNode, add);
    add->WriteSource(currentNode);
}

void MEIOutput::WriteAnnot(pugi::xml_node currentNode, Annot *annot)
{
    assert(annot);

    this->WriteEditorialElement(currentNode, annot);
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

    this->WriteEditorialElement(currentNode, app);
}

void MEIOutput::WriteChoice(pugi::xml_node currentNode, Choice *choice)
{
    assert(choice);

    this->WriteEditorialElement(currentNode, choice);
}

void MEIOutput::WriteCorr(pugi::xml_node currentNode, Corr *corr)
{
    assert(corr);

    this->WriteEditorialElement(currentNode, corr);
    corr->WriteSource(currentNode);
}

void MEIOutput::WriteDamage(pugi::xml_node currentNode, Damage *damage)
{
    assert(damage);

    this->WriteEditorialElement(currentNode, damage);
    damage->WriteSource(currentNode);
}

void MEIOutput::WriteDel(pugi::xml_node currentNode, Del *del)
{
    assert(del);

    this->WriteEditorialElement(currentNode, del);
    del->WriteSource(currentNode);
}

void MEIOutput::WriteExpan(pugi::xml_node currentNode, Expan *expan)
{
    assert(expan);

    this->WriteEditorialElement(currentNode, expan);
    expan->WriteSource(currentNode);
}

void MEIOutput::WriteLem(pugi::xml_node currentNode, Lem *lem)
{
    assert(lem);

    this->WriteEditorialElement(currentNode, lem);
    lem->WriteSource(currentNode);
}

void MEIOutput::WriteOrig(pugi::xml_node currentNode, Orig *orig)
{
    assert(orig);

    this->WriteEditorialElement(currentNode, orig);
    orig->WriteSource(currentNode);
}

void MEIOutput::WriteRdg(pugi::xml_node currentNode, Rdg *rdg)
{
    assert(rdg);

    this->WriteEditorialElement(currentNode, rdg);
    rdg->WriteSource(currentNode);
}

void MEIOutput::WriteRef(pugi::xml_node currentNode, Ref *ref)
{
    assert(ref);

    this->WriteEditorialElement(currentNode, ref);
}

void MEIOutput::WriteReg(pugi::xml_node currentNode, Reg *reg)
{
    assert(reg);

    this->WriteEditorialElement(currentNode, reg);
    reg->WriteSource(currentNode);
}

void MEIOutput::WriteRestore(pugi::xml_node currentNode, Restore *restore)
{
    assert(restore);

    this->WriteEditorialElement(currentNode, restore);
    restore->WriteSource(currentNode);

    return;
}

void MEIOutput::WriteSic(pugi::xml_node currentNode, Sic *sic)
{
    assert(sic);

    this->WriteEditorialElement(currentNode, sic);
    sic->WriteSource(currentNode);
}

void MEIOutput::WriteSubst(pugi::xml_node currentNode, Subst *subst)
{
    assert(subst);

    this->WriteEditorialElement(currentNode, subst);
}

void MEIOutput::WriteSupplied(pugi::xml_node currentNode, Supplied *supplied)
{
    assert(supplied);

    this->WriteEditorialElement(currentNode, supplied);
    supplied->WriteSource(currentNode);
}

void MEIOutput::WriteUnclear(pugi::xml_node currentNode, Unclear *unclear)
{
    assert(unclear);

    this->WriteEditorialElement(currentNode, unclear);
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
        return this->ReadDoc(root);
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
    if (this->IsEditorialElementName(element)) {
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
        else if (element == "tabGrp") {
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
        else if (element == "tabGrp") {
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
    m_readingScoreBased = true;

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
    if (!facsimile.empty()) {
        this->ReadFacsimile(m_doc, facsimile);
        if (m_doc->GetOptions()->m_useFacsimile.GetValue()) {
            m_doc->SetType(Facs);
            m_doc->m_drawingPageHeight = m_doc->GetFacsimile()->GetMaxY();
            m_doc->m_drawingPageWidth = m_doc->GetFacsimile()->GetMaxX();
        }
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
    pages = body.child("pages");
    m_selectedMdiv = body.child("mdiv");

    if (m_selectedMdiv.empty() && pages.empty()) {
        LogError("No <mdiv> or no <pages> element found in the MEI data");
        return false;
    }
    else if (m_selectedMdiv.empty()) {
        m_readingScoreBased = false;
    }
    // Old page-based files. We skip the mdiv and load the pages element.
    // The mdiv and score boundaries are added by UpgradePageTo_5_0_0.
    // This work only for single page files
    else if (m_selectedMdiv.child("pages") && (m_version == MEI_2013)) {
        pages = m_selectedMdiv.child("pages");
        m_readingScoreBased = false;
    }

    // Reading score-based MEI
    if (m_readingScoreBased) {
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

        if ((m_selectedMdiv.select_nodes(".//score").size() > 0)
            && (m_selectedMdiv.select_nodes(".//pages").size() > 0)) {
            LogError("An <mdiv> with only one <pages> or one <score> descendant must be selected");
            return false;
        }

        const bool allMdivVisible = m_doc->GetOptions()->m_mdivAll.GetValue();
        success = this->ReadMdivChildren(m_doc, body, allMdivVisible);

        if (success) {
            m_doc->ExpandExpansions();
        }

        if (success) {
            m_doc->ConvertToPageBasedDoc();
            m_doc->ConvertMarkupDoc(!m_doc->GetOptions()->m_preserveAnalyticalMarkup.GetValue());
        }

        if (success && !m_hasScoreDef) {
            LogWarning("No scoreDef provided, trying to generate one...");
            success = m_doc->GenerateDocumentScoreDef();
        }
    }
    // Reading page-based MEI
    else {
        success = this->ReadPages(m_doc, pages);

        if (success && !m_hasScoreDef) {
            LogWarning("No scoreDef provided, trying to generate one...");
            success = m_doc->GenerateDocumentScoreDef();
        }
    }

    return success;
}

bool MEIInput::ReadPages(Object *parent, pugi::xml_node pages)
{
    Pages *vrvPages = new Pages();
    this->SetMeiID(pages, vrvPages);

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
    m_layoutInformation = LAYOUT_DONE;

    bool success = true;
    /*
    // We require to have s <scoreDef> as first child of <score>
    pugi::xml_node scoreDef = pages.first_child();
    if (!scoreDef || (std::string(scoreDef.name()) != "scoreDef")) {
        LogWarning("No <scoreDef> provided, trying to proceed... ");
    }
    else {
        // This actually sets the Doc::m_scoreDef
        success = this->ReadScoreDef(vrvPages, scoreDef);
    }

    if (!success) return false;
    */

    // No need to have ReadPagesChildren for this...
    pugi::xml_node current;
    for (current = pages.first_child(); current; current = current.next_sibling()) {
        if (!success) break;
        // page
        if (std::string(current.name()) == "page") {
            success = this->ReadPage(vrvPages, current);
        }
        // xml comment
        else if (std::string(current.name()) == "") {
            success = this->ReadXMLComment(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <pages>", current.name());
        }
    }

    this->ReadUnsupportedAttr(pages, vrvPages);
    return success;
}

bool MEIInput::ReadPage(Object *parent, pugi::xml_node page)
{
    Page *vrvPage = new Page();
    this->SetMeiID(page, vrvPage);

    if ((m_doc->GetType() == Transcription) && (m_version == MEI_2013)) {
        UpgradePageTo_3_0_0(vrvPage, m_doc);
    }

    if (page.attribute("page.height")) {
        vrvPage->m_pageHeight = page.attribute("page.height").as_int() * DEFINITION_FACTOR;
        page.remove_attribute("page.height");
    }
    if (page.attribute("page.width")) {
        vrvPage->m_pageWidth = page.attribute("page.width").as_int() * DEFINITION_FACTOR;
        page.remove_attribute("page.width");
    }
    if (page.attribute("page.botmar")) {
        vrvPage->m_pageMarginBottom = page.attribute("page.botmar").as_int() * DEFINITION_FACTOR;
        page.remove_attribute("page.botmar");
    }
    if (page.attribute("page.leftmar")) {
        vrvPage->m_pageMarginLeft = page.attribute("page.leftmar").as_int() * DEFINITION_FACTOR;
        page.remove_attribute("page.leftmar");
    }
    if (page.attribute("page.rightmar")) {
        vrvPage->m_pageMarginRight = page.attribute("page.rightmar").as_int() * DEFINITION_FACTOR;
        page.remove_attribute("page.rightmar");
    }
    if (page.attribute("page.topmar")) {
        vrvPage->m_pageMarginTop = page.attribute("page.topmar").as_int() * DEFINITION_FACTOR;
        page.remove_attribute("page.topmar");
    }
    if (page.attribute("surface")) {
        vrvPage->m_surface = page.attribute("surface").value();
        page.remove_attribute("surface");
    }
    if (page.attribute("ppu")) {
        vrvPage->m_PPUFactor = page.attribute("ppu").as_double();
    }

    parent->AddChild(vrvPage);
    bool success = this->ReadPageChildren(vrvPage, page);

    if (success && (m_doc->GetType() == Transcription) && (vrvPage->GetPPUFactor() != 1.0)) {
        ApplyPPUFactorParams applyPPUFactorParams;
        Functor applyPPUFactor(&Object::ApplyPPUFactor);
        vrvPage->Process(&applyPPUFactor, &applyPPUFactorParams);
    }

    if ((m_doc->GetType() == Transcription) && (m_version == MEI_2013)) {
        UpgradePageTo_5_0_0(vrvPage);
    }

    this->ReadUnsupportedAttr(page, vrvPage);
    return success;
}

bool MEIInput::ReadPageChildren(Object *parent, pugi::xml_node parentNode)
{
    // If we allow <app> between <page> elements
    // assert(dynamic_cast<Page*>(parent) || dynamic_cast<EditorialElement*>(parent));
    assert(dynamic_cast<Page *>(parent));

    pugi::xml_node current;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        if (std::string(current.name()) == "mdiv") {
            this->ReadMdiv(parent, current, true);
        }
        else if (std::string(current.name()) == "score") {
            this->ReadScore(parent, current);
        }
        else if (std::string(current.name()) == "system") {
            this->ReadSystem(parent, current);
        }
        // mdiv in page-based MEI
        else if (std::string(current.name()) == "mdivb") {
            this->ReadMdiv(parent, current, true);
        }
        else if (std::string(current.name()) == "milestoneEnd") {
            this->ReadPageMilestoneEnd(parent, current);
        }
        // xml comment
        else if (std::string(current.name()) == "") {
            this->ReadXMLComment(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <page>", current.name());
        }
    }

    return true;
}

bool MEIInput::ReadPageMilestoneEnd(Object *parent, pugi::xml_node milestoneEnd)
{
    assert(dynamic_cast<Page *>(parent));

    // Check that we have a @startid
    if (!milestoneEnd.attribute("startid")) {
        LogError("Missing @startid on  milestoneEnd");
        return false;
    }

    // Find the element pointing to it
    std::string startID = milestoneEnd.attribute("startid").value();
    Object *start = m_doc->FindDescendantByID(ExtractIDFragment(startID));
    if (!start) {
        LogError("Could not find start element '%s' for milestoneEnd", startID.c_str());
        return false;
    }

    // Check that it is a page milestone
    PageMilestoneInterface *interface = dynamic_cast<PageMilestoneInterface *>(start);
    if (!interface) {
        LogError("The start element  '%s' is not a page milestone element", startID.c_str());
        return false;
    }

    PageMilestoneEnd *vrvElementEnd = new PageMilestoneEnd(start);
    this->SetMeiID(milestoneEnd, vrvElementEnd);
    interface->SetEnd(vrvElementEnd);

    parent->AddChild(vrvElementEnd);
    return true;
}

bool MEIInput::ReadMdiv(Object *parent, pugi::xml_node mdiv, bool isVisible)
{
    Mdiv *vrvMdiv = new Mdiv();
    this->SetMeiID(mdiv, vrvMdiv);

    vrvMdiv->ReadLabelled(mdiv);
    vrvMdiv->ReadNNumberLike(mdiv);

    parent->AddChild(vrvMdiv);

    if (isVisible) {
        vrvMdiv->MakeVisible();
    }

    this->ReadUnsupportedAttr(mdiv, vrvMdiv);
    return this->ReadMdivChildren(vrvMdiv, mdiv, isVisible);
}

bool MEIInput::ReadMdivChildren(Object *parent, pugi::xml_node parentNode, bool isVisible)
{
    assert(dynamic_cast<Doc *>(parent) || dynamic_cast<Mdiv *>(parent));

    if (!m_readingScoreBased) {
        if (parentNode.first_child()) {
            LogWarning("Unexpected <mdiv> content in page-based MEI");
        }
        return true;
    }

    pugi::xml_node current;
    bool success = true;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        // We make the mdiv visible if already set or if matching the desired selection
        bool makeVisible = (isVisible || (m_selectedMdiv == current));
        if (!success) break;
        if (std::string(current.name()) == "mdiv") {
            success = this->ReadMdiv(parent, current, makeVisible);
        }
        else if (std::string(current.name()) == "score") {
            success = this->ReadScore(parent, current);
            if (parentNode.last_child() != current) {
                LogWarning("Skipping nodes after <score> element");
            }
            break;
        }
        // xml comment
        else if (std::string(current.name()) == "") {
            success = this->ReadXMLComment(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <mdiv>", current.name());
        }
    }

    return success;
}

bool MEIInput::ReadScore(Object *parent, pugi::xml_node score)
{
    Score *vrvScore = new Score();
    this->SetMeiID(score, vrvScore);

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
    bool success = this->ReadScoreDef(vrvScore, scoreDef);

    if (!success) return false;

    pugi::xml_node current;
    for (current = scoreDef.next_sibling(); current; current = current.next_sibling()) {
        if (!success) break;
        this->NormalizeAttributes(current);
        std::string elementName = std::string(current.name());
        // editorial
        if (this->IsEditorialElementName(current.name())) {
            success = this->ReadEditorialElement(vrvScore, current, EDITORIAL_TOPLEVEL);
        }
        // content
        else if (elementName == "ending") {
            success = this->ReadEnding(vrvScore, current);
        }
        else if (elementName == "section") {
            success = this->ReadSection(vrvScore, current);
        }
        else if (elementName == "sb") {
            success = this->ReadSb(vrvScore, current);
        }
        else if (elementName == "pb") {
            success = this->ReadPb(vrvScore, current);
        }
        // xml comment
        else if (std::string(current.name()) == "") {
            success = this->ReadXMLComment(parent, current);
        }
        else {
            LogWarning("Element <%s> within <score> is not supported and will be ignored ", elementName.c_str());
        }
    }

    this->ReadUnsupportedAttr(score, vrvScore);
    return success;
}

bool MEIInput::ReadSection(Object *parent, pugi::xml_node section)
{
    Section *vrvSection = new Section();
    this->SetMeiID(section, vrvSection);

    vrvSection->ReadNNumberLike(section);
    vrvSection->ReadSectionVis(section);

    parent->AddChild(vrvSection);
    this->ReadUnsupportedAttr(section, vrvSection);
    if (m_readingScoreBased) {
        return this->ReadSectionChildren(vrvSection, section);
    }
    else if (section.first_child()) {
        LogWarning("Unexpected <section> content in page-based MEI");
    }
    return true;
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
        this->NormalizeAttributes(current);
        // editorial
        if (this->IsEditorialElementName(current.name())) {
            success = this->ReadEditorialElement(parent, current, EDITORIAL_TOPLEVEL);
        }
        // content
        else if (std::string(current.name()) == "ending") {
            // we should not endings with unmeasured music ... (?)
            assert(!unmeasured);
            success = this->ReadEnding(parent, current);
        }
        else if (std::string(current.name()) == "expansion") {
            success = this->ReadExpansion(parent, current);
        }
        else if (std::string(current.name()) == "scoreDef") {
            success = this->ReadScoreDef(parent, current);
        }
        else if (std::string(current.name()) == "section") {
            success = this->ReadSection(parent, current);
        }
        // pb and sb
        else if (std::string(current.name()) == "pb") {
            success = this->ReadPb(parent, current);
        }
        else if (std::string(current.name()) == "sb") {
            success = this->ReadSb(parent, current);
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
            success = this->ReadStaff(unmeasured, current);
        }
        else if (std::string(current.name()) == "measure") {
            // we should not mix measured and unmeasured music within a system...
            assert(!unmeasured);
            // if (parent->IsEditorialElement()) {
            //    m_hasMeasureWithinEditMarkup = true;
            //}
            success = this->ReadMeasure(parent, current);
        }
        // xml comment
        else if (std::string(current.name()) == "") {
            success = this->ReadXMLComment(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <section>", current.name());
        }
    }
    return success;
}

bool MEIInput::ReadSystemElement(pugi::xml_node element, SystemElement *object)
{
    this->SetMeiID(element, object);
    object->ReadTyped(element);

    return true;
}

bool MEIInput::ReadEnding(Object *parent, pugi::xml_node ending)
{
    Ending *vrvEnding = new Ending();
    this->ReadSystemElement(ending, vrvEnding);

    vrvEnding->ReadLineRend(ending);
    vrvEnding->ReadNNumberLike(ending);

    parent->AddChild(vrvEnding);
    this->ReadUnsupportedAttr(ending, vrvEnding);
    if (m_readingScoreBased) {
        return this->ReadSectionChildren(vrvEnding, ending);
    }
    return true;
}

bool MEIInput::ReadExpansion(Object *parent, pugi::xml_node expansion)
{
    Expansion *vrvExpansion = new Expansion();
    this->ReadSystemElement(expansion, vrvExpansion);
    this->ReadPlistInterface(expansion, vrvExpansion);

    parent->AddChild(vrvExpansion);
    this->ReadUnsupportedAttr(expansion, vrvExpansion);
    if (m_readingScoreBased) {
        return this->ReadSectionChildren(vrvExpansion, expansion);
    }
    return true;
}

bool MEIInput::ReadPb(Object *parent, pugi::xml_node pb)
{
    m_layoutInformation = LAYOUT_ENCODED;

    Pb *vrvPb = new Pb();
    this->ReadSystemElement(pb, vrvPb);

    vrvPb->ReadNNumberLike(pb);

    parent->AddChild(vrvPb);
    this->ReadUnsupportedAttr(pb, vrvPb);
    return true;
}

bool MEIInput::ReadSb(Object *parent, pugi::xml_node sb)
{
    // Consider a <sb> to be enough as an indication that we have layout encoded information
    // This is debatable because having a <pb> might be seen as a requirement for this
    m_layoutInformation = LAYOUT_ENCODED;

    Sb *vrvSb = new Sb();
    this->ReadSystemElement(sb, vrvSb);

    vrvSb->ReadNNumberLike(sb);

    parent->AddChild(vrvSb);
    this->ReadUnsupportedAttr(sb, vrvSb);
    return true;
}

bool MEIInput::ReadSystem(Object *parent, pugi::xml_node system)
{
    // If we allow <app> between <page> elements
    // assert(dynamic_cast<Page*>(parent) || dynamic_cast<EditorialElement*>(parent));
    assert(dynamic_cast<Page *>(parent));

    System *vrvSystem = new System();
    this->SetMeiID(system, vrvSystem);
    vrvSystem->ReadTyped(system);

    if (system.attribute("system.leftmar")) {
        vrvSystem->m_systemLeftMar = system.attribute("system.leftmar").as_int();
        system.remove_attribute("system.leftmar");
    }
    if (system.attribute("system.rightmar")) {
        vrvSystem->m_systemRightMar = system.attribute("system.rightmar").as_int();
        system.remove_attribute("system.rightmar");
    }
    if (system.attribute("uly") && (m_doc->GetType() == Transcription)) {
        vrvSystem->m_yAbs = system.attribute("uly").as_int() * DEFINITION_FACTOR;
        system.remove_attribute("uly");
    }

    parent->AddChild(vrvSystem);
    this->ReadUnsupportedAttr(system, vrvSystem);
    return this->ReadSystemChildren(vrvSystem, system);
}

bool MEIInput::ReadSystemChildren(Object *parent, pugi::xml_node parentNode)
{
    assert(dynamic_cast<System *>(parent) || dynamic_cast<EditorialElement *>(parent));

    bool success = true;
    pugi::xml_node current;
    Measure *unmeasured = NULL;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        if (!success) break;
        this->NormalizeAttributes(current);
        // editorial
        if (this->IsEditorialElementName(current.name())) {
            success = this->ReadEditorialElement(parent, current, EDITORIAL_TOPLEVEL);
        }
        // section
        else if (std::string(current.name()) == "section") {
            success = this->ReadSection(parent, current);
        }
        // section in page-based MEI
        else if (std::string(current.name()) == "secb") {
            success = this->ReadSection(parent, current);
        }
        else if (std::string(current.name()) == "milestoneEnd") {
            success = this->ReadSystemMilestoneEnd(parent, current);
        }
        // content
        else if (std::string(current.name()) == "scoreDef") {
            // we should not have scoredef with unmeasured music within a system... (?)
            assert(!unmeasured);
            this->ReadScoreDef(parent, current);
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
            success = this->ReadStaff(unmeasured, current);
        }
        else if (std::string(current.name()) == "measure") {
            // we should not mix measured and unmeasured music within a system...
            assert(!unmeasured);
            success = this->ReadMeasure(parent, current);
        }
        // xml comment
        else if (std::string(current.name()) == "") {
            success = this->ReadXMLComment(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <system>", current.name());
        }
    }
    return success;
}

bool MEIInput::ReadSystemMilestoneEnd(Object *parent, pugi::xml_node milestoneEnd)
{
    assert(dynamic_cast<System *>(parent));

    // Check that we have a @startid
    if (!milestoneEnd.attribute("startid")) {
        LogError("Missing @startid on  milestoneEnd");
        return false;
    }

    // Find the element pointing to it
    std::string startID = milestoneEnd.attribute("startid").value();
    Object *start = m_doc->FindDescendantByID(ExtractIDFragment(startID));
    if (!start) {
        LogError("Could not find start element '%s' for milestoneEnd", startID.c_str());
        return false;
    }

    // Check that it is a page milestone
    SystemMilestoneInterface *interface = dynamic_cast<SystemMilestoneInterface *>(start);
    if (!interface) {
        LogError("The start element  '%s' is not a system milestone element", startID.c_str());
        return false;
    }

    SystemMilestoneEnd *vrvElementEnd = new SystemMilestoneEnd(start);
    this->SetMeiID(milestoneEnd, vrvElementEnd);
    interface->SetEnd(vrvElementEnd);

    parent->AddChild(vrvElementEnd);
    return true;
}

bool MEIInput::ReadScoreDefElement(pugi::xml_node element, ScoreDefElement *object)
{
    this->SetMeiID(element, object);
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
    // We are reading the score/scoreDef
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
    this->ReadScoreDefElement(scoreDef, vrvScoreDef);

    if (m_version < MEI_4_0_0) {
        UpgradeScoreDefElementTo_4_0_0(scoreDef, vrvScoreDef);
    }

    this->ReadScoreDefInterface(scoreDef, vrvScoreDef);
    vrvScoreDef->ReadDistances(scoreDef);
    vrvScoreDef->ReadEndings(scoreDef);
    vrvScoreDef->ReadOptimization(scoreDef);
    vrvScoreDef->ReadTimeBase(scoreDef);

    this->ReadUnsupportedAttr(scoreDef, vrvScoreDef);
    return this->ReadScoreDefChildren(vrvScoreDef, scoreDef);
}

bool MEIInput::ReadScoreDefChildren(Object *parent, pugi::xml_node parentNode)
{
    assert(dynamic_cast<ScoreDef *>(parent) || dynamic_cast<EditorialElement *>(parent));

    bool success = true;
    pugi::xml_node current;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        if (!success) break;
        this->NormalizeAttributes(current);
        // editorial
        if (this->IsEditorialElementName(current.name())) {
            success = this->ReadEditorialElement(parent, current, EDITORIAL_SCOREDEF);
        }
        // clef, keySig, etc.
        else if (std::string(current.name()) == "clef") {
            success = this->ReadClef(parent, current);
        }
        else if (std::string(current.name()) == "grpSym") {
            success = this->ReadGrpSym(parent, current);
        }
        else if (std::string(current.name()) == "keySig") {
            success = this->ReadKeySig(parent, current);
        }
        else if (std::string(current.name()) == "mensur") {
            success = this->ReadMensur(parent, current);
        }
        else if (std::string(current.name()) == "meterSig") {
            success = this->ReadMeterSig(parent, current);
        }
        else if (std::string(current.name()) == "meterSigGrp") {
            success = this->ReadMeterSigGrp(parent, current);
        }
        // headers and footers
        else if (std::string(current.name()) == "pgFoot") {
            success = this->ReadPgFoot(parent, current);
        }
        else if (std::string(current.name()) == "pgFoot2") {
            success = this->ReadPgFoot2(parent, current);
        }
        else if (std::string(current.name()) == "pgHead") {
            success = this->ReadPgHead(parent, current);
        }
        else if (std::string(current.name()) == "pgHead2") {
            success = this->ReadPgHead2(parent, current);
        }
        // content
        else if (std::string(current.name()) == "staffGrp") {
            success = this->ReadStaffGrp(parent, current);
        }
        // xml comment
        else if (std::string(current.name()) == "") {
            success = this->ReadXMLComment(parent, current);
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
    this->SetMeiID(staffGrp, vrvStaffGrp);

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
    this->ReadUnsupportedAttr(staffGrp, vrvStaffGrp);
    return this->ReadStaffGrpChildren(vrvStaffGrp, staffGrp);
}

bool MEIInput::ReadStaffGrpChildren(Object *parent, pugi::xml_node parentNode)
{
    assert(dynamic_cast<StaffGrp *>(parent) || dynamic_cast<EditorialElement *>(parent));

    bool success = true;
    bool missingStaffDef = true;
    pugi::xml_node current;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        if (!success) break;
        this->NormalizeAttributes(current);
        // editorial
        if (this->IsEditorialElementName(current.name())) {
            success = this->ReadEditorialElement(parent, current, EDITORIAL_STAFFGRP);
        }
        // content
        else if (std::string(current.name()) == "grpSym") {
            success = this->ReadGrpSym(parent, current);
        }
        else if (std::string(current.name()) == "instrDef") {
            success = this->ReadInstrDef(parent, current);
        }
        else if (std::string(current.name()) == "label") {
            success = this->ReadLabel(parent, current);
        }
        else if (std::string(current.name()) == "labelAbbr") {
            success = this->ReadLabelAbbr(parent, current);
        }
        else if (std::string(current.name()) == "staffGrp") {
            success = this->ReadStaffGrp(parent, current);
            missingStaffDef = false; // innermost staffGrp child will report missing staffDef
        }
        else if (std::string(current.name()) == "staffDef") {
            success = this->ReadStaffDef(parent, current);
            missingStaffDef = false;
        }
        // xml comment
        else if (std::string(current.name()) == "") {
            success = this->ReadXMLComment(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <staffGrp>", current.name());
        }
    }

    // Missing staffDefs lead to crashes in the ScoreDefSetCurrent functor
    if (success && missingStaffDef) {
        LogError("Each <staffGrp> must contain at least one <staffDef>.");
        success = false;
    }

    return success;
}

bool MEIInput::ReadRunningElement(pugi::xml_node element, RunningElement *object)
{
    this->SetMeiID(element, object);
    object->ReadHorizontalAlign(element);
    object->ReadTyped(element);

    return true;
}

bool MEIInput::ReadGrpSym(Object *parent, pugi::xml_node grpSym)
{
    GrpSym *vrvGrpSym = new GrpSym();
    this->SetMeiID(grpSym, vrvGrpSym);

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
    this->ReadUnsupportedAttr(grpSym, vrvGrpSym);
    return true;
}

bool MEIInput::ReadPgFoot(Object *parent, pugi::xml_node pgFoot)
{
    PgFoot *vrvPgFoot = new PgFoot();
    this->ReadRunningElement(pgFoot, vrvPgFoot);

    parent->AddChild(vrvPgFoot);
    this->ReadUnsupportedAttr(pgFoot, vrvPgFoot);
    return this->ReadRunningChildren(vrvPgFoot, pgFoot, vrvPgFoot);
}

bool MEIInput::ReadPgFoot2(Object *parent, pugi::xml_node pgFoot2)
{
    PgFoot2 *vrvPgFoot2 = new PgFoot2();
    this->ReadRunningElement(pgFoot2, vrvPgFoot2);

    parent->AddChild(vrvPgFoot2);
    this->ReadUnsupportedAttr(pgFoot2, vrvPgFoot2);
    return this->ReadRunningChildren(vrvPgFoot2, pgFoot2, vrvPgFoot2);
}

bool MEIInput::ReadPgHead(Object *parent, pugi::xml_node pgHead)
{
    PgHead *vrvPgHead = new PgHead();
    this->ReadRunningElement(pgHead, vrvPgHead);

    parent->AddChild(vrvPgHead);
    this->ReadUnsupportedAttr(pgHead, vrvPgHead);
    return this->ReadRunningChildren(vrvPgHead, pgHead, vrvPgHead);
}

bool MEIInput::ReadPgHead2(Object *parent, pugi::xml_node pgHead2)
{
    PgHead2 *vrvPgHead2 = new PgHead2();
    this->ReadRunningElement(pgHead2, vrvPgHead2);

    parent->AddChild(vrvPgHead2);
    this->ReadUnsupportedAttr(pgHead2, vrvPgHead2);
    return this->ReadRunningChildren(vrvPgHead2, pgHead2, vrvPgHead2);
}

bool MEIInput::ReadRunningChildren(Object *parent, pugi::xml_node parentNode, Object *filter)
{
    bool success = true;
    pugi::xml_node xmlElement;
    std::string elementName;
    for (xmlElement = parentNode.first_child(); xmlElement; xmlElement = xmlElement.next_sibling()) {
        if (!success) {
            break;
        }
        this->NormalizeAttributes(xmlElement);
        elementName = std::string(xmlElement.name());
        if (filter && !this->IsAllowed(elementName, filter)) {
            std::string meiElementName = filter->GetClassName();
            std::transform(meiElementName.begin(), meiElementName.begin() + 1, meiElementName.begin(), ::tolower);
            LogWarning("Element <%s> within <%s> is not supported and will be ignored ", xmlElement.name(),
                meiElementName.c_str());
            continue;
        }
        // editorial
        if (this->IsEditorialElementName(xmlElement.name())) {
            success = this->ReadEditorialElement(parent, xmlElement, EDITORIAL_RUNNING, filter);
        }
        // content
        else if (elementName == "fig") {
            success = this->ReadFig(parent, xmlElement);
        }
        else if (elementName == "rend") {
            success = this->ReadRend(parent, xmlElement);
        }
        // xml comment
        else if (elementName == "") {
            success = this->ReadXMLComment(parent, xmlElement);
        }
        // unknown
        else {
            LogWarning("Element <%s> is unknown and will be ignored", xmlElement.name());
        }
    }
    return success;
}

bool MEIInput::ReadStaffDef(Object *parent, pugi::xml_node staffDef)
{
    assert(dynamic_cast<StaffGrp *>(parent) || dynamic_cast<EditorialElement *>(parent));

    StaffDef *vrvStaffDef = new StaffDef();
    this->ReadScoreDefElement(staffDef, vrvStaffDef);

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

    this->ReadScoreDefInterface(staffDef, vrvStaffDef);

    m_doc->m_notationType = vrvStaffDef->GetNotationtype();

    parent->AddChild(vrvStaffDef);
    this->ReadUnsupportedAttr(staffDef, vrvStaffDef);
    return this->ReadStaffDefChildren(vrvStaffDef, staffDef);
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
            success = this->ReadClef(parent, current);
        }
        else if (std::string(current.name()) == "keySig") {
            success = this->ReadKeySig(parent, current);
        }
        else if (std::string(current.name()) == "mensur") {
            success = this->ReadMensur(parent, current);
        }
        else if (std::string(current.name()) == "meterSig") {
            success = this->ReadMeterSig(parent, current);
        }
        else if (std::string(current.name()) == "meterSigGrp") {
            success = this->ReadMeterSigGrp(parent, current);
        }
        // content
        else if (std::string(current.name()) == "instrDef") {
            success = this->ReadInstrDef(parent, current);
        }
        else if (std::string(current.name()) == "label") {
            success = this->ReadLabel(parent, current);
        }
        else if (std::string(current.name()) == "labelAbbr") {
            success = this->ReadLabelAbbr(parent, current);
        }
        else if (std::string(current.name()) == "layerDef") {
            success = this->ReadLayerDef(parent, current);
        }
        else if (std::string(current.name()) == "tuning") {
            success = this->ReadTuning(parent, current);
        }
        // xml comment
        else if (std::string(current.name()) == "") {
            success = this->ReadXMLComment(parent, current);
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
    this->SetMeiID(tuning, vrvTuning);

    parent->AddChild(vrvTuning);
    vrvTuning->ReadCourseLog(tuning);

    this->ReadUnsupportedAttr(tuning, vrvTuning);
    return this->ReadTuningChildren(vrvTuning, tuning);
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
            success = this->ReadCourse(parent, current);
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
    this->SetMeiID(course, vrvCourse);

    parent->AddChild(vrvCourse);
    vrvCourse->ReadAccidental(course);
    vrvCourse->ReadNNumberLike(course);
    vrvCourse->ReadOctave(course);
    vrvCourse->ReadPitch(course);

    this->ReadUnsupportedAttr(course, vrvCourse);

    return true;
}

bool MEIInput::ReadInstrDef(Object *parent, pugi::xml_node instrDef)
{
    InstrDef *vrvInstrDef = new InstrDef();
    this->SetMeiID(instrDef, vrvInstrDef);

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
    this->SetMeiID(label, vrvLabel);

    parent->AddChild(vrvLabel);
    this->ReadUnsupportedAttr(label, vrvLabel);
    return this->ReadTextChildren(vrvLabel, label, vrvLabel);
}

bool MEIInput::ReadLabelAbbr(Object *parent, pugi::xml_node labelAbbr)
{
    LabelAbbr *vrvLabelAbbr = new LabelAbbr();
    this->SetMeiID(labelAbbr, vrvLabelAbbr);

    parent->AddChild(vrvLabelAbbr);
    this->ReadUnsupportedAttr(labelAbbr, vrvLabelAbbr);
    return this->ReadTextChildren(vrvLabelAbbr, labelAbbr, vrvLabelAbbr);
}

bool MEIInput::ReadLayerDef(Object *parent, pugi::xml_node layerDef)
{
    LayerDef *vrvLayerDef = new LayerDef();
    this->SetMeiID(layerDef, vrvLayerDef);

    vrvLayerDef->ReadLabelled(layerDef);
    vrvLayerDef->ReadNInteger(layerDef);
    vrvLayerDef->ReadTyped(layerDef);

    parent->AddChild(vrvLayerDef);
    this->ReadUnsupportedAttr(layerDef, vrvLayerDef);
    return this->ReadLayerDefChildren(vrvLayerDef, layerDef);
}

bool MEIInput::ReadLayerDefChildren(Object *parent, pugi::xml_node parentNode)
{
    assert(dynamic_cast<LayerDef *>(parent));

    bool success = true;
    for (const pugi::xml_node &current : parentNode.children()) {
        const std::string currentName = current.name();
        if (!success)
            break;
        else if (currentName == "instrDef") {
            success = this->ReadInstrDef(parent, current);
        }
        else if (currentName == "label") {
            success = this->ReadLabel(parent, current);
        }
        else if (currentName == "labelAbbr") {
            success = this->ReadLabelAbbr(parent, current);
        }
        // xml comment
        else if (currentName == "") {
            success = this->ReadXMLComment(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <layerDef>", current.name());
        }
    }
    return success;
}

bool MEIInput::ReadMeasure(Object *parent, pugi::xml_node measure)
{
    Measure *vrvMeasure = new Measure();
    if (m_doc->IsMensuralMusicOnly()) {
        LogWarning("Mixing mensural and non mensural music is not supported. Trying to go ahead...");
        m_doc->SetMensuralMusicOnly(false);
    }
    this->SetMeiID(measure, vrvMeasure);

    vrvMeasure->ReadBarring(measure);
    vrvMeasure->ReadMeasureLog(measure);
    vrvMeasure->ReadMeterConformanceBar(measure);
    vrvMeasure->ReadNNumberLike(measure);
    vrvMeasure->ReadPointing(measure);
    vrvMeasure->ReadTyped(measure);

    if ((m_doc->GetType() == Transcription) && (m_version == MEI_2013)) {
        UpgradeMeasureTo_5_0_0(measure);
    }

    if (measure.attribute("coord.x1") && measure.attribute("coord.x2") && (m_doc->GetType() == Transcription)) {
        vrvMeasure->ReadCoordX1(measure);
        vrvMeasure->ReadCoordX2(measure);
        vrvMeasure->m_xAbs = vrvMeasure->GetCoordX1() * DEFINITION_FACTOR;
        vrvMeasure->m_xAbs2 = vrvMeasure->GetCoordX2() * DEFINITION_FACTOR;
    }

    parent->AddChild(vrvMeasure);
    this->ReadUnsupportedAttr(measure, vrvMeasure);
    return this->ReadMeasureChildren(vrvMeasure, measure);
}

bool MEIInput::ReadMeasureChildren(Object *parent, pugi::xml_node parentNode)
{
    assert(dynamic_cast<Measure *>(parent) || dynamic_cast<EditorialElement *>(parent));

    bool success = true;
    pugi::xml_node current;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        const std::string currentName = current.name();
        if (!success) break;
        this->NormalizeAttributes(current);
        // editorial
        if (this->IsEditorialElementName(currentName)) {
            success = this->ReadEditorialElement(parent, current, EDITORIAL_MEASURE);
        }
        // content
        else if (currentName == "anchoredText") {
            success = this->ReadAnchoredText(parent, current);
        }
        else if (currentName == "arpeg") {
            success = this->ReadArpeg(parent, current);
        }
        else if (currentName == "beamSpan") {
            success = this->ReadBeamSpan(parent, current);
        }
        else if (currentName == "bracketSpan") {
            success = this->ReadBracketSpan(parent, current);
        }
        else if (currentName == "breath") {
            success = this->ReadBreath(parent, current);
        }
        else if (currentName == "caesura") {
            success = this->ReadCaesura(parent, current);
        }
        else if (currentName == "dir") {
            success = this->ReadDir(parent, current);
        }
        else if (currentName == "dynam") {
            success = this->ReadDynam(parent, current);
        }
        else if (currentName == "fermata") {
            success = this->ReadFermata(parent, current);
        }
        else if (currentName == "fing") {
            success = this->ReadFing(parent, current);
        }
        else if (currentName == "gliss") {
            success = this->ReadGliss(parent, current);
        }
        else if (currentName == "hairpin") {
            success = this->ReadHairpin(parent, current);
        }
        else if (currentName == "harm") {
            success = this->ReadHarm(parent, current);
        }
        else if (currentName == "lv") {
            success = this->ReadLv(parent, current);
        }
        else if (currentName == "mNum") {
            success = this->ReadMNum(parent, current);
        }
        else if (currentName == "mordent") {
            success = this->ReadMordent(parent, current);
        }
        else if (currentName == "octave") {
            success = this->ReadOctave(parent, current);
        }
        else if (currentName == "pedal") {
            success = this->ReadPedal(parent, current);
        }
        else if (currentName == "phrase") {
            success = this->ReadPhrase(parent, current);
        }
        else if (currentName == "pitchInflection") {
            success = this->ReadPitchInflection(parent, current);
        }
        else if (currentName == "reh") {
            success = this->ReadReh(parent, current);
        }
        else if (currentName == "slur") {
            success = this->ReadSlur(parent, current);
        }
        else if (currentName == "staff") {
            success = this->ReadStaff(parent, current);
        }
        else if (currentName == "tempo") {
            success = this->ReadTempo(parent, current);
        }
        else if (currentName == "tie") {
            success = this->ReadTie(parent, current);
        }
        else if (currentName == "trill") {
            success = this->ReadTrill(parent, current);
        }
        else if (currentName == "turn") {
            success = this->ReadTurn(parent, current);
        }
        else if (currentName == "tupletSpan") {
            if (!ReadTupletSpanAsTuplet(dynamic_cast<Measure *>(parent), current)) {
                LogWarning("<tupletSpan> is not readable as <tuplet> and will be ignored");
            }
        }
        // xml comment
        else if (currentName == "") {
            success = this->ReadXMLComment(parent, current);
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
    this->SetMeiID(meterSigGrp, vrvMeterSigGrp);
    this->ReadLinkingInterface(meterSigGrp, vrvMeterSigGrp);
    vrvMeterSigGrp->ReadBasic(meterSigGrp);
    vrvMeterSigGrp->ReadLabelled(meterSigGrp);
    vrvMeterSigGrp->ReadMeterSigGrpLog(meterSigGrp);
    vrvMeterSigGrp->ReadTyped(meterSigGrp);

    parent->AddChild(vrvMeterSigGrp);
    this->ReadUnsupportedAttr(meterSigGrp, vrvMeterSigGrp);
    return this->ReadMeterSigGrpChildren(vrvMeterSigGrp, meterSigGrp);
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
            success = this->ReadMeterSig(parent, current);
        }
        // xml comment
        else if (std::string(current.name()) == "") {
            success = this->ReadXMLComment(parent, current);
        }
        else {
            LogWarning("Unsupported '<%s>' within <meterSigGrp>", current.name());
        }
    }
    return success;
}

bool MEIInput::ReadControlElement(pugi::xml_node element, ControlElement *object)
{
    this->SetMeiID(element, object);
    this->ReadLinkingInterface(element, object);
    object->ReadLabelled(element);
    object->ReadTyped(element);

    return true;
}

bool MEIInput::ReadAnchoredText(Object *parent, pugi::xml_node anchoredText)
{
    AnchoredText *vrvAnchoredText = new AnchoredText();
    this->ReadControlElement(anchoredText, vrvAnchoredText);

    this->ReadTextDirInterface(anchoredText, vrvAnchoredText);

    this->ReadUnsupportedAttr(anchoredText, vrvAnchoredText);
    parent->AddChild(vrvAnchoredText);
    return this->ReadTextChildren(vrvAnchoredText, anchoredText, vrvAnchoredText);
}

bool MEIInput::ReadArpeg(Object *parent, pugi::xml_node arpeg)
{
    Arpeg *vrvArpeg = new Arpeg();
    this->ReadControlElement(arpeg, vrvArpeg);

    this->ReadPlistInterface(arpeg, vrvArpeg);
    this->ReadTimePointInterface(arpeg, vrvArpeg);
    vrvArpeg->ReadArpegLog(arpeg);
    vrvArpeg->ReadArpegVis(arpeg);
    vrvArpeg->ReadColor(arpeg);
    vrvArpeg->ReadEnclosingChars(arpeg);

    parent->AddChild(vrvArpeg);
    this->ReadUnsupportedAttr(arpeg, vrvArpeg);
    return true;
}

bool MEIInput::ReadBeamSpan(Object *parent, pugi::xml_node beamSpan)
{
    BeamSpan *vrvBeamSpan = new BeamSpan();
    ReadControlElement(beamSpan, vrvBeamSpan);

    ReadPlistInterface(beamSpan, vrvBeamSpan);
    ReadTimeSpanningInterface(beamSpan, vrvBeamSpan);
    vrvBeamSpan->ReadBeamedWith(beamSpan);
    vrvBeamSpan->ReadBeamRend(beamSpan);
    vrvBeamSpan->ReadColor(beamSpan);

    parent->AddChild(vrvBeamSpan);
    ReadUnsupportedAttr(beamSpan, vrvBeamSpan);
    return true;
}

bool MEIInput::ReadBracketSpan(Object *parent, pugi::xml_node bracketSpan)
{
    BracketSpan *vrvBracketSpan = new BracketSpan();
    this->ReadControlElement(bracketSpan, vrvBracketSpan);

    this->ReadTimeSpanningInterface(bracketSpan, vrvBracketSpan);
    vrvBracketSpan->ReadBracketSpanLog(bracketSpan);
    vrvBracketSpan->ReadColor(bracketSpan);
    vrvBracketSpan->ReadLineRend(bracketSpan);
    vrvBracketSpan->ReadLineRendBase(bracketSpan);

    parent->AddChild(vrvBracketSpan);
    this->ReadUnsupportedAttr(bracketSpan, vrvBracketSpan);
    return true;
}

bool MEIInput::ReadBreath(Object *parent, pugi::xml_node breath)
{
    Breath *vrvBreath = new Breath();
    this->ReadControlElement(breath, vrvBreath);

    this->ReadTimePointInterface(breath, vrvBreath);
    vrvBreath->ReadColor(breath);
    vrvBreath->ReadPlacementRelStaff(breath);

    parent->AddChild(vrvBreath);
    this->ReadUnsupportedAttr(breath, vrvBreath);
    return true;
}

bool MEIInput::ReadCaesura(Object *parent, pugi::xml_node caesura)
{
    Caesura *vrvCaesura = new Caesura();
    this->ReadControlElement(caesura, vrvCaesura);

    this->ReadTimePointInterface(caesura, vrvCaesura);
    vrvCaesura->ReadColor(caesura);
    vrvCaesura->ReadPlacementRelStaff(caesura);

    parent->AddChild(vrvCaesura);
    this->ReadUnsupportedAttr(caesura, vrvCaesura);
    return true;
}

bool MEIInput::ReadDir(Object *parent, pugi::xml_node dir)
{
    Dir *vrvDir = new Dir();
    this->ReadControlElement(dir, vrvDir);

    this->ReadTextDirInterface(dir, vrvDir);
    this->ReadTimeSpanningInterface(dir, vrvDir);
    vrvDir->ReadLang(dir);
    vrvDir->ReadLineRendBase(dir);
    vrvDir->ReadExtender(dir);
    vrvDir->ReadVerticalGroup(dir);

    parent->AddChild(vrvDir);
    this->ReadUnsupportedAttr(dir, vrvDir);
    return this->ReadTextChildren(vrvDir, dir, vrvDir);
}

bool MEIInput::ReadDynam(Object *parent, pugi::xml_node dynam)
{
    Dynam *vrvDynam = new Dynam();
    this->ReadControlElement(dynam, vrvDynam);

    this->ReadTextDirInterface(dynam, vrvDynam);
    this->ReadTimeSpanningInterface(dynam, vrvDynam);
    vrvDynam->ReadEnclosingChars(dynam);
    vrvDynam->ReadExtender(dynam);
    vrvDynam->ReadLineRendBase(dynam);
    vrvDynam->ReadMidiValue(dynam);
    vrvDynam->ReadMidiValue2(dynam);
    vrvDynam->ReadVerticalGroup(dynam);

    parent->AddChild(vrvDynam);
    this->ReadUnsupportedAttr(dynam, vrvDynam);
    return this->ReadTextChildren(vrvDynam, dynam, vrvDynam);
}

bool MEIInput::ReadFermata(Object *parent, pugi::xml_node fermata)
{
    Fermata *vrvFermata = new Fermata();
    this->ReadControlElement(fermata, vrvFermata);

    this->ReadTimePointInterface(fermata, vrvFermata);
    vrvFermata->ReadColor(fermata);
    vrvFermata->ReadEnclosingChars(fermata);
    vrvFermata->ReadExtSym(fermata);
    vrvFermata->ReadFermataVis(fermata);
    vrvFermata->ReadPlacementRelStaff(fermata);

    parent->AddChild(vrvFermata);
    this->ReadUnsupportedAttr(fermata, vrvFermata);
    return true;
}

bool MEIInput::ReadFing(Object *parent, pugi::xml_node fing)
{
    Fing *vrvFing = new Fing();
    this->ReadControlElement(fing, vrvFing);

    this->ReadTextDirInterface(fing, vrvFing);
    this->ReadTimePointInterface(fing, vrvFing);
    vrvFing->ReadNNumberLike(fing);

    parent->AddChild(vrvFing);
    this->ReadUnsupportedAttr(fing, vrvFing);
    return this->ReadTextChildren(vrvFing, fing, vrvFing);
}

bool MEIInput::ReadGliss(Object *parent, pugi::xml_node gliss)
{
    Gliss *vrvGliss = new Gliss();
    this->ReadControlElement(gliss, vrvGliss);

    this->ReadTimeSpanningInterface(gliss, vrvGliss);
    vrvGliss->ReadColor(gliss);
    vrvGliss->ReadLineRend(gliss);
    vrvGliss->ReadLineRendBase(gliss);
    vrvGliss->ReadNNumberLike(gliss);

    parent->AddChild(vrvGliss);
    this->ReadUnsupportedAttr(gliss, vrvGliss);
    return true;
}

bool MEIInput::ReadHairpin(Object *parent, pugi::xml_node hairpin)
{
    Hairpin *vrvHairpin = new Hairpin();
    this->ReadControlElement(hairpin, vrvHairpin);

    this->ReadTimeSpanningInterface(hairpin, vrvHairpin);
    vrvHairpin->ReadColor(hairpin);
    vrvHairpin->ReadHairpinLog(hairpin);
    vrvHairpin->ReadHairpinVis(hairpin);
    vrvHairpin->ReadPlacementRelStaff(hairpin);
    vrvHairpin->ReadVerticalGroup(hairpin);

    parent->AddChild(vrvHairpin);
    this->ReadUnsupportedAttr(hairpin, vrvHairpin);
    return true;
}

bool MEIInput::ReadHarm(Object *parent, pugi::xml_node harm)
{
    Harm *vrvHarm = new Harm();
    this->ReadControlElement(harm, vrvHarm);

    this->ReadTextDirInterface(harm, vrvHarm);
    this->ReadTimeSpanningInterface(harm, vrvHarm);
    vrvHarm->ReadLang(harm);
    vrvHarm->ReadNNumberLike(harm);

    parent->AddChild(vrvHarm);
    this->ReadUnsupportedAttr(harm, vrvHarm);
    return this->ReadTextChildren(vrvHarm, harm, vrvHarm);
}

bool MEIInput::ReadLv(Object *parent, pugi::xml_node lv)
{
    Lv *vrvLv = new Lv();
    this->ReadControlElement(lv, vrvLv);

    this->ReadTimeSpanningInterface(lv, vrvLv);
    vrvLv->ReadColor(lv);
    vrvLv->ReadCurvature(lv);
    vrvLv->ReadCurveRend(lv);

    parent->AddChild(vrvLv);
    this->ReadUnsupportedAttr(lv, vrvLv);
    return true;
}

bool MEIInput::ReadMNum(Object *parent, pugi::xml_node mNum)
{
    MNum *vrvMNum = new MNum();
    this->ReadControlElement(mNum, vrvMNum);

    this->ReadTextDirInterface(mNum, vrvMNum);
    this->ReadTimePointInterface(mNum, vrvMNum);
    vrvMNum->ReadColor(mNum);
    vrvMNum->ReadLang(mNum);
    vrvMNum->ReadTypography(mNum);

    parent->AddChild(vrvMNum);
    return this->ReadTextChildren(vrvMNum, mNum, vrvMNum);
}

bool MEIInput::ReadMordent(Object *parent, pugi::xml_node mordent)
{
    Mordent *vrvMordent = new Mordent();
    this->ReadControlElement(mordent, vrvMordent);

    if (m_version < MEI_4_0_0) {
        UpgradeMordentTo_4_0_0(mordent, vrvMordent);
    }

    this->ReadTimePointInterface(mordent, vrvMordent);
    vrvMordent->ReadColor(mordent);
    vrvMordent->ReadExtSym(mordent);
    vrvMordent->ReadOrnamentAccid(mordent);
    vrvMordent->ReadPlacementRelStaff(mordent);
    vrvMordent->ReadMordentLog(mordent);

    parent->AddChild(vrvMordent);
    this->ReadUnsupportedAttr(mordent, vrvMordent);
    return true;
}

bool MEIInput::ReadOctave(Object *parent, pugi::xml_node octave)
{
    Octave *vrvOctave = new Octave();
    this->ReadControlElement(octave, vrvOctave);

    this->ReadTimeSpanningInterface(octave, vrvOctave);
    vrvOctave->ReadColor(octave);
    vrvOctave->ReadExtender(octave);
    vrvOctave->ReadLineRend(octave);
    vrvOctave->ReadLineRendBase(octave);
    vrvOctave->ReadNNumberLike(octave);
    vrvOctave->ReadOctaveDisplacement(octave);

    parent->AddChild(vrvOctave);
    this->ReadUnsupportedAttr(octave, vrvOctave);
    return true;
}

bool MEIInput::ReadPedal(Object *parent, pugi::xml_node pedal)
{
    Pedal *vrvPedal = new Pedal();
    this->ReadControlElement(pedal, vrvPedal);

    this->ReadTimeSpanningInterface(pedal, vrvPedal);
    vrvPedal->ReadColor(pedal);
    vrvPedal->ReadExtSym(pedal);
    vrvPedal->ReadPedalLog(pedal);
    vrvPedal->ReadPedalVis(pedal);
    vrvPedal->ReadPlacementRelStaff(pedal);
    vrvPedal->ReadVerticalGroup(pedal);

    parent->AddChild(vrvPedal);
    this->ReadUnsupportedAttr(pedal, vrvPedal);
    return true;
}

bool MEIInput::ReadPhrase(Object *parent, pugi::xml_node phrase)
{
    Phrase *vrvPhrase = new Phrase();
    this->ReadControlElement(phrase, vrvPhrase);

    this->ReadTimeSpanningInterface(phrase, vrvPhrase);
    vrvPhrase->ReadColor(phrase);
    vrvPhrase->ReadCurvature(phrase);
    vrvPhrase->ReadCurveRend(phrase);
    vrvPhrase->ReadLayerIdent(phrase);

    parent->AddChild(vrvPhrase);
    this->ReadUnsupportedAttr(phrase, vrvPhrase);
    return true;
}

bool MEIInput::ReadPitchInflection(Object *parent, pugi::xml_node pitchInflection)
{
    PitchInflection *vrvPitchInflection = new PitchInflection();
    this->ReadControlElement(pitchInflection, vrvPitchInflection);

    this->ReadTimeSpanningInterface(pitchInflection, vrvPitchInflection);

    parent->AddChild(vrvPitchInflection);
    this->ReadUnsupportedAttr(pitchInflection, vrvPitchInflection);
    return true;
}

bool MEIInput::ReadReh(Object *parent, pugi::xml_node reh)
{
    Reh *vrvReh = new Reh();
    this->ReadControlElement(reh, vrvReh);

    this->ReadTextDirInterface(reh, vrvReh);
    this->ReadTimePointInterface(reh, vrvReh);
    vrvReh->ReadColor(reh);
    vrvReh->ReadLang(reh);
    vrvReh->ReadVerticalGroup(reh);

    parent->AddChild(vrvReh);
    this->ReadUnsupportedAttr(reh, vrvReh);
    return this->ReadTextChildren(vrvReh, reh, vrvReh);
}

bool MEIInput::ReadSlur(Object *parent, pugi::xml_node slur)
{
    Slur *vrvSlur = new Slur();
    this->ReadControlElement(slur, vrvSlur);

    this->ReadTimeSpanningInterface(slur, vrvSlur);
    vrvSlur->ReadColor(slur);
    vrvSlur->ReadCurvature(slur);
    vrvSlur->ReadCurveRend(slur);
    vrvSlur->ReadLayerIdent(slur);

    parent->AddChild(vrvSlur);
    this->ReadUnsupportedAttr(slur, vrvSlur);
    return true;
}

bool MEIInput::ReadTempo(Object *parent, pugi::xml_node tempo)
{
    Tempo *vrvTempo = new Tempo();
    this->ReadControlElement(tempo, vrvTempo);

    this->ReadTextDirInterface(tempo, vrvTempo);
    this->ReadTimePointInterface(tempo, vrvTempo);
    vrvTempo->ReadLang(tempo);
    vrvTempo->ReadMidiTempo(tempo);
    vrvTempo->ReadMmTempo(tempo);

    parent->AddChild(vrvTempo);
    this->ReadUnsupportedAttr(tempo, vrvTempo);
    return this->ReadTextChildren(vrvTempo, tempo, vrvTempo);
}

bool MEIInput::ReadTie(Object *parent, pugi::xml_node tie)
{
    Tie *vrvTie = new Tie();
    this->ReadControlElement(tie, vrvTie);

    this->ReadTimeSpanningInterface(tie, vrvTie);
    vrvTie->ReadColor(tie);
    vrvTie->ReadCurvature(tie);
    vrvTie->ReadCurveRend(tie);

    parent->AddChild(vrvTie);
    this->ReadUnsupportedAttr(tie, vrvTie);
    return true;
}

bool MEIInput::ReadTrill(Object *parent, pugi::xml_node trill)
{
    Trill *vrvTrill = new Trill();
    this->ReadControlElement(trill, vrvTrill);

    this->ReadTimeSpanningInterface(trill, vrvTrill);
    vrvTrill->ReadColor(trill);
    vrvTrill->ReadExtender(trill);
    vrvTrill->ReadExtSym(trill);
    vrvTrill->ReadLineRend(trill);
    vrvTrill->ReadNNumberLike(trill);
    vrvTrill->ReadOrnamentAccid(trill);
    vrvTrill->ReadPlacementRelStaff(trill);

    parent->AddChild(vrvTrill);
    this->ReadUnsupportedAttr(trill, vrvTrill);
    return true;
}

bool MEIInput::ReadTurn(Object *parent, pugi::xml_node turn)
{
    Turn *vrvTurn = new Turn();
    this->ReadControlElement(turn, vrvTurn);

    if (m_version < MEI_4_0_0) {
        UpgradeTurnTo_4_0_0(turn, vrvTurn);
    }

    this->ReadTimePointInterface(turn, vrvTurn);
    vrvTurn->ReadColor(turn);
    vrvTurn->ReadExtSym(turn);
    vrvTurn->ReadOrnamentAccid(turn);
    vrvTurn->ReadPlacementRelStaff(turn);
    vrvTurn->ReadTurnLog(turn);

    parent->AddChild(vrvTurn);
    this->ReadUnsupportedAttr(turn, vrvTurn);
    return true;
}

bool MEIInput::ReadFb(Object *parent, pugi::xml_node fb)
{
    Fb *vrvFb = new Fb();
    this->SetMeiID(fb, vrvFb);

    parent->AddChild(vrvFb);
    this->ReadUnsupportedAttr(fb, vrvFb);
    return this->ReadFbChildren(vrvFb, fb);
}

bool MEIInput::ReadFbChildren(Object *parent, pugi::xml_node parentNode)
{
    assert(dynamic_cast<Fb *>(parent) || dynamic_cast<EditorialElement *>(parent));

    bool success = true;
    pugi::xml_node current;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        if (!success) break;
        this->NormalizeAttributes(current);
        // editorial
        if (this->IsEditorialElementName(current.name())) {
            success = this->ReadEditorialElement(parent, current, EDITORIAL_FB);
        }
        // content
        else if (std::string(current.name()) == "f") {
            success = this->ReadF(parent, current);
        }
        // xml comment
        else if (std::string(current.name()) == "") {
            success = this->ReadXMLComment(parent, current);
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
    this->SetMeiID(staff, vrvStaff);

    vrvStaff->ReadFacsimile(staff);
    vrvStaff->ReadNInteger(staff);
    vrvStaff->ReadTyped(staff);
    vrvStaff->ReadVisibility(staff);

    if ((m_doc->GetType() == Transcription) && (m_version == MEI_2013)) {
        UpgradeStaffTo_5_0_0(staff);
    }

    if (staff.attribute("coord.y1") && (m_doc->GetType() == Transcription)) {
        vrvStaff->ReadCoordY1(staff);
        vrvStaff->m_yAbs = vrvStaff->GetCoordY1() * DEFINITION_FACTOR;
    }

    if (!vrvStaff->HasN() || (vrvStaff->GetN() == 0)) {
        LogWarning("No @n on <staff> or a value of 0 might yield unpredictable results");
    }

    parent->AddChild(vrvStaff);
    this->ReadUnsupportedAttr(staff, vrvStaff);
    return this->ReadStaffChildren(vrvStaff, staff);
}

bool MEIInput::ReadStaffChildren(Object *parent, pugi::xml_node parentNode)
{
    assert(dynamic_cast<Staff *>(parent) || dynamic_cast<EditorialElement *>(parent));

    bool success = true;
    pugi::xml_node current;
    for (current = parentNode.first_child(); current; current = current.next_sibling()) {
        if (!success) break;
        this->NormalizeAttributes(current);
        // editorial
        if (this->IsEditorialElementName(current.name())) {
            success = this->ReadEditorialElement(parent, current, EDITORIAL_STAFF);
        }
        // content
        else if (std::string(current.name()) == "layer") {
            success = this->ReadLayer(parent, current);
        }
        // xml comment
        else if (std::string(current.name()) == "") {
            success = this->ReadXMLComment(parent, current);
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
    this->SetMeiID(layer, vrvLayer);

    vrvLayer->ReadCue(layer);
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
    this->ReadUnsupportedAttr(layer, vrvLayer);
    return this->ReadLayerChildren(vrvLayer, layer);
}

bool MEIInput::ReadLayerChildren(Object *parent, pugi::xml_node parentNode, Object *filter)
{
    bool success = true;
    pugi::xml_node xmlElement;
    std::string elementName;
    for (xmlElement = parentNode.first_child(); xmlElement; xmlElement = xmlElement.next_sibling()) {
        if (!success) break;
        this->NormalizeAttributes(xmlElement);

        elementName = std::string(xmlElement.name());
        // LogDebug("ReadLayerChildren: element <%s>", xmlElement.name());
        if (!this->IsAllowed(elementName, filter)) {
            std::string meiElementName = filter->GetClassName();
            std::transform(meiElementName.begin(), meiElementName.begin() + 1, meiElementName.begin(), ::tolower);
            LogWarning("Element <%s> within <%s> is not supported and will be ignored ", xmlElement.name(),
                meiElementName.c_str());
            continue;
        }
        // editorial
        else if (this->IsEditorialElementName(xmlElement.name())) {
            success = this->ReadEditorialElement(parent, xmlElement, EDITORIAL_LAYER, filter);
        }
        // content
        else if (elementName == "accid") {
            success = this->ReadAccid(parent, xmlElement);
        }
        else if (elementName == "artic") {
            success = this->ReadArtic(parent, xmlElement);
        }
        else if (elementName == "barLine") {
            success = this->ReadBarLine(parent, xmlElement);
        }
        else if (elementName == "beam") {
            success = this->ReadBeam(parent, xmlElement);
        }
        else if (elementName == "beatRpt") {
            success = this->ReadBeatRpt(parent, xmlElement);
        }
        else if (elementName == "bTrem") {
            success = this->ReadBTrem(parent, xmlElement);
        }
        else if (elementName == "chord") {
            success = this->ReadChord(parent, xmlElement);
        }
        else if (elementName == "clef") {
            success = this->ReadClef(parent, xmlElement);
        }
        else if (elementName == "custos") {
            success = this->ReadCustos(parent, xmlElement);
        }
        else if (elementName == "dot") {
            success = this->ReadDot(parent, xmlElement);
        }
        else if (elementName == "fTrem") {
            success = this->ReadFTrem(parent, xmlElement);
        }
        else if (elementName == "graceGrp") {
            success = this->ReadGraceGrp(parent, xmlElement);
        }
        else if (elementName == "halfmRpt") {
            success = this->ReadHalfmRpt(parent, xmlElement);
        }
        else if (elementName == "keyAccid") {
            success = this->ReadKeyAccid(parent, xmlElement);
        }
        else if (elementName == "keySig") {
            success = this->ReadKeySig(parent, xmlElement);
        }
        else if (elementName == "label") {
            success = this->ReadLabel(parent, xmlElement);
        }
        else if (elementName == "labelAbbr") {
            success = this->ReadLabelAbbr(parent, xmlElement);
        }
        else if (elementName == "ligature") {
            success = this->ReadLigature(parent, xmlElement);
        }
        else if (elementName == "mensur") {
            success = this->ReadMensur(parent, xmlElement);
        }
        else if (elementName == "meterSig") {
            success = this->ReadMeterSig(parent, xmlElement);
        }
        else if (elementName == "meterSigGrp") {
            success = this->ReadMeterSigGrp(parent, xmlElement);
        }
        else if (elementName == "nc") {
            success = this->ReadNc(parent, xmlElement);
        }
        else if (elementName == "neume") {
            success = this->ReadNeume(parent, xmlElement);
        }
        else if (elementName == "note") {
            success = this->ReadNote(parent, xmlElement);
        }
        else if (elementName == "rest") {
            success = this->ReadRest(parent, xmlElement);
        }
        else if (elementName == "mRest") {
            success = this->ReadMRest(parent, xmlElement);
        }
        else if (elementName == "mRpt") {
            success = this->ReadMRpt(parent, xmlElement);
        }
        else if (elementName == "mRpt2") {
            success = this->ReadMRpt2(parent, xmlElement);
        }
        else if (elementName == "mSpace") {
            success = this->ReadMSpace(parent, xmlElement);
        }
        else if (elementName == "multiRest") {
            success = this->ReadMultiRest(parent, xmlElement);
        }
        else if (elementName == "multiRpt") {
            success = this->ReadMultiRpt(parent, xmlElement);
        }
        else if (elementName == "plica") {
            success = this->ReadPlica(parent, xmlElement);
        }
        else if (elementName == "proport") {
            success = this->ReadProport(parent, xmlElement);
        }
        else if (elementName == "space") {
            success = this->ReadSpace(parent, xmlElement);
        }
        else if (elementName == "syl") {
            success = this->ReadSyl(parent, xmlElement);
        }
        else if (elementName == "syllable") {
            success = this->ReadSyllable(parent, xmlElement);
        }
        else if (elementName == "tabDurSym") {
            success = this->ReadTabDurSym(parent, xmlElement);
        }
        else if (elementName == "tabGrp") {
            success = this->ReadTabGrp(parent, xmlElement);
        }
        else if (elementName == "tuplet") {
            success = this->ReadTuplet(parent, xmlElement);
        }
        else if (elementName == "verse") {
            success = this->ReadVerse(parent, xmlElement);
        }
        // xml comment
        else if (elementName == "") {
            success = this->ReadXMLComment(parent, xmlElement);
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
    this->SetMeiID(element, object);
    this->ReadLinkingInterface(element, object);
    object->ReadLabelled(element);
    object->ReadTyped(element);

    if ((m_doc->GetType() == Transcription) && (m_version == MEI_2013)) {
        UpgradeLayerElementTo_5_0_0(element);
    }

    if (element.attribute("coord.x1") && (m_doc->GetType() == Transcription)) {
        object->ReadCoordX1(element);
        object->m_xAbs = object->GetCoordX1() * DEFINITION_FACTOR;
    }

    return true;
}

bool MEIInput::ReadAccid(Object *parent, pugi::xml_node accid)
{
    Accid *vrvAccid = new Accid();
    this->ReadLayerElement(accid, vrvAccid);

    this->ReadPositionInterface(accid, vrvAccid);
    vrvAccid->ReadAccidental(accid);
    vrvAccid->ReadAccidentalGestural(accid);
    vrvAccid->ReadAccidLog(accid);
    vrvAccid->ReadColor(accid);
    vrvAccid->ReadEnclosingChars(accid);
    vrvAccid->ReadExtSym(accid);
    vrvAccid->ReadPlacementOnStaff(accid);
    vrvAccid->ReadPlacementRelEvent(accid);

    parent->AddChild(vrvAccid);
    this->ReadUnsupportedAttr(accid, vrvAccid);
    return true;
}

bool MEIInput::ReadArtic(Object *parent, pugi::xml_node artic)
{
    Artic *vrvArtic = new Artic();
    this->ReadLayerElement(artic, vrvArtic);

    vrvArtic->ReadArticulation(artic);
    vrvArtic->ReadArticulationGestural(artic);
    vrvArtic->ReadColor(artic);
    vrvArtic->ReadEnclosingChars(artic);
    vrvArtic->ReadExtSym(artic);
    vrvArtic->ReadPlacementRelEvent(artic);

    if (vrvArtic->GetArtic().size() > 1) {
        m_doc->SetMarkup(MARKUP_ARTIC_MULTIVAL);
    }

    parent->AddChild(vrvArtic);
    this->ReadUnsupportedAttr(artic, vrvArtic);
    return true;
}

bool MEIInput::ReadBarLine(Object *parent, pugi::xml_node barLine)
{
    BarLine *vrvBarLine = new BarLine();
    this->ReadLayerElement(barLine, vrvBarLine);

    vrvBarLine->ReadBarLineLog(barLine);
    vrvBarLine->ReadColor(barLine);
    vrvBarLine->ReadNNumberLike(barLine);
    vrvBarLine->ReadVisibility(barLine);

    parent->AddChild(vrvBarLine);
    this->ReadUnsupportedAttr(barLine, vrvBarLine);
    return true;
}

bool MEIInput::ReadBeam(Object *parent, pugi::xml_node beam)
{
    Beam *vrvBeam = new Beam();
    this->ReadLayerElement(beam, vrvBeam);

    vrvBeam->ReadBeamedWith(beam);
    vrvBeam->ReadBeamRend(beam);
    vrvBeam->ReadColor(beam);
    vrvBeam->ReadCue(beam);

    parent->AddChild(vrvBeam);
    this->ReadUnsupportedAttr(beam, vrvBeam);
    return this->ReadLayerChildren(vrvBeam, beam, vrvBeam);
}

bool MEIInput::ReadBeatRpt(Object *parent, pugi::xml_node beatRpt)
{
    BeatRpt *vrvBeatRpt = new BeatRpt();
    this->ReadLayerElement(beatRpt, vrvBeatRpt);

    vrvBeatRpt->ReadColor(beatRpt);
    vrvBeatRpt->ReadBeatRptLog(beatRpt);
    vrvBeatRpt->ReadBeatRptVis(beatRpt);

    if (m_version < MEI_4_0_0) {
        UpgradeBeatRptTo_4_0_0(beatRpt, vrvBeatRpt);
    }

    parent->AddChild(vrvBeatRpt);
    this->ReadUnsupportedAttr(beatRpt, vrvBeatRpt);
    return true;
}

bool MEIInput::ReadBTrem(Object *parent, pugi::xml_node bTrem)
{
    BTrem *vrvBTrem = new BTrem();
    this->ReadLayerElement(bTrem, vrvBTrem);

    vrvBTrem->ReadBTremLog(bTrem);
    vrvBTrem->ReadNumbered(bTrem);
    vrvBTrem->ReadNumberPlacement(bTrem);
    vrvBTrem->ReadTremMeasured(bTrem);

    parent->AddChild(vrvBTrem);
    this->ReadUnsupportedAttr(bTrem, vrvBTrem);
    return this->ReadLayerChildren(vrvBTrem, bTrem, vrvBTrem);
}

bool MEIInput::ReadChord(Object *parent, pugi::xml_node chord)
{
    Chord *vrvChord = new Chord();
    this->ReadLayerElement(chord, vrvChord);

    if (m_version < MEI_4_0_0) {
        if (chord.attribute("size")) {
            chord.remove_attribute("size");
            chord.append_attribute("cue").set_value("true");
        }
    }

    this->ReadDurationInterface(chord, vrvChord);
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
    this->ReadUnsupportedAttr(chord, vrvChord);
    return this->ReadLayerChildren(vrvChord, chord, vrvChord);
}

bool MEIInput::ReadClef(Object *parent, pugi::xml_node clef)
{
    Clef *vrvClef = new Clef();
    this->ReadLayerElement(clef, vrvClef);
    this->ReadFacsimileInterface(clef, vrvClef);

    vrvClef->ReadClefShape(clef);
    vrvClef->ReadColor(clef);
    vrvClef->ReadEnclosingChars(clef);
    vrvClef->ReadExtSym(clef);
    vrvClef->ReadLineLoc(clef);
    vrvClef->ReadOctaveDisplacement(clef);
    vrvClef->ReadStaffIdent(clef);
    vrvClef->ReadVisibility(clef);

    parent->AddChild(vrvClef);
    this->ReadUnsupportedAttr(clef, vrvClef);
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
    this->ReadLayerElement(custos, vrvCustos);

    this->ReadFacsimileInterface(custos, vrvCustos);
    this->ReadPitchInterface(custos, vrvCustos);
    this->ReadPositionInterface(custos, vrvCustos);
    vrvCustos->ReadColor(custos);
    vrvCustos->ReadExtSym(custos);

    this->ReadAccidAttr(custos, vrvCustos);

    parent->AddChild(vrvCustos);
    this->ReadUnsupportedAttr(custos, vrvCustos);
    return this->ReadLayerChildren(vrvCustos, custos, vrvCustos);
}

bool MEIInput::ReadDot(Object *parent, pugi::xml_node dot)
{
    Dot *vrvDot = new Dot();
    this->ReadLayerElement(dot, vrvDot);

    this->ReadPositionInterface(dot, vrvDot);
    vrvDot->ReadColor(dot);
    vrvDot->ReadDotLog(dot);

    parent->AddChild(vrvDot);
    this->ReadUnsupportedAttr(dot, vrvDot);
    return true;
}

bool MEIInput::ReadFTrem(Object *parent, pugi::xml_node fTrem)
{
    FTrem *vrvFTrem = new FTrem();
    this->ReadLayerElement(fTrem, vrvFTrem);

    if (m_version < MEI_4_0_0) {
        UpgradeFTremTo_4_0_0(fTrem, vrvFTrem);
    }

    vrvFTrem->ReadFTremVis(fTrem);
    vrvFTrem->ReadTremMeasured(fTrem);

    parent->AddChild(vrvFTrem);
    this->ReadUnsupportedAttr(fTrem, vrvFTrem);
    return this->ReadLayerChildren(vrvFTrem, fTrem, vrvFTrem);
}

bool MEIInput::ReadGraceGrp(Object *parent, pugi::xml_node graceGrp)
{
    GraceGrp *vrvGraceGrp = new GraceGrp();
    this->ReadLayerElement(graceGrp, vrvGraceGrp);

    vrvGraceGrp->ReadColor(graceGrp);
    vrvGraceGrp->ReadGraced(graceGrp);
    vrvGraceGrp->ReadGraceGrpLog(graceGrp);

    parent->AddChild(vrvGraceGrp);
    this->ReadUnsupportedAttr(graceGrp, vrvGraceGrp);
    return this->ReadLayerChildren(vrvGraceGrp, graceGrp, vrvGraceGrp);
}

bool MEIInput::ReadHalfmRpt(Object *parent, pugi::xml_node halfmRpt)
{
    HalfmRpt *vrvHalfmRpt = new HalfmRpt();
    this->ReadLayerElement(halfmRpt, vrvHalfmRpt);

    vrvHalfmRpt->ReadColor(halfmRpt);

    parent->AddChild(vrvHalfmRpt);
    this->ReadUnsupportedAttr(halfmRpt, vrvHalfmRpt);
    return true;
}

bool MEIInput::ReadKeyAccid(Object *parent, pugi::xml_node keyAccid)
{
    KeyAccid *vrvKeyAccid = new KeyAccid();
    this->ReadLayerElement(keyAccid, vrvKeyAccid);

    this->ReadPitchInterface(keyAccid, vrvKeyAccid);
    this->ReadPositionInterface(keyAccid, vrvKeyAccid);
    vrvKeyAccid->ReadAccidental(keyAccid);
    vrvKeyAccid->ReadColor(keyAccid);
    vrvKeyAccid->ReadEnclosingChars(keyAccid);
    vrvKeyAccid->ReadExtSym(keyAccid);

    parent->AddChild(vrvKeyAccid);
    this->ReadUnsupportedAttr(keyAccid, vrvKeyAccid);
    return true;
}

bool MEIInput::ReadKeySig(Object *parent, pugi::xml_node keySig)
{
    KeySig *vrvKeySig = new KeySig();
    this->ReadLayerElement(keySig, vrvKeySig);

    vrvKeySig->ReadAccidental(keySig);
    vrvKeySig->ReadPitch(keySig);
    vrvKeySig->ReadKeySigAnl(keySig);
    vrvKeySig->ReadKeySigLog(keySig);
    vrvKeySig->ReadKeySigVis(keySig);
    vrvKeySig->ReadVisibility(keySig);

    parent->AddChild(vrvKeySig);
    this->ReadUnsupportedAttr(keySig, vrvKeySig);
    return this->ReadLayerChildren(vrvKeySig, keySig, vrvKeySig);
}

bool MEIInput::ReadLigature(Object *parent, pugi::xml_node ligature)
{
    Ligature *vrvLigature = new Ligature();
    this->SetMeiID(ligature, vrvLigature);

    vrvLigature->ReadLigatureVis(ligature);

    parent->AddChild(vrvLigature);
    this->ReadUnsupportedAttr(ligature, vrvLigature);
    return this->ReadLayerChildren(vrvLigature, ligature, vrvLigature);
}

bool MEIInput::ReadMensur(Object *parent, pugi::xml_node mensur)
{
    Mensur *vrvMensur = new Mensur();
    this->ReadLayerElement(mensur, vrvMensur);

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
    this->ReadUnsupportedAttr(mensur, vrvMensur);
    return true;
}

bool MEIInput::ReadMeterSig(Object *parent, pugi::xml_node meterSig)
{
    MeterSig *vrvMeterSig = new MeterSig();
    this->ReadLayerElement(meterSig, vrvMeterSig);

    vrvMeterSig->ReadEnclosingChars(meterSig);
    vrvMeterSig->ReadMeterSigLog(meterSig);
    vrvMeterSig->ReadMeterSigVis(meterSig);

    parent->AddChild(vrvMeterSig);
    this->ReadUnsupportedAttr(meterSig, vrvMeterSig);
    return true;
}

bool MEIInput::ReadMRest(Object *parent, pugi::xml_node mRest)
{
    MRest *vrvMRest = new MRest();
    this->ReadLayerElement(mRest, vrvMRest);
    this->ReadPositionInterface(mRest, vrvMRest);

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
    this->ReadUnsupportedAttr(mRest, vrvMRest);
    return true;
}

bool MEIInput::ReadMRpt(Object *parent, pugi::xml_node mRpt)
{
    MRpt *vrvMRpt = new MRpt();
    this->ReadLayerElement(mRpt, vrvMRpt);

    vrvMRpt->ReadColor(mRpt);
    vrvMRpt->ReadNumbered(mRpt);
    vrvMRpt->ReadNumberPlacement(mRpt);

    parent->AddChild(vrvMRpt);
    this->ReadUnsupportedAttr(mRpt, vrvMRpt);
    return true;
}

bool MEIInput::ReadMRpt2(Object *parent, pugi::xml_node mRpt2)
{
    MRpt2 *vrvMRpt2 = new MRpt2();
    this->ReadLayerElement(mRpt2, vrvMRpt2);

    vrvMRpt2->ReadColor(mRpt2);

    parent->AddChild(vrvMRpt2);
    this->ReadUnsupportedAttr(mRpt2, vrvMRpt2);
    return true;
}

bool MEIInput::ReadMSpace(Object *parent, pugi::xml_node mSpace)
{
    MSpace *vrvMSpace = new MSpace();
    this->ReadLayerElement(mSpace, vrvMSpace);

    parent->AddChild(vrvMSpace);
    this->ReadUnsupportedAttr(mSpace, vrvMSpace);
    return true;
}

bool MEIInput::ReadMultiRest(Object *parent, pugi::xml_node multiRest)
{
    MultiRest *vrvMultiRest = new MultiRest();
    this->ReadLayerElement(multiRest, vrvMultiRest);

    this->ReadPositionInterface(multiRest, vrvMultiRest);
    vrvMultiRest->ReadColor(multiRest);
    vrvMultiRest->ReadMultiRestVis(multiRest);
    vrvMultiRest->ReadNumbered(multiRest);
    vrvMultiRest->ReadNumberPlacement(multiRest);
    vrvMultiRest->ReadWidth(multiRest);

    parent->AddChild(vrvMultiRest);
    this->ReadUnsupportedAttr(multiRest, vrvMultiRest);
    return true;
}

bool MEIInput::ReadMultiRpt(Object *parent, pugi::xml_node multiRpt)
{
    MultiRpt *vrvMultiRpt = new MultiRpt();
    this->ReadLayerElement(multiRpt, vrvMultiRpt);

    vrvMultiRpt->ReadNumbered(multiRpt);

    parent->AddChild(vrvMultiRpt);
    this->ReadUnsupportedAttr(multiRpt, vrvMultiRpt);
    return true;
}

bool MEIInput::ReadNc(Object *parent, pugi::xml_node nc)
{
    Nc *vrvNc = new Nc();
    this->ReadLayerElement(nc, vrvNc);

    this->ReadDurationInterface(nc, vrvNc);
    this->ReadFacsimileInterface(nc, vrvNc);
    this->ReadPitchInterface(nc, vrvNc);
    this->ReadPositionInterface(nc, vrvNc);
    vrvNc->ReadColor(nc);
    vrvNc->ReadIntervalMelodic(nc);
    vrvNc->ReadNcForm(nc);

    parent->AddChild(vrvNc);
    return this->ReadLayerChildren(vrvNc, nc, vrvNc);
}

bool MEIInput::ReadNeume(Object *parent, pugi::xml_node neume)
{
    Neume *vrvNeume = new Neume();
    this->ReadLayerElement(neume, vrvNeume);
    this->ReadFacsimileInterface(neume, vrvNeume);

    vrvNeume->ReadColor(neume);

    parent->AddChild(vrvNeume);
    return this->ReadLayerChildren(vrvNeume, neume, vrvNeume);
}

bool MEIInput::ReadNote(Object *parent, pugi::xml_node note)
{
    Note *vrvNote = new Note();
    this->ReadLayerElement(note, vrvNote);

    if (m_version < MEI_4_0_0) {
        if (note.attribute("size")) {
            note.remove_attribute("size");
            note.append_attribute("cue").set_value("true");
        }
    }

    this->ReadDurationInterface(note, vrvNote);
    this->ReadPitchInterface(note, vrvNote);
    this->ReadPositionInterface(note, vrvNote);
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

    this->ReadAccidAttr(note, vrvNote);

    if (vrvNote->HasTie()) {
        m_doc->SetMarkup(MARKUP_ANALYTICAL_TIE);
    }

    parent->AddChild(vrvNote);
    this->ReadUnsupportedAttr(note, vrvNote);
    return this->ReadLayerChildren(vrvNote, note, vrvNote);
}

bool MEIInput::ReadRest(Object *parent, pugi::xml_node rest)
{
    Rest *vrvRest = new Rest();
    this->ReadLayerElement(rest, vrvRest);

    if (m_version < MEI_4_0_0) {
        if (rest.attribute("size")) {
            rest.remove_attribute("size");
            rest.append_attribute("cue").set_value("true");
        }
    }

    this->ReadDurationInterface(rest, vrvRest);
    this->ReadPositionInterface(rest, vrvRest);
    vrvRest->ReadColor(rest);
    vrvRest->ReadCue(rest);
    vrvRest->ReadExtSym(rest);
    vrvRest->ReadRestVisMensural(rest);

    parent->AddChild(vrvRest);
    this->ReadUnsupportedAttr(rest, vrvRest);
    return this->ReadLayerChildren(vrvRest, rest, vrvRest);
}

bool MEIInput::ReadPlica(Object *parent, pugi::xml_node plica)
{
    Plica *vrvPlica = new Plica();
    this->ReadLayerElement(plica, vrvPlica);

    vrvPlica->ReadPlicaVis(plica);

    parent->AddChild(vrvPlica);
    this->ReadUnsupportedAttr(plica, vrvPlica);
    return true;
}

bool MEIInput::ReadProport(Object *parent, pugi::xml_node proport)
{
    Proport *vrvProport = new Proport();
    this->ReadLayerElement(proport, vrvProport);

    vrvProport->ReadDurationRatio(proport);

    parent->AddChild(vrvProport);
    this->ReadUnsupportedAttr(proport, vrvProport);
    return true;
}

bool MEIInput::ReadSpace(Object *parent, pugi::xml_node space)
{
    Space *vrvSpace = new Space();
    this->ReadLayerElement(space, vrvSpace);

    this->ReadDurationInterface(space, vrvSpace);

    parent->AddChild(vrvSpace);
    this->ReadUnsupportedAttr(space, vrvSpace);
    return true;
}

bool MEIInput::ReadSyl(Object *parent, pugi::xml_node syl)
{
    Syl *vrvSyl = new Syl();
    this->ReadLayerElement(syl, vrvSyl);

    vrvSyl->ReadLang(syl);
    vrvSyl->ReadTypography(syl);
    vrvSyl->ReadSylLog(syl);

    this->ReadFacsimileInterface(syl, vrvSyl);

    parent->AddChild(vrvSyl);
    this->ReadUnsupportedAttr(syl, vrvSyl);
    return this->ReadTextChildren(vrvSyl, syl, vrvSyl);
}

bool MEIInput::ReadSyllable(Object *parent, pugi::xml_node syllable)
{
    Syllable *vrvSyllable = new Syllable();
    this->ReadLayerElement(syllable, vrvSyllable);

    vrvSyllable->ReadColor(syllable);
    vrvSyllable->ReadSlashCount(syllable);

    parent->AddChild(vrvSyllable);
    return this->ReadLayerChildren(vrvSyllable, syllable, vrvSyllable);
}

bool MEIInput::ReadTabDurSym(Object *parent, pugi::xml_node tabRhyhtm)
{
    TabDurSym *vrvTabDurSym = new TabDurSym();
    this->ReadLayerElement(tabRhyhtm, vrvTabDurSym);

    vrvTabDurSym->ReadNNumberLike(tabRhyhtm);

    parent->AddChild(vrvTabDurSym);
    this->ReadUnsupportedAttr(tabRhyhtm, vrvTabDurSym);
    return true;
}

bool MEIInput::ReadTabGrp(Object *parent, pugi::xml_node tabGrp)
{
    TabGrp *vrvTabGrp = new TabGrp();
    this->ReadLayerElement(tabGrp, vrvTabGrp);

    this->ReadDurationInterface(tabGrp, vrvTabGrp);

    parent->AddChild(vrvTabGrp);
    this->ReadUnsupportedAttr(tabGrp, vrvTabGrp);
    return this->ReadLayerChildren(vrvTabGrp, tabGrp, vrvTabGrp);
}

bool MEIInput::ReadTuplet(Object *parent, pugi::xml_node tuplet)
{
    Tuplet *vrvTuplet = new Tuplet();
    this->ReadLayerElement(tuplet, vrvTuplet);

    vrvTuplet->ReadColor(tuplet);
    vrvTuplet->ReadDurationRatio(tuplet);
    vrvTuplet->ReadNumberPlacement(tuplet);
    vrvTuplet->ReadTupletVis(tuplet);

    parent->AddChild(vrvTuplet);
    this->ReadUnsupportedAttr(tuplet, vrvTuplet);
    return this->ReadLayerChildren(vrvTuplet, tuplet, vrvTuplet);
}

bool MEIInput::ReadVerse(Object *parent, pugi::xml_node verse)
{
    Verse *vrvVerse = new Verse();
    this->ReadLayerElement(verse, vrvVerse);

    vrvVerse->ReadColor(verse);
    vrvVerse->ReadLang(verse);
    vrvVerse->ReadNInteger(verse);
    vrvVerse->ReadTypography(verse);

    parent->AddChild(vrvVerse);
    this->ReadUnsupportedAttr(verse, vrvVerse);
    return this->ReadLayerChildren(vrvVerse, verse, vrvVerse);
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
        this->NormalizeAttributes(xmlElement);
        elementName = std::string(xmlElement.name());
        if (filter && !this->IsAllowed(elementName, filter)) {
            std::string meiElementName = filter->GetClassName();
            std::transform(meiElementName.begin(), meiElementName.begin() + 1, meiElementName.begin(), ::tolower);
            LogWarning("Element <%s> within <%s> is not supported and will be ignored ", xmlElement.name(),
                meiElementName.c_str());
            continue;
        }
        // editorial
        else if (this->IsEditorialElementName(xmlElement.name())) {
            success = this->ReadEditorialElement(parent, xmlElement, EDITORIAL_TEXT, filter);
        }
        // content
        else if (elementName == "fig") {
            success = this->ReadFig(parent, xmlElement);
        }
        else if (elementName == "lb") {
            success = this->ReadLb(parent, xmlElement);
        }
        else if (elementName == "num") {
            success = this->ReadNum(parent, xmlElement);
        }
        else if (elementName == "rend") {
            success = this->ReadRend(parent, xmlElement);
        }
        else if (elementName == "svg") {
            success = this->ReadSvg(parent, xmlElement);
        }
        else if (xmlElement.text()) {
            bool trimLeft = (i == 0);
            bool trimRight = (!xmlElement.next_sibling());
            success = this->ReadText(parent, xmlElement, trimLeft, trimRight);
        }
        // figured bass
        else if (elementName == "fb") {
            success = this->ReadFb(parent, xmlElement);
        }
        // xml comment
        else if (elementName == "") {
            success = this->ReadXMLComment(parent, xmlElement);
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
    this->SetMeiID(element, object);
    object->ReadLabelled(element);
    object->ReadTyped(element);

    return true;
}

bool MEIInput::ReadF(Object *parent, pugi::xml_node f)
{
    F *vrvF = new F();
    this->ReadTextElement(f, vrvF);

    this->ReadTimeSpanningInterface(f, vrvF);
    vrvF->ReadExtender(f);

    parent->AddChild(vrvF);
    this->ReadUnsupportedAttr(f, vrvF);
    return this->ReadTextChildren(vrvF, f, vrvF);
}

bool MEIInput::ReadFig(Object *parent, pugi::xml_node fig)
{
    Fig *vrvFig = new Fig();
    this->ReadTextElement(fig, vrvFig);

    this->ReadAreaPosInterface(fig, vrvFig);

    parent->AddChild(vrvFig);
    this->ReadUnsupportedAttr(fig, vrvFig);
    return this->ReadTextChildren(vrvFig, fig, vrvFig);
}

bool MEIInput::ReadLb(Object *parent, pugi::xml_node lb)
{
    Lb *vrvLb = new Lb();
    this->ReadTextElement(lb, vrvLb);

    parent->AddChild(vrvLb);
    this->ReadUnsupportedAttr(lb, vrvLb);
    return true;
}

bool MEIInput::ReadNum(Object *parent, pugi::xml_node num)
{
    Num *vrvNum = new Num();
    this->ReadTextElement(num, vrvNum);

    parent->AddChild(vrvNum);
    this->ReadUnsupportedAttr(num, vrvNum);
    return this->ReadTextChildren(vrvNum, num, vrvNum);
}

bool MEIInput::ReadRend(Object *parent, pugi::xml_node rend)
{
    Rend *vrvRend = new Rend();
    this->ReadTextElement(rend, vrvRend);

    this->ReadAreaPosInterface(rend, vrvRend);

    vrvRend->ReadColor(rend);
    vrvRend->ReadLang(rend);
    vrvRend->ReadTextRendition(rend);
    vrvRend->ReadTypography(rend);
    vrvRend->ReadWhitespace(rend);

    if (vrvRend->GetFirstAncestor(REND) && (vrvRend->HasHalign() || vrvRend->HasValign())) {
        LogWarning("@halign or @valign in nested <rend> element <rend> %s will be ignored", vrvRend->GetID().c_str());
        // Eventually to be added to unsupported attributes?
        vrvRend->SetHalign(HORIZONTALALIGNMENT_NONE);
        vrvRend->SetValign(VERTICALALIGNMENT_NONE);
    }

    parent->AddChild(vrvRend);
    this->ReadUnsupportedAttr(rend, vrvRend);
    return this->ReadTextChildren(vrvRend, rend, vrvRend);
}

bool MEIInput::ReadSvg(Object *parent, pugi::xml_node svg)
{
    Svg *vrvSvg = new Svg();
    this->SetMeiID(svg, vrvSvg);

    if (std::string(svg.name()) == "svg") {
        vrvSvg->Set(svg);
    }
    else {
        LogWarning("No svg content found for <fig> %s", parent->GetID().c_str());
    }

    parent->AddChild(vrvSvg);
    this->ReadUnsupportedAttr(svg, vrvSvg);
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
    interface->ReadDurationDefault(element);
    interface->ReadLyricStyle(element);
    interface->ReadMidiTempo(element);
    interface->ReadMultinumMeasures(element);
    interface->ReadPianoPedals(element);
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
    this->ReadTimePointInterface(element, interface);
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
        return this->ReadAbbr(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "add") {
        return this->ReadAdd(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "app") {
        return this->ReadApp(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "annot") {
        return this->ReadAnnot(parent, current);
    }
    else if (std::string(current.name()) == "choice") {
        return this->ReadChoice(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "corr") {
        return this->ReadCorr(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "damage") {
        return this->ReadDamage(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "del") {
        return this->ReadDel(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "expan") {
        return this->ReadExpan(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "orig") {
        return this->ReadOrig(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "ref") {
        return this->ReadRef(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "reg") {
        return this->ReadReg(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "restore") {
        return this->ReadRestore(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "sic") {
        return this->ReadSic(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "subst") {
        return this->ReadSubst(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "supplied") {
        return this->ReadSupplied(parent, current, level, filter);
    }
    else if (std::string(current.name()) == "unclear") {
        return this->ReadUnclear(parent, current, level, filter);
    }
    else {
        assert(false); // this should never happen, MEIInput::s_editorialElementNames should be updated
        return false;
    }
}

bool MEIInput::ReadEditorialElement(pugi::xml_node element, EditorialElement *object)
{
    this->SetMeiID(element, object);

    object->ReadLabelled(element);
    object->ReadTyped(element);

    return true;
}

bool MEIInput::ReadAbbr(Object *parent, pugi::xml_node abbr, EditorialLevel level, Object *filter)
{
    Abbr *vrvAbbr = new Abbr();
    this->ReadEditorialElement(abbr, vrvAbbr);

    vrvAbbr->ReadSource(abbr);

    parent->AddChild(vrvAbbr);
    this->ReadUnsupportedAttr(abbr, vrvAbbr);
    return this->ReadEditorialChildren(vrvAbbr, abbr, level, filter);
}

bool MEIInput::ReadAdd(Object *parent, pugi::xml_node add, EditorialLevel level, Object *filter)
{
    Add *vrvAdd = new Add();
    this->ReadEditorialElement(add, vrvAdd);

    vrvAdd->ReadSource(add);

    parent->AddChild(vrvAdd);
    this->ReadUnsupportedAttr(add, vrvAdd);
    return this->ReadEditorialChildren(vrvAdd, add, level, filter);
}

bool MEIInput::ReadAnnot(Object *parent, pugi::xml_node annot)
{
    Annot *vrvAnnot = new Annot();
    this->ReadEditorialElement(annot, vrvAnnot);

    vrvAnnot->ReadPlist(annot);
    vrvAnnot->ReadSource(annot);

    vrvAnnot->m_content.reset();
    // copy all the nodes inside into the document
    for (pugi::xml_node child = annot.first_child(); child; child = child.next_sibling()) {
        vrvAnnot->m_content.append_copy(child);
    }

    parent->AddChild(vrvAnnot);
    this->ReadUnsupportedAttr(annot, vrvAnnot);
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
    this->ReadEditorialElement(app, vrvApp);

    parent->AddChild(vrvApp);
    this->ReadUnsupportedAttr(app, vrvApp);
    return this->ReadAppChildren(vrvApp, app, level, filter);
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
            success = this->ReadLem(parent, current, level, filter);
        }
        else if (std::string(current.name()) == "rdg") {
            success = this->ReadRdg(parent, current, level, filter);
        }
        // xml comment
        else if (std::string(current.name()) == "") {
            success = this->ReadXMLComment(parent, current);
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
    this->ReadEditorialElement(choice, vrvChoice);

    parent->AddChild(vrvChoice);
    this->ReadUnsupportedAttr(choice, vrvChoice);
    return this->ReadChoiceChildren(vrvChoice, choice, level, filter);
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
            success = this->ReadAbbr(parent, current, level, filter);
        }
        else if (std::string(current.name()) == "choice") {
            success = this->ReadChoice(parent, current, level, filter);
        }
        else if (std::string(current.name()) == "corr") {
            success = this->ReadCorr(parent, current, level, filter);
        }
        else if (std::string(current.name()) == "expan") {
            success = this->ReadExpan(parent, current, level, filter);
        }
        else if (std::string(current.name()) == "orig") {
            success = this->ReadOrig(parent, current, level, filter);
        }
        else if (std::string(current.name()) == "ref") {
            success = this->ReadRef(parent, current, level, filter);
        }
        else if (std::string(current.name()) == "reg") {
            success = this->ReadReg(parent, current, level, filter);
        }
        else if (std::string(current.name()) == "sic") {
            success = this->ReadSic(parent, current, level, filter);
        }
        else if (std::string(current.name()) == "unclear") {
            success = this->ReadUnclear(parent, current, level, filter);
        }
        // xml comment
        else if (std::string(current.name()) == "") {
            success = this->ReadXMLComment(parent, current);
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
    this->ReadEditorialElement(corr, vrvCorr);

    vrvCorr->ReadSource(corr);

    parent->AddChild(vrvCorr);
    this->ReadUnsupportedAttr(corr, vrvCorr);
    return this->ReadEditorialChildren(vrvCorr, corr, level, filter);
}

bool MEIInput::ReadDamage(Object *parent, pugi::xml_node damage, EditorialLevel level, Object *filter)
{
    Damage *vrvDamage = new Damage();
    this->ReadEditorialElement(damage, vrvDamage);

    vrvDamage->ReadSource(damage);

    parent->AddChild(vrvDamage);
    this->ReadUnsupportedAttr(damage, vrvDamage);
    return this->ReadEditorialChildren(vrvDamage, damage, level, filter);
}

bool MEIInput::ReadDel(Object *parent, pugi::xml_node del, EditorialLevel level, Object *filter)
{
    Del *vrvDel = new Del();
    this->ReadEditorialElement(del, vrvDel);

    vrvDel->ReadSource(del);

    parent->AddChild(vrvDel);
    this->ReadUnsupportedAttr(del, vrvDel);
    return this->ReadEditorialChildren(vrvDel, del, level, filter);
}

bool MEIInput::ReadExpan(Object *parent, pugi::xml_node expan, EditorialLevel level, Object *filter)
{
    Expan *vrvExpan = new Expan();
    this->ReadEditorialElement(expan, vrvExpan);

    vrvExpan->ReadSource(expan);

    parent->AddChild(vrvExpan);
    this->ReadUnsupportedAttr(expan, vrvExpan);
    return this->ReadEditorialChildren(vrvExpan, expan, level, filter);
}

bool MEIInput::ReadLem(Object *parent, pugi::xml_node lem, EditorialLevel level, Object *filter)
{
    assert(dynamic_cast<App *>(parent));

    Lem *vrvLem = new Lem();
    // By default make them all hidden. MEIInput::ReadAppChildren will make one visible.
    vrvLem->m_visibility = Hidden;
    this->ReadEditorialElement(lem, vrvLem);

    vrvLem->ReadSource(lem);

    parent->AddChild(vrvLem);
    this->ReadUnsupportedAttr(lem, vrvLem);
    return this->ReadEditorialChildren(vrvLem, lem, level, filter);
}

bool MEIInput::ReadOrig(Object *parent, pugi::xml_node orig, EditorialLevel level, Object *filter)
{
    Orig *vrvOrig = new Orig();
    this->ReadEditorialElement(orig, vrvOrig);

    vrvOrig->ReadSource(orig);

    parent->AddChild(vrvOrig);
    this->ReadUnsupportedAttr(orig, vrvOrig);
    return this->ReadEditorialChildren(vrvOrig, orig, level, filter);
}

bool MEIInput::ReadRdg(Object *parent, pugi::xml_node rdg, EditorialLevel level, Object *filter)
{
    assert(dynamic_cast<App *>(parent));

    Rdg *vrvRdg = new Rdg();
    // By default make them all hidden. MEIInput::ReadAppChildren will make one visible.
    vrvRdg->m_visibility = Hidden;
    this->ReadEditorialElement(rdg, vrvRdg);

    vrvRdg->ReadSource(rdg);

    parent->AddChild(vrvRdg);
    this->ReadUnsupportedAttr(rdg, vrvRdg);
    return this->ReadEditorialChildren(vrvRdg, rdg, level, filter);
}

bool MEIInput::ReadRef(Object *parent, pugi::xml_node ref, EditorialLevel level, Object *filter)
{
    Ref *vrvRef = new Ref();
    this->ReadEditorialElement(ref, vrvRef);

    parent->AddChild(vrvRef);
    this->ReadUnsupportedAttr(ref, vrvRef);
    return this->ReadEditorialChildren(vrvRef, ref, level, filter);
}

bool MEIInput::ReadReg(Object *parent, pugi::xml_node reg, EditorialLevel level, Object *filter)
{
    Reg *vrvReg = new Reg();
    this->ReadEditorialElement(reg, vrvReg);

    vrvReg->ReadSource(reg);

    parent->AddChild(vrvReg);
    this->ReadUnsupportedAttr(reg, vrvReg);
    return this->ReadEditorialChildren(vrvReg, reg, level, filter);
}

bool MEIInput::ReadRestore(Object *parent, pugi::xml_node restore, EditorialLevel level, Object *filter)
{
    Restore *vrvRestore = new Restore();
    this->ReadEditorialElement(restore, vrvRestore);

    vrvRestore->ReadSource(restore);

    parent->AddChild(vrvRestore);
    this->ReadUnsupportedAttr(restore, vrvRestore);
    return this->ReadEditorialChildren(vrvRestore, restore, level, filter);
}

bool MEIInput::ReadSic(Object *parent, pugi::xml_node sic, EditorialLevel level, Object *filter)
{
    Sic *vrvSic = new Sic();
    this->ReadEditorialElement(sic, vrvSic);

    vrvSic->ReadSource(sic);

    parent->AddChild(vrvSic);
    this->ReadUnsupportedAttr(sic, vrvSic);
    return this->ReadEditorialChildren(vrvSic, sic, level, filter);
}

bool MEIInput::ReadSubst(Object *parent, pugi::xml_node subst, EditorialLevel level, Object *filter)
{
    if (!m_hasScoreDef) {
        LogError("<subst> before any <scoreDef> is not supported");
        return false;
    }
    Subst *vrvSubst = new Subst(level);
    this->ReadEditorialElement(subst, vrvSubst);

    parent->AddChild(vrvSubst);
    this->ReadUnsupportedAttr(subst, vrvSubst);
    return this->ReadSubstChildren(vrvSubst, subst, level, filter);
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
            success = this->ReadAdd(parent, current, level, filter);
        }
        else if (std::string(current.name()) == "del") {
            success = this->ReadDel(parent, current, level, filter);
        }
        else if (std::string(current.name()) == "subst") {
            success = this->ReadSubst(parent, current, level, filter);
        }
        // xml comment
        else if (std::string(current.name()) == "") {
            success = this->ReadXMLComment(parent, current);
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
    this->ReadEditorialElement(supplied, vrvSupplied);

    vrvSupplied->ReadSource(supplied);

    parent->AddChild(vrvSupplied);
    this->ReadUnsupportedAttr(supplied, vrvSupplied);
    return this->ReadEditorialChildren(vrvSupplied, supplied, level, filter);
}

bool MEIInput::ReadUnclear(Object *parent, pugi::xml_node unclear, EditorialLevel level, Object *filter)
{
    Unclear *vrvUnclear = new Unclear();
    this->ReadEditorialElement(unclear, vrvUnclear);

    vrvUnclear->ReadSource(unclear);

    parent->AddChild(vrvUnclear);
    this->ReadUnsupportedAttr(unclear, vrvUnclear);
    return this->ReadEditorialChildren(vrvUnclear, unclear, level, filter);
}

bool MEIInput::ReadEditorialChildren(Object *parent, pugi::xml_node parentNode, EditorialLevel level, Object *filter)
{
    assert(dynamic_cast<EditorialElement *>(parent));

    if (level == EDITORIAL_TOPLEVEL) {
        if (m_readingScoreBased)
            return this->ReadSectionChildren(parent, parentNode);
        else
            return this->ReadSystemChildren(parent, parentNode);
    }
    else if (level == EDITORIAL_SCOREDEF) {
        return this->ReadScoreDefChildren(parent, parentNode);
    }
    else if (level == EDITORIAL_STAFFGRP) {
        return this->ReadStaffGrpChildren(parent, parentNode);
    }
    else if (level == EDITORIAL_MEASURE) {
        return this->ReadMeasureChildren(parent, parentNode);
    }
    else if (level == EDITORIAL_STAFF) {
        return this->ReadStaffChildren(parent, parentNode);
    }
    else if (level == EDITORIAL_LAYER) {
        return this->ReadLayerChildren(parent, parentNode, filter);
    }
    else if (level == EDITORIAL_TEXT) {
        return this->ReadTextChildren(parent, parentNode, filter);
    }
    else if (level == EDITORIAL_FB) {
        return this->ReadFbChildren(parent, parentNode);
    }
    else if (level == EDITORIAL_RUNNING) {
        return this->ReadRunningChildren(parent, parentNode, filter);
    }
    else {
        return false;
    }
}

bool MEIInput::ReadTupletSpanAsTuplet(Measure *measure, pugi::xml_node tupletSpan)
{
    if (!measure) {
        LogWarning("Cannot read <tupletSpan> within editorial markup");
        return false;
    }

    Tuplet *tuplet = new Tuplet();
    this->SetMeiID(tupletSpan, tuplet);

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
        tuplet->SetNum(tupletSpan.attribute("num").as_int());
    }
    if (tupletSpan.attribute("numbase")) {
        tuplet->SetNumbase(tupletSpan.attribute("numbase").as_int());
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
        std::string refId = ExtractIDFragment(tupletSpan.attribute("startid").value());
        start = dynamic_cast<LayerElement *>(measure->FindDescendantByID(refId));
        if (!start) {
            LogWarning("Element with @startid '%s' not found when trying to read the <tupletSpan>", refId.c_str());
        }
    }
    if (tupletSpan.attribute("endid")) {
        std::string refId = ExtractIDFragment(tupletSpan.attribute("endid").value());
        end = dynamic_cast<LayerElement *>(measure->FindDescendantByID(refId));
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
        LogWarning("Start and end elements for <tupletSpan> '%s' not in the same layer", tuplet->GetID().c_str());
        delete tuplet;
        return false;
    }

    Layer *parentLayer = dynamic_cast<Layer *>(startChild->GetParent());
    assert(parentLayer);

    int startIdx = startChild->GetIdx();
    int endIdx = endChild->GetIdx();
    // LogDebug("%d %d %s!", startIdx, endIdx, start->GetID().c_str());
    int i;
    for (i = endIdx; i >= startIdx; i--) {
        LayerElement *element = dynamic_cast<LayerElement *>(parentLayer->DetachChild(i));
        if (element) tuplet->AddChild(element);
    }
    tuplet->SetParent(parentLayer);
    parentLayer->InsertChild(tuplet, startIdx);

    return true;
}

void MEIInput::SetMeiID(pugi::xml_node element, Object *object)
{
    if (!m_comment.empty()) {
        object->SetComment(m_comment);
        m_comment.clear();
    }

    if (!element.attribute("xml:id")) {
        return;
    }

    object->SetID(element.attribute("xml:id").value());
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

void MEIInput::NormalizeAttributes(pugi::xml_node &xmlElement)
{
    for (auto elem : xmlElement.attributes()) {
        std::string name = elem.name();
        std::string value = elem.value();

        size_t pos = value.find_first_not_of(' ');
        if (pos != std::string::npos) value = value.substr(pos);
        pos = value.find_last_not_of(' ');
        if (pos != std::string::npos) value = value.substr(0, pos + 1);

        elem.set_value(value.c_str());
    }
}

void MEIInput::UpgradePageTo_5_0_0(Page *page)
{
    assert(page);

    // Upgrade old page-based files by inserting a mdiv and score with corresponding boundaries
    // Works only for single page files

    Score *score = new Score();
    score->SetParent(page);
    page->InsertChild(score, 0);

    PageMilestoneEnd *scoreEnd = new PageMilestoneEnd(score);
    page->AddChild(scoreEnd);

    Mdiv *mdiv = new Mdiv();
    mdiv->SetParent(page);
    page->InsertChild(mdiv, 0);

    PageMilestoneEnd *mdivEnd = new PageMilestoneEnd(mdiv);
    page->AddChild(mdivEnd);
}

void MEIInput::UpgradeMeasureTo_5_0_0(pugi::xml_node measure)
{
    if (measure.attribute("ulx")) {
        measure.attribute("ulx").set_name("coord.x1");
    }
    if (measure.attribute("lrx")) {
        measure.attribute("lrx").set_name("coord.x2");
    }
}

void MEIInput::UpgradeStaffTo_5_0_0(pugi::xml_node staff)
{
    if (staff.attribute("uly")) {
        staff.attribute("uly").set_name("coord.y1");
    }
}

void MEIInput::UpgradeLayerElementTo_5_0_0(pugi::xml_node element)
{
    if (element.attribute("ulx")) {
        element.attribute("ulx").set_name("coord.x1");
    }
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
            interface->SetDurPpq(std::stoi(durGes));
        }
        else if (durGes.back() == 'r') {
            durGes.pop_back();
            interface->SetDurRecip(durGes);
        }
        else if (durGes.back() == 's') {
            durGes.pop_back();
            interface->SetDurReal(std::stof(durGes));
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
    this->SetMeiID(surface, vrvSurface);
    vrvSurface->ReadCoordinated(surface);
    vrvSurface->ReadTyped(surface);

    for (pugi::xml_node child = surface.first_child(); child; child = child.next_sibling()) {
        if (strcmp(child.name(), "zone") == 0) {
            this->ReadZone(vrvSurface, child);
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
    this->SetMeiID(zone, vrvZone);
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
    this->SetMeiID(facsimile, vrvFacsimile);
    // Read children
    for (pugi::xml_node child = facsimile.first_child(); child; child = child.next_sibling()) {
        if (strcmp(child.name(), "surface") == 0) {
            this->ReadSurface(vrvFacsimile, child);
        }
        else {
            LogWarning("Unsupported element <%s> in <facsimile>", child.name());
        }
    }
    doc->SetFacsimile(vrvFacsimile);
    return true;
}

} // namespace vrv
