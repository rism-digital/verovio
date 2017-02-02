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

#include "aligner.h"
#include "dir.h"
#include "doc.h"
#include "dynam.h"
#include "fermata.h"
#include "hairpin.h"
#include "harm.h"
#include "octave.h"
#include "pedal.h"
#include "slur.h"
#include "tempo.h"
#include "trill.h"
#include "timeinterface.h"
#include "vrv.h"

namespace vrv {

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

FloatingObject::~FloatingObject()
{
}

void FloatingObject::Reset()
{
    Object::Reset();

    m_drawingGrpId = DRAWING_GRP_NONE;
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

//----------------------------------------------------------------------------
// FloatingPositioner
//----------------------------------------------------------------------------

FloatingPositioner::FloatingPositioner(FloatingObject *object) : BoundingBox()
{
    assert(object);

    m_object = object;
    if (object->Is() == DIR) {
        Dir *dir = dynamic_cast<Dir *>(object);
        assert(dir);
        // dir below by default
        m_place = dir->HasPlace() ? dir->GetPlace() : STAFFREL_below;
    }
    else if (object->Is() == DYNAM) {
        Dynam *dynam = dynamic_cast<Dynam *>(object);
        assert(dynam);
        // dynam below by default
        m_place = dynam->HasPlace() ? dynam->GetPlace() : STAFFREL_below;
    }
    else if (object->Is() == ENDING) {
        // endings always above;
        m_place = STAFFREL_above;
    }
    else if (object->Is() == FERMATA) {
        Fermata *fermata = dynamic_cast<Fermata *>(object);
        assert(fermata);
        // fermata above by default
        m_place = fermata->HasPlace() ? fermata->GetPlace() : STAFFREL_above;
    }
    else if (object->Is() == HAIRPIN) {
        Hairpin *hairpin = dynamic_cast<Hairpin *>(object);
        assert(hairpin);
        // haripin below by default;
        m_place = hairpin->HasPlace() ? hairpin->GetPlace() : STAFFREL_below;
    }
    else if (object->Is() == HARM) {
        Harm *harm = dynamic_cast<Harm *>(object);
        assert(harm);
        // harm above by default
        m_place = harm->HasPlace() ? harm->GetPlace() : STAFFREL_above;
    }
    else if (object->Is() == OCTAVE) {
        Octave *octave = dynamic_cast<Octave *>(object);
        assert(octave);
        // octave below by default (won't draw without @dis.place anyway);
        m_place = (octave->GetDisPlace() == PLACE_above) ? STAFFREL_above : STAFFREL_below;
    }
    else if (object->Is() == PEDAL) {
        Pedal *pedal = dynamic_cast<Pedal *>(object);
        assert(pedal);
        // pedal below by default
        m_place = pedal->HasPlace() ? pedal->GetPlace() : STAFFREL_below;
    }
    else if (object->Is() == TEMPO) {
        Tempo *tempo = dynamic_cast<Tempo *>(object);
        assert(tempo);
        // tempo above by default;
        m_place = tempo->HasPlace() ? tempo->GetPlace() : STAFFREL_above;
    }
    else if (object->Is() == TRILL) {
        Trill *trill = dynamic_cast<Trill *>(object);
        assert(trill);
        // trill above by default;
        m_place = trill->HasPlace() ? trill->GetPlace() : STAFFREL_above;
    }
    else {
        m_place = STAFFREL_NONE;
    }
    ResetPositioner();
};

void FloatingPositioner::ResetPositioner()
{
    BoundingBox::ResetBoundingBox();

    m_drawingYRel = 0;
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
    return m_drawingX;
}

int FloatingPositioner::GetDrawingY() const
{
    return BoundingBox::GetDrawingY() - this->GetDrawingYRel();
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
    assert((this->GetObject()->Is() == SLUR) || (this->GetObject()->Is() == TIE));
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

void FloatingPositioner::SetDrawingYRel(int drawingYRel)
{
    if (m_place == STAFFREL_above) {
        if (drawingYRel < m_drawingYRel) m_drawingYRel = drawingYRel;
    }
    else {
        if (drawingYRel > m_drawingYRel) m_drawingYRel = drawingYRel;
    }
};

bool FloatingPositioner::CalcDrawingYRel(Doc *doc, StaffAlignment *staffAlignment, BoundingBox *horizOverlapingBBox)
{
    assert(doc);
    assert(staffAlignment);

    int staffSize = staffAlignment->GetStaffSize();
    int yRel;

    if (horizOverlapingBBox == NULL) {
        if (this->m_place == STAFFREL_above) {
            yRel = m_contentBB_y1;
            yRel -= doc->GetBottomMargin(this->m_object->Is()) * doc->GetDrawingUnit(staffSize) / PARAM_DENOMINATOR;
            this->SetDrawingYRel(yRel);
        }
        else {
            yRel = staffAlignment->GetStaffHeight() + m_contentBB_y2;
            yRel += doc->GetTopMargin(this->m_object->Is()) * doc->GetDrawingUnit(staffSize) / PARAM_DENOMINATOR;
            this->SetDrawingYRel(yRel);
        }
    }
    else {
        FloatingPositioner *curve = dynamic_cast<FloatingPositioner *>(horizOverlapingBBox);
        if (curve) {
            assert(curve->m_object);
        }
        if (this->m_place == STAFFREL_above) {
            if (curve && ((curve->m_object->Is() == SLUR) || (curve->m_object->Is() == TIE))) {
                int shift = this->Intersects(curve, doc->GetDrawingUnit(staffSize));
                if (shift != 0) {
                    this->SetDrawingYRel(this->GetDrawingYRel() - shift);
                    // LogDebug("Shift %d", shift);
                }
                return true;
            }
            yRel = -staffAlignment->CalcOverflowAbove(horizOverlapingBBox) + m_contentBB_y1;
            yRel -= doc->GetBottomMargin(this->m_object->Is()) * doc->GetDrawingUnit(staffSize) / PARAM_DENOMINATOR;
            this->SetDrawingYRel(yRel);
        }
        else {
            if (curve && ((curve->m_object->Is() == SLUR) || (curve->m_object->Is() == TIE))) {
                int shift = this->Intersects(curve, doc->GetDrawingUnit(staffSize));
                if (shift != 0) {
                    this->SetDrawingYRel(this->GetDrawingYRel() - shift);
                    // LogDebug("Shift %d", shift);
                }
                return true;
            }
            yRel = staffAlignment->CalcOverflowBelow(horizOverlapingBBox) + staffAlignment->GetStaffHeight()
                + m_contentBB_y2;
            yRel += doc->GetTopMargin(this->m_object->Is()) * doc->GetDrawingUnit(staffSize) / PARAM_DENOMINATOR;
            this->SetDrawingYRel(yRel);
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
    m_currentPositioner = NULL;
    // Pass it to the pseudo functor of the interface
    if (this->HasInterface(INTERFACE_TIME_SPANNING)) {
        TimeSpanningInterface *interface = this->GetTimeSpanningInterface();
        assert(interface);
        return interface->InterfaceResetDrawing(functorParams, this);
    }
    m_drawingGrpId = DRAWING_GRP_NONE;
    return FUNCTOR_CONTINUE;
};

int FloatingObject::UnCastOff(FunctorParams *functorParams)
{
    m_currentPositioner = NULL;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
