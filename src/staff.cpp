/////////////////////////////////////////////////////////////////////////////
// Name:        staff.cpp
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "staff.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <iterator>
#include <vector>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "doc.h"
#include "editorial.h"
#include "functor.h"
#include "hairpin.h"
#include "keysig.h"
#include "layer.h"
#include "measure.h"
#include "note.h"
#include "page.h"
#include "staffdef.h"
#include "syl.h"
#include "system.h"
#include "timeinterface.h"
#include "tuning.h"
#include "verse.h"
#include "vrv.h"
#include "zone.h"

namespace vrv {

//----------------------------------------------------------------------------
// Staff
//----------------------------------------------------------------------------

static const ClassRegistrar<Staff> s_factory("staff", STAFF);

Staff::Staff(int n) : Object(STAFF), FacsimileInterface(), AttCoordY1(), AttNInteger(), AttTyped(), AttVisibility()
{
    this->RegisterAttClass(ATT_COORDY1);
    this->RegisterAttClass(ATT_NINTEGER);
    this->RegisterAttClass(ATT_TYPED);
    this->RegisterAttClass(ATT_VISIBILITY);
    this->RegisterInterface(FacsimileInterface::GetAttClasses(), FacsimileInterface::IsInterface());

    this->Reset();
    this->SetN(n);
}

Staff::~Staff() {}

void Staff::Reset()
{
    Object::Reset();
    FacsimileInterface::Reset();
    this->ResetCoordY1();
    this->ResetNInteger();
    this->ResetTyped();
    this->ResetVisibility();

    m_drawingFacsY = VRV_UNSET;

    m_drawingStaffSize = 100;
    m_drawingLines = 5;
    m_drawingNotationType = NOTATIONTYPE_NONE;
    m_staffAlignment = NULL;
    m_timeSpanningElements.clear();
    m_drawingStaffDef = NULL;
    m_drawingTuning = NULL;
    m_drawingRotation = 0.0;

    this->ClearLedgerLines();
}

void Staff::CloneReset()
{
    Object::CloneReset();

    m_drawingStaffSize = 100;
    m_drawingLines = 5;
    m_drawingNotationType = NOTATIONTYPE_NONE;
    m_staffAlignment = NULL;
    m_timeSpanningElements.clear();
    m_drawingStaffDef = NULL;
    m_drawingTuning = NULL;
    m_drawingRotation = 0.0;
}

int Staff::GetDrawingRotationOffsetFor(int x)
{
    int xDiff = x - this->GetDrawingX();
    return int(xDiff * tan(this->GetDrawingRotation() * M_PI / 180.0));
}

void Staff::ClearLedgerLines()
{
    m_ledgerLinesAbove.clear();
    m_ledgerLinesBelow.clear();
    m_ledgerLinesAboveCue.clear();
    m_ledgerLinesBelowCue.clear();
}

bool Staff::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ LAYER };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else if (Object::IsEditorialElement(classId)) {
        return true;
    }
    else {
        return false;
    }
}

bool Staff::AddChildAdditionalCheck(Object *child)
{
    if (child->Is(LAYER)) {
        Layer *layer = vrv_cast<Layer *>(child);
        assert(layer);
        if (layer && !layer->HasN()) {
            // This is not 100% safe if we have a <app> and <rdg> with more than
            // one layer as a previous child.
            layer->SetN(this->GetChildCount(LAYER) + 1);
        }
    }
    return (Object::AddChildAdditionalCheck(child));
}

int Staff::GetDrawingX() const
{
    if (this->HasFacs()) {
        const Doc *doc = vrv_cast<const Doc *>(this->GetFirstAncestor(DOC));
        assert(doc);
        if (doc->IsFacs()) {
            return FacsimileInterface::GetDrawingX();
        }
    }
    return Object::GetDrawingX();
}

int Staff::GetDrawingY() const
{

    if (m_drawingFacsY != VRV_UNSET) return m_drawingFacsY;

    if (!m_staffAlignment) return 0;

    if (m_cachedDrawingY != VRV_UNSET) return m_cachedDrawingY;

    const System *system = vrv_cast<const System *>(this->GetFirstAncestor(SYSTEM));
    assert(system);

    m_cachedDrawingY = system->GetDrawingY() + m_staffAlignment->GetYRel();
    return m_cachedDrawingY;
}

double Staff::GetDrawingRotate() const
{
    if (this->HasFacs()) {
        const Doc *doc = vrv_cast<const Doc *>(this->GetFirstAncestor(DOC));
        assert(doc);
        if (doc->IsFacs() || doc->IsTranscription()) {
            return FacsimileInterface::GetDrawingRotate();
        }
    }
    return 0;
}

void Staff::AdjustDrawingStaffSize()
{
    if (this->HasFacs()) {
        Doc *doc = vrv_cast<Doc *>(this->GetFirstAncestor(DOC));
        assert(doc);
        if (doc->IsFacs() || doc->IsNeumeLines()) {
            double rotate = this->GetDrawingRotate();
            Zone *zone = this->GetZone();
            assert(zone);
            int yDiff
                = zone->GetLry() - zone->GetUly() - (zone->GetLrx() - zone->GetUlx()) * tan(abs(rotate) * M_PI / 180.0);
            m_drawingStaffSize = 100 * yDiff / (doc->GetOptions()->m_unit.GetValue() * 2 * (m_drawingLines - 1));
        }
    }
}

int Staff::GetDrawingStaffNotationSize() const
{
    if (this->IsTabLuteGerman()) return m_drawingStaffSize / GERMAN_TAB_STAFF_RATIO;

    return (this->IsTablature()) ? m_drawingStaffSize / TABLATURE_STAFF_RATIO : m_drawingStaffSize;
}

bool Staff::DrawingIsVisible() const
{
    const System *system = vrv_cast<const System *>(this->GetFirstAncestor(SYSTEM));
    assert(system);
    assert(system->GetDrawingScoreDef());

    const StaffDef *staffDef = system->GetDrawingScoreDef()->GetStaffDef(this->GetN());
    assert(staffDef);
    return (staffDef->GetDrawingVisibility() != OPTIMIZATION_HIDDEN);
}

bool Staff::IsMensural() const
{
    bool isMensural
        = (m_drawingNotationType == NOTATIONTYPE_mensural || m_drawingNotationType == NOTATIONTYPE_mensural_white
            || m_drawingNotationType == NOTATIONTYPE_mensural_black);
    return isMensural;
}

bool Staff::IsNeume() const
{
    bool isNeume = (m_drawingNotationType == NOTATIONTYPE_neume);
    return isNeume;
}

bool Staff::IsTablature() const
{
    bool isTablature = (m_drawingNotationType == NOTATIONTYPE_tab || m_drawingNotationType == NOTATIONTYPE_tab_guitar
        || m_drawingNotationType == NOTATIONTYPE_tab_lute_italian
        || m_drawingNotationType == NOTATIONTYPE_tab_lute_french
        || m_drawingNotationType == NOTATIONTYPE_tab_lute_german);
    return isTablature;
}

bool Staff::IsTabWithStemsOutside() const
{
    if (!m_drawingStaffDef) return false;
    // Temporary implementation looking at staffDef@type
    return (!this->IsTabGuitar() || !m_drawingStaffDef->HasType() || m_drawingStaffDef->GetType() != "stems.within");
}

int Staff::CalcPitchPosYRel(const Doc *doc, int loc) const
{
    assert(doc);

    // the staff loc offset is based on the number of lines: 0 with 1 line, 2 with 2, etc
    int staffLocOffset = (m_drawingLines - 1) * 2;
    return (loc - staffLocOffset) * doc->GetDrawingUnit(m_drawingStaffSize);
}

void Staff::AddLedgerLineAbove(int count, int left, int right, int extension, bool cueSize, const Object *event)
{
    this->AddLedgerLines(cueSize ? m_ledgerLinesAboveCue : m_ledgerLinesAbove, count, left, right, extension, event);
}

void Staff::AddLedgerLineBelow(int count, int left, int right, int extension, bool cueSize, const Object *event)
{
    this->AddLedgerLines(cueSize ? m_ledgerLinesBelowCue : m_ledgerLinesBelow, count, left, right, extension, event);
}

void Staff::AddLedgerLines(
    ArrayOfLedgerLines &lines, int count, int left, int right, int extension, const Object *event)
{
    assert(left < right);

    if ((int)lines.size() < count) lines.resize(count);
    for (int i = 0; i < count; ++i) {
        lines.at(i).AddDash(left, right, extension, event);
    }
}

void Staff::SetFromFacsimile(Doc *doc)
{
    if (!this->HasFacs()) return;
    if (this->GetZone() == NULL) {
        assert(doc);
        Zone *zone = doc->GetFacsimile()->FindZoneByID(this->GetFacs());
        assert(zone);
        this->AttachZone(zone);
    }
    this->AdjustDrawingStaffSize();
}

bool Staff::IsOnStaffLine(int y, const Doc *doc) const
{
    assert(doc);

    return ((y - this->GetDrawingY()) % (2 * doc->GetDrawingUnit(m_drawingStaffSize)) == 0);
}

int Staff::GetNearestInterStaffPosition(int y, const Doc *doc, data_STAFFREL place) const
{
    assert(doc);

    int yPos = y - this->GetDrawingY();
    int distance = yPos % doc->GetDrawingUnit(m_drawingStaffSize);
    if (place == STAFFREL_above) {
        if (distance > 0) distance = doc->GetDrawingUnit(m_drawingStaffSize) - distance;
        return y - distance + doc->GetDrawingUnit(m_drawingStaffSize);
    }
    else {
        if (distance < 0) distance = doc->GetDrawingUnit(m_drawingStaffSize) + distance;
        return y - distance - doc->GetDrawingUnit(m_drawingStaffSize);
    }
}

//----------------------------------------------------------------------------
// LedgerLine
//----------------------------------------------------------------------------

void LedgerLine::AddDash(int left, int right, int extension, const Object *event)
{
    assert(left < right);

    std::list<LedgerLine::Dash>::iterator iter;

    // First add the dash
    for (iter = m_dashes.begin(); iter != m_dashes.end(); ++iter) {
        if (iter->m_x1 > left) break;
    }
    m_dashes.insert(iter, LedgerLine::Dash(left, right, event));

    // Merge dashes which overlap by more than 1.5 extensions
    // => Dashes belonging to the same chord overlap at least by two extensions and will get merged
    // => Overlapping dashes of adjacent notes will not get merged
    std::list<LedgerLine::Dash>::iterator previous = m_dashes.begin();
    iter = m_dashes.begin();
    ++iter;
    while (iter != m_dashes.end()) {
        if (previous->m_x2 > iter->m_x1 + 1.5 * extension) {
            previous->MergeWith(*iter);
            iter = m_dashes.erase(iter);
        }
        else {
            previous = iter;
            ++iter;
        }
    }
}

//----------------------------------------------------------------------------
// Staff functor methods
//----------------------------------------------------------------------------

FunctorCode Staff::Accept(Functor &functor)
{
    return functor.VisitStaff(this);
}

FunctorCode Staff::Accept(ConstFunctor &functor) const
{
    return functor.VisitStaff(this);
}

FunctorCode Staff::AcceptEnd(Functor &functor)
{
    return functor.VisitStaffEnd(this);
}

FunctorCode Staff::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitStaffEnd(this);
}

} // namespace vrv
