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
#include "hairpin.h"
#include "harm.h"
#include "mordent.h"
#include "octave.h"
#include "pedal.h"
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

std::vector<void *> FloatingObject::s_drawingObjectIds;

//----------------------------------------------------------------------------
// FloatingObject
//----------------------------------------------------------------------------

FloatingObject::FloatingObject() : Object("fe")
{
    Reset();
}

FloatingObject::FloatingObject(std::string classid) : Object(classid)
{
    Reset();

    m_currentPositioner = NULL;
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
        m_place = STAFFREL_basic_above;
    }
    else if (object->Is(BREATH)) {
        Breath *breath = dynamic_cast<Breath *>(object);
        assert(breath);
        // breath above by default
        m_place = (breath->GetPlaceAlternate()->GetBasic() != STAFFREL_basic_NONE)
            ? breath->GetPlaceAlternate()->GetBasic()
            : STAFFREL_basic_above;
    }
    else if (object->Is(DIR)) {
        Dir *dir = dynamic_cast<Dir *>(object);
        assert(dir);
        // dir below by default
        m_place = (dir->GetPlaceAlternate()->GetBasic() != STAFFREL_basic_NONE) ? dir->GetPlaceAlternate()->GetBasic()
                                                                                : STAFFREL_basic_below;
    }
    else if (object->Is(DYNAM)) {
        Dynam *dynam = dynamic_cast<Dynam *>(object);
        assert(dynam);
        // dynam below by default
        m_place = (dynam->GetPlaceAlternate()->GetBasic() != STAFFREL_basic_NONE)
            ? dynam->GetPlaceAlternate()->GetBasic()
            : STAFFREL_basic_below;
    }
    else if (object->Is(ENDING)) {
        // endings always above;
        m_place = STAFFREL_basic_above;
    }
    else if (object->Is(FERMATA)) {
        Fermata *fermata = dynamic_cast<Fermata *>(object);
        assert(fermata);
        // fermata above by default
        m_place = (fermata->GetPlaceAlternate()->GetBasic() != STAFFREL_basic_NONE)
            ? fermata->GetPlaceAlternate()->GetBasic()
            : STAFFREL_basic_above;
    }
    else if (object->Is(HAIRPIN)) {
        Hairpin *hairpin = dynamic_cast<Hairpin *>(object);
        assert(hairpin);
        // haripin below by default;
        m_place = (hairpin->GetPlaceAlternate()->GetBasic() != STAFFREL_basic_NONE)
            ? hairpin->GetPlaceAlternate()->GetBasic()
            : STAFFREL_basic_below;
    }
    else if (object->Is(HARM)) {
        Harm *harm = dynamic_cast<Harm *>(object);
        assert(harm);
        // harm above by default
        m_place = (harm->GetPlaceAlternate()->GetBasic() != STAFFREL_basic_NONE) ? harm->GetPlaceAlternate()->GetBasic()
                                                                                 : STAFFREL_basic_above;
        if ((harm->GetPlaceAlternate()->GetBasic() == STAFFREL_basic_NONE) && object->GetFirst()->Is(FB))
            m_place = STAFFREL_basic_below;
    }
    else if (object->Is(MORDENT)) {
        Mordent *mordent = dynamic_cast<Mordent *>(object);
        assert(mordent);
        // mordent above by default;
        m_place = (mordent->GetPlaceAlternate()->GetBasic() != STAFFREL_basic_NONE)
            ? mordent->GetPlaceAlternate()->GetBasic()
            : STAFFREL_basic_above;
    }
    else if (object->Is(OCTAVE)) {
        Octave *octave = dynamic_cast<Octave *>(object);
        assert(octave);
        // octave below by default (won't draw without @dis.place anyway);
        m_place = (octave->GetDisPlace() == STAFFREL_basic_above) ? STAFFREL_basic_above : STAFFREL_basic_below;
    }
    else if (object->Is(PEDAL)) {
        Pedal *pedal = dynamic_cast<Pedal *>(object);
        assert(pedal);
        // pedal below by default
        m_place = (pedal->GetPlaceAlternate()->GetBasic() != STAFFREL_basic_NONE)
            ? pedal->GetPlaceAlternate()->GetBasic()
            : STAFFREL_basic_below;
    }
    else if (object->Is(TEMPO)) {
        Tempo *tempo = dynamic_cast<Tempo *>(object);
        assert(tempo);
        // tempo above by default;
        m_place = (tempo->GetPlaceAlternate()->GetBasic() != STAFFREL_basic_NONE)
            ? tempo->GetPlaceAlternate()->GetBasic()
            : STAFFREL_basic_above;
    }
    else if (object->Is(TRILL)) {
        Trill *trill = dynamic_cast<Trill *>(object);
        assert(trill);
        // trill above by default;
        m_place = (trill->GetPlaceAlternate()->GetBasic() != STAFFREL_basic_NONE)
            ? trill->GetPlaceAlternate()->GetBasic()
            : STAFFREL_basic_above;
    }
    else if (object->Is(TURN)) {
        Turn *turn = dynamic_cast<Turn *>(object);
        assert(turn);
        // turn above by default;
        m_place = (turn->GetPlaceAlternate()->GetBasic() != STAFFREL_basic_NONE) ? turn->GetPlaceAlternate()->GetBasic()
                                                                                 : STAFFREL_basic_above;
    }
    else {
        m_place = STAFFREL_basic_NONE;
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

void FloatingPositioner::SetDrawingYRel(int drawingYRel)
{
    ResetCachedDrawingY();
    if (m_place == STAFFREL_basic_above) {
        if (drawingYRel < m_drawingYRel) m_drawingYRel = drawingYRel;
    }
    else {
        if (drawingYRel > m_drawingYRel) m_drawingYRel = drawingYRel;
    }
}

bool FloatingPositioner::CalcDrawingYRel(Doc *doc, StaffAlignment *staffAlignment, BoundingBox *horizOverlapingBBox)
{
    assert(doc);
    assert(staffAlignment);

    int staffSize = staffAlignment->GetStaffSize();
    int yRel;

    if (horizOverlapingBBox == NULL) {
        if (this->m_place == STAFFREL_basic_above) {
            yRel = GetContentY1();
            yRel -= doc->GetBottomMargin(this->m_object->GetClassId()) * doc->GetDrawingUnit(staffSize);
            this->SetDrawingYRel(yRel);
        }
        else {
            yRel = staffAlignment->GetStaffHeight() + GetContentY2();
            yRel += doc->GetTopMargin(this->m_object->GetClassId()) * doc->GetDrawingUnit(staffSize);
            this->SetDrawingYRel(yRel);
        }
    }
    else {
        FloatingCurvePositioner *curve = dynamic_cast<FloatingCurvePositioner *>(horizOverlapingBBox);
        if (curve) {
            assert(curve->m_object);
        }
        int margin = doc->GetBottomMargin(this->m_object->GetClassId()) * doc->GetDrawingUnit(staffSize);

        if (this->m_place == STAFFREL_basic_above) {
            if (curve && curve->m_object->Is({ SLUR, TIE })) {
                int shift = this->Intersects(curve, CONTENT, doc->GetDrawingUnit(staffSize));
                if (shift != 0) {
                    this->SetDrawingYRel(this->GetDrawingYRel() - shift);
                    // LogDebug("Shift %d", shift);
                }
                return true;
            }
            yRel = -staffAlignment->CalcOverflowAbove(horizOverlapingBBox) + GetContentY1() - margin;
            Object *object = dynamic_cast<Object *>(horizOverlapingBBox);
            // With LayerElement always move them up
            if (object && object->IsLayerElement()) {
                if (yRel < 0) this->SetDrawingYRel(yRel);
            }
            // Otherwise only if the is a vertical overlap
            else if (this->VerticalContentOverlap(horizOverlapingBBox, margin)) {
                this->SetDrawingYRel(yRel);
            }
        }
        else {
            if (curve && curve->m_object->Is({ SLUR, TIE })) {
                int shift = this->Intersects(curve, CONTENT, doc->GetDrawingUnit(staffSize));
                if (shift != 0) {
                    this->SetDrawingYRel(this->GetDrawingYRel() - shift);
                    // LogDebug("Shift %d", shift);
                }
                return true;
            }
            yRel = staffAlignment->CalcOverflowBelow(horizOverlapingBBox) + staffAlignment->GetStaffHeight()
                + GetContentY2() + margin;
            Object *object = dynamic_cast<Object *>(horizOverlapingBBox);
            // With LayerElement always move them down
            if (object && object->IsLayerElement()) {
                if (yRel > 0) this->SetDrawingYRel(yRel);
            }
            // Otherwise only if the is a vertical overlap
            else if (this->VerticalContentOverlap(horizOverlapingBBox, margin)) {
                this->SetDrawingYRel(yRel);
            }
        }
    }
    return true;
}

//----------------------------------------------------------------------------
// FloatingCurvePositioner
//----------------------------------------------------------------------------

FloatingCurvePositioner::FloatingCurvePositioner(FloatingObject *object, StaffAlignment *alignment, char spanningType)
    : FloatingPositioner(object, alignment, spanningType)
{
    ResetCurveParams();
}

void FloatingCurvePositioner::ResetPositioner()
{
    FloatingPositioner::ResetPositioner();

    ResetCurveParams();
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
    m_cachedMinMaxY = VRV_UNSET;
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
    
    m_stages.push_back(m_points[0]);
    m_stages.push_back(m_points[1]);
    m_stages.push_back(m_points[2]);
    m_stages.push_back(m_points[3]);
}

int FloatingCurvePositioner::CalcMinMaxY(const Point points[4])
{
    assert(this->GetObject());
    assert(this->GetObject()->Is({ SLUR, TIE }));
    assert(m_dir != curvature_CURVEDIR_NONE);

    if (m_cachedMinMaxY != VRV_UNSET) return m_cachedMinMaxY;
    Point pos;
    int width, height;
    int minYPos, maxYPos;

    BoundingBox::ApproximateBezierBoundingBox(points, pos, width, height, minYPos, maxYPos);
    m_cachedMinMaxY = (m_dir == curvature_CURVEDIR_above) ? maxYPos : minYPos;

    return m_cachedMinMaxY;
}
    
int FloatingCurvePositioner::CalcAdjustment(LayerElement *element, bool &discard, int margin)
{
    assert(element);
    assert(element->HasSelfBB());
    
    Point points[4];
    // We need to get the points because then stored points are relative
    this->GetPoints(points);
    
    // for lisability
    Point p1 = points[0];
    Point p2 = points[3];
    
    Accessor type = SELF;
    bool keepInside = (element->Is({ARTIC, ARTIC_PART, NOTE, STEM}));
    discard = false;
    
    // first check if they overlap at all
    if (p2.x < element->GetLeftBy(type) + margin) return 0;
    if (p1.x > element->GetRightBy(type) + margin) return 0;
    
    Point topBezier[4], bottomBezier[4];
    BoundingBox::CalcThickBezier(points, this->GetThickness(), this->GetAngle(), topBezier, bottomBezier);
    
    // The curve overflows on both sides
    if ((p1.x < element->GetLeftBy(type)) && p2.x > element->GetRightBy(type)) {
        // LogDebug("overflows both sides");
        if (this->GetDir() == curvature_CURVEDIR_above) {
            // The curve is below the content - if the element needs to be kept inside (e.g. a note), then do not return.
            if (((this->GetTopBy(type) + margin) < element->GetBottomBy(type)) && !keepInside) return 0;
            // Calcuate the y positions
            int leftY = BoundingBox::CalcBezierAtPosition(bottomBezier, element->GetLeftBy(type)) - margin;
            int rightY = BoundingBox::CalcBezierAtPosition(bottomBezier, element->GetRightBy(type)) - margin;
            // Everything is underneath - we can discard the element
            if ((leftY >= element->GetTopBy(type)) && (rightY >= element->GetTopBy(type))) {
                discard = true;
                return 0;
            }
            // Return the maximum adjustment required
            return std::max(element->GetTopBy(type) - leftY, element->GetBottomBy(type) - rightY);
        }
        else {
            // The curve is above the content  - if the element needs to be kept inside (e.g. a note), then do not return.
            if (((this->GetBottomBy(type) - margin) > element->GetTopBy(type)) && !keepInside) return 0;
            // Calculate the y positions
            int leftY = BoundingBox::CalcBezierAtPosition(topBezier, element->GetLeftBy(type)) + margin;
            int rightY = BoundingBox::CalcBezierAtPosition(topBezier, element->GetRightBy(type)) + margin;
            // Everything is above - we can discard the element
            if ((leftY <= element->GetBottomBy(type)) && (rightY <= element->GetBottomBy(type))) {
                discard = true;
                return 0;
            }
            // Return the maximum adjustment required
            return std::max(leftY - element->GetBottomBy(type), rightY - element->GetBottomBy(type));
        }
    }
    // The curve overflows on the left
    else if ((p1.x < element->GetLeftBy(type)) && p2.x <= element->GetRightBy(type)) {
        // LogDebug("left T-L %d %d ; B-R %d %d", element->GetTopBy(type), element->GetLeftBy(type),
        // element->GetBottom(), element->GetRight());
        if (this->GetDir() == curvature_CURVEDIR_above) {
            int xMaxY = this->CalcMinMaxY(topBezier);
            // The starting point is above the top
            if (p2.y > element->GetTopBy(type) + margin) return 0;
            // The left point is beyond (before) the summit of the curve
            if (element->GetLeftBy(type) < (p1.x + xMaxY))
                return (this->GetTopBy(type) - element->GetBottomBy(type) + margin);
            // Calcultate the Y position of the curve one the left
            int leftY = BoundingBox::CalcBezierAtPosition(topBezier, element->GetLeftBy(type)) + margin;
            // LogDebug("leftY %d, %d, %d", leftY, element->GetBottomBy(type), element->GetTopBy(type));
            // The content left is below the bottom
            if (leftY < element->GetBottomBy(type)) return 0;
            // Else return the shift needed
            return (leftY - element->GetBottomBy(type));
        }
        else {
            int xMinY = this->CalcMinMaxY(topBezier);
            // The starting point is below the bottom
            if (p2.y < element->GetBottomBy(type) + margin) return 0;
            // The left point is beyond (before) the summit of the curve
            if (element->GetLeftBy(type) < (p1.x + xMinY))
                return (this->GetBottomBy(type) - element->GetTopBy(type) - margin);
            // Calcultate the Y position of the curve one the left
            int leftY = BoundingBox::CalcBezierAtPosition(bottomBezier, element->GetLeftBy(type)) - margin;
            // LogDebug("leftY %d, %d, %d", leftY, element->GetBottomBy(type), element->GetTopBy(type));
            // The content left is above the top
            if (leftY > element->GetTopBy(type)) return 0;
            // Else return the shift needed
            return (leftY - element->GetTopBy(type));
        }
    }
    // The curve overflows on the right
    else if ((p1.x >= element->GetLeftBy(type)) && p2.x > element->GetRightBy(type)) {
        // LogDebug("right T-L %d %d ; B-R %d %d", element->GetTopBy(type), element->GetLeftBy(type),
        // element->GetBottomBy(type), element->GetRightBy(type));
        if (this->GetDir() == curvature_CURVEDIR_above) {
            int xMaxY = this->CalcMinMaxY(topBezier);
            // The starting point is above the top
            if (p1.y > element->GetTopBy(type) + margin) return 0;
            // The right point is beyond the summit of the curve
            if (element->GetRightBy(type) > (p1.x + xMaxY))
                return (this->GetTopBy(type) - element->GetBottomBy(type) + margin);
            // Calcultate the Y position of the curve one the right
            int rightY = BoundingBox::CalcBezierAtPosition(topBezier, element->GetRightBy(type)) + margin;
            // LogDebug("rightY %d, %d, %d", rightY, element->GetBottomBy(type), element->GetTopBy(type));
            // The content right is below the bottom
            if (rightY < element->GetBottomBy(type)) return 0;
            // Return the shift needed
            return (rightY - element->GetBottomBy(type));
        }
        else {
            int xMinY = this->CalcMinMaxY(bottomBezier);
            // The starting point is below the bottom
            if (p1.y < element->GetBottomBy(type) + margin) return 0;
            // The right point is beyond the summit of the curve
            if (element->GetRightBy(type) > (p1.x + xMinY))
                return (this->GetBottomBy(type) - element->GetTopBy(type) - margin);
            // Calcultate the Y position of the curve one the right
            int rightY = BoundingBox::CalcBezierAtPosition(bottomBezier, element->GetRightBy(type)) - margin;
            // LogDebug("rightY %d, %d, %d", rightY, element->GetBottomBy(type), element->GetTopBy(type));
            // The content right is above the top
            if (rightY > element->GetTopBy(type)) return 0;
            // Return the shift needed
            return (rightY - element->GetTopBy(type));
        }
    }
    // The curve in inside the left and right side of the content
    else if ((p1.x >= element->GetLeftBy(type)) && p2.x <= element->GetRightBy(type)) {
        // LogDebug("inside");
        if (this->GetDir() == curvature_CURVEDIR_above) {
            return (this->GetTopBy(type) - element->GetBottomBy(type) + margin);
        }
        else {
            return (this->GetBottomBy(type) - element->GetTopBy(type) - margin);
        }
    }
    else {
        LogDebug("This should not happen");
    }
    
    return 0;
    
}

void FloatingCurvePositioner::GetPoints(Point points[4], int stage)
{
    if (stage == -1) {
        points[0] = m_points[0];
        points[1] = m_points[1];
        points[2] = m_points[2];
        points[3] = m_points[3];
    }
    else {
        points[0] = m_stages.at(0 + stage * 4);
        points[1] = m_stages.at(1 + stage * 4);
        points[2] = m_stages.at(2 + stage * 4);
        points[3] = m_stages.at(3 + stage * 4);
    }
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
