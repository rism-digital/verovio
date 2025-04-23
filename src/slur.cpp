/////////////////////////////////////////////////////////////////////////////
// Name:        slur.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "slur.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <cmath>
#include <math.h>

//----------------------------------------------------------------------------

#include "beam.h"
#include "chord.h"
#include "comparison.h"
#include "doc.h"
#include "elementpart.h"
#include "findlayerelementsfunctor.h"
#include "ftrem.h"
#include "functor.h"
#include "layer.h"
#include "layerelement.h"
#include "note.h"
#include "staff.h"
#include "system.h"
#include "tuplet.h"
#include "verticalaligner.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Slur
//----------------------------------------------------------------------------

static const ClassRegistrar<Slur> s_factory("slur", SLUR);

Slur::Slur() : ControlElement(SLUR), TimeSpanningInterface(), AttCurvature(), AttLayerIdent(), AttLineRendBase()
{
    this->RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    this->RegisterAttClass(ATT_CURVATURE);
    this->RegisterAttClass(ATT_LAYERIDENT);
    this->RegisterAttClass(ATT_LINERENDBASE);

    this->Reset();
}

Slur::Slur(ClassId classId)
    : ControlElement(classId), TimeSpanningInterface(), AttCurvature(), AttLayerIdent(), AttLineRendBase()
{
    this->RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    this->RegisterAttClass(ATT_CURVATURE);
    this->RegisterAttClass(ATT_LAYERIDENT);
    this->RegisterAttClass(ATT_LINERENDBASE);

    this->Reset();
}

Slur::~Slur() {}

void Slur::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    this->ResetCurvature();
    this->ResetLayerIdent();
    this->ResetLineRendBase();

    m_drawingCurveDir = SlurCurveDirection::None;
}

curvature_CURVEDIR Slur::CalcDrawingCurveDir(char spanningType) const
{
    switch (m_drawingCurveDir) {
        case SlurCurveDirection::Above: return curvature_CURVEDIR_above;
        case SlurCurveDirection::Below: return curvature_CURVEDIR_below;
        case SlurCurveDirection::AboveBelow: {
            switch (spanningType) {
                case SPANNING_START_END: return curvature_CURVEDIR_mixed;
                case SPANNING_START: return curvature_CURVEDIR_above;
                default: return curvature_CURVEDIR_below;
            }
        }
        case SlurCurveDirection::BelowAbove: {
            switch (spanningType) {
                case SPANNING_START_END: return curvature_CURVEDIR_mixed;
                case SPANNING_START: return curvature_CURVEDIR_below;
                default: return curvature_CURVEDIR_above;
            }
        }
        default: return curvature_CURVEDIR_NONE;
    }
}

bool Slur::HasInnerSlur(const Slur *innerSlur) const
{
    // Check drawing curve direction
    if (this->GetDrawingCurveDir() != innerSlur->GetDrawingCurveDir()) return false;
    if (this->HasMixedCurveDir()) return false;

    // Check the layer
    const LayerElement *start = this->GetStart();
    const LayerElement *end = this->GetEnd();
    if (!start || !end) return false;
    const LayerElement *innerStart = innerSlur->GetStart();
    const LayerElement *innerEnd = innerSlur->GetEnd();
    if (!innerStart || !innerEnd) return false;

    std::set<int> admissibleLayers = { std::abs(start->GetAlignmentLayerN()), std::abs(end->GetAlignmentLayerN()) };
    std::set<int> innerLayers
        = { std::abs(innerStart->GetAlignmentLayerN()), std::abs(innerEnd->GetAlignmentLayerN()) };
    if (!std::includes(admissibleLayers.begin(), admissibleLayers.end(), innerLayers.begin(), innerLayers.end())) {
        return false;
    }

    // Check the alignment
    if (this->IsOrdered(innerStart, start) || this->IsOrdered(end, innerEnd)) return false;
    return ((this->IsOrdered(start, innerStart)) || (this->IsOrdered(innerEnd, end)));
}

std::pair<const Layer *, const LayerElement *> Slur::GetBoundaryLayer() const
{
    const LayerElement *start = this->GetStart();
    const LayerElement *end = this->GetEnd();

    if (!start || !end) return { NULL, NULL };

    const Layer *layer = NULL;
    const LayerElement *layerElement = NULL;
    if (!start->Is(TIMESTAMP_ATTR)) {
        layer = vrv_cast<const Layer *>(start->GetFirstAncestor(LAYER));
        layerElement = start;
    }
    if (!end->Is(TIMESTAMP_ATTR)) {
        // Prefer non grace notes if possible
        if (!layerElement || layerElement->IsGraceNote()) {
            layer = vrv_cast<const Layer *>(end->GetFirstAncestor(LAYER));
            layerElement = end;
        }
    }
    if (layerElement && layerElement->m_crossStaff) layer = layerElement->m_crossLayer;

    return { layer, layerElement };
}

const Staff *Slur::GetBoundaryCrossStaff() const
{
    const LayerElement *start = this->GetStart();
    const LayerElement *end = this->GetEnd();

    if (!start || !end) return NULL;

    if (start->m_crossStaff != end->m_crossStaff) {
        return end->m_crossStaff;
    }
    else {
        // Check if the two elements are in different staves (but themselves not cross-staff)
        const Staff *startStaff = start->GetAncestorStaff(ANCESTOR_ONLY, false);
        const Staff *endStaff = end->GetAncestorStaff(ANCESTOR_ONLY, false);
        if (startStaff && endStaff && (startStaff->GetN() != endStaff->GetN())) {
            return endStaff;
        }
        else {
            return NULL;
        }
    }
}

void Slur::CalcSpannedElements(FloatingCurvePositioner *curve)
{
    Staff *staff = dynamic_cast<Staff *>(curve->GetObjectY());
    if (!staff) return;

    // Determine x1 and x2
    Point points[4];
    curve->GetPoints(points);
    const int x1 = points[0].x;
    const int x2 = points[3].x;

    // Collect, filter and store the spanned elements
    const SpannedElements spannedElements = this->CollectSpannedElements(staff, x1, x2);
    this->AddSpannedElements(curve, spannedElements, staff, x1, x2);
}

SpannedElements Slur::CollectSpannedElements(const Staff *staff, int xMin, int xMax) const
{
    // Decide whether we search the whole parent system or just one measure which is much faster
    const Object *container = this->IsSpanningMeasures() ? staff->GetFirstAncestor(SYSTEM) : this->GetStartMeasure();

    FindSpannedLayerElementsFunctor findSpannedLayerElements(this);
    findSpannedLayerElements.SetMinMaxPos(xMin, xMax);
    findSpannedLayerElements.SetClassIds({ ACCID, ARTIC, CHORD, CLEF, DOT, DOTS, FLAG, GLISS, NOTE, STEM,
        TUPLET_BRACKET, TUPLET_NUM }); // Ties should be handled separately

    std::set<int> staffNumbers;
    staffNumbers.emplace(staff->GetN());
    const Staff *startStaff = this->GetStart()->GetAncestorStaff(RESOLVE_CROSS_STAFF, false);
    const Staff *endStaff = this->GetEnd()->GetAncestorStaff(RESOLVE_CROSS_STAFF, false);
    if (startStaff && (startStaff != staff)) {
        staffNumbers.emplace(startStaff->GetN());
    }
    else if (endStaff && (endStaff != staff)) {
        staffNumbers.emplace(endStaff->GetN());
    }
    findSpannedLayerElements.SetStaffNs(staffNumbers);

    // Run the search without layer bounds
    container->Process(findSpannedLayerElements);

    // Now determine the minimal and maximal layer
    std::set<int> layersN;
    if (this->HasLayer()) {
        layersN = { this->GetLayer() };
    }
    else {
        for (const LayerElement *element : { this->GetStart(), this->GetEnd() }) {
            const int layerN = element->GetOriginalLayerN();
            layersN.insert(layerN);
        }
    }
    const int minLayerN = *layersN.begin();
    const int maxLayerN = *layersN.rbegin();

    // Check whether outside layers exist
    std::vector<const LayerElement *> spannedElements = findSpannedLayerElements.GetElements();
    const bool hasOutsideLayers = std::any_of(
        spannedElements.cbegin(), spannedElements.cend(), [minLayerN, maxLayerN](const LayerElement *element) {
            const int layerN = element->GetOriginalLayerN();
            return ((layerN < minLayerN) || (layerN > maxLayerN));
        });

    if (hasOutsideLayers) {
        // Filter all notes, also include the notes of the start and end of the slur
        ListOfConstObjects notes;
        std::copy_if(spannedElements.cbegin(), spannedElements.cend(), std::back_inserter(notes),
            [](const LayerElement *element) { return element->Is(NOTE); });
        ClassIdComparison cmp(NOTE);
        if (this->GetStart()->Is(NOTE)) {
            notes.push_back(this->GetStart());
        }
        else {
            this->GetStart()->FindAllDescendantsByComparison(&notes, &cmp, 1, FORWARD, false);
        }
        if (this->GetEnd()->Is(NOTE)) {
            notes.push_back(this->GetEnd());
        }
        else {
            this->GetEnd()->FindAllDescendantsByComparison(&notes, &cmp, 1, FORWARD, false);
        }

        // Determine the minimal and maximal diatonic pitch
        int minPitch = 1000;
        int maxPitch = 0;
        for (const Object *object : notes) {
            const Note *note = vrv_cast<const Note *>(object);
            assert(note);
            const int layerN = note->GetOriginalLayerN();
            if (layerN == maxLayerN) {
                minPitch = std::min(note->GetDiatonicPitch(), minPitch);
            }
            if (layerN == minLayerN) {
                maxPitch = std::max(note->GetDiatonicPitch(), maxPitch);
            }
        }

        // Check if voices are separated
        const bool layersAreSeparated = std::all_of(
            notes.cbegin(), notes.cend(), [minLayerN, maxLayerN, minPitch, maxPitch](const Object *object) {
                const Note *note = vrv_cast<const Note *>(object);
                const int layerN = note->GetOriginalLayerN();
                if (layerN < minLayerN) {
                    return (note->GetDiatonicPitch() > maxPitch);
                }
                if (layerN > maxLayerN) {
                    return (note->GetDiatonicPitch() < minPitch);
                }
                return true;
            });

        // For separated voices or prescribed layers rerun the search with layer bounds
        if (layersAreSeparated || this->HasLayer()) {
            findSpannedLayerElements.ClearElements();
            findSpannedLayerElements.SetMinMaxLayerN(minLayerN, maxLayerN);
            container->Process(findSpannedLayerElements);
            spannedElements = findSpannedLayerElements.GetElements();
        }
    }

    // Collect the layers used for collision avoidance
    std::for_each(spannedElements.cbegin(), spannedElements.cend(),
        [&layersN](const LayerElement *element) { layersN.insert(element->GetOriginalLayerN()); });

    return { spannedElements, layersN };
}

void Slur::AddSpannedElements(
    FloatingCurvePositioner *curve, const SpannedElements &spanned, Staff *staff, int xMin, int xMax)
{
    Staff *startStaff = this->GetStart()->GetAncestorStaff(RESOLVE_CROSS_STAFF, false);
    Staff *endStaff = this->GetEnd()->GetAncestorStaff(RESOLVE_CROSS_STAFF, false);
    if (startStaff && endStaff && (startStaff->GetN() != endStaff->GetN())) {
        curve->SetCrossStaff(endStaff);
    }

    curve->ClearSpannedElements();
    for (const LayerElement *element : spanned.elements) {
        const int xLeft = element->GetSelfLeft();
        const int xRight = element->GetSelfRight();
        const bool isOverlapping = ((xLeft > xMin) && (xLeft < xMax)) || ((xRight > xMin) && (xRight < xMax));

        if (isOverlapping || element->Is(TUPLET_BRACKET)) {
            CurveSpannedElement *spannedElement = new CurveSpannedElement();
            spannedElement->m_boundingBox = element;
            spannedElement->m_isBelow = this->IsElementBelow(element, startStaff, endStaff);
            curve->AddSpannedElement(spannedElement);
        }

        if (!curve->IsCrossStaff() && element->m_crossStaff) {
            curve->SetCrossStaff(element->m_crossStaff);
        }
    }

    // Some tuplet elements are discarded immediately, if they should be rendered outside the slur
    // => Flexible layout priority
    this->DiscardTupletElements(curve, xMin, xMax);

    // Ties can be broken across systems, so we have to look for all floating curve positioners that represent them.
    // This might be refined later, since using the entire bounding box of a tie for collision avoidance with slurs is
    // coarse.
    ArrayOfFloatingPositioners tiePositioners = staff->GetAlignment()->FindAllFloatingPositioners(TIE);
    if (startStaff && (startStaff != staff) && startStaff->GetAlignment()) {
        const ArrayOfFloatingPositioners startTiePositioners
            = startStaff->GetAlignment()->FindAllFloatingPositioners(TIE);
        std::copy(startTiePositioners.begin(), startTiePositioners.end(), std::back_inserter(tiePositioners));
    }
    else if (endStaff && (endStaff != staff) && endStaff->GetAlignment()) {
        const ArrayOfFloatingPositioners endTiePositioners = endStaff->GetAlignment()->FindAllFloatingPositioners(TIE);
        std::copy(endTiePositioners.begin(), endTiePositioners.end(), std::back_inserter(tiePositioners));
    }

    // Only consider ties in collision layers
    tiePositioners.erase(std::remove_if(tiePositioners.begin(), tiePositioners.end(),
                             [&spanned](FloatingPositioner *positioner) {
                                 TimeSpanningInterface *interface = positioner->GetObject()->GetTimeSpanningInterface();
                                 assert(interface);
                                 if (!interface->GetStart() || !interface->GetEnd()) return true;
                                 const bool startsInCollisionLayer
                                     = spanned.layersN.contains(interface->GetStart()->GetOriginalLayerN());
                                 const bool endsInCollisionLayer
                                     = spanned.layersN.contains(interface->GetEnd()->GetOriginalLayerN());
                                 return (!startsInCollisionLayer && !endsInCollisionLayer);
                             }),
        tiePositioners.end());

    // Add ties to spanning elements
    for (FloatingPositioner *positioner : tiePositioners) {
        if (positioner->GetAlignment()->GetParentSystem() == curve->GetAlignment()->GetParentSystem()) {
            if (positioner->HasContentBB() && (positioner->GetContentRight() > xMin)
                && (positioner->GetContentLeft() < xMax)) {
                CurveSpannedElement *spannedElement = new CurveSpannedElement();
                spannedElement->m_boundingBox = positioner;
                spannedElement->m_isBelow = this->IsElementBelow(positioner, startStaff, endStaff);
                curve->AddSpannedElement(spannedElement);
            }
        }
    }
}

void Slur::DiscardTupletElements(FloatingCurvePositioner *curve, int xMin, int xMax)
{
    const ArrayOfCurveSpannedElements *spannedElements = curve->GetSpannedElements();
    for (CurveSpannedElement *spannedElement : *spannedElements) {
        if (spannedElement->m_boundingBox->Is(TUPLET_BRACKET)) {
            const TupletBracket *tupletBracket = vrv_cast<const TupletBracket *>(spannedElement->m_boundingBox);
            assert(tupletBracket->GetParent()->Is(TUPLET));
            const Tuplet *tuplet = vrv_cast<const Tuplet *>(tupletBracket->GetParent());

            const int xLeft = tupletBracket->GetSelfLeft();
            const int xRight = tupletBracket->GetSelfRight();
            const bool isContained = (xLeft > xMin) && (xRight < xMax);
            const bool isOverlapping = ((xLeft > xMin) && (xLeft < xMax)) || ((xRight > xMin) && (xRight < xMax));

            // Slurs avoid inner tuplets
            if (isContained) continue;

            // Slurs avoid overlapping tuplets which are beam aligned or not significantly longer
            if (isOverlapping) {
                if (tuplet->GetBracketAlignedBeam()) continue;
                if (xRight - xLeft < 2 * (xMax - xMin)) continue;
            }

            // Discard the tuplet bracket and register the slur for tuplet adjustment
            spannedElement->m_discarded = true;
            // Exceptional case where the slur actually modifies a spanned element
            const_cast<Tuplet *>(tuplet)->AddInnerSlur(curve);

            // Discard any associated tuplet number as well
            const TupletNum *tupletNum = tupletBracket->GetAlignedNum();
            if (tupletNum) {
                auto elementIter = std::find_if(spannedElements->begin(), spannedElements->end(),
                    [tupletNum](CurveSpannedElement *element) { return (element->m_boundingBox == tupletNum); });
                if (elementIter != spannedElements->end()) {
                    (*elementIter)->m_discarded = true;
                }
            }
        }
    }
}

void Slur::AddPositionerToArticulations(FloatingCurvePositioner *curve)
{
    LayerElement *start = this->GetStart();
    LayerElement *end = this->GetEnd();
    if (!start || !end) return;

    const char spanningType = curve->GetSpanningType();
    const curvature_CURVEDIR curveDir = this->CalcDrawingCurveDir(spanningType);

    // the normal case or start
    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_START)) {
        ListOfObjects artics = start->FindAllDescendantsByType(ARTIC);
        // Then the @n of each first staffDef
        for (Object *object : artics) {
            Artic *artic = vrv_cast<Artic *>(object);
            assert(artic);
            if (artic->IsOutsideArtic()) {
                if ((artic->GetPlace() == STAFFREL_above) && (curveDir == curvature_CURVEDIR_above)) {
                    artic->AddSlurPositioner(curve, true);
                }
                else if ((artic->GetPlace() == STAFFREL_below) && (curveDir == curvature_CURVEDIR_below)) {
                    artic->AddSlurPositioner(curve, true);
                }
            }
        }
    }
    // normal case or end
    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_END)) {
        ListOfObjects artics = end->FindAllDescendantsByType(ARTIC);
        // Then the @n of each first staffDef
        for (Object *object : artics) {
            Artic *artic = vrv_cast<Artic *>(object);
            assert(artic);
            if (artic->IsOutsideArtic()) {
                if ((artic->GetPlace() == STAFFREL_above) && (curveDir == curvature_CURVEDIR_above)) {
                    artic->AddSlurPositioner(curve, false);
                }
                else if ((artic->GetPlace() == STAFFREL_below) && (curveDir == curvature_CURVEDIR_below)) {
                    artic->AddSlurPositioner(curve, false);
                }
            }
        }
    }
}

Staff *Slur::CalculatePrincipalStaff(const Staff *staff, int xMin, int xMax)
{
    return const_cast<Staff *>(std::as_const(*this).CalculatePrincipalStaff(staff, xMin, xMax));
}

const Staff *Slur::CalculatePrincipalStaff(const Staff *staff, int xMin, int xMax) const
{
    assert(staff);

    const Staff *principalStaff = NULL;

    const SlurCurveDirection curveDir = this->GetDrawingCurveDir();
    const SpannedElements spanned = this->CollectSpannedElements(staff, xMin, xMax);
    if (spanned.elements.empty()) {
        return staff;
    }

    // The floating curve positioner of cross staff slurs should live in the lower/upper staff alignment
    // corresponding to whether the slur is curved below or not
    auto adaptStaff = [&principalStaff, curveDir](const LayerElement *element) {
        const Staff *elementStaff = element->GetAncestorStaff(RESOLVE_CROSS_STAFF);
        const bool updatePrincipal = !principalStaff
            || ((curveDir == SlurCurveDirection::Below) ? (elementStaff->GetN() > principalStaff->GetN())
                                                        : (elementStaff->GetN() < principalStaff->GetN()));
        if (updatePrincipal) {
            principalStaff = elementStaff;
        }
    };

    // Run once through all spanned elements
    std::for_each(spanned.elements.begin(), spanned.elements.end(), adaptStaff);

    // Also check the beams of spanned elements
    std::for_each(spanned.elements.begin(), spanned.elements.end(), [&adaptStaff](const LayerElement *element) {
        if (const Beam *beam = element->GetAncestorBeam(); beam) {
            adaptStaff(beam);
        }
    });

    assert(principalStaff);
    return principalStaff;
}

bool Slur::IsElementBelow(const LayerElement *element, const Staff *startStaff, const Staff *endStaff) const
{
    assert(element);
    // startStaff and endStaff may be NULL (if the boundary is a tstamp)
    // however, slurs with tstamp should never have S-shape

    switch (this->GetDrawingCurveDir()) {
        case SlurCurveDirection::Above: return true;
        case SlurCurveDirection::Below: return false;
        case SlurCurveDirection::AboveBelow:
            assert(startStaff);
            return (element->GetAncestorStaff(RESOLVE_CROSS_STAFF)->GetN() == startStaff->GetN());
        case SlurCurveDirection::BelowAbove:
            assert(endStaff);
            return (element->GetAncestorStaff(RESOLVE_CROSS_STAFF)->GetN() == endStaff->GetN());
        default: return false;
    }
}

bool Slur::IsElementBelow(const FloatingPositioner *positioner, const Staff *startStaff, const Staff *endStaff) const
{
    assert(positioner);
    // startStaff and endStaff may be NULL (if the boundary is a tstamp)
    // however, slurs with tstamp should never have S-shape

    switch (this->GetDrawingCurveDir()) {
        case SlurCurveDirection::Above: return true;
        case SlurCurveDirection::Below: return false;
        case SlurCurveDirection::AboveBelow:
            assert(startStaff);
            return (positioner->GetAlignment()->GetStaff()->GetN() == startStaff->GetN());
        case SlurCurveDirection::BelowAbove:
            assert(endStaff);
            return (positioner->GetAlignment()->GetStaff()->GetN() == endStaff->GetN());
        default: return false;
    }
}

void Slur::InitBezierControlSides(BezierCurve &bezier, curvature_CURVEDIR curveDir) const
{
    switch (curveDir) {
        case curvature_CURVEDIR_above: bezier.SetControlSides(true, true); break;
        case curvature_CURVEDIR_below: bezier.SetControlSides(false, false); break;
        case curvature_CURVEDIR_mixed:
            bezier.SetControlSides(this->HasEndpointAboveStart(), this->HasEndpointAboveEnd());
            break;
        default: break;
    }
}

float Slur::GetAdjustedSlurAngle(const Doc *doc, Point &p1, Point &p2, curvature_CURVEDIR curveDir) const
{
    float slurAngle = (p1 == p2) ? 0 : atan2(p2.y - p1.y, p2.x - p1.x);
    const float maxAngle = (float)doc->GetOptions()->m_slurMaxSlope.GetValue() * M_PI / 180.0;

    // the slope of the slur is high and needs to be corrected
    if (fabs(slurAngle) > maxAngle) {
        int side = (p2.x - p1.x) * tan(maxAngle);
        if (p2.y > p1.y) {
            if (curveDir == curvature_CURVEDIR_above) {
                p1.y = p2.y - side;
            }
            else {
                p2.y = p1.y + side;
            }
            slurAngle = maxAngle;
        }
        else {
            if (curveDir == curvature_CURVEDIR_above) {
                p2.y = p1.y - side;
            }
            else {
                p1.y = p2.y + side;
            }
            slurAngle = -maxAngle;
        }
    }

    return slurAngle;
}

std::pair<Point, Point> Slur::CalcEndPoints(const Doc *doc, const Staff *staff, NearEndCollision *nearEndCollision,
    int x1, int x2, curvature_CURVEDIR drawingCurveDir, char spanningType) const
{
    const StemmedDrawingInterface *startStemDrawInterface = this->GetStart()->GetStemmedDrawingInterface();
    const StemmedDrawingInterface *endStemDrawInterface = this->GetEnd()->GetStemmedDrawingInterface();

    data_STEMDIRECTION startStemDir = STEMDIRECTION_NONE;
    int startStemLen = 0;
    if (startStemDrawInterface) {
        startStemDir = startStemDrawInterface->GetDrawingStemDir();
        startStemLen = startStemDrawInterface->GetDrawingStemLen();
    }
    data_STEMDIRECTION endStemDir = STEMDIRECTION_NONE;
    int endStemLen = 0;
    if (endStemDrawInterface) {
        endStemDir = endStemDrawInterface->GetDrawingStemDir();
        endStemLen = endStemDrawInterface->GetDrawingStemLen();
    }

    // Pointers for the start point of the slur
    const Note *startNote = NULL;
    const LayerElement *start = this->GetStart();
    const Chord *startChord = NULL;
    bool hasStartFlag = false;
    if (start->Is(NOTE)) {
        startNote = vrv_cast<const Note *>(start);
        assert(startNote);
        startChord = startNote->IsChordTone();
        hasStartFlag = (startNote->FindDescendantByType(FLAG) != NULL);
    }
    else if (start->Is(CHORD)) {
        startChord = vrv_cast<const Chord *>(start);
        assert(startChord);
        hasStartFlag = (startChord->FindDescendantByType(FLAG) != NULL);
    }

    // Pointers for the end point of the slur
    const Note *endNote = NULL;
    const LayerElement *end = this->GetEnd();
    const Chord *endChord = NULL;
    if (end->Is(NOTE)) {
        endNote = vrv_cast<const Note *>(end);
        assert(endNote);
        endChord = endNote->IsChordTone();
    }
    else if (end->Is(CHORD)) {
        endChord = vrv_cast<const Chord *>(end);
        assert(endChord);
    }

    const bool isSshaped = this->HasMixedCurveDir();
    const bool isGraceToNoteSlur
        = !start->Is(TIMESTAMP_ATTR) && !end->Is(TIMESTAMP_ATTR) && start->IsGraceNote() && !end->IsGraceNote();

    const PortatoSlurType portatoSlurType = this->IsPortatoSlur(doc, startNote, startChord);

    int y1 = staff->GetDrawingY();
    int y2 = y1;

    bool isShortSlur = false;
    if (x2 - x1 < doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize)) isShortSlur = true;

    int yChordMax = 0, yChordMin = 0;
    const int unit = doc->GetDrawingUnit(staff->m_drawingStaffSize);
    if (((spanningType == SPANNING_START_END) || (spanningType == SPANNING_START)) && !start->Is(TIMESTAMP_ATTR)) {
        // get the radius for adjusting the x position
        const int startRadius = start->GetDrawingRadius(doc);
        // get the min max of the chord (if any) and horizontal correction for flipped notes
        if (startChord) {
            startChord->GetYExtremes(yChordMax, yChordMin);
            if (startNote && startNote->GetFlippedNotehead()) {
                const Note *refNote
                    = (startStemDir == STEMDIRECTION_down) ? startChord->GetTopNote() : startChord->GetBottomNote();
                x1 += refNote->GetDrawingX() - startNote->GetDrawingX();
            }
        }
        // slur is up
        if (this->HasEndpointAboveStart()) {
            // P(^)
            if ((startStemDir == STEMDIRECTION_down) || (startStemLen == 0)) {
                y1 = start->GetDrawingTop(doc, staff->m_drawingStaffSize);
            }
            //  d(^)d
            else if (isShortSlur) {
                y1 = start->GetDrawingTop(doc, staff->m_drawingStaffSize);
            }
            // s-shaped slurs
            else if (isSshaped) {
                y1 = start->GetDrawingTop(doc, staff->m_drawingStaffSize);
                x1 += startRadius - doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
            }
            // portato slurs
            else if (portatoSlurType != PortatoSlurType::None) {
                y1 = start->GetDrawingTop(doc, staff->m_drawingStaffSize);
                const Note *refNote = startChord ? startChord->GetBottomNote() : startNote;
                x1 = refNote->GetDrawingX() + startRadius;
                if (portatoSlurType == PortatoSlurType::StemSide) {
                    x1 += startRadius;
                }
            }
            // same but in beam - adjust the x too
            else if (this->StartsOnBeam() || isGraceToNoteSlur || hasStartFlag) {
                y1 = start->GetDrawingTop(doc, staff->m_drawingStaffSize);
                // Secondary endpoint for grace notes is further left
                double weight = 1.0;
                if (nearEndCollision && (nearEndCollision->metricAtStart > 1.0) && isGraceToNoteSlur) {
                    weight = -0.5;
                    nearEndCollision->endPointsAdjusted = true;
                }
                x1 += weight * (startRadius - doc->GetDrawingStemWidth(staff->m_drawingStaffSize));
            }
            // d(^)
            else {
                if (nearEndCollision && (nearEndCollision->metricAtStart > 0.3)) {
                    // Secondary endpoint on top
                    y1 = start->GetDrawingTop(doc, staff->m_drawingStaffSize);
                    x1 += startRadius - doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
                    nearEndCollision->endPointsAdjusted = true;
                }
                else {
                    // Primary endpoint on the side, move it right
                    x1 += unit * 2;
                    y1 = (startChord) ? (yChordMax + unit * 3) : (start->GetDrawingY() + unit * 3);
                }
            }
        }
        // slur is down
        else {
            // grace note
            if (isGraceToNoteSlur) {
                y1 = start->GetDrawingBottom(doc, staff->m_drawingStaffSize);
                if (startStemDir != STEMDIRECTION_up) {
                    x1 -= startRadius + doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
                }
                else {
                    y1 += unit / 2;
                }
            }
            // d(_)
            else if ((startStemDir == STEMDIRECTION_up) || (startStemLen == 0)) {
                y1 = start->GetDrawingBottom(doc, staff->m_drawingStaffSize);
            }
            // P(_)P
            else if (isShortSlur) {
                y1 = start->GetDrawingBottom(doc, staff->m_drawingStaffSize);
            }
            // s-shaped slurs
            else if (isSshaped) {
                y1 = start->GetDrawingBottom(doc, staff->m_drawingStaffSize);
                x1 -= startRadius - doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
            }
            // portato slurs
            else if (portatoSlurType != PortatoSlurType::None) {
                y1 = start->GetDrawingBottom(doc, staff->m_drawingStaffSize);
                const Note *refNote = startChord ? startChord->GetTopNote() : startNote;
                x1 = refNote->GetDrawingX();
                if (portatoSlurType == PortatoSlurType::Centered) {
                    x1 += startRadius;
                }
            }
            // same but in beam
            else if (this->StartsOnBeam() || hasStartFlag) {
                y1 = start->GetDrawingBottom(doc, staff->m_drawingStaffSize);
                x1 -= startRadius - doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
            }
            // P(_)
            else {
                if (nearEndCollision && (nearEndCollision->metricAtStart > 0.3)) {
                    // Secondary endpoint on bottom
                    y1 = start->GetDrawingBottom(doc, staff->m_drawingStaffSize);
                    x1 -= startRadius - doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
                    nearEndCollision->endPointsAdjusted = true;
                }
                else {
                    // Primary endpoint on the side, but no need to move it right
                    if (startChord) {
                        y1 = yChordMin - unit * 3;
                    }
                    else {
                        y1 = start->GetDrawingY() - unit * 3;
                    }
                }
            }
        }
    }
    if (((spanningType == SPANNING_START_END) || (spanningType == SPANNING_END)) && !end->Is(TIMESTAMP_ATTR)) {
        // get the radius for adjusting the x position
        const int endRadius = end->GetDrawingRadius(doc);
        // get the min max of the chord (if any) and horizontal correction for flipped notes
        if (endChord) {
            endChord->GetYExtremes(yChordMax, yChordMin);
            if (endNote && endNote->GetFlippedNotehead()) {
                const Note *refNote
                    = (endStemDir == STEMDIRECTION_down) ? endChord->GetTopNote() : endChord->GetBottomNote();
                x2 += refNote->GetDrawingX() - endNote->GetDrawingX();
            }
        }
        // get the stem direction of the end
        // slur is up
        if (this->HasEndpointAboveEnd()) {
            // (^)P
            if ((endStemDir == STEMDIRECTION_down) || (endStemLen == 0)) {
                y2 = end->GetDrawingTop(doc, staff->m_drawingStaffSize);
            }
            // d(^)d
            else if (isShortSlur) {
                y2 = end->GetDrawingTop(doc, staff->m_drawingStaffSize);
            }
            // s-shaped slurs
            else if (isSshaped) {
                y2 = end->GetDrawingTop(doc, staff->m_drawingStaffSize);
                x2 += endRadius - doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
            }
            // grace note
            else if (isGraceToNoteSlur) {
                const int yMin = y1 - unit * 4;
                const int yTop = end->GetDrawingTop(doc, staff->m_drawingStaffSize);
                y2 = std::max(end->GetDrawingY() + unit * 2, yMin);
                if (y2 > yTop - unit * 2) {
                    y2 = yTop;
                    x2 += endRadius - doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
                }
            }
            // portato slurs
            else if (portatoSlurType != PortatoSlurType::None) {
                y2 = end->GetDrawingTop(doc, staff->m_drawingStaffSize);
                const Note *refNote = endChord ? endChord->GetBottomNote() : endNote;
                x2 = refNote->GetDrawingX() + endRadius;
                if (portatoSlurType == PortatoSlurType::StemSide) {
                    x2 += endRadius;
                }
            }
            // same but in beam - adjust the x too
            else if (this->EndsOnBeam()) {
                y2 = end->GetDrawingTop(doc, staff->m_drawingStaffSize);
                x2 += endRadius - doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
            }
            // (^)d
            else {
                if (nearEndCollision && nearEndCollision->metricAtEnd > 0.3) {
                    // Secondary endpoint on top
                    y2 = end->GetDrawingTop(doc, staff->m_drawingStaffSize);
                    x2 += endRadius - doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
                    nearEndCollision->endPointsAdjusted = true;
                }
                else {
                    // Primary endpoint on the side, no need to move it left
                    if (endChord) {
                        y2 = yChordMax + unit * 3;
                    }
                    else {
                        y2 = end->GetDrawingY() + unit * 3;
                    }
                }
            }
        }
        else {
            // (_)d
            if ((endStemDir == STEMDIRECTION_up) || (endStemLen == 0)) {
                y2 = end->GetDrawingBottom(doc, staff->m_drawingStaffSize);
            }
            // P(_)P
            else if (isGraceToNoteSlur) {
                const int yMax = y1 + unit;
                const int yBottom = end->GetDrawingBottom(doc, staff->m_drawingStaffSize);
                y2 = std::min(end->GetDrawingY(), yMax);
                if (y2 < yBottom + unit) {
                    y2 = yBottom + unit * 2;
                }
                else {
                    x2 -= endRadius + 2 * doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
                }
            }
            else if (isShortSlur) {
                y2 = end->GetDrawingBottom(doc, staff->m_drawingStaffSize);
            }
            // s-shaped slurs
            else if (isSshaped) {
                y2 = end->GetDrawingBottom(doc, staff->m_drawingStaffSize);
                x2 -= endRadius - doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
            }
            // portato slurs
            else if (portatoSlurType != PortatoSlurType::None) {
                y2 = end->GetDrawingBottom(doc, staff->m_drawingStaffSize);
                const Note *refNote = endChord ? endChord->GetTopNote() : endNote;
                x2 = refNote->GetDrawingX();
                if (portatoSlurType == PortatoSlurType::Centered) {
                    x2 += endRadius;
                }
            }
            // same but in beam
            else if (this->EndsOnBeam()) {
                y2 = end->GetDrawingBottom(doc, staff->m_drawingStaffSize);
                x2 -= endRadius - doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
            }
            // (_)P
            else {
                if (nearEndCollision && nearEndCollision->metricAtEnd > 0.3) {
                    // Secondary endpoint on bottom
                    y2 = end->GetDrawingBottom(doc, staff->m_drawingStaffSize);
                    x2 -= endRadius - doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
                    nearEndCollision->endPointsAdjusted = true;
                }
                else {
                    // Primary endpoint on the side, move it left
                    x2 -= unit * 2;
                    if (endChord) {
                        y2 = yChordMin - unit * 3;
                    }
                    else {
                        y2 = end->GetDrawingY() - unit * 3;
                    }
                }
            }
        }
    }

    // Positions not attached to a note
    int startLoc = 0;
    int endLoc = 0;
    std::tie(startLoc, endLoc) = this->GetStartEndLocs(startNote, startChord, endNote, endChord);

    const int staffSize = doc->GetDrawingStaffSize(staff->m_drawingStaffSize);
    const int staffTop = staff->GetDrawingY();
    const int staffBottom = staffTop - staffSize;

    const int pitchDiff = this->CalcPitchDifference(staff, startLoc, endLoc);
    if (spanningType == SPANNING_START) {
        if (this->HasEndpointAboveStart()) {
            y2 = staffTop + unit;
            if (this->ConsiderMelodicDirection()) {
                y2 = std::max(staffTop, y1);
                y2 += pitchDiff * unit / 2;
                y2 = std::max(staffTop, y2);
            }
        }
        else {
            y2 = staffBottom - unit;
            if (this->ConsiderMelodicDirection()) {
                y2 = std::min(staffBottom, y1);
                y2 += pitchDiff * unit / 2;
                y2 = std::min(staffBottom, y2);
            }
        }
        // Make sure that broken slurs do not look like ties
        if ((std::abs(y1 - y2) < 2 * unit) && (std::abs(x1 - x2) < 2 * staffSize)) {
            const int sign = this->HasEndpointAboveStart() ? 1 : -1;
            y2 = y1 + 2 * sign * unit;
        }
        // At the end of a system, the slur finishes just short of the last barline
        x2 -= (doc->GetDrawingBarLineWidth(staff->m_drawingStaffSize) + unit) / 2;
    }
    if (end->Is(TIMESTAMP_ATTR)) {
        if (this->HasEndpointAboveStart()) {
            y2 = std::max(staffTop, y1);
        }
        else {
            y2 = std::min(staffBottom, y1);
        }
    }
    if (spanningType == SPANNING_END) {
        if (isSshaped != this->HasEndpointAboveEnd()) {
            y1 = staffTop + unit;
            if (this->ConsiderMelodicDirection()) {
                y1 = std::max(staffTop, y2);
                y1 -= pitchDiff * unit / 2;
                y1 = std::max(staffTop, y1);
            }
        }
        else {
            y1 = staffBottom - unit;
            if (this->ConsiderMelodicDirection()) {
                y1 = std::min(staffBottom, y2);
                y1 -= pitchDiff * unit / 2;
                y1 = std::min(staffBottom, y1);
            }
        }
        // Make sure that broken slurs do not look like ties
        if ((std::abs(y1 - y2) < 2 * unit) && (std::abs(x1 - x2) < 2 * staffSize)) {
            const int sign = this->HasEndpointAboveEnd() ? 1 : -1;
            y1 = y2 + 2 * sign * unit;
        }
    }
    if (start->Is(TIMESTAMP_ATTR)) {
        if (this->HasEndpointAboveEnd()) {
            y1 = std::max(staffTop, y2);
        }
        else {
            y1 = std::min(staffBottom, y2);
        }
    }
    // slur across an entire system; use the staff position
    else if (spanningType == SPANNING_MIDDLE) {
        y1 = (drawingCurveDir == curvature_CURVEDIR_above) ? staffTop + unit : staffBottom - unit;
        y2 = y1;
    }

    // Final vertical adjustment based on drawing curve direction
    int sign = (drawingCurveDir == curvature_CURVEDIR_above) ? 1 : -1;
    if (drawingCurveDir == curvature_CURVEDIR_mixed) {
        sign = this->HasEndpointAboveStart() ? 1 : -1;
    }
    y1 += 1.25 * sign * unit;
    if (drawingCurveDir == curvature_CURVEDIR_mixed) {
        sign = this->HasEndpointAboveEnd() ? 1 : -1;
    }
    y2 += 1.25 * sign * unit;

    return std::make_pair(Point(x1, y1), Point(x2, y2));
}

bool Slur::ConsiderMelodicDirection() const
{
    const Measure *startMeasure = vrv_cast<const Measure *>(this->GetStart()->GetFirstAncestor(MEASURE));
    const Measure *endMeasure = vrv_cast<const Measure *>(this->GetEnd()->GetFirstAncestor(MEASURE));

    // Return true if the slur starts in the last measure and ends in the first measure of the next system
    if (startMeasure && endMeasure) {
        return (startMeasure->IsLastInSystem() && (endMeasure->GetIndex() == startMeasure->GetIndex() + 1));
    }
    return false;
}

std::pair<int, int> Slur::GetStartEndLocs(
    const Note *startNote, const Chord *startChord, const Note *endNote, const Chord *endChord) const
{
    int startLoc = startNote ? startNote->GetDrawingLoc() : 0;
    if (startChord) {
        if (this->HasEndpointAboveStart()) {
            startLoc = startChord->GetTopNote()->GetDrawingLoc();
        }
        else {
            startLoc = startChord->GetBottomNote()->GetDrawingLoc();
        }
    }

    int endLoc = endNote ? endNote->GetDrawingLoc() : 0;
    if (endChord) {
        if (this->HasEndpointAboveEnd()) {
            endLoc = endChord->GetTopNote()->GetDrawingLoc();
        }
        else {
            endLoc = endChord->GetBottomNote()->GetDrawingLoc();
        }
    }

    return { startLoc, endLoc };
}

int Slur::CalcPitchDifference(const Staff *staff, int startLoc, int endLoc) const
{
    assert(staff);

    const int staffTopLoc = 2 * (staff->m_drawingLines - 1);
    const int loc1 = this->HasEndpointAboveStart() ? std::max(startLoc, staffTopLoc - 1) : std::min(startLoc, 1);
    const int loc2 = this->HasEndpointAboveEnd() ? std::max(endLoc, staffTopLoc - 1) : std::min(endLoc, 1);

    return loc2 - loc1;
}

PortatoSlurType Slur::IsPortatoSlur(const Doc *doc, const Note *startNote, const Chord *startChord) const
{
    ListOfConstObjects artics;
    if (startChord) {
        artics = startChord->FindAllDescendantsByType(ARTIC, true, 1);
    }
    else if (startNote) {
        artics = startNote->FindAllDescendantsByType(ARTIC, true, 1);
    }

    PortatoSlurType type = PortatoSlurType::None;
    if (!artics.empty()) {
        type = PortatoSlurType::Centered;
        const Artic *artic = vrv_cast<const Artic *>(artics.front());
        // Various cases where portato slurs shouldn't be applied
        if (!artic->IsInsideArtic() || ((artic->GetDrawingPlace() == STAFFREL_above) && this->HasEndpointBelowStart())
            || ((artic->GetDrawingPlace() == STAFFREL_below) && this->HasEndpointAboveStart())) {
            return PortatoSlurType::None;
        }
        // Check for stem side staccato
        // Stem direction is not considered here => this must be checked on client side
        if (!doc->GetOptions()->m_staccatoCenter.GetValue()) {
            const data_ARTICULATION articType = artic->GetArticFirst();
            if ((articType == ARTICULATION_stacc) || (articType == ARTICULATION_stacciss)) {
                type = PortatoSlurType::StemSide;
            }
        }
    }
    return type;
}

bool Slur::HasBoundaryOnBeam(bool isStart) const
{
    const LayerElement *boundary = isStart ? this->GetStart() : this->GetEnd();
    // Check for Beam
    if (const Beam *parentBeam = boundary->GetAncestorBeam(); parentBeam) {
        if (isStart && !parentBeam->IsLastIn(boundary)) return true;
        if (!isStart && !parentBeam->IsFirstIn(boundary)) return true;
    }
    // Check for FTrem
    if (const FTrem *parentFTrem = boundary->GetAncestorFTrem(); parentFTrem) {
        if (isStart && !parentFTrem->IsLastIn(boundary)) return true;
        if (!isStart && !parentFTrem->IsFirstIn(boundary)) return true;
    }
    // Check for BeamSpan
    if (boundary->GetIsInBeamSpan()) {
        return true;
    }
    if (boundary->Is(NOTE)) {
        const Chord *chord = vrv_cast<const Note *>(boundary)->IsChordTone();
        if (chord && chord->GetIsInBeamSpan()) {
            return true;
        }
    }
    return false;
}

void Slur::CalcInitialCurve(const Doc *doc, FloatingCurvePositioner *curve, NearEndCollision *nearEndCollision)
{
    LayerElement *start = this->GetStart();
    LayerElement *end = this->GetEnd();
    if (!start || !end) return;

    Staff *staff = dynamic_cast<Staff *>(curve->GetObjectY());
    if (!staff) return;

    const char spanningType = curve->GetSpanningType();
    const curvature_CURVEDIR curveDir = this->CalcDrawingCurveDir(spanningType);

    // Calculate endpoints
    assert(curve->HasCachedX12());
    const std::pair<int, int> cachedX12 = curve->GetCachedX12();
    const std::pair<Point, Point> endPoints
        = this->CalcEndPoints(doc, staff, nearEndCollision, cachedX12.first, cachedX12.second, curveDir, spanningType);

    // For now we pick C1 = P1 and C2 = P2
    BezierCurve bezier(endPoints.first, endPoints.first, endPoints.second, endPoints.second);
    this->InitBezierControlSides(bezier, curveDir);

    // Angle adjustment
    const bool dontAdjustAngle = curve->IsCrossStaff() || start->IsGraceNote();
    const float nonAdjustedAngle
        = (bezier.p2 == bezier.p1) ? 0 : atan2(bezier.p2.y - bezier.p1.y, bezier.p2.x - bezier.p1.x);
    const float slurAngle
        = dontAdjustAngle ? nonAdjustedAngle : this->GetAdjustedSlurAngle(doc, bezier.p1, bezier.p2, curveDir);
    if (curveDir != curvature_CURVEDIR_mixed) {
        bezier.p2 = BoundingBox::CalcPositionAfterRotation(bezier.p2, -slurAngle, bezier.p1);
    }

    // Calculate control points
    if (this->HasBulge()) {
        bezier.CalcInitialControlPointParams();
    }
    else {
        bezier.CalcInitialControlPointParams(doc, slurAngle, staff->m_drawingStaffSize);
    }
    bezier.UpdateControlPoints();
    if (curveDir != curvature_CURVEDIR_mixed) {
        bezier.Rotate(slurAngle, bezier.p1);
    }

    Point points[4];
    points[0] = bezier.p1;
    points[1] = bezier.c1;
    points[2] = bezier.c2;
    points[3] = bezier.p2;

    // Calculate thickness
    const int thickness
        = doc->GetDrawingUnit(staff->m_drawingStaffSize) * doc->GetOptions()->m_slurMidpointThickness.GetValue();

    // Store everything in floating curve positioner
    curve->UpdateCurveParams(points, thickness, curveDir);
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

FunctorCode Slur::Accept(Functor &functor)
{
    return functor.VisitSlur(this);
}

FunctorCode Slur::Accept(ConstFunctor &functor) const
{
    return functor.VisitSlur(this);
}

FunctorCode Slur::AcceptEnd(Functor &functor)
{
    return functor.VisitSlurEnd(this);
}

FunctorCode Slur::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitSlurEnd(this);
}

} // namespace vrv
