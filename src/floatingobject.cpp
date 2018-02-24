/////////////////////////////////////////////////////////////////////////////
// Name:        floatingobject.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "floatingobject.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

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

    if (object->Is(BREATH)) {
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
    m_cuvrePoints[0] = Point(0, 0);
    m_cuvrePoints[1] = Point(0, 0);
    m_cuvrePoints[2] = Point(0, 0);
    m_cuvrePoints[3] = Point(0, 0);
    m_cuvreAngle = 0.0;
    m_cuvreThickness = 0;
    m_cuvreDir = curvature_CURVEDIR_NONE;
    m_cuvreXMinMaxY = -1;
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

void FloatingPositioner::UpdateCurvePosition(
    const Point points[4], float angle, int thickness, curvature_CURVEDIR curveDir)
{
    m_cuvrePoints[0] = points[0];
    m_cuvrePoints[1] = points[1];
    m_cuvrePoints[2] = points[2];
    m_cuvrePoints[3] = points[3];
    m_cuvreAngle = angle;
    m_cuvreThickness = thickness;
    m_cuvreDir = curveDir;
    m_cuvreXMinMaxY = -1;
}

int FloatingPositioner::CalcXMinMaxY(const Point points[4])
{
    assert(this->GetObject());
    assert(this->GetObject()->Is({ SLUR, TIE }));
    assert(m_cuvreDir != curvature_CURVEDIR_NONE);

    if (m_cuvreXMinMaxY != -1) return m_cuvreXMinMaxY;
    Point pos;
    int width, height;
    int minYPos, maxYPos;

    BoundingBox::ApproximateBezierBoundingBox(points, pos, width, height, minYPos, maxYPos);
    if (m_cuvreDir == curvature_CURVEDIR_above)
        m_cuvreXMinMaxY = maxYPos;
    else
        m_cuvreXMinMaxY = minYPos;

    return m_cuvreXMinMaxY;
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
        FloatingPositioner *curve = dynamic_cast<FloatingPositioner *>(horizOverlapingBBox);
        if (curve) {
            assert(curve->m_object);
        }
        int margin = doc->GetBottomMargin(this->m_object->GetClassId()) * doc->GetDrawingUnit(staffSize);

        if (this->m_place == STAFFREL_basic_above) {
            if (curve && curve->m_object->Is({ SLUR, TIE })) {
                int shift = this->Intersects(curve, doc->GetDrawingUnit(staffSize));
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
                int shift = this->Intersects(curve, doc->GetDrawingUnit(staffSize));
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
        return interface->InterfaceFillStaffCurrentTimeSpanning(functorParams, this);
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
