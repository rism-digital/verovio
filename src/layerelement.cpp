/////////////////////////////////////////////////////////////////////////////
// Name:        layerelement.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "layerelement.h"

//----------------------------------------------------------------------------

#include <algorithm>
#include <cassert>
#include <climits>
#include <iterator>
#include <math.h>
#include <numeric>

//----------------------------------------------------------------------------

#include "accid.h"
#include "alignfunctor.h"
#include "barline.h"
#include "beam.h"
#include "beamspan.h"
#include "beatrpt.h"
#include "btrem.h"
#include "chord.h"
#include "clef.h"
#include "comparison.h"
#include "custos.h"
#include "doc.h"
#include "dot.h"
#include "elementpart.h"
#include "fraction.h"
#include "ftrem.h"
#include "functor.h"
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
#include "proport.h"
#include "rest.h"
#include "slur.h"
#include "smufl.h"
#include "space.h"
#include "staff.h"
#include "stem.h"
#include "syl.h"
#include "syllable.h"
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

// Large spacing between syllables is a quarter note space
#define NEUME_LARGE_SPACE Fraction(1, 4)
// Medium spacing between neume is a 8th note space
#define NEUME_MEDIUM_SPACE Fraction(1, 8)
// Small spacing between neume components is a 16th note space
#define NEUME_SMALL_SPACE Fraction(1, 16)

//----------------------------------------------------------------------------
// LayerElement
//----------------------------------------------------------------------------

LayerElement::LayerElement()
    : Object(LAYER_ELEMENT), FacsimileInterface(), LinkingInterface(), AttCoordX1(), AttLabelled(), AttTyped()
{
    this->RegisterInterface(FacsimileInterface::GetAttClasses(), FacsimileInterface::IsInterface());
    this->RegisterInterface(LinkingInterface::GetAttClasses(), LinkingInterface::IsInterface());
    this->RegisterAttClass(ATT_COORDX1);
    this->RegisterAttClass(ATT_LABELLED);
    this->RegisterAttClass(ATT_TYPED);

    this->Reset();
}

LayerElement::LayerElement(ClassId classId)
    : Object(classId), FacsimileInterface(), LinkingInterface(), AttCoordX1(), AttLabelled(), AttTyped()
{
    this->RegisterInterface(FacsimileInterface::GetAttClasses(), FacsimileInterface::IsInterface());
    this->RegisterInterface(LinkingInterface::GetAttClasses(), LinkingInterface::IsInterface());
    this->RegisterAttClass(ATT_COORDX1);
    this->RegisterAttClass(ATT_LABELLED);
    this->RegisterAttClass(ATT_TYPED);

    this->Reset();
}

void LayerElement::Reset()
{
    Object::Reset();
    FacsimileInterface::Reset();
    LinkingInterface::Reset();
    this->ResetCoordX1();
    this->ResetLabelled();
    this->ResetTyped();

    m_drawingFacsX = VRV_UNSET;
    m_drawingYRel = 0;
    m_drawingFacsY = VRV_UNSET;
    m_drawingXRel = 0;
    m_drawingCueSize = false;

    m_cachedYRel = VRV_UNSET;
    m_cachedXRel = VRV_UNSET;

    m_scoreDefRole = SCOREDEF_NONE;
    m_alignment = NULL;
    m_graceAlignment = NULL;
    m_alignmentLayerN = VRV_UNSET;

    m_crossStaff = NULL;
    m_crossLayer = NULL;

    m_isInBeamspan = false;
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

LayerElement *LayerElement::ThisOrSameasLink()
{
    return const_cast<LayerElement *>(std::as_const(*this).ThisOrSameasLink());
}

const LayerElement *LayerElement::ThisOrSameasLink() const
{
    if (!this->HasSameasLink()) {
        return this;
    }

    assert(this->GetSameasLink());

    return dynamic_cast<const LayerElement *>(this->GetSameasLink());
}

bool LayerElement::IsGraceNote() const
{
    // First, regardless of the type, check whether it's part of GRACEGRP
    if (this->GetFirstAncestor(GRACEGRP)) return true;
    // For note, we need to look at it or at the parent chord
    if (this->Is(NOTE)) {
        const Note *note = vrv_cast<const Note *>(this);
        assert(note);
        const Chord *chord = note->IsChordTone();
        return (chord) ? chord->HasGrace() : (note->HasGrace());
    }
    else if (this->Is(CHORD)) {
        const Chord *chord = vrv_cast<const Chord *>(this);
        assert(chord);
        return (chord->HasGrace());
    }
    else if (this->Is(TUPLET)) {
        ClassIdsComparison matchType({ NOTE, CHORD });
        ArrayOfObjects children;
        const LayerElement *child = dynamic_cast<const LayerElement *>(this->FindDescendantByComparison(&matchType));
        if (child) return child->IsGraceNote();
    }
    // For accid, artic, etc.. look at the parent note / chord
    else {
        // For an accid we expect to be the child of a note - the note will lookup at the chord parent in necessary
        const Note *note = vrv_cast<const Note *>(this->GetFirstAncestor(NOTE, MAX_ACCID_DEPTH));
        if (note) return note->IsGraceNote();
        // For an artic we can be direct child of a chord
        const Chord *chord = vrv_cast<const Chord *>(this->GetFirstAncestor(CHORD, MAX_ACCID_DEPTH));
        if (chord) return chord->IsGraceNote();
    }
    return false;
}

bool LayerElement::IsInLigature() const
{
    if (!this->Is(NOTE)) return false;
    return (this->GetFirstAncestor(LIGATURE, MAX_LIGATURE_DEPTH));
}

FTrem *LayerElement::GetAncestorFTrem()
{
    return const_cast<FTrem *>(std::as_const(*this).GetAncestorFTrem());
}

const FTrem *LayerElement::GetAncestorFTrem() const
{
    if (!this->Is({ CHORD, NOTE })) return NULL;
    return vrv_cast<const FTrem *>(this->GetFirstAncestor(FTREM, MAX_FTREM_DEPTH));
}

Beam *LayerElement::GetAncestorBeam()
{
    return const_cast<Beam *>(std::as_const(*this).GetAncestorBeam());
}

const Beam *LayerElement::GetAncestorBeam() const
{
    if (!this->Is({ CHORD, NOTE, REST, TABGRP, TABDURSYM, STEM })) return NULL;
    const Beam *beamParent = vrv_cast<const Beam *>(this->GetFirstAncestor(BEAM));
    if (this->Is(REST)) return beamParent;

    if (beamParent != NULL) {
        if (!this->IsGraceNote()) return beamParent;
        // This note is beamed and cue-sized - we will be able to get rid of this once MEI has a better modeling for
        // beamed grace notes
        const LayerElement *graceElement = this;
        if (this->Is(STEM)) {
            graceElement = vrv_cast<const LayerElement *>(this->GetFirstAncestor(NOTE));
            if (!graceElement) graceElement = vrv_cast<const LayerElement *>(this->GetFirstAncestor(CHORD));
            assert(graceElement);
        }
        // Make sure the object list is set
        beamParent->GetList();
        // If the note is part of the beam parent, this means we have a beam of graced notes
        if (beamParent->GetListIndex(graceElement) > -1) {
            return beamParent;
        }
        // otherwise it is a non-beamed grace note within a beam - return NULL
    }
    return NULL;
}

int LayerElement::GetOriginalLayerN() const
{
    int layerN = this->GetAlignmentLayerN();
    if (layerN < 0) {
        layerN = vrv_cast<const Layer *>(this->GetFirstAncestor(LAYER))->GetN();
    }
    return layerN;
}

void LayerElement::SetIsInBeamSpan(bool isInBeamSpan)
{
    if (!this->Is({ CHORD, NOTE, REST })) return;
    m_isInBeamspan = isInBeamSpan;
}

bool LayerElement::IsInBeam() const
{
    return (this->GetAncestorBeam() || this->GetIsInBeamSpan());
}

Staff *LayerElement::GetAncestorStaff(const StaffSearch strategy, const bool assertExistence)
{
    return const_cast<Staff *>(std::as_const(*this).GetAncestorStaff(strategy, assertExistence));
}

const Staff *LayerElement::GetAncestorStaff(const StaffSearch strategy, const bool assertExistence) const
{
    const Staff *staff = NULL;
    if (strategy == RESOLVE_CROSS_STAFF) {
        const Layer *layer = NULL;
        staff = this->GetCrossStaff(layer);
    }
    if (!staff) staff = vrv_cast<const Staff *>(this->GetFirstAncestor(STAFF));
    if (assertExistence) assert(staff);
    return staff;
}

Staff *LayerElement::GetCrossStaff(Layer *&layer)
{
    const Layer *layerRef = NULL;
    const Staff *staffRef = std::as_const(*this).GetCrossStaff(layerRef);
    layer = const_cast<Layer *>(layerRef);
    return const_cast<Staff *>(staffRef);
}

const Staff *LayerElement::GetCrossStaff(const Layer *&layer) const
{
    if (m_crossStaff) {
        assert(m_crossLayer);
        layer = m_crossLayer;
        return m_crossStaff;
    }

    const LayerElement *parent
        = dynamic_cast<const LayerElement *>(this->GetFirstAncestorInRange(LAYER_ELEMENT, LAYER_ELEMENT_max));

    if (parent) return parent->GetCrossStaff(layer);

    return NULL;
}

data_STAFFREL_basic LayerElement::GetCrossStaffRel() const
{
    if (!m_crossStaff) return STAFFREL_basic_NONE;

    const Staff *staff = this->GetAncestorStaff();

    return (m_crossStaff->GetN() < staff->GetN()) ? STAFFREL_basic_above : STAFFREL_basic_below;
}

void LayerElement::GetOverflowStaffAlignments(StaffAlignment *&above, StaffAlignment *&below)
{
    Staff *staff = this->GetAncestorStaff(RESOLVE_CROSS_STAFF);

    // By default use the alignment of the staff
    above = staff->GetAlignment();
    below = above;

    // Dots, flags and stems with cross-staff chords need special treatment
    this->GetChordOverflow(above, below, staff->GetN());

    // Stems cross-staff beam need special treatment but only if the beam itself is not cross-staff
    if (this->Is({ ARTIC, STEM })) {
        if (this->GetFirstAncestor(BEAM)) {
            Beam *beam = vrv_cast<Beam *>(this->GetFirstAncestor(BEAM));
            if (!beam->m_crossStaff) beam->GetBeamChildOverflow(above, below);
        }
        else if (this->GetFirstAncestor(FTREM)) {
            FTrem *fTrem = vrv_cast<FTrem *>(this->GetFirstAncestor(FTREM));
            if (!fTrem->m_crossStaff) fTrem->GetBeamChildOverflow(above, below);
        }
    }
    // Beams in cross-staff situation need special treatment
    else if (this->Is({ BEAM, FTREM }) && !m_crossStaff) {
        BeamDrawingInterface *interface = this->GetBeamDrawingInterface();
        assert(interface);
        interface->GetBeamOverflow(above, below);
    }
}

void LayerElement::GetChordOverflow(StaffAlignment *&above, StaffAlignment *&below, int staffN)
{
    Chord *chord = vrv_cast<Chord *>(this->GetFirstAncestor(CHORD));
    // Dots, flags and stems with cross-staff chords need special treatment
    if (this->Is({ DOTS, FLAG, STEM }) && chord && chord->HasCrossStaff()) {
        Staff *staffAbove = NULL;
        Staff *staffBelow = NULL;
        chord->GetCrossStaffExtremes(staffAbove, staffBelow);
        if (staffAbove && (staffAbove->GetN() < staffN)) {
            above = staffAbove->GetAlignment();
        }
        if (staffBelow && (staffBelow->GetN() > staffN)) {
            below = staffBelow->GetAlignment();
        }
    }
}

Alignment *LayerElement::GetGraceAlignment()
{
    return const_cast<Alignment *>(std::as_const(*this).GetGraceAlignment());
}

const Alignment *LayerElement::GetGraceAlignment() const
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

    // Since m_drawingFacsX is the left position, we adjust the XRel accordingly in AdjustXRelForTranscription
    if (m_drawingFacsX != VRV_UNSET) return m_drawingFacsX + this->GetDrawingXRel();

    if (m_cachedDrawingX != VRV_UNSET) return m_cachedDrawingX;

    if (!m_alignment) {
        // assert(this->Is({ BEAM, FTREM, TUPLET }));
        // Here we just get the measure position - no cast to Measure is necessary
        const Object *measure = this->GetFirstAncestor(MEASURE);
        assert(measure);
        m_cachedDrawingX = measure->GetDrawingX();
        return m_cachedDrawingX;
    }

    // First get the first layerElement parent (if any) and use its position if they share the same alignment
    const LayerElement *parent
        = dynamic_cast<const LayerElement *>(this->GetFirstAncestorInRange(LAYER_ELEMENT, LAYER_ELEMENT_max));
    if (parent && (parent->GetAlignment() == this->GetAlignment())) {
        m_cachedDrawingX = (parent->GetDrawingX() + this->GetDrawingXRel());
        return m_cachedDrawingX;
    }

    // Otherwise get the measure - no cast to Measure is necessary
    const Object *measure = this->GetFirstAncestor(MEASURE);
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

    if (m_drawingFacsY != VRV_UNSET) return m_drawingFacsY + this->GetDrawingYRel();

    if (m_cachedDrawingY != VRV_UNSET) return m_cachedDrawingY;

    // Look if we have a crossStaff situation
    const Object *object = m_crossStaff; // GetCrossStaff();
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

int LayerElement::GetDrawingArticulationTopOrBottom(data_STAFFREL place, ArticType type) const
{
    // It would not crash otherwise but there is not reason to call it
    assert(this->Is({ NOTE, CHORD }));

    ClassIdComparison isArtic(ARTIC);
    ListOfConstObjects artics;
    // Process backward because we want the farest away artic
    this->FindAllDescendantsByComparison(&artics, &isArtic, UNLIMITED_DEPTH, BACKWARD);

    const Artic *artic = NULL;
    for (const Object *child : artics) {
        artic = vrv_cast<const Artic *>(child);
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
    this->ResetCachedDrawingX();
    m_drawingXRel = drawingXRel;
}

void LayerElement::CacheXRel(bool restore)
{
    if (restore) {
        m_drawingXRel = m_cachedXRel;
    }
    else {
        m_cachedXRel = m_drawingXRel;
    }
}

void LayerElement::SetDrawingYRel(int drawingYRel)
{
    this->ResetCachedDrawingY();
    m_drawingYRel = drawingYRel;
}

void LayerElement::CacheYRel(bool restore)
{
    if (restore) {
        m_drawingYRel = m_cachedYRel;
    }
    else {
        m_cachedYRel = m_drawingYRel;
    }
}

void LayerElement::CenterDrawingX()
{
    if (m_drawingFacsX != VRV_UNSET) return;

    this->SetDrawingXRel(0);

    Measure *measure = vrv_cast<Measure *>(this->GetFirstAncestor(MEASURE));
    assert(measure);

    this->SetDrawingXRel(measure->GetInnerCenterX() - this->GetDrawingX());
}

int LayerElement::GetDrawingTop(const Doc *doc, int staffSize, bool withArtic, ArticType type) const
{
    if (this->Is({ NOTE, CHORD }) && withArtic) {
        int articY = this->GetDrawingArticulationTopOrBottom(STAFFREL_above, type);
        if (articY != VRV_UNSET) return articY;
    }

    const Note *note = NULL;
    if (this->Is(CHORD)) {
        const Chord *chord = vrv_cast<const Chord *>(this);
        assert(chord);
        note = chord->GetTopNote();
    }
    else if (this->Is(NOTE)) {
        note = vrv_cast<const Note *>(this);
        assert(note);
    }

    if (note) {
        const DurationInterface *durationInterface = this->GetDurationInterface();
        assert(durationInterface);
        if (durationInterface->GetNoteOrChordDur(this) < DURATION_2) {
            return note->GetDrawingY() + doc->GetDrawingUnit(staffSize);
        }
        // We should also take into account the stem shift to the right
        const StemmedDrawingInterface *stemmedDrawingInterface = this->GetStemmedDrawingInterface();
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

int LayerElement::GetDrawingBottom(const Doc *doc, int staffSize, bool withArtic, ArticType type) const
{
    if (this->Is({ NOTE, CHORD }) && withArtic) {
        int articY = this->GetDrawingArticulationTopOrBottom(STAFFREL_below, type);
        if (articY != -VRV_UNSET) return articY;
    }

    const Note *note = NULL;
    if (this->Is(CHORD)) {
        const Chord *chord = vrv_cast<const Chord *>(this);
        assert(chord);
        note = chord->GetBottomNote();
    }
    else if (this->Is(NOTE)) {
        note = vrv_cast<const Note *>(this);
        assert(note);
    }

    if (note) {
        const DurationInterface *durationInterface = this->GetDurationInterface();
        assert(durationInterface);
        if (durationInterface->GetNoteOrChordDur(this) < DURATION_2) {
            return note->GetDrawingY() - doc->GetDrawingUnit(staffSize);
        }
        // We should also take into account the stem shift to the right
        const StemmedDrawingInterface *stemmedDrawingInterface = this->GetStemmedDrawingInterface();
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

int LayerElement::GetDrawingRadius(const Doc *doc, bool isInLigature) const
{
    assert(doc);

    if (!this->Is({ CHORD, NC, NOTE, REST })) return 0;

    char32_t code = 0;
    data_DURATION dur = DURATION_4;
    const Staff *staff = this->GetAncestorStaff();
    bool isMensuralDur = false;
    if (this->Is(NOTE)) {
        const Note *note = vrv_cast<const Note *>(this);
        assert(note);
        dur = note->GetDrawingDur();
        isMensuralDur = note->IsMensuralDur();
        if (isMensuralDur && !isInLigature) {
            code = note->GetMensuralNoteheadGlyph();
        }
        else {
            code = note->GetNoteheadGlyph(dur);
        }
    }
    else if (this->Is(CHORD)) {
        const Chord *chord = vrv_cast<const Chord *>(this);
        assert(chord);
        dur = chord->GetActualDur();
        isMensuralDur = chord->IsMensuralDur();
        if (dur == DURATION_breve) {
            code = SMUFL_E0A1_noteheadDoubleWholeSquare;
        }
        else if (dur == DURATION_1) {
            code = SMUFL_E0A2_noteheadWhole;
        }
        else if (dur == DURATION_2) {
            code = SMUFL_E0A3_noteheadHalf;
        }
        else {
            code = SMUFL_E0A4_noteheadBlack;
        }
    }
    else if (this->Is({ REST, NC })) {
        code = SMUFL_E0A4_noteheadBlack;
    }

    // Mensural note shorter than DURATION_breve
    if ((isMensuralDur && (dur <= DURATION_breve)) || ((dur == DURATION_1) && isInLigature)) {
        int widthFactor = (dur == DURATION_maxima) ? 2 : 1;
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

Fraction LayerElement::GetAlignmentDuration(
    const AlignMeterParams &params, bool notGraceOnly, data_NOTATIONTYPE notationType) const
{
    if (this->IsGraceNote() && notGraceOnly) {
        return Fraction(0);
    }

    // Mensural chords are aligned looking at the duration of the notes
    if (this->Is(CHORD) && IsMensuralType(notationType)) {
        Fraction duration = 0;
        ListOfConstObjects notes = this->FindAllDescendantsByType(NOTE);
        for (const Object *object : notes) {
            const Note *note = vrv_cast<const Note *>(object);
            Fraction noteDuration = note->GetAlignmentDuration(params, notGraceOnly, notationType);
            duration = std::max(duration, noteDuration);
        }
        return duration;
    }

    // Only resolve simple sameas links to avoid infinite recursion
    const LayerElement *sameas = dynamic_cast<const LayerElement *>(this->GetSameasLink());
    if (sameas && !sameas->HasSameasLink()) {
        return sameas->GetAlignmentDuration(params, notGraceOnly, notationType);
    }

    if (this->HasInterface(INTERFACE_DURATION)) {
        int num = 1;
        int numbase = 1;

        if (params.proport) {
            // Proportion are applied reversly - higher ratio means shorter values
            if (params.proport->HasNum()) num *= params.proport->GetCumulatedNum();
            if (params.proport->HasNumbase()) numbase *= params.proport->GetCumulatedNumbase();
        }

        const Tuplet *tuplet = vrv_cast<const Tuplet *>(this->GetFirstAncestor(TUPLET, MAX_TUPLET_DEPTH));
        if (tuplet) {
            ListOfConstObjects objects;
            ClassIdsComparison ids({ CHORD, NOTE, REST, SPACE });
            tuplet->FindAllDescendantsByComparison(&objects, &ids);
            if (objects.size() > 0) {
                num = tuplet->GetNum();
                numbase = tuplet->GetNumbase();
                // Adjust VRV_UNSET and 0 - which is not valid in MEI anyway
                num = std::max(1, num);
                numbase = std::max(1, numbase);
            }
        }
        const DurationInterface *duration = this->GetDurationInterface();
        assert(duration);
        if (duration->IsMensuralDur() && (notationType != NOTATIONTYPE_cmn)) {
            return duration->GetInterfaceAlignmentMensuralDuration(num, numbase, params.mensur, params.equivalence);
        }
        if (this->Is(NC)) {
            // This is called only with --neume-as-note
            const Object *neume = this->GetFirstAncestor(NEUME);
            assert(neume);
            const Object *syllable = neume->GetFirstAncestor(SYLLABLE);
            assert(syllable);
            // Add a larger gap after the last nc of the last neume in the syllable
            if (neume->GetLast() == this) {
                return (syllable->GetLast() == neume) ? NEUME_LARGE_SPACE : NEUME_MEDIUM_SPACE;
            }
            else {
                return NEUME_SMALL_SPACE;
            }
        }
        Fraction durationValue = duration->GetInterfaceAlignmentDuration(num, numbase);
        // With fTrem we need to divide the duration by two
        const FTrem *fTrem = vrv_cast<const FTrem *>(this->GetFirstAncestor(FTREM, MAX_FTREM_DEPTH));
        if (fTrem) {
            durationValue = durationValue * Fraction(1, 2);
        }
        return durationValue;
    }
    else if (this->Is(BEATRPT)) {
        const BeatRpt *beatRpt = vrv_cast<const BeatRpt *>(this);
        assert(beatRpt);
        data_DURATION meterUnit = DURATION_4;
        if (params.meterSig && params.meterSig->HasUnit()) meterUnit = params.meterSig->GetUnitAsDur();
        return beatRpt->GetBeatRptAlignmentDuration(meterUnit);
    }
    else if (this->Is(TIMESTAMP_ATTR)) {
        const TimestampAttr *timestampAttr = vrv_cast<const TimestampAttr *>(this);
        assert(timestampAttr);
        data_DURATION meterUnit = DURATION_4;
        if (params.meterSig && params.meterSig->HasUnit()) meterUnit = params.meterSig->GetUnitAsDur();
        return timestampAttr->GetTimestampAttrAlignmentDuration(meterUnit);
    }
    // We align all full measure element to the current time signature, even the ones that last longer than one measure
    // If metcon is false, then the duration will remain 0 because it cannot be determined
    else if (params.metcon && this->Is({ HALFMRPT, MREST, MULTIREST, MRPT, MRPT2, MULTIRPT })) {
        data_DURATION meterUnit = DURATION_4;
        int meterCount = 4;
        if (params.meterSig && params.meterSig->HasUnit()) meterUnit = params.meterSig->GetUnitAsDur();
        if (params.meterSig && params.meterSig->HasCount()) meterCount = params.meterSig->GetTotalCount();
        Fraction duration = Fraction(meterUnit) * meterCount;
        return (this->Is(HALFMRPT)) ? (duration / 2) : duration;
    }
    // This is not called with --neume-as-note since otherwise each nc has an aligner
    else if (this->Is(NEUME)) {
        const Object *syllable = this->GetFirstAncestor(SYLLABLE);
        assert(syllable);
        // Add a larger gap after the last neume of the syllable
        return (syllable->GetLast() == this) ? NEUME_MEDIUM_SPACE : NEUME_SMALL_SPACE;
    }
    // This is called only with syallable without neume
    // Otherwise the duration is given by the neume (or by the nc with --neume-as-note)
    else if (this->Is(SYLLABLE) && !this->FindDescendantByType(NEUME)) {
        return NEUME_MEDIUM_SPACE;
    }
    else {
        return Fraction(0);
    }
}

Fraction LayerElement::GetAlignmentDuration(bool notGraceOnly, data_NOTATIONTYPE notationType) const
{
    AlignMeterParams params;
    return this->GetAlignmentDuration(params, notGraceOnly, notationType);
}

Fraction LayerElement::GetSameAsContentAlignmentDuration(
    const AlignMeterParams &params, bool notGraceOnly, data_NOTATIONTYPE notationType) const
{
    if (!this->HasSameasLink() || !this->GetSameasLink()->Is({ BEAM, FTREM, TUPLET })) {
        return Fraction(0);
    }

    const LayerElement *sameas = vrv_cast<const LayerElement *>(this->GetSameasLink());
    assert(sameas);

    return sameas->GetContentAlignmentDuration(params, notGraceOnly, notationType);
}

Fraction LayerElement::GetContentAlignmentDuration(
    const AlignMeterParams &params, bool notGraceOnly, data_NOTATIONTYPE notationType) const
{
    if (!this->Is({ BEAM, FTREM, TUPLET })) {
        return Fraction(0);
    }

    Fraction duration;

    for (const Object *child : this->GetChildren()) {
        // Skip everything that does not have a duration interface and notes in chords
        if (!child->HasInterface(INTERFACE_DURATION) || (child->GetFirstAncestor(CHORD, MAX_CHORD_DEPTH) != NULL)) {
            continue;
        }
        const LayerElement *element = vrv_cast<const LayerElement *>(child);
        assert(element);
        duration = duration + element->GetAlignmentDuration(params, notGraceOnly, notationType);
    }

    return duration;
}

Fraction LayerElement::GetContentAlignmentDuration(bool notGraceOnly, data_NOTATIONTYPE notationType) const
{
    AlignMeterParams params;
    return this->GetContentAlignmentDuration(params, notGraceOnly, notationType);
}

bool LayerElement::GenerateZoneBounds(int *ulx, int *uly, int *lrx, int *lry) const
{
    // Set integers to extremes
    *ulx = INT_MAX;
    *uly = INT_MAX;
    *lrx = INT_MIN;
    *lry = INT_MIN;
    ListOfConstObjects childrenWithFacsimileInterface;
    InterfaceComparison ic(INTERFACE_FACSIMILE);
    this->FindAllDescendantsByComparison(&childrenWithFacsimileInterface, &ic);
    bool result = false;
    for (const Object *object : childrenWithFacsimileInterface) {
        const FacsimileInterface *fi = object->GetFacsimileInterface();
        assert(fi);
        if (!object->Is(SYL) && fi->HasFacs()) {
            const Zone *zone = fi->GetZone();
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

std::vector<int> LayerElement::GetElementsInUnison(
    const std::set<int> &firstChord, const std::set<int> &secondChord, data_STEMDIRECTION stemDirection) const
{
    if (firstChord.empty() || secondChord.empty()) return {};
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
                return {};
            }
        }
    }

    // If there are no `middle` notes, check whether chords can be in unison with regards of stem direction
    // With DOWN stem direction, highest note of the chord HAS to be in unison. If topmost location of the chord
    // is higher than topmost location of the opposing chord it means that these elements cannot be in unison.
    // Same applies to the UP stem direction, just with reversed condition
    if (stemDirection == STEMDIRECTION_down) {
        if ((*firstChord.rbegin() > *secondChord.rbegin()) || (*firstChord.begin() > *secondChord.begin())) return {};
    }
    else {
        if ((*firstChord.rbegin() < *secondChord.rbegin()) || (*firstChord.begin() < *secondChord.begin())) return {};
    }

    // Finally, check if notes in unison are at the proper distance to be drawn as unison, as well as get number of
    // elements in unison
    std::vector<int> intersection;
    intersection.resize(firstChord.size() > secondChord.size() ? firstChord.size() : secondChord.size());
    auto it = std::set_intersection(
        firstChord.begin(), firstChord.end(), secondChord.begin(), secondChord.end(), intersection.begin());
    intersection.resize(it - intersection.begin());
    if (intersection.empty()) return {};
    for (int i = 0; i < (int)intersection.size() - 1; ++i) {
        if (std::abs(intersection.at(i) - intersection.at(i + 1)) == 1) {
            return {};
        }
    }

    return intersection;
}

MapOfDotLocs LayerElement::CalcOptimalDotLocations()
{
    if (!this->Is({ NOTE, CHORD })) {
        return {};
    }

    Layer *layer = vrv_cast<Layer *>(this->GetFirstAncestor(LAYER));
    const int layerCount = layer->GetLayerCountForTimeSpanOf(this);

    // Calculate primary/secondary dot locations
    const MapOfDotLocs dotLocs1 = this->CalcDotLocations(layerCount, true);
    const MapOfDotLocs dotLocs2 = this->CalcDotLocations(layerCount, false);

    // Special treatment for two layers
    if (layerCount == 2) {
        // Find the first note on the other layer, but in the same staff
        Alignment *alignment = this->GetAlignment();
        const Staff *currentStaff = this->GetAncestorStaff(RESOLVE_CROSS_STAFF);
        const int currentLayerN = abs(this->GetAlignmentLayerN());
        ListOfObjects notes = alignment->FindAllDescendantsByType(NOTE, false);
        auto noteIt = std::find_if(notes.cbegin(), notes.cend(), [currentLayerN, currentStaff](Object *obj) {
            const Note *otherNote = vrv_cast<Note *>(obj);
            const Staff *otherStaff = otherNote->GetAncestorStaff(RESOLVE_CROSS_STAFF);
            const int otherLayerN = abs(otherNote->GetAlignmentLayerN());
            return ((currentLayerN != otherLayerN) && (currentStaff == otherStaff));
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
            const int collisions11 = this->GetCollisionCount(dotLocs1, otherDotLocs1);
            const int collisions12 = this->GetCollisionCount(dotLocs1, otherDotLocs2);
            const int collisions21 = this->GetCollisionCount(dotLocs2, otherDotLocs1);
            const int collisions22 = this->GetCollisionCount(dotLocs2, otherDotLocs2);
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
    const bool usePrimary = (this->GetDotCount(dotLocs1) >= this->GetDotCount(dotLocs2));
    return usePrimary ? dotLocs1 : dotLocs2;
}

data_STEMMODIFIER LayerElement::GetDrawingStemMod() const
{
    const AttStems *stem = dynamic_cast<const AttStems *>(this);
    if (!stem) return STEMMODIFIER_NONE;

    return stem->GetStemMod();
}

char32_t LayerElement::StemModToGlyph(data_STEMMODIFIER stemMod) const
{
    switch (stemMod) {
        case STEMMODIFIER_1slash: return SMUFL_E220_tremolo1;
        case STEMMODIFIER_2slash: return SMUFL_E221_tremolo2;
        case STEMMODIFIER_3slash: return SMUFL_E222_tremolo3;
        case STEMMODIFIER_4slash: return SMUFL_E223_tremolo4;
        case STEMMODIFIER_5slash: return SMUFL_E224_tremolo5;
        case STEMMODIFIER_6slash: return SMUFL_E224_tremolo5;
        case STEMMODIFIER_sprech: return SMUFL_E645_vocalSprechgesang;
        case STEMMODIFIER_z: return SMUFL_E22A_buzzRoll;
        default: break;
    }
    return 0;
}

//----------------------------------------------------------------------------
// Static methods for LayerElement
//----------------------------------------------------------------------------

int LayerElement::GetDotCount(const MapOfDotLocs &dotLocations)
{
    return std::accumulate(dotLocations.cbegin(), dotLocations.cend(), 0,
        [](int sum, const MapOfDotLocs::value_type &mapEntry) { return sum + (int)mapEntry.second.size(); });
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

FunctorCode LayerElement::Accept(Functor &functor)
{
    return functor.VisitLayerElement(this);
}

FunctorCode LayerElement::Accept(ConstFunctor &functor) const
{
    return functor.VisitLayerElement(this);
}

FunctorCode LayerElement::AcceptEnd(Functor &functor)
{
    return functor.VisitLayerElementEnd(this);
}

FunctorCode LayerElement::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitLayerElementEnd(this);
}

int LayerElement::AdjustOverlappingLayers(const Doc *doc, const std::vector<LayerElement *> &otherElements,
    bool areDotsAdjusted, bool &isUnison, bool &stemSameas)
{
    if (this->Is(NOTE) && this->GetParent()->Is(CHORD))
        return 0;
    else if (this->Is(STEM) && isUnison) {
        isUnison = false;
        return 0;
    }
    else if (this->Is(STEM) && stemSameas) {
        stemSameas = false;
        return 0;
    }

    auto [margin, isInUnison] = CalcElementHorizontalOverlap(doc, otherElements, areDotsAdjusted, false);
    if (this->Is(NOTE)) {
        isUnison = isInUnison;
        if (isUnison) return 0;
        Note *note = vrv_cast<Note *>(this);
        assert(note);
        stemSameas = note->HasStemSameasNote();
        if (stemSameas) return 0;
    }

    if (this->Is({ ACCID, DOTS, STEM })) {
        LayerElement *parent
            = vrv_cast<LayerElement *>(this->GetFirstAncestorInRange(LAYER_ELEMENT, LAYER_ELEMENT_max));
        assert(parent);
        parent->SetDrawingXRel(parent->GetDrawingXRel() + margin);
    }
    else {
        this->SetDrawingXRel(this->GetDrawingXRel() + margin);
    }
    return margin;
}

std::pair<int, bool> LayerElement::CalcElementHorizontalOverlap(const Doc *doc,
    const std::vector<LayerElement *> &otherElements, bool areDotsAdjusted, bool isChordElement, bool isLowerElement,
    bool unison)
{
    Staff *staff = this->GetAncestorStaff();

    bool isInUnison = false;
    int shift = 0;

    for (int i = 0; i < int(otherElements.size()); ++i) {
        int verticalMargin = 0;
        int horizontalMargin = 2 * doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
        bool isUnisonElement = false;
        // Handle stem collisions
        if (this->Is(STEM)) {
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
        else if (this->Is(NOTE) && otherElements.at(i)->Is(NOTE)) {
            Note *currentNote = vrv_cast<Note *>(this);
            Note *previousNote = vrv_cast<Note *>(otherElements.at(i));
            assert(previousNote);
            isUnisonElement = currentNote->IsUnisonWith(previousNote, true);
            // Unisson, look at the duration for the note heads
            if (unison && currentNote->IsUnisonWith(previousNote, false)) {
                data_DURATION previousDuration = previousNote->GetDrawingDur();
                assert(previousNote->GetParent());
                const bool isPreviousChord = previousNote->GetParent()->Is(CHORD);
                bool isEdgeElement = false;
                data_STEMDIRECTION stemDir = currentNote->GetDrawingStemDir();
                if (isPreviousChord) {
                    Chord *parentChord = vrv_cast<Chord *>(previousNote->GetParent());
                    previousDuration = parentChord->GetDur();
                    isEdgeElement = ((STEMDIRECTION_down == stemDir) && (parentChord->GetBottomNote() == previousNote))
                        || ((STEMDIRECTION_up == stemDir) && (parentChord->GetTopNote() == previousNote));
                }
                // Reduce the margin to 0 for whole notes unisson
                else if ((currentNote->GetDrawingDur() == DURATION_1) && (previousDuration == DURATION_1)) {
                    horizontalMargin = 0;
                }
                if (!isPreviousChord || isEdgeElement || isChordElement) {
                    if ((currentNote->GetDrawingDur() == DURATION_2) && (previousDuration == DURATION_2)) {
                        isInUnison = true;
                    }
                    else if ((!currentNote->IsGraceNote() && !currentNote->GetDrawingCueSize())
                        && (previousNote->IsGraceNote() || previousNote->GetDrawingCueSize())
                        && (STEMDIRECTION_down == stemDir)) {
                        shift -= 0.8 * horizontalMargin;
                        continue;
                    }
                    else if ((currentNote->IsGraceNote() || currentNote->GetDrawingCueSize())
                        && (!previousNote->IsGraceNote() && !previousNote->GetDrawingCueSize())
                        && (STEMDIRECTION_up == stemDir)) {
                        currentNote->SetDrawingXRel(currentNote->GetDrawingXRel() + 0.8 * horizontalMargin);
                        isInUnison = true;
                        continue;
                    }
                    else if ((currentNote->GetDrawingDur() > DURATION_2) && (previousDuration > DURATION_2)) {
                        isInUnison = true;
                    }
                    if (isInUnison && (currentNote->GetDots() == previousNote->GetDots())) {
                        continue;
                    }
                    else {
                        isInUnison = false;
                        if ((currentNote->GetDrawingDur() <= DURATION_1)
                            || (previousNote->GetDrawingDur() <= DURATION_1)) {
                            horizontalMargin *= -1;
                        }
                        else {
                            horizontalMargin *= (currentNote->GetDots() >= previousNote->GetDots()) ? 0 : -1;
                        }
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
                else if ((currentNote->GetDrawingDur() <= DURATION_1)
                    && (previousNote->GetDrawingDur() <= DURATION_1)) {
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
        else if (this->Is(DOTS) && !otherElements.at(i)->Is(DOTS) && areDotsAdjusted) {
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
        else if (this->Is(ACCID) && otherElements.at(i)->Is(NOTE)) {
            Note *parentNote = vrv_cast<Note *>(this->GetFirstAncestor(NOTE));
            Note *otherNote = vrv_cast<Note *>(otherElements.at(i));
            const bool isUnisonOverlap = parentNote && parentNote->IsUnisonWith(otherNote, true)
                && !parentNote->IsUnisonWith(otherNote, false);
            if (isUnisonOverlap && this->HorizontalContentOverlap(otherElements.at(i)))
                shift += this->HorizontalRightOverlap(
                    otherElements.at(i), doc, -doc->GetDrawingUnit(staff->m_drawingStaffSize));
        }

        if (this->Is(NOTE) && !otherElements.at(i)->Is(STEM)) {
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
            if (otherElements.at(i)->Is(STEM) && (shift == 0) && areDotsAdjusted) {
                Stem *stem = vrv_cast<Stem *>(otherElements.at(i));
                // Nothing to do if note has same stem
                if (currentNote->HasStemSameasNote()) continue;

                shift -= stem->CompareToElementPosition(doc, currentNote, 0);
            }
        }
    }

    // If note is not in unison, has accidental and were to be shifted to the right - shift it to the left
    // That way accidental will be near note that actually has accidental and not near lowest-layer note
    if (this->Is(NOTE) && isChordElement && unison && (shift > 0)) {
        Note *currentNote = vrv_cast<Note *>(this);
        if (currentNote->GetDrawingAccid()) shift *= -1;
    }

    return { shift, isInUnison };
}

} // namespace vrv
