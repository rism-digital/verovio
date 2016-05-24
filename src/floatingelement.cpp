/////////////////////////////////////////////////////////////////////////////
// Name:        floatingelement.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "floatingelement.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "aligner.h"
#include "dir.h"
#include "doc.h"
#include "dynam.h"
#include "hairpin.h"
#include "octave.h"
#include "slur.h"
#include "tempo.h"
#include "timeinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// FloatingElement
//----------------------------------------------------------------------------

FloatingElement::FloatingElement() : Object("fe")
{
    Reset();
}

FloatingElement::FloatingElement(std::string classid) : Object(classid)
{
    Reset();

    m_currentPositioner = NULL;
}

FloatingElement::~FloatingElement()
{
}

void FloatingElement::Reset()
{
    Object::Reset();
}

void FloatingElement::UpdateContentBBoxX(int x1, int x2)
{
    if (!m_currentPositioner) return;
    m_currentPositioner->BoundingBox::UpdateContentBBoxX(x1, x2);
}

void FloatingElement::UpdateContentBBoxY(int y1, int y2)
{
    if (!m_currentPositioner) return;
    m_currentPositioner->BoundingBox::UpdateContentBBoxY(y1, y2);
}

void FloatingElement::UpdateSelfBBoxX(int x1, int x2)
{
    if (!m_currentPositioner) return;
    m_currentPositioner->BoundingBox::UpdateSelfBBoxX(x1, x2);
}

void FloatingElement::UpdateSelfBBoxY(int y1, int y2)
{
    if (!m_currentPositioner) return;
    m_currentPositioner->BoundingBox::UpdateSelfBBoxY(y1, y2);
}

int FloatingElement::GetDrawingX() const
{
    return m_drawingX;
}

int FloatingElement::GetDrawingY() const
{
    if (!m_currentPositioner) return 0;
    return m_currentPositioner->GetDrawingY() - m_currentPositioner->GetDrawingYRel();
}

void FloatingElement::SetCurrentFloatingPositioner(FloatingPositioner *boundingBox)
{
    m_currentPositioner = boundingBox;
}

//----------------------------------------------------------------------------
// FloatingPositioner
//----------------------------------------------------------------------------

FloatingPositioner::FloatingPositioner(FloatingElement *element) : BoundingBox()
{
    assert(element);

    m_element = element;
    if (element->Is() == DIR) {
        Dir *dir = dynamic_cast<Dir *>(element);
        assert(dir);
        // dir below by default
        m_place = dir->HasPlace() ? dir->GetPlace() : STAFFREL_below;
    }
    else if (element->Is() == DYNAM) {
        Dynam *dynam = dynamic_cast<Dynam *>(element);
        assert(dynam);
        // dynam below by default
        m_place = dynam->HasPlace() ? dynam->GetPlace() : STAFFREL_below;
    }
    else if (element->Is() == HAIRPIN) {
        Hairpin *hairpin = dynamic_cast<Hairpin *>(element);
        assert(hairpin);
        // haripin below by default;
        m_place = hairpin->HasPlace() ? hairpin->GetPlace() : STAFFREL_below;
    }
    else if (element->Is() == OCTAVE) {
        Octave *octave = dynamic_cast<Octave *>(element);
        assert(octave);
        // octave below by default (won't draw without @dis.place anyway);
        m_place = (octave->GetDisPlace() == PLACE_above) ? STAFFREL_above : STAFFREL_below;
    }
    else if (element->Is() == TEMPO) {
        Tempo *tempo = dynamic_cast<Tempo *>(element);
        assert(tempo);
        // tempo above by default;
        m_place = tempo->HasPlace() ? tempo->GetPlace() : STAFFREL_above;
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
    m_slurPoints[0] = Point(0, 0);
    m_slurPoints[1] = Point(0, 0);
    m_slurPoints[2] = Point(0, 0);
    m_slurPoints[3] = Point(0, 0);
    m_slurAngle = 0.0;
    m_slurThickness = 0;
    m_slurDir = curvature_CURVEDIR_NONE;
}

void FloatingPositioner::UpdateSlurPosition(
    const Point points[4], float angle, int thickness, curvature_CURVEDIR curveDir)
{
    m_slurPoints[0] = points[0];
    m_slurPoints[1] = points[1];
    m_slurPoints[2] = points[2];
    m_slurPoints[3] = points[3];
    m_slurAngle = angle;
    m_slurThickness = thickness;
    m_slurDir = curveDir;
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
            yRel -= doc->GetBottomMargin(this->m_element->Is()) * doc->GetDrawingUnit(staffSize) / PARAM_DENOMINATOR;
            this->SetDrawingYRel(yRel);
        }
        else {
            yRel = staffAlignment->GetStaffHeight() + m_contentBB_y2;
            yRel += doc->GetTopMargin(this->m_element->Is()) * doc->GetDrawingUnit(staffSize) / PARAM_DENOMINATOR;
            this->SetDrawingYRel(yRel);
        }
    }
    else {
        if (this->m_place == STAFFREL_above) {
            yRel = -staffAlignment->CalcOverflowAbove(horizOverlapingBBox) + m_contentBB_y1;
            yRel -= doc->GetBottomMargin(this->m_element->Is()) * doc->GetDrawingUnit(staffSize) / PARAM_DENOMINATOR;
            this->SetDrawingYRel(yRel);
        }
        else {
            yRel = staffAlignment->CalcOverflowBelow(horizOverlapingBBox) + staffAlignment->GetStaffHeight()
                + m_contentBB_y2;
            yRel += doc->GetTopMargin(this->m_element->Is()) * doc->GetDrawingUnit(staffSize) / PARAM_DENOMINATOR;
            this->SetDrawingYRel(yRel);
        }
    }
    return true;
}

//----------------------------------------------------------------------------
// FloatingElement functor methods
//----------------------------------------------------------------------------

int FloatingElement::PrepareTimeSpanning(ArrayPtrVoid *params)
{
    // Pass it to the pseudo functor of the interface
    if (this->HasInterface(INTERFACE_TIME_SPANNING)) {
        TimeSpanningInterface *interface = this->GetTimeSpanningInterface();
        assert(interface);
        return interface->InterfacePrepareTimeSpanning(params, this);
    }
    return FUNCTOR_CONTINUE;
}

int FloatingElement::PrepareTimestamps(ArrayPtrVoid *params)
{
    // Pass it to the pseudo functor of the interface
    if (this->HasInterface(INTERFACE_TIME_POINT)) {
        TimePointInterface *interface = this->GetTimePointInterface();
        assert(interface);
        return interface->InterfacePrepareTimestamps(params, this);
    }
    else if (this->HasInterface(INTERFACE_TIME_SPANNING)) {
        TimeSpanningInterface *interface = this->GetTimeSpanningInterface();
        assert(interface);
        return interface->InterfacePrepareTimestamps(params, this);
    }
    return FUNCTOR_CONTINUE;
}

int FloatingElement::FillStaffCurrentTimeSpanning(ArrayPtrVoid *params)
{
    // Pass it to the pseudo functor of the interface
    if (this->HasInterface(INTERFACE_TIME_SPANNING)) {
        TimeSpanningInterface *interface = this->GetTimeSpanningInterface();
        assert(interface);
        return interface->InterfaceFillStaffCurrentTimeSpanning(params, this);
    }
    return FUNCTOR_CONTINUE;
}

int FloatingElement::ResetDrawing(ArrayPtrVoid *params)
{
    m_currentPositioner = NULL;
    // Pass it to the pseudo functor of the interface
    if (this->HasInterface(INTERFACE_TIME_SPANNING)) {
        TimeSpanningInterface *interface = this->GetTimeSpanningInterface();
        assert(interface);
        return interface->InterfaceResetDrawing(params, this);
    }
    return FUNCTOR_CONTINUE;
};

int FloatingElement::UnCastOff(ArrayPtrVoid *params)
{
    m_currentPositioner = NULL;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
