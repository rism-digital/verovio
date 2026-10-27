/////////////////////////////////////////////////////////////////////////////
// Name:        editfunctor.cpp
// Author:      Laurent Pugin
// Created:     2025
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "editfunctor.h"

//----------------------------------------------------------------------------

#include "controlelement.h"
#include "cursor.h"
#include "doc.h"
#include "editorial.h"
#include "editortoolkit_cmn.h"
#include "layer.h"
#include "measure.h"
#include "score.h"
#include "scoredef.h"
#include "staff.h"
#include "staffdef.h"
#include "staffgrp.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AddLayerFunctor
//----------------------------------------------------------------------------

AddLayerFunctor::AddLayerFunctor(int n) : Functor()
{
    m_n = n;
    m_layerN = 1;
}

AddLayerFunctor::~AddLayerFunctor() {}

FunctorCode AddLayerFunctor::VisitDoc(Doc *doc)
{
    for (const Object *object : doc->FindAllDescendantsByType(STAFF, false)) {
        const Staff *staff = vrv_cast<const Staff *>(object);
        if (staff->GetN() != m_n) continue;

        for (const Object *layerObject : staff->FindAllDescendantsByType(LAYER, false)) {
            const Layer *layer = vrv_cast<const Layer *>(layerObject);
            if (layer->HasN()) m_layerN = std::max(m_layerN, layer->GetN() + 1);
        }
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode AddLayerFunctor::VisitStaff(Staff *staff)
{
    if (staff->GetN() != m_n) return FUNCTOR_SIBLINGS;

    Layer *layer = new Layer();
    layer->SetN(m_layerN);
    staff->AddChild(layer);

    return FUNCTOR_SIBLINGS;
}

//----------------------------------------------------------------------------
// DeleteLayerFunctor
//----------------------------------------------------------------------------

DeleteLayerFunctor::DeleteLayerFunctor(int staffN, int layerN) : Functor()
{
    m_staffN = staffN;
    m_layerN = layerN;
    m_canDelete = true;
}

DeleteLayerFunctor::~DeleteLayerFunctor() {}

FunctorCode DeleteLayerFunctor::VisitDoc(Doc *doc)
{
    for (Object *object : doc->FindAllDescendantsByType(STAFF, false)) {
        Staff *staff = vrv_cast<Staff *>(object);
        if (staff->GetN() != m_staffN) continue;

        const ListOfObjects layers = staff->FindAllDescendantsByType(LAYER, false);
        const auto layer = std::find_if(layers.begin(), layers.end(), [this](const Object *object) {
            const Layer *layer = vrv_cast<const Layer *>(object);
            return layer->GetN() == m_layerN;
        });
        if ((layer != layers.end()) && (layers.size() < 2)) {
            m_canDelete = false;
            break;
        }
    }

    return m_canDelete ? FUNCTOR_CONTINUE : FUNCTOR_STOP;
}

FunctorCode DeleteLayerFunctor::VisitStaff(Staff *staff)
{
    if (staff->GetN() != m_staffN) return FUNCTOR_SIBLINGS;

    Layer *layerToDelete = NULL;
    for (Object *object : staff->FindAllDescendantsByType(LAYER, false)) {
        Layer *layer = vrv_cast<Layer *>(object);
        if (layer->GetN() == m_layerN) {
            layerToDelete = layer;
        }
        else if (layer->GetN() > m_layerN) {
            layer->SetN(layer->GetN() - 1);
        }
    }

    if (layerToDelete) {
        Object *parent = layerToDelete->GetParent();
        assert(parent);
        parent->DeleteChild(layerToDelete);
    }

    return FUNCTOR_SIBLINGS;
}

//----------------------------------------------------------------------------
// AddStaffFunctor
//----------------------------------------------------------------------------

AddStaffFunctor::AddStaffFunctor(int n, StaffInsert insert) : Functor()
{
    m_n = n;
    m_insert = insert;
    m_staff = NULL;
    m_staffDef = NULL;
}

AddStaffFunctor::~AddStaffFunctor()
{
    if (m_staff) delete m_staff;
    if (m_staffDef) delete m_staffDef;
}

FunctorCode AddStaffFunctor::VisitScore(Score *score)
{
    if (m_staffDef) return FUNCTOR_SIBLINGS;

    ScoreDef *scoreDef = score->GetScoreDef();
    assert(scoreDef);

    std::vector<int> staffNs = scoreDef->GetStaffNs();
    int n = *std::max_element(staffNs.begin(), staffNs.end()) + 1;

    m_staff = new Staff(n);
    Layer *layer = new Layer();
    layer->SetN(1);
    m_staff->AddChild(layer);

    StaffDef *staffDef = scoreDef->GetStaffDef(m_n);
    m_staffDef = vrv_cast<StaffDef *>(staffDef->Clone());
    m_staffDef->CloneReset();
    m_staffDef->SetN(n);

    StaffDef *newStaffDef = vrv_cast<StaffDef *>(m_staffDef->Clone());
    newStaffDef->CloneReset();

    int offset = (m_insert == StaffInsert::INSERT_BELOW) ? 1 : 0;

    Object *parent = staffDef->GetParent();
    parent->InsertChild(newStaffDef, staffDef->GetIdx() + offset);
    scoreDef->Modify();

    return FUNCTOR_SIBLINGS;
}

FunctorCode AddStaffFunctor::VisitMeasure(Measure *measure)
{
    AttNIntegerComparison staffN(STAFF, m_n);
    Staff *staff = vrv_cast<Staff *>(measure->FindDescendantByComparison(&staffN));
    if (!staff) return FUNCTOR_SIBLINGS;

    Staff *newStaff = vrv_cast<Staff *>(m_staff->Clone());
    newStaff->CloneReset();

    int offset = (m_insert == StaffInsert::INSERT_BELOW) ? 1 : 0;

    Object *parent = staff->GetParent();
    parent->InsertChild(newStaff, staff->GetIdx() + offset);

    return FUNCTOR_SIBLINGS;
}

FunctorCode AddStaffFunctor::VisitScoreDef(ScoreDef *scoreDef)
{
    if (!m_staffDef) return FUNCTOR_SIBLINGS;

    StaffDef *staffDef = scoreDef->GetStaffDef(m_n);

    StaffDef *newStaffDef = vrv_cast<StaffDef *>(staffDef->Clone());
    newStaffDef->CloneReset();
    newStaffDef->SetN(m_staffDef->GetN());

    int offset = (m_insert == StaffInsert::INSERT_BELOW) ? 1 : 0;

    Object *parent = staffDef->GetParent();
    parent->InsertChild(newStaffDef, staffDef->GetIdx() + offset);

    return FUNCTOR_SIBLINGS;
}

//----------------------------------------------------------------------------
// DeleteStaffFunctor
//----------------------------------------------------------------------------

DeleteStaffFunctor::DeleteStaffFunctor(int n) : Functor()
{
    m_n = n;
    m_scoreDefProcessed = false;
}

DeleteStaffFunctor::~DeleteStaffFunctor() {}

FunctorCode DeleteStaffFunctor::VisitScore(Score *score)
{
    if (m_scoreDefProcessed) return FUNCTOR_SIBLINGS;

    ScoreDef *scoreDef = score->GetScoreDef();
    assert(scoreDef);

    std::vector<int> staffNs = scoreDef->GetStaffNs();
    // Make sure we keep at least one staff in the score
    if (staffNs.size() < 2) return FUNCTOR_STOP;

    this->DeleteStaffDef(scoreDef);
    m_scoreDefProcessed = true;

    scoreDef->Modify();

    return FUNCTOR_SIBLINGS;
}

FunctorCode DeleteStaffFunctor::VisitMeasure(Measure *measure)
{
    AttNIntegerComparison staffN(STAFF, m_n);
    Staff *staff = vrv_cast<Staff *>(measure->FindDescendantByComparison(&staffN));
    if (!staff) return FUNCTOR_SIBLINGS;

    m_objectsToDelete.insert(staff->GetID());

    Doc *doc = vrv_cast<Doc *>(staff->GetFirstAncestor(DOC));
    assert(doc);
    SetOfConstObjects visited;
    doc->CollectReferringObjects(staff, m_objectsToDelete, visited);

    return FUNCTOR_SIBLINGS;
}

FunctorCode DeleteStaffFunctor::VisitScoreDef(ScoreDef *scoreDef)
{
    if (!m_scoreDefProcessed) return FUNCTOR_SIBLINGS;

    this->DeleteStaffDef(scoreDef);

    return FUNCTOR_SIBLINGS;
}

void DeleteStaffFunctor::DeleteStaffDef(ScoreDef *scoreDef)
{
    StaffDef *staffDef = scoreDef->GetStaffDef(m_n);
    if (!staffDef) return;

    Object *parent = staffDef->GetParent();
    assert(parent);
    if (parent->Is(STAFFGRP) && (parent->GetDescendantCount(STAFFDEF) == 1)) {
        Object *grandParent = parent->GetParent();
        assert(grandParent);
        grandParent->DeleteChild(parent);
    }
    else {
        parent->DeleteChild(staffDef);
    }
}

//----------------------------------------------------------------------------
// MoveStaffFunctor
//----------------------------------------------------------------------------

MoveStaffFunctor::MoveStaffFunctor(int n, StaffMove move) : Functor()
{
    m_n = n;
    m_move = move;
    m_nTarget = VRV_UNSET;
}

MoveStaffFunctor::~MoveStaffFunctor() {}

FunctorCode MoveStaffFunctor::VisitScore(Score *score)
{
    if (m_nTarget != VRV_UNSET) return FUNCTOR_SIBLINGS;

    ScoreDef *scoreDef = score->GetScoreDef();
    assert(scoreDef);

    std::vector<int> staffNs = scoreDef->GetStaffNs();

    const auto staffN = std::find(staffNs.begin(), staffNs.end(), m_n);
    if (staffN == staffNs.end()) return FUNCTOR_SIBLINGS;

    if (m_move == StaffMove::MOVE_UP) {
        if (staffN == staffNs.begin()) return FUNCTOR_SIBLINGS;
        m_nTarget = *std::prev(staffN);
    }
    else {
        const auto nextStaffN = std::next(staffN);
        if (nextStaffN == staffNs.end()) return FUNCTOR_SIBLINGS;
        m_nTarget = *nextStaffN;
    }

    StaffDef *staffDef = scoreDef->GetStaffDef(m_n);
    StaffDef *targetStaffDef = scoreDef->GetStaffDef(m_nTarget);
    assert(staffDef && targetStaffDef);
    staffDef->SwapWith(targetStaffDef);

    return FUNCTOR_SIBLINGS;
}

FunctorCode MoveStaffFunctor::VisitMeasure(Measure *measure)
{
    if (m_nTarget == VRV_UNSET) return FUNCTOR_SIBLINGS;

    AttNIntegerComparison staffN(STAFF, m_n);
    Staff *staff = vrv_cast<Staff *>(measure->FindDescendantByComparison(&staffN));
    AttNIntegerComparison targetStaffN(STAFF, m_nTarget);
    Staff *staffTarget = vrv_cast<Staff *>(measure->FindDescendantByComparison(&targetStaffN));
    if (!staff || !staffTarget) return FUNCTOR_SIBLINGS;
    staff->SwapWith(staffTarget);

    return FUNCTOR_SIBLINGS;
}

FunctorCode MoveStaffFunctor::VisitScoreDef(ScoreDef *scoreDef)
{
    if (m_nTarget == VRV_UNSET) return FUNCTOR_SIBLINGS;

    StaffDef *staffDef = scoreDef->GetStaffDef(m_n);
    StaffDef *targetStaffDef = scoreDef->GetStaffDef(m_nTarget);
    if (!staffDef || !targetStaffDef) return FUNCTOR_SIBLINGS;
    staffDef->SwapWith(targetStaffDef);

    return FUNCTOR_SIBLINGS;
}

//----------------------------------------------------------------------------
// UpdateStaffGrpFunctor
//----------------------------------------------------------------------------

UpdateStaffGrpFunctor::UpdateStaffGrpFunctor(Doc *doc, ScoreDef *mainScoreDef, StaffGrp *staffGrp) : DocFunctor(doc)
{
    assert(mainScoreDef);
    assert(staffGrp);

    m_mainScoreDef = mainScoreDef;
    // The new staffGrp now set in the main scoreDef - make a copy as a template
    m_staffGrpTemplate = vrv_cast<StaffGrp *>(staffGrp->Clone());
    m_staffGrpTemplate->CloneReset();
    m_success = true;
    this->SetVisibleOnly(false);

    // Check it actually is in the main scoreDef
    if (staffGrp != m_mainScoreDef->FindDescendantByType(STAFFGRP)) {
        LogError("The updated staffGrp must be the first staffGrp of the main scoreDef");
        m_success = false;
        return;
    }

    // Build a mapping
    const ListOfObjects staffDefs = staffGrp->FindAllDescendantsByType(STAFFDEF);
    int position = 0;
    for (const Object *object : staffDefs) {
        const StaffDef *staffDef = vrv_cast<const StaffDef *>(object);
        assert(staffDef);
        if (!staffDef->HasN() || !m_staffPositions.emplace(staffDef->GetN(), position).second) {
            LogError("The updated staffGrp must contain one staffDef for each unique @n");
            m_success = false;
            break;
        }
        ++position;
    }
    if (m_staffPositions.empty()) {
        LogError("The updated staffGrp must contain at least one staffDef");
        m_success = false;
    }
}

UpdateStaffGrpFunctor::~UpdateStaffGrpFunctor()
{
    delete m_staffGrpTemplate;
}

FunctorCode UpdateStaffGrpFunctor::VisitMeasure(Measure *measure)
{
    if (!m_success || m_staffPositions.empty()) return FUNCTOR_STOP;

    int firstStaffIdx = VRV_UNSET;
    if (!this->GetStaffRange(measure, firstStaffIdx)) {
        m_success = false;
        return FUNCTOR_STOP;
    }

    std::vector<Staff *> orderedStaves(m_staffPositions.size(), NULL);
    for (int i = 0; i < (int)m_staffPositions.size(); ++i) {
        Staff *staff = vrv_cast<Staff *>(measure->GetChild(firstStaffIdx + i));
        assert(staff);
        orderedStaves.at(m_staffPositions.at(staff->GetN())) = staff;
    }

    ArrayOfObjects &children = measure->GetChildrenForModification();
    std::copy(orderedStaves.begin(), orderedStaves.end(), children.begin() + firstStaffIdx);
    measure->Modify();

    return FUNCTOR_SIBLINGS;
}

FunctorCode UpdateStaffGrpFunctor::VisitScoreDef(ScoreDef *scoreDef)
{
    if (!m_success || m_staffPositions.empty()) return FUNCTOR_STOP;

    StaffGrp *staffGrp = vrv_cast<StaffGrp *>(m_staffGrpTemplate->Clone());
    staffGrp->CloneReset();
    if (!this->UpdateScoreDef(scoreDef, staffGrp)) {
        if (!staffGrp->GetParent()) delete staffGrp;
        m_success = false;
        return FUNCTOR_STOP;
    }

    return FUNCTOR_SIBLINGS;
}

bool UpdateStaffGrpFunctor::GetStaffRange(const Measure *measure, int &firstStaffIdx) const
{
    firstStaffIdx = VRV_UNSET;
    int staffCount = 0;
    bool staffRangeEnded = false;
    std::set<int> staffNs;

    for (int i = 0; i < measure->GetChildCount(); ++i) {
        const Object *child = measure->GetChild(i);
        if (child->Is(STAFF)) {
            if (staffRangeEnded) {
                LogError("The staves in a measure must form one contiguous child range");
                return false;
            }
            if (firstStaffIdx == VRV_UNSET) firstStaffIdx = i;
            const Staff *staff = vrv_cast<const Staff *>(child);
            assert(staff);
            if (!m_staffPositions.count(staff->GetN()) || !staffNs.insert(staff->GetN()).second) {
                LogError("The staves in a measure do not match the updated staffGrp");
                return false;
            }
            ++staffCount;
        }
        else if (firstStaffIdx != VRV_UNSET) {
            staffRangeEnded = true;
        }
    }

    if ((staffCount != (int)m_staffPositions.size()) || (staffNs.size() != m_staffPositions.size())) {
        LogError("The number of staves in a measure does not match the updated staffGrp");
        return false;
    }

    return true;
}

bool UpdateStaffGrpFunctor::UpdateScoreDef(ScoreDef *scoreDef, StaffGrp *staffGrp)
{
    StaffGrp *oldStaffGrp = vrv_cast<StaffGrp *>(scoreDef->FindDescendantByType(STAFFGRP));
    if (!oldStaffGrp) return false;

    const ListOfObjects newStaffDefList = staffGrp->FindAllDescendantsByType(STAFFDEF);
    if (newStaffDefList.size() != m_staffPositions.size()) return false;

    std::vector<StaffDef *> newStaffDefs;
    for (Object *object : newStaffDefList) newStaffDefs.push_back(vrv_cast<StaffDef *>(object));

    std::vector<std::pair<StaffDef *, StaffDef *>> staffDefs;
    staffDefs.reserve(m_staffPositions.size());

    for (const auto &[staffN, position] : m_staffPositions) {
        StaffDef *oldStaffDef = scoreDef->GetStaffDef(staffN);
        StaffDef *newStaffDef = vrv_cast<StaffDef *>(newStaffDefs.at(position));
        if (!oldStaffDef || !newStaffDef || (newStaffDef->GetN() != staffN)) return false;
        staffDefs.emplace_back(oldStaffDef, newStaffDef);
    }

    for (const auto &[oldStaffDef, newStaffDef] : staffDefs) oldStaffDef->SwapWith(newStaffDef);

    Object *parent = oldStaffGrp->GetParent();
    assert(parent);
    parent->ReplaceChild(oldStaffGrp, staffGrp);
    delete oldStaffGrp;

    return true;
}

//----------------------------------------------------------------------------
// ReorderStaffNFunctor
//----------------------------------------------------------------------------

ReorderStaffNFunctor::ReorderStaffNFunctor() : Functor() {}

ReorderStaffNFunctor::~ReorderStaffNFunctor() {}

FunctorCode ReorderStaffNFunctor::VisitControlElement(ControlElement *controlElement)
{
    if (!controlElement->HasAttClass(ATT_STAFFIDENT)) return FUNCTOR_CONTINUE;

    this->MapStaffIdent(dynamic_cast<AttStaffIdent *>(controlElement));

    return FUNCTOR_CONTINUE;
}

FunctorCode ReorderStaffNFunctor::VisitLayerElement(LayerElement *layerElement)
{
    if (!layerElement->HasAttClass(ATT_STAFFIDENT)) return FUNCTOR_CONTINUE;

    this->MapStaffIdent(dynamic_cast<AttStaffIdent *>(layerElement));

    return FUNCTOR_CONTINUE;
}

FunctorCode ReorderStaffNFunctor::VisitScore(Score *score)
{
    if (!m_mapping.empty()) return FUNCTOR_SIBLINGS;

    ScoreDef *scoreDef = score->GetScoreDef();
    assert(scoreDef);

    std::vector<int> staffNs = scoreDef->GetStaffNs();
    int i = 1;
    for (auto staffN : staffNs) {
        m_mapping[staffN] = i++;
    }
    if (staffNs.size() != m_mapping.size()) {
        LogError("Count mismatch when reordering staff @n");
        return FUNCTOR_STOP;
    }

    // No change in the mapping, nothing to do
    if (std::all_of(m_mapping.begin(), m_mapping.end(), [](const auto &p) { return p.first == p.second; }))
        return FUNCTOR_STOP;

    scoreDef->Process(*this);

    return FUNCTOR_SIBLINGS;
}

FunctorCode ReorderStaffNFunctor::VisitStaff(Staff *staff)
{
    staff->SetN(m_mapping.at(staff->GetN()));

    return FUNCTOR_CONTINUE;
}

FunctorCode ReorderStaffNFunctor::VisitStaffDef(StaffDef *staffDef)
{
    staffDef->SetN(m_mapping.at(staffDef->GetN()));

    return FUNCTOR_SIBLINGS;
}

void ReorderStaffNFunctor::MapStaffIdent(AttStaffIdent *att)
{
    if (!att) return;

    xsdPositiveInteger_List mappedValues;
    for (auto staffN : att->GetStaff()) mappedValues.push_back(m_mapping.at(staffN));
    att->SetStaff(mappedValues);
}

//----------------------------------------------------------------------------
// CursorFunctor
//----------------------------------------------------------------------------

CursorFunctor::CursorFunctor(Layer *layer, LayerElement *position) : Functor()
{
    m_layer = layer;
    m_position = position;
    m_cursor = NULL;
    m_previous = NULL;
}

CursorFunctor::~CursorFunctor()
{
    if (m_previous) delete m_previous;
}

FunctorCode CursorFunctor::VisitLayer(Layer *layer)
{
    if (layer->HasCursor()) {
        // Store the values for when moving to the next layer
        m_previous = vrv_cast<Cursor *>(layer->GetCursor()->Clone());
        m_previous->CloneReset();
    }

    if (layer != m_layer) {
        layer->ResetCursor();
    }
    else {
        if (m_previous) {
            m_cursor = vrv_cast<Cursor *>(m_previous->Clone());
            m_cursor->CloneReset();
        }
        else {
            m_cursor = new Cursor();
            // Default it G2 clef
            int clefLocOffset = (layer->GetCurrentClef()) ? layer->GetCurrentClef()->GetClefLocOffset() : -4;
            // Place it on loc 4 (middle line assuming five lines for now)
            auto [pname, oct] = PitchInterface::CalcPitch(4, clefLocOffset);
            m_cursor->SetPname(pname);
            m_cursor->SetOct(oct);
        }
        assert(m_cursor);
        if (m_position) {
            m_cursor->SetPosition(m_position);
            if (m_position->Is(NOTE)) {
                const Note *note = vrv_cast<const Note *>(m_position);
                assert(note);
                m_cursor->SetPname(note->GetPname());
                m_cursor->SetOct(note->GetOct());
                m_cursor->SetAccidValue(note->GetDrawingAccid());
            }
        }
        layer->SetCursor(m_cursor);
    }

    return FUNCTOR_SIBLINGS;
}

//----------------------------------------------------------------------------
// SectionContextFunctor
//----------------------------------------------------------------------------

SectionContextFunctor::SectionContextFunctor(Object *object) : Functor()
{
    m_current = object;
}

FunctorCode SectionContextFunctor::VisitObject(Object *object)
{
    // In any case do not go beyond these
    if (object->GetParent() && object->GetParent()->IsAnyOf(std::array{ DIV, MEASURE, SCOREDEF })) {
        return FUNCTOR_SIBLINGS;
    }

    if (object->IsAnyOf(std::array{ DOC, PAGE, PAGES, PAGE_MILESTONE_END, SYSTEM, SYSTEM_MILESTONE_END })) {
        return FUNCTOR_CONTINUE;
    }

    bool ownChildren = false;
    if (object->IsAnyOf(std::array{ DIV, MEASURE, SCOREDEF })) {
        ownChildren = true;
    }

    EditorTreeObject *treeObject = new EditorTreeObject(object, ownChildren);
    m_current->AddChild(treeObject);
    m_current = treeObject;

    return FUNCTOR_CONTINUE;
}

FunctorCode SectionContextFunctor::VisitObjectEnd(Object *object)
{
    if (object->IsMilestoneElement()) {
        return FUNCTOR_CONTINUE;
    }

    if (object->IsAnyOf(std::array{ PAGE_MILESTONE_END, SYSTEM_MILESTONE_END })) {
        m_current = m_current->GetParent();
    }
    if (object->IsAnyOf(std::array{ DOC, PAGE, PAGES, PAGE_MILESTONE_END, SYSTEM, SYSTEM_MILESTONE_END })) {
        return FUNCTOR_CONTINUE;
    }

    m_current = m_current->GetParent();

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// ScoreContextFunctor
//----------------------------------------------------------------------------

ScoreContextFunctor::ScoreContextFunctor(Object *object) : Functor()
{
    // m_stack.push_back(node);
    m_current = object;

    m_inScoreLevel = NOT_IN_SCORE;
    this->SetVisibleOnly(false);
}

FunctorCode ScoreContextFunctor::VisitObject(Object *object)
{
    // In any case do not go beyond these
    if (object->GetParent() && object->GetParent()->IsAnyOf(std::array{ DIV, MEASURE, SCOREDEF })) {
        return FUNCTOR_SIBLINGS;
    }
    if (m_inScoreLevel == INCLUDED) {
        // Just return here, the level will be set back in VisitObjectEnd
        return FUNCTOR_SIBLINGS;
    }

    // Do not include in the tree
    if ((m_inScoreLevel == NOT_IN_SCORE) && !object->IsAnyOf(std::array{ MDIV, SCORE })) {
        return FUNCTOR_CONTINUE;
    }

    bool ownChildren = false;
    // The first one in the scoreDef subtree - simply use own children and stop
    if (m_inScoreLevel == TO_INCLUDE) {
        ownChildren = true;
    }

    EditorTreeObject *treeObject = new EditorTreeObject(object, ownChildren);
    treeObject->SetVisibility(Visible);
    m_current->AddChild(treeObject);
    m_current = treeObject;

    if (object->Is(SCORE)) {
        Score *score = vrv_cast<Score *>(object);
        assert(score);
        // We are now in the score and process the scoreDef subtree
        // Include the first one in the context tree - and then own children
        m_inScoreLevel = TO_INCLUDE;
        score->GetScoreDef()->Process(*this);
        m_inScoreLevel = NOT_IN_SCORE;
    }

    if (m_inScoreLevel == TO_INCLUDE) m_inScoreLevel = INCLUDED;

    return FUNCTOR_CONTINUE;
}

FunctorCode ScoreContextFunctor::VisitObjectEnd(Object *object)
{
    if (object->IsMilestoneElement()) {
        return FUNCTOR_CONTINUE;
    }
    // End milestone for mdiv or score - pop it
    if (object->Is(PAGE_MILESTONE_END)) {
        m_current = m_current->GetParent();
    }
    // The have not been pushed, continue
    if ((m_inScoreLevel == NOT_IN_SCORE) && !object->IsAnyOf(std::array{ MDIV, SCORE })) {
        return FUNCTOR_CONTINUE;
    }

    m_current = m_current->GetParent();

    // We included the first one of the scoreDef substree with its own children, we should stop adding them
    if (m_inScoreLevel == INCLUDED) m_inScoreLevel = NOT_IN_SCORE;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
