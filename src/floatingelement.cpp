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
#include "dynam.h"
#include "hairpin.h"
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

    m_currentBoundingBox = NULL;
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
    // assert(m_currentBoundingBox);
    if (!m_currentBoundingBox) return;
    m_currentBoundingBox->BoundingBox::UpdateContentBBoxX(x1, x2);
}

void FloatingElement::UpdateContentBBoxY(int y1, int y2)
{
    // assert(m_currentBoundingBox);
    if (!m_currentBoundingBox) return;
    m_currentBoundingBox->BoundingBox::UpdateContentBBoxY(y1, y2);
}

void FloatingElement::UpdateSelfBBoxX(int x1, int x2)
{
    // assert(m_currentBoundingBox);
    if (!m_currentBoundingBox) return;
    m_currentBoundingBox->BoundingBox::UpdateSelfBBoxX(x1, x2);
}

void FloatingElement::UpdateSelfBBoxY(int y1, int y2)
{
    // assert(m_currentBoundingBox);
    if (!m_currentBoundingBox) return;
    m_currentBoundingBox->BoundingBox::UpdateSelfBBoxY(y1, y2);
}

int FloatingElement::GetDrawingX() const
{
    return m_drawingX;
}

int FloatingElement::GetDrawingY() const
{
    // assert(m_currentBoundingBox);
    if (!m_currentBoundingBox) return 0;
    return m_currentBoundingBox->GetDrawingY() - m_currentBoundingBox->GetDrawingYRel();
}

void FloatingElement::SetCurrentFloatingPositioner(FloatingPositioner *boundingBox)
{
    m_currentBoundingBox = boundingBox;
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
        m_place = hairpin->GetPlace() ? hairpin->GetPlace() : STAFFREL_below;
    }
    else if (element->Is() == TEMPO) {
        Tempo *tempo = dynamic_cast<Tempo *>(element);
        assert(tempo);
        // tempo above by default;
        m_place = tempo->GetPlace() ? tempo->GetPlace() : STAFFREL_above;
    }
    else {
        m_place = STAFFREL_NONE;
    }
    ResetBoundingBox();
};

void FloatingPositioner::ResetBoundingBox()
{
    BoundingBox::ResetBoundingBox();

    m_drawingYRel = 0;
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
    if (horizOverlapingBBox == NULL) {
        if (this->m_place == STAFFREL_above) {
            int yRel = m_contentBB_y1;
            this->SetDrawingYRel(yRel);
        }
        else {
            int yRel = staffAlignment->m_staffHeight + m_contentBB_y2;
            this->SetDrawingYRel(yRel);
        }
    }
    else {
        if (this->m_place == STAFFREL_above) {
            int yRel = -staffAlignment->CalcOverflowAbove(horizOverlapingBBox) + m_contentBB_y1;
            this->SetDrawingYRel(yRel);
        }
        else {
            int yRel = staffAlignment->CalcOverflowBelow(horizOverlapingBBox) + staffAlignment->m_staffHeight
                + m_contentBB_y2;
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
    m_currentBoundingBox = NULL;
    // Pass it to the pseudo functor of the interface
    if (this->HasInterface(INTERFACE_TIME_SPANNING)) {
        TimeSpanningInterface *interface = this->GetTimeSpanningInterface();
        assert(interface);
        return interface->InterfaceResetDrawing(params, this);
    }
    return FUNCTOR_CONTINUE;
};

} // namespace vrv
