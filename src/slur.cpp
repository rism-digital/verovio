/////////////////////////////////////////////////////////////////////////////
// Name:        slur.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "slur.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <cmath>
#include <math.h>

//----------------------------------------------------------------------------

#include "beam.h"
#include "chord.h"
#include "comparison.h"
#include "doc.h"
#include "ftrem.h"
#include "functorparams.h"
#include "layer.h"
#include "layerelement.h"
#include "note.h"
#include "staff.h"
#include "system.h"
#include "verticalaligner.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Slur
//----------------------------------------------------------------------------

static const ClassRegistrar<Slur> s_factory("slur", SLUR);

Slur::Slur()
    : ControlElement(SLUR, "slur-")
    , TimeSpanningInterface()
    , AttColor()
    , AttCurvature()
    , AttCurveRend()
    , AttLayerIdent()
{
    this->RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_CURVATURE);
    this->RegisterAttClass(ATT_CURVEREND);
    this->RegisterAttClass(ATT_LAYERIDENT);

    this->Reset();
}

Slur::Slur(ClassId classId)
    : ControlElement(classId, "slur-")
    , TimeSpanningInterface()
    , AttColor()
    , AttCurvature()
    , AttCurveRend()
    , AttLayerIdent()
{
    this->RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_CURVATURE);
    this->RegisterAttClass(ATT_CURVEREND);
    this->RegisterAttClass(ATT_LAYERIDENT);

    this->Reset();
}

Slur::Slur(ClassId classId, const std::string &classIdStr)
    : ControlElement(classId, classIdStr)
    , TimeSpanningInterface()
    , AttColor()
    , AttCurvature()
    , AttCurveRend()
    , AttLayerIdent()
{
    this->RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_CURVATURE);
    this->RegisterAttClass(ATT_CURVEREND);
    this->RegisterAttClass(ATT_LAYERIDENT);

    this->Reset();
}

Slur::~Slur() {}

void Slur::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    this->ResetColor();
    this->ResetCurvature();
    this->ResetCurveRend();
    this->ResetLayerIdent();

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

std::pair<Layer *, LayerElement *> Slur::GetBoundaryLayer()
{
    LayerElement *start = this->GetStart();
    LayerElement *end = this->GetEnd();

    if (!start || !end) return { NULL, NULL };

    Layer *layer = NULL;
    LayerElement *layerElement = NULL;
    // For now, with timestamps, get the first layer. We should eventually look at the @layerident (not implemented)
    if (!start->Is(TIMESTAMP_ATTR)) {
        layer = dynamic_cast<Layer *>(start->GetFirstAncestor(LAYER));
        layerElement = start;
    }
    else if (!end->Is(TIMESTAMP_ATTR)) {
        layer = dynamic_cast<Layer *>(end->GetFirstAncestor(LAYER));
        layerElement = end;
    }
    if (layerElement && layerElement->m_crossStaff) layer = layerElement->m_crossLayer;

    return { layer, layerElement };
}

Staff *Slur::GetBoundaryCrossStaff()
{
    LayerElement *start = this->GetStart();
    LayerElement *end = this->GetEnd();

    if (!start || !end) return NULL;

    if (start->m_crossStaff != end->m_crossStaff) {
        return end->m_crossStaff;
    }
    else {
        // Check if the two elements are in different staves (but themselves not cross-staff)
        Staff *startStaff = start->GetAncestorStaff(ANCESTOR_ONLY, false);
        Staff *endStaff = end->GetAncestorStaff(ANCESTOR_ONLY, false);
        if (startStaff && endStaff && (startStaff->GetN() != endStaff->GetN())) {
            return endStaff;
        }
        else {
            return NULL;
        }
    }
}

std::vector<LayerElement *> Slur::CollectSpannedElements(Staff *staff, int xMin, int xMax, char spanningType)
{
    // Decide whether we search the whole parent system or just one measure which is much faster
    Object *container = this->IsSpanningMeasures() ? staff->GetFirstAncestor(SYSTEM) : this->GetStartMeasure();

    FindSpannedLayerElementsParams findSpannedLayerElementsParams(this);
    findSpannedLayerElementsParams.m_minPos = xMin;
    findSpannedLayerElementsParams.m_maxPos = xMax;
    findSpannedLayerElementsParams.m_inMeasureRange
        = ((spanningType == SPANNING_MIDDLE) || (spanningType == SPANNING_END));
    findSpannedLayerElementsParams.m_classIds = { ACCID, ARTIC, CHORD, CLEF, FLAG, GLISS, NOTE, STEM, TUPLET_BRACKET,
        TUPLET_NUM }; // Ties should be handled separately

    std::set<int> staffNumbers;
    staffNumbers.emplace(staff->GetN());
    Staff *startStaff = this->GetStart()->GetAncestorStaff(RESOLVE_CROSS_STAFF, false);
    Staff *endStaff = this->GetEnd()->GetAncestorStaff(RESOLVE_CROSS_STAFF, false);
    if (startStaff && (startStaff != staff)) {
        staffNumbers.emplace(startStaff->GetN());
    }
    else if (endStaff && (endStaff != staff)) {
        staffNumbers.emplace(endStaff->GetN());
    }
    findSpannedLayerElementsParams.m_staffNs = staffNumbers;

    // Run the search without layer bounds
    Functor findSpannedLayerElements(&Object::FindSpannedLayerElements);
    Functor findSpannedLayerElementsEnd(&Object::FindSpannedLayerElementsEnd);
    container->Process(&findSpannedLayerElements, &findSpannedLayerElementsParams, &findSpannedLayerElementsEnd);

    // Now determine the minimal and maximal layer
    std::set<int> layersN;
    if (this->HasLayer()) {
        layersN = { this->GetLayer() };
    }
    else {
        for (LayerElement *element : { this->GetStart(), this->GetEnd() }) {
            const int layerN = element->GetOriginalLayerN();
            layersN.insert(layerN);
        }
    }
    const int minLayerN = *layersN.begin();
    const int maxLayerN = *layersN.rbegin();

    // Check whether outside layers exist
    const bool hasOutsideLayers = std::any_of(findSpannedLayerElementsParams.m_elements.cbegin(),
        findSpannedLayerElementsParams.m_elements.cend(), [minLayerN, maxLayerN](LayerElement *element) {
            const int layerN = element->GetOriginalLayerN();
            return ((layerN < minLayerN) || (layerN > maxLayerN));
        });

    if (hasOutsideLayers) {
        // Filter all notes, also include the notes of the start and end of the slur
        ListOfObjects notes;
        std::copy_if(findSpannedLayerElementsParams.m_elements.cbegin(),
            findSpannedLayerElementsParams.m_elements.cend(), std::back_inserter(notes),
            [](LayerElement *element) { return element->Is(NOTE); });
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
        for (Object *object : notes) {
            Note *note = vrv_cast<Note *>(object);
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
        const bool layersAreSeparated
            = std::all_of(notes.cbegin(), notes.cend(), [minLayerN, maxLayerN, minPitch, maxPitch](Object *object) {
                  Note *note = vrv_cast<Note *>(object);
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
            findSpannedLayerElementsParams.m_elements.clear();
            findSpannedLayerElementsParams.m_inMeasureRange
                = ((spanningType == SPANNING_MIDDLE) || (spanningType == SPANNING_END));
            findSpannedLayerElementsParams.m_minLayerN = minLayerN;
            findSpannedLayerElementsParams.m_maxLayerN = maxLayerN;
            container->Process(
                &findSpannedLayerElements, &findSpannedLayerElementsParams, &findSpannedLayerElementsEnd);
        }
    }

    return findSpannedLayerElementsParams.m_elements;
}

Staff *Slur::CalculateExtremalStaff(Staff *staff, int xMin, int xMax, char spanningType)
{
    Staff *extremalStaff = staff;

    const SlurCurveDirection curveDir = this->GetDrawingCurveDir();
    const std::vector<LayerElement *> spannedElements = this->CollectSpannedElements(staff, xMin, xMax, spanningType);

    // The floating curve positioner of cross staff slurs should live in the lower/upper staff alignment
    // corresponding to whether the slur is curved below or not
    auto adaptStaff = [&extremalStaff, curveDir](LayerElement *element) {
        Staff *elementStaff = element->GetAncestorStaff(RESOLVE_CROSS_STAFF);
        const bool updateExtremal = (curveDir == SlurCurveDirection::Below)
            ? (elementStaff->GetN() > extremalStaff->GetN())
            : (elementStaff->GetN() < extremalStaff->GetN());
        if (updateExtremal) {
            extremalStaff = elementStaff;
        }
    };

    // Run once through all spanned elements
    std::for_each(spannedElements.begin(), spannedElements.end(), adaptStaff);

    // Also check the beams of spanned elements
    std::for_each(spannedElements.begin(), spannedElements.end(), [&adaptStaff](LayerElement *element) {
        if (Beam *beam = element->IsInBeam(); beam) {
            adaptStaff(beam);
        }
    });

    return extremalStaff;
}

bool Slur::IsElementBelow(LayerElement *element, Staff *startStaff, Staff *endStaff) const
{
    switch (this->GetDrawingCurveDir()) {
        case SlurCurveDirection::Above: return true;
        case SlurCurveDirection::Below: return false;
        case SlurCurveDirection::AboveBelow:
            return (element->GetAncestorStaff(RESOLVE_CROSS_STAFF)->GetN() == startStaff->GetN());
        case SlurCurveDirection::BelowAbove:
            return (element->GetAncestorStaff(RESOLVE_CROSS_STAFF)->GetN() == endStaff->GetN());
        default: return false;
    }
}

bool Slur::IsElementBelow(FloatingPositioner *positioner, Staff *startStaff, Staff *endStaff) const
{
    switch (this->GetDrawingCurveDir()) {
        case SlurCurveDirection::Above: return true;
        case SlurCurveDirection::Below: return false;
        case SlurCurveDirection::AboveBelow:
            return (positioner->GetAlignment()->GetStaff()->GetN() == startStaff->GetN());
        case SlurCurveDirection::BelowAbove:
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

void Slur::AdjustSlur(Doc *doc, FloatingCurvePositioner *curve, Staff *staff)
{
    assert(doc);
    assert(curve);
    assert(staff);

    Point points[4];
    curve->GetPoints(points);
    BezierCurve bezier(points[0], points[1], points[2], points[3]);
    this->InitBezierControlSides(bezier, curve->GetDir());
    bezier.UpdateControlPointParams();

    const int unit = doc->GetDrawingUnit(100);
    const int margin = doc->GetOptions()->m_slurMargin.GetValue() * unit;

    // STEP 1: Filter spanned elements and discard certain bounding boxes even though they collide
    this->FilterSpannedElements(curve, bezier, margin);

    // STEP 2: Calculate the vertical adjustment of end points. This shifts the slur vertically.
    // Only collisions near the endpoints are taken into account.
    int endPointShiftLeft = 0;
    int endPointShiftRight = 0;
    std::tie(endPointShiftLeft, endPointShiftRight) = this->CalcEndPointShift(curve, bezier, margin, unit);
    if ((endPointShiftLeft != 0) || (endPointShiftRight != 0)) {
        const int signLeft = bezier.IsLeftControlAbove() ? 1 : -1;
        const int signRight = bezier.IsRightControlAbove() ? 1 : -1;
        bezier.p1.y += signLeft * endPointShiftLeft;
        bezier.p2.y += signRight * endPointShiftRight;
        if (bezier.p1.x != bezier.p2.x) {
            double lambda = double(bezier.c1.x - bezier.p1.x) / double(bezier.p2.x - bezier.p1.x);
            bezier.c1.y += signLeft * (1.0 - lambda) * endPointShiftLeft + signRight * lambda * endPointShiftRight;
            lambda = double(bezier.c2.x - bezier.p1.x) / double(bezier.p2.x - bezier.p1.x);
            bezier.c2.y += signLeft * (1.0 - lambda) * endPointShiftLeft + signRight * lambda * endPointShiftRight;
        }
        bezier.UpdateControlPointParams();
        curve->UpdatePoints(bezier);
    }

    // STEP 3: Calculate the horizontal offset of the control points.
    // The idea is to shift control points to the outside if there is an obstacle in the vicinity of the corresponding
    // endpoint. For C1 we consider the largest angle <)BP1P2 where B is a colliding left bounding box corner and choose
    // C1 in this direction. Similar for C2.
    bool ok = false;
    int controlPointOffsetLeft = 0;
    int controlPointOffsetRight = 0;
    std::tie(ok, controlPointOffsetLeft, controlPointOffsetRight) = this->CalcControlPointOffset(curve, bezier, margin);
    if (ok) {
        bezier.SetLeftControlOffset(controlPointOffsetLeft);
        bezier.SetRightControlOffset(controlPointOffsetRight);
        bezier.UpdateControlPoints();
        curve->UpdatePoints(bezier);
    }

    // STEP 4: Calculate the vertical shift of the control points.
    // For each colliding bounding box we formulate a constraint ax + by >= c
    // where x, y denote the vertical adjustments of the control points and c is the size of the collision.
    // The coefficients a, b are calculated from the Bezier curve equation.
    // After collecting all constraints we calculate a solution.
    const ControlPointAdjustment adjustment = this->CalcControlPointVerticalShift(curve, bezier, margin);
    const int leftSign = (bezier.IsLeftControlAbove() == adjustment.moveUpwards) ? 1 : -1;
    bezier.SetLeftControlHeight(bezier.GetLeftControlHeight() + leftSign * adjustment.leftShift);
    const int rightSign = (bezier.IsRightControlAbove() == adjustment.moveUpwards) ? 1 : -1;
    bezier.SetRightControlHeight(bezier.GetRightControlHeight() + rightSign * adjustment.rightShift);
    bezier.UpdateControlPoints();
    curve->UpdatePoints(bezier);
    curve->SetRequestedStaffSpace(adjustment.requestedStaffSpace);

    // STEP 5: Adjust the slur shape
    // Through the control point adjustments in step 3 and 4 it can happen that the slur looses its desired shape.
    // We correct the shape if the slur is too flat or not convex.
    if (curve->GetDir() != curvature_CURVEDIR_mixed) {
        this->AdjustSlurShape(bezier, curve->GetDir(), unit);
        curve->UpdatePoints(bezier);
    }

    // Since we are going to redraw it, reset its bounding box
    curve->BoundingBox::ResetBoundingBox();
}

void Slur::FilterSpannedElements(FloatingCurvePositioner *curve, const BezierCurve &bezierCurve, int margin)
{
    if (bezierCurve.p1.x >= bezierCurve.p2.x) return;

    const int dist = bezierCurve.p2.x - bezierCurve.p1.x;

    const ArrayOfCurveSpannedElements *spannedElements = curve->GetSpannedElements();

    for (auto spannedElement : *spannedElements) {

        if (spannedElement->m_discarded) {
            continue;
        }

        bool discard = false;
        const int intersection = curve->CalcDirectionalAdjustment(
            spannedElement->m_boundingBox, spannedElement->m_isBelow, discard, margin);
        const int xMiddle
            = (spannedElement->m_boundingBox->GetSelfLeft() + spannedElement->m_boundingBox->GetSelfRight()) / 2.0;
        const float distanceRatio = float(xMiddle - bezierCurve.p1.x) / float(dist);

        // In case of cross-staff, ignore obstacles which completely lie on the other side of the slur near the
        // endpoints
        const int elementHeight
            = std::abs(spannedElement->m_boundingBox->GetSelfTop() - spannedElement->m_boundingBox->GetSelfBottom());
        if (curve->IsCrossStaff() && (intersection > elementHeight + 4 * margin)
            && (std::abs(distanceRatio - 0.5) > 0.45)) {
            spannedElement->m_discarded = true;
        }
    }
}

std::pair<int, int> Slur::CalcEndPointShift(
    FloatingCurvePositioner *curve, const BezierCurve &bezierCurve, const int margin, const int unit)
{
    if (bezierCurve.p1.x >= bezierCurve.p2.x) return { 0, 0 };

    int shiftLeft = 0;
    int shiftRight = 0;

    const int dist = bezierCurve.p2.x - bezierCurve.p1.x;

    const ArrayOfCurveSpannedElements *spannedElements = curve->GetSpannedElements();
    for (auto spannedElement : *spannedElements) {

        if (spannedElement->m_discarded) {
            continue;
        }

        bool discard = false;
        int intersectionLeft, intersectionRight;
        std::tie(intersectionLeft, intersectionRight) = curve->CalcDirectionalLeftRightAdjustment(
            spannedElement->m_boundingBox, spannedElement->m_isBelow, discard, margin);

        if (discard) {
            spannedElement->m_discarded = true;
            continue;
        }

        if ((intersectionLeft > 0) || (intersectionRight > 0)) {
            // Now apply the intersections on the left and right hand side of the bounding box
            const int xLeft = std::max(bezierCurve.p1.x, spannedElement->m_boundingBox->GetSelfLeft());
            const float distanceRatioLeft = float(xLeft - bezierCurve.p1.x) / float(dist);
            this->ShiftEndPoints(shiftLeft, shiftRight, distanceRatioLeft, intersectionLeft, spannedElement->m_isBelow);

            const int xRight = std::min(bezierCurve.p2.x, spannedElement->m_boundingBox->GetSelfRight());
            const float distanceRatioRight = float(xRight - bezierCurve.p1.x) / float(dist);
            this->ShiftEndPoints(
                shiftLeft, shiftRight, distanceRatioRight, intersectionRight, spannedElement->m_isBelow);
        }
    }

    this->RebalanceShifts(shiftLeft, shiftRight, dist, unit);

    return { shiftLeft, shiftRight };
}

void Slur::ShiftEndPoints(int &shiftLeft, int &shiftRight, double ratio, int intersection, bool isBelow) const
{
    // Filter collisions near the endpoints
    // Collisions with 0.15 <= ratio <= 0.85 do not contribute to shifts
    // They are compensated later by shifting the control points
    if ((ratio < 0.15) && (this->HasEndpointAboveStart() == isBelow)) {
        if (ratio > 0.05) {
            // For 0.05 <= ratio <= 0.15 collisions only partially contribute to shifts
            // We multiply with a function that interpolates between 1 and 0
            intersection *= pow(1.5 - 10.0 * ratio, 2.0);
        }
        shiftLeft = std::max(shiftLeft, intersection);
    }
    else if ((ratio > 0.85) && (this->HasEndpointAboveEnd() == isBelow)) {
        if (ratio < 0.95) {
            // For 0.85 <= ratio <= 0.95 collisions only partially contribute to shifts
            // We multiply with a function that interpolates between 0 and 1
            intersection *= pow(10.0 * ratio - 8.5, 2.0);
        }
        shiftRight = std::max(shiftRight, intersection);
    }
}

void Slur::RebalanceShifts(int &shiftLeft, int &shiftRight, const double distance, const int unit) const
{
    // alpha is 1 for dist <= 4U, 0 for dist >= 8U, interpolated between 4U and 8U
    double alpha = 0.0;
    if (distance <= 4.0 * unit) {
        alpha = 1.0;
    }
    else if (distance <= 8.0 * unit) {
        alpha = 2.0 - distance / (4.0 * unit);
    }

    const int difference = std::abs(shiftLeft - shiftRight);
    if (shiftLeft < shiftRight) {
        shiftLeft += alpha * difference;
    }
    else {
        shiftRight += alpha * difference;
    }
}

std::tuple<bool, int, int> Slur::CalcControlPointOffset(
    FloatingCurvePositioner *curve, const BezierCurve &bezierCurve, int margin)
{
    if (bezierCurve.p1.x >= bezierCurve.p2.x) return { false, 0, 0 };

    // Initially we start with the slopes of the lines P1-C1 and P2-C2
    double leftSlopeMax = std::abs(BoundingBox::CalcSlope(bezierCurve.p1, bezierCurve.c1));
    double rightSlopeMax = std::abs(BoundingBox::CalcSlope(bezierCurve.p2, bezierCurve.c2));
    const ArrayOfCurveSpannedElements *spannedElements = curve->GetSpannedElements();
    for (auto spannedElement : *spannedElements) {

        if (spannedElement->m_discarded) {
            continue;
        }

        const int bbY = spannedElement->m_isBelow ? spannedElement->m_boundingBox->GetSelfTop()
                                                  : spannedElement->m_boundingBox->GetSelfBottom();
        const Point pLeft = { spannedElement->m_boundingBox->GetSelfLeft(), bbY };
        const Point pRight = { spannedElement->m_boundingBox->GetSelfRight(), bbY };

        // Prefer the (increased) slope of P1-B1, if larger
        // B1 is the upper left bounding box corner of a colliding obstacle
        if ((pLeft.x > bezierCurve.p1.x + margin) && (bezierCurve.IsLeftControlAbove() == spannedElement->m_isBelow)) {
            const double slope = BoundingBox::CalcSlope(bezierCurve.p1, pLeft);
            if ((slope > 0.0) && bezierCurve.IsLeftControlAbove()) {
                const double adjustedSlope = this->RotateSlope(slope, 10.0, 2.5, true);
                leftSlopeMax = std::max(leftSlopeMax, adjustedSlope);
            }
            if ((slope < 0.0) && !bezierCurve.IsLeftControlAbove()) {
                const double adjustedSlope = this->RotateSlope(-slope, 10.0, 2.5, true);
                leftSlopeMax = std::max(leftSlopeMax, adjustedSlope);
            }
        }

        // Prefer the (increased) slope of P2-B2, if larger
        // B2 is the upper right bounding box corner of a colliding obstacle
        if ((pRight.x < bezierCurve.p2.x - margin)
            && (bezierCurve.IsRightControlAbove() == spannedElement->m_isBelow)) {
            const double slope = BoundingBox::CalcSlope(bezierCurve.p2, pRight);
            if ((slope < 0.0) && bezierCurve.IsRightControlAbove()) {
                const double adjustedSlope = this->RotateSlope(-slope, 10.0, 2.5, true);
                rightSlopeMax = std::max(rightSlopeMax, adjustedSlope);
            }
            if ((slope > 0.0) && !bezierCurve.IsRightControlAbove()) {
                const double adjustedSlope = this->RotateSlope(slope, 10.0, 2.5, true);
                rightSlopeMax = std::max(rightSlopeMax, adjustedSlope);
            }
        }
    }

    if ((leftSlopeMax == 0.0) || (rightSlopeMax == 0.0)) return { false, 0, 0 };

    // Calculate offset from extremal slope, but use 1/20 of horizontal distance as minimum
    const int minOffset = (bezierCurve.p2.x - bezierCurve.p1.x) / 20;
    int leftOffset = minOffset;
    if (bezierCurve.GetLeftControlOffset() > 0) {
        leftOffset = std::max<int>(leftOffset, std::abs(bezierCurve.GetLeftControlHeight()) / leftSlopeMax);
    }
    int rightOffset = minOffset;
    if (bezierCurve.GetRightControlOffset() > 0) {
        rightOffset = std::max<int>(rightOffset, std::abs(bezierCurve.GetRightControlHeight()) / rightSlopeMax);
    }

    return { true, leftOffset, rightOffset };
}

ControlPointAdjustment Slur::CalcControlPointVerticalShift(
    FloatingCurvePositioner *curve, const BezierCurve &bezierCurve, int margin)
{
    ControlPointAdjustment adjustment{ 0, 0, false, 0 };
    if (bezierCurve.p1.x >= bezierCurve.p2.x) return adjustment;

    std::list<ControlPointConstraint> aboveConstraints;
    std::list<ControlPointConstraint> belowConstraints;
    int maxIntersectionAbove = 0;
    int maxIntersectionBelow = 0;

    const int dist = bezierCurve.p2.x - bezierCurve.p1.x;

    const ArrayOfCurveSpannedElements *spannedElements = curve->GetSpannedElements();

    for (auto spannedElement : *spannedElements) {

        if (spannedElement->m_discarded) {
            continue;
        }

        bool discard = false;
        int intersectionLeft, intersectionRight;
        std::tie(intersectionLeft, intersectionRight) = curve->CalcDirectionalLeftRightAdjustment(
            spannedElement->m_boundingBox, spannedElement->m_isBelow, discard, margin);

        if (discard) {
            spannedElement->m_discarded = true;
            continue;
        }

        std::list<ControlPointConstraint> &constraints
            = spannedElement->m_isBelow ? belowConstraints : aboveConstraints;
        int &maxIntersection = spannedElement->m_isBelow ? maxIntersectionBelow : maxIntersectionAbove;

        if ((intersectionLeft > 0) || (intersectionRight > 0)) {
            Point points[4];
            points[0] = bezierCurve.p1;
            points[1] = bezierCurve.c1;
            points[2] = bezierCurve.c2;
            points[3] = bezierCurve.p2;

            // Add constraint for the left boundary of the colliding bounding box
            const int xLeft = std::max(bezierCurve.p1.x, spannedElement->m_boundingBox->GetSelfLeft());
            float distanceRatio = float(xLeft - bezierCurve.p1.x) / float(dist);
            // Ignore obstacles close to the endpoints, because this would result in very large shifts
            if ((std::abs(0.5 - distanceRatio) < 0.45) && (intersectionLeft > 0)) {
                const double t = BoundingBox::CalcBezierParamAtPosition(points, xLeft);
                constraints.push_back(
                    { 3.0 * pow(1.0 - t, 2.0) * t, 3.0 * (1.0 - t) * pow(t, 2.0), double(intersectionLeft) });
                maxIntersection = std::max(maxIntersection, intersectionLeft);
            }

            // Add constraint for the right boundary of the colliding bounding box
            const int xRight = std::min(bezierCurve.p2.x, spannedElement->m_boundingBox->GetSelfRight());
            distanceRatio = float(xRight - bezierCurve.p1.x) / float(dist);
            // Ignore obstacles close to the endpoints, because this would result in very large shifts
            if ((std::abs(0.5 - distanceRatio) < 0.45) && (intersectionRight > 0)) {
                const double t = BoundingBox::CalcBezierParamAtPosition(points, xRight);
                constraints.push_back(
                    { 3.0 * pow(1.0 - t, 2.0) * t, 3.0 * (1.0 - t) * pow(t, 2.0), double(intersectionRight) });
                maxIntersection = std::max(maxIntersection, intersectionRight);
            }
        }
    }

    // Solve the constraints and calculate the adjustment
    if (maxIntersectionAbove > maxIntersectionBelow) {
        std::tie(adjustment.leftShift, adjustment.rightShift) = this->SolveControlPointConstraints(aboveConstraints);
        adjustment.moveUpwards = false;
    }
    else {
        std::tie(adjustment.leftShift, adjustment.rightShift) = this->SolveControlPointConstraints(belowConstraints);
        adjustment.moveUpwards = true;
    }

    // Determine the requested staff space
    if (bezierCurve.IsLeftControlAbove() && !bezierCurve.IsRightControlAbove()) {
        adjustment.requestedStaffSpace = std::max(bezierCurve.p1.y - bezierCurve.p2.y + 6 * margin, 0);
    }
    else if (!bezierCurve.IsLeftControlAbove() && bezierCurve.IsRightControlAbove()) {
        adjustment.requestedStaffSpace = std::max(bezierCurve.p2.y - bezierCurve.p1.y + 6 * margin, 0);
    }
    if ((maxIntersectionAbove > 0) && (maxIntersectionBelow > 0)) {
        adjustment.requestedStaffSpace
            = std::max(adjustment.requestedStaffSpace, maxIntersectionAbove + maxIntersectionBelow);
    }

    return adjustment;
}

std::pair<int, int> Slur::SolveControlPointConstraints(const std::list<ControlPointConstraint> &constraints)
{
    if (constraints.empty()) return { 0, 0 };

    // Each constraint corresponds to a halfplane in the upper right quadrant.
    // We consider the line through the origin orthogonal to the halfplane's boundary
    // and average the slopes of these orthogonal lines.
    double weightSum = 0.0;
    double weightedAngleSum = 0.0;
    for (const ControlPointConstraint &constraint : constraints) {
        // Use the distance of the halfplane's boundary to the origin as weight
        const double weight = constraint.c / std::hypot(constraint.a, constraint.b);
        weightedAngleSum += weight * atan(constraint.b / constraint.a);
        weightSum += weight;
    }
    const double slope = tan(weightedAngleSum / weightSum);

    // Now follow the line with the averaged slope until we have hit all halfplanes.
    // For each constraint we must solve: slope * x = c/b - a/b * x
    double xMax = 0.0;
    for (const ControlPointConstraint &constraint : constraints) {
        const double x = constraint.c / (constraint.a + slope * constraint.b);
        xMax = std::max(xMax, x);
    }

    // The point which hits the last halfplane is the desired solution.
    return { xMax, slope * xMax };
}

void Slur::AdjustSlurShape(BezierCurve &bezierCurve, curvature_CURVEDIR dir, int unit)
{
    if (bezierCurve.p1.x >= bezierCurve.p2.x) return;

    // Normalize the slur via rotation (such that p1-p2 is horizontal)
    const float angle = atan2(bezierCurve.p2.y - bezierCurve.p1.y, bezierCurve.p2.x - bezierCurve.p1.x);
    bezierCurve.Rotate(-angle, bezierCurve.p1);
    bezierCurve.UpdateControlPointParams();

    // *** STEP 1: Ensure MINIMAL HEIGHT ***
    // <)C1P1P2 should be at least 30 degrees, but allow smaller angles if the midpoint would be lifted more than 6 MEI
    // units
    // Similar for <)P1P2C2
    const int sign = (dir == curvature_CURVEDIR_above) ? 1 : -1;
    const Point shiftedMidpoint(
        (bezierCurve.p1.x + bezierCurve.p2.x) / 2, (bezierCurve.p1.y + bezierCurve.p2.y) / 2 + sign * 6 * unit);
    const float minAngle = this->GetMinControlPointAngle(bezierCurve, angle / M_PI * 180.0, unit);
    const bool ignoreLeft = (bezierCurve.c1.x <= bezierCurve.p1.x);
    const bool ignoreRight = (bezierCurve.c2.x >= bezierCurve.p2.x);
    double slopeLeft = BoundingBox::CalcSlope(bezierCurve.p1, bezierCurve.c1);
    double slopeRight = BoundingBox::CalcSlope(bezierCurve.p2, bezierCurve.c2);
    const double slopeBase = BoundingBox::CalcSlope(bezierCurve.p1, bezierCurve.p2);

    if (dir == curvature_CURVEDIR_above) {
        double minSlopeLeft = this->RotateSlope(slopeBase, minAngle, 1.0, true);
        minSlopeLeft = std::min(minSlopeLeft, BoundingBox::CalcSlope(bezierCurve.p1, shiftedMidpoint));
        slopeLeft = std::max(slopeLeft, minSlopeLeft);
        double minSlopeRight = this->RotateSlope(slopeBase, minAngle, 1.0, false);
        minSlopeRight = std::max(minSlopeRight, BoundingBox::CalcSlope(bezierCurve.p2, shiftedMidpoint));
        slopeRight = std::min(slopeRight, minSlopeRight);
    }
    else if (dir == curvature_CURVEDIR_below) {
        double minSlopeLeft = this->RotateSlope(slopeBase, minAngle, 1.0, false);
        minSlopeLeft = std::max(minSlopeLeft, BoundingBox::CalcSlope(bezierCurve.p1, shiftedMidpoint));
        if (!ignoreLeft) slopeLeft = std::min(slopeLeft, minSlopeLeft);
        double minSlopeRight = this->RotateSlope(slopeBase, minAngle, 1.0, true);
        minSlopeRight = std::min(minSlopeRight, BoundingBox::CalcSlope(bezierCurve.p2, shiftedMidpoint));
        if (!ignoreRight) slopeRight = std::max(slopeRight, minSlopeRight);
    }

    // Update control points
    if (!ignoreLeft) bezierCurve.SetLeftControlHeight(slopeLeft * sign * bezierCurve.GetLeftControlOffset());
    if (!ignoreRight) bezierCurve.SetRightControlHeight(slopeRight * -sign * bezierCurve.GetRightControlOffset());
    bezierCurve.UpdateControlPoints();

    // *** STEP 2: Ensure CONVEXITY ***
    // <)C1P1C2 and <)C1P2C2 should be at least 3 degree
    if (dir == curvature_CURVEDIR_above) {
        const double minSlopeLeft
            = this->RotateSlope(BoundingBox::CalcSlope(bezierCurve.p1, bezierCurve.c2), 3.0, 10.0, true);
        slopeLeft = std::max(slopeLeft, minSlopeLeft);
        const double minSlopeRight
            = this->RotateSlope(BoundingBox::CalcSlope(bezierCurve.p2, bezierCurve.c1), 3.0, 10.0, false);
        slopeRight = std::min(slopeRight, minSlopeRight);
    }
    else if (dir == curvature_CURVEDIR_below) {
        const double minSlopeLeft
            = this->RotateSlope(BoundingBox::CalcSlope(bezierCurve.p1, bezierCurve.c2), 3.0, 10.0, false);
        slopeLeft = std::min(slopeLeft, minSlopeLeft);
        const double minSlopeRight
            = this->RotateSlope(BoundingBox::CalcSlope(bezierCurve.p2, bezierCurve.c1), 3.0, 10.0, true);
        slopeRight = std::max(slopeRight, minSlopeRight);
    }

    // Update control points
    if (!ignoreLeft) bezierCurve.SetLeftControlHeight(slopeLeft * sign * bezierCurve.GetLeftControlOffset());
    if (!ignoreRight) bezierCurve.SetRightControlHeight(slopeRight * -sign * bezierCurve.GetRightControlOffset());
    bezierCurve.UpdateControlPoints();

    // Rotate back
    bezierCurve.Rotate(angle, bezierCurve.p1);
    bezierCurve.UpdateControlPointParams();
}

double Slur::RotateSlope(double slope, double degrees, double doublingBound, bool upwards) const
{
    assert(degrees >= 0.0);
    assert(doublingBound >= 0.0);

    if (upwards && (slope >= doublingBound)) return slope * 2.0;
    if (!upwards && (slope <= -doublingBound)) return slope * 2.0;
    const int sign = upwards ? 1 : -1;
    return tan(atan(slope) + sign * M_PI * degrees / 180.0);
}

float Slur::GetMinControlPointAngle(const BezierCurve &bezierCurve, float angle, int unit) const
{
    angle = abs(angle);
    const double distance = double(bezierCurve.p2.x - bezierCurve.p1.x) / unit;

    // Increase min angle for short and angled slurs
    double angleIncrement = std::min(angle / 4.0, 15.0); // values in [0.0, 15.0]
    double factor = 1.0 - (distance - 8.0) / 8.0;
    factor = std::min(factor, 1.0);
    factor = std::max(factor, 0.0); // values in [0.0, 1.0]

    // not if control points are horizontally in a degenerated position
    if ((bezierCurve.c1.x < bezierCurve.p1.x) || (2.0 * bezierCurve.c1.x > bezierCurve.p1.x + bezierCurve.p2.x)) {
        angleIncrement = 0.0;
    }
    if ((bezierCurve.c2.x > bezierCurve.p2.x) || (2.0 * bezierCurve.c2.x < bezierCurve.p1.x + bezierCurve.p2.x)) {
        angleIncrement = 0.0;
    }

    return 30.0 + angleIncrement * factor;
}

float Slur::GetAdjustedSlurAngle(Doc *doc, Point &p1, Point &p2, curvature_CURVEDIR curveDir)
{
    float slurAngle = (p1 == p2) ? 0 : atan2(p2.y - p1.y, p2.x - p1.x);
    const float maxAngle = (float)doc->GetOptions()->m_slurMaxSlope.GetValue() * M_PI / 180.0;

    // the slope of the slur is high and needs to be corrected
    if (fabs(slurAngle) > maxAngle) {
        int side = (p2.x - p1.x) * tan(maxAngle);
        if (p2.y > p1.y) {
            if (curveDir == curvature_CURVEDIR_above)
                p1.y = p2.y - side;
            else
                p2.y = p1.y + side;
            slurAngle = maxAngle;
        }
        else {
            if (curveDir == curvature_CURVEDIR_above)
                p2.y = p1.y - side;
            else
                p1.y = p2.y + side;
            slurAngle = -maxAngle;
        }
    }

    return slurAngle;
}

curvature_CURVEDIR Slur::GetGraceCurveDirection(Doc *doc)
{
    LayerElement *start = this->GetStart();
    LayerElement *end = this->GetEnd();
    const bool overlap = start->VerticalContentOverlap(end);
    const int topDiff = std::abs(start->GetContentTop() - end->GetContentTop());
    const int bottomDiff = std::abs(start->GetContentBottom() - end->GetContentBottom());
    if (overlap) {
        if (bottomDiff < 1.5 * topDiff) {
            return curvature_CURVEDIR_below;
        }
        else if ((bottomDiff < 3 * topDiff) && (NULL != end->IsInBeam())) {
            return curvature_CURVEDIR_below;
        }
        else {
            return curvature_CURVEDIR_above;
        }
    }
    else {
        StemmedDrawingInterface *endStemDrawInterface = end->GetStemmedDrawingInterface();
        data_STEMDIRECTION endStemDir = STEMDIRECTION_NONE;
        if (endStemDrawInterface) {
            endStemDir = endStemDrawInterface->GetDrawingStemDir();
        }

        if (end->GetContentBottom() > start->GetContentTop()) {
            if (endStemDir == STEMDIRECTION_up)
                return curvature_CURVEDIR_above;
            else {
                return (bottomDiff < topDiff) ? curvature_CURVEDIR_below : curvature_CURVEDIR_above;
            }
        }
        else if (end->GetContentTop() < start->GetContentBottom()) {
            if (endStemDir == STEMDIRECTION_down)
                return curvature_CURVEDIR_below;
            else {
                return (bottomDiff < topDiff) ? curvature_CURVEDIR_below : curvature_CURVEDIR_above;
            }
        }
    }
    return curvature_CURVEDIR_below;
}

curvature_CURVEDIR Slur::GetPreferredCurveDirection(Doc *doc, data_STEMDIRECTION noteStemDir, bool isAboveStaffCenter)
{
    const bool isGraceToNoteSlur = !this->GetStart()->Is(TIMESTAMP_ATTR) && !this->GetEnd()->Is(TIMESTAMP_ATTR)
        && this->GetStart()->IsGraceNote() && !this->GetEnd()->IsGraceNote();
    Note *startNote = NULL;
    Chord *startParentChord = NULL;
    if (this->GetStart()->Is(NOTE)) {
        startNote = vrv_cast<Note *>(this->GetStart());
        assert(startNote);
        startParentChord = startNote->IsChordTone();
    }

    Layer *layer = NULL;
    LayerElement *layerElement = NULL;
    std::tie(layer, layerElement) = this->GetBoundaryLayer();
    data_STEMDIRECTION layerStemDir = STEMDIRECTION_NONE;

    curvature_CURVEDIR drawingCurveDir = curvature_CURVEDIR_above;
    // first should be the slur @curvedir
    if (this->HasCurvedir()) {
        drawingCurveDir
            = (this->GetCurvedir() == curvature_CURVEDIR_above) ? curvature_CURVEDIR_above : curvature_CURVEDIR_below;
    }
    // grace notes - always below unless we have a drawing stem direction on the layer
    else if (isGraceToNoteSlur && layer && layerElement
        && (layer->GetDrawingStemDir(layerElement) == STEMDIRECTION_NONE)) {
        drawingCurveDir = this->GetGraceCurveDirection(doc);
    }
    // then layer direction trumps note direction
    else if (layer && layerElement && ((layerStemDir = layer->GetDrawingStemDir(layerElement)) != STEMDIRECTION_NONE)) {
        drawingCurveDir = (layerStemDir == STEMDIRECTION_up) ? curvature_CURVEDIR_above : curvature_CURVEDIR_below;
    }
    // look if in a chord
    else if (startParentChord) {
        if (startParentChord->PositionInChord(startNote) < 0) {
            drawingCurveDir = curvature_CURVEDIR_below;
        }
        else if (startParentChord->PositionInChord(startNote) > 0) {
            drawingCurveDir = curvature_CURVEDIR_above;
        }
        // away from the stem if odd number (center note)
        else {
            drawingCurveDir = (noteStemDir != STEMDIRECTION_up) ? curvature_CURVEDIR_above : curvature_CURVEDIR_below;
        }
    }
    else if (noteStemDir == STEMDIRECTION_up) {
        drawingCurveDir = curvature_CURVEDIR_below;
    }
    else if (noteStemDir == STEMDIRECTION_NONE) {
        // no information from the note stem directions, look at the position in the notes
        drawingCurveDir = (isAboveStaffCenter) ? curvature_CURVEDIR_above : curvature_CURVEDIR_below;
    }

    return drawingCurveDir;
}

std::pair<Point, Point> Slur::AdjustCoordinates(
    Doc *doc, Staff *staff, std::pair<Point, Point> points, char spanningType)
{
    StemmedDrawingInterface *startStemDrawInterface = this->GetStart()->GetStemmedDrawingInterface();
    StemmedDrawingInterface *endStemDrawInterface = this->GetEnd()->GetStemmedDrawingInterface();

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
    Note *startNote = NULL;
    LayerElement *start = this->GetStart();
    Chord *startChord = NULL;
    if (start->Is(NOTE)) {
        startNote = vrv_cast<Note *>(start);
        assert(startNote);
        startChord = startNote->IsChordTone();
    }
    else if (start->Is(CHORD)) {
        startChord = vrv_cast<Chord *>(start);
        assert(startChord);
    }

    // Pointers for the end point of the slur
    Note *endNote = NULL;
    LayerElement *end = this->GetEnd();
    Chord *endChord = NULL;
    if (end->Is(NOTE)) {
        endNote = vrv_cast<Note *>(end);
        assert(endNote);
        endChord = endNote->IsChordTone();
    }
    else if (end->Is(CHORD)) {
        endChord = vrv_cast<Chord *>(end);
        assert(endChord);
    }

    const bool isSshaped = this->HasMixedCurveDir();
    const bool isGraceToNoteSlur
        = !start->Is(TIMESTAMP_ATTR) && !end->Is(TIMESTAMP_ATTR) && start->IsGraceNote() && !end->IsGraceNote();

    const PortatoSlurType portatoSlurType = this->IsPortatoSlur(doc, startNote, startChord);

    int x1, x2, y1, y2;
    std::tie(x1, x2, y1, y2) = std::tie(points.first.x, points.second.x, points.first.y, points.second.y);

    bool isShortSlur = false;
    if (x2 - x1 < doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize)) isShortSlur = true;

    Beam *parentBeam = NULL;
    FTrem *parentFTrem = NULL;
    int yChordMax = 0, yChordMin = 0;
    const int unit = doc->GetDrawingUnit(staff->m_drawingStaffSize);
    if (((spanningType == SPANNING_START_END) || (spanningType == SPANNING_START)) && !start->Is(TIMESTAMP_ATTR)) {
        // get the radius for adjusting the x position
        const int startRadius = start->GetDrawingRadius(doc);
        // get the min max of the chord (if any)
        if (startChord) {
            startChord->GetYExtremes(yChordMax, yChordMin);
        }
        // slur is up
        if (this->HasEndpointAboveStart()) {
            // P(^)
            if (startStemDir == STEMDIRECTION_down || startStemLen == 0) {
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
                Note *refNote = startChord ? startChord->GetBottomNote() : startNote;
                x1 = refNote->GetDrawingX() + startRadius;
                if (portatoSlurType == PortatoSlurType::StemSide) {
                    x1 += startRadius;
                }
            }
            // same but in beam - adjust the x too
            else if (((parentBeam = start->IsInBeam()) && !parentBeam->IsLastIn(parentBeam, start))
                || ((parentFTrem = start->IsInFTrem()) && !parentFTrem->IsLastIn(parentFTrem, start))
                || isGraceToNoteSlur) {
                y1 = start->GetDrawingTop(doc, staff->m_drawingStaffSize);
                x1 += startRadius - doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
            }
            // d(^)
            else {
                // put it on the side, move it right
                if (startStemLen != 0) x1 += unit * 2;
                if (startChord)
                    y1 = yChordMax + unit * 3;
                else
                    y1 = start->GetDrawingY() + unit * 3;
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
            else if (startStemDir == STEMDIRECTION_up || startStemLen == 0) {
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
                Note *refNote = startChord ? startChord->GetTopNote() : startNote;
                x1 = refNote->GetDrawingX();
                if (portatoSlurType == PortatoSlurType::Centered) {
                    x1 += startRadius;
                }
            }
            // same but in beam
            else if (((parentBeam = start->IsInBeam()) && !parentBeam->IsLastIn(parentBeam, start))
                || ((parentFTrem = start->IsInFTrem()) && !parentFTrem->IsLastIn(parentFTrem, start))) {
                y1 = start->GetDrawingBottom(doc, staff->m_drawingStaffSize);
                x1 -= startRadius - doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
            }
            // P(_)
            else {
                // put it on the side, but no need to move it right
                if (startChord) {
                    y1 = yChordMin - unit * 3;
                }
                else {
                    y1 = start->GetDrawingY() - unit * 3;
                }
            }
        }
    }
    if (((spanningType == SPANNING_START_END) || (spanningType == SPANNING_END)) && !end->Is(TIMESTAMP_ATTR)) {
        // get the radius for adjusting the x position
        const int endRadius = end->GetDrawingRadius(doc);
        // get the min max of the chord if any
        if (endChord) {
            endChord->GetYExtremes(yChordMax, yChordMin);
        }
        // get the stem direction of the end
        // slur is up
        if (this->HasEndpointAboveEnd()) {
            // (^)P
            if (endStemDir == STEMDIRECTION_down || endStemLen == 0) {
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
                Note *refNote = endChord ? endChord->GetBottomNote() : endNote;
                x2 = refNote->GetDrawingX() + endRadius;
                if (portatoSlurType == PortatoSlurType::StemSide) {
                    x2 += endRadius;
                }
            }
            // same but in beam - adjust the x too
            else if (((parentBeam = end->IsInBeam()) && !parentBeam->IsFirstIn(parentBeam, end))
                || ((parentFTrem = end->IsInFTrem()) && !parentFTrem->IsFirstIn(parentFTrem, end))) {
                y2 = end->GetDrawingTop(doc, staff->m_drawingStaffSize);
                x2 += endRadius - doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
            }
            // (^)d
            else {
                // put it on the side, no need to move it left
                if (endChord) {
                    y2 = yChordMax + unit * 3;
                }
                else {
                    y2 = end->GetDrawingY() + unit * 3;
                }
            }
        }
        else {
            // (_)d
            if (endStemDir == STEMDIRECTION_up || endStemLen == 0) {
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
                Note *refNote = endChord ? endChord->GetTopNote() : endNote;
                x2 = refNote->GetDrawingX();
                if (portatoSlurType == PortatoSlurType::Centered) {
                    x2 += endRadius;
                }
            }
            // same but in beam
            else if (((parentBeam = end->IsInBeam()) && !parentBeam->IsFirstIn(parentBeam, end))
                || ((parentFTrem = end->IsInFTrem()) && !parentFTrem->IsFirstIn(parentFTrem, end))) {

                y2 = end->GetDrawingBottom(doc, staff->m_drawingStaffSize);
                x2 -= endRadius - doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
            }
            // (_)P
            else {
                // put it on the side, move it left
                if (endStemLen != 0) x2 -= unit * 2;
                if (endChord) {
                    y2 = yChordMin - unit * 3;
                }
                else {
                    y2 = end->GetDrawingY() - unit * 3;
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

    int brokenLoc = 0;
    int pitchDiff = 0;
    std::tie(brokenLoc, pitchDiff) = this->CalcBrokenLoc(staff, startLoc, endLoc);
    if (spanningType == SPANNING_START) {
        if (this->HasEndpointAboveStart()) {
            y2 = std::max(staffTop, y1);
            y2 += pitchDiff * unit / 2;
            y2 = std::max(staffTop, y2);
        }
        else {
            y2 = std::min(staffBottom, y1);
            y2 += pitchDiff * unit / 2;
            y2 = std::min(staffBottom, y2);
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
            y1 = std::max(staffTop, y2);
            y1 -= pitchDiff * unit / 2;
            y1 = std::max(staffTop, y1);
        }
        else {
            y1 = std::min(staffBottom, y2);
            y1 -= pitchDiff * unit / 2;
            y1 = std::min(staffBottom, y1);
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
        y1 = staffBottom + brokenLoc * unit;
        y2 = y1;
    }

    return std::make_pair(Point(x1, y1), Point(x2, y2));
}

std::pair<int, int> Slur::GetStartEndLocs(Note *startNote, Chord *startChord, Note *endNote, Chord *endChord) const
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

std::pair<int, int> Slur::CalcBrokenLoc(Staff *staff, int startLoc, int endLoc) const
{
    assert(staff);

    const int staffTopLoc = 2 * (staff->m_drawingLines - 1);
    const int loc1 = this->HasEndpointAboveStart() ? std::max(startLoc, staffTopLoc - 1) : std::min(startLoc, 1);
    const int loc2 = this->HasEndpointAboveEnd() ? std::max(endLoc, staffTopLoc - 1) : std::min(endLoc, 1);

    return { (loc1 + loc2) / 2, loc2 - loc1 };
}

PortatoSlurType Slur::IsPortatoSlur(Doc *doc, Note *startNote, Chord *startChord) const
{
    ListOfObjects artics;
    if (startChord) {
        artics = startChord->FindAllDescendantsByType(ARTIC, true, 1);
    }
    else if (startNote) {
        artics = startNote->FindAllDescendantsByType(ARTIC, true, 1);
    }

    PortatoSlurType type = PortatoSlurType::None;
    if (!artics.empty()) {
        type = PortatoSlurType::Centered;
        Artic *artic = vrv_cast<Artic *>(artics.front());
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

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Slur::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    ControlElement::ResetDrawing(functorParams);

    m_drawingCurveDir = SlurCurveDirection::None;
    // m_isCrossStaff = false;

    return FUNCTOR_CONTINUE;
}

int Slur::CalcSlurs(FunctorParams *functorParams)
{
    CalcSlursParams *params = vrv_params_cast<CalcSlursParams *>(functorParams);
    assert(params);

    // If curve direction is prescribed as above or below, use it
    if (this->HasCurvedir() && (this->GetCurvedir() != curvature_CURVEDIR_mixed)) {
        this->SetDrawingCurveDir(
            (this->GetCurvedir() == curvature_CURVEDIR_above) ? SlurCurveDirection::Above : SlurCurveDirection::Below);
    }
    if (this->HasDrawingCurveDir()) return FUNCTOR_CONTINUE;

    // Retrieve boundary
    LayerElement *start = this->GetStart();
    LayerElement *end = this->GetEnd();
    if (!start || !end) {
        this->SetDrawingCurveDir(SlurCurveDirection::Above);
        return FUNCTOR_CONTINUE;
    }

    // If curve direction is prescribed as mixed, use it if boundary lies in different staves
    if (this->GetCurvedir() == curvature_CURVEDIR_mixed) {
        const int startStaffN = start->GetAncestorStaff(RESOLVE_CROSS_STAFF)->GetN();
        const int endStaffN = end->GetAncestorStaff(RESOLVE_CROSS_STAFF)->GetN();
        if (startStaffN < endStaffN) {
            this->SetDrawingCurveDir(SlurCurveDirection::BelowAbove);
            return FUNCTOR_CONTINUE;
        }
        else if (startStaffN > endStaffN) {
            this->SetDrawingCurveDir(SlurCurveDirection::AboveBelow);
            return FUNCTOR_CONTINUE;
        }
        else {
            LogWarning("Mixed curve direction is ignored for slurs starting and ending on the same staff.");
        }
    }

    // Retrieve staves and system
    std::vector<Staff *> staffList = this->GetTstampStaves(this->GetStartMeasure(), this);
    if (staffList.empty()) {
        this->SetDrawingCurveDir(SlurCurveDirection::Above);
        return FUNCTOR_CONTINUE;
    }
    Staff *staff = staffList.at(0);
    Staff *crossStaff = this->GetBoundaryCrossStaff();

    System *system = vrv_cast<System *>(staff->GetFirstAncestor(SYSTEM));
    assert(system);

    if (!start->Is(TIMESTAMP_ATTR) && !end->Is(TIMESTAMP_ATTR) && system->HasMixedDrawingStemDir(start, end)) {
        // Handle mixed stem direction
        if (crossStaff && (system->GetPreferredCurveDirection(start, end, this) == curvature_CURVEDIR_below)) {
            this->SetDrawingCurveDir(SlurCurveDirection::Below);
        }
        else {
            this->SetDrawingCurveDir(SlurCurveDirection::Above);
        }
    }
    else {
        // Handle uniform stem direction and time stamp boundaries
        StemmedDrawingInterface *startStemDrawInterface = start->GetStemmedDrawingInterface();
        data_STEMDIRECTION startStemDir = STEMDIRECTION_NONE;
        if (startStemDrawInterface) {
            startStemDir = startStemDrawInterface->GetDrawingStemDir();
        }

        const int center = staff->GetDrawingY() - params->m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize) / 2;
        const bool isAboveStaffCenter = (start->GetDrawingY() > center);
        if (this->GetPreferredCurveDirection(params->m_doc, startStemDir, isAboveStaffCenter)
            == curvature_CURVEDIR_below) {
            this->SetDrawingCurveDir(SlurCurveDirection::Below);
        }
        else {
            this->SetDrawingCurveDir(SlurCurveDirection::Above);
        }
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
