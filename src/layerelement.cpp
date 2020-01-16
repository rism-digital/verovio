/////////////////////////////////////////////////////////////////////////////
// Name:        layerelement.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "accid.h"
#include "barline.h"
#include "beam.h"
#include "beatrpt.h"
#include "btrem.h"
#include "chord.h"
#include "clef.h"
#include "comparison.h"
#include "custos.h"
#include "doc.h"
#include "dot.h"
#include "ftrem.h"
#include "functorparams.h"
#include "horizontalaligner.h"
#include "keysig.h"
#include "layer.h"
#include "measure.h"
#include "mensur.h"
#include "metersig.h"
#include "mrest.h"
#include "mrpt.h"
#include "mrpt2.h"
#include "multirest.h"
#include "multirpt.h"
#include "neume.h"
#include "note.h"
#include "page.h"
#include "rest.h"
#include "slur.h"
#include "smufl.h"
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

LayerElement::LayerElement() : Object("le-"), FacsimileInterface(), LinkingInterface(), AttLabelled(), AttTyped()
{
    RegisterInterface(FacsimileInterface::GetAttClasses(), FacsimileInterface::IsInterface());
    RegisterInterface(LinkingInterface::GetAttClasses(), LinkingInterface::IsInterface());
    RegisterAttClass(ATT_LABELLED);
    RegisterAttClass(ATT_TYPED);

    Reset();
}

LayerElement::LayerElement(std::string classid)
    : Object(classid), FacsimileInterface(), LinkingInterface(), AttLabelled(), AttTyped()
{
    RegisterInterface(FacsimileInterface::GetAttClasses(), FacsimileInterface::IsInterface());
    RegisterInterface(LinkingInterface::GetAttClasses(), LinkingInterface::IsInterface());
    RegisterAttClass(ATT_LABELLED);
    RegisterAttClass(ATT_TYPED);

    Reset();
}

void LayerElement::Reset()
{
    Object::Reset();
    FacsimileInterface::Reset();
    LinkingInterface::Reset();
    ResetLabelled();
    ResetTyped();

    m_xAbs = VRV_UNSET;
    m_drawingYRel = 0;
    m_drawingXRel = 0;
    m_drawingCueSize = false;

    m_scoreDefRole = SCOREDEF_NONE;
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

void LayerElement::CloneReset()
{
    Object::CloneReset();

    // pointers have to be NULL
    m_alignment = NULL;
    m_graceAlignment = NULL;
    m_alignmentLayerN = VRV_UNSET;
    m_beamElementCoord = NULL;

    m_crossStaff = NULL;
    m_crossLayer = NULL;
}

LayerElement *LayerElement::ThisOrSameasAsLink()
{
    if (!this->HasSameasLink()) {
        return this;
    }

    assert(this->GetSameasLink());

    return dynamic_cast<LayerElement *>(this->GetSameasLink());
}

bool LayerElement::IsGraceNote()
{
    // For note, we need to look at it or at the parent chord
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
        ClassIdsComparison matchType({ NOTE, CHORD });
        ArrayOfObjects children;
        LayerElement *child = dynamic_cast<LayerElement *>(this->FindDescendantByComparison(&matchType));
        if (child) return child->IsGraceNote();
    }
    // For accid, artic, etc.. look at the parent note / chord
    else {
        // For an accid we expect to be the child of a note - the note will lookup at the chord parent in necessary
        Note *note = dynamic_cast<Note *>(this->GetFirstAncestor(NOTE, MAX_ACCID_DEPTH));
        if (note) return note->IsGraceNote();
        // For an artic we can be direct child of a chord
        Chord *chord = dynamic_cast<Chord *>(this->GetFirstAncestor(CHORD, MAX_ACCID_DEPTH));
        if (chord) return chord->IsGraceNote();
    }
    return false;
}

bool LayerElement::GetDrawingCueSize()
{
    return m_drawingCueSize;
}

bool LayerElement::IsInLigature() const
{
    if (!this->Is(NOTE)) return false;
    return (this->GetFirstAncestor(LIGATURE, MAX_LIGATURE_DEPTH));
}

bool LayerElement::IsInFTrem()
{
    if (!this->Is({ CHORD, NOTE })) return false;
    return (this->GetFirstAncestor(FTREM, MAX_FTREM_DEPTH));
}

Beam *LayerElement::IsInBeam()
{
    if (!this->Is({ CHORD, NOTE, STEM })) return NULL;
    Beam *beamParent = dynamic_cast<Beam *>(this->GetFirstAncestor(BEAM, MAX_BEAM_DEPTH));
    if (beamParent != NULL) {
        // This note is beamed and cue-sized - we will be able to get rid of this once MEI has a better modeling for
        // beamed grace notes
        if (this->IsGraceNote()) {
            LayerElement *graceNote = this;
            if (this->Is(STEM)) graceNote = dynamic_cast<LayerElement *>(this->GetFirstAncestor(NOTE, MAX_BEAM_DEPTH));
            // Make sure the object list is set
            beamParent->GetList(beamParent);
            // If the note is part of the beam parent, this means we have a beam of graced notes
            if (beamParent->GetListIndex(graceNote) > -1) {
                return beamParent;
            }
            // otherwise it is a non-beamed grace note within a beam - return NULL
            else {
                return NULL;
            }
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

    LayerElement *parent
        = dynamic_cast<LayerElement *>(this->GetFirstAncestorInRange(LAYER_ELEMENT, LAYER_ELEMENT_max));

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
    // If this element has a facsimile and we are in facsimile mode, use Facsimile::GetDrawingX
    if (this->HasFacs()) {
        Doc *doc = dynamic_cast<Doc *>(this->GetFirstAncestor(DOC));
        assert(doc);
        if (doc->GetType() == Facs) {
            return FacsimileInterface::GetDrawingX();
        }
    }

    // Since m_xAbs is the left position, we adjust the XRel accordingly in AdjustXRelForTranscription
    if (m_xAbs != VRV_UNSET) return m_xAbs + this->GetDrawingXRel();

    if (m_cachedDrawingX != VRV_UNSET) return m_cachedDrawingX;

    if (!m_alignment) {
        // assert(this->Is({ BEAM, FTREM, TUPLET }));
        // Here we just get the measure position - no cast to Measure is necessary
        Object *measure = this->GetFirstAncestor(MEASURE);
        assert(measure);
        m_cachedDrawingX = measure->GetDrawingX();
        return m_cachedDrawingX;
    }

    // First get the first layerElement parent (if any) and use its position if they share the same alignment
    LayerElement *parent
        = dynamic_cast<LayerElement *>(this->GetFirstAncestorInRange(LAYER_ELEMENT, LAYER_ELEMENT_max));
    if (parent && (parent->GetAlignment() == this->GetAlignment())) {
        m_cachedDrawingX = (parent->GetDrawingX() + this->GetDrawingXRel());
        return m_cachedDrawingX;
    }

    // Otherwise get the measure - no cast to Measure is necessary
    Object *measure = this->GetFirstAncestor(MEASURE);
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
    // If this element has a facsimile and we are in facsimile mode, use Facsimile::GetDrawingY
    if (this->HasFacs()) {
        Doc *doc = dynamic_cast<Doc *>(this->GetFirstAncestor(DOC));
        assert(doc);
        if (doc->GetType() == Facs) {
            return FacsimileInterface::GetDrawingY();
        }
    }

    if (m_cachedDrawingY != VRV_UNSET) return m_cachedDrawingY;

    // Look if we have a crossStaff situation
    Object *object = this->m_crossStaff; // GetCrossStaff();
    // First get the first layerElement parent (if any) but only if the element is not directly relative to staff (e.g.,
    // artic, syl)
    if (!object && !this->IsRelativeToStaff()) object = this->GetFirstAncestorInRange(LAYER_ELEMENT, LAYER_ELEMENT_max);
    // Otherwise get the first staff
    if (!object) object = this->GetFirstAncestor(STAFF);
    // Otherwise the first measure (this is the case with barLineAttr
    if (!object) object = this->GetFirstAncestor(MEASURE);

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
    Artic *firstArtic = dynamic_cast<Artic *>(this->FindDescendantByType(ARTIC));
    Artic *lastArtic = dynamic_cast<Artic *>(this->FindDescendantByType(ARTIC, MAX_ACCID_DEPTH, BACKWARD));
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

    Measure *measure = dynamic_cast<Measure *>(this->GetFirstAncestor(MEASURE));
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

int LayerElement::GetDrawingRadius(Doc *doc)
{
    assert(doc);

    if (!this->Is({ CHORD, NOTE, REST })) return 0;

    wchar_t code = 0;

    int dur = DUR_4;
    if (this->Is(NOTE)) {
        Note *note = dynamic_cast<Note *>(this);
        assert(note);
        dur = note->GetDrawingDur();
        if (note->IsMensural()) {
            code = note->GetMensuralSmuflNoteHead();
        }
    }
    else if (this->Is(CHORD)) {
        Chord *chord = dynamic_cast<Chord *>(this);
        assert(chord);
        dur = chord->GetActualDur();
    }

    Staff *staff = dynamic_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff);
    // Mensural note shorter than DUR_BR
    if (code) {
        return doc->GetGlyphWidth(code, staff->m_drawingStaffSize, this->GetDrawingCueSize()) / 2;
    }
    else if (dur <= DUR_BR) {
        if (staff->m_drawingNotationType == NOTATIONTYPE_mensural_black) {
            return doc->GetDrawingBrevisWidth(staff->m_drawingStaffSize) * 0.8;
        }
        else {
            return doc->GetDrawingBrevisWidth(staff->m_drawingStaffSize);
        }
    }
    else if (dur == DUR_1) {
        return doc->GetGlyphWidth(SMUFL_E0A2_noteheadWhole, staff->m_drawingStaffSize, this->GetDrawingCueSize()) / 2;
    }
    else if (dur == DUR_2) {
        return doc->GetGlyphWidth(SMUFL_E0A3_noteheadHalf, staff->m_drawingStaffSize, this->GetDrawingCueSize()) / 2;
    }
    else {
        return doc->GetGlyphWidth(SMUFL_E0A4_noteheadBlack, staff->m_drawingStaffSize, this->GetDrawingCueSize()) / 2;
    }
}

double LayerElement::GetAlignmentDuration(
    Mensur *mensur, MeterSig *meterSig, bool notGraceOnly, data_NOTATIONTYPE notationType)
{
    if (this->IsGraceNote() && notGraceOnly) {
        return 0.0;
    }

    if (this->HasSameasLink() && this->GetSameasLink()->IsLayerElement()) {
        LayerElement *sameas = dynamic_cast<LayerElement *>(this->GetSameasLink());
        assert(sameas);
        return sameas->GetAlignmentDuration(mensur, meterSig, notGraceOnly, notationType);
    }
    else if (this->HasInterface(INTERFACE_DURATION)) {
        int num = 1;
        int numbase = 1;
        Tuplet *tuplet = dynamic_cast<Tuplet *>(this->GetFirstAncestor(TUPLET, MAX_TUPLET_DEPTH));
        if (tuplet) {
            num = tuplet->GetNum();
            numbase = tuplet->GetNumbase();
            // 0 is not valid in MEI anyway - just correct it silently
            if (num == 0) num = 1;
            if (numbase == 0) numbase = 1;
        }
        DurationInterface *duration = this->GetDurationInterface();
        assert(duration);
        if (duration->IsMensural() && (notationType != NOTATIONTYPE_cmn)) {
            return duration->GetInterfaceAlignmentMensuralDuration(num, numbase, mensur);
        }
        if (this->Is(NC)) {
            Neume *neume = dynamic_cast<Neume *>(this->GetFirstAncestor(NEUME));
            if (neume->IsLastInNeume(this)) {
                return 128;
            }
            else {
                return 16;
            }
        }
        double durationValue = duration->GetInterfaceAlignmentDuration(num, numbase);
        // With fTrem we need to divide the duration by two
        FTrem *fTrem = dynamic_cast<FTrem *>(this->GetFirstAncestor(FTREM, MAX_FTREM_DEPTH));
        if (fTrem) {
            durationValue /= 2.0;
        }
        return durationValue;
    }
    else if (this->Is(BEATRPT)) {
        BeatRpt *beatRpt = dynamic_cast<BeatRpt *>(this);
        assert(beatRpt);
        int meterUnit = 4;
        if (meterSig && meterSig->HasUnit()) meterUnit = meterSig->GetUnit();
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
    else if (this->Is({ HALFMRPT, MREST, MULTIREST, MRPT, MRPT2, MULTIRPT })) {
        int meterUnit = 4;
        int meterCount = 4;
        if (meterSig && meterSig->HasUnit()) meterUnit = meterSig->GetUnit();
        if (meterSig && meterSig->HasCount()) meterCount = meterSig->GetCount();

        if (this->Is(HALFMRPT)) {
            return (DUR_MAX / meterUnit * meterCount) / 2;
        }
        else {
            return DUR_MAX / meterUnit * meterCount;
        }
    }
    else {
        return 0.0;
    }
}

double LayerElement::GetContentAlignmentDuration(
    Mensur *mensur, MeterSig *meterSig, bool notGraceOnly, data_NOTATIONTYPE notationType)
{
    if (!this->HasSameasLink() || !this->GetSameasLink()->Is({ BEAM, FTREM, TUPLET })) {
        return 0.0;
    }

    double duration = 0.0;

    LayerElement *sameas = dynamic_cast<LayerElement *>(this->GetSameasLink());
    assert(sameas);

    for (auto child : *sameas->GetChildren()) {
        // Skip everything that does not have a duration interface and notes in chords
        if (!child->HasInterface(INTERFACE_DURATION) || (child->GetFirstAncestor(CHORD, MAX_CHORD_DEPTH) != NULL)) {
            continue;
        }
        LayerElement *element = dynamic_cast<LayerElement *>(child);
        assert(element);
        duration += element->GetAlignmentDuration(mensur, meterSig, notGraceOnly, notationType);
    }

    return duration;
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

int LayerElement::ApplyPPUFactor(FunctorParams *functorParams)
{
    ApplyPPUFactorParams *params = dynamic_cast<ApplyPPUFactorParams *>(functorParams);
    assert(params);

    if (this->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    if (m_xAbs != VRV_UNSET) m_xAbs /= params->m_page->GetPPUFactor();

    return FUNCTOR_CONTINUE;
}

int LayerElement::AlignHorizontally(FunctorParams *functorParams)
{
    AlignHorizontallyParams *params = dynamic_cast<AlignHorizontallyParams *>(functorParams);
    assert(params);

    // if (m_alignment) LogDebug("Element %s %s", this->GetUuid().c_str(), this->GetClassName().c_str());
    assert(!m_alignment);

    if (this->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    this->SetScoreDefRole(params->m_scoreDefRole);

    AlignmentType type = ALIGNMENT_DEFAULT;

    Chord *chordParent = dynamic_cast<Chord *>(this->GetFirstAncestor(CHORD, MAX_CHORD_DEPTH));
    Note *noteParent = dynamic_cast<Note *>(this->GetFirstAncestor(NOTE, MAX_NOTE_DEPTH));
    Rest *restParent = dynamic_cast<Rest *>(this->GetFirstAncestor(REST, MAX_NOTE_DEPTH));

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
    else if (this->Is({ BEAM, LIGATURE, FTREM, TUPLET })) {
        double duration = this->GetContentAlignmentDuration(
            params->m_currentMensur, params->m_currentMeterSig, true, params->m_notationType);
        params->m_time += duration;
        return FUNCTOR_CONTINUE;
    }
    else if (this->Is(BARLINE)) {
        type = ALIGNMENT_BARLINE;
    }
    else if (this->Is(CLEF)) {
        if ((this->GetScoreDefRole() == SCOREDEF_SYSTEM) || (this->GetScoreDefRole() == SCOREDEF_INTERMEDIATE))
            type = ALIGNMENT_SCOREDEF_CLEF;
        else if (this->GetScoreDefRole() == SCOREDEF_CAUTIONARY)
            type = ALIGNMENT_SCOREDEF_CAUTION_CLEF;
        else {
            type = ALIGNMENT_CLEF;
        }
    }
    else if (this->Is(KEYSIG)) {
        if ((this->GetScoreDefRole() == SCOREDEF_SYSTEM) || (this->GetScoreDefRole() == SCOREDEF_INTERMEDIATE))
            type = ALIGNMENT_SCOREDEF_KEYSIG;
        else if (this->GetScoreDefRole() == SCOREDEF_CAUTIONARY)
            type = ALIGNMENT_SCOREDEF_CAUTION_KEYSIG;
        else {
            // type = ALIGNMENT_KEYSIG;
            // We force this because they should appear only at the beginning of a measure and should be non-justifiable
            // We also need it because the PAE importer creates keySig (and not staffDef @key.sig)
            type = ALIGNMENT_SCOREDEF_KEYSIG;
        }
    }
    else if (this->Is(MENSUR)) {
        if ((this->GetScoreDefRole() == SCOREDEF_SYSTEM) || (this->GetScoreDefRole() == SCOREDEF_INTERMEDIATE))
            type = ALIGNMENT_SCOREDEF_MENSUR;
        else if (this->GetScoreDefRole() == SCOREDEF_CAUTIONARY)
            type = ALIGNMENT_SCOREDEF_CAUTION_MENSUR;
        else {
            // replace the current mensur
            params->m_currentMensur = dynamic_cast<Mensur *>(this);
            assert(params->m_currentMensur);
            type = ALIGNMENT_MENSUR;
        }
    }
    else if (this->Is(METERSIG)) {
        if ((this->GetScoreDefRole() == SCOREDEF_SYSTEM) || (this->GetScoreDefRole() == SCOREDEF_INTERMEDIATE))
            type = ALIGNMENT_SCOREDEF_METERSIG;
        else if (this->GetScoreDefRole() == SCOREDEF_CAUTIONARY)
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
        Dot *dot = dynamic_cast<Dot *>(this);
        assert(dot);
        if (dot->m_drawingNote) {
            m_alignment = dot->m_drawingNote->GetAlignment();
        }
        else {
            // Create an alignment only if the dot has no resolved preceeding note
            type = ALIGNMENT_DOT;
        }
    }
    else if (this->Is(ACCID)) {
        // accid within note was already taken into account by noteParent
        type = ALIGNMENT_ACCID;
    }
    else if (this->Is({ ARTIC, ARTIC_PART })) {
        // Refer to the note parent
        Note *note = dynamic_cast<Note *>(this->GetFirstAncestor(NOTE));
        assert(note);
        m_alignment = note->GetAlignment();
    }
    else if (this->Is(SYL)) {
        Staff *staff = dynamic_cast<Staff *>(this->GetFirstAncestor(STAFF));
        assert(staff);

        if (staff->m_drawingNotationType == NOTATIONTYPE_neume) {
            type = ALIGNMENT_DEFAULT;
        }
        else {
            Note *note = dynamic_cast<Note *>(this->GetFirstAncestor(NOTE));
            assert(note);
            m_alignment = note->GetAlignment();
        }
    }
    else if (this->Is(VERSE)) {
        // Idem
        Note *note = dynamic_cast<Note *>(this->GetFirstAncestor(NOTE));
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
        duration = this->GetAlignmentDuration(
            params->m_currentMensur, params->m_currentMeterSig, true, params->m_notationType);

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
            Staff *staff = dynamic_cast<Staff *>(this->GetFirstAncestor(STAFF));
            assert(staff);
            int graceAlignerId = params->m_doc->GetOptions()->m_graceRhythmAlign.GetValue() ? 0 : staff->GetN();
            GraceAligner *graceAligner = m_alignment->GetGraceAligner(graceAlignerId);
            // We know that this is a note or a chord - we stack them and they will be added at the end of the layer
            // This will also see it for all their children
            graceAligner->StackGraceElement(this);
        }
    }

    // LogDebug("Element %f %s", params->m_time, this->GetClassName().c_str());

    if (!this->Is(TIMESTAMP_ATTR)) {
        // increase the time position, but only when not a timestamp (it would actually do nothing)
        params->m_time += duration;
    }

    return FUNCTOR_CONTINUE;
}

int LayerElement::SetAlignmentPitchPos(FunctorParams *functorParams)
{
    SetAlignmentPitchPosParams *params = dynamic_cast<SetAlignmentPitchPosParams *>(functorParams);
    assert(params);

    if (this->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    LayerElement *layerElementY = this;
    Staff *staffY = dynamic_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staffY);
    Layer *layerY = dynamic_cast<Layer *>(this->GetFirstAncestor(LAYER));
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
        Note *note = dynamic_cast<Note *>(this->GetFirstAncestor(NOTE));
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
        int loc = PitchInterface::CalcLoc(this, layerY, layerElementY, true);
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
        int loc = 0;
        if (note->HasPname()) {
            loc = PitchInterface::CalcLoc(note, layerY, layerElementY);
        }
        int yRel = staffY->CalcPitchPosYRel(params->m_doc, loc);
        // Make it relative to the top note one (see above) but not for cross-staff notes in chords
        if (chord && !m_crossStaff) {
            yRel -= chord->GetDrawingYRel();
        }
        note->SetDrawingLoc(loc);
        this->SetDrawingYRel(yRel);
    }
    else if (this->Is(MREST)) {
        MRest *mRest = dynamic_cast<MRest *>(this);
        assert(mRest);
        int loc = 0;
        if (mRest->HasPloc() && mRest->HasOloc()) {
            loc = PitchInterface::CalcLoc(mRest->GetPloc(), mRest->GetOloc(), layerY->GetClefLocOffset(layerElementY));
        }
        else if (mRest->HasLoc()) {
            loc = mRest->GetLoc();
        }
        // Automatically calculate rest position
        else {
            // set default location to the middle of the staff
            Staff *staff = dynamic_cast<Staff *>(this->GetFirstAncestor(STAFF));
            assert(staff);
            loc = staff->m_drawingLines - 1;
            // Limitation: GetLayerCount does not take into account editorial markup
            // should be refined later
            bool hasMultipleLayer = (staffY->GetChildCount(LAYER) > 1);
            if (hasMultipleLayer) {
                Layer *firstLayer = dynamic_cast<Layer *>(staffY->FindDescendantByType(LAYER));
                assert(firstLayer);
                if (firstLayer->GetN() == layerY->GetN())
                    loc += 2;
                else
                    loc -= 2;
            }

            // add offset
            else if (staff->m_drawingLines > 3)
                loc += 2;
        }

        mRest->SetDrawingLoc(loc);
        this->SetDrawingYRel(staffY->CalcPitchPosYRel(params->m_doc, loc));
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
            // set default location to the middle of the staff
            Staff *staff = dynamic_cast<Staff *>(this->GetFirstAncestor(STAFF));
            assert(staff);
            loc = staff->m_drawingLines - 1;

            Beam *beam = dynamic_cast<Beam *>(this->GetFirstAncestor(BEAM, 1));
            // Limitation: GetLayerCount does not take into account editorial markup
            // should be refined later
            bool hasMultipleLayer = (staffY->GetChildCount(LAYER) > 1);

            // If within a beam, calculate the rest's height based on it's relationship to the notes that surround it
            if (beam) {
                beam->ResetList(beam);

                const ArrayOfObjects *beamList = beam->GetList(beam);
                int restIndex = beam->GetChildIndex(rest);

                int leftLoc = loc;
                ArrayOfObjects::const_iterator it = beamList->begin();
                std::advance(it, restIndex);
                ArrayOfObjects::const_reverse_iterator rit(it);
                // iterate through the elements from the rest to the beginning of the beam
                // until we hit a note or chord, which we will use to determine where the rest should be placed
                for (; rit != beamList->rend(); ++rit) {
                    LayerElement *layerElement = dynamic_cast<LayerElement *>(*rit);
                    assert(layerElement);
                    if (layerElement->Is(NOTE)) {
                        leftLoc = PitchInterface::CalcLoc(layerElement, layerY, layerElementY);
                        break;
                    }
                    else if (layerElement->Is(CHORD)) {
                        int topChordLoc = PitchInterface::CalcLoc(layerElement, layerY, layerElementY, true);
                        int bottomChordLoc = PitchInterface::CalcLoc(layerElement, layerY, layerElementY, false);
                        // if it's a rest, use the middle of the chord as the rest's location
                        leftLoc = (topChordLoc + bottomChordLoc) / 2;
                        break;
                    }
                }

                int rightLoc = loc;
                it = beamList->begin();
                std::advance(it, restIndex);
                // iterate through the elements from the rest to the end of the beam
                // until we hit a note or chord, which we will use to determine where the rest should be placed
                for (; it != beamList->end(); ++it) {
                    LayerElement *layerElement = dynamic_cast<LayerElement *>(*it);
                    assert(layerElement);
                    if (layerElement->Is(NOTE)) {
                        rightLoc = PitchInterface::CalcLoc(layerElement, layerY, layerElementY);
                        break;
                    }
                    else if (layerElement->Is(CHORD)) {
                        int topChordLoc = PitchInterface::CalcLoc(layerElement, layerY, layerElementY, true);
                        int bottomChordLoc = PitchInterface::CalcLoc(layerElement, layerY, layerElementY, false);
                        // if it's a rest, use the middle of the chord as the rest's location
                        rightLoc = (topChordLoc + bottomChordLoc) / 2;
                        break;
                    }
                }

                // average the left note and right note's locations together to get our rest location
                const int locAvg = (rightLoc + leftLoc) / 2;
                if (abs(locAvg - loc) > 3) {
                    loc = locAvg;
                }

                // note: bottomAlignedLoc and topAlignedLoc are only accouting for discrepencies
                // between 8th, 16th and 32nd notes, not 64th's and on
                // I've described how to implement 64ths and beyond below

                // we need to check for bottom and top alignment because a 32nd rest that's top is in the space
                // under the staff (d4 on treble) can not be moved any closer to center by an incriment of 1
                // because the dots will collide with the staff
                // whereas a 16th rest that is in the same "loc" as the 32nd is actually below the 32nd
                // (the top of the 16th will be in note b3 on treble clef) so can be moved closer to the staff
                // than the 32nd without fear of the dots colliding with the staff lines

                // bottomAlignedLoc is the location where all of the rest's stems align to form a straight line
                int bottomAlignedLoc = loc;
                // 8th note rests are aligned with the top of a 16th note rest, so to bottom align we have to push it
                // down 2
                if (rest->GetActualDur() == DURATION_8) bottomAlignedLoc -= 2;
                // for durations smaller than 32nd, bottomAlignedLoc will need to decrease by 2 every iteration greater
                // than from 32 so 32 will be -0, 64 is -2, 128 is -4 (currently not implemented)

                // topAlignedLoc is the location where all of the top of the rests align to form a straight line
                int topAlignedLoc = loc;
                if (rest->GetActualDur() == DURATION_32) topAlignedLoc += 2;
                // for smaller durations, topAlignedLoc offset will increase by 2 every iteration greater than from 32
                // so 32 will need to be +2, 64 is +4, 128 is +6, etc.
                // (currently only implemented for 32nds)

                const int topOfStaffLoc = 10;
                const int bottomOfStaffLoc = -4;

                // move the extrema towards center a little for aesthetic reasons
                const bool restAboveStaff = bottomAlignedLoc >= topOfStaffLoc;
                const bool restBelowStaff = topAlignedLoc <= bottomOfStaffLoc;
                if (restAboveStaff) {
                    loc--;
                }
                else if (restBelowStaff) {
                    loc++;
                }

                // if loc is odd, we need to offset it to be even
                // so that the dots do not collide with the staff lines
                // or on ledger lines
                if (loc % 2 != 0) {
                    // if it's above the staff, offset downwards
                    // if below the staff, offset upwards
                    if (loc > 4)
                        loc--;
                    else
                        loc++;
                }
            }
            else if (hasMultipleLayer) {
                Layer *parentLayer = dynamic_cast<Layer *>(this->GetFirstAncestor(LAYER));
                assert(parentLayer);
                int layerCount = parentLayer->GetLayerCountForTimeSpanOf(this);
                if (layerCount > 1) {
                    Layer *firstLayer = dynamic_cast<Layer *>(staffY->FindDescendantByType(LAYER));
                    assert(firstLayer);
                    if (firstLayer->GetN() == layerY->GetN())
                        loc += 2;
                    else
                        loc -= 2;
                }
            }
        }
        loc = rest->GetRestLocOffset(loc);
        rest->SetDrawingLoc(loc);
        this->SetDrawingYRel(staffY->CalcPitchPosYRel(params->m_doc, loc));
    }

    return FUNCTOR_CONTINUE;
}

int LayerElement::AdjustLayers(FunctorParams *functorParams)
{
    AdjustLayersParams *params = dynamic_cast<AdjustLayersParams *>(functorParams);
    assert(params);

    if (this->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    // Check if we are starting a new layer content - if yes copy the current elements to previous
    if (!params->m_current.empty() && (this->GetAlignmentLayerN() != params->m_currentLayerN)) {
        params->m_previous.reserve(params->m_previous.size() + params->m_current.size());
        params->m_previous.insert(params->m_previous.end(), params->m_current.begin(), params->m_current.end());
        params->m_current.clear();
    }

    params->m_currentLayerN = this->GetAlignmentLayerN();

    // These are the only ones we want to keep for further collision detection
    // Eventually  we also need stem for overlapping voices
    if (this->Is({ DOTS, NOTE }) && this->HasSelfBB()) {
        params->m_current.push_back(this);
    }

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

        Staff *staff = dynamic_cast<Staff *>(this->GetFirstAncestor(STAFF));
        assert(staff);

        std::vector<LayerElement *>::iterator iter;
        for (iter = params->m_previous.begin(); iter != params->m_previous.end(); ++iter) {

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
                else if (abs(previousNote->GetDrawingLoc() - params->m_currentNote->GetDrawingLoc()) > 1)
                    continue;
            }

            if (this->Is(DOTS) && (*iter)->Is(DOTS)) {
                continue;
            }

            // Nothing to do if we have no vertical overlap
            if (!this->VerticalSelfOverlap(*iter, verticalMargin)) continue;

            // Nothing to do either if we have no horizontal overlap
            if (!this->HorizontalSelfOverlap(*iter, horizontalMargin)) continue;

            int xRelShift = this->HorizontalLeftOverlap(*iter, params->m_doc, horizontalMargin, verticalMargin);

            // Move the appropriate parent to the left
            if (xRelShift > 0) {
                if (params->m_currentChord)
                    params->m_currentChord->SetDrawingXRel(params->m_currentChord->GetDrawingXRel() + xRelShift);
                else if (params->m_currentNote)
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

    if (this->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    if (params->m_graceCumulatedXShift == VRV_UNSET) params->m_graceCumulatedXShift = 0;

    // LogDebug("********* Aligning %s", this->GetClassName().c_str());

    // With non grace alignment we do not need to do this
    this->ResetCachedDrawingX();

    if (!this->HasGraceAlignment()) return FUNCTOR_SIBLINGS;

    if (!this->HasSelfBB() || this->HasEmptyBB()) {
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
            * params->m_doc->GetDrawingUnit(params->m_doc->GetCueSize(100));

    params->m_graceUpcomingMaxPos = std::min(selfLeft, params->m_graceUpcomingMaxPos);

    return FUNCTOR_SIBLINGS;
}

int LayerElement::AdjustXPos(FunctorParams *functorParams)
{
    AdjustXPosParams *params = dynamic_cast<AdjustXPosParams *>(functorParams);
    assert(params);

    if (this->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    // we should have processed aligned before
    assert(this->GetAlignment());

    if (!this->HasToBeAligned()) {
        // if nothing to do with this type of element
        // this happens for example with Artic where only ArticPart children are aligned
        return FUNCTOR_SIBLINGS;
    }

    if (this->HasSameasLink()) {
        // nothing to do when the element has a @sameas attribute
        return FUNCTOR_SIBLINGS;
    }

    int selfLeft;
    if (!this->HasSelfBB() || this->HasEmptyBB()) {
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
            - params->m_doc->GetLeftMargin(this->GetClassId()) * params->m_doc->GetDrawingUnit(100);
    }

    int offset = selfLeft - params->m_minPos;
    if (offset < 0) {
        this->GetAlignment()->SetXRel(this->GetAlignment()->GetXRel() - offset);
        // Also move the cumultated x shift and the minimum position for the next alignment accordingly
        params->m_cumulatedXShift += (-offset);
        params->m_upcomingMinPos += (-offset);
    }

    int selfRight;
    if (!this->HasSelfBB() || this->HasEmptyBB()) {
        selfRight = this->GetAlignment()->GetXRel()
            + params->m_doc->GetRightMargin(this->GetClassId()) * params->m_doc->GetDrawingUnit(100);
    }
    else {
        selfRight = this->GetSelfRight()
            + params->m_doc->GetRightMargin(this->GetClassId()) * params->m_doc->GetDrawingUnit(100);
    }

    params->m_upcomingMinPos = std::max(selfRight, params->m_upcomingMinPos);

    return FUNCTOR_SIBLINGS;
}

int LayerElement::AdjustXRelForTranscription(FunctorParams *)
{
    if (this->m_xAbs == VRV_UNSET) return FUNCTOR_CONTINUE;

    if (this->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    if (!this->HasSelfBB()) return FUNCTOR_CONTINUE;

    this->SetDrawingXRel(-this->GetSelfX1());

    return FUNCTOR_CONTINUE;
}

int LayerElement::PrepareDrawingCueSize(FunctorParams *functorParams)
{
    if (this->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    if (this->IsGraceNote() || this->GetFirstAncestor(GRACEGRP)) {
        m_drawingCueSize = true;
    }
    // This cover the case when the @size is given on the element
    else if (this->HasAttClass(ATT_CUE)) {
        AttCue *att = dynamic_cast<AttCue *>(this);
        assert(att);
        if (att->HasCue()) m_drawingCueSize = (att->GetCue() == BOOLEAN_true);
    }
    // For note, we also need to look at the parent chord
    else if (this->Is(NOTE)) {
        Note const *note = dynamic_cast<Note const *>(this);
        assert(note);
        Chord *chord = note->IsChordTone();
        if (chord) m_drawingCueSize = chord->GetDrawingCueSize();
    }
    // For tuplet, we also need to look at the first note or chord
    else if (this->Is(TUPLET)) {
        ClassIdsComparison matchType({ NOTE, CHORD });
        ArrayOfObjects children;
        LayerElement *child = dynamic_cast<LayerElement *>(this->FindDescendantByComparison(&matchType));
        if (child) m_drawingCueSize = child->GetDrawingCueSize();
    }
    // For accid, look at the parent if @func="edit" or otherwise to the parent note
    else if (this->Is(ACCID)) {
        Accid const *accid = dynamic_cast<Accid *>(this);
        assert(accid);
        if ((accid->GetFunc() == accidLog_FUNC_edit) && !accid->HasEnclose())
            m_drawingCueSize = true;
        else {
            Note *note = dynamic_cast<Note *>(this->GetFirstAncestor(NOTE, MAX_ACCID_DEPTH));
            if (note) m_drawingCueSize = note->GetDrawingCueSize();
        }
    }
    else if (this->Is({ DOTS, FLAG, STEM })) {
        Note *note = dynamic_cast<Note *>(this->GetFirstAncestor(NOTE, MAX_NOTE_DEPTH));
        if (note)
            m_drawingCueSize = note->GetDrawingCueSize();
        else {
            Chord *chord = dynamic_cast<Chord *>(this->GetFirstAncestor(CHORD, MAX_CHORD_DEPTH));
            if (chord) m_drawingCueSize = chord->GetDrawingCueSize();
        }
    }

    return FUNCTOR_CONTINUE;
}

int LayerElement::PrepareCrossStaff(FunctorParams *functorParams)
{
    PrepareCrossStaffParams *params = dynamic_cast<PrepareCrossStaffParams *>(functorParams);
    assert(params);

    if (this->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

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

    AttNIntegerComparison comparisonFirst(STAFF, durElement->GetStaff().at(0));
    m_crossStaff = dynamic_cast<Staff *>(params->m_currentMeasure->FindDescendantByComparison(&comparisonFirst, 1));
    if (!m_crossStaff) {
        LogWarning("Could not get the cross staff reference '%d' for element '%s'", durElement->GetStaff().at(0),
            this->GetUuid().c_str());
        return FUNCTOR_CONTINUE;
    }

    Staff *parentStaff = dynamic_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(parentStaff);
    // Check if we have a cross-staff to itself...
    if (m_crossStaff == parentStaff) {
        LogWarning("The cross staff reference '%d' for element '%s' seems to be identical to the parent staff",
            durElement->GetStaff().at(0), this->GetUuid().c_str());
        m_crossStaff = NULL;
        return FUNCTOR_CONTINUE;
    }

    Layer *parentLayer = dynamic_cast<Layer *>(this->GetFirstAncestor(LAYER));
    assert(parentLayer);
    // Now try to get the corresponding layer - for now look for the same layer @n
    int layerN = parentLayer->GetN();
    // When we will have allowed @layer in <note>, we will have to do:
    // int layerN = durElement->HasLayer() ? durElement->GetLayer() : (*currentLayer)->GetN();
    AttNIntegerComparison comparisonFirstLayer(LAYER, layerN);
    m_crossLayer = dynamic_cast<Layer *>(m_crossStaff->FindDescendantByComparison(&comparisonFirstLayer, 1));
    if (!m_crossLayer) {
        // Just try to pick the first one...
        m_crossLayer = dynamic_cast<Layer *>(m_crossStaff->FindDescendantByType(LAYER));
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

    if (this->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

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

int LayerElement::PreparePointersByLayer(FunctorParams *functorParams)
{
    PreparePointersByLayerParams *params = dynamic_cast<PreparePointersByLayerParams *>(functorParams);
    assert(params);

    if (this->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    if (params->m_lastDot) {
        params->m_lastDot->m_drawingNextElement = this;
        params->m_lastDot = NULL;
    }
    if (this->Is(BARLINE)) {
        // Do not attach a note when a barline is passed
        params->m_currentNote = NULL;
    }

    return FUNCTOR_CONTINUE;
}

int LayerElement::PrepareTimePointing(FunctorParams *functorParams)
{
    PrepareTimePointingParams *params = dynamic_cast<PrepareTimePointingParams *>(functorParams);
    assert(params);

    if (this->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    // Do not look for tstamp pointing to these
    if (this->Is({ ARTIC, ARTIC_PART, BEAM, FLAG, TUPLET, STEM, VERSE })) return FUNCTOR_CONTINUE;

    ArrayOfPointingInterClassIdPairs::iterator iter = params->m_timePointingInterfaces.begin();
    while (iter != params->m_timePointingInterfaces.end()) {
        if (iter->first->SetStartOnly(this)) {
            // We have both the start and the end that are matched
            iter = params->m_timePointingInterfaces.erase(iter);
        }
        else {
            ++iter;
        }
    }

    return FUNCTOR_CONTINUE;
}

int LayerElement::PrepareTimeSpanning(FunctorParams *functorParams)
{
    PrepareTimeSpanningParams *params = dynamic_cast<PrepareTimeSpanningParams *>(functorParams);
    assert(params);

    if (this->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    // Do not look for tstamp pointing to these
    if (this->Is({ ARTIC, ARTIC_PART, BEAM, FLAG, TUPLET, STEM, VERSE })) return FUNCTOR_CONTINUE;

    ArrayOfSpanningInterClassIdPairs::iterator iter = params->m_timeSpanningInterfaces.begin();
    while (iter != params->m_timeSpanningInterfaces.end()) {
        if (iter->first->SetStartAndEnd(this)) {
            // We have both the start and the end that are matched
            iter = params->m_timeSpanningInterfaces.erase(iter);
        }
        else {
            ++iter;
        }
    }

    return FUNCTOR_CONTINUE;
}

int LayerElement::LayerCountInTimeSpan(FunctorParams *functorParams)
{
    LayerCountInTimeSpanParams *params = dynamic_cast<LayerCountInTimeSpanParams *>(functorParams);
    assert(params);

    if (this->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    // For mRest we do not look at the time span
    if (this->Is(MREST)) {
        // Add the layerN to the list of layer element occuring in this time frame
        if (std::find(params->m_layers.begin(), params->m_layers.end(), this->GetAlignmentLayerN())
            == params->m_layers.end()) {
            params->m_layers.push_back(this->GetAlignmentLayerN());
        }
        return FUNCTOR_SIBLINGS;
    }

    if (!this->GetDurationInterface() || this->Is(SPACE) || this->HasSameasLink()) return FUNCTOR_CONTINUE;

    double duration = this->GetAlignmentDuration(params->m_mensur, params->m_meterSig);
    double time = m_alignment->GetTime();

    // The event is starting after the end of the element
    if ((time + duration) <= params->m_time) {
        return FUNCTOR_CONTINUE;
    }
    // The element is starting after the event end - we can stop here
    else if (time >= (params->m_time + params->m_duration)) {
        return FUNCTOR_STOP;
    }

    // Add the layerN to the list of layer element occuring in this time frame
    if (std::find(params->m_layers.begin(), params->m_layers.end(), this->GetAlignmentLayerN())
        == params->m_layers.end()) {
        params->m_layers.push_back(this->GetAlignmentLayerN());
    }

    // Not need to recurse for chords? Not quite sure about it.
    return (this->Is(CHORD)) ? FUNCTOR_SIBLINGS : FUNCTOR_CONTINUE;
}

int LayerElement::FindSpannedLayerElements(FunctorParams *functorParams)
{
    FindSpannedLayerElementsParams *params = dynamic_cast<FindSpannedLayerElementsParams *>(functorParams);
    assert(params);

    if (this->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    if (!this->Is(params->m_classIds)) {
        return FUNCTOR_CONTINUE;
    }

    if (this->HasContentBB() && (this->GetContentRight() > params->m_minPos)
        && (this->GetContentLeft() < params->m_maxPos)) {

        // We skip the start or end of the slur
        if ((this == params->m_interface->GetStart()) || (this == params->m_interface->GetEnd())) {
            return FUNCTOR_CONTINUE;
        }
        if (params->m_interface->GetStart()->HasDescendant(this)
            || this->HasDescendant(params->m_interface->GetStart())) {
            return FUNCTOR_CONTINUE;
        }
        if (params->m_interface->GetEnd()->HasDescendant(this) || this->HasDescendant(params->m_interface->GetEnd())) {
            return FUNCTOR_CONTINUE;
        }

        params->m_elements.push_back(this);
    }
    else if (this->GetDrawingX() > params->m_maxPos) {
        return FUNCTOR_STOP;
    }

    return FUNCTOR_CONTINUE;
}

int LayerElement::CalcOnsetOffset(FunctorParams *functorParams)
{
    CalcOnsetOffsetParams *params = dynamic_cast<CalcOnsetOffsetParams *>(functorParams);
    assert(params);

    if (this->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    LayerElement *element = this->ThisOrSameasAsLink();

    double incrementScoreTime;

    if (element->Is(REST) || element->Is(SPACE)) {
        incrementScoreTime = element->GetAlignmentDuration(
            params->m_currentMensur, params->m_currentMeterSig, true, params->m_notationType);
        incrementScoreTime = incrementScoreTime / (DUR_MAX / DURATION_4);
        params->m_currentScoreTime += incrementScoreTime;
        params->m_currentRealTimeSeconds += incrementScoreTime * 60.0 / params->m_currentTempo;
    }
    else if (element->Is(NOTE)) {
        Note *note = dynamic_cast<Note *>(element);
        assert(note);

        // For now just ignore grace notes
        if (note->IsGraceNote()) return FUNCTOR_CONTINUE;

        Chord *chord = note->IsChordTone();

        // If the note has a @dur or a @dur.ges, take it into account
        // This means that overwriting only @dots or @dots.ges will not be taken into account
        if (chord && !note->HasDur() && !note->HasDurGes()) {
            incrementScoreTime = chord->GetAlignmentDuration(
                params->m_currentMensur, params->m_currentMeterSig, true, params->m_notationType);
        }
        else {
            incrementScoreTime = note->GetAlignmentDuration(
                params->m_currentMensur, params->m_currentMeterSig, true, params->m_notationType);
        }
        incrementScoreTime = incrementScoreTime / (DUR_MAX / DURATION_4);
        double realTimeIncrementSeconds = incrementScoreTime * 60.0 / params->m_currentTempo;

        // LogDebug("Note Alignment Duration %f - Dur %d - Diatonic Pitch %d - Track %d", GetAlignmentDuration(),
        // note->GetNoteOrChordDur(element), note->GetDiatonicPitch(), *midiTrack);
        // LogDebug("Oct %d - Pname %d - Accid %d", note->GetOct(), note->GetPname(), note->GetAccid());

        Note *storeNote = note;
        // When we have a @sameas, do store the onset / offset values of the pointed note in the pointing note
        if (this != element) {
            storeNote = dynamic_cast<Note *>(this);
        }
        assert(storeNote);
        storeNote->SetScoreTimeOnset(params->m_currentScoreTime);
        storeNote->SetRealTimeOnsetSeconds(params->m_currentRealTimeSeconds);
        storeNote->SetScoreTimeOffset(params->m_currentScoreTime + incrementScoreTime);
        storeNote->SetRealTimeOffsetSeconds(params->m_currentRealTimeSeconds + realTimeIncrementSeconds);

        // increase the currentTime accordingly, but only if not in a chord - checkit with note->IsChordTone()
        if (!(note->IsChordTone())) {
            params->m_currentScoreTime += incrementScoreTime;
            params->m_currentRealTimeSeconds += realTimeIncrementSeconds;
        }
    }
    else if (element->Is(BEATRPT)) {
        BeatRpt *rpt = dynamic_cast<BeatRpt *>(element);
        assert(rpt);

        incrementScoreTime = rpt->GetAlignmentDuration(
            params->m_currentMensur, params->m_currentMeterSig, true, params->m_notationType);
        incrementScoreTime = incrementScoreTime / (DUR_MAX / DURATION_4);
        rpt->SetScoreTimeOnset(params->m_currentScoreTime);
        params->m_currentScoreTime += incrementScoreTime;
        params->m_currentRealTimeSeconds += incrementScoreTime * 60.0 / params->m_currentTempo;
    }
    else if (this->Is({ BEAM, LIGATURE, FTREM, TUPLET }) && this->HasSameasLink()) {
        incrementScoreTime = this->GetContentAlignmentDuration(
            params->m_currentMensur, params->m_currentMeterSig, true, params->m_notationType);
        incrementScoreTime = incrementScoreTime / (DUR_MAX / DURATION_4);
        params->m_currentScoreTime += incrementScoreTime;
        params->m_currentRealTimeSeconds += incrementScoreTime * 60.0 / params->m_currentTempo;
    }
    return FUNCTOR_CONTINUE;
}

int LayerElement::ResolveMIDITies(FunctorParams *)
{
    return FUNCTOR_CONTINUE;
}

int LayerElement::GenerateMIDI(FunctorParams *functorParams)
{
    GenerateMIDIParams *params = dynamic_cast<GenerateMIDIParams *>(functorParams);
    assert(params);

    if (this->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    if (this->HasSameasLink()) {
        assert(this->GetSameasLink());
        this->GetSameasLink()->Process(params->m_functor, functorParams);
    }

    return FUNCTOR_CONTINUE;
}

int LayerElement::GenerateTimemap(FunctorParams *functorParams)
{
    GenerateTimemapParams *params = dynamic_cast<GenerateTimemapParams *>(functorParams);
    assert(params);

    if (this->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    if (this->HasSameasLink()) {
        assert(this->GetSameasLink());
        this->GetSameasLink()->Process(params->m_functor, functorParams);
    }

    return FUNCTOR_CONTINUE;
}

int LayerElement::ResetDrawing(FunctorParams *)
{
    m_drawingCueSize = false;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
