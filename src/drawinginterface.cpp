/////////////////////////////////////////////////////////////////////////////
// Name:        drawinginterface.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "drawinginterface.h"

//----------------------------------------------------------------------------

#include "clef.h"
#include "keysig.h"
#include "layerelement.h"
#include "mensur.h"
#include "metersig.h"

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

void DrawingListInterface::AddToDrawingList( Object *object)
{
    m_drawingList.push_back(object);
    m_drawingList.sort();
    m_drawingList.unique();
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
    // owned pointers need to be set to NULL;
    m_currentClef = NULL;
    m_currentKeySig = NULL;
    m_currentMensur = NULL;
    m_currentMeterSig = NULL;
    Reset();
}

StaffDefDrawingInterface::~StaffDefDrawingInterface()
{
    Reset();
}

void StaffDefDrawingInterface::Reset()
{
    if (m_currentClef) {
        delete m_currentClef;
        m_currentClef = NULL;
    }
    if (m_currentKeySig) {
        delete m_currentKeySig;
        m_currentKeySig = NULL;
    }
    if (m_currentMensur) {
        delete m_currentMensur;
        m_currentMensur = NULL;
    }
    if (m_currentMeterSig) {
        delete m_currentMeterSig;
        m_currentMeterSig = NULL;
    }
    m_drawClef = false;
    m_drawKeySig = false;
    m_drawKeySigCancellation = false;
    m_drawMensur = false;
    m_drawMeterSig = false;
}

void StaffDefDrawingInterface::SetCurrentClef(Clef *clef)
{
    if (clef) {
        if (m_currentClef) delete m_currentClef;
        m_currentClef = clef;
        m_currentClef->SetScoreOrStaffDefAttr(true);
    }
}

void StaffDefDrawingInterface::SetCurrentKeySig(KeySig *keySig)
{
    if (keySig) {
        if (m_currentKeySig) {
            keySig->m_drawingCancelAccidCount = m_currentKeySig->GetAlterationNumber();
            keySig->m_drawingCancelAccidType = m_currentKeySig->GetAlterationType();
            delete m_currentKeySig;
        }
        m_currentKeySig = keySig;
        m_currentKeySig->SetScoreOrStaffDefAttr(true);
    }
}

void StaffDefDrawingInterface::SetCurrentMensur(Mensur *mensur)
{
    if (mensur) {
        if (m_currentMensur) delete m_currentMensur;
        m_currentMensur = mensur;
        m_currentMensur->SetScoreOrStaffDefAttr(true);
    }
}

void StaffDefDrawingInterface::SetCurrentMeterSig(MeterSig *meterSig)
{
    if (meterSig) {
        if (m_currentMeterSig) delete m_currentMeterSig;
        m_currentMeterSig = meterSig;
        m_currentMeterSig->SetScoreOrStaffDefAttr(true);
    }
}

StaffDefDrawingInterface::StaffDefDrawingInterface(const StaffDefDrawingInterface &interface)
{
    m_currentClef = NULL;
    m_currentKeySig = NULL;
    m_currentMensur = NULL;
    m_currentMeterSig = NULL;
    Reset();
}

StaffDefDrawingInterface &StaffDefDrawingInterface::operator=(const StaffDefDrawingInterface &interface)
{
    // not self assignement
    if (this != &interface) {
        if (m_currentClef) {
            delete m_currentClef;
            m_currentClef = NULL;
        }
        if (m_currentKeySig) {
            delete m_currentKeySig;
            m_currentKeySig = NULL;
        }
        if (m_currentMensur) {
            delete m_currentMensur;
            m_currentMensur = NULL;
        }
        if (m_currentMeterSig) {
            delete m_currentMeterSig;
            m_currentMeterSig = NULL;
        }
        Reset();
    }
    return *this;
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
