/////////////////////////////////////////////////////////////////////////////
// Name:        slur.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "slur.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Slur
//----------------------------------------------------------------------------

Slur::Slur() : FloatingElement("slur-"), TimeSpanningInterface(), AttCurvature()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_CURVATURE);

    Reset();
}

Slur::~Slur()
{
}

void Slur::Reset()
{
    FloatingElement::Reset();
    TimeSpanningInterface::Reset();
    ResetCurvature();

    //m_systemStaffBoundingBoxes.clear();
}

/*
void Slur::ClearBoundingBoxes(System *system)
{
if (m_systemStaffBoundingBoxes.count(system)) {
    m_currentSystemStaffBoundingBoxes = &m_systemStaffBoundingBoxes.at(system);
    m_currentSystemStaffBoundingBoxes->clear();
}
else {
    m_systemStaffBoundingBoxes[system];
    m_currentSystemStaffBoundingBoxes = &m_systemStaffBoundingBoxes.at(system);
}
}

void Slur::AddBoundingBox(Staff *staff, int x, int y)
{
m_currentSystemStaffBoundingBoxes->push_back(std::make_pair(staff, BoundingBox()));
m_currentStaffBoundingBox = &m_currentSystemStaffBoundingBoxes->back();
m_currentStaffBoundingBox->second.SetDrawingX(x);
m_currentStaffBoundingBox->second.SetDrawingY(y);
LogDebug("%d bb for this slur", m_currentSystemStaffBoundingBoxes->size());
}

ArrayOfStaffBoundingBoxPairs *Slur::GetStaffBoundingBoxPairs(System *system)
{
if (m_systemStaffBoundingBoxes.count(system)) {
    return &m_systemStaffBoundingBoxes.at(system);
}
LogDebug("ArrayOfStaffBoundingBoxPairs not found");
return NULL;
}
*/

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

int Slur::ResetDrawing(ArrayPtrVoid *params)
{
    return FUNCTOR_CONTINUE;
};

} // namespace vrv
