/////////////////////////////////////////////////////////////////////////////
// Name:        floatingobject.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "floatingobject.h"

//----------------------------------------------------------------------------

#include <algorithm>
#include <cassert>
#include <math.h>

//----------------------------------------------------------------------------

#include "beam.h"
#include "bracketspan.h"
#include "breath.h"
#include "caesura.h"
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
    this->Reset();
}

FloatingObject::FloatingObject(ClassId classId) : Object(classId, "fe")
{
    this->Reset();
}

FloatingObject::FloatingObject(ClassId classId, const std::string &classIdStr) : Object(classId, classIdStr)
{
    this->Reset();

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

FloatingPositioner *FloatingObject::GetCorrespFloatingPositioner(const FloatingObject *object)
{
    return const_cast<FloatingPositioner *>(std::as_const(*this).GetCorrespFloatingPositioner(object));
}

const FloatingPositioner *FloatingObject::GetCorrespFloatingPositioner(const FloatingObject *object) const
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
    else if (object->Is(CAESURA)) {
        Caesura *caesura = vrv_cast<Caesura *>(object);
        assert(caesura);
        // caesura within by default
        m_place = (caesura->GetPlace() != STAFFREL_NONE) ? caesura->GetPlace() : STAFFREL_within;
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
        // fb below by default
        if (harm->GetPlace() == STAFFREL_NONE) {
            Object *firstChild = harm->GetFirst();
            if (firstChild && firstChild->Is(FB)) m_place = STAFFREL_below;
        }
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
    this->ResetPositioner();
}

void FloatingPositioner::ResetPositioner()
{
    BoundingBox::ResetBoundingBox();
    this->ResetCachedDrawingX();
    this->ResetCachedDrawingY();

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
    this->ResetCachedDrawingX();
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
        this->ResetCachedDrawingY();
        m_drawingYRel = drawingYRel;
    }
}

bool FloatingPositioner::CalcDrawingYRel(
    Doc *doc, const StaffAlignment *staffAlignment, const BoundingBox *horizOverlapingBBox)
{
    assert(doc);
    assert(staffAlignment);
    assert(m_object);

    int staffSize = staffAlignment->GetStaffSize();
    int yRel;

    const int unit = doc->GetDrawingUnit(staffSize);
    if (horizOverlapingBBox == NULL) {
        // Apply element margin and enforce minimal staff distance
        int staffIndex = staffAlignment->GetStaff()->GetN();
        int minStaffDistance
            = doc->GetStaffDistance(m_object->GetClassId(), staffIndex, m_place) * doc->GetDrawingUnit(staffSize);
        if (this->GetObject()->Is(FERMATA) && (staffAlignment->GetStaff()->m_drawingLines == 1)) {
            minStaffDistance = 2.5 * doc->GetDrawingUnit(staffSize);
        }
        if (m_place == STAFFREL_above) {
            yRel = this->GetContentY1();
            yRel -= doc->GetBottomMargin(m_object->GetClassId()) * unit;
            this->SetDrawingYRel(yRel);
            this->SetDrawingYRel(-minStaffDistance);
        }
        else if (m_place == STAFFREL_within) {
            yRel = staffAlignment->GetStaffHeight() / 2;
            if (m_object->Is(TURN)) {
                Turn *turn = vrv_cast<Turn *>(m_object);
                assert(turn);
                yRel += turn->GetTurnHeight(doc, staffSize) / 2;
            }
            else if (!m_object->Is({ DIR, HAIRPIN })) {
                yRel += (this->GetContentY2() - this->GetContentY1()) / 2;
            }
            this->SetDrawingYRel(yRel);
        }
        else {
            yRel = staffAlignment->GetStaffHeight() + this->GetContentY2();
            yRel += doc->GetTopMargin(m_object->GetClassId()) * unit;
            this->SetDrawingYRel(yRel);
            this->SetDrawingYRel(minStaffDistance + staffAlignment->GetStaffHeight());
        }
    }
    else {
        const FloatingCurvePositioner *curve = dynamic_cast<const FloatingCurvePositioner *>(horizOverlapingBBox);
        if (curve) {
            assert(curve->m_object);
        }
        int margin = doc->GetBottomMargin(m_object->GetClassId()) * unit;
        const bool isExtender = m_object->Is({ DIR, DYNAM }) && m_object->IsExtenderElement();

        if (m_place == STAFFREL_above) {
            if (curve && curve->m_object->Is({ LV, PHRASE, SLUR, TIE })) {
                const int shift = this->Intersects(curve, CONTENT, unit);
                if (shift != 0) {
                    this->SetDrawingYRel(this->GetDrawingYRel() - shift);
                }
                return true;
            }
            else if (horizOverlapingBBox->Is(BEAM) && !isExtender) {
                const int shift = this->Intersects(vrv_cast<const Beam *>(horizOverlapingBBox), CONTENT, unit / 2);
                if (shift != 0) {
                    this->SetDrawingYRel(this->GetDrawingYRel() - shift);
                }
                return true;
            }
            yRel = -staffAlignment->CalcOverflowAbove(horizOverlapingBBox) + this->GetContentY1() - margin;

            const Object *object = dynamic_cast<const Object *>(horizOverlapingBBox);
            // For elements, that can have extender lines, we need to make sure that they continue in next system on the
            // same height, as they were before (even if there are no overlapping elements in subsequent measures)
            if (isExtender) {
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
                const int shift = this->Intersects(curve, CONTENT, unit);
                if (shift != 0) {
                    this->SetDrawingYRel(this->GetDrawingYRel() - shift);
                }
                return true;
            }
            else if (horizOverlapingBBox->Is(BEAM) && !isExtender) {
                const int shift = this->Intersects(vrv_cast<const Beam *>(horizOverlapingBBox), CONTENT, unit / 2);
                if (shift != 0) {
                    this->SetDrawingYRel(this->GetDrawingYRel() - shift);
                }
                return true;
            }
            yRel = staffAlignment->CalcOverflowBelow(horizOverlapingBBox) + staffAlignment->GetStaffHeight()
                + this->GetContentY2() + margin;

            const Object *object = dynamic_cast<const Object *>(horizOverlapingBBox);
            // For elements, that can have extender lines, we need to make sure that they continue in next system on the
            // same height, as they were before (even if there are no overlapping elements in subsequent measures)
            if (isExtender) {
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

int FloatingPositioner::GetSpaceBelow(
    const Doc *doc, const StaffAlignment *staffAlignment, const BoundingBox *horizOverlapingBBox) const
{
    if (m_place != STAFFREL_between) return VRV_UNSET;

    int staffSize = staffAlignment->GetStaffSize();

    const FloatingCurvePositioner *curve = dynamic_cast<const FloatingCurvePositioner *>(horizOverlapingBBox);
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
    this->ResetCurveParams();
}

FloatingCurvePositioner::~FloatingCurvePositioner()
{
    ClearSpannedElements();
}

void FloatingCurvePositioner::ResetPositioner()
{
    FloatingPositioner::ResetPositioner();

    this->ResetCurveParams();
}

bool FloatingCurvePositioner::HasCachedX12() const
{
    return ((m_cachedX12.first != VRV_UNSET) && (m_cachedX12.second != VRV_UNSET));
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
    m_thickness = 0;
    m_dir = curvature_CURVEDIR_NONE;
    m_crossStaff = NULL;
    m_cachedMinMaxY = VRV_UNSET;
    m_cachedX12 = { VRV_UNSET, VRV_UNSET };
    m_requestedStaffSpace = 0;
    this->ClearSpannedElements();
}

void FloatingCurvePositioner::UpdateCurveParams(const Point points[4], int thickness, curvature_CURVEDIR curveDir)
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
    this->UpdateCurveParams(points, m_thickness, m_dir);
}

void FloatingCurvePositioner::MoveFrontHorizontal(int distance)
{
    m_points[0].x += distance;
    m_points[1].x += distance;
}

void FloatingCurvePositioner::MoveBackHorizontal(int distance)
{
    m_points[2].x += distance;
    m_points[3].x += distance;
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

int FloatingCurvePositioner::CalcMinMaxY(const Point points[4]) const
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

int FloatingCurvePositioner::CalcAdjustment(
    const BoundingBox *boundingBox, bool &discard, int margin, bool horizontalOverlap) const
{
    int leftAdjustment, rightAdjustment;
    std::tie(leftAdjustment, rightAdjustment)
        = this->CalcLeftRightAdjustment(boundingBox, discard, margin, horizontalOverlap);
    return std::max(leftAdjustment, rightAdjustment);
}

int FloatingCurvePositioner::CalcDirectionalAdjustment(
    const BoundingBox *boundingBox, bool isCurveAbove, bool &discard, int margin, bool horizontalOverlap) const
{
    int leftAdjustment, rightAdjustment;
    std::tie(leftAdjustment, rightAdjustment)
        = this->CalcDirectionalLeftRightAdjustment(boundingBox, isCurveAbove, discard, margin, horizontalOverlap);
    return std::max(leftAdjustment, rightAdjustment);
}

std::pair<int, int> FloatingCurvePositioner::CalcLeftRightAdjustment(
    const BoundingBox *boundingBox, bool &discard, int margin, bool horizontalOverlap) const
{
    return this->CalcDirectionalLeftRightAdjustment(
        boundingBox, (this->GetDir() == curvature_CURVEDIR_above), discard, margin, horizontalOverlap);
}

std::pair<int, int> FloatingCurvePositioner::CalcDirectionalLeftRightAdjustment(
    const BoundingBox *boundingBox, bool isCurveAbove, bool &discard, int margin, bool horizontalOverlap) const
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
    discard = false;

    // first check if they overlap at all
    if (horizontalOverlap) {
        if (p2.x < boundingBox->GetLeftBy(type) - margin) return { 0, 0 };
        if (p1.x > boundingBox->GetRightBy(type) + margin) return { 0, 0 };
    }

    Point topBezier[4], bottomBezier[4];
    BoundingBox::CalcThickBezier(points, this->GetThickness(), topBezier, bottomBezier);

    // Now calculate the left and right adjustments
    int leftAdjustment = 0;
    int rightAdjustment = 0;

    if (isCurveAbove) {
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

        // For selected types use the cut out boundary
        int boxTopY = boundingBox->GetTopBy(type);
        if (boundingBox->Is(ACCID)) {
            const Resources *resources = vrv_cast<const Object *>(boundingBox)->GetDocResources();
            if (resources) {
                boxTopY = boundingBox->GetCutOutTop(*resources);
            }
        }

        leftAdjustment = std::max(boxTopY - leftY, 0);
        rightAdjustment = std::max(boxTopY - rightY, 0);
    }
    else {
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

        // For selected types use the cut out boundary
        int boxBottomY = boundingBox->GetBottomBy(type);
        if (boundingBox->Is(ACCID)) {
            const Resources *resources = vrv_cast<const Object *>(boundingBox)->GetDocResources();
            if (resources) {
                boxBottomY = boundingBox->GetCutOutBottom(*resources);
            }
        }

        leftAdjustment = std::max(leftY - boxBottomY, 0);
        rightAdjustment = std::max(rightY - boxBottomY, 0);
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

std::pair<int, int> FloatingCurvePositioner::CalcRequestedStaffSpace(const StaffAlignment *alignment) const
{
    assert(alignment);

    const TimeSpanningInterface *interface = this->GetObject()->GetTimeSpanningInterface();
    if (interface) {
        const Staff *startStaff = interface->GetStart()->GetAncestorStaff(RESOLVE_CROSS_STAFF, false);
        const Staff *endStaff = interface->GetEnd()->GetAncestorStaff(RESOLVE_CROSS_STAFF, false);

        if (startStaff && endStaff) {
            const int startStaffN = startStaff->GetN();
            const int endStaffN = endStaff->GetN();
            if (startStaffN != endStaffN) {
                if (alignment->GetStaff()->GetN() == std::min(startStaffN, endStaffN)) {
                    return { 0, m_requestedStaffSpace };
                }
                if (alignment->GetStaff()->GetN() == std::max(startStaffN, endStaffN)) {
                    return { m_requestedStaffSpace, 0 };
                }
            }
        }
    }

    return { 0, 0 };
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

int FloatingObject::PrepareStaffCurrentTimeSpanning(FunctorParams *functorParams)
{
    // Pass it to the pseudo functor of the interface
    if (this->HasInterface(INTERFACE_TIME_SPANNING)) {
        TimeSpanningInterface *interface = this->GetTimeSpanningInterface();
        assert(interface);
        interface->InterfacePrepareStaffCurrentTimeSpanning(functorParams, this);
    }
    if (this->HasInterface(INTERFACE_LINKING)) {
        LinkingInterface *interface = this->GetLinkingInterface();
        assert(interface);
        interface->InterfacePrepareStaffCurrentTimeSpanning(functorParams, this);
    }
    return FUNCTOR_CONTINUE;
}

int FloatingObject::ResetData(FunctorParams *functorParams)
{
    // Clear all
    FloatingObject::s_drawingObjectIds.clear();

    m_currentPositioner = NULL;
    // Pass it to the pseudo functor of the interface
    if (this->HasInterface(INTERFACE_TIME_SPANNING)) {
        TimeSpanningInterface *interface = this->GetTimeSpanningInterface();
        assert(interface);
        return interface->InterfaceResetData(functorParams, this);
    }
    else if (this->HasInterface(INTERFACE_TIME_POINT)) {
        TimePointInterface *interface = this->GetTimePointInterface();
        assert(interface);
        return interface->InterfaceResetData(functorParams, this);
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
