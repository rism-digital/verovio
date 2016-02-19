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

namespace vrv {

//----------------------------------------------------------------------------
// LayerElement
//----------------------------------------------------------------------------

LayerElement::LayerElement() : DocObject("le-")
{
    Reset();
}

LayerElement::LayerElement(std::string classid) : DocObject(classid)
{
    Reset();
}

void LayerElement::Reset()
{
    DocObject::Reset();

    m_xAbs = VRV_UNSET;
    m_drawingX = 0;
    m_drawingY = 0;

    m_isScoreOrStaffDefAttr = false;
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

bool LayerElement::IsGraceNote()
{
    Note *note = dynamic_cast<Note *>(this);
    return (note && note->HasGrace());
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

int LayerElement::GetDrawingTop(Doc *doc, int staffSize)
{
    if ((this->Is() == NOTE) || (this->Is() == CHORD)) {
        DurationInterface *durationInterface = dynamic_cast<DurationInterface *>(this);
        assert(durationInterface);
        if (durationInterface->GetNoteOrChordDur(this) < DUR_2)
            return this->GetDrawingY() + doc->GetDrawingUnit(staffSize);
        // We should also take into accound the stem shift to the right
        StemmedDrawingInterface *stemmedDrawingInterface = dynamic_cast<StemmedDrawingInterface *>(this);
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

int LayerElement::GetDrawingBottom(Doc *doc, int staffSize)
{
    if ((this->Is() == NOTE) || (this->Is() == CHORD)) {
        DurationInterface *durationInterface = dynamic_cast<DurationInterface *>(this);
        assert(durationInterface);
        if (durationInterface->GetNoteOrChordDur(this) < DUR_2)
            return this->GetDrawingY() - doc->GetDrawingUnit(staffSize);
        // We should also take into accound the stem shift to the right
        StemmedDrawingInterface *stemmedDrawingInterface = dynamic_cast<StemmedDrawingInterface *>(this);
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
        Note *note = dynamic_cast<Note *>(this);
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
        DurationInterface *duration = dynamic_cast<DurationInterface *>(this);
        assert(duration);
        if (duration->IsMensural()) {
            return duration->GetAlignmentMensuralDuration(num, numbase, mensur);
        }
        double durationValue = duration->GetAlignmentDuration(num, numbase);
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
        if (meterSig) meterSig->GetUnit();
        return beatRpt->GetBeatRptAlignmentDuration(meterUnit);
    }
    else if (this->Is() == TIMESTAMP_ATTR) {
        TimestampAttr *timestampAttr = dynamic_cast<TimestampAttr *>(this);
        assert(timestampAttr);
        int meterUnit = 4;
        if (meterSig) meterSig->GetUnit();
        return timestampAttr->GetTimestampAttrAlignmentDuration(meterUnit);
    }
    else {
        return 0.0;
    }
}

int LayerElement::GetXRel()
{
    if (m_alignment) {
        return m_alignment->GetXRel();
    }
    return 0;
}

//----------------------------------------------------------------------------
// LayerElement functors methods
//----------------------------------------------------------------------------

int LayerElement::ResetHorizontalAlignment(ArrayPtrVoid *params)
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

int LayerElement::AlignHorizontally(ArrayPtrVoid *params)
{
    // param 0: the measureAligner
    // param 1: the time
    // param 2: the current Mensur
    // param 3: the current MeterSig (unused)
    MeasureAligner **measureAligner = static_cast<MeasureAligner **>((*params).at(0));
    double *time = static_cast<double *>((*params).at(1));
    Mensur **currentMensur = static_cast<Mensur **>((*params).at(2));
    MeterSig **currentMeterSig = static_cast<MeterSig **>((*params).at(3));

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
        if (this->GetScoreOrStaffDefAttr()) {
            type = ALIGNMENT_CLEF_ATTR;
        }
        else {
            type = ALIGNMENT_CLEF;
        }
    }
    else if (this->Is() == KEYSIG) {
        if (this->GetScoreOrStaffDefAttr()) {
            type = ALIGNMENT_KEYSIG_ATTR;
        }
        else {
            // type = ALIGNMENT_KEYSIG;
            // We force this because they should appear only at the beginning of a measure and should be non-justifiable
            // We also need it because the PAE importer creates keySig (and not staffDef @key.sig)
            type = ALIGNMENT_KEYSIG_ATTR;
        }
    }
    else if (this->Is() == MENSUR) {
        if (this->GetScoreOrStaffDefAttr()) {
            type = ALIGNMENT_MENSUR_ATTR;
        }
        else {
            // replace the current mensur
            (*currentMensur) = dynamic_cast<Mensur *>(this);
            assert(*currentMensur);
            type = ALIGNMENT_MENSUR;
        }
    }
    else if (this->Is() == METERSIG) {
        if (this->GetScoreOrStaffDefAttr()) {
            type = ALIGNMENT_METERSIG_ATTR;
        }
        else {
            // replace the current meter signature
            (*currentMeterSig) = dynamic_cast<MeterSig *>(this);
            assert(*currentMeterSig);
            // type = ALIGNMENT_METERSIG
            // We force this because they should appear only at the beginning of a measure and should be non-justifiable
            // We also need it because the PAE importer creates meterSig (and not staffDef @meter)
            type = ALIGNMENT_METERSIG_ATTR;
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
    else if ((this->Is() == BEAM) || (this->Is() == TUPLET) || (this->Is() == VERSE) || (this->Is() == SYL)) {
        type = ALIGNMENT_CONTAINER;
    }
    else if (this->Is() == DOT) {
        type = ALIGNMENT_DOT;
    }
    else if (this->Is() == ACCID) {
        type = ALIGNMENT_ACCID;
    }

    // get the duration of the event
    double duration = this->GetAlignmentDuration(*currentMensur);

    // For timestamp, what we get from GetAlignmentDuration is actually the position of the timestamp
    // So use it as current time - we can do this because the timestamp loop is redirected from the measure
    // The time will be reset to 0.0 when starting a new layer anyway
    if (this->Is() == TIMESTAMP_ATTR)
        (*time) = duration;
    else
        (*measureAligner)->SetMaxTime((*time) + duration);

    m_alignment = (*measureAligner)->GetAlignmentAtTime(*time, type);

    if (this->IsGraceNote()) {
        GraceAligner *graceAligner = m_alignment->GetGraceAligner();
        graceAligner->StackNote(dynamic_cast<Note *>(this));
    }

    // LogDebug("AlignHorizontally: Time %f - %s", (*time), this->GetClassName().c_str());

    // increase the time position, but only when not a timestamp (it would actually do nothing)
    if (this->Is() != TIMESTAMP_ATTR) {
        (*time) += duration;
    }

    return FUNCTOR_CONTINUE;
}

int LayerElement::PrepareTimeSpanning(ArrayPtrVoid *params)
{
    // param 0: std::vector<DocObject*>* that holds the current elements to match
    // param 1: bool* fillList for indicating whether the elements have to be stacked or not (unused)
    std::vector<DocObject *> *elements = static_cast<std::vector<DocObject *> *>((*params).at(0));

    std::vector<DocObject *>::iterator iter = elements->begin();
    while (iter != elements->end()) {
        TimeSpanningInterface *interface = dynamic_cast<TimeSpanningInterface *>(*iter);
        assert(interface);
        if (interface->SetStartAndEnd(this)) {
            // Check that there is no timestamp?
            iter = elements->erase(iter);
        }
        else {
            iter++;
        }
    }

    return FUNCTOR_CONTINUE;
}

int LayerElement::SetDrawingXY(ArrayPtrVoid *params)
{
    // param 0: a pointer doc
    // param 1: a pointer to the current system (unused)
    // param 2: a pointer to the current measure
    // param 3: a pointer to the current staff
    // param 4: a pointer to the current layer
    // param 5: a pointer to the view
    // param 6: a bool indicating if we are processing layer elements or not
    // param 7: a pointer to the functor for passing it to the timestamps (unused)
    Doc *doc = static_cast<Doc *>((*params).at(0));
    Measure **currentMeasure = static_cast<Measure **>((*params).at(2));
    Staff **currentStaff = static_cast<Staff **>((*params).at(3));
    Layer **currentLayer = static_cast<Layer **>((*params).at(4));
    View *view = static_cast<View *>((*params).at(5));
    bool *processLayerElements = static_cast<bool *>((*params).at(6));

    // First pass, only set the X position
    if ((*processLayerElements) == false) {
        // Here we set the appropriate x value to be used for drawing
        // With Raw documents, we use m_drawingXRel that is calculated by the layout algorithm
        // With Transcription documents, we use the m_xAbs
        if (this->m_xAbs == VRV_UNSET) {
            assert(doc->GetType() == Raw);
            this->SetDrawingX(this->GetXRel() + (*currentMeasure)->GetDrawingX());
            // Grace notes, also take into account the GraceAlignment
            Note *note = dynamic_cast<Note *>(this);
            if (note && note->HasGraceAlignment()) {
                this->SetDrawingX(this->GetDrawingX() - note->GetAlignment()->GetGraceAligner()->GetWidth()
                    + note->GetGraceAlignment()->GetXRel());
            }
        }
        else {
            assert(doc->GetType() == Transcription);
            this->SetDrawingX(this->m_xAbs);
        }
        return FUNCTOR_CONTINUE;
    }

    LayerElement *layerElementY = this;

    // Look for cross-staff situations
    // If we have one, make is available in m_crossStaff
    DurationInterface *durElement = dynamic_cast<DurationInterface *>(this);
    if (durElement && durElement->HasStaff()) {
        AttCommonNComparison comparisonFirst(STAFF, durElement->GetStaff().at(0));
        m_crossStaff = dynamic_cast<Staff *>((*currentMeasure)->FindChildByAttComparison(&comparisonFirst, 1));
        if (m_crossStaff) {
            if (m_crossStaff == (*currentStaff))
                LogWarning("The cross staff reference '%d' for element '%s' seems to be identical to the parent staff",
                    durElement->GetStaff().at(0), this->GetUuid().c_str());
            // Now try to get the corresponding layer - for now look for the same layer @n
            int layerN = (*currentLayer)->GetN();
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

    Staff *staffY = m_crossStaff ? m_crossStaff : (*currentStaff);
    Layer *layerY = m_crossLayer ? m_crossLayer : (*currentLayer);

    // Here we set the appropriate Y value to be used for drawing
    if (this->m_xAbs == VRV_UNSET) {
        assert(doc->GetType() == Raw);
        this->SetDrawingY(staffY->GetDrawingY());
    }
    else {
        assert(doc->GetType() == Transcription);
        this->SetDrawingY(staffY->GetDrawingY());
    }

    // Finally, adjust Y for notes and rests
    if (this->Is() == NOTE) {
        Note *note = dynamic_cast<Note *>(this);
        assert(note);
        this->SetDrawingY(this->GetDrawingY()
            + view->CalculatePitchPosY(staffY, note->GetPname(), layerY->GetClefOffset(layerElementY), note->GetOct()));
    }
    else if (this->Is() == REST) {
        Rest *rest = dynamic_cast<Rest *>(this);
        assert(rest);
        // Automatically calculate rest position, if so requested
        if (rest->GetPloc() == PITCHNAME_NONE) {
            this->SetDrawingY(this->GetDrawingY() + view->CalculateRestPosY(staffY, rest->GetActualDur()));
        }
        else {
            this->SetDrawingY(this->GetDrawingY()
                + view->CalculatePitchPosY(
                      staffY, rest->GetPloc(), layerY->GetClefOffset(layerElementY), rest->GetOloc()));
        }
    }

    // For avoiding unused variable warning in non debug mode
    doc = NULL;

    return FUNCTOR_CONTINUE;
}

int LayerElement::TimeSpanningLayerElements(ArrayPtrVoid *params)
{
    // param 0: a pointer to the vector of LayerElement pointer to fill
    // param 1: the minimum position
    // param 2: the maximum position
    std::vector<LayerElement *> *spanningContent = static_cast<std::vector<LayerElement *> *>((*params).at(0));
    int *min_pos = static_cast<int *>((*params).at(1));
    int *max_pos = static_cast<int *>((*params).at(2));

    if (this->GetDrawingX() > (*min_pos) && this->GetDrawingX() < (*max_pos)) {
        spanningContent->push_back(this);
    }
    else if (this->GetDrawingX() > (*max_pos)) {
        return FUNCTOR_STOP;
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
