/////////////////////////////////////////////////////////////////////////////
// Name:        drawinginterface.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "drawinginterface.h"

//----------------------------------------------------------------------------

#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// DrawingListInterface
//----------------------------------------------------------------------------

DrawingListInterface::DrawingListInterface()
{
    Reset();
}

DrawingListInterface::~DrawingListInterface()
{
}

void DrawingListInterface::Reset()
{
    m_drawingList.clear();
}

void DrawingListInterface::AddToDrawingList(Object *object)
{
    if (std::find(m_drawingList.begin(), m_drawingList.end(), object) == m_drawingList.end()) {
        // someName not in name, add it
        m_drawingList.push_back(object);
    }

    /*
    m_drawingList.push_back(object);
    m_drawingList.sort();
    m_drawingList.unique();
     */
}

ListOfObjects *DrawingListInterface::GetDrawingList()
{
    return &m_drawingList;
}

void DrawingListInterface::ResetDrawingList()
{
    m_drawingList.clear();
}

//----------------------------------------------------------------------------
// StaffDefDrawingInterface
//----------------------------------------------------------------------------

StaffDefDrawingInterface::StaffDefDrawingInterface()
{
    Reset();
}

StaffDefDrawingInterface::~StaffDefDrawingInterface()
{
}

void StaffDefDrawingInterface::Reset()
{
    m_currentClef.Reset();
    m_currentKeySig.Reset();
    m_currentMensur.Reset();
    m_currentMeterSig.Reset();

    if (m_currentClef.GetLine() > 4) {
        Clef clef2;
    }

    m_drawClef = false;
    m_drawKeySig = false;
    m_drawMensur = false;
    m_drawMeterSig = false;
}

void StaffDefDrawingInterface::SetCurrentClef(Clef *clef)
{
    if (clef) {
        m_currentClef = *clef;
    }
}

void StaffDefDrawingInterface::SetCurrentKeySig(KeySig *keySig)
{
    if (keySig) {
        char drawingCancelAccidCount = m_currentKeySig.GetAlterationNumber();
        data_ACCIDENTAL_EXPLICIT drawingCancelAccidType = m_currentKeySig.GetAlterationType();
        m_currentKeySig = *keySig;
        m_currentKeySig.m_drawingCancelAccidCount = drawingCancelAccidCount;
        m_currentKeySig.m_drawingCancelAccidType = drawingCancelAccidType;
    }
}

void StaffDefDrawingInterface::SetCurrentMensur(Mensur *mensur)
{
    if (mensur) {
        m_currentMensur = *mensur;
    }
}

void StaffDefDrawingInterface::SetCurrentMeterSig(MeterSig *meterSig)
{
    if (meterSig) {
        m_currentMeterSig = *meterSig;
    }
}

//----------------------------------------------------------------------------
// StemmedDrawingInterface
//----------------------------------------------------------------------------

StemmedDrawingInterface::StemmedDrawingInterface()
{
    Reset();
}

StemmedDrawingInterface::~StemmedDrawingInterface()
{
}

void StemmedDrawingInterface::Reset()
{
    m_drawingStemDir = STEMDIRECTION_NONE;
    m_drawingStemStart = Point(0, 0);
    m_drawingStemEnd = Point(0, 0);
}

void StemmedDrawingInterface::SetDrawingStemDir(data_STEMDIRECTION stemDir)
{
    m_drawingStemDir = stemDir;
}

data_STEMDIRECTION StemmedDrawingInterface::GetDrawingStemDir()
{
    return m_drawingStemDir;
}

void StemmedDrawingInterface::SetDrawingStemStart(Point stemStart)
{
    m_drawingStemStart = stemStart;
}

Point StemmedDrawingInterface::GetDrawingStemStart()
{
    return m_drawingStemStart;
}

void StemmedDrawingInterface::SetDrawingStemEnd(Point stemEnd)
{
    m_drawingStemEnd = stemEnd;
}

Point StemmedDrawingInterface::GetDrawingStemEnd()
{
    return m_drawingStemEnd;
}

} // namespace vrv
