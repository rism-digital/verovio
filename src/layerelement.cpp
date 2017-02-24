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
#include "aligner.h"
#include "attcomparison.h"
#include "barline.h"
#include "beam.h"
#include "chord.h"
#include "clef.h"
#include "custos.h"
#include "doc.h"
#include "dot.h"
#include "functorparams.h"
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

LayerElement::LayerElement() : Object("le-")
{
    Reset();
}

LayerElement::LayerElement(std::string classid) : Object(classid)
{
    Reset();
}

void LayerElement::Reset()
{
    Object::Reset();

    m_xAbs = VRV_UNSET;
    m_drawingYRel = 0;
    m_drawingXRel = 0;

    m_scoreDefRole = NONE;
    m_alignment = NULL;
    m_graceAlignment = NULL;
    m_beamElementCoord = NULL;

    m_crossStaff = NULL;
    m_crossLayer = NULL;
}

LayerElement::~LayerElement()
{
}

LayerElement &LayerElement::operator=(const LayerElement &element)
{
    // not self assignement
    if (this != &element) {
        // pointers have to be NULL
        m_parent = NULL;
        m_alignment = NULL;
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
    // For accid, look at the parent note
    else if (this->Is(ACCID)) {
        Note *note = dynamic_cast<Note *>(this->GetFirstParent(NOTE, MAX_ACCID_DEPTH));
        return (note && (note->HasGrace()));
    }
    return false;
}

bool LayerElement::IsCueSize()
{
    if (this->IsGraceNote()) return true;

    // This cover the case when the @size is given on the element
    if (this->HasAttClass(ATT_RELATIVESIZE)) {
        AttRelativesize *att = dynamic_cast<AttRelativesize *>(this);
        assert(att);
        if (att->HasSize()) return (att->GetSize() == SIZE_cue);
    }

    // For note, we also need to look at the parent chord
    if (this->Is(NOTE)) {
        Note const *note = dynamic_cast<Note const *>(this);
        assert(note);
        Chord *chord = note->IsChordTone();
        if (chord) return chord->IsCueSize();
    }
    // For tuplet, we also need to look at the first note or chord
    else if (this->Is(TUPLET)) {
        AttComparisonAny matchType({ NOTE, CHORD });
        ArrayOfObjects children;
        LayerElement *child = dynamic_cast<LayerElement *>(this->FindChildByAttComparison(&matchType));
        if (child) return child->IsCueSize();
    }
    // For accid, look at the parent note
    else if (this->Is(ACCID)) {
        Note *note = dynamic_cast<Note *>(this->GetFirstParent(NOTE, MAX_ACCID_DEPTH));
        if (note) return note->IsCueSize();
    }
    return false;
}

bool LayerElement::IsFirstInLigature()
{
    if (!this->Is(NOTE)) return false;
    Object *ligature = this->GetFirstParent(LIGATURE, MAX_LIGATURE_DEPTH);
    if (!ligature) return false;
    if (ligature->GetFirst(NOTE) == this)
        return false;
    else
        return true;
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
    assert(m_alignment);

    Measure *measure = dynamic_cast<Measure *>(this->GetFirstParent(MEASURE));
    assert(measure);

    int graceNoteShift = 0;
    if (this->HasGraceAlignment()) {
        graceNoteShift = this->GetGraceAlignment()->GetXRel();
    }

    return (measure->GetDrawingX() + m_alignment->GetXRel() + this->GetDrawingXRel() + graceNoteShift);
}

int LayerElement::GetDrawingY() const
{
    // First look if we have a crossStaff situation
    Object *object = m_crossStaff;
    // Otherwise get the first staff
    if (!object) object = this->GetFirstParent(STAFF);
    // Otherwise the first measure (this is the case with barLineAttr
    if (!object) object = this->GetFirstParent(MEASURE);

    assert(object);
    return object->GetDrawingY() + this->GetDrawingYRel();
}

int LayerElement::GetDrawingArticulationTopOrBottom(data_STAFFREL place, ArticPartType type)
{
    // It would not crash otherwise but there is not reason to call it
    assert((this->Is(NOTE)) || (this->Is(CHORD)));

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

void LayerElement::CenterDrawingX()
{
    m_drawingXRel = 0;

    Measure *measure = dynamic_cast<Measure *>(this->GetFirstParent(MEASURE));
    assert(measure);

    m_drawingXRel = measure->GetInnerCenterX() - this->GetDrawingX();
}

int LayerElement::GetDrawingTop(Doc *doc, int staffSize, bool withArtic, ArticPartType type)
{
    if ((this->Is(NOTE)) || (this->Is(CHORD))) {
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
                chord->GetYExtremes(&yChordMax, &yChordMin);
                return yChordMax + doc->GetDrawingUnit(staffSize);
            }
            else
                return this->GetDrawingY() + doc->GetDrawingUnit(staffSize);
        }
        // We should also take into accound the stem shift to the right
        StemmedDrawingInterface *stemmedDrawingInterface = this->GetStemmedDrawingInterface();
        assert(stemmedDrawingInterface);
        if (stemmedDrawingInterface->GetDrawingStemDir() == STEMDIRECTION_up) {
            return stemmedDrawingInterface->GetDrawingStemEnd().y;
        }
        else {
            return stemmedDrawingInterface->GetDrawingStemStart().y + doc->GetDrawingUnit(staffSize);
        }
    }
    return this->GetDrawingY();
}

int LayerElement::GetDrawingBottom(Doc *doc, int staffSize, bool withArtic, ArticPartType type)
{
    if ((this->Is(NOTE)) || (this->Is(CHORD))) {
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
                chord->GetYExtremes(&yChordMax, &yChordMin);
                return yChordMin - doc->GetDrawingUnit(staffSize);
            }
            else
                return this->GetDrawingY() - doc->GetDrawingUnit(staffSize);
        }
        // We should also take into accound the stem shift to the right
        StemmedDrawingInterface *stemmedDrawingInterface = this->GetStemmedDrawingInterface();
        assert(stemmedDrawingInterface);
        if (stemmedDrawingInterface->GetDrawingStemDir() == STEMDIRECTION_up) {
            return stemmedDrawingInterface->GetDrawingStemStart().y - doc->GetDrawingUnit(staffSize);
        }
        else {
            return stemmedDrawingInterface->GetDrawingStemEnd().y;
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
    else if (this->Is(MREST) || this->Is(MULTIREST) || this->Is(MRPT) || this->Is(MRPT2) || this->Is(MULTIREST)) {
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
    m_drawingXRel = 0;
    // Exception here: the LayerElement::m_drawingYRel position is already set for horizontal alignment
    // See Object::SetAlignmentPitchPos - for this reason we need to reset it here and not in ResetVerticalAlignment
    m_drawingYRel = 0;

    m_alignment = NULL;
    m_graceAlignment = NULL;

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

    this->SetScoreDefRole(params->m_scoreDefRole);

    Chord *chordParent = dynamic_cast<Chord *>(this->GetFirstParent(CHORD, MAX_CHORD_DEPTH));
    if (chordParent) {
        m_alignment = chordParent->GetAlignment();
        return FUNCTOR_CONTINUE;
    }

    AlignmentType type = ALIGNMENT_DEFAULT;
    if (this->Is(BARLINE)) {
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
    else if (this->Is(MULTIREST) || this->Is(MREST) || this->Is(MRPT)) {
        type = ALIGNMENT_FULLMEASURE;
    }
    else if (this->Is(MRPT2) || this->Is(MULTIRPT)) {
        type = ALIGNMENT_FULLMEASURE2;
    }
    else if (this->Is(BEAM) || this->Is(LIGATURE) || this->Is(TUPLET)) {
        type = ALIGNMENT_CONTAINER;
    }
    else if (this->Is(DOT)) {
        type = ALIGNMENT_DOT;
    }
    else if (this->Is(ACCID)) {
        // Refer to the note parent (if any?)
        Note *note = dynamic_cast<Note *>(this->GetFirstParent(NOTE));
        if (note) {
            m_alignment = note->GetAlignment();
            return FUNCTOR_CONTINUE;
        }
        type = ALIGNMENT_ACCID;
    }
    else if (this->Is(ARTIC) || this->Is(ARTIC_PART) || this->Is(SYL)) {
        // Refer to the note parent
        Note *note = dynamic_cast<Note *>(this->GetFirstParent(NOTE));
        assert(note);
        m_alignment = note->GetAlignment();
        return FUNCTOR_CONTINUE;
    }
    else if (this->Is(VERSE)) {
        // Idem
        Note *note = dynamic_cast<Note *>(this->GetFirstParent(NOTE));
        assert(note);
        m_alignment = note->GetAlignment();
        return FUNCTOR_CONTINUE;
    }
    else if (this->IsGraceNote()) {
        type = ALIGNMENT_GRACENOTE;
    }

    // get the duration of the event
    double duration = this->GetAlignmentDuration(params->m_currentMensur, params->m_currentMeterSig);

    // For timestamp, what we get from GetAlignmentDuration is actually the position of the timestamp
    // So use it as current time - we can do this because the timestamp loop is redirected from the measure
    // The time will be reset to 0.0 when starting a new layer anyway
    if (this->Is(TIMESTAMP_ATTR))
        params->m_time = duration;
    else
        params->m_measureAligner->SetMaxTime(params->m_time + duration);

    m_alignment = params->m_measureAligner->GetAlignmentAtTime(params->m_time, type);
    if (type != ALIGNMENT_GRACENOTE) m_alignment->AddLayerElementRef(this);

    if (this->IsGraceNote()) {
        GraceAligner *graceAligner = m_alignment->GetGraceAligner();
        // We know that this is a note
        graceAligner->StackGraceElement(this);
    }

    // LogDebug("AlignHorizontally: Time %f - %s", (*time), this->GetClassName().c_str());

    // increase the time position, but only when not a timestamp (it would actually do nothing)
    if (!this->Is(TIMESTAMP_ATTR)) {
        params->m_time += duration;
    }

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
        assert(layerElementY);
        staffY = m_crossStaff;
        layerY = m_crossLayer;
    }

    // Adjust m_drawingYRel for notes and rests, etc.
    if (this->Is(ACCID)) {
        Accid *accid = dynamic_cast<Accid *>(this);
        assert(accid);
        Note *note = dynamic_cast<Note *>(this->GetFirstParent(NOTE));
        if (note) {
            accid->SetDrawingYRel(note->GetDrawingYRel());
        }
        else {
            // do something for accid that are not children of a note - e.g., mensural?
            int loc
                = PitchInterface::CalcLoc(accid->GetPloc(), accid->GetOloc(), layerY->GetClefLocOffset(layerElementY));
            // Override it if we have a @loc ?
            if (accid->HasLoc()) loc = accid->GetLoc();
            this->SetDrawingYRel(staffY->CalcPitchPosYRel(params->m_doc, loc));
        }
    }
    else if (this->Is(CUSTOS) || this->Is(DOT)) {
        PositionInterface *interface = dynamic_cast<PositionInterface *>(this);
        assert(interface);
        int loc = PitchInterface::CalcLoc(
            interface->GetPloc(), interface->GetOloc(), layerY->GetClefLocOffset(layerElementY));
        if (interface->HasLoc()) loc = interface->GetLoc();
        this->SetDrawingYRel(staffY->CalcPitchPosYRel(params->m_doc, loc));
    }
    else if (this->Is(NOTE)) {
        Note *note = dynamic_cast<Note *>(this);
        assert(note);
        int loc = PitchInterface::CalcLoc(note->GetPname(), note->GetOct(), layerY->GetClefLocOffset(layerElementY));
        // Once we have AttLoc on Note
        // if (note->HasLoc()) loc = note->GetLoc();
        this->SetDrawingYRel(staffY->CalcPitchPosYRel(params->m_doc, loc));
    }
    else if (this->Is(REST)) {
        Rest *rest = dynamic_cast<Rest *>(this);
        assert(rest);
        // Automatically calculate rest position, if so requested
        if (rest->GetPloc() == PITCHNAME_NONE) {
            // Limitation: GetLayerCount does not take into account editorial markup
            bool hasMultipleLayer = (staffY->GetLayerCount() > 1);
            bool isFirstLayer = false;
            if (hasMultipleLayer) {
                Layer *firstLayer = dynamic_cast<Layer *>(staffY->FindChildByType(LAYER));
                assert(firstLayer);
                if (firstLayer->GetN() == layerY->GetN()) isFirstLayer = true;
            }
            // We should change this and use the new PitchInterface::CalcLoc (or similar method)
            // to calculate a loc if none if provided. The use Staff::CalcPitchPosYRel to calculate the y.
            // See above for notes
            int staffLoc = 4;
            if (rest->HasLoc()) staffLoc = rest->GetLoc();
            this->SetDrawingYRel(params->m_view->CalculateRestPosY(
                staffY, rest->GetActualDur(), staffLoc, hasMultipleLayer, isFirstLayer));
        }
        else {
            int loc
                = PitchInterface::CalcLoc(rest->GetPloc(), rest->GetOloc(), layerY->GetClefLocOffset(layerElementY));
            // Override it if we have a @loc ?
            if (rest->HasLoc()) loc = rest->GetLoc();
            this->SetDrawingYRel(staffY->CalcPitchPosYRel(params->m_doc, loc));
        }
    }

    return FUNCTOR_CONTINUE;
}

int LayerElement::AdjustGraceXPos(FunctorParams *functorParams)
{
    AdjustGraceXPosParams *params = dynamic_cast<AdjustGraceXPosParams *>(functorParams);
    assert(params);

    if (!this->HasGraceAlignment()) return FUNCTOR_CONTINUE;

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
            * params->m_doc->GetDrawingUnit(params->m_doc->GetGraceSize(100)) / PARAM_DENOMINATOR;

    params->m_graceUpcomingMaxPos = std::min(selfLeft, params->m_graceUpcomingMaxPos);

    return FUNCTOR_CONTINUE;
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
        return FUNCTOR_CONTINUE;
    }

    int selfLeft;
    if (!this->HasUpdatedBB()) {
        // if nothing was drawn, do not take it into account
        assert(this->Is(BARLINE_ATTR_LEFT) || this->Is(BARLINE_ATTR_RIGHT));
        // This should happen for invis barline attribute. Otherwise the BB should be set to empty with
        // Object::SetEmptyBB()
        // LogDebug("Nothing drawn for '%s' '%s'", this->GetClassName().c_str(), this->GetUuid().c_str());
        selfLeft = this->GetAlignment()->GetXRel();
    }
    else if (this->IsFirstInLigature()) {
        // We add it to the upcoming bouding boxes
        params->m_upcomingBoundingBoxes.push_back(this);
        selfLeft = this->GetSelfLeft();
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
    if (!this->HasUpdatedBB())
        selfRight = this->GetAlignment()->GetXRel()
            + params->m_doc->GetRightMargin(this->GetClassId()) * params->m_doc->GetDrawingUnit(100)
                / PARAM_DENOMINATOR;
    else
        selfRight = this->GetSelfRight()
            + params->m_doc->GetRightMargin(this->GetClassId()) * params->m_doc->GetDrawingUnit(100)
                / PARAM_DENOMINATOR;

    params->m_upcomingMinPos = std::max(selfRight, params->m_upcomingMinPos);

    return FUNCTOR_CONTINUE;

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
    if (!durElement || !durElement->HasStaff()) {
        return FUNCTOR_CONTINUE;
    }

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
        LogWarning("Could not get the layer with cross-staff reference '%d' for element '%s'",
            durElement->GetStaff().at(0), this->GetUuid().c_str());
        m_crossStaff = NULL;
    }

    return FUNCTOR_CONTINUE;
}

int LayerElement::PrepareTimePointing(FunctorParams *functorParams)
{
    PrepareTimePointingParams *params = dynamic_cast<PrepareTimePointingParams *>(functorParams);
    assert(params);

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

} // namespace vrv
