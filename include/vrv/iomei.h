/////////////////////////////////////////////////////////////////////////////
// Name:        iomei.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_IOMEI_H__
#define __VRV_IOMEI_H__

#include <sstream>

//----------------------------------------------------------------------------

#include "doc.h"
#include "io.h"

//----------------------------------------------------------------------------

#include "pugixml.hpp"

namespace vrv {

class Abbr;
class Accid;
class Add;
class AnchoredText;
class Annot;
class App;
class AreaPosInterface;
class Arpeg;
class Artic;
class BarLine;
class Beam;
class BeatRpt;
class BoundaryEnd;
class BracketSpan;
class Breath;
class BTrem;
class Choice;
class Chord;
class Clef;
class ControlElement;
class Corr;
class Custos;
class Damage;
class Del;
class Dot;
class Dir;
class DurationInterface;
class Dynam;
class Ending;
class Expan;
class Expansion;
class F;
class Fb;
class Fig;
class Fing;
class Fermata;
class FloatingElement;
class FTrem;
class Gliss;
class GraceGrp;
class GrpSym;
class Hairpin;
class HalfmRpt;
class Harm;
class KeyAccid;
class KeySig;
class LinkingInterface;
class InstrDef;
class Label;
class LabelAbbr;
class Layer;
class LayerElement;
class Lb;
class Lem;
class Ligature;
class Mdiv;
class Measure;
class Mensur;
class MeterSig;
class MNum;
class Mordent;
class MRest;
class MRpt;
class MRpt2;
class MSpace;
class MultiRest;
class MultiRpt;
class Nc;
class Neume;
class Note;
class Num;
class Octave;
class Orig;
class Page;
class Pages;
class Pb;
class Pedal;
class PgFoot;
class PgFoot2;
class PgHead;
class PgHead2;
class Phrase;
class PitchInterface;
class Plica;
class PlistInterface;
class PositionInterface;
class Proport;
class Rdg;
class Ref;
class Reg;
class Reh;
class Rend;
class Rest;
class Restore;
class RunningElement;
class Score;
class ScoreDef;
class ScoreDefElement;
class ScoreDefInterface;
class Sb;
class Section;
class Sic;
class Slur;
class Space;
class Staff;
class Subst;
class Supplied;
class Surface;
class Svg;
class Syl;
class Syllable;
class System;
class SystemElement;
class Tempo;
class Text;
class TextDirInterface;
class TextElement;
class Tie;
class TimePointInterface;
class TimeSpanningInterface;
class Trill;
class Turn;
class Tuplet;
class Unclear;
class Verse;
class Zone;

//----------------------------------------------------------------------------
// MEIOutput
//----------------------------------------------------------------------------

/**
 * This class is a file output stream for writing MEI files.
 * It uses the libmei C++ library.
 * Not implemented.
 */
class MEIOutput : public Output {
public:
    /** @name Constructors and destructor */
    ///@{
    MEIOutput(Doc *doc);
    virtual ~MEIOutput();
    ///@}

    /**
     * The main method for exporting the file to MEI.
     */
    bool Export();

    /**
     * The main method for write objects.
     */
    virtual bool WriteObject(Object *object);

    /**
     * Writing object method that must be overridden in the child class.
     */
    virtual bool WriteObjectEnd(Object *object);

    /**
     * Return the output as a string by writing it to the stringstream member.
     */
    std::string GetOutput(int page = -1);

    /**
     * Setter for score-based MEI output
     */
    void SetScoreBasedMEI(bool scoreBasedMEI) { m_scoreBasedMEI = scoreBasedMEI; }

    /**
     * Setter for indent for the MEI output (default is 3, -1 for tabs)
     */
    void SetIndent(int indent) { m_indent = indent; }

    /**
     * Setter for remove Ids flag for the MEI output (default is false)
     */
    void SetRemoveIds(bool removeIds) { m_removeIds = removeIds; }

private:
    bool WriteDoc(Doc *doc);

    /**
     * Write the @xml:id to the currentNode
     */
    void WriteXmlId(pugi::xml_node currentNode, Object *object);

    /**
     * @name Methods for writing MEI body elements.
     */
    ///@{
    void WriteMdiv(pugi::xml_node currentNode, Mdiv *mdiv);
    void WritePages(pugi::xml_node currentNode, Pages *pages);
    void WriteScore(pugi::xml_node currentNode, Score *score);
    ///@}

    /**
     * @name Methods for writing MEI score-based elements
     */
    ///@{
    void WriteSection(pugi::xml_node currentNode, Section *section);
    void WriteEnding(pugi::xml_node currentNote, Ending *ending);
    void WriteExpansion(pugi::xml_node currentNote, Expansion *expansion);
    void WritePb(pugi::xml_node currentNode, Pb *pb);
    void WriteSb(pugi::xml_node currentNode, Sb *sb);
    ///@}

    /**
     * @name Methods for writing MEI containers (measures, staff, etc) scoreDef and related.
     */
    ///@{
    void WritePage(pugi::xml_node currentNode, Page *page);
    void WriteSystem(pugi::xml_node currentNode, System *system);
    void WriteBoundaryEnd(pugi::xml_node currentNode, BoundaryEnd *boundaryEnd);
    void WriteScoreDef(pugi::xml_node currentNode, ScoreDef *scoreDef);
    void WriteGrpSym(pugi::xml_node currentNode, GrpSym *grmSym);
    void WritePgFoot(pugi::xml_node currentNode, PgFoot *pgFoot);
    void WritePgFoot2(pugi::xml_node currentNode, PgFoot2 *pgFoot2);
    void WritePgHead(pugi::xml_node currentNode, PgHead *pgHead);
    void WritePgHead2(pugi::xml_node currentNode, PgHead2 *pgHead2);
    void WriteStaffGrp(pugi::xml_node currentNode, StaffGrp *staffGrp);
    void WriteStaffDef(pugi::xml_node currentNode, StaffDef *staffDef);
    void WriteInstrDef(pugi::xml_node currentNode, InstrDef *instrDef);
    void WriteLabel(pugi::xml_node currentNode, Label *label);
    void WriteLabelAbbr(pugi::xml_node currentNode, LabelAbbr *labelAbbr);
    void WriteMeasure(pugi::xml_node currentNode, Measure *measure);
    void WriteFb(pugi::xml_node currentNode, Fb *fb);
    void WriteStaff(pugi::xml_node currentNode, Staff *staff);
    void WriteLayer(pugi::xml_node currentNode, Layer *layer);
    ///@}

    /**
     * @name Methods for writing LayerElement children.
     * Called from WriteLayerElement.
     */
    ///@{
    void WriteAccid(pugi::xml_node currentNode, Accid *accid);
    void WriteArtic(pugi::xml_node currentNode, Artic *artic);
    void WriteBarLine(pugi::xml_node currentNode, BarLine *barLine);
    void WriteBeam(pugi::xml_node currentNode, Beam *beam);
    void WriteBeatRpt(pugi::xml_node currentNode, BeatRpt *beatRpt);
    void WriteBTrem(pugi::xml_node currentNode, BTrem *bTrem);
    void WriteChord(pugi::xml_node currentNode, Chord *chord);
    void WriteClef(pugi::xml_node currentNode, Clef *clef);
    void WriteCustos(pugi::xml_node currentNode, Custos *custos);
    void WriteDot(pugi::xml_node currentNode, Dot *dot);
    void WriteFTrem(pugi::xml_node currentNode, FTrem *fTrem);
    void WriteGraceGrp(pugi::xml_node currentNode, GraceGrp *graceGrp);
    void WriteHalfmRpt(pugi::xml_node currentNode, HalfmRpt *halfmRpt);
    void WriteKeyAccid(pugi::xml_node currentNode, KeyAccid *keyAccid);
    void WriteKeySig(pugi::xml_node currentNode, KeySig *keySig);
    void WriteLigature(pugi::xml_node currentNode, Ligature *ligature);
    void WriteMensur(pugi::xml_node currentNode, Mensur *mensur);
    void WriteMeterSig(pugi::xml_node currentNode, MeterSig *meterSig);
    void WriteMRest(pugi::xml_node currentNode, MRest *mRest);
    void WriteMRpt(pugi::xml_node currentNode, MRpt *mRpt);
    void WriteMRpt2(pugi::xml_node currentNode, MRpt2 *mRpt2);
    void WriteMSpace(pugi::xml_node currentNode, MSpace *mSpace);
    void WriteMultiRest(pugi::xml_node currentNode, MultiRest *multiRest);
    void WriteMultiRpt(pugi::xml_node currentNode, MultiRpt *multiRpt);
    void WriteNc(pugi::xml_node currentNode, Nc *nc);
    void WriteNeume(pugi::xml_node currentNode, Neume *neume);
    void WriteNote(pugi::xml_node currentNode, Note *note);
    void WritePlica(pugi::xml_node currentNode, Plica *plica);
    void WriteProport(pugi::xml_node currentNode, Proport *proport);
    void WriteRest(pugi::xml_node currentNode, Rest *rest);
    void WriteSpace(pugi::xml_node currentNode, Space *space);
    void WriteSyllable(pugi::xml_node currentNode, Syllable *syllable);
    void WriteTuplet(pugi::xml_node currentNode, Tuplet *tuplet);
    ///@}

    /**
     * @name Methods for writing ControlElement
     */
    ///@{
    void WriteAnchoredText(pugi::xml_node currentNode, AnchoredText *anchoredText);
    void WriteArpeg(pugi::xml_node currentNode, Arpeg *arpeg);
    void WriteBracketSpan(pugi::xml_node currentNode, BracketSpan *bracketSpan);
    void WriteBreath(pugi::xml_node currentNode, Breath *breath);
    void WriteDir(pugi::xml_node currentNode, Dir *dir);
    void WriteDynam(pugi::xml_node currentNode, Dynam *dynam);
    void WriteFermata(pugi::xml_node currentNode, Fermata *fermata);
    void WriteFing(pugi::xml_node currentNode, Fing *fing);
    void WriteGliss(pugi::xml_node currentNode, Gliss *gliss);
    void WriteHairpin(pugi::xml_node currentNode, Hairpin *hairpin);
    void WriteHarm(pugi::xml_node currentNode, Harm *harm);
    void WriteMNum(pugi::xml_node currentNode, MNum *mnum);
    void WriteMordent(pugi::xml_node currentNode, Mordent *mordent);
    void WriteOctave(pugi::xml_node currentNode, Octave *octave);
    void WritePedal(pugi::xml_node currentNode, Pedal *pedal);
    void WritePhrase(pugi::xml_node currentNode, Phrase *phrase);
    void WriteReh(pugi::xml_node currentNode, Reh *reh);
    void WriteSlur(pugi::xml_node currentNode, Slur *slur);
    void WriteTempo(pugi::xml_node currentNode, Tempo *tempo);
    void WriteTie(pugi::xml_node currentNode, Tie *tie);
    void WriteTrill(pugi::xml_node currentNode, Trill *trill);
    void WriteTurn(pugi::xml_node currentNode, Turn *turn);
    ///@}

    /**
     * @name Methods for writing text and figure elements
     */
    ///@{
    void WriteF(pugi::xml_node currentNode, F *f);
    void WriteFig(pugi::xml_node currentNode, Fig *fig);
    void WriteLb(pugi::xml_node currentNode, Lb *lb);
    void WriteNum(pugi::xml_node currentNode, Num *num);
    void WriteRend(pugi::xml_node currentNode, Rend *rend);
    void WriteSvg(pugi::xml_node currentNode, Svg *svg);
    void WriteText(pugi::xml_node currentNode, Text *text);
    ///@}

    /**
     * @name Methods for writing editorial markup
     */
    ///@{
    void WriteAbbr(pugi::xml_node currentNode, Abbr *abbr);
    void WriteAdd(pugi::xml_node currentNode, Add *add);
    void WriteAnnot(pugi::xml_node currentNode, Annot *annot);
    void WriteApp(pugi::xml_node currentNode, App *app);
    void WriteChoice(pugi::xml_node currentNode, Choice *choice);
    void WriteCorr(pugi::xml_node currentNode, Corr *corr);
    void WriteDamage(pugi::xml_node currentNode, Damage *damage);
    void WriteDel(pugi::xml_node currentNode, Del *del);
    void WriteExpan(pugi::xml_node currentNode, Expan *expan);
    void WriteLem(pugi::xml_node currentNode, Lem *lem);
    void WriteOrig(pugi::xml_node currentNode, Orig *orig);
    void WriteRdg(pugi::xml_node currentNode, Rdg *rdg);
    void WriteRef(pugi::xml_node currentNode, Ref *Ref);
    void WriteReg(pugi::xml_node currentNode, Reg *Reg);
    void WriteRestore(pugi::xml_node currentNode, Restore *restore);
    void WriteSic(pugi::xml_node currentNode, Sic *sic);
    void WriteSubst(pugi::xml_node currentNode, Subst *subst);
    void WriteSupplied(pugi::xml_node currentNode, Supplied *supplied);
    void WriteUnclear(pugi::xml_node currentNode, Unclear *unclear);
    ///@}

    /**
     * @name Methods for writing other mei elements
     */
    ///@{
    void WriteVerse(pugi::xml_node currentNode, Verse *verse);
    void WriteSyl(pugi::xml_node currentNode, Syl *syl);
    void WriteZone(pugi::xml_node currentNode, Zone *zone);
    void WriteSurface(pugi::xml_node currentNode, Surface *surface);
    void WriteFacsimile(pugi::xml_node currentNode, Facsimile *facsimile);
    ///@}

    /**
     * Write unsupported attributes stored in Object::m_unsupported (not tested)
     */
    void WriteUnsupportedAttr(pugi::xml_node currentNode, Object *object);

    /**
     * @name Methods for writing LayerElement, EditorialElement and interfaces.
     * Call WriteDurationInferface from WriteNote, for example.
     */
    ///@{
    void WriteControlElement(pugi::xml_node element, ControlElement *object);
    void WriteEditorialElement(pugi::xml_node element, EditorialElement *object);
    void WriteLayerElement(pugi::xml_node element, LayerElement *object);
    void WriteRunningElement(pugi::xml_node element, RunningElement *object);
    void WriteScoreDefElement(pugi::xml_node element, ScoreDefElement *object);
    void WriteSystemElement(pugi::xml_node element, SystemElement *object);
    void WriteTextElement(pugi::xml_node element, TextElement *object);
    //
    void WriteAreaPosInterface(pugi::xml_node currentNode, AreaPosInterface *interface);
    void WriteDurationInterface(pugi::xml_node currentNode, DurationInterface *interface);
    void WriteLinkingInterface(pugi::xml_node currentNode, LinkingInterface *interface);
    void WriteFacsimileInterface(pugi::xml_node element, FacsimileInterface *interface);
    void WritePitchInterface(pugi::xml_node currentNode, PitchInterface *interface);
    void WritePlistInterface(pugi::xml_node currentNode, PlistInterface *interface);
    void WritePositionInterface(pugi::xml_node currentNode, PositionInterface *interface);
    void WriteScoreDefInterface(pugi::xml_node currentNode, ScoreDefInterface *interface);
    void WriteTextDirInterface(pugi::xml_node currentNode, TextDirInterface *interface);
    void WriteTimePointInterface(pugi::xml_node currentNode, TimePointInterface *interface);
    void WriteTimeSpanningInterface(pugi::xml_node currentNode, TimeSpanningInterface *interface);
    ///@}

    /**
     * Escapes SMuFL characters to entities (e.g., &#xe1e7;).
     * Must be used in conjunction with (pugi::format_default | pugi::format_no_escapes).
     * Unused for now (see WriteText) because of un-escaped entities in the header.
     */
    std::wstring EscapeSMuFL(std::wstring data);

    /** @name Methods for converting members into MEI attributes. */
    ///@{
    std::string UuidToMeiStr(Object *element);
    std::string DocTypeToStr(DocType type);
    ///@}

public:
    //
private:
    std::ostringstream m_streamStringOutput;
    int m_indent;
    int m_page;
    bool m_scoreBasedMEI;
    pugi::xml_node m_mei;
    /** @name Current element */
    pugi::xml_node m_currentNode;
    std::list<pugi::xml_node> m_nodeStack;
    bool m_removeIds;
    ListOfObjects m_referredObjects;
};

//----------------------------------------------------------------------------
// MEIInput
//----------------------------------------------------------------------------

/**
 * This class is a file input stream for reading MEI files.
 * It uses the libmei C++ library.
 * Under development.
 */
class MEIInput : public Input {
public:
    // constructors and destructors
    MEIInput(Doc *doc);
    virtual ~MEIInput();

    virtual bool Import(const std::string &mei);

private:
    bool ReadDoc(pugi::xml_node root);

    ///@{
    bool ReadMdiv(Object *parent, pugi::xml_node parentNode, bool isVisible);
    bool ReadMdivChildren(Object *parent, pugi::xml_node parentNode, bool isVisible);
    bool ReadPages(Object *parent, pugi::xml_node parentNode);
    bool ReadScore(Object *parent, pugi::xml_node parentNode);
    ///@}

    /**
     * @name Methods for reading MEI score-based elements
     */
    ///@{
    bool ReadSection(Object *parent, pugi::xml_node section);
    bool ReadSectionChildren(Object *parent, pugi::xml_node parentNode);
    bool ReadEnding(Object *parent, pugi::xml_node ending);
    bool ReadExpansion(Object *parent, pugi::xml_node expansion);
    bool ReadPb(Object *parent, pugi::xml_node pb);
    bool ReadSb(Object *parent, pugi::xml_node sb);
    ///@}

    /**
     * @name Methods for reading  MEI containers (measures, staff, etc) scoreDef and related.
     * For each container (page, system, measure, staff and layer) there is one method for
     * reading the element and one method for reading its children. The method for reading
     * the children can also be called when reading EditorialElement objects (<lem> or <rdg>
     * for example. The filter object is optional and can be set for filtering the allowed
     * children (see MEIInput::IsAllowed)
     */
    ///@{
    bool ReadPage(Object *parent, pugi::xml_node parentNode);
    bool ReadPageChildren(Object *parent, pugi::xml_node parentNode);
    bool ReadSystem(Object *parent, pugi::xml_node system);
    bool ReadSystemChildren(Object *parent, pugi::xml_node parentNode);
    bool ReadBoundaryEnd(Object *parent, pugi::xml_node boundaryEnd);
    bool ReadScoreDef(Object *parent, pugi::xml_node scoreDef);
    bool ReadScoreDefChildren(Object *parent, pugi::xml_node parentNode);
    bool ReadGrpSym(Object *parent, pugi::xml_node grpSym);
    bool ReadPgFoot(Object *parent, pugi::xml_node pgFoot);
    bool ReadPgFoot2(Object *parent, pugi::xml_node pgFoot2);
    bool ReadPgHead(Object *parent, pugi::xml_node pgHead);
    bool ReadPgHead2(Object *parent, pugi::xml_node pgHead2);
    bool ReadRunningChildren(Object *parent, pugi::xml_node parentNode, Object *filter = NULL);
    bool ReadStaffGrp(Object *parent, pugi::xml_node staffGrp);
    bool ReadStaffGrpChildren(Object *parent, pugi::xml_node parentNode);
    bool ReadStaffDef(Object *parent, pugi::xml_node staffDef);
    bool ReadStaffDefChildren(Object *parent, pugi::xml_node parentNode);
    bool ReadInstrDef(Object *parent, pugi::xml_node instrDef);
    bool ReadLabel(Object *parent, pugi::xml_node label);
    bool ReadLabelAbbr(Object *parent, pugi::xml_node labelAbbr);
    bool ReadMeasure(Object *parent, pugi::xml_node measure);
    bool ReadMeasureChildren(Object *parent, pugi::xml_node parentNode);
    bool ReadFb(Object *parent, pugi::xml_node fb);
    bool ReadFbChildren(Object *parent, pugi::xml_node parentNode);
    bool ReadNum(Object *parent, pugi::xml_node parentNode);
    bool ReadStaff(Object *parent, pugi::xml_node staff);
    bool ReadStaffChildren(Object *parent, pugi::xml_node parentNode);
    bool ReadLayer(Object *parent, pugi::xml_node layer);
    bool ReadLayerChildren(Object *parent, pugi::xml_node parentNode, Object *filter = NULL);
    bool ReadTextChildren(Object *parent, pugi::xml_node parentNode, Object *filter = NULL);
    ///@}

    /**
     * @name Methods for reading MEI layer elements
     */
    ///@{
    bool ReadAccid(Object *parent, pugi::xml_node accid);
    bool ReadArtic(Object *parent, pugi::xml_node artic);
    bool ReadBarLine(Object *parent, pugi::xml_node barLine);
    bool ReadBeam(Object *parent, pugi::xml_node beam);
    bool ReadBeatRpt(Object *parent, pugi::xml_node beatRpt);
    bool ReadBTrem(Object *parent, pugi::xml_node bTrem);
    bool ReadChord(Object *parent, pugi::xml_node chord);
    bool ReadClef(Object *parent, pugi::xml_node clef);
    bool ReadCustos(Object *parent, pugi::xml_node custos);
    bool ReadDot(Object *parent, pugi::xml_node dot);
    bool ReadFTrem(Object *parent, pugi::xml_node fTrem);
    bool ReadGraceGrp(Object *parent, pugi::xml_node graceGrp);
    bool ReadHalfmRpt(Object *parent, pugi::xml_node halfmRpt);
    bool ReadKeyAccid(Object *parent, pugi::xml_node keyAccid);
    bool ReadKeySig(Object *parent, pugi::xml_node keySig);
    bool ReadLigature(Object *parent, pugi::xml_node ligature);
    bool ReadMensur(Object *parent, pugi::xml_node mensur);
    bool ReadMeterSig(Object *parent, pugi::xml_node meterSig);
    bool ReadMRest(Object *parent, pugi::xml_node mRest);
    bool ReadMRpt(Object *parent, pugi::xml_node mRpt);
    bool ReadMRpt2(Object *parent, pugi::xml_node mRpt2);
    bool ReadMSpace(Object *parent, pugi::xml_node mSpace);
    bool ReadMultiRest(Object *parent, pugi::xml_node multiRest);
    bool ReadMultiRpt(Object *parent, pugi::xml_node multiRpt);
    bool ReadNc(Object *parent, pugi::xml_node nc);
    bool ReadNeume(Object *parent, pugi::xml_node note);
    bool ReadNote(Object *parent, pugi::xml_node note);
    bool ReadPlica(Object *parent, pugi::xml_node plica);
    bool ReadProport(Object *parent, pugi::xml_node proport);
    bool ReadRest(Object *parent, pugi::xml_node rest);
    bool ReadSpace(Object *parent, pugi::xml_node space);
    bool ReadSyl(Object *parent, pugi::xml_node syl);
    bool ReadSyllable(Object *parent, pugi::xml_node syllable);
    bool ReadTuplet(Object *parent, pugi::xml_node tuplet);
    bool ReadVerse(Object *parent, pugi::xml_node verse);
    ///@}

    /**
     * @name Methods for reading MEI control elements
     */
    ///@{
    bool ReadAnchoredText(Object *parent, pugi::xml_node anchoredText);
    bool ReadArpeg(Object *parent, pugi::xml_node arpeg);
    bool ReadBracketSpan(Object *parent, pugi::xml_node bracketSpan);
    bool ReadBreath(Object *parent, pugi::xml_node breath);
    bool ReadDir(Object *parent, pugi::xml_node dir);
    bool ReadDynam(Object *parent, pugi::xml_node dynam);
    bool ReadFermata(Object *parent, pugi::xml_node fermata);
    bool ReadFing(Object *parent, pugi::xml_node fing);
    bool ReadGliss(Object *parent, pugi::xml_node gliss);
    bool ReadHairpin(Object *parent, pugi::xml_node hairpin);
    bool ReadHarm(Object *parent, pugi::xml_node harm);
    bool ReadMNum(Object *parent, pugi::xml_node mnum);
    bool ReadMordent(Object *parent, pugi::xml_node mordent);
    bool ReadOctave(Object *parent, pugi::xml_node octave);
    bool ReadPedal(Object *parent, pugi::xml_node pedal);
    bool ReadPhrase(Object *parent, pugi::xml_node phrase);
    bool ReadReh(Object *parent, pugi::xml_node reh);
    bool ReadSlur(Object *parent, pugi::xml_node slur);
    bool ReadTempo(Object *parent, pugi::xml_node tempo);
    bool ReadTie(Object *parent, pugi::xml_node tie);
    bool ReadTrill(Object *parent, pugi::xml_node trill);
    bool ReadTurn(Object *parent, pugi::xml_node turn);
    ///@}

    /**
     * @name Methods for reading MEI text and figure elements
     */
    ///@{
    bool ReadF(Object *parent, pugi::xml_node f);
    bool ReadFig(Object *parent, pugi::xml_node fig);
    bool ReadLb(Object *parent, pugi::xml_node lb);
    bool ReadRend(Object *parent, pugi::xml_node rend);
    bool ReadSvg(Object *parent, pugi::xml_node svg);
    bool ReadText(Object *parent, pugi::xml_node text, bool trimLeft, bool trimRight);
    ///@}

    /**
     * @name Methods for reading critical apparatus, other editorial markup and <annot>.
     * The filter is propagated (if any)
     */
    ///@{
    bool ReadEditorialElement(Object *parent, pugi::xml_node app, EditorialLevel level, Object *filter = NULL);
    bool ReadAbbr(Object *parent, pugi::xml_node abbr, EditorialLevel level, Object *filter = NULL);
    bool ReadAdd(Object *parent, pugi::xml_node add, EditorialLevel level, Object *filter = NULL);
    bool ReadAnnot(Object *parent, pugi::xml_node annot);
    bool ReadApp(Object *parent, pugi::xml_node app, EditorialLevel level, Object *filter = NULL);
    bool ReadAppChildren(Object *parent, pugi::xml_node parentNode, EditorialLevel level, Object *filter = NULL);
    bool ReadChoice(Object *parent, pugi::xml_node choice, EditorialLevel level, Object *filter = NULL);
    bool ReadChoiceChildren(Object *parent, pugi::xml_node parentNode, EditorialLevel level, Object *filter = NULL);
    bool ReadCorr(Object *parent, pugi::xml_node corr, EditorialLevel level, Object *filter = NULL);
    bool ReadDamage(Object *parent, pugi::xml_node damage, EditorialLevel level, Object *filter = NULL);
    bool ReadDel(Object *parent, pugi::xml_node del, EditorialLevel level, Object *filter = NULL);
    bool ReadExpan(Object *parent, pugi::xml_node expan, EditorialLevel level, Object *filter = NULL);
    bool ReadLem(Object *parent, pugi::xml_node lem, EditorialLevel level, Object *filter = NULL);
    bool ReadOrig(Object *parent, pugi::xml_node orig, EditorialLevel level, Object *filter = NULL);
    bool ReadRdg(Object *parent, pugi::xml_node rdg, EditorialLevel level, Object *filter = NULL);
    bool ReadRef(Object *parent, pugi::xml_node ref, EditorialLevel level, Object *filter = NULL);
    bool ReadReg(Object *parent, pugi::xml_node reg, EditorialLevel level, Object *filter = NULL);
    bool ReadRestore(Object *parent, pugi::xml_node restore, EditorialLevel level, Object *filter = NULL);
    bool ReadSic(Object *parent, pugi::xml_node sic, EditorialLevel level, Object *filter = NULL);
    bool ReadSubst(Object *parent, pugi::xml_node subst, EditorialLevel level, Object *filter = NULL);
    bool ReadSubstChildren(Object *parent, pugi::xml_node parentNode, EditorialLevel level, Object *filter = NULL);
    bool ReadSupplied(Object *parent, pugi::xml_node supplied, EditorialLevel level, Object *filter = NULL);
    bool ReadUnclear(Object *parent, pugi::xml_node unclear, EditorialLevel level, Object *filter = NULL);
    bool ReadEditorialChildren(Object *parent, pugi::xml_node supplied, EditorialLevel level, Object *filter = NULL);
    ///@}

    /**
     * @name Methods for reading MEI LayerElement, EidtorialElement and interfaces
     */
    ///@{
    bool ReadControlElement(pugi::xml_node element, ControlElement *object);
    bool ReadEditorialElement(pugi::xml_node element, EditorialElement *object);
    bool ReadLayerElement(pugi::xml_node element, LayerElement *object);
    bool ReadRunningElement(pugi::xml_node element, RunningElement *object);
    bool ReadScoreDefElement(pugi::xml_node element, ScoreDefElement *object);
    bool ReadSystemElement(pugi::xml_node element, SystemElement *object);
    bool ReadTextElement(pugi::xml_node element, TextElement *object);

    bool ReadAreaPosInterface(pugi::xml_node element, AreaPosInterface *interface);
    bool ReadDurationInterface(pugi::xml_node element, DurationInterface *interface);
    bool ReadLinkingInterface(pugi::xml_node element, LinkingInterface *interface);
    bool ReadFacsimileInterface(pugi::xml_node element, FacsimileInterface *interface);
    bool ReadPitchInterface(pugi::xml_node element, PitchInterface *interface);
    bool ReadPlistInterface(pugi::xml_node element, PlistInterface *interface);
    bool ReadPositionInterface(pugi::xml_node element, PositionInterface *interface);
    bool ReadScoreDefInterface(pugi::xml_node element, ScoreDefInterface *interface);
    bool ReadTextDirInterface(pugi::xml_node element, TextDirInterface *interface);
    bool ReadTimePointInterface(pugi::xml_node element, TimePointInterface *interface);
    bool ReadTimeSpanningInterface(pugi::xml_node element, TimeSpanningInterface *interface);
    ///@}

    /**
     * @name Methods for reading other MEI elements.
     */
    ///@{
    bool ReadFacsimile(Doc *doc, pugi::xml_node facsimile);
    bool ReadSurface(Facsimile *parent, pugi::xml_node surface);
    bool ReadBeamSpanAsBeam(Measure *measure, pugi::xml_node beamSpan);
    bool ReadTupletSpanAsTuplet(Measure *measure, pugi::xml_node tupletSpan);
    bool ReadZone(Surface *parent, pugi::xml_node zone);
    ///@}

    /**
     * Write unsupported attributes and store them in Object::m_unsupported (not tested)
     */
    void ReadUnsupportedAttr(pugi::xml_node element, Object *object);

    /**
     * Returns true if the element is name is an editorial element (e.g., "app", "supplied", etc.)
     */
    bool IsEditorialElementName(std::string elementName);

    /**
     * Read score-based MEI.
     * The data is read into an object, which is then converted to page-based MEI.
     * See MEIInput::ReadDoc, Doc::CreateScoreBuffer and Doc::ConvertToPageBasedDoc
     */
    bool ReadScoreBasedMei(pugi::xml_node element, Score *parent);

    /**
     * @name Various methods for reading / converting values.
     */
    ///@{
    void SetMeiUuid(pugi::xml_node element, Object *object);
    DocType StrToDocType(std::string type);
    std::wstring LeftTrim(std::wstring str);
    std::wstring RightTrim(std::wstring str);
    bool ReadXMLComment(Object *object, pugi::xml_node element);
    ///@}

    /**
     * @name Various methods for upgrading older MEI files
     */
    ///@{
    // to MEI 4.0.0
    void UpgradeBeatRptTo_4_0_0(pugi::xml_node beatRpt, BeatRpt *vrvBeatRpt);
    void UpgradeDurGesTo_4_0_0(pugi::xml_node element, DurationInterface *interface);
    void UpgradeFTremTo_4_0_0(pugi::xml_node fTrem, FTrem *vrvFTrem);
    void UpgradeMensurTo_5_0_0(pugi::xml_node mensur, Mensur *vrvMensur);
    void UpgradeMordentTo_4_0_0(pugi::xml_node mordent, Mordent *vrvMordent);
    void UpgradeScoreDefElementTo_4_0_0(pugi::xml_node scoreDefElement, ScoreDefElement *vrvScoreDefElement);
    void UpgradeStaffDefTo_4_0_0(pugi::xml_node staffDef, StaffDef *vrvStaffDef);
    void UpgradeStaffGrpTo_4_0_0(pugi::xml_node staffGrp, StaffGrp *vrvStaffGrp);
    void UpgradeTurnTo_4_0_0(pugi::xml_node turn, Turn *vrvTurn);
    // to MEI 3.0.0 (Page-Based MEI only)
    void UpgradeMeasureTo_3_0_0(Measure *measure, System *system);
    void UpgradePageTo_3_0_0(Page *page, Doc *doc);
    ///@}

public:
    //
private:
    /**
     * The full filename of the file being read
     */
    std::string m_filename;

    /**
     * The version of the file being read
     */
    MEIVersion m_version;

    /**
     * A flag indicating wheather we are reading page-based or score-based MEI
     */
    bool m_readingScoreBased;

    /**
     * This is used when reading a standard MEI file to specify if a scoreDef has already been read or not.
     */
    bool m_hasScoreDef;

    /**
     * Check if an element is allowed within a given parent
     */
    bool IsAllowed(std::string element, Object *filterParent);

    /**
     * A static array for storing the implemented editorial elements
     */
    static std::vector<std::string> s_editorialElementNames;

    /**
     * The selected <mdiv>.
     * If not specified by --mdiv-x-path query, then it is the first <mdiv> in the body
     */
    pugi::xml_node m_selectedMdiv;

    /**
     * A flag indicating if the first scoreDef has to be used as Doc scoreDef.
     * This is not the case when selecting a mDiv that is not the first one with a score in the tree.
     */
    bool m_useScoreDefForDoc;

    /**
     * The comment to be attached to the next Object
     */
    std::string m_comment;
};

} // namespace vrv

#endif
