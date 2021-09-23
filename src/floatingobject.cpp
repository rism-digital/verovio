/////////////////////////////////////////////////////////////////////////////
// Name:        floatingobject.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "floatingobject.h"

//----------------------------------------------------------------------------

#include <algorithm>
#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "bracketspan.h"
#include "breath.h"
#include "dir.h"
#include "doc.h"
#include "dynam.h"
#include "fermata.h"
#include "fing.h"
#include "hairpin.h"
#include "harm.h"
#include "mordent.h"
#include "octave.h"
#include "pedal.h"
#include "pitchinflection.h"
#include "reh.h"
#include "slur.h"
#include "staff.h"
#include "tempo.h"
#include "timeinterface.h"
#include "trill.h"
#include "turn.h"
#include "verticalaligner.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Static members
//----------------------------------------------------------------------------

thread_local std::vector<void *> FloatingObject::s_drawingObjectIds;

//----------------------------------------------------------------------------
// FloatingObject
//----------------------------------------------------------------------------

FloatingObject::FloatingObject() : Object(FLOATING_OBJECT, "fe")
{
    Reset();
}

FloatingObject::FloatingObject(ClassId classId) : Object(classId, "fe")
{
    Reset();
}

FloatingObject::FloatingObject(ClassId classId, const std::string &classIdStr) : Object(classId, classIdStr)
{
    Reset();

    m_currentPositioner = NULL;
    m_maxDrawingYRel = VRV_UNSET;
}

FloatingObject::~FloatingObject() {}

void FloatingObject::Reset()
{
    Object::Reset();

    m_drawingGrpId = 0;
}

void FloatingObject::UpdateContentBBoxX(int x1, int x2)
{
    if (!m_currentPositioner) return;
    m_currentPositioner->BoundingBox::UpdateContentBBoxX(x1, x2);
}

void FloatingObject::UpdateContentBBoxY(int y1, int y2)
{
    if (!m_currentPositioner) return;
    m_currentPositioner->BoundingBox::UpdateContentBBoxY(y1, y2);
}

void FloatingObject::UpdateSelfBBoxX(int x1, int x2)
{
    if (!m_currentPositioner) return;
    m_currentPositioner->BoundingBox::UpdateSelfBBoxX(x1, x2);
}

void FloatingObject::UpdateSelfBBoxY(int y1, int y2)
{
    if (!m_currentPositioner) return;
    m_currentPositioner->BoundingBox::UpdateSelfBBoxY(y1, y2);
}

int FloatingObject::GetDrawingX() const
{
    if (!m_currentPositioner) return 0;
    return m_currentPositioner->GetDrawingX();
}

int FloatingObject::GetDrawingY() const
{
    if (!m_currentPositioner) return 0;
    return m_currentPositioner->GetDrawingY();
}

void FloatingObject::SetMaxDrawingYRel(int maxDrawingYRel)
{
    if (!m_currentPositioner) return;
    data_STAFFREL drawingPlace = m_currentPositioner->GetDrawingPlace();
    if (drawingPlace == STAFFREL_above) {
        if ((m_maxDrawingYRel == VRV_UNSET) || (m_maxDrawingYRel > maxDrawingYRel)) m_maxDrawingYRel = maxDrawingYRel;
    }
    else {
        if ((m_maxDrawingYRel == VRV_UNSET) || (m_maxDrawingYRel < maxDrawingYRel)) m_maxDrawingYRel = maxDrawingYRel;
    }
}

void FloatingObject::SetCurrentFloatingPositioner(FloatingPositioner *boundingBox)
{
    m_currentPositioner = boundingBox;
}

FloatingPositioner *FloatingObject::GetCorrespFloatingPositioner(FloatingObject *object)
{
    if (!object || !m_currentPositioner) return NULL;

    return m_currentPositioner->GetAlignment()->GetCorrespFloatingPositioner(object);
}

int FloatingObject::SetDrawingGrpObject(void *drawingGrpObject)
{
    assert(drawingGrpObject);

    int idx = 0;
    std::vector<void *>::const_iterator it
        = std::find(s_drawingObjectIds.begin(), s_drawingObjectIds.end(), drawingGrpObject);
    if (it == s_drawingObjectIds.end()) {
        idx = (int)s_drawingObjectIds.size();
        s_drawingObjectIds.push_back(drawingGrpObject);
        // LogDebug("Creating grpId %d", idx);
    }
    else {
        idx = (int)(it - s_drawingObjectIds.begin());
        // LogDebug("Using grpId %d", idx);
    }
    m_drawingGrpId = idx + 1000;
    return m_drawingGrpId;
}

//----------------------------------------------------------------------------
// FloatingPositioner
//----------------------------------------------------------------------------

FloatingPositioner::FloatingPositioner(FloatingObject *object, StaffAlignment *alignment, char spanningType)
    : BoundingBox()
{
    assert(object);
    assert(alignment);

    m_object = object;
    m_alignment = alignment;
    m_spanningType = spanningType;

    if (object->Is(BRACKETSPAN)) {
        m_place = STAFFREL_above;
    }
    else if (object->Is(BREATH)) {
        Breath *breath = vrv_cast<Breath *>(object);
        assert(breath);
        // breath above by default
        m_place = (breath->GetPlace() != STAFFREL_NONE) ? breath->GetPlace() : STAFFREL_above;
    }
    else if (object->Is(DIR)) {
        Dir *dir = vrv_cast<Dir *>(object);
        assert(dir);
        // dir below by default
        m_place = (dir->GetPlace() != STAFFREL_NONE) ? dir->GetPlace() : STAFFREL_below;
    }
    else if (object->Is(DYNAM)) {
        Dynam *dynam = vrv_cast<Dynam *>(object);
        assert(dynam);
        // dynam below by default
        m_place = (dynam->GetPlace() != STAFFREL_NONE) ? dynam->GetPlace() : STAFFREL_below;
    }
    else if (object->Is(ENDING)) {
        // endings always above;
        m_place = STAFFREL_above;
    }
    else if (object->Is(FERMATA)) {
        Fermata *fermata = vrv_cast<Fermata *>(object);
        assert(fermata);
        // fermata above by default
        m_place = (fermata->GetPlace() != STAFFREL_NONE) ? fermata->GetPlace() : STAFFREL_above;
    }
    else if (object->Is(FING)) {
        Fing *fing = vrv_cast<Fing *>(object);
        assert(fing);
        // fing above by default
        m_place = (fing->GetPlace() != STAFFREL_NONE) ? fing->GetPlace() : STAFFREL_above;
    }
    else if (object->Is(HAIRPIN)) {
        Hairpin *hairpin = vrv_cast<Hairpin *>(object);
        assert(hairpin);
        // haripin below by default;
        m_place = (hairpin->GetPlace() != STAFFREL_NONE) ? hairpin->GetPlace() : STAFFREL_below;
    }
    else if (object->Is(HARM)) {
        Harm *harm = vrv_cast<Harm *>(object);
        assert(harm);
        // harm above by default
        m_place = (harm->GetPlace() != STAFFREL_NONE) ? harm->GetPlace() : STAFFREL_above;
        if ((harm->GetPlace() == STAFFREL_NONE) && object->GetFirst()->Is(FB)) m_place = STAFFREL_below;
    }
    else if (object->Is(MORDENT)) {
        Mordent *mordent = vrv_cast<Mordent *>(object);
        assert(mordent);
        // mordent above by default;
        m_place = (mordent->GetPlace() != STAFFREL_NONE) ? mordent->GetPlace() : mordent->GetLayerPlace(STAFFREL_above);
    }
    else if (object->Is(OCTAVE)) {
        Octave *octave = vrv_cast<Octave *>(object);
        assert(octave);
        // octave below by default (won't draw without @dis.place anyway);
        m_place = (octave->GetDisPlace() == STAFFREL_basic_above) ? STAFFREL_above : STAFFREL_below;
    }
    else if (object->Is(PITCHINFLECTION)) {
        // PitchInflection *pitchInflection = vrv_cast<PitchInflection *>(object);
        // assert(pitchInflection);
        // pitchInflection above by default;
        // m_place = (pitchInflection->GetPlace() != STAFFREL_NONE) ? pitchInflection->GetPlace() : STAFFREL_above;
        m_place = STAFFREL_above;
    }
    else if (object->Is(PEDAL)) {
        Pedal *pedal = vrv_cast<Pedal *>(object);
        assert(pedal);
        // pedal below by default
        m_place = (pedal->GetPlace() != STAFFREL_NONE) ? pedal->GetPlace() : STAFFREL_below;
    }
    else if (object->Is(REH)) {
        Reh *reh = vrv_cast<Reh *>(object);
        assert(reh);
        // reh above by default
        m_place = (reh->GetPlace() != STAFFREL_NONE) ? reh->GetPlace() : STAFFREL_above;
    }
    else if (object->Is(TEMPO)) {
        Tempo *tempo = vrv_cast<Tempo *>(object);
        assert(tempo);
        // tempo above by default;
        m_place = (tempo->GetPlace() != STAFFREL_NONE) ? tempo->GetPlace() : STAFFREL_above;
    }
    else if (object->Is(TRILL)) {
        Trill *trill = vrv_cast<Trill *>(object);
        assert(trill);
        trill->GetStart();
        // trill above by default;
        m_place = (trill->GetPlace() != STAFFREL_NONE) ? trill->GetPlace() : trill->GetLayerPlace(STAFFREL_above);
    }
    else if (object->Is(TURN)) {
        Turn *turn = vrv_cast<Turn *>(object);
        assert(turn);
        // turn above by default;
        m_place = (turn->GetPlace() != STAFFREL_NONE) ? turn->GetPlace() : turn->GetLayerPlace(STAFFREL_above);
    }
    else {
        m_place = STAFFREL_NONE;
    }
    ResetPositioner();
}

void FloatingPositioner::ResetPositioner()
{
    BoundingBox::ResetBoundingBox();
    ResetCachedDrawingY();

    m_objectX = NULL;
    m_objectY = NULL;

    m_drawingYRel = 0;
    m_drawingXRel = 0;
}

int FloatingPositioner::GetDrawingX() const
{
    assert(m_objectX);
    return m_objectX->GetDrawingX() + this->GetDrawingXRel();
}

int FloatingPositioner::GetDrawingY() const
{
    assert(m_objectY);
    return (m_objectY->GetDrawingY() - this->GetDrawingYRel());
}

void FloatingPositioner::ResetCachedDrawingX() const
{
    m_cachedDrawingX = VRV_UNSET;
}

void FloatingPositioner::ResetCachedDrawingY() const
{
    m_cachedDrawingY = VRV_UNSET;
}

void FloatingPositioner::SetObjectXY(Object *objectX, Object *objectY)
{
    assert(objectX);
    assert(objectY);

    m_objectX = objectX;
    m_objectY = objectY;
}

void FloatingPositioner::SetDrawingXRel(int drawingXRel)
{
    ResetCachedDrawingX();
    m_drawingXRel = drawingXRel;
}

void FloatingPositioner::SetDrawingYRel(int drawingYRel, bool force)
{
    bool setValue = force;
    if (m_place == STAFFREL_above) {
        if (drawingYRel < m_drawingYRel) setValue = true;
    }
    else {
        if (drawingYRel > m_drawingYRel) setValue = true;
    }

    if (setValue) {
        ResetCachedDrawingY();
        m_drawingYRel = drawingYRel;
    }
}

bool FloatingPositioner::CalcDrawingYRel(Doc *doc, StaffAlignment *staffAlignment, BoundingBox *horizOverlapingBBox)
{
    assert(doc);
    assert(staffAlignment);

    int staffSize = staffAlignment->GetStaffSize();
    int yRel;

    if (horizOverlapingBBox == NULL) {
        // Apply element margin and enforce minimal staff distance
        int staffIndex = staffAlignment->GetStaff()->GetN();
        int minStaffDistance
            = doc->GetStaffDistance(m_object->GetClassId(), staffIndex, m_place) * doc->GetDrawingUnit(staffSize);
        if (m_place == STAFFREL_above) {
            yRel = this->GetContentY1();
            yRel -= doc->GetBottomMargin(m_object->GetClassId()) * doc->GetDrawingUnit(staffSize);
            this->SetDrawingYRel(yRel);
            this->SetDrawingYRel(-minStaffDistance);
        }
        else {
            yRel = staffAlignment->GetStaffHeight() + this->GetContentY2();
            yRel += doc->GetTopMargin(m_object->GetClassId()) * doc->GetDrawingUnit(staffSize);
            this->SetDrawingYRel(yRel);
            this->SetDrawingYRel(minStaffDistance + staffAlignment->GetStaffHeight());
        }
    }
    else {
        FloatingCurvePositioner *curve = dynamic_cast<FloatingCurvePositioner *>(horizOverlapingBBox);
        if (curve) {
            assert(curve->m_object);
        }
        int margin = doc->GetBottomMargin(m_object->GetClassId()) * doc->GetDrawingUnit(staffSize);

        if (m_place == STAFFREL_above) {
            if (curve && curve->m_object->Is({ LV, PHRASE, SLUR, TIE })) {
                const int shift = this->Intersects(curve, CONTENT, doc->GetDrawingUnit(staffSize));
                if (shift != 0) {
                    this->SetDrawingYRel(this->GetDrawingYRel() - shift);
                }
                return true;
            }
            else if (m_object->Is(DYNAM) && horizOverlapingBBox->Is(BEAM)) {
                // Try to avoid comparing with BEAM BB since it might be much larger overlap while having a lot of
                // whitespace. For such cases, DYNAM should be compared to individual elements of BEAM instead
                return true;
            }
            yRel = -staffAlignment->CalcOverflowAbove(horizOverlapingBBox) + this->GetContentY1() - margin;

            Object *object = dynamic_cast<Object *>(horizOverlapingBBox);
            // For elements, that can have extender lines, we need to make sure that they continue in next system on the
            // same height, as they were before (even if there are no overlapping elements in subsequent measures)
            if (m_object->Is({ DIR, DYNAM }) && m_object->IsExtenderElement()) {
                m_object->SetMaxDrawingYRel(yRel);
                this->SetDrawingYRel(std::min(yRel, m_object->GetMaxDrawingYRel()));
            }
            // With LayerElement always move them up
            else if (object && object->IsLayerElement()) {
                if (yRel < 0) this->SetDrawingYRel(yRel);
            }
            // Otherwise only if there is a vertical overlap
            else if (this->VerticalContentOverlap(horizOverlapingBBox, margin)) {
                this->SetDrawingYRel(yRel);
            }
        }
        else {
            if (curve && curve->m_object->Is({ LV, PHRASE, SLUR, TIE })) {
                const int shift = this->Intersects(curve, CONTENT, doc->GetDrawingUnit(staffSize));
                if (shift != 0) {
                    this->SetDrawingYRel(this->GetDrawingYRel() - shift);
                }
                return true;
            }
            yRel = staffAlignment->CalcOverflowBelow(horizOverlapingBBox) + staffAlignment->GetStaffHeight()
                + this->GetContentY2() + margin;

            Object *object = dynamic_cast<Object *>(horizOverlapingBBox);
            // For elements, that can have extender lines, we need to make sure that they continue in next system on the
            // same height, as they were before (even if there are no overlapping elements in subsequent measures)
            if (m_object->Is({ DIR, DYNAM }) && m_object->IsExtenderElement()) {
                m_object->SetMaxDrawingYRel(yRel);
                this->SetDrawingYRel(std::max(yRel, m_object->GetMaxDrawingYRel()));
            }
            // With LayerElement always move them down
            else if (object && object->IsLayerElement()) {
                if (yRel > 0) this->SetDrawingYRel(yRel);
            }
            // Otherwise only if there is a vertical overlap
            else if (this->VerticalContentOverlap(horizOverlapingBBox, margin)) {
                this->SetDrawingYRel(yRel);
            }
        }
    }
    return true;
}

int FloatingPositioner::GetSpaceBelow(Doc *doc, StaffAlignment *staffAlignment, BoundingBox *horizOverlapingBBox)
{
    if (m_place != STAFFREL_between) return VRV_UNSET;

    int staffSize = staffAlignment->GetStaffSize();

    FloatingCurvePositioner *curve = dynamic_cast<FloatingCurvePositioner *>(horizOverlapingBBox);
    if (curve) {
        assert(curve->m_object);
    }
    int margin = doc->GetBottomMargin(m_object->GetClassId()) * doc->GetDrawingUnit(staffSize);

    if (curve && curve->m_object->Is({ LV, PHRASE, SLUR, TIE })) {
        // For now ignore curves
        return 0;
    }

    return this->GetContentBottom() - horizOverlapingBBox->GetSelfTop() - margin;
}

//----------------------------------------------------------------------------
// FloatingCurvePositioner
//----------------------------------------------------------------------------

FloatingCurvePositioner::FloatingCurvePositioner(FloatingObject *object, StaffAlignment *alignment, char spanningType)
    : FloatingPositioner(object, alignment, spanningType)
{
    ResetCurveParams();
}

FloatingCurvePositioner::~FloatingCurvePositioner()
{
    ClearSpannedElements();
}

void FloatingCurvePositioner::ResetPositioner()
{
    FloatingPositioner::ResetPositioner();

    ResetCurveParams();
}

void FloatingCurvePositioner::ClearSpannedElements()
{
    for (auto &spannedElement : m_spannedElements) {
        delete spannedElement;
    }
    m_spannedElements.clear();
}

void FloatingCurvePositioner::ResetCurveParams()
{
    m_points[0] = Point(0, 0);
    m_points[1] = Point(0, 0);
    m_points[2] = Point(0, 0);
    m_points[3] = Point(0, 0);
    m_angle = 0.0;
    m_thickness = 0;
    m_dir = curvature_CURVEDIR_NONE;
    m_crossStaff = NULL;
    m_cachedMinMaxY = VRV_UNSET;
    ClearSpannedElements();
}

void FloatingCurvePositioner::UpdateCurveParams(
    const Point points[4], float angle, int thickness, curvature_CURVEDIR curveDir)
{
    m_points[0] = points[0];
    m_points[1] = points[1];
    m_points[2] = points[2];
    m_points[3] = points[3];
    int currentY = this->GetDrawingY();
    m_points[0].y -= currentY;
    m_points[1].y -= currentY;
    m_points[2].y -= currentY;
    m_points[3].y -= currentY;
    m_angle = angle;
    m_thickness = thickness;
    m_dir = curveDir;
    m_cachedMinMaxY = VRV_UNSET;
}

void FloatingCurvePositioner::UpdatePoints(const BezierCurve &bezier)
{
    Point points[4];
    points[0] = bezier.p1;
    points[1] = bezier.c1;
    points[2] = bezier.c2;
    points[3] = bezier.p2;
    this->UpdateCurveParams(points, m_angle, m_thickness, m_dir);
}

void FloatingCurvePositioner::MoveFrontVertical(int distance)
{
    m_points[0].y += distance;
    m_points[1].y += distance;
}

void FloatingCurvePositioner::MoveBackVertical(int distance)
{
    m_points[2].y += distance;
    m_points[3].y += distance;
}

int FloatingCurvePositioner::CalcMinMaxY(const Point points[4])
{
    assert(this->GetObject());
    assert(this->GetObject()->Is({ LV, PHRASE, SLUR, TIE }));
    assert(m_dir != curvature_CURVEDIR_NONE);

    if (m_cachedMinMaxY != VRV_UNSET) return m_cachedMinMaxY;
    Point pos;
    int width, height;
    int minYPos, maxYPos;

    BoundingBox::ApproximateBezierBoundingBox(points, pos, width, height, minYPos, maxYPos);
    m_cachedMinMaxY = (m_dir == curvature_CURVEDIR_above) ? maxYPos : minYPos;

    return m_cachedMinMaxY;
}

int FloatingCurvePositioner::CalcAdjustment(BoundingBox *boundingBox, bool &discard, int margin, bool horizontalOverlap)
{
    int leftAdjustment, rightAdjustment;
    std::tie(leftAdjustment, rightAdjustment)
        = CalcLeftRightAdjustment(boundingBox, discard, margin, horizontalOverlap);
    return std::max(leftAdjustment, rightAdjustment);
}

std::pair<int, int> FloatingCurvePositioner::CalcLeftRightAdjustment(
    BoundingBox *boundingBox, bool &discard, int margin, bool horizontalOverlap)
{
    assert(boundingBox);
    assert(boundingBox->HasSelfBB());

    Point points[4];
    // We need to get the points because then stored points are relative
    this->GetPoints(points);

    // for lisability
    Point p1 = points[0];
    Point p2 = points[3];

    Accessor type = SELF;
    // bool keepInside = element->Is({ARTIC, ARTIC_PART, NOTE, STEM}));
    // The idea is to force only some of the elements to be inside a slur.
    // However, this currently does work because skipping an adjustment can cause collision later depending on how
    // the slur is eventually adjusted. Keeping everything inside now.
    bool keepInside = true;
    discard = false;

    // first check if they overlap at all
    if (horizontalOverlap) {
        if (p2.x < boundingBox->GetLeftBy(type) + margin) return { 0, 0 };
        if (p1.x > boundingBox->GetRightBy(type) + margin) return { 0, 0 };
    }

    Point topBezier[4], bottomBezier[4];
    BoundingBox::CalcThickBezier(points, this->GetThickness(), this->GetAngle(), topBezier, bottomBezier);

    // Now calculate the left and right adjustments
    int leftAdjustment = 0;
    int rightAdjustment = 0;

    if (this->GetDir() == curvature_CURVEDIR_above) {
        // The curve is below the content - if the element needs to be kept inside (e.g. a note), then do not return.
        if (((this->GetTopBy(type) + margin) < boundingBox->GetBottomBy(type)) && !keepInside) {
            return { 0, 0 };
        }
        int leftY = 0;
        int rightY = 0;
        // The curve overflows on both sides
        if ((p1.x < boundingBox->GetLeftBy(type)) && p2.x > boundingBox->GetRightBy(type)) {
            // calculate the y positions
            leftY = BoundingBox::CalcBezierAtPosition(bottomBezier, boundingBox->GetLeftBy(type)) - margin;
            rightY = BoundingBox::CalcBezierAtPosition(bottomBezier, boundingBox->GetRightBy(type)) - margin;
        }
        // The curve overflows on the left
        else if ((p1.x < boundingBox->GetLeftBy(type)) && p2.x <= boundingBox->GetRightBy(type)) {
            leftY = BoundingBox::CalcBezierAtPosition(bottomBezier, boundingBox->GetLeftBy(type)) - margin;
            rightY = p2.y - margin;
        }
        // The curve overflows on the right
        else if ((p1.x >= boundingBox->GetLeftBy(type)) && p2.x > boundingBox->GetRightBy(type)) {
            leftY = p1.y - margin;
            rightY = BoundingBox::CalcBezierAtPosition(bottomBezier, boundingBox->GetRightBy(type)) - margin;
        }
        // The curve is inside the left and right side of the content
        else {
            leftY = p1.y - margin;
            rightY = p2.y - margin;
        }

        leftAdjustment = std::max(boundingBox->GetTopBy(type) - leftY, 0);
        rightAdjustment = std::max(boundingBox->GetTopBy(type) - rightY, 0);
    }
    else {
        // The curve is below the content - if the element needs to be kept inside (e.g. a note), then do not return.
        if (((this->GetTopBy(type) + margin) < boundingBox->GetBottomBy(type)) && !keepInside) {
            return { 0, 0 };
        }
        int leftY = 0;
        int rightY = 0;
        // The curve overflows on both sides
        if ((p1.x < boundingBox->GetLeftBy(type)) && p2.x > boundingBox->GetRightBy(type)) {
            // calculate the y positions
            leftY = BoundingBox::CalcBezierAtPosition(topBezier, boundingBox->GetLeftBy(type)) + margin;
            rightY = BoundingBox::CalcBezierAtPosition(topBezier, boundingBox->GetRightBy(type)) + margin;
        }
        // The curve overflows on the left
        else if ((p1.x < boundingBox->GetLeftBy(type)) && p2.x <= boundingBox->GetRightBy(type)) {
            leftY = BoundingBox::CalcBezierAtPosition(topBezier, boundingBox->GetLeftBy(type)) + margin;
            rightY = p2.y + margin;
        }
        // The curve overflows on the right
        else if ((p1.x >= boundingBox->GetLeftBy(type)) && p2.x > boundingBox->GetRightBy(type)) {
            leftY = p1.y + margin;
            rightY = BoundingBox::CalcBezierAtPosition(topBezier, boundingBox->GetRightBy(type)) + margin;
        }
        // The curve is inside the left and right side of the content
        else {
            leftY = p1.y + margin;
            rightY = p2.y + margin;
        }

        leftAdjustment = std::max(leftY - boundingBox->GetBottomBy(type), 0);
        rightAdjustment = std::max(rightY - boundingBox->GetBottomBy(type), 0);
    }

    if ((leftAdjustment == 0) && (rightAdjustment == 0)) {
        // Everything is above or below - we can discard the element
        discard = true;
    }

    return { leftAdjustment, rightAdjustment };
}

void FloatingCurvePositioner::GetPoints(Point points[4]) const
{
    points[0] = m_points[0];
    points[1] = m_points[1];
    points[2] = m_points[2];
    points[3] = m_points[3];
    int currentY = this->GetDrawingY();
    points[0].y += currentY;
    points[1].y += currentY;
    points[2].y += currentY;
    points[3].y += currentY;
}

//----------------------------------------------------------------------------
// FloatingObject functor methods
//----------------------------------------------------------------------------

int FloatingObject::ResetHorizontalAlignment(FunctorParams *functorParams)
{
    m_currentPositioner = NULL;

    return FUNCTOR_CONTINUE;
}

int FloatingObject::ResetVerticalAlignment(FunctorParams *functorParams)
{
    m_currentPositioner = NULL;

    return FUNCTOR_CONTINUE;
}

int FloatingObject::PrepareTimePointing(FunctorParams *functorParams)
{
    // Pass it to the pseudo functor of the interface
    if (this->HasInterface(INTERFACE_TIME_POINT)) {
        TimePointInterface *interface = this->GetTimePointInterface();
        assert(interface);
        return interface->InterfacePrepareTimePointing(functorParams, this);
    }
    return FUNCTOR_CONTINUE;
}

int FloatingObject::PrepareTimeSpanning(FunctorParams *functorParams)
{
    // Pass it to the pseudo functor of the interface
    if (this->HasInterface(INTERFACE_TIME_SPANNING)) {
        TimeSpanningInterface *interface = this->GetTimeSpanningInterface();
        assert(interface);
        return interface->InterfacePrepareTimeSpanning(functorParams, this);
    }
    return FUNCTOR_CONTINUE;
}

int FloatingObject::PrepareTimestamps(FunctorParams *functorParams)
{
    // Pass it to the pseudo functor of the interface
    if (this->HasInterface(INTERFACE_TIME_POINT)) {
        TimePointInterface *interface = this->GetTimePointInterface();
        assert(interface);
        return interface->InterfacePrepareTimestamps(functorParams, this);
    }
    else if (this->HasInterface(INTERFACE_TIME_SPANNING)) {
        TimeSpanningInterface *interface = this->GetTimeSpanningInterface();
        assert(interface);
        return interface->InterfacePrepareTimestamps(functorParams, this);
    }
    return FUNCTOR_CONTINUE;
}

int FloatingObject::FillStaffCurrentTimeSpanning(FunctorParams *functorParams)
{
    // Pass it to the pseudo functor of the interface
    if (this->HasInterface(INTERFACE_TIME_SPANNING)) {
        TimeSpanningInterface *interface = this->GetTimeSpanningInterface();
        assert(interface);
        interface->InterfaceFillStaffCurrentTimeSpanning(functorParams, this);
    }
    if (this->HasInterface(INTERFACE_LINKING)) {
        LinkingInterface *interface = this->GetLinkingInterface();
        assert(interface);
        interface->InterfaceFillStaffCurrentTimeSpanning(functorParams, this);
    }
    return FUNCTOR_CONTINUE;
}

int FloatingObject::ResetDrawing(FunctorParams *functorParams)
{
    // Clear all
    FloatingObject::s_drawingObjectIds.clear();

    m_currentPositioner = NULL;
    // Pass it to the pseudo functor of the interface
    if (this->HasInterface(INTERFACE_TIME_SPANNING)) {
        TimeSpanningInterface *interface = this->GetTimeSpanningInterface();
        assert(interface);
        return interface->InterfaceResetDrawing(functorParams, this);
    }
    else if (this->HasInterface(INTERFACE_TIME_POINT)) {
        TimePointInterface *interface = this->GetTimePointInterface();
        assert(interface);
        return interface->InterfaceResetDrawing(functorParams, this);
    }
    m_drawingGrpId = 0;
    return FUNCTOR_CONTINUE;
}

int FloatingObject::UnCastOff(FunctorParams *functorParams)
{
    m_currentPositioner = NULL;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
