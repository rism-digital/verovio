/////////////////////////////////////////////////////////////////////////////
// Name:        layerelement.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "accid.h"
#include "attcomparison.h"
#include "barline.h"
#include "beam.h"
#include "chord.h"
#include "clef.h"
#include "custos.h"
#include "doc.h"
#include "dot.h"
#include "functorparams.h"
#include "horizontalaligner.h"
#include "keysig.h"
#include "layer.h"
#include "measure.h"
#include "mensur.h"
#include "metersig.h"
#include "mrest.h"
#include "multirest.h"
#include "note.h"
#include "rest.h"
#include "rpt.h"
#include "space.h"
#include "staff.h"
#include "syl.h"
#include "tie.h"
#include "timeinterface.h"
#include "timestamp.h"
#include "tuplet.h"
#include "verse.h"
#include "view.h"
#include "vrv.h"

//----------------------------------------------------------------------------

#include "MidiFile.h"

namespace vrv {

//----------------------------------------------------------------------------
// LayerElement
//----------------------------------------------------------------------------

LayerElement::LayerElement() : Object("le-"), AttCommon(), AttTyped()
{
    RegisterAttClass(ATT_COMMON);
    RegisterAttClass(ATT_TYPED);

    Reset();
}

LayerElement::LayerElement(std::string classid) : Object(classid), AttCommon(), AttTyped()
{
    RegisterAttClass(ATT_COMMON);
    RegisterAttClass(ATT_TYPED);

    Reset();
}

void LayerElement::Reset()
{
    Object::Reset();
    ResetCommon();
    ResetTyped();

    m_xAbs = VRV_UNSET;
    m_drawingYRel = 0;
    m_drawingXRel = 0;
    m_drawingCueSize = false;

    m_scoreDefRole = NONE;
    m_alignment = NULL;
    m_graceAlignment = NULL;
    m_alignmentLayerN = VRV_UNSET;
    m_beamElementCoord = NULL;

    m_crossStaff = NULL;
    m_crossLayer = NULL;
}

LayerElement::~LayerElement()
{
    // set the pointer of the m_beamElementCoord to NULL;
    if (m_beamElementCoord) {
        m_beamElementCoord->m_element = NULL;
    }
}

LayerElement &LayerElement::operator=(const LayerElement &element)
{
    // not self assignement
    if (this != &element) {
        // pointers have to be NULL
        ResetParent();
        m_alignment = NULL;
        m_graceAlignment = NULL;
        m_beamElementCoord = NULL;
    }
    return *this;
}

bool LayerElement::IsGraceNote()
{
    // For note, we need to look it or at the parent chord
    if (this->Is(NOTE)) {
        Note const *note = dynamic_cast<Note const *>(this);
        assert(note);
        Chord *chord = note->IsChordTone();
        if (chord)
            return chord->HasGrace();
        else
            return (note->HasGrace());
    }
    else if (this->Is(CHORD)) {
        Chord const *chord = dynamic_cast<Chord const *>(this);
        assert(chord);
        return (chord->HasGrace());
    }
    else if (this->Is(TUPLET)) {
        AttComparisonAny matchType({ NOTE, CHORD });
        ArrayOfObjects children;
        LayerElement *child = dynamic_cast<LayerElement *>(this->FindChildByAttComparison(&matchType));
        if (child) return child->IsGraceNote();
    }
    // For accid, artic, etc.. look at the parent note / chord
    else {
        // For an accid we expect to be the child of a note - the note will lookup at the chord parent in necessary
        Note *note = dynamic_cast<Note *>(this->GetFirstParent(NOTE, MAX_ACCID_DEPTH));
        if (note) return note->IsGraceNote();
        // For an artic we can be direct child of a chord
        Chord *chord = dynamic_cast<Chord *>(this->GetFirstParent(CHORD, MAX_ACCID_DEPTH));
        if (chord) return chord->IsGraceNote();
    }
    return false;
}

bool LayerElement::IsCueSize()
{
    return m_drawingCueSize;
}

bool LayerElement::IsInLigature()
{
    if (!this->Is(NOTE)) return false;
    return (this->GetFirstParent(LIGATURE, MAX_LIGATURE_DEPTH));
}

bool LayerElement::IsInFTrem()
{
    if ((!this->Is(NOTE)) || (this->Is(CHORD))) return false;
    return (this->GetFirstParent(FTREM, MAX_FTREM_DEPTH));
}

Beam *LayerElement::IsInBeam()
{
    if (!this->Is(NOTE) && !this->Is(CHORD)) return NULL;
    Beam *beamParent = dynamic_cast<Beam *>(this->GetFirstParent(BEAM, MAX_BEAM_DEPTH));
    if (beamParent != NULL) {
        // This note is beamed and cue-sized
        if (this->IsGraceNote()) {
            // If the note is part of the beam parent, this means we
            // have a beam of graced notes
            if (beamParent->GetListIndex(this) > -1) return beamParent;
            // otherwise it is a non-beamed grace note within a beam - will return false
        }
        else {
            return beamParent;
        }
    }
    return NULL;
}

Staff *LayerElement::GetCrossStaff(Layer *&layer) const
{
    if (m_crossStaff) {
        assert(m_crossLayer);
        layer = m_crossLayer;
        return m_crossStaff;
    }

    LayerElement *parent = dynamic_cast<LayerElement *>(this->GetFirstParentInRange(LAYER_ELEMENT, LAYER_ELEMENT_max));

    if (parent) return parent->GetCrossStaff(layer);

    return NULL;
}

Alignment *LayerElement::GetGraceAlignment() const
{
    assert(m_graceAlignment);
    return m_graceAlignment;
}

void LayerElement::SetGraceAlignment(Alignment *graceAlignment)
{
    assert(!m_graceAlignment && graceAlignment);
    m_graceAlignment = graceAlignment;
}

int LayerElement::GetDrawingX() const
{
    if (m_xAbs != VRV_UNSET) return m_xAbs;

    if (m_cachedDrawingX != VRV_UNSET) return m_cachedDrawingX;

    if (!m_alignment) {
        // assert(this->Is({ BEAM, FTREM, TUPLET }));
        // Here we just get the measure position - no cast to Measure is necessary
        Object *measure = this->GetFirstParent(MEASURE);
        assert(measure);
        m_cachedDrawingX = measure->GetDrawingX();
        return m_cachedDrawingX;
    }

    // First get the first layerElement parent (if any) and use its position if they share the same alignment
    LayerElement *parent = dynamic_cast<LayerElement *>(this->GetFirstParentInRange(LAYER_ELEMENT, LAYER_ELEMENT_max));
    if (parent && (parent->GetAlignment() == this->GetAlignment())) {
        m_cachedDrawingX = (parent->GetDrawingX() + this->GetDrawingXRel());
        return m_cachedDrawingX;
    }

    // Otherwise get the measure - no cast to Measure is necessary
    Object *measure = this->GetFirstParent(MEASURE);
    assert(measure);

    int graceNoteShift = 0;
    if (this->HasGraceAlignment()) {
        graceNoteShift = this->GetGraceAlignment()->GetXRel();
        // const Note *note = dynamic_cast<const Note*>(this);
        // LogDebug("Grace Note %d  Shift %d", note->GetPname(), graceNoteShift);
    }

    m_cachedDrawingX = (measure->GetDrawingX() + m_alignment->GetXRel() + this->GetDrawingXRel() + graceNoteShift);
    return m_cachedDrawingX;
}

int LayerElement::GetDrawingY() const
{
    if (m_cachedDrawingY != VRV_UNSET) return m_cachedDrawingY;

    Object *object = NULL;
    // Otherwise look if we have a crossStaff situation
    if (!object) object = this->m_crossStaff; // GetCrossStaff();
    // First get the first layerElement parent (if any) but only if the element is not directly relative to staff (e.g.,
    // artic, syl)
    if (!object && !this->IsRelativeToStaff()) object = this->GetFirstParentInRange(LAYER_ELEMENT, LAYER_ELEMENT_max);
    // Otherwise get the first staff
    if (!object) object = this->GetFirstParent(STAFF);
    // Otherwise the first measure (this is the case with barLineAttr
    if (!object) object = this->GetFirstParent(MEASURE);

    assert(object);

    m_cachedDrawingY = object->GetDrawingY() + this->GetDrawingYRel();
    return m_cachedDrawingY;
}

int LayerElement::GetDrawingArticulationTopOrBottom(data_STAFFREL place, ArticPartType type)
{
    // It would not crash otherwise but there is not reason to call it
    assert(this->Is({ NOTE, CHORD }));

    ArticPart *firstArticPart = NULL;
    ArticPart *lastArticPart = NULL;

    // We limit support to two artic elements, get them by searching in both directions
    Artic *firstArtic = dynamic_cast<Artic *>(this->FindChildByType(ARTIC));
    Artic *lastArtic = dynamic_cast<Artic *>(this->FindChildByType(ARTIC, MAX_ACCID_DEPTH, BACKWARD));
    // If they are the same (we have only one artic child), then ignore the second one
    if (firstArtic == lastArtic) lastArtic = NULL;
    // Look for the outside part first if necessary
    if (type == ARTIC_PART_OUTSIDE) {
        if (firstArtic) firstArticPart = firstArtic->GetOutsidePart();
        if (lastArtic) lastArticPart = lastArtic->GetOutsidePart();
        // Ignore them if on the opposite side of what we are looking for
        if (firstArticPart && (firstArticPart->GetPlace() != place)) firstArticPart = NULL;
        if (lastArticPart && (lastArticPart->GetPlace() != place)) lastArticPart = NULL;
    }
    // Looking at the inside if nothing is given outside
    if (firstArtic && !firstArticPart) {
        firstArticPart = firstArtic->GetInsidePart();
        if (firstArticPart && (firstArticPart->GetPlace() != place)) firstArticPart = NULL;
    }
    if (lastArtic && !lastArticPart) {
        lastArticPart = lastArtic->GetInsidePart();
        if (lastArticPart && (lastArticPart->GetPlace() != place)) lastArticPart = NULL;
    }

    if (place == STAFFREL_above) {
        int firstY = !firstArticPart ? VRV_UNSET : firstArticPart->GetSelfTop();
        int lastY = !lastArticPart ? VRV_UNSET : lastArticPart->GetSelfTop();
        return std::max(firstY, lastY);
    }
    else {
        int firstY = !firstArticPart ? -VRV_UNSET : firstArticPart->GetSelfBottom();
        int lastY = !lastArticPart ? -VRV_UNSET : lastArticPart->GetSelfBottom();
        return std::min(firstY, lastY);
    }
}

void LayerElement::SetDrawingXRel(int drawingXRel)
{
    ResetCachedDrawingX();
    m_drawingXRel = drawingXRel;
}

void LayerElement::SetDrawingYRel(int drawingYRel)
{
    ResetCachedDrawingY();
    m_drawingYRel = drawingYRel;
}

void LayerElement::CenterDrawingX()
{
    if (this->m_xAbs != VRV_UNSET) return;

    SetDrawingXRel(0);

    Measure *measure = dynamic_cast<Measure *>(this->GetFirstParent(MEASURE));
    assert(measure);

    SetDrawingXRel(measure->GetInnerCenterX() - this->GetDrawingX());
}

int LayerElement::GetDrawingTop(Doc *doc, int staffSize, bool withArtic, ArticPartType type)
{
    if (this->Is({ NOTE, CHORD })) {
        if (withArtic) {
            int articY = GetDrawingArticulationTopOrBottom(STAFFREL_above, type);
            if (articY != VRV_UNSET) return articY;
        }
        DurationInterface *durationInterface = this->GetDurationInterface();
        assert(durationInterface);
        if (durationInterface->GetNoteOrChordDur(this) < DUR_2) {
            if (this->Is(CHORD)) {
                int yChordMax = 0, yChordMin = 0;
                Chord *chord = dynamic_cast<Chord *>(this);
                assert(chord);
                chord->GetYExtremes(yChordMax, yChordMin);
                return yChordMax + doc->GetDrawingUnit(staffSize);
            }
            else
                return this->GetDrawingY() + doc->GetDrawingUnit(staffSize);
        }
        // We should also take into accound the stem shift to the right
        StemmedDrawingInterface *stemmedDrawingInterface = this->GetStemmedDrawingInterface();
        assert(stemmedDrawingInterface);
        if (stemmedDrawingInterface->GetDrawingStemDir() == STEMDIRECTION_up) {
            return stemmedDrawingInterface->GetDrawingStemEnd(this).y;
        }
        else {
            return stemmedDrawingInterface->GetDrawingStemStart(this).y + doc->GetDrawingUnit(staffSize);
        }
    }
    return this->GetDrawingY();
}

int LayerElement::GetDrawingBottom(Doc *doc, int staffSize, bool withArtic, ArticPartType type)
{
    if (this->Is({ NOTE, CHORD })) {
        if (withArtic) {
            int articY = GetDrawingArticulationTopOrBottom(STAFFREL_below, type);
            if (articY != -VRV_UNSET) return articY;
        }
        DurationInterface *durationInterface = this->GetDurationInterface();
        assert(durationInterface);
        if (durationInterface->GetNoteOrChordDur(this) < DUR_2) {
            if (this->Is(CHORD)) {
                int yChordMax = 0, yChordMin = 0;
                Chord *chord = dynamic_cast<Chord *>(this);
                assert(chord);
                chord->GetYExtremes(yChordMax, yChordMin);
                return yChordMin - doc->GetDrawingUnit(staffSize);
            }
            else
                return this->GetDrawingY() - doc->GetDrawingUnit(staffSize);
        }
        // We should also take into accound the stem shift to the right
        StemmedDrawingInterface *stemmedDrawingInterface = this->GetStemmedDrawingInterface();
        assert(stemmedDrawingInterface);
        if (stemmedDrawingInterface->GetDrawingStemDir() == STEMDIRECTION_up) {
            return stemmedDrawingInterface->GetDrawingStemStart(this).y - doc->GetDrawingUnit(staffSize);
        }
        else {
            return stemmedDrawingInterface->GetDrawingStemEnd(this).y;
        }
    }
    return this->GetDrawingY();
}

double LayerElement::GetAlignmentDuration(Mensur *mensur, MeterSig *meterSig, bool notGraceOnly)
{
    if (this->IsGraceNote() && notGraceOnly) {
        return 0.0;
    }

    if (this->HasInterface(INTERFACE_DURATION)) {
        int num = 1;
        int numbase = 1;
        Tuplet *tuplet = dynamic_cast<Tuplet *>(this->GetFirstParent(TUPLET, MAX_TUPLET_DEPTH));
        if (tuplet) {
            num = tuplet->GetNum();
            numbase = tuplet->GetNumbase();
            // 0 is not valid in MEI anyway - just correct it silently
            if (num == 0) num = 1;
            if (numbase == 0) numbase = 1;
        }
        DurationInterface *duration = this->GetDurationInterface();
        assert(duration);
        if (duration->IsMensural()) {
            return duration->GetInterfaceAlignmentMensuralDuration(num, numbase, mensur);
        }
        double durationValue = duration->GetInterfaceAlignmentDuration(num, numbase);
        // With fTrem we need to divide the duration by two
        FTrem *fTrem = dynamic_cast<FTrem *>(this->GetFirstParent(FTREM, MAX_FTREM_DEPTH));
        if (fTrem) {
            durationValue /= 2.0;
        }
        return durationValue;
    }
    else if (this->Is(BEATRPT)) {
        BeatRpt *beatRpt = dynamic_cast<BeatRpt *>(this);
        assert(beatRpt);
        int meterUnit = 4;
        if (meterSig && meterSig->HasUnit()) meterSig->GetUnit();
        return beatRpt->GetBeatRptAlignmentDuration(meterUnit);
    }
    else if (this->Is(TIMESTAMP_ATTR)) {
        TimestampAttr *timestampAttr = dynamic_cast<TimestampAttr *>(this);
        assert(timestampAttr);
        int meterUnit = 4;
        if (meterSig && meterSig->HasUnit()) meterUnit = meterSig->GetUnit();
        return timestampAttr->GetTimestampAttrAlignmentDuration(meterUnit);
    }
    // We align all full measure element to the current time signature, even the ones that last longer than one measure
    else if (this->Is({ MREST, MULTIREST, MRPT, MRPT2, MULTIREST })) {
        int meterUnit = 4;
        int meterCount = 4;
        if (meterSig && meterSig->HasUnit()) meterUnit = meterSig->GetUnit();
        if (meterSig && meterSig->HasCount()) meterCount = meterSig->GetCount();
        return DUR_MAX / meterUnit * meterCount;
    }
    else {
        return 0.0;
    }
}

//----------------------------------------------------------------------------
// LayerElement functors methods
//----------------------------------------------------------------------------

int LayerElement::ResetHorizontalAlignment(FunctorParams *functorParams)
{
    SetDrawingXRel(0);
    // Exception here: the LayerElement::m_drawingYRel position is already set for horizontal alignment
    // See Object::SetAlignmentPitchPos - for this reason we need to reset it here and not in ResetVerticalAlignment
    SetDrawingYRel(0);

    m_alignment = NULL;
    m_graceAlignment = NULL;
    m_alignmentLayerN = VRV_UNSET;

    return FUNCTOR_CONTINUE;
}

int LayerElement::ResetVerticalAlignment(FunctorParams *functorParams)
{
    // Nothing to do since m_drawingYRel is reset in ResetHorizontalAlignment and set in SetAlignmentPitchPos

    return FUNCTOR_CONTINUE;
}

int LayerElement::AlignHorizontally(FunctorParams *functorParams)
{
    AlignHorizontallyParams *params = dynamic_cast<AlignHorizontallyParams *>(functorParams);
    assert(params);

    assert(!m_alignment);

    this->SetScoreDefRole(params->m_scoreDefRole);

    AlignmentType type = ALIGNMENT_DEFAULT;

    Chord *chordParent = dynamic_cast<Chord *>(this->GetFirstParent(CHORD, MAX_CHORD_DEPTH));
    Note *noteParent = dynamic_cast<Note *>(this->GetFirstParent(NOTE, MAX_NOTE_DEPTH));
    Rest *restParent = dynamic_cast<Rest *>(this->GetFirstParent(REST, MAX_NOTE_DEPTH));

    if (chordParent) {
        m_alignment = chordParent->GetAlignment();
    }
    else if (noteParent) {
        m_alignment = noteParent->GetAlignment();
    }
    else if (restParent) {
        m_alignment = restParent->GetAlignment();
    }
    else if (this->Is({ DOTS, FLAG, STEM })) {
        assert(false);
    }
    // We do not align these (formely container). Any other?
    else if (this->Is({ BEAM, FTREM, TUPLET })) {
        return FUNCTOR_CONTINUE;
    }
    else if (this->Is(BARLINE)) {
        type = ALIGNMENT_BARLINE;
    }
    else if (this->Is(CLEF)) {
        if ((this->GetScoreDefRole() == SYSTEM_SCOREDEF) || (this->GetScoreDefRole() == INTERMEDIATE_SCOREDEF))
            type = ALIGNMENT_SCOREDEF_CLEF;
        else if (this->GetScoreDefRole() == CAUTIONARY_SCOREDEF)
            type = ALIGNMENT_SCOREDEF_CAUTION_CLEF;
        else {
            type = ALIGNMENT_CLEF;
        }
    }
    else if (this->Is(KEYSIG)) {
        if ((this->GetScoreDefRole() == SYSTEM_SCOREDEF) || (this->GetScoreDefRole() == INTERMEDIATE_SCOREDEF))
            type = ALIGNMENT_SCOREDEF_KEYSIG;
        else if (this->GetScoreDefRole() == CAUTIONARY_SCOREDEF)
            type = ALIGNMENT_SCOREDEF_CAUTION_KEYSIG;
        else {
            // type = ALIGNMENT_KEYSIG;
            // We force this because they should appear only at the beginning of a measure and should be non-justifiable
            // We also need it because the PAE importer creates keySig (and not staffDef @key.sig)
            type = ALIGNMENT_SCOREDEF_KEYSIG;
        }
    }
    else if (this->Is(MENSUR)) {
        if ((this->GetScoreDefRole() == SYSTEM_SCOREDEF) || (this->GetScoreDefRole() == INTERMEDIATE_SCOREDEF))
            type = ALIGNMENT_SCOREDEF_MENSUR;
        else if (this->GetScoreDefRole() == CAUTIONARY_SCOREDEF)
            type = ALIGNMENT_SCOREDEF_CAUTION_MENSUR;
        else {
            // replace the current mensur
            params->m_currentMensur = dynamic_cast<Mensur *>(this);
            assert(params->m_currentMensur);
            type = ALIGNMENT_MENSUR;
        }
    }
    else if (this->Is(METERSIG)) {
        if ((this->GetScoreDefRole() == SYSTEM_SCOREDEF) || (this->GetScoreDefRole() == INTERMEDIATE_SCOREDEF))
            type = ALIGNMENT_SCOREDEF_METERSIG;
        else if (this->GetScoreDefRole() == CAUTIONARY_SCOREDEF)
            type = ALIGNMENT_SCOREDEF_CAUTION_METERSIG;
        else {
            // replace the current meter signature
            params->m_currentMeterSig = dynamic_cast<MeterSig *>(this);
            assert(params->m_currentMeterSig);
            // type = ALIGNMENT_METERSIG
            // We force this because they should appear only at the beginning of a measure and should be non-justifiable
            // We also need it because the PAE importer creates meterSig (and not staffDef @meter)
            type = ALIGNMENT_SCOREDEF_METERSIG;
        }
    }
    else if (this->Is({ MULTIREST, MREST, MRPT })) {
        type = ALIGNMENT_FULLMEASURE;
    }
    else if (this->Is({ MRPT2, MULTIRPT })) {
        type = ALIGNMENT_FULLMEASURE2;
    }
    else if (this->Is(DOT)) {
        type = ALIGNMENT_DOT;
    }
    else if (this->Is(ACCID)) {
        // accid within note was already taken into account by noteParent
        type = ALIGNMENT_ACCID;
    }
    else if (this->Is({ ARTIC, ARTIC_PART, SYL })) {
        // Refer to the note parent
        Note *note = dynamic_cast<Note *>(this->GetFirstParent(NOTE));
        assert(note);
        m_alignment = note->GetAlignment();
    }
    else if (this->Is(VERSE)) {
        // Idem
        Note *note = dynamic_cast<Note *>(this->GetFirstParent(NOTE));
        assert(note);
        m_alignment = note->GetAlignment();
    }
    else if (this->IsGraceNote()) {
        type = ALIGNMENT_GRACENOTE;
    }

    double duration = 0.0;
    // We have already an alignment with grace note children - skip this
    if (!m_alignment) {
        // get the duration of the event
        duration = this->GetAlignmentDuration(params->m_currentMensur, params->m_currentMeterSig);

        // For timestamp, what we get from GetAlignmentDuration is actually the position of the timestamp
        // So use it as current time - we can do this because the timestamp loop is redirected from the measure
        // The time will be reset to 0.0 when starting a new layer anyway
        if (this->Is(TIMESTAMP_ATTR))
            params->m_time = duration;
        else
            params->m_measureAligner->SetMaxTime(params->m_time + duration);

        m_alignment = params->m_measureAligner->GetAlignmentAtTime(params->m_time, type);
        assert(m_alignment);
    }

    if (m_alignment->GetType() != ALIGNMENT_GRACENOTE) {
        if (m_alignment->AddLayerElementRef(this)) params->m_hasMultipleLayer = true;
    }
    // For grace note aligner do not add them to the reference list because they will be processed by their original
    // hierarchy from the GraceAligner
    else {
        assert(this->IsGraceNote());
        if (this->Is(CHORD) || (this->Is(NOTE) && !chordParent)) {
            GraceAligner *graceAligner = m_alignment->GetGraceAligner();
            // We know that this is a note or a chord - we stack them and they will be added at the end of the layer
            // This will also see it for all their children
            graceAligner->StackGraceElement(this);
        }
    }

    if (!this->Is(TIMESTAMP_ATTR)) {
        // increase the time position, but only when not a timestamp (it would actually do nothing)
        params->m_time += duration;
    }

    // LogDebug("AlignHorizontally: Time %f - %s", (*time), this->GetClassName().c_str());

    return FUNCTOR_CONTINUE;
}

int LayerElement::SetAlignmentPitchPos(FunctorParams *functorParams)
{
    SetAlignmentPitchPosParams *params = dynamic_cast<SetAlignmentPitchPosParams *>(functorParams);
    assert(params);

    LayerElement *layerElementY = this;
    Staff *staffY = dynamic_cast<Staff *>(this->GetFirstParent(STAFF));
    assert(staffY);
    Layer *layerY = dynamic_cast<Layer *>(this->GetFirstParent(LAYER));
    assert(layerY);

    if (m_crossStaff && m_crossLayer) {
        layerElementY = m_crossLayer->GetAtPos(this->GetDrawingX());
        staffY = m_crossStaff;
        layerY = m_crossLayer;
    }

    // Adjust m_drawingYRel for notes and rests, etc.
    if (this->Is(ACCID)) {
        Accid *accid = dynamic_cast<Accid *>(this);
        assert(accid);
        Note *note = dynamic_cast<Note *>(this->GetFirstParent(NOTE));
        // We should probably also avoid to add editorial accidentals to the accid space
        // However, since they are placed above by View::DrawNote it works without avoiding it
        if (note) {
            if (note->HasGraceAlignment())
                note->GetGraceAlignment()->AddToAccidSpace(accid);
            else
                m_alignment->AddToAccidSpace(accid);
        }
        else {
            // do something for accid that are not children of a note - e.g., mensural?
            this->SetDrawingYRel(staffY->CalcPitchPosYRel(params->m_doc, accid->CalcDrawingLoc(layerY, layerElementY)));
        }
    }
    else if (this->Is(CHORD)) {
        // The y position is set to the top note one
        Chord *chord = dynamic_cast<Chord *>(this);
        assert(chord);
        Note *note = chord->GetTopNote();
        assert(note);
        int loc = PitchInterface::CalcLoc(note->GetPname(), note->GetOct(), layerY->GetClefLocOffset(layerElementY));
        // Once we have AttLoc on Note
        // if (note->HasLoc()) loc = note->GetLoc();
        this->SetDrawingYRel(staffY->CalcPitchPosYRel(params->m_doc, loc));
    }
    else if (this->Is({ CUSTOS, DOT })) {
        PositionInterface *interface = dynamic_cast<PositionInterface *>(this);
        assert(interface);
        this->SetDrawingYRel(staffY->CalcPitchPosYRel(params->m_doc, interface->CalcDrawingLoc(layerY, layerElementY)));
    }
    else if (this->Is(NOTE)) {
        Note *note = dynamic_cast<Note *>(this);
        assert(note);
        Chord *chord = note->IsChordTone();
        int loc = PitchInterface::CalcLoc(note->GetPname(), note->GetOct(), layerY->GetClefLocOffset(layerElementY));
        // Once we have AttLoc on Note
        // if (note->HasLoc()) loc = note->GetLoc();
        int yRel = staffY->CalcPitchPosYRel(params->m_doc, loc);
        // Make it relative to the top note one (see above) but not for cross-staff notes in chords
        if (chord && !m_crossStaff) {
            yRel -= chord->GetDrawingYRel();
        }
        note->SetDrawingLoc(loc);
        this->SetDrawingYRel(yRel);
    }
    else if (this->Is(REST)) {
        Rest *rest = dynamic_cast<Rest *>(this);
        assert(rest);
        int loc = 0;
        if (rest->HasPloc() && rest->HasOloc()) {
            loc = PitchInterface::CalcLoc(rest->GetPloc(), rest->GetOloc(), layerY->GetClefLocOffset(layerElementY));
        }
        else if (rest->HasLoc()) {
            loc = rest->GetLoc();
        }
        // Automatically calculate rest position
        else {
            // Limitation: GetLayerCount does not take into account editorial markup
            bool hasMultipleLayer = (staffY->GetLayerCount() > 1);
            bool isFirstLayer = false;
            if (hasMultipleLayer) {
                Layer *firstLayer = dynamic_cast<Layer *>(staffY->FindChildByType(LAYER));
                assert(firstLayer);
                if (firstLayer->GetN() == layerY->GetN()) isFirstLayer = true;
            }
            loc = rest->GetDefaultLoc(hasMultipleLayer, isFirstLayer);
        }
        rest->SetDrawingLoc(loc);
        this->SetDrawingYRel(staffY->CalcPitchPosYRel(params->m_doc, loc));
    }

    return FUNCTOR_CONTINUE;
}

int LayerElement::AdjustLayers(FunctorParams *functorParams)
{
    AdjustLayersParams *params = dynamic_cast<AdjustLayersParams *>(functorParams);
    assert(params);

    // Check if we are starting a new layer content - if yes copy the current elements to previous
    if (!params->m_current.empty() && (this->GetAlignmentLayerN() != params->m_currentLayerN)) {
        params->m_previous.reserve(params->m_previous.size() + params->m_current.size());
        params->m_previous.insert(params->m_previous.end(), params->m_current.begin(), params->m_current.end());
        params->m_current.clear();
    }

    params->m_currentLayerN = this->GetAlignmentLayerN();

    // These are the only ones we want to keep for further collision detection
    // Eventually  we also need stem for overlapping voices
    if (this->Is({ DOTS, NOTE }) && this->HasUpdatedBB()) params->m_current.push_back(this);

    // We are processing the first layer, nothing to do yet
    if (params->m_previous.empty()) return FUNCTOR_SIBLINGS;

    if (this->Is(NOTE)) {
        params->m_currentNote = dynamic_cast<Note *>(this);
        assert(params->m_currentNote);
        if (!params->m_currentNote->IsChordTone()) params->m_currentChord = NULL;
    }
    else if (this->Is(CHORD)) {
        params->m_currentChord = dynamic_cast<Chord *>(this);
        assert(params->m_currentChord);
    }

    // Eventually we also want to have stem for overlapping voices
    if (this->Is({ NOTE, DOTS })) {

        Staff *staff = dynamic_cast<Staff *>(this->GetFirstParent(STAFF));
        assert(staff);

        std::vector<LayerElement *>::iterator iter;
        for (iter = params->m_previous.begin(); iter != params->m_previous.end(); iter++) {

            int verticalMargin = 0; // 1 * params->m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
            int horizontalMargin = 2 * params->m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);

            if (this->Is(NOTE) && (*iter)->Is(NOTE)) {
                assert(params->m_currentNote);
                Note *previousNote = dynamic_cast<Note *>(*iter);
                assert(previousNote);
                // Unisson, look at the duration for the note heads
                if (params->m_currentNote->IsUnissonWith(previousNote, false)) {
                    if ((params->m_currentNote->GetDrawingDur() == DUR_2) && (previousNote->GetDrawingDur() == DUR_2))
                        continue;
                    else if ((params->m_currentNote->GetDrawingDur() > DUR_2)
                        && (previousNote->GetDrawingDur() > DUR_2))
                        continue;
                    // Reduce the margin to 0 for whole notes unisson
                    else if ((params->m_currentNote->GetDrawingDur() == DUR_1)
                        && (previousNote->GetDrawingDur() == DUR_1))
                        horizontalMargin = 0;
                }
                else if ((previousNote->GetDrawingLoc() - params->m_currentNote->GetDrawingLoc()) > 1)
                    continue;
            }

            // Nothing to do if we have no vertical overlapping
            if (!this->VerticalSelfOverlap(*iter, verticalMargin)) continue;

            int xRelShift = this->HorizontalLeftOverlap(*iter, params->m_doc, horizontalMargin, verticalMargin);

            // Move the appropriate parent to the left
            if (xRelShift > 0) {
                if (params->m_currentChord)
                    params->m_currentChord->SetDrawingXRel(params->m_currentChord->GetDrawingXRel() + xRelShift);
                else
                    params->m_currentNote->SetDrawingXRel(params->m_currentNote->GetDrawingXRel() + xRelShift);
            }
        }
    }

    return FUNCTOR_SIBLINGS;
}

int LayerElement::AdjustGraceXPos(FunctorParams *functorParams)
{
    AdjustGraceXPosParams *params = dynamic_cast<AdjustGraceXPosParams *>(functorParams);
    assert(params);

    if (params->m_graceCumulatedXShift == VRV_UNSET) params->m_graceCumulatedXShift = 0;

    // LogDebug("Aligning %s", this->GetClassName().c_str());

    // With non grace alignment we do not need to do this
    this->ResetCachedDrawingX();

    if (!this->HasGraceAlignment()) return FUNCTOR_SIBLINGS;

    if (!this->HasUpdatedBB() || this->HasEmptyBB()) {
        // if nothing was drawn, do not take it into account
        return FUNCTOR_SIBLINGS;
    }

    int selfRight = this->GetSelfRight();
    int offset = selfRight - params->m_graceMaxPos;
    if (offset > 0) {
        this->GetGraceAlignment()->SetXRel(this->GetGraceAlignment()->GetXRel() - offset);
        // Also move the cumultated x shift and the minimum position for the next alignment accordingly
        params->m_graceCumulatedXShift += (-offset);
        params->m_graceUpcomingMaxPos += (-offset);
    }

    int selfLeft = this->GetSelfLeft()
        - params->m_doc->GetLeftMargin(this->GetClassId())
            * params->m_doc->GetDrawingUnit(params->m_doc->GetCueSize(100)) / PARAM_DENOMINATOR;

    params->m_graceUpcomingMaxPos = std::min(selfLeft, params->m_graceUpcomingMaxPos);

    return FUNCTOR_SIBLINGS;
}

int LayerElement::AdjustXPos(FunctorParams *functorParams)
{
    AdjustXPosParams *params = dynamic_cast<AdjustXPosParams *>(functorParams);
    assert(params);

    // we should have processed aligned before
    assert(this->GetAlignment());

    if (!this->HasToBeAligned()) {
        // if nothing to do with this type of element
        // this happens for example with Artic where only ArticPart children are aligned
        return FUNCTOR_SIBLINGS;
    }

    int selfLeft;
    if (!this->HasUpdatedBB() || this->HasEmptyBB()) {
        // if nothing was drawn, do not take it into account
        // assert(this->Is({ BARLINE_ATTR_LEFT, BARLINE_ATTR_RIGHT }));
        // This should happen for invis barline attribute but also chords in beam. Otherwise the BB should be set to
        // empty with
        // Object::SetEmptyBB()
        // LogDebug("Nothing drawn for '%s' '%s'", this->GetClassName().c_str(), this->GetUuid().c_str());
        selfLeft = this->GetAlignment()->GetXRel();
    }
    else {
        // We add it to the upcoming bouding boxes
        params->m_upcomingBoundingBoxes.push_back(this);
        selfLeft = this->GetSelfLeft()
            - params->m_doc->GetLeftMargin(this->GetClassId()) * params->m_doc->GetDrawingUnit(100) / PARAM_DENOMINATOR;
    }

    int offset = selfLeft - params->m_minPos;
    if (offset < 0) {
        this->GetAlignment()->SetXRel(this->GetAlignment()->GetXRel() - offset);
        // Also move the cumultated x shift and the minimum position for the next alignment accordingly
        params->m_cumulatedXShift += (-offset);
        params->m_upcomingMinPos += (-offset);
    }

    int selfRight;
    if (!this->HasUpdatedBB() || this->HasEmptyBB())
        selfRight = this->GetAlignment()->GetXRel()
            + params->m_doc->GetRightMargin(this->GetClassId()) * params->m_doc->GetDrawingUnit(100)
                / PARAM_DENOMINATOR;
    else
        selfRight = this->GetSelfRight()
            + params->m_doc->GetRightMargin(this->GetClassId()) * params->m_doc->GetDrawingUnit(100)
                / PARAM_DENOMINATOR;

    params->m_upcomingMinPos = std::max(selfRight, params->m_upcomingMinPos);

    return FUNCTOR_SIBLINGS;
}

int LayerElement::PrepareDrawingCueSize(FunctorParams *functorParams)
{
    if (this->IsGraceNote()) {
        m_drawingCueSize = true;
    }
    // This cover the case when the @size is given on the element
    else if (this->HasAttClass(ATT_RELATIVESIZE)) {
        AttRelativesize *att = dynamic_cast<AttRelativesize *>(this);
        assert(att);
        if (att->HasSize()) m_drawingCueSize = (att->GetSize() == SIZE_cue);
    }
    // For note, we also need to look at the parent chord
    else if (this->Is(NOTE)) {
        Note const *note = dynamic_cast<Note const *>(this);
        assert(note);
        Chord *chord = note->IsChordTone();
        if (chord) m_drawingCueSize = chord->IsCueSize();
    }
    // For tuplet, we also need to look at the first note or chord
    else if (this->Is(TUPLET)) {
        AttComparisonAny matchType({ NOTE, CHORD });
        ArrayOfObjects children;
        LayerElement *child = dynamic_cast<LayerElement *>(this->FindChildByAttComparison(&matchType));
        if (child) m_drawingCueSize = child->IsCueSize();
    }
    // For accid, look at the parent if @func="edit" or otherwise to the parent note
    else if (this->Is(ACCID)) {
        Accid const *accid = dynamic_cast<Accid *>(this);
        assert(accid);
        if (accid->GetFunc() == accidLog_FUNC_edit)
            m_drawingCueSize = true;
        else {
            Note *note = dynamic_cast<Note *>(this->GetFirstParent(NOTE, MAX_ACCID_DEPTH));
            if (note) m_drawingCueSize = note->IsCueSize();
        }
    }
    else if (this->Is({ DOTS, FLAG, STEM })) {
        Note *note = dynamic_cast<Note *>(this->GetFirstParent(NOTE, MAX_NOTE_DEPTH));
        if (note)
            m_drawingCueSize = note->IsCueSize();
        else {
            Chord *chord = dynamic_cast<Chord *>(this->GetFirstParent(CHORD, MAX_CHORD_DEPTH));
            if (chord) m_drawingCueSize = chord->IsCueSize();
        }
    }

    return FUNCTOR_CONTINUE;
}

int LayerElement::PrepareCrossStaff(FunctorParams *functorParams)
{
    PrepareCrossStaffParams *params = dynamic_cast<PrepareCrossStaffParams *>(functorParams);
    assert(params);

    m_crossStaff = NULL;
    m_crossLayer = NULL;

    // Look for cross-staff situations
    // If we have one, make is available in m_crossStaff
    DurationInterface *durElement = this->GetDurationInterface();
    if (!durElement) return FUNCTOR_CONTINUE;

    // If we have not @staff, set to what we had before (quite likely NULL for all non cross staff cases)
    if (!durElement->HasStaff()) {
        m_crossStaff = params->m_currentCrossStaff;
        m_crossLayer = params->m_currentCrossLayer;
        return FUNCTOR_CONTINUE;
    }

    // We have a @staff, set the current pointers to NULL before assigning them
    params->m_currentCrossStaff = NULL;
    params->m_currentCrossLayer = NULL;

    AttCommonNComparison comparisonFirst(STAFF, durElement->GetStaff().at(0));
    m_crossStaff = dynamic_cast<Staff *>(params->m_currentMeasure->FindChildByAttComparison(&comparisonFirst, 1));
    if (!m_crossStaff) {
        LogWarning("Could not get the cross staff reference '%d' for element '%s'", durElement->GetStaff().at(0),
            this->GetUuid().c_str());
        return FUNCTOR_CONTINUE;
    }

    Staff *parentStaff = dynamic_cast<Staff *>(this->GetFirstParent(STAFF));
    assert(parentStaff);
    // Check if we have a cross-staff to itself...
    if (m_crossStaff == parentStaff) {
        LogWarning("The cross staff reference '%d' for element '%s' seems to be identical to the parent staff",
            durElement->GetStaff().at(0), this->GetUuid().c_str());
        m_crossStaff = NULL;
        return FUNCTOR_CONTINUE;
    }

    Layer *parentLayer = dynamic_cast<Layer *>(this->GetFirstParent(LAYER));
    assert(parentLayer);
    // Now try to get the corresponding layer - for now look for the same layer @n
    int layerN = parentLayer->GetN();
    // When we will have allowed @layer in <note>, we will have to do:
    // int layerN = durElement->HasLayer() ? durElement->GetLayer() : (*currentLayer)->GetN();
    AttCommonNComparison comparisonFirstLayer(LAYER, layerN);
    m_crossLayer = dynamic_cast<Layer *>(m_crossStaff->FindChildByAttComparison(&comparisonFirstLayer, 1));
    if (!m_crossLayer) {
        // Just try to pick the first one...
        m_crossLayer = dynamic_cast<Layer *>(m_crossStaff->FindChildByType(LAYER));

    }
    if (!m_crossLayer) {
        // Nothing we can do
        LogWarning("Could not get the layer with cross-staff reference '%d' for element '%s'",
            durElement->GetStaff().at(0), this->GetUuid().c_str());
        m_crossStaff = NULL;
    }

    params->m_currentCrossStaff = m_crossStaff;
    params->m_currentCrossLayer = m_crossLayer;

    return FUNCTOR_CONTINUE;
}

int LayerElement::PrepareCrossStaffEnd(FunctorParams *functorParams)
{
    PrepareCrossStaffParams *params = dynamic_cast<PrepareCrossStaffParams *>(functorParams);
    assert(params);

    DurationInterface *durElement = this->GetDurationInterface();
    if (!durElement) return FUNCTOR_CONTINUE;

    // If we have  @staff, set reset it to NULL - this can be problematic if we have different @staff attributes
    // in the the children of one element. We do not consider this now because it seems over the top
    // We would need to look at the @n attribute and to have a stack to handle this properly
    if (durElement->HasStaff()) {
        params->m_currentCrossStaff = NULL;
        params->m_currentCrossLayer = NULL;
    }

    return FUNCTOR_CONTINUE;
}

int LayerElement::PrepareTimePointing(FunctorParams *functorParams)
{
    PrepareTimePointingParams *params = dynamic_cast<PrepareTimePointingParams *>(functorParams);
    assert(params);

    // Do not look for tstamp pointing to these
    if (this->Is({ ARTIC, ARTIC_PART, BEAM, FLAG, TUPLET, STEM, VERSE })) return FUNCTOR_CONTINUE;

    ArrayOfPointingInterClassIdPairs::iterator iter = params->m_timePointingInterfaces.begin();
    while (iter != params->m_timePointingInterfaces.end()) {
        if (iter->first->SetStartOnly(this)) {
            // We have both the start and the end that are matched
            iter = params->m_timePointingInterfaces.erase(iter);
        }
        else {
            iter++;
        }
    }

    return FUNCTOR_CONTINUE;
}

int LayerElement::PrepareTimeSpanning(FunctorParams *functorParams)
{
    PrepareTimeSpanningParams *params = dynamic_cast<PrepareTimeSpanningParams *>(functorParams);
    assert(params);

    // Do not look for tstamp pointing to these
    if (this->Is({ ARTIC, ARTIC_PART, BEAM, FLAG, TUPLET, STEM, VERSE })) return FUNCTOR_CONTINUE;

    ArrayOfSpanningInterClassIdPairs::iterator iter = params->m_timeSpanningInterfaces.begin();
    while (iter != params->m_timeSpanningInterfaces.end()) {
        if (iter->first->SetStartAndEnd(this)) {
            // We have both the start and the end that are matched
            iter = params->m_timeSpanningInterfaces.erase(iter);
        }
        else {
            iter++;
        }
    }

    return FUNCTOR_CONTINUE;
}

int LayerElement::FindTimeSpanningLayerElements(FunctorParams *functorParams)
{
    FindTimeSpanningLayerElementsParams *params = dynamic_cast<FindTimeSpanningLayerElementsParams *>(functorParams);
    assert(params);

    if ((this->GetDrawingX() > params->m_minPos) && (this->GetDrawingX() < params->m_maxPos)) {
        params->m_spanningContent.push_back(this);
    }
    else if (this->GetDrawingX() > params->m_maxPos) {
        return FUNCTOR_STOP;
    }

    return FUNCTOR_CONTINUE;
}

int LayerElement::GenerateMIDI(FunctorParams *functorParams)
{
    GenerateMIDIParams *params = dynamic_cast<GenerateMIDIParams *>(functorParams);
    assert(params);

    // Here we need to check if the LayerElement as a duration, otherwise we can continue
    if (!this->HasInterface(INTERFACE_DURATION)) return FUNCTOR_CONTINUE;

    // Now deal with the different elements
    if (this->Is(REST)) {
        // Rest *rest = dynamic_cast<Rest *>(this);
        // assert(rest);
        // LogMessage("Rest %f", GetAlignmentDuration());
        // increase the currentTime accordingly
        params->m_currentMeasureTime += GetAlignmentDuration() * params->m_currentBpm / (DUR_MAX / DURATION_4);
    }
    else if (this->Is(NOTE)) {
        Note *note = dynamic_cast<Note *>(this);
        assert(note);

        // For now just ignore grace notes
        if (note->HasGrace()) return FUNCTOR_CONTINUE;

        Chord *chord = note->IsChordTone();

        double dur;
        if (chord)
            dur = chord->GetAlignmentDuration();
        else
            dur = note->GetAlignmentDuration();
        dur = dur * params->m_currentBpm / (DUR_MAX / DURATION_4);

        // LogDebug("Note Alignment Duration %f - Dur %d - Diatonic Pitch %d - Track %d", GetAlignmentDuration(),
        // note->GetNoteOrChordDur(this), note->GetDiatonicPitch(), *midiTrack);
        // LogDebug("Oct %d - Pname %d - Accid %d", note->GetOct(), note->GetPname(), note->GetAccid());

        Accid *accid = note->GetDrawingAccid();

        // Create midi note
        int midiBase = 0;
        data_PITCHNAME pname = note->GetPname();
        switch (pname) {
            case PITCHNAME_c: midiBase = 0; break;
            case PITCHNAME_d: midiBase = 2; break;
            case PITCHNAME_e: midiBase = 4; break;
            case PITCHNAME_f: midiBase = 5; break;
            case PITCHNAME_g: midiBase = 7; break;
            case PITCHNAME_a: midiBase = 9; break;
            case PITCHNAME_b: midiBase = 11; break;
            case PITCHNAME_NONE: break;
        }
        // Check for accidentals
        if (accid && accid->HasAccidGes()) {
            data_ACCIDENTAL_IMPLICIT accImp = accid->GetAccidGes();
            switch (accImp) {
                case ACCIDENTAL_IMPLICIT_s: midiBase += 1; break;
                case ACCIDENTAL_IMPLICIT_f: midiBase -= 1; break;
                case ACCIDENTAL_IMPLICIT_ss: midiBase += 2; break;
                case ACCIDENTAL_IMPLICIT_ff: midiBase -= 2; break;
                default: break;
            }
        }
        else if (accid) {
            data_ACCIDENTAL_EXPLICIT accExp = accid->GetAccid();
            switch (accExp) {
                case ACCIDENTAL_EXPLICIT_s: midiBase += 1; break;
                case ACCIDENTAL_EXPLICIT_f: midiBase -= 1; break;
                case ACCIDENTAL_EXPLICIT_ss: midiBase += 2; break;
                case ACCIDENTAL_EXPLICIT_x: midiBase += 2; break;
                case ACCIDENTAL_EXPLICIT_ff: midiBase -= 2; break;
                case ACCIDENTAL_EXPLICIT_xs: midiBase += 3; break;
                case ACCIDENTAL_EXPLICIT_ts: midiBase += 3; break;
                case ACCIDENTAL_EXPLICIT_tf: midiBase -= 3; break;
                case ACCIDENTAL_EXPLICIT_nf: midiBase -= 1; break;
                case ACCIDENTAL_EXPLICIT_ns: midiBase += 1; break;
                default: break;
            }
        }

        // Adjustment for transposition intruments
        midiBase += params->m_transSemi;

        int oct = note->GetOct();
        if (note->HasOctGes()) oct = note->GetOctGes();

        int pitch = midiBase + (oct + 1) * 12;
        int channel = 0;
        int velocity = 64;
        params->m_midiFile->addNoteOn(
            params->m_midiTrack, params->m_totalTime + params->m_currentMeasureTime, channel, pitch, velocity);
        params->m_midiFile->addNoteOff(
            params->m_midiTrack, params->m_totalTime + params->m_currentMeasureTime + dur, channel, pitch);

        note->m_playingOnset = params->m_totalTime + params->m_currentMeasureTime;
        note->m_playingOffset = params->m_totalTime + params->m_currentMeasureTime + dur;

        // increase the currentTime accordingly, but only if not in a chord - checkit with note->IsChordTone()
        if (!(note->IsChordTone())) {
            params->m_currentMeasureTime += GetAlignmentDuration() * params->m_currentBpm / (DUR_MAX / DURATION_4);
        }
    }
    else if (this->Is(SPACE)) {
        // Space *space = dynamic_cast<Space *>(this);
        // assert(space);
        // LogMessage("Space %f", GetAlignmentDuration());
        // increase the currentTime accordingly
        params->m_currentMeasureTime += GetAlignmentDuration() * params->m_currentBpm / (DUR_MAX / DURATION_4);
    }
    return FUNCTOR_CONTINUE;
}

int LayerElement::GenerateMIDIEnd(FunctorParams *functorParams)
{
    GenerateMIDIParams *params = dynamic_cast<GenerateMIDIParams *>(functorParams);
    assert(params);

    if (this->Is(CHORD)) {
        // Chord *chord = dynamic_cast<Chord *>(this);
        // assert(chord);
        // LogMessage("Chord %f", GetAlignmentDuration());
        // increase the currentTime accordingly.
        params->m_currentMeasureTime += GetAlignmentDuration() * params->m_currentBpm / (DUR_MAX / DURATION_4);
    }

    return FUNCTOR_CONTINUE;
}

int LayerElement::CalcMaxMeasureDuration(FunctorParams *functorParams)
{
    CalcMaxMeasureDurationParams *params = dynamic_cast<CalcMaxMeasureDurationParams *>(functorParams);
    assert(params);

    // Here we need to check if the LayerElement as a duration, otherwise we can continue
    if (!this->HasInterface(INTERFACE_DURATION)) return FUNCTOR_CONTINUE;

    if (this->Is(NOTE)) {
        Note *note = dynamic_cast<Note *>(this);
        assert(note);

        // For now just ignore grace notes
        if (note->HasGrace()) return FUNCTOR_CONTINUE;

        // The is increased by the chord element
        if (note->IsChordTone()) return FUNCTOR_CONTINUE;
    }

    // increase the currentTime accordingly
    params->m_currentValue += GetAlignmentDuration() * params->m_currentBpm / (DUR_MAX / DURATION_4);

    // now if we have cummulated in the layer a longer duration for the current measure, replace it
    if (params->m_maxValues.back() < params->m_currentValue) params->m_maxValues.back() = params->m_currentValue;

    return FUNCTOR_CONTINUE;
}

int LayerElement::ResetDrawing(FunctorParams *)
{
    m_drawingCueSize = false;

    return FUNCTOR_CONTINUE;
};

} // namespace vrv
