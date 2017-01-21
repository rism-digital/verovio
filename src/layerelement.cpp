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
    m_drawingX = 0;
    m_drawingY = 0;

    m_scoreDefRole = NONE;
    m_alignment = NULL;
    m_beamElementCoord = NULL;
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

bool LayerElement::IsGraceNote() const
{
    if (this->Is() != NOTE) return false;
    Note const *note = dynamic_cast<Note const *>(this);
    assert(note);
    return (note && note->HasGrace());
}

bool LayerElement::IsInLigature()
{
    if (this->Is() != NOTE) return false;
    return (this->GetFirstParent(LIGATURE, MAX_LIGATURE_DEPTH));
}

bool LayerElement::IsInFTrem()
{
    if ((this->Is() != NOTE) || (this->Is() == CHORD)) return false;
    return (this->GetFirstParent(FTREM, MAX_FTREM_DEPTH));
}

Beam *LayerElement::IsInBeam()
{
    if ((this->Is() != NOTE) && (this->Is() != CHORD)) return NULL;
    Beam *beamParent = dynamic_cast<Beam *>(this->GetFirstParent(BEAM, MAX_BEAM_DEPTH));
    if (beamParent != NULL) {
        // This note is beamed and cue-sized
        if (this->IsCueSize()) {
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

int LayerElement::GetDrawingArticulationTopOrBottom(data_STAFFREL place, ArticPartType type)
{
    // It would not crash otherwise but there is not reason to call it
    assert((this->Is() == NOTE) || (this->Is() == CHORD));

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

int LayerElement::GetDrawingTop(Doc *doc, int staffSize, bool withArtic, ArticPartType type)
{
    if ((this->Is() == NOTE) || (this->Is() == CHORD)) {
        if (withArtic) {
            int articY = GetDrawingArticulationTopOrBottom(STAFFREL_above, type);
            if (articY != VRV_UNSET) return articY;
        }
        DurationInterface *durationInterface = this->GetDurationInterface();
        assert(durationInterface);
        if (durationInterface->GetNoteOrChordDur(this) < DUR_2) {
            if (this->Is() == CHORD) {
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
    if ((this->Is() == NOTE) || (this->Is() == CHORD)) {
        if (withArtic) {
            int articY = GetDrawingArticulationTopOrBottom(STAFFREL_below, type);
            if (articY != -VRV_UNSET) return articY;
        }
        DurationInterface *durationInterface = this->GetDurationInterface();
        assert(durationInterface);
        if (durationInterface->GetNoteOrChordDur(this) < DUR_2) {
            if (this->Is() == CHORD) {
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

bool LayerElement::IsCueSize()
{
    if (this->Is() == NOTE) {
        const Note *note = dynamic_cast<const Note *>(this);
        assert(note);
        return (note->HasGrace());
    }
    Note *note = dynamic_cast<Note *>(this->GetFirstParent(NOTE, MAX_ACCID_DEPTH));
    return (note && (note->HasGrace()));
}

void LayerElement::AdjustPname(int *pname, int *oct)
{
    if ((*pname) < PITCHNAME_c) {
        if ((*oct) > 0) (*oct)--;
        (*pname) = PITCHNAME_b;
    }
    else if ((*pname) > PITCHNAME_b) {
        if ((*oct) < 7) (*oct)++;
        (*pname) = PITCHNAME_c;
    }
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
    else if (this->Is() == BEATRPT) {
        BeatRpt *beatRpt = dynamic_cast<BeatRpt *>(this);
        assert(beatRpt);
        int meterUnit = 4;
        if (meterSig && meterSig->HasUnit()) meterSig->GetUnit();
        return beatRpt->GetBeatRptAlignmentDuration(meterUnit);
    }
    else if (this->Is() == TIMESTAMP_ATTR) {
        TimestampAttr *timestampAttr = dynamic_cast<TimestampAttr *>(this);
        assert(timestampAttr);
        int meterUnit = 4;
        if (meterSig && meterSig->HasUnit()) meterUnit = meterSig->GetUnit();
        return timestampAttr->GetTimestampAttrAlignmentDuration(meterUnit);
    }
    else {
        return 0.0;
    }
}

int LayerElement::GetXRel() const
{
    if (m_alignment) {
        return m_alignment->GetXRel();
    }
    return 0;
}

//----------------------------------------------------------------------------
// LayerElement functors methods
//----------------------------------------------------------------------------

int LayerElement::ResetHorizontalAlignment(FunctorParams *functorParams)
{
    m_drawingX = 0;
    m_alignment = NULL;
    if (this->Is() == NOTE) {
        Note *note = dynamic_cast<Note *>(this);
        assert(note);
        note->ResetGraceAlignment();
    }

    return FUNCTOR_CONTINUE;
}

int LayerElement::ResetVerticalAlignment(FunctorParams *functorParams)
{
    m_drawingY = 0;

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
    if (this->Is() == BARLINE) {
        type = ALIGNMENT_BARLINE;
    }
    else if (this->Is() == CLEF) {
        if ((this->GetScoreDefRole() == SYSTEM_SCOREDEF) || (this->GetScoreDefRole() == INTERMEDIATE_SCOREDEF))
            type = ALIGNMENT_SCOREDEF_CLEF;
        else if (this->GetScoreDefRole() == CAUTIONARY_SCOREDEF)
            type = ALIGNMENT_SCOREDEF_CAUTION_CLEF;
        else {
            type = ALIGNMENT_CLEF;
        }
    }
    else if (this->Is() == KEYSIG) {
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
    else if (this->Is() == MENSUR) {
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
    else if (this->Is() == METERSIG) {
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
    else if ((this->Is() == MULTIREST) || (this->Is() == MREST) || (this->Is() == MRPT)) {
        type = ALIGNMENT_FULLMEASURE;
    }
    else if ((this->Is() == MRPT2) || (this->Is() == MULTIRPT)) {
        type = ALIGNMENT_FULLMEASURE2;
    }
    else if (this->IsGraceNote()) {
        type = ALIGNMENT_GRACENOTE;
    }
    else if ((this->Is() == BEAM) || (this->Is() == TUPLET)) {
        type = ALIGNMENT_CONTAINER;
    }
    else if (this->Is() == DOT) {
        type = ALIGNMENT_DOT;
    }
    else if (this->Is() == ACCID) {
        type = ALIGNMENT_ACCID;
    }
    else if ((this->Is() == SYL) || (this->Is() == ARTIC) || (this->Is() == ARTIC_PART)) {
        // Refer to the note parent
        Note *note = dynamic_cast<Note *>(this->GetFirstParent(NOTE));
        assert(note);
        m_alignment = note->GetAlignment();
        return FUNCTOR_CONTINUE;
    }
    else if (this->Is() == VERSE) {
        // Idem
        Note *note = dynamic_cast<Note *>(this->GetFirstParent(NOTE));
        assert(note);
        m_alignment = note->GetAlignment();
        return FUNCTOR_CONTINUE;
    }

    // get the duration of the event
    double duration = this->GetAlignmentDuration(params->m_currentMensur, params->m_currentMeterSig);

    // For timestamp, what we get from GetAlignmentDuration is actually the position of the timestamp
    // So use it as current time - we can do this because the timestamp loop is redirected from the measure
    // The time will be reset to 0.0 when starting a new layer anyway
    if (this->Is() == TIMESTAMP_ATTR)
        params->m_time = duration;
    else
        params->m_measureAligner->SetMaxTime(params->m_time + duration);

    m_alignment = params->m_measureAligner->GetAlignmentAtTime(params->m_time, type);
    m_alignment->AddLayerElementRef(this);

    if (this->IsGraceNote()) {
        GraceAligner *graceAligner = m_alignment->GetGraceAligner();
        // We know that this is a note
        graceAligner->StackNote(dynamic_cast<Note *>(this));
    }

    // LogDebug("AlignHorizontally: Time %f - %s", (*time), this->GetClassName().c_str());

    // increase the time position, but only when not a timestamp (it would actually do nothing)
    if (this->Is() != TIMESTAMP_ATTR) {
        params->m_time += duration;
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

int LayerElement::SetDrawingXY(FunctorParams *functorParams)
{
    SetDrawingXYParams *params = dynamic_cast<SetDrawingXYParams *>(functorParams);
    assert(params);

    // First pass, only set the X position
    if (params->m_processLayerElements == false) {
        // Here we set the appropriate x value to be used for drawing
        // With Raw documents, we use m_drawingXRel that is calculated by the layout algorithm
        // With Transcription documents, we use the m_xAbs
        if (this->m_xAbs == VRV_UNSET) {
            assert(params->m_doc->GetType() == Raw);
            this->SetDrawingX(this->GetXRel() + params->m_currentMeasure->GetDrawingX());
            // Grace notes, also take into account the GraceAlignment
            if (this->Is() == NOTE) {
                Note *note = dynamic_cast<Note *>(this);
                assert(note);
                if (note->HasGraceAlignment()) {
                    this->SetDrawingX(this->GetDrawingX() - note->GetAlignment()->GetGraceAligner()->GetWidth()
                        + note->GetGraceAlignment()->GetXRel());
                }
            }
        }
        else {
            assert(params->m_doc->GetType() == Transcription);
            this->SetDrawingX(this->m_xAbs);
        }
        return FUNCTOR_CONTINUE;
    }

    LayerElement *layerElementY = this;

    // Look for cross-staff situations
    // If we have one, make is available in m_crossStaff
    DurationInterface *durElement = this->GetDurationInterface();
    if (durElement && durElement->HasStaff()) {
        AttCommonNComparison comparisonFirst(STAFF, durElement->GetStaff().at(0));
        m_crossStaff = dynamic_cast<Staff *>(params->m_currentMeasure->FindChildByAttComparison(&comparisonFirst, 1));
        if (m_crossStaff) {
            if (m_crossStaff == params->m_currentStaff)
                LogWarning("The cross staff reference '%d' for element '%s' seems to be identical to the parent staff",
                    durElement->GetStaff().at(0), this->GetUuid().c_str());
            // Now try to get the corresponding layer - for now look for the same layer @n
            int layerN = params->m_currentLayer->GetN();
            // When we will have allowed @layer in <note>, we will have to do:
            // int layerN = durElement->HasLayer() ? durElement->GetLayer() : (*currentLayer)->GetN();
            AttCommonNComparison comparisonFirstLayer(LAYER, layerN);
            m_crossLayer = dynamic_cast<Layer *>(m_crossStaff->FindChildByAttComparison(&comparisonFirstLayer, 1));
            if (m_crossLayer) {
                // Now we need to yet the element at the same position in the cross-staff layer of getting the right
                // clef
                layerElementY = m_crossLayer->GetAtPos(this->GetDrawingX());
            }
            else {
                LogWarning("Could not get the layer with cross-staff reference '%d' for element '%s'",
                    durElement->GetStaff().at(0), this->GetUuid().c_str());
            }
        }
        else {
            LogWarning("Could not get the cross staff reference '%d' for element '%s'", durElement->GetStaff().at(0),
                this->GetUuid().c_str());
        }
        // If we have a @layer we probably also want to change the layer element (for getting the right clef if
        // different)
    }
    else {
        m_crossStaff = NULL;
        m_crossLayer = NULL;
    }

    Staff *staffY = m_crossStaff ? m_crossStaff : params->m_currentStaff;
    Layer *layerY = m_crossLayer ? m_crossLayer : params->m_currentLayer;

    // Here we set the appropriate Y value to be used for drawing
    if (this->m_xAbs == VRV_UNSET) {
        assert(params->m_doc->GetType() == Raw);
        this->SetDrawingY(staffY->GetDrawingY());
    }
    else {
        assert(params->m_doc->GetType() == Transcription);
        this->SetDrawingY(staffY->GetDrawingY());
    }

    // Finally, adjust Y for notes and rests
    if (this->Is() == NOTE) {
        Note *note = dynamic_cast<Note *>(this);
        this->SetDrawingY(this->GetDrawingY()
            + params->m_view->CalculatePitchPosY(
                  staffY, note->GetPname(), layerY->GetClefOffset(layerElementY), note->GetOct()));
    }
    else if (this->Is() == REST) {
        Rest *rest = dynamic_cast<Rest *>(this);
        assert(rest);
        // Automatically calculate rest position, if so requested
        if (rest->GetPloc() == PITCHNAME_NONE) {
            bool hasMultipleLayer = (staffY->GetLayerCount() > 1);
            bool isFirstLayer = false;
            if (hasMultipleLayer) {
                Layer *firstLayer = dynamic_cast<Layer *>(staffY->FindChildByType(LAYER));
                assert(firstLayer);
                if (firstLayer->GetN() == layerY->GetN()) isFirstLayer = true;
            }
            this->SetDrawingY(this->GetDrawingY()
                + params->m_view->CalculateRestPosY(staffY, rest->GetActualDur(), hasMultipleLayer, isFirstLayer));
        }
        else {
            this->SetDrawingY(this->GetDrawingY()
                + params->m_view->CalculatePitchPosY(
                      staffY, rest->GetPloc(), layerY->GetClefOffset(layerElementY), rest->GetOloc()));
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
    if (this->Is() == REST) {
        // Rest *rest = dynamic_cast<Rest *>(this);
        // assert(rest);
        // LogMessage("Rest %f", GetAlignmentDuration());
        // increase the currentTime accordingly
        params->m_currentMeasureTime += GetAlignmentDuration() * params->m_currentBpm / (DUR_MAX / DURATION_4);
    }
    else if (this->Is() == NOTE) {
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
        if (note->HasAccidGes()) {
            data_ACCIDENTAL_IMPLICIT acc_imp = note->GetAccidGes();
            switch (acc_imp) {
                case ACCIDENTAL_IMPLICIT_s: midiBase += 1; break;
                case ACCIDENTAL_IMPLICIT_f: midiBase -= 1; break;
                case ACCIDENTAL_IMPLICIT_ss: midiBase += 2; break;
                case ACCIDENTAL_IMPLICIT_ff: midiBase -= 2; break;
                default: break;
            }
        }
        else if (note->m_drawingAccid) {
            data_ACCIDENTAL_EXPLICIT acc_exp = note->m_drawingAccid->GetAccid();
            switch (acc_exp) {
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
    else if (this->Is() == SPACE) {
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

    if (this->Is() == CHORD) {
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

    if (this->Is() == NOTE) {
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
