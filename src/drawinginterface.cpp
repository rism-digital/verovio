/////////////////////////////////////////////////////////////////////////////
// Name:        drawinginterface.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "drawinginterface.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "chord.h"
#include "elementpart.h"
#include "layerelement.h"
#include "note.h"
#include "object.h"

namespace vrv {

//----------------------------------------------------------------------------
// DrawingListInterface
//----------------------------------------------------------------------------

DrawingListInterface::DrawingListInterface()
{
    Reset();
}

DrawingListInterface::~DrawingListInterface() {}

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

ArrayOfObjects *DrawingListInterface::GetDrawingList()
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

StaffDefDrawingInterface::~StaffDefDrawingInterface() {}

void StaffDefDrawingInterface::Reset()
{
    m_currentClef.Reset();
    m_currentKeySig.Reset();
    m_currentMensur.Reset();
    m_currentMeterSig.Reset();

    m_drawClef = false;
    m_drawKeySig = false;
    m_drawMensur = false;
    m_drawMeterSig = false;
}

void StaffDefDrawingInterface::SetCurrentClef(Clef const *clef)
{
    if (clef) {
        m_currentClef = *clef;
        m_currentClef.CloneReset();
    }
}

void StaffDefDrawingInterface::SetCurrentKeySig(KeySig const *keySig)
{
    if (keySig) {
        char drawingCancelAccidCount = m_currentKeySig.GetAccidCount();
        data_ACCIDENTAL_WRITTEN drawingCancelAccidType = m_currentKeySig.GetAccidType();
        m_currentKeySig = *keySig;
        m_currentKeySig.CloneReset();
        m_currentKeySig.m_drawingCancelAccidCount = drawingCancelAccidCount;
        m_currentKeySig.m_drawingCancelAccidType = drawingCancelAccidType;
    }
}

void StaffDefDrawingInterface::SetCurrentMensur(Mensur const *mensur)
{
    if (mensur) {
        m_currentMensur = *mensur;
        m_currentMensur.CloneReset();
    }
}

void StaffDefDrawingInterface::SetCurrentMeterSig(MeterSig const *meterSig)
{
    if (meterSig) {
        m_currentMeterSig = *meterSig;
        m_currentMeterSig.CloneReset();
    }
}

//----------------------------------------------------------------------------
// StemmedDrawingInterface
//----------------------------------------------------------------------------

StemmedDrawingInterface::StemmedDrawingInterface()
{
    Reset();
}

StemmedDrawingInterface::~StemmedDrawingInterface() {}

void StemmedDrawingInterface::Reset()
{
    m_drawingStem = NULL;
}

void StemmedDrawingInterface::SetDrawingStem(Stem *stem)
{
    m_drawingStem = stem;
}

void StemmedDrawingInterface::SetDrawingStemDir(data_STEMDIRECTION stemDir)
{
    if (m_drawingStem) m_drawingStem->SetDrawingStemDir(stemDir);
}

data_STEMDIRECTION StemmedDrawingInterface::GetDrawingStemDir()
{
    if (m_drawingStem) return m_drawingStem->GetDrawingStemDir();
    return STEMDIRECTION_NONE;
}

void StemmedDrawingInterface::SetDrawingStemLen(int stemLen)
{
    if (m_drawingStem) m_drawingStem->SetDrawingStemLen(stemLen);
}

int StemmedDrawingInterface::GetDrawingStemLen()
{
    if (m_drawingStem) return m_drawingStem->GetDrawingStemLen();
    return 0;
}

Point StemmedDrawingInterface::GetDrawingStemStart(Object *object)
{
    assert(m_drawingStem || object);
    if (object && !m_drawingStem) {
        assert(this == dynamic_cast<StemmedDrawingInterface *>(object));
        return Point(object->GetDrawingX(), object->GetDrawingY());
    }
    return Point(m_drawingStem->GetDrawingX(), m_drawingStem->GetDrawingY());
}

Point StemmedDrawingInterface::GetDrawingStemEnd(Object *object)
{
    assert(m_drawingStem || object);
    if (object && !m_drawingStem) {
        assert(this == dynamic_cast<StemmedDrawingInterface *>(object));
        if (!m_drawingStem) {
            // Somehow arbitrary for chord - stem end it the bottom with no stem
            if (object->Is(CHORD)) {
                Chord *chord = dynamic_cast<Chord *>(object);
                assert(chord);
                return Point(object->GetDrawingX(), chord->GetYBottom());
            }
            return Point(object->GetDrawingX(), object->GetDrawingY());
        }
    }
    return Point(m_drawingStem->GetDrawingX(), m_drawingStem->GetDrawingY() - GetDrawingStemLen());
}

} // namespace vrv
