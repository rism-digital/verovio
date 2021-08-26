/////////////////////////////////////////////////////////////////////////////
// Name:        layerelement.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------

#include <algorithm>
#include <assert.h>
#include <climits>
#include <math.h>
#include <numeric>

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
#include "elementpart.h"
#include "ftrem.h"
#include "functorparams.h"
#include "horizontalaligner.h"
#include "keysig.h"
#include "layer.h"
#include "ligature.h"
#include "measure.h"
#include "mensur.h"
#include "metersig.h"
#include "metersiggrp.h"
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
#include "tabgrp.h"
#include "tie.h"
#include "timeinterface.h"
#include "timestamp.h"
#include "tuning.h"
#include "tuplet.h"
#include "turn.h"
#include "verse.h"
#include "view.h"
#include "vrv.h"
#include "zone.h"

namespace vrv {

//----------------------------------------------------------------------------
// LayerElement
//----------------------------------------------------------------------------

LayerElement::LayerElement()
    : Object(LAYER_ELEMENT, "le-"), FacsimileInterface(), LinkingInterface(), AttLabelled(), AttTyped()
{
    RegisterInterface(FacsimileInterface::GetAttClasses(), FacsimileInterface::IsInterface());
    RegisterInterface(LinkingInterface::GetAttClasses(), LinkingInterface::IsInterface());
    RegisterAttClass(ATT_LABELLED);
    RegisterAttClass(ATT_TYPED);

    Reset();
}

LayerElement::LayerElement(ClassId classId)
    : Object(classId, "le-"), FacsimileInterface(), LinkingInterface(), AttLabelled(), AttTyped()
{
    RegisterInterface(FacsimileInterface::GetAttClasses(), FacsimileInterface::IsInterface());
    RegisterInterface(LinkingInterface::GetAttClasses(), LinkingInterface::IsInterface());
    RegisterAttClass(ATT_LABELLED);
    RegisterAttClass(ATT_TYPED);

    Reset();
}

LayerElement::LayerElement(ClassId classId, const std::string &classIdStr)
    : Object(classId, classIdStr), FacsimileInterface(), LinkingInterface(), AttLabelled(), AttTyped()
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

    m_crossStaff = NULL;
    m_crossLayer = NULL;
}

LayerElement::~LayerElement() {}

void LayerElement::CloneReset()
{
    Object::CloneReset();

    // pointers have to be NULL
    m_alignment = NULL;
    m_graceAlignment = NULL;
    m_alignmentLayerN = VRV_UNSET;

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
        Note const *note = vrv_cast<Note const *>(this);
        assert(note);
        Chord *chord = note->IsChordTone();
        if (chord)
            return chord->HasGrace();
        else
            return (note->HasGrace());
    }
    else if (this->Is(CHORD)) {
        Chord const *chord = vrv_cast<Chord const *>(this);
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

bool LayerElement::GetDrawingCueSize() const
{
    return m_drawingCueSize;
}

bool LayerElement::IsInLigature() const
{
    if (!this->Is(NOTE)) return false;
    return (this->GetFirstAncestor(LIGATURE, MAX_LIGATURE_DEPTH));
}

FTrem *LayerElement::IsInFTrem()
{
    if (!this->Is({ CHORD, NOTE })) return NULL;
    return dynamic_cast<FTrem *>(this->GetFirstAncestor(FTREM, MAX_FTREM_DEPTH));
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

data_STAFFREL_basic LayerElement::GetCrossStaffRel()
{
    if (!m_crossStaff) return STAFFREL_basic_NONE;

    Staff *staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff);

    return (m_crossStaff->GetN() < staff->GetN()) ? STAFFREL_basic_above : STAFFREL_basic_below;
}

void LayerElement::GetOverflowStaffAlignments(StaffAlignment *&above, StaffAlignment *&below)
{
    Staff *staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff);

    // By default use the alignment of the parent staff
    above = staff->GetAlignment();
    below = above;

    // Chord and beam parent (if any)
    Chord *chord = dynamic_cast<Chord *>(this->GetFirstAncestor(CHORD));
    Beam *beam = dynamic_cast<Beam *>(this->GetFirstAncestor(BEAM));

    Layer *crossLayer = NULL;
    Staff *crossStaff = this->GetCrossStaff(crossLayer);

    // By default for cross-staff element, use the cross-staff alignment
    if (crossStaff && crossStaff->GetAlignment()) {
        above = crossStaff->GetAlignment();
        below = above;
    }
    // Dots, flags and stems with cross-staff chords need special treatment
    if (this->Is({ DOTS, FLAG, STEM }) && chord && chord->HasCrossStaff()) {
        Staff *staffAbove = NULL;
        Staff *staffBelow = NULL;
        chord->GetCrossStaffExtremes(staffAbove, staffBelow);
        if (staffAbove) {
            above = staffAbove->GetAlignment();
        }
        if (staffBelow) {
            below = staffBelow->GetAlignment();
        }
    }
    // Stems cross-staff beam need special treatment but only if the beam itself is not cross-staff
    if (this->Is({ ARTIC, STEM }) && beam && beam->m_crossStaffContent && !beam->m_crossStaff) {
        data_STAFFREL_basic direction = beam->m_crossStaffRel;
        if (direction == STAFFREL_basic_above) {
            above = beam->m_crossStaffContent->GetAlignment();
            below = beam->m_beamStaff->GetAlignment();
        }
        else {
            above = beam->m_beamStaff->GetAlignment();
            below = beam->m_crossStaffContent->GetAlignment();
        }
    }
    // Beams in cross-staff situation need special treatment
    if (this->Is(BEAM)) {
        beam = vrv_cast<Beam *>(this);
        assert(beam);
        // Beam between the staves - ignore both above and below
        if (beam->m_crossStaffContent && !beam->m_crossStaff) {
            data_STAFFREL_basic direction = beam->m_crossStaffRel;
            if (beam->m_drawingPlace == BEAMPLACE_mixed) {
                above = NULL;
                below = NULL;
            }
            // Beam below - ignore above and find the appropriate below staff
            else if (beam->m_drawingPlace == BEAMPLACE_below) {
                above = NULL;
                if (direction == STAFFREL_basic_above) {
                    below = beam->m_beamStaff->GetAlignment();
                }
                else {
                    below = beam->m_crossStaffContent->GetAlignment();
                }
            }
            // Beam above - ignore below and find the appropriate above staff
            else if (beam->m_drawingPlace == BEAMPLACE_above) {
                below = NULL;
                if (direction == STAFFREL_basic_below) {
                    above = beam->m_beamStaff->GetAlignment();
                }
                else {
                    above = beam->m_crossStaffContent->GetAlignment();
                }
            }
        }
    }
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
        Doc *doc = vrv_cast<Doc *>(this->GetFirstAncestor(DOC));
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
        Doc *doc = vrv_cast<Doc *>(this->GetFirstAncestor(DOC));
        assert(doc);
        if (doc->GetType() == Facs) {
            return FacsimileInterface::GetDrawingY();
        }
    }

    if (m_cachedDrawingY != VRV_UNSET) return m_cachedDrawingY;

    // Look if we have a crossStaff situation
    Object *object = m_crossStaff; // GetCrossStaff();
    // First get the first layerElement parent (if any) but only if the element is not directly relative to staff
    // (e.g. artic, syl)
    if (!object && !this->IsRelativeToStaff()) object = this->GetFirstAncestorInRange(LAYER_ELEMENT, LAYER_ELEMENT_max);
    // Otherwise get the first staff
    if (!object) object = this->GetFirstAncestor(STAFF);
    // Otherwise the first measure (this is the case with barLineAttr)
    if (!object) object = this->GetFirstAncestor(MEASURE);

    assert(object);

    m_cachedDrawingY = object->GetDrawingY() + this->GetDrawingYRel();
    return m_cachedDrawingY;
}

int LayerElement::GetDrawingArticulationTopOrBottom(data_STAFFREL place, ArticType type)
{
    // It would not crash otherwise but there is not reason to call it
    assert(this->Is({ NOTE, CHORD }));

    ClassIdComparison isArtic(ARTIC);
    ListOfObjects artics;
    // Process backward because we want the farest away artic
    this->FindAllDescendantByComparison(&artics, &isArtic, UNLIMITED_DEPTH, BACKWARD);

    Artic *artic = NULL;
    for (auto &child : artics) {
        artic = vrv_cast<Artic *>(child);
        assert(artic);
        if (artic->GetDrawingPlace() == place) break;
        // otherwise reset it because we test on the pointer below
        artic = NULL;
    }

    int y = (place == STAFFREL_above) ? VRV_UNSET : -VRV_UNSET;
    if (artic) {
        y = (place == STAFFREL_above) ? artic->GetSelfTop() : artic->GetSelfBottom();
    }

    return y;
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
    if (m_xAbs != VRV_UNSET) return;

    SetDrawingXRel(0);

    Measure *measure = vrv_cast<Measure *>(this->GetFirstAncestor(MEASURE));
    assert(measure);

    SetDrawingXRel(measure->GetInnerCenterX() - this->GetDrawingX());
}

int LayerElement::GetDrawingTop(Doc *doc, int staffSize, bool withArtic, ArticType type)
{
    if (this->Is({ NOTE, CHORD }) && withArtic) {
        int articY = this->GetDrawingArticulationTopOrBottom(STAFFREL_above, type);
        if (articY != VRV_UNSET) return articY;
    }

    Note *note = NULL;
    if (this->Is(CHORD)) {
        Chord *chord = vrv_cast<Chord *>(this);
        assert(chord);
        note = chord->GetTopNote();
    }
    else if (this->Is(NOTE)) {
        note = vrv_cast<Note *>(this);
        assert(note);
    }

    if (note) {
        DurationInterface *durationInterface = this->GetDurationInterface();
        assert(durationInterface);
        if (durationInterface->GetNoteOrChordDur(this) < DUR_2) {
            return note->GetDrawingY() + doc->GetDrawingUnit(staffSize);
        }
        // We should also take into account the stem shift to the right
        StemmedDrawingInterface *stemmedDrawingInterface = this->GetStemmedDrawingInterface();
        assert(stemmedDrawingInterface);
        if (stemmedDrawingInterface->GetDrawingStemDir() == STEMDIRECTION_up) {
            return stemmedDrawingInterface->GetDrawingStemEnd(this).y;
        }
        else {
            // this does not take into account the glyph's actual size
            return note->GetDrawingY() + doc->GetDrawingUnit(staffSize);
        }
    }
    return this->GetDrawingY();
}

int LayerElement::GetDrawingBottom(Doc *doc, int staffSize, bool withArtic, ArticType type)
{
    if (this->Is({ NOTE, CHORD }) && withArtic) {
        int articY = this->GetDrawingArticulationTopOrBottom(STAFFREL_below, type);
        if (articY != -VRV_UNSET) return articY;
    }

    Note *note = NULL;
    if (this->Is(CHORD)) {
        Chord *chord = vrv_cast<Chord *>(this);
        assert(chord);
        note = chord->GetBottomNote();
    }
    else if (this->Is(NOTE)) {
        note = vrv_cast<Note *>(this);
        assert(note);
    }

    if (note) {
        DurationInterface *durationInterface = this->GetDurationInterface();
        assert(durationInterface);
        if (durationInterface->GetNoteOrChordDur(this) < DUR_2) {
            return note->GetDrawingY() - doc->GetDrawingUnit(staffSize);
        }
        // We should also take into account the stem shift to the right
        StemmedDrawingInterface *stemmedDrawingInterface = this->GetStemmedDrawingInterface();
        assert(stemmedDrawingInterface);
        if (stemmedDrawingInterface->GetDrawingStemDir() == STEMDIRECTION_up) {
            // this does not take into account the glyph's actual size
            return note->GetDrawingY() - doc->GetDrawingUnit(staffSize);
        }
        else {
            return stemmedDrawingInterface->GetDrawingStemEnd(this).y;
        }
    }
    return this->GetDrawingY();
}

int LayerElement::GetDrawingRadius(Doc *doc, bool isInLigature)
{
    assert(doc);

    if (!this->Is({ CHORD, NOTE, REST })) return 0;

    wchar_t code = 0;
    int dur = DUR_4;
    Staff *staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff);
    if (this->Is(NOTE)) {
        Note *note = vrv_cast<Note *>(this);
        assert(note);
        dur = note->GetDrawingDur();
        if (note->IsMensuralDur() && !isInLigature) {
            code = note->GetMensuralNoteheadGlyph();
        }
        else {
            code = note->GetNoteheadGlyph(dur);
        }
    }
    else if (this->Is(CHORD)) {
        Chord *chord = vrv_cast<Chord *>(this);
        assert(chord);
        dur = chord->GetActualDur();
        if (dur == DUR_1)
            code = SMUFL_E0A2_noteheadWhole;
        else if (dur == DUR_2)
            code = SMUFL_E0A3_noteheadHalf;
        else
            code = SMUFL_E0A4_noteheadBlack;
    }
    else if (this->Is(REST)) {
        code = SMUFL_E0A4_noteheadBlack;
    }

    // Mensural note shorter than DUR_BR
    if ((dur <= DUR_BR) || ((dur == DUR_1) && isInLigature)) {
        int widthFactor = (dur == DUR_MX) ? 2 : 1;
        if (staff->m_drawingNotationType == NOTATIONTYPE_mensural_black) {
            return widthFactor * doc->GetDrawingBrevisWidth(staff->m_drawingStaffSize) * 0.7;
        }
        else {
            return widthFactor * doc->GetDrawingBrevisWidth(staff->m_drawingStaffSize);
        }
    }

    // The code should not be null at this stage. It can above but only with mensural notation.
    // Maybe this can be refactored with a distinct method for radius in mensural notation.
    assert(code);

    return doc->GetGlyphWidth(code, staff->m_drawingStaffSize, this->GetDrawingCueSize()) / 2;
}

double LayerElement::GetAlignmentDuration(
    Mensur *mensur, MeterSig *meterSig, bool notGraceOnly, data_NOTATIONTYPE notationType)
{
    if (this->IsGraceNote() && notGraceOnly) {
        return 0.0;
    }

    if (this->HasSameasLink() && this->GetSameasLink()->IsLayerElement()) {
        LayerElement *sameas = vrv_cast<LayerElement *>(this->GetSameasLink());
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
        if (duration->IsMensuralDur() && (notationType != NOTATIONTYPE_cmn)) {
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
        BeatRpt *beatRpt = vrv_cast<BeatRpt *>(this);
        assert(beatRpt);
        int meterUnit = 4;
        if (meterSig && meterSig->HasUnit()) meterUnit = meterSig->GetUnit();
        return beatRpt->GetBeatRptAlignmentDuration(meterUnit);
    }
    else if (this->Is(TIMESTAMP_ATTR)) {
        TimestampAttr *timestampAttr = vrv_cast<TimestampAttr *>(this);
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
        if (meterSig && meterSig->HasCount()) meterCount = meterSig->GetTotalCount();

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

double LayerElement::GetSameAsContentAlignmentDuration(
    Mensur *mensur, MeterSig *meterSig, bool notGraceOnly, data_NOTATIONTYPE notationType) const
{
    if (!this->HasSameasLink() || !this->GetSameasLink()->Is({ BEAM, FTREM, TUPLET })) {
        return 0.0;
    }

    LayerElement *sameas = vrv_cast<LayerElement *>(this->GetSameasLink());
    assert(sameas);

    return sameas->GetContentAlignmentDuration(mensur, meterSig, notGraceOnly, notationType);
}

double LayerElement::GetContentAlignmentDuration(
    Mensur *mensur, MeterSig *meterSig, bool notGraceOnly, data_NOTATIONTYPE notationType) const
{
    if (!this->Is({ BEAM, FTREM, TUPLET })) {
        return 0.0;
    }

    double duration = 0.0;

    for (auto child : *this->GetChildren()) {
        // Skip everything that does not have a duration interface and notes in chords
        if (!child->HasInterface(INTERFACE_DURATION) || (child->GetFirstAncestor(CHORD, MAX_CHORD_DEPTH) != NULL)) {
            continue;
        }
        LayerElement *element = vrv_cast<LayerElement *>(child);
        assert(element);
        duration += element->GetAlignmentDuration(mensur, meterSig, notGraceOnly, notationType);
    }

    return duration;
}

bool LayerElement::GenerateZoneBounds(int *ulx, int *uly, int *lrx, int *lry)
{
    // Set integers to extremes
    *ulx = INT_MAX;
    *uly = INT_MAX;
    *lrx = INT_MIN;
    *lry = INT_MIN;
    ListOfObjects childrenWithFacsimileInterface;
    InterfaceComparison ic(INTERFACE_FACSIMILE);
    this->FindAllDescendantByComparison(&childrenWithFacsimileInterface, &ic);
    bool result = false;
    for (auto it = childrenWithFacsimileInterface.begin(); it != childrenWithFacsimileInterface.end(); ++it) {
        FacsimileInterface *fi = dynamic_cast<FacsimileInterface *>(*it);
        assert(fi);
        if (!(*it)->Is(SYL) && fi->HasFacs()) {
            Zone *zone = fi->GetZone();
            assert(zone);
            *ulx = std::min(*ulx, zone->GetUlx());
            *uly = std::min(*uly, zone->GetUly());
            *lrx = std::max(*lrx, zone->GetLrx());
            *lry = std::max(*lry, zone->GetLry());
            result |= true;
        }
    }
    return result;
}

int LayerElement::CountElementsInUnison(
    const std::set<int> &firstChord, const std::set<int> &secondChord, data_STEMDIRECTION stemDirection)
{
    if (firstChord.empty() || secondChord.empty()) return 0;
    // Set always sorts elements, hence note locations stored will always be in ascending order, regardless
    // of how they are encoded in the MEI file
    std::set<int> difference;
    if (firstChord.size() > secondChord.size()) {
        std::set_difference(firstChord.begin(), firstChord.end(), secondChord.begin(), secondChord.end(),
            std::inserter(difference, difference.begin()));
    }
    else {
        std::set_difference(secondChord.begin(), secondChord.end(), firstChord.begin(), firstChord.end(),
            std::inserter(difference, difference.begin()));
    }
    // If there is a difference, there are two situations:
    // 1. Location is between start and end of either chords/set of notes - this means these elements cannot be
    //    in unison (since there is interfering note there)
    // 2. Location is lesser/greater than start/end of either - it's ok, overlapping notes can still be in unison
    if (!difference.empty()) {
        for (const auto &element : difference) {
            if (((firstChord.size() <= secondChord.size()) && (element > *firstChord.begin())
                    && (element < *firstChord.rbegin()))
                || ((firstChord.size() > secondChord.size()) && (element > *secondChord.begin())
                    && (element < *secondChord.rbegin()))) {
                return 0;
            }
        }
    }

    // If there are no `middle` notes, check whether chords can be in unison with regards of stem direction
    // With DOWN stem direction, highest note of the chord HAS to be in unison. If topmost location of the chord
    // is higher than topmost location of the opposing chord it means that these elements cannot be in unison.
    // Same applies to the UP stem direction, just with reversed condition
    if (stemDirection == STEMDIRECTION_down) {
        if ((*firstChord.rbegin() > *secondChord.rbegin()) || (*firstChord.begin() > *secondChord.begin())) return 0;
    }
    else {
        if ((*firstChord.rbegin() < *secondChord.rbegin()) || (*firstChord.begin() < *secondChord.begin())) return 0;
    }

    // Finally, check if notes in unison are at the proper distance to be drawn as unison, as well as get number of
    // elements in unison
    std::vector<int> intersection;
    intersection.resize(firstChord.size() > secondChord.size() ? firstChord.size() : secondChord.size());
    auto it = std::set_intersection(
        firstChord.begin(), firstChord.end(), secondChord.begin(), secondChord.end(), intersection.begin());
    intersection.resize(it - intersection.begin());
    if (intersection.empty()) return false;
    for (int i = 0; i < (int)intersection.size() - 1; ++i) {
        if (std::abs(intersection.at(i) - intersection.at(i + 1)) == 1) {
            return 0;
        }
    }

    return (int)intersection.size();
}

MapOfDotLocs LayerElement::CalcOptimalDotLocations()
{
    if (!this->Is({ NOTE, CHORD })) {
        return {};
    }

    Staff *staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
    const int layerCount = staff->GetChildCount(LAYER);

    // Calculate primary/secondary dot locations
    const MapOfDotLocs dotLocs1 = this->CalcDotLocations(layerCount, true);
    const MapOfDotLocs dotLocs2 = this->CalcDotLocations(layerCount, false);

    // Special treatment for two layers
    if (layerCount == 2) {
        // Find the first note on the other layer
        Alignment *alignment = this->GetAlignment();
        const int currentLayerN = abs(this->GetAlignmentLayerN());
        ListOfObjects notes;
        ClassIdComparison noteCmp(NOTE);
        alignment->FindAllDescendantByComparison(&notes, &noteCmp, 2);
        auto noteIt = std::find_if(notes.cbegin(), notes.cend(), [currentLayerN](Object *obj) {
            const int otherLayerN = abs(vrv_cast<Note *>(obj)->GetAlignmentLayerN());
            return (currentLayerN != otherLayerN);
        });

        if (noteIt != notes.cend()) {
            // Prefer the note's chord if it has one
            LayerElement *other = vrv_cast<Note *>(*noteIt);
            if (Chord *chord = vrv_cast<Note *>(*noteIt)->IsChordTone(); chord) {
                other = chord;
            }
            assert(other);
            const int otherLayerN = abs(other->GetAlignmentLayerN());

            // Calculate the primary/secondary dot locations
            const MapOfDotLocs otherDotLocs1 = other->CalcDotLocations(layerCount, true);
            const MapOfDotLocs otherDotLocs2 = other->CalcDotLocations(layerCount, false);

            // Handling of unisons
            if (this->Is(NOTE) && other->Is(NOTE)) {
                Note *note = vrv_cast<Note *>(this);
                Note *otherNote = vrv_cast<Note *>(other);
                if (note->IsUnisonWith(otherNote)) {
                    if (note->GetDrawingStemDir() == STEMDIRECTION_up) {
                        otherNote->AlignDotsShift(note);
                    }
                    else if (otherNote->GetDrawingStemDir() == STEMDIRECTION_up) {
                        note->AlignDotsShift(otherNote);
                    }
                    return (currentLayerN < otherLayerN) ? dotLocs1 : dotLocs2;
                }
            }

            // Count collisions between each pair of dot choices
            const int collisions11 = GetCollisionCount(dotLocs1, otherDotLocs1);
            const int collisions12 = GetCollisionCount(dotLocs1, otherDotLocs2);
            const int collisions21 = GetCollisionCount(dotLocs2, otherDotLocs1);
            const int collisions22 = GetCollisionCount(dotLocs2, otherDotLocs2);
            const int maxCollisions = std::max({ collisions11, collisions12, collisions21, collisions22 });

            if (maxCollisions > 0) {
                // Collisions might occur => choose dots which minimize the number of collisions
                const int minCollisions = std::min({ collisions11, collisions12, collisions21, collisions22 });
                if (collisions11 == minCollisions) return dotLocs1;
                if (collisions12 == minCollisions) {
                    if (collisions21 == minCollisions) {
                        // Symmetric case: choose primary dot location on upper layer
                        return (currentLayerN < otherLayerN) ? dotLocs1 : dotLocs2;
                    }
                    return dotLocs1;
                }
                return dotLocs2;
            }
        }
    }

    // Count dots to decide which set is used
    const bool usePrimary = (GetDotCount(dotLocs1) >= GetDotCount(dotLocs2));
    return usePrimary ? dotLocs1 : dotLocs2;
}

//----------------------------------------------------------------------------
// Static methods for LayerElement
//----------------------------------------------------------------------------

int LayerElement::GetDotCount(const MapOfDotLocs &dotLocations)
{
    return std::accumulate(dotLocations.cbegin(), dotLocations.cend(), 0,
        [](int sum, const MapOfDotLocs::value_type &mapEntry) { return sum + mapEntry.second.size(); });
}

int LayerElement::GetCollisionCount(const MapOfDotLocs &dotLocs1, const MapOfDotLocs &dotLocs2)
{
    int count = 0;
    for (const auto &mapEntry : dotLocs1) {
        if (dotLocs2.find(mapEntry.first) != dotLocs2.cend()) {
            std::set<int> commonElements;
            std::set_intersection(mapEntry.second.cbegin(), mapEntry.second.cend(),
                dotLocs2.at(mapEntry.first).cbegin(), dotLocs2.at(mapEntry.first).cend(),
                std::inserter(commonElements, commonElements.begin()));
            count += commonElements.size();
        }
    }
    return count;
}

//----------------------------------------------------------------------------
// LayerElement functor methods
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
    ApplyPPUFactorParams *params = vrv_params_cast<ApplyPPUFactorParams *>(functorParams);
    assert(params);

    if (this->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    if (m_xAbs != VRV_UNSET) m_xAbs /= params->m_page->GetPPUFactor();

    return FUNCTOR_CONTINUE;
}

int LayerElement::AlignHorizontally(FunctorParams *functorParams)
{
    AlignHorizontallyParams *params = vrv_params_cast<AlignHorizontallyParams *>(functorParams);
    assert(params);

    // if (m_alignment) LogDebug("Element %s %s", this->GetUuid().c_str(), this->GetClassName().c_str());
    assert(!m_alignment);

    if (this->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    this->SetScoreDefRole(params->m_scoreDefRole);

    AlignmentType type = ALIGNMENT_DEFAULT;

    Chord *chordParent = dynamic_cast<Chord *>(this->GetFirstAncestor(CHORD, MAX_CHORD_DEPTH));
    Ligature *ligatureParent = dynamic_cast<Ligature *>(this->GetFirstAncestor(LIGATURE, MAX_LIGATURE_DEPTH));
    Note *noteParent = dynamic_cast<Note *>(this->GetFirstAncestor(NOTE, MAX_NOTE_DEPTH));
    Rest *restParent = dynamic_cast<Rest *>(this->GetFirstAncestor(REST, MAX_NOTE_DEPTH));
    TabGrp *tabGrpParent = dynamic_cast<TabGrp *>(this->GetFirstAncestor(TABGRP, MAX_TABGRP_DEPTH));
    const bool ligatureAsBracket = params->m_doc->GetOptions()->m_ligatureAsBracket.GetValue();

    if (chordParent) {
        m_alignment = chordParent->GetAlignment();
    }
    else if (noteParent) {
        m_alignment = noteParent->GetAlignment();
    }
    else if (restParent) {
        m_alignment = restParent->GetAlignment();
    }
    else if (tabGrpParent) {
        m_alignment = tabGrpParent->GetAlignment();
    }
    else if (this->Is({ DOTS, FLAG, STEM })) {
        assert(false);
    }
    else if (ligatureParent && this->Is(NOTE) && !ligatureAsBracket) {
        // Ligature notes are all aligned with the first note
        Note *note = vrv_cast<Note *>(this);
        assert(note);
        Note *firstNote = dynamic_cast<Note *>(ligatureParent->GetList(ligatureParent)->front());
        if (firstNote && (firstNote != note)) {
            m_alignment = firstNote->GetAlignment();
            m_alignment->AddLayerElementRef(this);
            double duration = this->GetAlignmentDuration(
                params->m_currentMensur, params->m_currentMeterSig, true, params->m_notationType);
            params->m_time += duration;
            return FUNCTOR_CONTINUE;
        }
    }
    // We do not align these (formely container). Any other?
    else if (this->Is({ BEAM, LIGATURE, FTREM, TUPLET })) {
        double duration = this->GetSameAsContentAlignmentDuration(
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
            params->m_currentMensur = vrv_cast<Mensur *>(this);
            assert(params->m_currentMensur);
            type = ALIGNMENT_MENSUR;
        }
    }
    else if (this->Is(METERSIG)) {
        if ((this->GetScoreDefRole() == SCOREDEF_SYSTEM) || (this->GetScoreDefRole() == SCOREDEF_INTERMEDIATE))
            type = ALIGNMENT_SCOREDEF_METERSIG;
        else if (this->GetScoreDefRole() == SCOREDEF_CAUTIONARY)
            type = ALIGNMENT_SCOREDEF_CAUTION_METERSIG;
        else if (this->GetParent() && this->GetParent()->Is(METERSIGGRP))
            type = ALIGNMENT_SCOREDEF_METERSIG;
        else {
            // replace the current meter signature
            params->m_currentMeterSig = vrv_cast<MeterSig *>(this);
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
        Dot *dot = vrv_cast<Dot *>(this);
        assert(dot);
        if (dot->m_drawingPreviousElement) {
            m_alignment = dot->m_drawingPreviousElement->GetAlignment();
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
    else if (this->Is(ARTIC)) {
        // Refer to the note parent
        Note *note = vrv_cast<Note *>(this->GetFirstAncestor(NOTE));
        assert(note);
        m_alignment = note->GetAlignment();
    }
    else if (this->Is(SYL)) {
        Staff *staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
        assert(staff);

        if (staff->m_drawingNotationType == NOTATIONTYPE_neume) {
            type = ALIGNMENT_DEFAULT;
        }
        else {
            Note *note = vrv_cast<Note *>(this->GetFirstAncestor(NOTE));
            assert(note);
            m_alignment = note->GetAlignment();
        }
    }
    else if (this->Is(VERSE)) {
        // Idem
        Note *note = vrv_cast<Note *>(this->GetFirstAncestor(NOTE));
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
        if (this->Is(TIMESTAMP_ATTR)) {
            params->m_time = duration;
        }
        else {
            params->m_measureAligner->SetMaxTime(params->m_time + duration);
        }

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
            Staff *staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
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
    SetAlignmentPitchPosParams *params = vrv_params_cast<SetAlignmentPitchPosParams *>(functorParams);
    assert(params);

    if (this->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    LayerElement *layerElementY = this;
    Staff *staffY = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staffY);
    Layer *layerY = vrv_cast<Layer *>(this->GetFirstAncestor(LAYER));
    assert(layerY);

    if (m_crossStaff && m_crossLayer) {
        layerElementY = m_crossLayer->GetAtPos(this->GetDrawingX());
        staffY = m_crossStaff;
        layerY = m_crossLayer;
    }

    // Adjust m_drawingYRel for notes and rests, etc.
    if (this->Is(ACCID)) {
        Accid *accid = vrv_cast<Accid *>(this);
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
            Custos *custos = vrv_cast<Custos *>(this->GetFirstAncestor(CUSTOS));
            if (custos) {
                m_alignment->AddToAccidSpace(
                    accid); // If this is not added, the custos is drawn an octave below the custos
            }
            else {
                // do something for accid that are not children of a note - e.g., mensural?
                this->SetDrawingYRel(
                    staffY->CalcPitchPosYRel(params->m_doc, accid->CalcDrawingLoc(layerY, layerElementY)));
            }
        }
    }
    else if (this->Is(CHORD)) {
        // The y position is set to the top note one
        int loc = PitchInterface::CalcLoc(this, layerY, layerElementY, true);
        this->SetDrawingYRel(staffY->CalcPitchPosYRel(params->m_doc, loc));
    }
    else if (this->Is(DOT)) {
        PositionInterface *interface = dynamic_cast<PositionInterface *>(this);
        assert(interface);
        this->SetDrawingYRel(staffY->CalcPitchPosYRel(params->m_doc, interface->CalcDrawingLoc(layerY, layerElementY)));
    }
    else if (this->Is(CUSTOS)) {
        Custos *custos = vrv_cast<Custos *>(this);
        assert(custos);
        int loc = 0;
        if (custos->HasPname()) {
            loc = PitchInterface::CalcLoc(custos, layerY, layerElementY);
        }
        int yRel = staffY->CalcPitchPosYRel(params->m_doc, loc);
        custos->SetDrawingLoc(loc);
        this->SetDrawingYRel(yRel);
    }
    else if (this->Is(NOTE)) {
        Note *note = vrv_cast<Note *>(this);
        assert(note);
        Chord *chord = note->IsChordTone();
        int loc = 0;
        TabGrp *tabGrp = note->IsTabGrpNote();
        if (tabGrp) {
            assert(staffY->m_drawingTuning);
            loc = staffY->m_drawingTuning->CalcPitchPos(
                note->GetTabCourse(), staffY->m_drawingNotationType, staffY->m_drawingLines);
        }
        else if (note->HasPname() || note->HasLoc()) {
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
        MRest *mRest = vrv_cast<MRest *>(this);
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
            Staff *staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
            assert(staff);
            loc = staff->m_drawingLines - 1;
            if (loc % 2 != 0) --loc;
            if (staff->m_drawingLines > 1) loc += 2;
            // Limitation: GetLayerCount does not take into account editorial markup
            // should be refined later
            bool hasMultipleLayer = (staffY->GetChildCount(LAYER) > 1);
            if (hasMultipleLayer) {
                loc = mRest->GetOptimalLayerLocation(staffY, layerY, loc);
            }
        }

        mRest->SetDrawingLoc(loc);
        this->SetDrawingYRel(staffY->CalcPitchPosYRel(params->m_doc, loc));
    }
    else if (this->Is(REST)) {
        Rest *rest = vrv_cast<Rest *>(this);
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
            Staff *staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
            assert(staff);
            loc = staff->m_drawingLines - 1;
            if ((rest->GetDur() < DUR_4) && (loc % 2 != 0)) --loc;
            // Adjust special cases
            if ((rest->GetDur() == DUR_1) && (staff->m_drawingLines > 1)) loc += 2;
            if ((rest->GetDur() == DUR_BR) && (staff->m_drawingLines < 2)) loc -= 2;

            // If within a beam, calculate the rest's height based on it's relationship to the notes that surround it
            Beam *beam = dynamic_cast<Beam *>(this->GetFirstAncestor(BEAM, 1));
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
                    LayerElement *layerElement = vrv_cast<LayerElement *>(*rit);
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
                    LayerElement *layerElement = vrv_cast<LayerElement *>(*it);
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
                // under the staff (d4 on treble) can not be moved any closer to center by an increment of 1
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

            Layer *layer = vrv_cast<Layer *>(this->GetFirstAncestor(LAYER));
            loc = rest->GetOptimalLayerLocation(staff, layer, loc);
        }
        rest->SetDrawingLoc(loc);
        this->SetDrawingYRel(staffY->CalcPitchPosYRel(params->m_doc, loc));
    }

    return FUNCTOR_CONTINUE;
}

int LayerElement::AdjustBeams(FunctorParams *functorParams)
{
    AdjustBeamParams *params = vrv_params_cast<AdjustBeamParams *>(functorParams);
    assert(params);

    // ignore elements that are not in the beam or are direct children of the beam
    if (!params->m_beam) return FUNCTOR_CONTINUE;
    if (!params->m_isOtherLayer && !Is(ACCID) && (GetFirstAncestor(BEAM) == params->m_beam) && !IsGraceNote())
        return FUNCTOR_CONTINUE;
    // ignore elements that are both on other layer and cross-staff
    if (params->m_isOtherLayer && m_crossStaff) return FUNCTOR_CONTINUE;
    // ignore specific elements, since they should not be influencing beam positioning
    if (Is({ BTREM, GRACEGRP, SPACE, TUPLET, TUPLET_BRACKET, TUPLET_NUM })) return FUNCTOR_CONTINUE;
    // ignore elements that start before the beam
    if (this->GetDrawingX() < params->m_x1) return FUNCTOR_CONTINUE;

    Staff *staff = vrv_cast<Staff *>(GetFirstAncestor(STAFF));
    assert(staff);

    // check if top/bottom of the element overlaps with beam coordinates
    int leftMargin = 0, rightMargin = 0;
    Beam *beam = vrv_cast<Beam *>(params->m_beam);
    const int beamCount = beam->m_beamSegment.GetAdjacentElementsDuration(GetDrawingX()) - DUR_8;
    const int currentBeamYLeft = params->m_y1 + params->m_beamSlope * (this->GetContentLeft() - params->m_x1);
    const int currentBeamYRight = params->m_y1 + params->m_beamSlope * (this->GetContentRight() - params->m_x1);
    if (params->m_directionBias > 0) {
        leftMargin = GetContentTop() - currentBeamYLeft + beamCount * beam->m_beamWidth + beam->m_beamWidthBlack;
        rightMargin = GetContentTop() - currentBeamYRight + beamCount * beam->m_beamWidth + beam->m_beamWidthBlack;
    }
    else {
        leftMargin = GetContentBottom() - currentBeamYLeft - beamCount * beam->m_beamWidth - beam->m_beamWidthBlack;
        rightMargin = GetContentBottom() - currentBeamYRight - beamCount * beam->m_beamWidth - beam->m_beamWidthBlack;
    }

    const int overlapMargin = std::max(leftMargin * params->m_directionBias, rightMargin * params->m_directionBias);
    if (overlapMargin >= params->m_directionBias * params->m_overlapMargin) {
        const int staffOffset = params->m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        params->m_overlapMargin
            = (((overlapMargin + staffOffset - 1) / staffOffset + 0.5) * staffOffset) * params->m_directionBias;
    }

    return FUNCTOR_CONTINUE;
}

int LayerElement::AdjustDots(FunctorParams *functorParams)
{
    AdjustDotsParams *params = vrv_params_cast<AdjustDotsParams *>(functorParams);
    assert(params);

    if (Is(NOTE) && GetParent()->Is(CHORD)) return FUNCTOR_SIBLINGS;
    if (Is(DOTS)) {
        params->m_dots.push_back(this);
    }
    else {
        params->m_elements.push_back(this);
    }

    return FUNCTOR_SIBLINGS;
}

int LayerElement::AdjustLayers(FunctorParams *functorParams)
{
    AdjustLayersParams *params = vrv_params_cast<AdjustLayersParams *>(functorParams);
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
    if (this->HasSelfBB()) {
        if (this->Is({ NOTE, STEM })) {
            params->m_current.push_back(this);
        }
        else if (!params->m_ignoreDots && this->Is(DOTS)) {
            params->m_current.push_back(this);
        }
    }

    // We are processing the first layer, nothing to do yet
    if (params->m_previous.empty()) return FUNCTOR_SIBLINGS;

    const int shift
        = AdjustOverlappingLayers(params->m_doc, params->m_previous, !params->m_ignoreDots, params->m_unison);
    params->m_accumulatedShift += shift;

    return FUNCTOR_SIBLINGS;
}

int LayerElement::AdjustOverlappingLayers(
    Doc *doc, const std::vector<LayerElement *> &otherElements, bool areDotsAdjusted, bool &isUnison)
{
    if (Is(NOTE) && GetParent()->Is(CHORD))
        return 0;
    else if (Is(STEM) && isUnison) {
        isUnison = false;
        return 0;
    }

    auto [margin, isInUnison] = CalcElementHorizontalOverlap(doc, otherElements, areDotsAdjusted, false);
    if (Is(NOTE)) {
        isUnison = isInUnison;
        if (isUnison) return 0;
    }

    if (Is({ DOTS, STEM })) {
        LayerElement *parent = vrv_cast<LayerElement *>(GetParent());
        assert(parent);
        parent->SetDrawingXRel(parent->GetDrawingXRel() + margin);
    }
    else {
        SetDrawingXRel(GetDrawingXRel() + margin);
    }
    return margin;
}

std::pair<int, bool> LayerElement::CalcElementHorizontalOverlap(Doc *doc,
    const std::vector<LayerElement *> &otherElements, bool areDotsAdjusted, bool isChordElement, bool isLowerElement,
    bool unison)
{
    Staff *staff = vrv_cast<Staff *>(GetFirstAncestor(STAFF));
    assert(staff);

    bool isInUnison = false;
    int shift = 0;

    for (int i = 0; i < int(otherElements.size()); ++i) {
        int verticalMargin = 0;
        int horizontalMargin = 2 * doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
        bool isUnisonElement = false;
        // Handle stem collisions
        if (Is(STEM)) {
            Stem *stem = vrv_cast<Stem *>(this);
            if (otherElements.at(i)->Is(NOTE)) {
                shift += stem->CompareToElementPosition(doc, otherElements.at(i), -shift);
            }
            else if (otherElements.at(i)->Is(DOTS) && HorizontalSelfOverlap(otherElements.at(i), horizontalMargin)) {
                shift += stem->HorizontalLeftOverlap(otherElements.at(i), doc, 0, 0) + horizontalMargin / 2;
            }
            if (shift) break;
        }
        // handle note collisions
        else if (Is(NOTE) && otherElements.at(i)->Is(NOTE)) {
            Note *currentNote = vrv_cast<Note *>(this);
            Note *previousNote = vrv_cast<Note *>(otherElements.at(i));
            assert(previousNote);
            isUnisonElement = currentNote->IsUnisonWith(previousNote, true);
            // Unisson, look at the duration for the note heads
            if (unison && currentNote->IsUnisonWith(previousNote, false)) {
                int previousDuration = previousNote->GetDrawingDur();
                const bool isPreviousCoord = previousNote->GetParent()->Is(CHORD);
                bool isEdgeElement = false;
                if (isPreviousCoord) {
                    Chord *parentChord = vrv_cast<Chord *>(previousNote->GetParent());
                    data_STEMDIRECTION stemDir = currentNote->GetDrawingStemDir();
                    previousDuration = parentChord->GetDur();
                    isEdgeElement = ((STEMDIRECTION_down == stemDir) && (parentChord->GetBottomNote() == previousNote))
                        || ((STEMDIRECTION_up == stemDir) && (parentChord->GetTopNote() == previousNote));
                }
                // Reduce the margin to 0 for whole notes unisson
                else if ((currentNote->GetDrawingDur() == DUR_1) && (previousDuration == DUR_1)) {
                    horizontalMargin = 0;
                }
                if (!isPreviousCoord || isEdgeElement || isChordElement) {
                    if ((currentNote->GetDrawingDur() == DUR_2) && (previousDuration == DUR_2)) {
                        isInUnison = true;
                        continue;
                    }
                    else if ((currentNote->GetDrawingDur() > DUR_2) && (previousDuration > DUR_2)) {
                        isInUnison = true;
                        continue;
                    }
                }
                else {
                    horizontalMargin *= -1;
                }
            }
            else if (previousNote->GetDrawingLoc() - currentNote->GetDrawingLoc() > 1) {
                continue;
            }
            else if (previousNote->GetDrawingLoc() - currentNote->GetDrawingLoc() == 1) {
                horizontalMargin = 0;
            }
            else if ((previousNote->GetDrawingLoc() - currentNote->GetDrawingLoc() < 0)
                && (previousNote->GetDrawingStemDir() != currentNote->GetDrawingStemDir()) /* && !isChordElement*/) {
                if (previousNote->GetDrawingLoc() - currentNote->GetDrawingLoc() == -1) {
                    horizontalMargin *= -1;
                }
                else if ((currentNote->GetDrawingDur() <= DUR_1) && (previousNote->GetDrawingDur() <= DUR_1)) {
                    continue;
                }
                else if (previousNote->m_crossStaff || m_crossStaff)
                    continue;
                else {
                    horizontalMargin *= -1;
                    verticalMargin = horizontalMargin;
                }
            }
        }
        // handle dot collisions
        else if (Is(DOTS) && !otherElements.at(i)->Is(DOTS) && areDotsAdjusted) {
            // No need for shift if dot is adjusted
            Dots *dot = vrv_cast<Dots *>(this);
            if (dot->IsAdjusted() || !HorizontalSelfOverlap(otherElements.at(i), horizontalMargin)) continue;

            if (otherElements.at(i)->Is({ NOTE, STEM })) {
                shift -= otherElements.at(i)->HorizontalLeftOverlap(this, doc, shift + horizontalMargin / 2, 0);
            }
            else {
                shift -= HorizontalRightOverlap(otherElements.at(i), doc, -shift, verticalMargin);
            }
        }

        if (Is(NOTE) && !otherElements.at(i)->Is(STEM)) {
            // Nothing to do if we have no vertical overlap
            if (!VerticalSelfOverlap(otherElements.at(i), verticalMargin)) continue;

            // Nothing to do either if we have no horizontal overlap
            if (!HorizontalSelfOverlap(otherElements.at(i), horizontalMargin + shift)) continue;

            if (horizontalMargin < 0 || isLowerElement) {
                shift -= HorizontalRightOverlap(otherElements.at(i), doc, -shift, verticalMargin);
                if (!isUnisonElement) shift -= horizontalMargin;
            }
            else if ((horizontalMargin >= 0) || isChordElement) {
                shift += HorizontalLeftOverlap(otherElements.at(i), doc, horizontalMargin - shift, verticalMargin);

                // Make additional adjustments for cross-staff and unison notes
                if (m_crossStaff) shift -= horizontalMargin;
                if (isInUnison) shift *= -1;
            }
            else {
                // Otherwise move the appropriate parent to the right
                shift -= horizontalMargin
                    - HorizontalRightOverlap(otherElements.at(i), doc, horizontalMargin - shift, verticalMargin);
            }
        }
        else if (this->Is(NOTE)) {
            Note *currentNote = vrv_cast<Note *>(this);
            assert(currentNote);
            if ((currentNote->GetDrawingDur() == DUR_1) && otherElements.at(i)->Is(STEM)) {
                const int horizontalMargin = doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
                Stem *stem = vrv_cast<Stem *>(otherElements.at(i));
                data_STEMDIRECTION stemDir = stem->GetDrawingStemDir();
                if (this->HorizontalLeftOverlap(otherElements.at(i), doc, 0, 0) != 0) {
                    shift = 3 * horizontalMargin;
                    if (stemDir == STEMDIRECTION_up) {
                        shift *= -1;
                    }
                }
            }
        }
    }

    // If note is not in unison, has accidental and were to be shifted to the right - shift it to the left
    // That way accidental will be near note that actually has accidental and not near lowest-layer note
    if (Is(NOTE) && isChordElement && unison && (shift > 0)) {
        Note *currentNote = vrv_cast<Note *>(this);
        if (currentNote->GetDrawingAccid()) shift *= -1;
    }

    return { shift, isInUnison };
}

int LayerElement::AdjustGraceXPos(FunctorParams *functorParams)
{
    AdjustGraceXPosParams *params = vrv_params_cast<AdjustGraceXPosParams *>(functorParams);
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
        // Also move the accumulated x shift and the minimum position for the next alignment accordingly
        params->m_graceCumulatedXShift += (-offset);
        params->m_graceUpcomingMaxPos += (-offset);
    }

    int selfLeft = this->GetSelfLeft()
        - params->m_doc->GetLeftMargin(this) * params->m_doc->GetDrawingUnit(params->m_doc->GetCueSize(100));

    params->m_graceUpcomingMaxPos = std::min(selfLeft, params->m_graceUpcomingMaxPos);

    return FUNCTOR_SIBLINGS;
}

int LayerElement::AdjustTupletNumOverlap(FunctorParams *functorParams)
{
    AdjustTupletNumOverlapParams *params = vrv_params_cast<AdjustTupletNumOverlapParams *>(functorParams);
    assert(params);

    if (!Is({ ARTIC, ACCID, CHORD, DOT, FLAG, NOTE, REST, STEM }) || !HasSelfBB()) return FUNCTOR_CONTINUE;

    if (params->m_ignoreCrossStaff && Is({ CHORD, NOTE, REST }) && m_crossStaff) return FUNCTOR_SIBLINGS;

    if (!params->m_tupletNum->HorizontalSelfOverlap(this, params->m_horizontalMargin)
        && !params->m_tupletNum->VerticalSelfOverlap(this, params->m_verticalMargin)) {
        return FUNCTOR_CONTINUE;
    }

    if (params->m_drawingNumPos == STAFFREL_basic_above) {
        int dist = GetSelfTop();
        if (params->m_yRel < dist) params->m_yRel = dist;
    }
    else {
        int dist = GetSelfBottom();
        if (params->m_yRel > dist) params->m_yRel = dist;
    }

    return FUNCTOR_CONTINUE;
}

int LayerElement::AdjustXPos(FunctorParams *functorParams)
{
    AdjustXPosParams *params = vrv_params_cast<AdjustXPosParams *>(functorParams);
    assert(params);

    if (this->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    // we should have processed aligned before
    assert(this->GetAlignment());

    if (!this->HasToBeAligned()) {
        // if nothing to do with this type of element
        // this happens for example with Artic where only ArticPart children are aligned
        return FUNCTOR_SIBLINGS;
    }

    // If we have a list of types to excludes and it is one of them, stop it
    if (!params->m_excludes.empty() && this->Is(params->m_excludes)) {
        return FUNCTOR_CONTINUE;
    }

    // If we have a list of types to include and it is not one of them, stop it
    if (!params->m_includes.empty() && !this->Is(params->m_includes)) {
        return FUNCTOR_CONTINUE;
    }

    // If desired only handle barlines which are right positioned
    if (params->m_rightBarLinesOnly && this->Is(BARLINE)) {
        if (vrv_cast<BarLine *>(this)->GetPosition() != BarLinePosition::Right) {
            return FUNCTOR_CONTINUE;
        }
    }

    if (this->HasSameasLink()) {
        // nothing to do when the element has a @sameas attribute
        return FUNCTOR_SIBLINGS;
    }

    if (this->GetAlignment()->GetType() == ALIGNMENT_CLEF) {
        return FUNCTOR_CONTINUE;
    }

    int offset = 0;
    int selfLeft;
    int drawingUnit = params->m_doc->GetDrawingUnit(params->m_staffSize);

    // Nested aligment of bounding boxes is performed only when both the previous alignment and
    // the current one allow it. For example, when one of them is a barline, we do not look how
    // bounding boxes can be nested but instead only look at the horizontal position
    bool performBoundingBoxAlignment = (params->m_previousAlignment.m_alignment
        && params->m_previousAlignment.m_alignment->PerfomBoundingBoxAlignment()
        && this->GetAlignment()->PerfomBoundingBoxAlignment());

    if (!this->HasSelfBB() || this->HasEmptyBB()) {
        // if nothing was drawn, do not take it into account
        // This should happen for barline position none but also chords in beam. Otherwise the BB should be set to
        // empty with
        // Object::SetEmptyBB()
        // LogDebug("Nothing drawn for '%s' '%s'", this->GetClassName().c_str(), this->GetUuid().c_str());
        selfLeft = this->GetAlignment()->GetXRel();
    }
    else {
        // We add it to the upcoming bouding boxes
        params->m_upcomingBoundingBoxes.push_back(this);
        params->m_currentAlignment.m_alignment = GetAlignment();
        // Here we look how bounding boxes overlap and adjust the position only when necessary
        if (performBoundingBoxAlignment) {
            selfLeft = this->GetAlignment()->GetXRel();
            // If we want the nesting to be reduced, we can set to:
            // selfLeft = this->GetSelfLeft();
            // This could be made an option (--spacing-limited-nesting)
            int selfLeftMargin = params->m_doc->GetLeftMargin(this);
            int overlap = 0;
            for (auto &boundingBox : params->m_boundingBoxes) {
                LayerElement *element = vrv_cast<LayerElement *>(boundingBox);
                assert(element);
                int margin = (params->m_doc->GetRightMargin(element) + selfLeftMargin) * drawingUnit;
                bool hasOverlap = this->HorizontalContentOverlap(boundingBox, margin);

                if (hasOverlap) {
                    // For note to note alignment, make sure there is a standard spacing even if they to not overlap
                    // vertically
                    if (this->Is(NOTE) && element->Is(NOTE)) {
                        overlap = std::max(overlap, element->GetSelfRight() - this->GetSelfLeft() + margin);
                    }
                    else {
                        overlap = std::max(overlap, boundingBox->HorizontalRightOverlap(this, params->m_doc, margin));
                    }
                    // LogDebug("%s overlaps of %d, margin %d", this->GetClassName().c_str(), overlap, margin);
                }
            }
            offset -= overlap;
        }
        // Otherwise only look at the horizontal position
        else {
            selfLeft = this->GetSelfLeft();
            selfLeft -= params->m_doc->GetLeftMargin(this) * params->m_doc->GetDrawingUnit(100);
        }
    }

    offset = std::min(offset, selfLeft - params->m_minPos);
    if (offset < 0) {
        this->GetAlignment()->SetXRel(this->GetAlignment()->GetXRel() - offset);
        // Also move the accumulated x shift and the minimum position for the next alignment accordingly
        params->m_cumulatedXShift += (-offset);
        params->m_upcomingMinPos += (-offset);
    }

    int selfRight = this->GetAlignment()->GetXRel();
    if (!this->HasSelfBB() || this->HasEmptyBB()) {
        selfRight = this->GetAlignment()->GetXRel() + params->m_doc->GetRightMargin(this) * drawingUnit;
    }
    else {
        selfRight = this->GetSelfRight() + params->m_doc->GetRightMargin(this) * drawingUnit;
    }

    // In case of dots/flags we need to hold off of adjusting upcoming min position right away - if it happens that
    // these elements do not overlap with other elements we can draw them as is and save space
    AlignmentReference *currentReference = GetAlignment()->GetReferenceWithElement(this, params->m_staffN);
    Alignment *nextAlignment = vrv_cast<Alignment *>(GetAlignment()->GetParent()->GetNext(GetAlignment(), ALIGNMENT));
    AlignmentType next = nextAlignment ? nextAlignment->GetType() : ALIGNMENT_DEFAULT;
    if (Is({ DOTS, FLAG }) && (currentReference->HasMultipleLayer() || currentReference->HasCrossStaffElements())
        && (next != ALIGNMENT_MEASURE_RIGHT_BARLINE)) {
        const int additionalOffset = selfRight - params->m_upcomingMinPos;
        if (additionalOffset > params->m_currentAlignment.m_offset) {
            params->m_currentAlignment.m_offset = additionalOffset;
            params->m_currentAlignment.m_overlappingBB = this;
        }
    }
    else {
        params->m_upcomingMinPos = std::max(selfRight, params->m_upcomingMinPos);
    }

    auto it = std::find_if(params->m_measureTieEndpoints.begin(), params->m_measureTieEndpoints.end(),
        [this](const std::pair<LayerElement *, LayerElement *> &pair) { return pair.second == this; });
    if (it != params->m_measureTieEndpoints.end()) {
        const int minTiedDistance = 7 * drawingUnit;
        const int alignmentDistance = it->second->GetAlignment()->GetXRel() - it->first->GetAlignment()->GetXRel();
        if ((alignmentDistance < minTiedDistance)
            && ((this->GetFirstAncestor(CHORD) != NULL) || (it->first->FindDescendantByType(FLAG) != NULL))) {
            const int adjust = minTiedDistance - alignmentDistance;
            this->GetAlignment()->SetXRel(this->GetAlignment()->GetXRel() + adjust);
            // Also move the accumulated x shift and the minimum position for the next alignment accordingly
            params->m_cumulatedXShift += adjust;
            params->m_upcomingMinPos += adjust;
        }
    }

    return FUNCTOR_SIBLINGS;
}

int LayerElement::AdjustXRelForTranscription(FunctorParams *)
{
    if (m_xAbs == VRV_UNSET) return FUNCTOR_CONTINUE;

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
        Note const *note = vrv_cast<Note const *>(this);
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
        Accid const *accid = vrv_cast<Accid *>(this);
        assert(accid);
        if (accid->GetFunc() == accidLog_FUNC_edit)
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
    PrepareCrossStaffParams *params = vrv_params_cast<PrepareCrossStaffParams *>(functorParams);
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

    Staff *parentStaff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(parentStaff);
    // Check if we have a cross-staff to itself...
    if (m_crossStaff == parentStaff) {
        LogWarning("The cross staff reference '%d' for element '%s' seems to be identical to the parent staff",
            durElement->GetStaff().at(0), this->GetUuid().c_str());
        m_crossStaff = NULL;
        return FUNCTOR_CONTINUE;
    }

    Layer *parentLayer = vrv_cast<Layer *>(this->GetFirstAncestor(LAYER));
    assert(parentLayer);
    // Now try to get the corresponding layer - for now look for the same layer @n
    int layerN = parentLayer->GetN();
    // When we will have allowed @layer in <note>, we will have to do:
    // int layerN = durElement->HasLayer() ? durElement->GetLayer() : (*currentLayer)->GetN();
    AttNIntegerComparison comparisonFirstLayer(LAYER, layerN);
    bool direction = (parentStaff->GetN() < m_crossStaff->GetN()) ? FORWARD : BACKWARD;
    m_crossLayer = dynamic_cast<Layer *>(m_crossStaff->FindDescendantByComparison(&comparisonFirstLayer, 1));
    if (!m_crossLayer) {
        // Just try to pick the first one... (i.e., last one when crossing above)
        m_crossLayer = dynamic_cast<Layer *>(m_crossStaff->FindDescendantByType(LAYER, UNSPECIFIED, direction));
    }
    if (!m_crossLayer) {
        // Nothing we can do
        LogWarning("Could not get the layer with cross-staff reference '%d' for element '%s'",
            durElement->GetStaff().at(0), this->GetUuid().c_str());
        m_crossStaff = NULL;
    }

    if (direction == FORWARD) {
        m_crossLayer->SetCrossStaffFromAbove(true);
    }
    else {
        m_crossLayer->SetCrossStaffFromBelow(true);
    }

    params->m_currentCrossStaff = m_crossStaff;
    params->m_currentCrossLayer = m_crossLayer;

    return FUNCTOR_CONTINUE;
}

int LayerElement::PrepareCrossStaffEnd(FunctorParams *functorParams)
{
    PrepareCrossStaffParams *params = vrv_params_cast<PrepareCrossStaffParams *>(functorParams);
    assert(params);

    if (this->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    DurationInterface *durInterface = this->GetDurationInterface();
    if (durInterface) {
        // If we have  @staff, set reset it to NULL - this can be problematic if we have different @staff attributes
        // in the the children of one element. We do not consider this now because it seems over the top
        // We would need to look at the @n attribute and to have a stack to handle this properly
        if (durInterface->HasStaff()) {
            params->m_currentCrossStaff = NULL;
            params->m_currentCrossLayer = NULL;
        }
    }
    else if (this->Is({ BEAM, BTREM, FTREM, TUPLET })) {
        // For other elements (e.g., beams, tuplets) check if all their children duration element are cross-staff
        // If yes, make them cross-staff themselves.
        ListOfObjects durations;
        InterfaceComparison hasInterface(INTERFACE_DURATION);
        this->FindAllDescendantByComparison(&durations, &hasInterface);
        Staff *crossStaff = NULL;
        Layer *crossLayer = NULL;
        for (auto object : durations) {
            LayerElement *durElement = vrv_cast<LayerElement *>(object);
            assert(durElement);
            // The duration element is not cross-staff, of the cross-staff is not that same staff (very rare)
            if (!durElement->m_crossStaff || (crossStaff && (durElement->m_crossStaff != crossStaff))) {
                crossStaff = NULL;
                // We can stop here
                break;
            }
            else {
                crossStaff = durElement->m_crossStaff;
                crossLayer = durElement->m_crossLayer;
            }
        }
        if (crossStaff) {
            m_crossStaff = crossStaff;
            m_crossLayer = crossLayer;
        }
    }

    return FUNCTOR_CONTINUE;
}

int LayerElement::PreparePointersByLayer(FunctorParams *functorParams)
{
    PreparePointersByLayerParams *params = vrv_params_cast<PreparePointersByLayerParams *>(functorParams);
    assert(params);

    if (this->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    // Skip ligatures because we want it attached to the first note in it
    if (params->m_lastDot && !this->Is(LIGATURE)) {
        params->m_lastDot->m_drawingNextElement = this;
        params->m_lastDot = NULL;
    }
    if (this->Is(BARLINE)) {
        // Do not attach a note when a barline is passed
        params->m_currentElement = NULL;
    }
    else if (this->Is({ NOTE, REST })) {
        params->m_currentElement = this;
    }

    return FUNCTOR_CONTINUE;
}

int LayerElement::PrepareDelayedTurns(FunctorParams *functorParams)
{
    PrepareDelayedTurnsParams *params = vrv_params_cast<PrepareDelayedTurnsParams *>(functorParams);
    assert(params);

    // We are initializing the params->m_delayedTurns map
    if (params->m_initMap) return FUNCTOR_CONTINUE;

    if (!this->HasInterface(INTERFACE_DURATION)) return FUNCTOR_CONTINUE;

    if (params->m_previousElement) {
        assert(params->m_currentTurn);
        params->m_currentTurn->m_drawingEndElement = this;
        params->m_currentTurn = NULL;
        params->m_previousElement = NULL;
    }

    if (params->m_delayedTurns.count(this)) {
        params->m_previousElement = this;
        params->m_currentTurn = params->m_delayedTurns.at(this);
    }

    return FUNCTOR_CONTINUE;
}

int LayerElement::PrepareTimePointing(FunctorParams *functorParams)
{
    PrepareTimePointingParams *params = vrv_params_cast<PrepareTimePointingParams *>(functorParams);
    assert(params);

    if (this->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    // Do not look for tstamp pointing to these
    if (this->Is({ ARTIC, BEAM, FLAG, TUPLET, STEM, VERSE })) return FUNCTOR_CONTINUE;

    ListOfPointingInterClassIdPairs::iterator iter = params->m_timePointingInterfaces.begin();
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
    PrepareTimeSpanningParams *params = vrv_params_cast<PrepareTimeSpanningParams *>(functorParams);
    assert(params);

    if (this->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    // Do not look for tstamp pointing to these
    if (this->Is({ ARTIC, BEAM, FLAG, TUPLET, STEM, VERSE })) return FUNCTOR_CONTINUE;

    ListOfSpanningInterClassIdPairs::iterator iter = params->m_timeSpanningInterfaces.begin();
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
    LayerCountInTimeSpanParams *params = vrv_params_cast<LayerCountInTimeSpanParams *>(functorParams);
    assert(params);

    if (this->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    // For mRest we do not look at the time span
    if (this->Is(MREST)) {
        // Add the layerN to the list of layer elements occuring in this time frame
        params->m_layers.insert(this->GetAlignmentLayerN());

        return FUNCTOR_SIBLINGS;
    }

    if (!this->GetDurationInterface() || this->Is(MSPACE) || this->Is(SPACE) || this->HasSameasLink())
        return FUNCTOR_CONTINUE;
    if (this->Is(NOTE) && this->GetParent()->Is(CHORD)) return FUNCTOR_CONTINUE;

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

    // Add the layerN to the list of layer elements occuring in this time frame
    params->m_layers.insert(this->GetAlignmentLayerN());

    // Not need to recurse for chords? Not quite sure about it.
    return (this->Is(CHORD)) ? FUNCTOR_SIBLINGS : FUNCTOR_CONTINUE;
}

int LayerElement::LayerElementsInTimeSpan(FunctorParams *functorParams)
{
    LayerElementsInTimeSpanParams *params = vrv_params_cast<LayerElementsInTimeSpanParams *>(functorParams);
    assert(params);

    Layer *currentLayer = vrv_cast<Layer *>(GetFirstAncestor(LAYER));
    // Either get layer refernced by @m_layer or all layers but it, depending on the @m_allLayersButCurrent flag
    if ((!params->m_allLayersButCurrent && (currentLayer != params->m_layer))
        || (params->m_allLayersButCurrent && (currentLayer == params->m_layer))) {
        return FUNCTOR_SIBLINGS;
    }
    if (!currentLayer || IsScoreDefElement() || Is(MREST)) return FUNCTOR_SIBLINGS;
    if (!GetDurationInterface() || Is(MSPACE) || Is(SPACE) || HasSameasLink()) return FUNCTOR_CONTINUE;

    const double duration = !GetParent()->Is(CHORD)
        ? GetAlignmentDuration(params->m_mensur, params->m_meterSig)
        : vrv_cast<Chord *>(GetParent())->GetAlignmentDuration(params->m_mensur, params->m_meterSig);

    const double time = m_alignment->GetTime();

    // The event is starting after the end of the element
    if ((time + duration) <= params->m_time) return FUNCTOR_CONTINUE;
    // The element is starting after the event end - we can stop here
    if (time >= (params->m_time + params->m_duration)) return FUNCTOR_STOP;

    params->m_elements.push_back(this);

    // Not need to recurse for chords
    return Is(CHORD) ? FUNCTOR_SIBLINGS : FUNCTOR_CONTINUE;
}

int LayerElement::FindSpannedLayerElements(FunctorParams *functorParams)
{
    FindSpannedLayerElementsParams *params = vrv_params_cast<FindSpannedLayerElementsParams *>(functorParams);
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
    CalcOnsetOffsetParams *params = vrv_params_cast<CalcOnsetOffsetParams *>(functorParams);
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
        Note *note = vrv_cast<Note *>(element);
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
        BeatRpt *rpt = vrv_cast<BeatRpt *>(element);
        assert(rpt);

        incrementScoreTime = rpt->GetAlignmentDuration(
            params->m_currentMensur, params->m_currentMeterSig, true, params->m_notationType);
        incrementScoreTime = incrementScoreTime / (DUR_MAX / DURATION_4);
        rpt->SetScoreTimeOnset(params->m_currentScoreTime);
        params->m_currentScoreTime += incrementScoreTime;
        params->m_currentRealTimeSeconds += incrementScoreTime * 60.0 / params->m_currentTempo;
    }
    else if (this->Is({ BEAM, LIGATURE, FTREM, TUPLET }) && this->HasSameasLink()) {
        incrementScoreTime = this->GetSameAsContentAlignmentDuration(
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
    GenerateMIDIParams *params = vrv_params_cast<GenerateMIDIParams *>(functorParams);
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
    GenerateTimemapParams *params = vrv_params_cast<GenerateTimemapParams *>(functorParams);
    assert(params);

    if (this->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    if (this->HasSameasLink()) {
        assert(this->GetSameasLink());
        this->GetSameasLink()->Process(params->m_functor, functorParams);
    }

    return FUNCTOR_CONTINUE;
}

int LayerElement::ResetDrawing(FunctorParams *functorParams)
{
    m_drawingCueSize = false;
    m_crossStaff = NULL;
    m_crossLayer = NULL;

    // Pass it to the pseudo functor of the interface
    LinkingInterface *interface = this->GetLinkingInterface();
    assert(interface);
    interface->InterfaceResetDrawing(functorParams, this);

    return FUNCTOR_CONTINUE;
}

int LayerElement::GetRelativeLayerElement(FunctorParams *functorParams)
{
    GetRelativeLayerElementParams *params = vrv_params_cast<GetRelativeLayerElementParams *>(functorParams);
    assert(params);

    // Do not check for index of the element if we're looking into neighboring layer or if nested element is being
    // processed (e.g. ignore index children of beams, since they have their own indices irrelevant to the one that
    // has been passed inside this functor)
    if (!params->m_isInNeighboringLayer && GetParent()->Is(LAYER)) {
        if (params->m_searchDirection == FORWARD && (GetIdx() < params->m_initialElementId)) return FUNCTOR_SIBLINGS;
        if (params->m_searchDirection == BACKWARD && (GetIdx() > params->m_initialElementId)) return FUNCTOR_SIBLINGS;
    }

    if (Is({ NOTE, CHORD, FTREM })) {
        params->m_relativeElement = this;
        return FUNCTOR_STOP;
    }

    if (Is(REST)) return params->m_isInNeighboringLayer ? FUNCTOR_STOP : FUNCTOR_SIBLINGS;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
