/////////////////////////////////////////////////////////////////////////////
// Name:        iocmme.cpp
// Author:      Laurent Pugin
// Created:     2024
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "iocmme.h"

//----------------------------------------------------------------------------

#include <algorithm>
#include <cassert>
#include <fstream>
#include <sstream>
#include <string>

//----------------------------------------------------------------------------

#include "accid.h"
#include "annot.h"
#include "app.h"
#include "barline.h"
#include "chord.h"
#include "clef.h"
#include "custos.h"
#include "doc.h"
#include "dot.h"
#include "genericlayerelement.h"
#include "keyaccid.h"
#include "keysig.h"
#include "label.h"
#include "layer.h"
#include "lem.h"
#include "ligature.h"
#include "mdiv.h"
#include "measure.h"
#include "mensur.h"
#include "note.h"
#include "page.h"
#include "proport.h"
#include "rdg.h"
#include "rest.h"
#include "score.h"
#include "section.h"
#include "space.h"
#include "staff.h"
#include "staffdef.h"
#include "staffgrp.h"
#include "supplied.h"
#include "syl.h"
#include "system.h"
#include "text.h"
#include "verse.h"
#include "vrv.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// CmmeInput
//----------------------------------------------------------------------------

CmmeInput::CmmeInput(Doc *doc) : Input(doc)
{
    m_score = NULL;
    m_currentSection = NULL;
    m_currentContainer = NULL;
    m_currentSignature = NULL;
    m_currentNote = NULL;
    m_isInSyllable = false;
    m_mensInfo = NULL;
    m_numVoices = 0;
    m_activeTempoChange = false;
}

CmmeInput::~CmmeInput() {}

//////////////////////////////////////////////////////////////////////////

bool CmmeInput::Import(const std::string &cmme)
{
    try {
        m_doc->Reset();
        m_doc->SetType(Raw);
        m_doc->SetMensuralMusicOnly(BOOLEAN_true);

        // Generate the header and add a comment to the project description
        m_doc->GenerateMEIHeader();
        pugi::xml_node projectDesc = m_doc->m_header.first_child().select_node("//projectDesc").node();
        if (projectDesc) {
            pugi::xml_node p1 = projectDesc.append_child("p");
            p1.text().set("Converted from CMME XML");
        }

        pugi::xml_document doc;
        doc.load_string(cmme.c_str(), (pugi::parse_comments | pugi::parse_default) & ~pugi::parse_eol);
        pugi::xml_node root = doc.first_child();

        if (root.child("GeneralData")) {
            this->CreateMetadata(root.child("GeneralData"));
        }

        // The mDiv
        Mdiv *mdiv = new Mdiv();
        mdiv->SetVisibility(Visible);
        m_doc->AddChild(mdiv);
        // The score
        m_score = new Score();
        mdiv->AddChild(m_score);

        // We asssume that there is always as many Voice elements than given in NumVoices
        pugi::xpath_node_set voices = root.select_nodes("/Piece/VoiceData/Voice");
        for (pugi::xpath_node voiceNode : voices) {
            m_numVoices++;
            // Get the voice name if any
            std::string name = ChildAsString(voiceNode.node(), "Name");
            m_voices.push_back(name);
        }
        // Allocatate the mensural infos initialized with everything binary
        m_mensInfos.resize(m_numVoices);

        pugi::xpath_node_set musicSections = root.select_nodes("/Piece/MusicSection/*");

        for (pugi::xpath_node musicSectionNode : musicSections) {
            this->CreateSection(musicSectionNode.node());
        }

        // add minimal scoreDef
        StaffGrp *staffGrp = new StaffGrp();
        GrpSym *grpSym = new GrpSym();
        grpSym->SetSymbol(staffGroupingSym_SYMBOL_bracket);
        staffGrp->AddChild(grpSym);
        for (int i = 0; i < m_numVoices; ++i) {
            StaffDef *staffDef = new StaffDef();
            staffDef->SetN(i + 1);
            staffDef->SetLines(5);
            staffDef->SetNotationtype(NOTATIONTYPE_mensural);
            staffGrp->AddChild(staffDef);
            // Label
            if (!m_voices.at(i).empty()) {
                Label *label = new Label();
                Text *text = new Text();
                text->SetText(UTF8to32(m_voices.at(i)));
                label->AddChild(text);
                staffDef->AddChild(label);
            }
            // Default mensur with everything binary in CMME
            Mensur *mensur = new Mensur();
            mensur->SetProlatio(PROLATIO_2);
            mensur->SetTempus(TEMPUS_2);
            mensur->SetModusminor(MODUSMINOR_2);
            mensur->SetModusmaior(MODUSMAIOR_2);
            staffDef->AddChild(mensur);
        }

        assert(m_score->GetScoreDef());
        m_score->GetScoreDef()->AddChild(staffGrp);

        m_doc->ConvertToPageBasedDoc();

        this->PostProcessProport();
    }
    catch (char *str) {
        LogError("%s", str);
        return false;
    }

    return true;
}

void CmmeInput::PostProcessProport()
{
    m_doc->PrepareData();
    m_doc->ScoreDefSetCurrentDoc();

    Page *contentPage = m_doc->SetDrawingPage(0);
    assert(contentPage);

    // Layout the content with tempo changes as original
    contentPage->LayOutHorizontally();

    // Go through all proportion marked as potentential tempo change, which is their
    // status at this stage of import
    // See if they are occurring at all voices with the same values
    // If yes, this is a tempo change and mark them as such
    // Otherwise mark them as "reset" since CMME does not cummulate values for tempo change
    ListOfObjects proports = contentPage->FindAllDescendantsByType(PROPORT);
    for (Object *object : proports) {
        Proport *proport = vrv_cast<Proport *>(object);
        assert(proport);
        // Not a potential reset, or already processed
        if (proport->GetType() != "reset?") continue;
        // Default type for tempo changes
        std::string propType = "cmme_tempo_change";
        proport->SetType(propType);

        // Look at proportion aligned with it
        Alignment *alignment = proport->GetAlignment();
        Object *measure = proport->GetFirstAncestor(MEASURE);
        // Just in case
        if (!measure || !alignment) continue;
        // Check for the number of layer within the measure (e.g., section in CMME)
        const int nbLayers = measure->GetDescendantCount(LAYER);

        bool isTempoChange = true;
        bool allVoices = true;
        // Not at all voices
        if (nbLayers != alignment->GetChildCount()) {
            isTempoChange = false;
            allVoices = false;
        }
        // Check if the value of other proportions are the same
        ListOfObjects alignProports = alignment->FindAllDescendantsByType(PROPORT);
        for (Object *alignObject : alignProports) {
            Proport *alignProport = vrv_cast<Proport *>(alignObject);
            assert(proport);
            isTempoChange = isTempoChange && (proport->GetNum() == alignProport->GetNum());
            isTempoChange = isTempoChange && (proport->GetNumbase() == alignProport->GetNumbase());
        }
        if (!isTempoChange) {
            propType = "reset";
            if (allVoices) {
                LogWarning("A tempo change at all voices with different values detected");
            }
            else {
                LogWarning("A tempo change not occurring at all voices detected");
            }
        }

        for (Object *alignObject : alignProports) {
            Proport *alignProport = vrv_cast<Proport *>(alignObject);
            assert(proport);
            alignProport->SetType(propType);
        }
    }

    m_doc->ResetToLoading();

    m_doc->ResetDataPage();
}

void CmmeInput::CreateMetadata(pugi::xml_node metadataNode)
{
    pugi::xml_node titleStmt = m_doc->m_header.first_child().select_node("//titleStmt").node();
    // Should have been created by Doc::GenerateHeader
    if (!titleStmt) return;

    std::string titleStr = this->ChildAsString(metadataNode, "Title");
    if (!titleStr.empty()) {
        pugi::xml_node title = titleStmt.child("title");
        title.text().set(titleStr.c_str());
    }

    std::string sectionStr = this->ChildAsString(metadataNode, "Section");
    if (!sectionStr.empty()) {
        pugi::xml_node title = titleStmt.append_child("title");
        title.append_attribute("type") = "subordinate";
        title.text().set(sectionStr.c_str());
    }

    std::string composerStr = this->ChildAsString(metadataNode, "Composer");
    if (!composerStr.empty()) {
        // pugi::xml_node respStmt = titleStmt.append_child("respStmt");
        // pugi::xml_node persName = respStmt.append_child("persName");
        // persName.append_attribute("role") = "composer";
        // persName.text().set(composerStr.c_str());
        pugi::xml_node composer = titleStmt.append_child("composer");
        composer.text().set(composerStr.c_str());
    }
}

void CmmeInput::CreateSection(pugi::xml_node musicSectionNode)
{
    assert(m_score);

    std::string sectionType = musicSectionNode.name();

    // Create a new section
    Section *section = new Section();
    // Add the section type (MensuralMusic, Plainchant) to `@type`
    section->SetType(sectionType);
    m_score->AddChild(section);

    // Set the current section to an invisible unmeasured measure
    m_currentSection = new Measure(UNMEASURED, 1);
    section->AddChild(m_currentSection);

    // Loop through the number of voices and parse Voice or create an empty staff if not given
    for (int i = 0; i < m_numVoices; ++i) {
        std::string xpath = StringFormat("./Voice[VoiceNum[text()='%d']]", i + 1);
        pugi::xpath_node voice = musicSectionNode.select_node(xpath.c_str());
        if (voice) {
            this->CreateStaff(voice.node());
        }
        else {
            Staff *staff = new Staff(i + 1);
            staff->SetVisible(BOOLEAN_false);
            m_currentSection->AddChild(staff);
        }
    }
}

void CmmeInput::CreateStaff(pugi::xml_node voiceNode)
{
    assert(m_currentSection);

    int numVoice = this->ChildAsInt(voiceNode, "VoiceNum");

    Staff *staff = new Staff(numVoice);
    Layer *layer = new Layer();
    layer->SetN(1);
    m_currentContainer = layer;

    // (Re)-set the current mens info to the corresponding voice
    m_mensInfo = &m_mensInfos.at(numVoice - 1);
    // Reset the syllable position
    m_isInSyllable = false;
    m_currentSignature = NULL;
    m_activeTempoChange = false;

    staff->AddChild(m_currentContainer);
    m_currentSection->AddChild(staff);

    // Loop through the event lists
    this->ReadEvents(voiceNode.child("EventList"));
}

void CmmeInput::CreateApp(pugi::xml_node appNode)
{
    assert(m_currentContainer);

    App *app = new App(EDITORIAL_LAYER);
    m_currentContainer->AddChild(app);
    m_currentContainer = app;

    // Loop through the event lists
    pugi::xpath_node_set lemOrRdgs = appNode.select_nodes("./Reading");
    bool isFirst = true;
    for (pugi::xpath_node lemOrRdg : lemOrRdgs) {
        pugi::xml_node lemOrRdgNode = lemOrRdg.node();
        this->CreateLemOrRdg(lemOrRdgNode, isFirst);
        isFirst = false;
    }

    m_currentContainer = m_currentContainer->GetParent();
}

void CmmeInput::CreateLemOrRdg(pugi::xml_node lemOrRdgNode, bool isFirst)
{
    assert(m_currentContainer);
    std::string versionId = this->ChildAsString(lemOrRdgNode, "VariantVersionID");

    EditorialElement *lemOrRdg = NULL;
    if (isFirst && (lemOrRdgNode.child("PreferredReading") || (versionId == "DEFAULT"))) {
        lemOrRdg = new Lem();
    }
    else {
        lemOrRdg = new Rdg();
    }
    lemOrRdg->SetVisibility((isFirst) ? Visible : Hidden);

    if (lemOrRdg->Is(RDG)) {
        std::string label;
        // Loop through the event lists
        pugi::xpath_node_set variants = lemOrRdgNode.select_nodes("./VariantVersionID");
        bool isFirst = true;
        for (pugi::xpath_node variant : variants) {
            if (!isFirst) label += "; ";
            label += this->AsString(variant.node());
            isFirst = false;
        }
        lemOrRdg->SetLabel(label);
    }

    if (lemOrRdgNode.child("Error")) {
        lemOrRdg->SetType("Error");
    }
    else if (lemOrRdgNode.child("Lacuna")) {
        lemOrRdg->SetType("Lacuna");
    }

    m_currentContainer->AddChild(lemOrRdg);

    m_currentContainer = lemOrRdg;

    this->ReadEvents(lemOrRdgNode.child("Music"));

    m_currentContainer = m_currentContainer->GetParent();
}

void CmmeInput::ReadEvents(pugi::xml_node eventsNode)
{
    assert(m_currentContainer);

    bool keySigFound = false;

    // Loop through the event lists
    pugi::xpath_node_set events = eventsNode.select_nodes("./*");
    for (pugi::xpath_node event : events) {
        pugi::xml_node eventNode = event.node();
        std::string name = eventNode.name();
        if (name == "Clef") {
            if (this->IsClef(eventNode)) {
                this->CreateClef(eventNode);
            }
            else if (eventNode.select_node("./Signature")) {
                keySigFound = true;
                this->CreateKeySig(eventNode);
            }
            else {
                this->CreateAccid(eventNode);
            }
        }
        else if (name == "ColorChange") {
            this->CreateColorChange(eventNode);
        }
        else if (name == "Custos") {
            this->CreateCustos(eventNode);
        }
        else if (name == "Dot") {
            this->CreateDot(eventNode);
        }
        else if (name == "LineEnd") {
            this->CreateBreak(eventNode);
        }
        else if (name == "Mensuration") {
            this->CreateMensuration(eventNode);
        }
        else if (name == "MiscItem") {
            /// Assuming that a MiscItem contains only one child
            if (eventNode.select_node("./Barline")) {
                pugi::xml_node barlineNode = eventNode.select_node("./Barline").node();
                this->CreateBarline(barlineNode);
            }
            else if (eventNode.child("Ellipsis")) {
                this->CreateEllipsis();
            }
            else if (eventNode.child("Lacuna")) {
                this->CreateLacuna(eventNode.child("Lacuna"));
            }
            else {
                LogWarning("Unsupported MiscItem content");
            }
        }
        else if (name == "MultiEvent") {
            /// Assuming that a multievent contains a key signature, all events are key signatures
            if (eventNode.select_node("./Clef/Signature")) {
                m_currentSignature = NULL;
                pugi::xpath_node_set clefs = eventNode.select_nodes("./Clef");
                for (pugi::xpath_node clef : clefs) {
                    pugi::xml_node clefNode = clef.node();
                    this->CreateKeySig(clefNode);
                }
            }
            else if (eventNode.select_node("./Note")) {
                // Assuming that this only contains notes (and is a chord)
                this->CreateChord(eventNode);
            }
            else {
                LogWarning("Unsupported event '%s'", name.c_str());
            }
        }
        else if (name == "Note") {
            this->CreateNote(eventNode);
        }
        else if (name == "OriginalText") {
            this->CreateOriginalText(eventNode);
        }
        else if (name == "Proportion") {
            this->CreateProport(eventNode);
        }
        else if (name == "Rest") {
            this->CreateRest(eventNode);
        }
        else if (name == "VariantReadings") {
            this->CreateApp(eventNode);
        }
        else {
            LogWarning("Unsupported event '%s'", name.c_str());
        }
        if (!keySigFound) {
            m_currentSignature = NULL;
        }
        keySigFound = false;
    }
}

void CmmeInput::ReadEditorialCommentary(pugi::xml_node eventNode, Object *object)
{
    std::string commentary = this->ChildAsString(eventNode, "EditorialCommentary");

    if (!commentary.empty()) {
        Annot *annot = new Annot();
        Text *text = new Text();
        text->SetText(UTF8to32(commentary));
        annot->AddChild(text);
        xsdAnyURI_List list;
        list.push_back("#" + object->GetID());
        annot->SetPlist(list);
        m_currentSection->AddChild(annot);
    }
}

void CmmeInput::CreateAccid(pugi::xml_node accidNode)
{
    static const std::map<std::string, data_ACCIDENTAL_WRITTEN> shapeMap{
        { "Bmol", ACCIDENTAL_WRITTEN_f }, //
        { "BmolDouble", ACCIDENTAL_WRITTEN_f }, //
        { "Bqua", ACCIDENTAL_WRITTEN_n }, //
        { "Diesis", ACCIDENTAL_WRITTEN_s }, //
    };

    static const std::map<std::string, data_PITCHNAME> pitchMap{
        { "C", PITCHNAME_c }, //
        { "D", PITCHNAME_d }, //
        { "E", PITCHNAME_e }, //
        { "F", PITCHNAME_f }, //
        { "G", PITCHNAME_g }, //
        { "A", PITCHNAME_a }, //
        { "B", PITCHNAME_b } //
    };

    assert(m_currentContainer);

    Accid *accid = new Accid();
    std::string appearance = this->ChildAsString(accidNode, "Appearance");
    data_ACCIDENTAL_WRITTEN accidWritten
        = shapeMap.contains(appearance) ? shapeMap.at(appearance) : ACCIDENTAL_WRITTEN_f;
    accid->SetAccid(accidWritten);

    std::string step = this->ChildAsString(accidNode, "Pitch/LetterName");
    // Default pitch to C
    data_PITCHNAME ploc = pitchMap.contains(step) ? pitchMap.at(step) : PITCHNAME_c;
    accid->SetPloc(ploc);

    int oct = this->ChildAsInt(accidNode, "Pitch/OctaveNum");
    if ((ploc != PITCHNAME_a) && (ploc != PITCHNAME_b)) oct += 1;
    accid->SetOloc(oct);

    int staffLoc = this->ChildAsInt(accidNode, "StaffLoc");
    accid->SetLoc(staffLoc - 1);

    this->ReadEditorialCommentary(accidNode, accid);

    m_currentContainer->AddChild(accid);
}

void CmmeInput::CreateBarline(pugi::xml_node barlineNode)
{
    assert(m_currentContainer);

    BarLine *barLine = new BarLine();

    // Determine the barLine/@form based on the CMME <Barline>'s <NumLines> and <RepeatSign>
    int formNumLines = this->ChildAsInt(barlineNode, "NumLines");
    if (formNumLines == 1) {
        barLine->SetForm(BARRENDITION_single);
    }
    else if (formNumLines == 2) {
        barLine->SetForm(BARRENDITION_dbl);
    }
    else if (formNumLines != VRV_UNSET) {
        LogWarning("Unsupported barline (with more than 2 lines)");
    }

    // `@form` is overwritten to 'rptboth' when <RepeatSign> is used
    if (barlineNode.select_node("./RepeatSign")) {
        barLine->SetForm(BARRENDITION_rptboth);
    }
    // Determine the barLine/@place
    int bottomLine = this->ChildAsInt(barlineNode, "BottomStaffLine");
    if (bottomLine != VRV_UNSET) {
        int place = bottomLine * 2;
        barLine->SetPlace(place);
    }
    // Determine the barLine/@len
    int numSpaces = this->ChildAsInt(barlineNode, "NumSpaces");
    if (numSpaces != VRV_UNSET) {
        barLine->SetLen(numSpaces * 2);
    }

    m_currentContainer->AddChild(barLine);
}

void CmmeInput::CreateBreak(pugi::xml_node breakNode)
{
    assert(m_currentContainer);

    // This is either a system or page break (usually only
    // in one part, so not easy to visualise in score)
    if (breakNode.select_node("./PageEnd")) {
        GenericLayerElement *pb = new GenericLayerElement("pb");
        m_currentContainer->AddChild(pb);
    }
    else {
        GenericLayerElement *sb = new GenericLayerElement("sb");
        m_currentContainer->AddChild(sb);
    }
}

void CmmeInput::CreateChord(pugi::xml_node chordNode)
{
    assert(m_currentContainer);

    // CMME can have 'chords' in ligatures - we keep only the first note
    bool inLigature = (m_currentContainer->Is(LIGATURE));

    if (!inLigature) {
        Chord *chord = new Chord();
        m_currentContainer->AddChild(chord);
        m_currentContainer = chord;
    }
    pugi::xpath_node_set events = chordNode.select_nodes("./*");
    for (pugi::xpath_node event : events) {
        pugi::xml_node eventNode = event.node();
        std::string name = eventNode.name();
        if (name == "Note") {
            this->CreateNote(eventNode);
            if (inLigature) break;
        }
        else {
            LogWarning("Unsupported chord component: '%s'", name.c_str());
        }
    }
    if (!inLigature) m_currentContainer = m_currentContainer->GetParent();
}

void CmmeInput::CreateClef(pugi::xml_node clefNode)
{
    static const std::map<std::string, data_CLEFSHAPE> shapeMap{
        { "C", CLEFSHAPE_C }, //
        { "F", CLEFSHAPE_F }, //
        { "G", CLEFSHAPE_G }, //
        { "Frnd", CLEFSHAPE_F }, //
        { "Fsqr", CLEFSHAPE_F }, //
    };

    assert(m_currentContainer);

    Clef *clef = new Clef();
    int staffLoc = this->ChildAsInt(clefNode, "StaffLoc");
    staffLoc = (staffLoc + 1) / 2;
    clef->SetLine(staffLoc);

    std::string appearance = this->ChildAsString(clefNode, "Appearance");
    // Default clef to C
    data_CLEFSHAPE shape = shapeMap.contains(appearance) ? shapeMap.at(appearance) : CLEFSHAPE_C;
    clef->SetShape(shape);

    this->ReadEditorialCommentary(clefNode, clef);

    m_currentContainer->AddChild(clef);

    return;
}

void CmmeInput::CreateColorChange(pugi::xml_node colorChangeNode)
{
    static const std::map<std::string, std::string> colorMap{
        { "Black", "" }, //
        { "Red", "red" }, //
        { "Blue", "blue" }, //
        { "Green", "green" }, //
        { "Yellow", "yellow" }, //
    };

    assert(m_currentContainer);

    std::string color;
    if (colorChangeNode.child("PrimaryColor")) {
        color = this->ChildAsString(colorChangeNode.child("PrimaryColor"), "Color");
    }
    m_currentColor = colorMap.contains(color) ? colorMap.at(color) : "";

    return;
}

void CmmeInput::CreateCustos(pugi::xml_node custosNode)
{
    static const std::map<std::string, data_PITCHNAME> pitchMap{
        { "C", PITCHNAME_c }, //
        { "D", PITCHNAME_d }, //
        { "E", PITCHNAME_e }, //
        { "F", PITCHNAME_f }, //
        { "G", PITCHNAME_g }, //
        { "A", PITCHNAME_a }, //
        { "B", PITCHNAME_b } //
    };

    assert(m_currentContainer);

    Custos *custos = new Custos();
    std::string step = this->ChildAsString(custosNode, "LetterName");
    // Default pitch to C
    data_PITCHNAME pname = pitchMap.contains(step) ? pitchMap.at(step) : PITCHNAME_c;
    custos->SetPname(pname);
    if (!m_currentColor.empty()) custos->SetColor(m_currentColor);

    int oct = this->ChildAsInt(custosNode, "OctaveNum");
    if ((pname != PITCHNAME_a) && (pname != PITCHNAME_b)) oct += 1;
    custos->SetOct(oct);

    this->ReadEditorialCommentary(custosNode, custos);

    m_currentContainer->AddChild(custos);

    return;
}

void CmmeInput::CreateDot(pugi::xml_node dotNode)
{
    assert(m_currentContainer);

    Dot *dot = new Dot();
    m_currentContainer->AddChild(dot);

    this->ReadEditorialCommentary(dotNode, dot);

    return;
}

void CmmeInput::CreateEllipsis()
{
    assert(m_currentContainer);

    GenericLayerElement *gap = new GenericLayerElement("gap");
    gap->SetType("cmme_ellipsis");
    gap->m_unsupported.push_back(std::make_pair("reason", "incipit"));
    m_currentContainer->AddChild(gap);
}

void CmmeInput::CreateKeySig(pugi::xml_node keyNode)
{
    static const std::map<std::string, data_ACCIDENTAL_WRITTEN> shapeMap{
        { "Bmol", ACCIDENTAL_WRITTEN_f }, //
        { "BmolDouble", ACCIDENTAL_WRITTEN_f }, //
        { "Bqua", ACCIDENTAL_WRITTEN_n }, //
        { "Diesis", ACCIDENTAL_WRITTEN_s }, //
    };

    static const std::map<std::string, data_PITCHNAME> pitchMap{
        { "C", PITCHNAME_c }, //
        { "D", PITCHNAME_d }, //
        { "E", PITCHNAME_e }, //
        { "F", PITCHNAME_f }, //
        { "G", PITCHNAME_g }, //
        { "A", PITCHNAME_a }, //
        { "B", PITCHNAME_b } //
    };

    assert(m_currentContainer);

    if (!m_currentSignature) {
        m_currentSignature = new KeySig();
        m_currentContainer->AddChild(m_currentSignature);
    }

    KeyAccid *keyAccid = new KeyAccid();
    std::string appearance = this->ChildAsString(keyNode, "Appearance");
    data_ACCIDENTAL_WRITTEN accid = shapeMap.contains(appearance) ? shapeMap.at(appearance) : ACCIDENTAL_WRITTEN_f;
    keyAccid->SetAccid(accid);

    std::string step = this->ChildAsString(keyNode, "Pitch/LetterName");
    // Default pitch to C
    data_PITCHNAME pname = pitchMap.contains(step) ? pitchMap.at(step) : PITCHNAME_c;
    keyAccid->SetPname(pname);

    int oct = this->ChildAsInt(keyNode, "Pitch/OctaveNum");
    if ((pname != PITCHNAME_a) && (pname != PITCHNAME_b)) oct += 1;
    keyAccid->SetOct(oct);

    int staffLoc = this->ChildAsInt(keyNode, "StaffLoc");
    keyAccid->SetLoc(staffLoc - 1);

    this->ReadEditorialCommentary(keyNode, keyAccid);

    m_currentSignature->AddChild(keyAccid);
}

void CmmeInput::CreateLacuna(pugi::xml_node lacunaNode)
{
    // A lacuna is used in CMME to pad a part where
    // the scribe's version is temporally incomplete.
    // We use mei:space, but since this is not explicit
    // in the source, we wrap it in mei:supplied
    assert(m_currentContainer);
    Space *space = new Space();
    Supplied *supplied = new Supplied();
    supplied->AddChild(space);
    int num;
    int numbase;
    data_DURATION duration = this->ReadDuration(lacunaNode, num, numbase);
    space->SetDur(duration);
    space->SetType("cmme_lacuna");
    supplied->SetType("cmme_lacuna");
    m_currentContainer->AddChild(supplied);
}

void CmmeInput::CreateMensuration(pugi::xml_node mensurationNode)
{
    assert(m_currentContainer);
    assert(m_mensInfo);

    pugi::xml_node mensInfo = mensurationNode.child("MensInfo");
    if (mensInfo) {
        m_mensInfo->prolatio = this->ChildAsInt(mensInfo, "Prolatio");
        m_mensInfo->tempus = this->ChildAsInt(mensInfo, "Tempus");
        m_mensInfo->modusminor = this->ChildAsInt(mensInfo, "ModusMinor");
        m_mensInfo->modusmaior = this->ChildAsInt(mensInfo, "ModusMaior");
    }
    // If there is no <MensInfo> then resets everything to binary
    else {
        m_mensInfo->prolatio = 2;
        m_mensInfo->tempus = 2;
        m_mensInfo->modusminor = 2;
        m_mensInfo->modusmaior = 2;
    }

    /// Mensuration: logical domain
    Mensur *mensur = new Mensur();
    data_PROLATIO prolatio = (m_mensInfo->prolatio == 3) ? PROLATIO_3 : PROLATIO_2;
    mensur->SetProlatio(prolatio);
    data_TEMPUS tempus = (m_mensInfo->tempus == 3) ? TEMPUS_3 : TEMPUS_2;
    mensur->SetTempus(tempus);
    data_MODUSMINOR modusminor = (m_mensInfo->modusminor == 3) ? MODUSMINOR_3 : MODUSMINOR_2;
    mensur->SetModusminor(modusminor);
    data_MODUSMAIOR modusmaior = (m_mensInfo->modusmaior == 3) ? MODUSMAIOR_3 : MODUSMAIOR_2;
    mensur->SetModusmaior(modusmaior);

    /// Mensuration: visual domain
    /// Mensuration/Sign/MainSymbol to @sign
    pugi::xml_node signNode = mensurationNode.child("Sign");
    std::string signValue = this->ChildAsString(signNode, "MainSymbol");
    if (signValue == "O") {
        mensur->SetSign(MENSURATIONSIGN_O);
        // If the  is no mensInfo, infer it from the Sign
        if (!mensInfo) {
            m_mensInfo->tempus = 3;
            mensur->SetTempus(TEMPUS_3);
        }
    }
    else if (signValue == "C") {
        mensur->SetSign(MENSURATIONSIGN_C);
    }
    else if (signValue != "") {
        LogWarning("Unsupported mesuration sign in CMME (not 'O' or 'C')");
    }

    /// Mensuration/Sign/Dot to @dot
    if (signNode.child("Dot")) {
        mensur->SetDot(BOOLEAN_true);
        // If there is no mensInfo, infer it from the Dot
        if (!mensInfo) {
            m_mensInfo->prolatio = 3;
            mensur->SetProlatio(PROLATIO_3);
        }
    }

    /// Mensuration/Sign/Strokes to @slash
    int strokes = this->ChildAsInt(signNode, "Strokes");
    if (strokes != VRV_UNSET) {
        mensur->SetSlash(strokes);
    }

    /// Mensuration/Sign/Orientation to @orient
    static const std::map<std::string, data_ORIENTATION> orientationMap{
        { "Reversed", ORIENTATION_reversed }, //
        { "90CW", ORIENTATION_90CW }, //
        { "90CCW", ORIENTATION_90CCW } //
    };
    std::string orientation = this->ChildAsString(signNode, "Orientation");
    data_ORIENTATION orient = orientationMap.contains(orientation) ? orientationMap.at(orientation) : ORIENTATION_NONE;
    mensur->SetOrient(orient);

    /// Mensuration/Small to @fontsize=small (not yet rendered in Verovio).
    /// In the long run, we should add @size to att.mensur.vis because we have @mensur.size for <staffDef>, see class
    /// att.mensural.vis
    pugi::xml_node smallNode = mensurationNode.child("Small");
    if (smallNode != NULL) {
        mensur->m_unsupported.push_back(std::make_pair("fontsize", "small"));
    }

    /// Mesuration/NoScoreEffect to @type = cmme_no_score_effect
    pugi::xml_node noScoreEffect = mensurationNode.child("NoScoreEffect");
    if (noScoreEffect != NULL) {
        mensur->SetType("cmme_no_score_effect");
    }

    /// Mensuration/Number/Num to @num and Number/Den to @numbase
    /// However, Number/Den cannot be entered in the CMME Editor.
    /// It can only be added in the XML manually and imported into the CMME Editor,
    /// where it won't render, but one can see it in the "Event Inspector."
    pugi::xml_node numberNode = mensurationNode.child("Number");
    if (numberNode != NULL) {
        int numValue = this->ChildAsInt(numberNode, "Num");
        int denValue = this->ChildAsInt(numberNode, "Den");
        if (numValue != VRV_UNSET and numValue != 0) {
            mensur->SetNum(numValue);
        }
        if (denValue != VRV_UNSET and denValue != 0) {
            mensur->SetNumbase(denValue);
        }
    }

    /// Mensuration/StaffLoc to @loc
    int staffLoc = this->ChildAsInt(mensurationNode, "StaffLoc");
    if (staffLoc != VRV_UNSET) {
        mensur->SetLoc(staffLoc);
    }

    this->ReadEditorialCommentary(mensurationNode, mensur);

    m_currentContainer->AddChild(mensur);

    /// Proportion part coming from CMME <TempoChange> in <MensInfo> in <Mensuration>. In this case, create an MEI
    /// <proport> element that follows the MEI <mensuration> element and that contains the proport/@num and
    /// proport/@numbase values of 'num' and 'den'
    pugi::xml_node tempoChangeNode = mensInfo.child("TempoChange");
    if (tempoChangeNode != NULL) {
        Proport *proport = new Proport();
        int numVal = this->ChildAsInt(tempoChangeNode, "Num");
        int denVal = this->ChildAsInt(tempoChangeNode, "Den");
        if (numVal != VRV_UNSET) {
            proport->SetNum(numVal);
        }
        if (denVal != VRV_UNSET) {
            proport->SetNumbase(denVal);
        }
        // Mark them as potential tempo changes (i.e., reset)
        proport->SetType("reset?");
        m_currentContainer->AddChild(proport);
        m_activeTempoChange = true;
    }
    // CMME resets the tempo change with a Mensuration with no tempo change
    // We need to add one in the MEI
    else if (m_activeTempoChange) {
        Proport *proport = new Proport();
        proport->SetNum(1);
        proport->SetNumbase(1);
        // Mark them as potential tempo changes
        proport->SetType("cmme_tempo_change?");
        m_currentContainer->AddChild(proport);
        m_activeTempoChange = false;
    }

    return;
}

void CmmeInput::CreateNote(pugi::xml_node noteNode)
{
    static const std::map<std::string, data_PITCHNAME> pitchMap{
        { "C", PITCHNAME_c }, //
        { "D", PITCHNAME_d }, //
        { "E", PITCHNAME_e }, //
        { "F", PITCHNAME_f }, //
        { "G", PITCHNAME_g }, //
        { "A", PITCHNAME_a }, //
        { "B", PITCHNAME_b } //
    };

    static const std::map<int, data_ACCIDENTAL_WRITTEN> accidMap{
        { -3, ACCIDENTAL_WRITTEN_tf }, //
        { -2, ACCIDENTAL_WRITTEN_ff }, //
        { -1, ACCIDENTAL_WRITTEN_f }, //
        { 0, ACCIDENTAL_WRITTEN_n }, //
        { 1, ACCIDENTAL_WRITTEN_s }, //
        { 2, ACCIDENTAL_WRITTEN_ss }, //
        { 3, ACCIDENTAL_WRITTEN_sx }, //
    };

    static const std::map<std::string, data_STEMDIRECTION> stemDirMap{
        { "Up", STEMDIRECTION_up }, //
        { "Down", STEMDIRECTION_down }, //
        { "Left", STEMDIRECTION_left }, //
        { "Right", STEMDIRECTION_right }, //
    };

    assert(m_currentContainer);

    Note *note = new Note();
    std::string step = this->ChildAsString(noteNode, "LetterName");
    // Default pitch to C
    data_PITCHNAME pname = pitchMap.contains(step) ? pitchMap.at(step) : PITCHNAME_c;
    note->SetPname(pname);
    if (!m_currentColor.empty()) note->SetColor(m_currentColor);

    int num;
    int numbase;
    data_DURATION duration = this->ReadDuration(noteNode, num, numbase);
    note->SetDur(duration);
    if (num != VRV_UNSET && numbase != VRV_UNSET) {
        note->SetNum(num);
        note->SetNumbase(numbase);
    }

    int oct = this->ChildAsInt(noteNode, "OctaveNum");
    if ((pname != PITCHNAME_a) && (pname != PITCHNAME_b)) oct += 1;
    note->SetOct(oct);

    if (noteNode.child("Colored")) {
        note->SetColored(BOOLEAN_true);
    }

    if (noteNode.child("ModernText")) {
        m_currentNote = note;
        this->CreateVerse(noteNode.child("ModernText"));
        m_currentNote = NULL;
    }

    if (noteNode.child("Corona")) {
        data_STAFFREL_basic position = STAFFREL_basic_above;
        if (noteNode.select_node("Corona/Orientation[text()='Down']")) {
            position = STAFFREL_basic_below;
        }
        note->SetFermata(position);
    }

    if (noteNode.child("ModernAccidental")) {
        Accid *accid = new Accid();
        int offset = this->ChildAsInt(noteNode.child("ModernAccidental"), "PitchOffset");
        offset = std::min(3, offset);
        offset = std::max(-3, offset);
        // Default pitch to C
        data_ACCIDENTAL_WRITTEN accidWritten = accidMap.contains(offset) ? accidMap.at(offset) : ACCIDENTAL_WRITTEN_n;
        accid->SetAccid(accidWritten);
        accid->SetFunc(accidLog_FUNC_edit);
        note->AddChild(accid);
    }

    if (noteNode.child("Signum")) {
        // MEI currently lacks signum congruentiae, so we warn and set not type
        LogWarning("Signum Congruentiae in CMME mapped to @type");
        note->SetType("cmme_signum_congruentiae");
    }

    if (noteNode.child("Stem")) {
        std::string dir = this->ChildAsString(noteNode.child("Stem"), "Dir");
        if (dir == "Barline") {
            LogWarning("Unsupported 'Barline' stem direction");
        }
        data_STEMDIRECTION stemDir = stemDirMap.contains(dir) ? stemDirMap.at(dir) : STEMDIRECTION_NONE;
        note->SetStemDir(stemDir);

        std::string side = this->ChildAsString(noteNode.child("Stem"), "Side");
        if (side == "Left") {
            note->SetStemPos(STEMPOSITION_left);
        }
        else if (side == "Right") {
            note->SetStemPos(STEMPOSITION_right);
        }
    }

    if (noteNode.child("Lig")) {
        std::string lig = this->ChildAsString(noteNode, "Lig");
        if (lig == "Retrorsum") {
            LogWarning("Unsupported 'Retrorsum' ligature");
        }
        data_LIGATUREFORM form = (lig == "Obliqua") ? LIGATUREFORM_obliqua : LIGATUREFORM_recta;
        // First note of the ligature, create the ligature element
        if (!m_currentContainer->Is(LIGATURE)) {
            if (m_currentContainer->Is(CHORD)) {
                LogWarning("Ligature within chord is not supported");
            }
            else {
                Ligature *ligature = new Ligature();
                ligature->SetForm(form);
                m_currentContainer->AddChild(ligature);
                m_currentContainer = ligature;
            }
        }
        // Otherwise simply add the `@lig` to the note
        else {
            note->SetLig(form);
        }
    }

    this->ReadEditorialCommentary(noteNode, note);

    m_currentContainer->AddChild(note);

    // We have processed the last note of a ligature
    if (m_currentContainer->Is(LIGATURE) && !noteNode.child("Lig")) {
        m_currentContainer = m_currentContainer->GetParent();
    }

    return;
}

void CmmeInput::CreateOriginalText(pugi::xml_node originalTextNode)
{
    return;
}

void CmmeInput::CreateProport(pugi::xml_node proportNode)
{
    assert(m_currentContainer);
    assert(m_mensInfo);

    /// Proportion part coming from CMME <Proportion>. In this case, create an MEI <proport> element is created alone
    /// (not following an MEI <mensuration> element)
    Proport *proport = new Proport();
    int numVal = this->ChildAsInt(proportNode, "Num");
    int denVal = this->ChildAsInt(proportNode, "Den");
    if (numVal != VRV_UNSET) {
        proport->SetNum(numVal);
        // Cumulated it
        m_mensInfo->proportNum *= numVal;
    }
    if (denVal != VRV_UNSET) {
        proport->SetNumbase(denVal);
        // Cumulated it
        m_mensInfo->proportDen *= denVal;
    }
    Fraction::Reduce(m_mensInfo->proportNum, m_mensInfo->proportDen);
    proport->SetType("cmme_proportion");
    m_currentContainer->AddChild(proport);
    return;
}

void CmmeInput::CreateRest(pugi::xml_node restNode)
{
    assert(m_currentContainer);

    Rest *rest = new Rest();
    int num;
    int numbase;
    data_DURATION duration = this->ReadDuration(restNode, num, numbase);
    rest->SetDur(duration);
    if (!m_currentColor.empty()) rest->SetColor(m_currentColor);

    if (num != VRV_UNSET && numbase != VRV_UNSET) {
        rest->SetNum(num);
        rest->SetNumbase(numbase);
    }

    this->ReadEditorialCommentary(restNode, rest);

    if (restNode.child("Signum")) {
        // MEI currently lacks signum congruentiae, so we warn and set not type
        LogWarning("Signum Congruentiae in CMME mapped to @type");
        rest->SetType("cmme_signum_congruentiae");
    }

    m_currentContainer->AddChild(rest);

    return;
}

void CmmeInput::CreateVerse(pugi::xml_node verseNode)
{
    assert(m_currentNote);

    Verse *verse = new Verse();
    verse->SetN(1);
    Syl *syl = new Syl();
    Text *text = new Text();
    std::string sylText = this->ChildAsString(verseNode, "Syllable");
    text->SetText(UTF8to32(sylText));

    if (verseNode.child("WordEnd")) {
        syl->SetWordpos(sylLog_WORDPOS_t);
        m_isInSyllable = false;
    }
    else {
        if (m_isInSyllable) {
            syl->SetWordpos(sylLog_WORDPOS_m);
        }
        else {
            syl->SetWordpos(sylLog_WORDPOS_i);
        }
        m_isInSyllable = true;
        syl->SetCon(sylLog_CON_d);
    }

    syl->AddChild(text);
    verse->AddChild(syl);
    m_currentNote->AddChild(verse);
}

data_DURATION CmmeInput::ReadDuration(pugi::xml_node durationNode, int &num, int &numbase) const
{
    static const std::map<std::string, data_DURATION> durationMap{
        { "Maxima", DURATION_maxima }, //
        { "Longa", DURATION_longa }, //
        { "Brevis", DURATION_brevis }, //
        { "Semibrevis", DURATION_semibrevis }, //
        { "Minima", DURATION_minima }, //
        { "Semiminima", DURATION_semiminima }, //
        { "Fusa", DURATION_fusa }, //
        { "Semifusa", DURATION_semifusa } //
    };

    assert(m_mensInfo);

    std::string type = this->ChildAsString(durationNode, "Type");
    // Default duration to brevis
    data_DURATION duration = durationMap.contains(type) ? durationMap.at(type) : DURATION_brevis;

    num = VRV_UNSET;
    numbase = VRV_UNSET;

    if (durationNode.child("Length")) {
        int cmmeNum = this->ChildAsInt(durationNode.child("Length"), "Num");
        int cmmeDen = this->ChildAsInt(durationNode.child("Length"), "Den");

        if ((cmmeNum == VRV_UNSET) || (cmmeDen == VRV_UNSET)) {
            return duration;
        }

        std::pair<int, int> ratio = { 1, 1 };

        if (type == "Maxima") {
            ratio.first *= m_mensInfo->modusmaior * m_mensInfo->modusminor * m_mensInfo->tempus * m_mensInfo->prolatio;
        }
        else if (type == "Longa") {
            ratio.first *= m_mensInfo->modusminor * m_mensInfo->tempus * m_mensInfo->prolatio;
        }
        else if (type == "Brevis") {
            ratio.first *= m_mensInfo->tempus * m_mensInfo->prolatio;
        }
        else if (type == "Semibrevis") {
            ratio.first *= m_mensInfo->prolatio;
        }
        else if (type == "Semiminima") {
            ratio.second = 2;
        }
        else if (type == "Fusa") {
            ratio.second = 4;
        }
        else if (type == "Semifusa") {
            ratio.second = 8;
        }

        // That would apply the proportion, but not needed because CMME works the other way around
        // That is, the propostion is not coded in the note value but applied by the CMME processor
        // cmmeNum *= m_mensInfo->proportNum;
        // cmmeDen *= m_mensInfo->proportDen;

        cmmeNum *= ratio.second;
        cmmeDen *= ratio.first;

        // MEI num and numabase are cmme den and num respectively
        num = cmmeDen;
        numbase = cmmeNum;

        Fraction::Reduce(num, numbase);

        if (num == numbase) {
            num = VRV_UNSET;
            numbase = VRV_UNSET;
        }
    }

    return duration;
}

bool CmmeInput::IsClef(const pugi::xml_node clefNode) const
{
    static std::vector<std::string> clefs = { "C", "F", "Fsqr", "Frnd", "G" };

    // Checking this is not enough since it is somethimes missing in CMME files
    if (clefNode.select_node("./Signature")) return false;

    // Also check the clef appearance
    std::string appearance = this->ChildAsString(clefNode, "Appearance");
    return (std::find(clefs.begin(), clefs.end(), appearance) != clefs.end());
}

std::string CmmeInput::AsString(const pugi::xml_node node) const
{
    if (!node) return "";

    if (node.text()) {
        return std::string(node.text().as_string());
    }
    return "";
}

std::string CmmeInput::ChildAsString(const pugi::xml_node node, const std::string &child) const
{
    if (!node) return "";

    pugi::xpath_node childNode = node.select_node(child.c_str());
    if (childNode.node()) {
        return this->AsString(childNode.node());
    }
    return "";
}

int CmmeInput::AsInt(const pugi::xml_node node) const
{
    if (!node) return VRV_UNSET;

    if (node.text()) {
        return (node.text().as_int());
    }
    return VRV_UNSET;
}

int CmmeInput::ChildAsInt(const pugi::xml_node node, const std::string &child) const
{
    if (!node) return VRV_UNSET;

    pugi::xpath_node childNode = node.select_node(child.c_str());
    if (childNode.node()) {
        return this->AsInt(childNode.node());
    }
    return VRV_UNSET;
}

} // namespace vrv
