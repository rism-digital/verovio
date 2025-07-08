/////////////////////////////////////////////////////////////////////////////
// Name:        editortoolkit_neume.cpp
// Author:      Laurent Pugin, Juliette Regimbal, Zoe McLennan
// Created:     04/06/2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "editortoolkit_neume.h"

//--------------------------------------------------------------------------------

#include <algorithm>
#include <iterator>
#include <limits>
#include <locale>
#include <math.h>
#include <set>

//--------------------------------------------------------------------------------

#include "accid.h"
#include "clef.h"
#include "comparison.h"
#include "custos.h"
#include "divline.h"
#include "layer.h"
#include "liquescent.h"
#include "measure.h"
#include "nc.h"
#include "neume.h"
#include "oriscus.h"
#include "page.h"
#include "quilisma.h"
#include "rend.h"
#include "sb.h"
#include "score.h"
#include "staff.h"
#include "staffdef.h"
#include "surface.h"
#include "syl.h"
#include "syllable.h"
#include "system.h"
#include "text.h"
#include "vrv.h"
//--------------------------------------------------------------------------------

namespace vrv {
std::string EditorToolkitNeume::EditInfo()
{
    return m_editInfo.json();
}

bool EditorToolkitNeume::ParseEditorAction(const std::string &json_editorAction)
{
    jsonxx::Object json;
    m_editInfo.reset();

    // Read JSON actions
    if (!json.parse(json_editorAction)) {
        LogError("Cannot parse JSON std::string.");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Cannot parse JSON from std::string " + json_editorAction);
        return false;
    }

    if (!json.has<jsonxx::String>("action")
        || (!json.has<jsonxx::Object>("param") && !json.has<jsonxx::Array>("param"))) {
        LogWarning("Incorrectly formatted JSON action");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "JSON action misformatted.");
        return false;
    }

    std::string action = json.get<jsonxx::String>("action");

    if (action != "chain" && json.has<jsonxx::Array>("param")) {
        LogWarning("Only 'chain' uses 'param' as an array.");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "'param' can only be an array for a chain action.");
        return false;
    }
    if (action == "addSyl") {
        std::string elementId, sylText;
        if (this->ParseAddSylAction(json.get<jsonxx::Object>("param"), &elementId, &sylText)) {
            return this->AddSyl(elementId, sylText);
        }
        LogWarning("Could not parse the addSyl action");
    }
    else if (action == "drag") {
        std::string elementId;
        int x, y;
        if (this->ParseDragAction(json.get<jsonxx::Object>("param"), &elementId, &x, &y)) {
            return this->Drag(elementId, x, y);
        }
        LogWarning("Could not parse the drag action");
    }
    else if (action == "insert") {
        std::string elementType, startId, endId, staffId;
        int ulx = 0, uly = 0, lrx = 0, lry = 0;
        std::vector<std::pair<std::string, std::string>> attributes;
        if (this->ParseInsertAction(
                json.get<jsonxx::Object>("param"), &elementType, &staffId, &ulx, &uly, &lrx, &lry, &attributes)) {
            return this->Insert(elementType, staffId, ulx, uly, lrx, lry, attributes);
        }
        LogWarning("Could not parse the insert action");
    }
    else if (action == "insertToSyllable") {
        std::string elementId;
        if (this->ParseInsertToSyllableAction(json.get<jsonxx::Object>("param"), &elementId)) {
            return this->InsertToSyllable(elementId);
        }
        LogWarning("Could not parse the insert action");
    }
    else if (action == "moveOutsideSyllable") {
        std::string elementId;
        if (this->ParseMoveOutsideSyllableAction(json.get<jsonxx::Object>("param"), &elementId)) {
            return this->MoveOutsideSyllable(elementId);
        }
        LogWarning("Could not parse the insert action");
    }
    else if (action == "displaceClefOctave") {
        std::string elementId, direction;
        if (this->ParseDisplaceClefAction(json.get<jsonxx::Object>("param"), &elementId, &direction)) {
            return this->DisplaceClefOctave(elementId, direction);
        }
        LogWarning("Could not parse the displace clef octave action");
    }
    else if (action == "set") {
        std::string elementId, attrType, attrValue;
        if (this->ParseSetAction(json.get<jsonxx::Object>("param"), &elementId, &attrType, &attrValue)) {
            return this->Set(elementId, attrType, attrValue);
        }
        LogWarning("Could not parse the set action");
    }
    else if (action == "setText") {
        std::string elementId, text;
        if (this->ParseSetTextAction(json.get<jsonxx::Object>("param"), &elementId, &text)) {
            return this->SetText(elementId, text);
        }
        LogWarning("Could not parse the set text action");
    }
    else if (action == "setClef") {
        std::string elementId, shape;
        if (this->ParseSetClefAction(json.get<jsonxx::Object>("param"), &elementId, &shape)) {
            return this->SetClef(elementId, shape);
        }
        LogWarning("Could not parse the set clef action");
    }
    else if (action == "setLiquescent") {
        std::string elementId, curve;
        if (this->ParseSetLiquescentAction(json.get<jsonxx::Object>("param"), &elementId, &curve)) {
            return this->SetLiquescent(elementId, curve);
        }
        LogWarning("Could not parse the set liquescent action");
    }
    else if (action == "setAquitanianElement") {
        std::string elementId, shape;
        if (this->ParseSetAquitanianElementAction(json.get<jsonxx::Object>("param"), &elementId, &shape)) {
            return this->SetAquitanianElement(elementId, shape);
        }
        LogWarning("Could not parse the set aquitanian element action");
    }
    else if (action == "remove") {
        std::string elementId;
        if (this->ParseRemoveAction(json.get<jsonxx::Object>("param"), &elementId)) {
            return this->Remove(elementId);
        }
        LogWarning("Could not parse the remove action");
    }
    else if (action == "resize") {
        std::string elementId;
        int ulx, uly, lrx, lry;
        if (this->ParseResizeAction(json.get<jsonxx::Object>("param"), &elementId, &ulx, &uly, &lrx, &lry)) {
            return this->Resize(elementId, ulx, uly, lrx, lry);
        }
        LogWarning("Could not parse the resize action");
    }
    else if (action == "resizeRotate") {
        std::string elementId;
        int ulx, uly, lrx, lry;
        float rotate;
        if (this->ParseResizeRotateAction(
                json.get<jsonxx::Object>("param"), &elementId, &ulx, &uly, &lrx, &lry, &rotate)) {
            return this->Resize(elementId, ulx, uly, lrx, lry, rotate);
        }
    }
    else if (action == "chain") {
        if (!json.has<jsonxx::Array>("param")) {
            LogError("Incorrectly formatted JSON action");
            return false;
        }
        return this->Chain(json.get<jsonxx::Array>("param"));
    }
    else if (action == "group") {
        std::string groupType;
        std::vector<std::string> elementIds;
        if (this->ParseGroupAction(json.get<jsonxx::Object>("param"), &groupType, &elementIds)) {
            return this->Group(groupType, elementIds);
        }
    }
    else if (action == "ungroup") {
        std::string groupType;
        std::vector<std::string> elementIds;
        if (this->ParseUngroupAction(json.get<jsonxx::Object>("param"), &groupType, &elementIds)) {
            return this->Ungroup(groupType, elementIds);
        }
    }
    else if (action == "splitNeume") {
        std::string elementId;
        std::string ncId;
        if (this->ParseSplitNeumeAction(json.get<jsonxx::Object>("param"), &elementId, &ncId)) {
            return this->SplitNeume(elementId, ncId);
        }
    }
    else if (action == "matchHeight") {
        std::string elementId;
        if (this->ParseMatchHeightAction(json.get<jsonxx::Object>("param"), &elementId)) {
            return this->MatchHeight(elementId);
        }
        LogWarning("Could not parse the insert action");
    }
    else if (action == "merge") {
        std::vector<std::string> elementIds;
        if (this->ParseMergeAction(json.get<jsonxx::Object>("param"), &elementIds)) {
            return this->Merge(elementIds);
        }
        LogWarning("Could not parse merge action");
    }
    else if (action == "split") {
        std::string elementId;
        int x;
        if (this->ParseSplitAction(json.get<jsonxx::Object>("param"), &elementId, &x)) {
            return this->Split(elementId, x);
        }
        LogWarning("Could not parse split action");
    }
    else if (action == "changeGroup") {
        std::string elementId;
        std::string contour;
        if (this->ParseChangeGroupAction(json.get<jsonxx::Object>("param"), &elementId, &contour)) {
            return this->ChangeGroup(elementId, contour);
        }
        LogWarning("Could not parse change group action");
    }
    else if (action == "toggleLigature") {
        std::vector<std::string> elementIds;
        std::string isLigature;
        if (this->ParseToggleLigatureAction(json.get<jsonxx::Object>("param"), &elementIds)) {
            return this->ToggleLigature(elementIds);
        }
        LogWarning("Could not parse toggle ligature action");
    }
    else if (action == "changeStaff") {
        std::string elementId;
        if (this->ParseChangeStaffAction(json.get<jsonxx::Object>("param"), &elementId)) {
            return this->ChangeStaff(elementId);
        }
        LogWarning("Could not parse change staff action");
    }
    else if (action == "changeStaffTo") {
        std::string elementId, staffId;
        if (this->ParseChangeStaffToAction(json.get<jsonxx::Object>("param"), &elementId, &staffId)) {
            return this->ChangeStaffTo(elementId, staffId);
        }
        LogWarning("Could not parse change staff action");
    }
    else {
        LogWarning("Unknown action type '%s'.", action.c_str());
    }
    m_editInfo.import("status", "FAILURE");
    m_editInfo.import("message", "Action " + action + " could not be parsed or is unknown.");
    return false;
}

bool EditorToolkitNeume::Chain(jsonxx::Array actions)
{
    // LogMessage("%s", actions.get<jsonxx::Object>(0).json().c_str());
    bool status = true;
    jsonxx::Object results;
    for (int i = 0; i < (int)actions.size(); i++) {
        if (!actions.has<jsonxx::Object>(0)) {
            LogError("Action %d was not an object", i);
            m_editInfo.reset();
            m_editInfo.import("status", "FAILURE");
            m_editInfo.import("message", "Action " + std::to_string(i) + " was not an object.");
            return false;
        }
        status |= this->ParseEditorAction(actions.get<jsonxx::Object>(i).json());
        results.import(std::to_string(i), m_editInfo);
    }
    m_editInfo = results;
    return status;
}

bool EditorToolkitNeume::AddSyl(std::string elementId, std::string sylText)
{
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page.");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Could not get the drawing page.");
        return true;
    }

    Syllable *syllable = dynamic_cast<Syllable *>(m_doc->GetDrawingPage()->FindDescendantByID(elementId));
    if (syllable == NULL) {
        LogError("Unable to find syllable with id %s", elementId.c_str());
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Unable to find neume with id " + elementId + ".");
        return false;
    }

    // Create a new syl element
    Syl *syl = new Syl();
    Text *text = new Text();
    text->SetText(UTF8to32(sylText));
    syl->AddChild(text);
    syllable->AddChild(syl);

    // Create default bounding box if facs
    if (m_doc->HasFacsimile()) {
        Zone *zone = new Zone();
        Staff *staff = syllable->GetAncestorStaff();
        const int staffSize = m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);

        zone->SetUlx(syllable->GetFirst(NEUME)->GetFirst(NC)->GetFacsimileInterface()->GetZone()->GetUlx());
        zone->SetUly(staff->GetFacsimileInterface()->GetZone()->GetLry());
        zone->SetLrx(syllable->GetLast(NEUME)->GetLast(NC)->GetFacsimileInterface()->GetZone()->GetLrx());
        zone->SetLry(zone->GetUly() + staffSize * 2);

        // Make bbox larger if it has less than 2 ncs
        if (syllable->GetChildCount(NC, 2) <= 2) {
            zone->SetLrx(zone->GetLrx() + 50);
        }

        assert(m_doc->GetFacsimile());
        m_doc->GetFacsimile()->FindDescendantByType(SURFACE)->AddChild(zone);
        FacsimileInterface *fi = syl->GetFacsimileInterface();
        assert(fi);
        fi->AttachZone(zone);

        if (m_doc->IsTranscription() && m_doc->HasFacsimile()) m_doc->SyncFromFacsimileDoc();
    }

    m_editInfo.import("uuid", elementId);
    m_editInfo.import("status", "OK");
    m_editInfo.import("message", "");
    return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
// Author: jacob-hutnyk
//
// This function is dedicated to ensuring that pitched elements
// retain their position on the staves by adjusting their pitch position to match their new clefs.
// With the respect to to this goal there are two main cases. Throughout this comment "this clef"
// refers to the clef being dragged.
//
//  Case 1:
//      The clef you're dragging stays between the same two bounding clefs. In this case
//      elements that are newly associated to this clef need to have their pitch changed from
//      the clef preceding this clef (previousClef) to this clef. The elements that are associated
//      with this clef before and after the drag need to have their pitch changed only if the line
//      of the clef changed during the drag.
//  Case 2:
//      The clef you're dragging moves across other clefs. In other words the preceding and
//      subsequent clefs are different before and after the drag. In this case elements that were
//      associated with this clef before the drag need to be reassociated to the clef that preceded
//      this clef before the drag. Elements that become newly associated with the clef after the drag
//      need to be reassociated from the clef that preceeds this clef after the drag to this clef.
//
// Extracting the exact elements that need to have their pitch modified in each of these cases is
// tricky, and required some dicey naming.
//
// The clefs we're interested are named precedingClefBefore
// (meaning the clef that preceded this clef before the drag action), precedingClefAfter (meaning the
// clef that preceded this clef after the drag action), nextClefBefore (meaning the clef that came
// after this clef before the drag action), and nextClefAfter. And of course clef just refers to
// this clef, the clef being dragged.
//
// There are also ListOfObjects which refer to which elements
// were associated to different clefs at different times. with{ClefName} just refers to the pitched
// elements that were associated to that clef at that time. For example withThisClefBefore refers
// to the elements that were associated with this clef before the drag action took place.
//
// There are also some slightly trickier array names like withNewPrecedingClefBefore.
// In this case the the before/after part refers to what elements were associated to
// this clef at what time, while the new/old part refers to the time of the clef having the
// preceding/next relationship to this clef. Let's take withNewPrecedingClefBefore as an example.
// Let's say the order of the clefs before the drag is:
//
//          A B C D
//
// with C being the clef we're dragging. And after the drag action the order of the clefs is:
//
//          A C B D
//
// withNewPrecedingClefBefore would refer to the elements between A and B, since A is the
// new preceding clef (it becomes the clef that precedes this clef after the drag action)
// and before the drag action all the elements between A and B were associated with A.
// By comparison, withPrecedingClefAfter (or withNewPrecedingClefAfter) would refer to only
// the elements between A and C in the lower depiction.
//
// The final piece of the naming scheme is the naming of the arrays whose pitch values actually
// need to be changed. These should be pretty clear. stillWithThisClef for example, refers to
// the elements that were associated with this clef before and after the drag action, and thus whose
// pitch values only needs to be changed if the line of this clef changed.
//
// The algorithm is implemented by finding all of the aforementioned clefs and the elements between
// them, and then using std::set_difference to find the elements whose pitch values may need to change.
// For example: in case 2 noLongerWithThisClef is found by taking the difference between
// withOldPrecedingClefAfter and withPrecedingClefBefore, since that difference is the stuff that
// became associated with the clef that used to preceed this clef, meaning the stuff that was associated
// with clef, but no longer is.
//
// One other aspect that might seem confusing is exactly when clef->SetLine() gets called. The reason
// that these calls are oddly placed is that AdjustPitchForNewClef() uses the line of the clef.
// So if we're changing an element's pitch from this clef to something else, we need the line of
// this clef to be what it was before the drag. On the other hand, if we're reassociating an element
// from some clef to the clef we're dragging, we need the line of this clef to be the one it is after
// the drag action. Each of the clef->SetLine() calls are placed so as to accommodate this.
//////////////////////////////////////////////////////////////////////////////////////////////////////
bool EditorToolkitNeume::ClefMovementHandler(Clef *clef, int x, int y)
{

    Layer *layer = dynamic_cast<Layer *>(clef->GetFirstAncestor(LAYER));
    if (!layer) return false;

    Staff *staff = dynamic_cast<Staff *>(layer->GetFirstAncestor(STAFF));
    assert(staff);

    // Note that y param is relative to initial position for clefs
    int initialClefLine = clef->GetLine();
    int clefLine = round(((double)y - x * tan(staff->GetDrawingRotation() * M_PI / 180.0))
            / (double)m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize)
        + initialClefLine);

    int lineDiff = clefLine - initialClefLine;

    ListOfObjects withThisClefBefore;
    ListOfObjects withPrecedingClefBefore;

    ClassIdComparison ac(CLEF);
    InterfaceComparison ic(INTERFACE_PITCH);

    Clef *precedingClefBefore = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindPreviousChild(&ac, clef));
    Clef *nextClefBefore = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindNextChild(&ac, clef));

    m_doc->GetDrawingPage()->FindAllDescendantsBetween(&withThisClefBefore, &ic, clef,
        (nextClefBefore != layer->GetCurrentClef()) ? nextClefBefore : m_doc->GetDrawingPage()->GetLast());
    m_doc->GetDrawingPage()->FindAllDescendantsBetween(&withPrecedingClefBefore, &ic, precedingClefBefore, clef);

    // Adjust facsimile for clef (if it exists)
    if (clef->HasFacs()) {
        Zone *zone = clef->GetZone();
        assert(zone);
        int y = (clefLine - initialClefLine) * 2 * staff->m_drawingStaffSize
            - x * tan(staff->GetDrawingRotation() * M_PI / 180.0);
        zone->ShiftByXY(x, -y);
    }

    layer->ReorderByXPos();

    Clef *precedingClefAfter = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindPreviousChild(&ac, clef));
    Clef *nextClefAfter = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindNextChild(&ac, clef));

    // case 1
    if (precedingClefAfter == precedingClefBefore && nextClefAfter == nextClefBefore) {
        ListOfObjects withThisClefAfter;
        ListOfObjects withPrecedingClefAfter;

        m_doc->GetDrawingPage()->FindAllDescendantsBetween(&withThisClefAfter, &ic, clef,
            (nextClefAfter != NULL) ? nextClefAfter : m_doc->GetDrawingPage()->GetLast());
        m_doc->GetDrawingPage()->FindAllDescendantsBetween(&withPrecedingClefAfter, &ic, precedingClefBefore, clef);

        // moved left
        if (withPrecedingClefBefore.size() > withPrecedingClefAfter.size()) {
            ListOfObjects newlyWithThisClef;

            clef->SetLine(clefLine);

            std::set_difference(withPrecedingClefBefore.begin(), withPrecedingClefBefore.end(),
                withPrecedingClefAfter.begin(), withPrecedingClefAfter.end(),
                std::inserter(newlyWithThisClef, newlyWithThisClef.begin()));

            for (auto iter = newlyWithThisClef.begin(); iter != newlyWithThisClef.end(); ++iter) {
                (*iter)->GetPitchInterface()->AdjustPitchForNewClef(
                    (precedingClefBefore != NULL) ? precedingClefBefore : layer->GetCurrentClef(), clef);
            }
            if (lineDiff != 0) {
                for (auto iter = withThisClefBefore.begin(); iter != withThisClefBefore.end(); ++iter) {
                    (*iter)->GetPitchInterface()->AdjustPitchByOffset(lineDiff * -2);
                }
            }
        }
        // moved right
        else if (withPrecedingClefBefore.size() < withPrecedingClefAfter.size()) {
            ListOfObjects noLongerWithThisClef;

            std::set_difference(withPrecedingClefAfter.begin(), withPrecedingClefAfter.end(),
                withPrecedingClefBefore.begin(), withPrecedingClefBefore.end(),
                std::inserter(noLongerWithThisClef, noLongerWithThisClef.begin()));

            for (auto iter = noLongerWithThisClef.begin(); iter != noLongerWithThisClef.end(); ++iter) {
                (*iter)->GetPitchInterface()->AdjustPitchForNewClef(
                    clef, (precedingClefBefore != NULL) ? precedingClefBefore : layer->GetCurrentClef());
            }
            if (lineDiff != 0) {
                for (auto iter = withThisClefAfter.begin(); iter != withThisClefAfter.end(); ++iter) {
                    (*iter)->GetPitchInterface()->AdjustPitchByOffset(lineDiff * -2);
                }
            }
            clef->SetLine(clefLine);
        }
        else {
            clef->SetLine(clefLine);
            if (lineDiff != 0) {
                for (auto iter = withThisClefBefore.begin(); iter != withThisClefBefore.end(); ++iter) {
                    (*iter)->GetPitchInterface()->AdjustPitchByOffset(lineDiff * -2);
                }
            }
        }
    }
    // case 2
    else {
        ListOfObjects withOldPrecedingClefAfter;
        ListOfObjects withNewPrecedingClefBefore;
        ListOfObjects withNewPrecedingClefAfter;
        ListOfObjects noLongerWithThisClef;
        ListOfObjects newlyWithThisClef;

        m_doc->GetDrawingPage()->FindAllDescendantsBetween(&withOldPrecedingClefAfter, &ic, precedingClefBefore,
            (nextClefBefore != NULL) ? nextClefBefore : m_doc->GetDrawingPage()->GetLast());

        m_doc->GetDrawingPage()->FindAllDescendantsBetween(&withNewPrecedingClefBefore, &ic, precedingClefAfter,
            (nextClefAfter != NULL) ? nextClefAfter : m_doc->GetDrawingPage()->GetLast());

        m_doc->GetDrawingPage()->FindAllDescendantsBetween(&withNewPrecedingClefAfter, &ic, precedingClefAfter, clef);

        std::set_difference(withOldPrecedingClefAfter.begin(), withOldPrecedingClefAfter.end(),
            withPrecedingClefBefore.begin(), withPrecedingClefBefore.end(),
            std::inserter(noLongerWithThisClef, noLongerWithThisClef.begin()));

        std::set_difference(withNewPrecedingClefBefore.begin(), withNewPrecedingClefBefore.end(),
            withNewPrecedingClefAfter.begin(), withNewPrecedingClefAfter.end(),
            std::inserter(newlyWithThisClef, newlyWithThisClef.begin()));

        for (auto iter = noLongerWithThisClef.begin(); iter != noLongerWithThisClef.end(); ++iter) {
            (*iter)->GetPitchInterface()->AdjustPitchForNewClef(
                clef, (precedingClefBefore != NULL) ? precedingClefBefore : layer->GetCurrentClef());
        }

        clef->SetLine(clefLine);

        for (auto iter = newlyWithThisClef.begin(); iter != newlyWithThisClef.end(); ++iter) {
            (*iter)->GetPitchInterface()->AdjustPitchForNewClef(
                (precedingClefAfter != NULL) ? precedingClefAfter : layer->GetCurrentClef(), clef);
        }
    }
    return true;
}

bool EditorToolkitNeume::Drag(std::string elementId, int x, int y, bool topLevel)
{
    std::string status = "OK", message = "";
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get drawing page.");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Could not get drawing page.");
        return false;
    }

    // Try to get the element on the current drawing page
    Object *element = m_doc->GetDrawingPage()->FindDescendantByID(elementId);

    // If it wasn't there, go back up to the whole doc
    if (!element) {
        element = m_doc->FindDescendantByID(elementId);
    }
    if (!element) {
        LogWarning("element is null");
        status = "WARNING";
        message += "Element is null. ";
    }
    assert(element);

    if (element->Is(CUSTOS)) {
        FacsimileInterface *fi = element->GetFacsimileInterface();
        assert(fi);
        Zone *zone = fi->GetZone();
        assert(zone);
        zone->ShiftByXY(x, -y);

        this->AdjustPitchAfterDrag(element, y);
    }
    else if (element->Is(SYLLABLE)) {
        // Check for clefs in syllable
        ListOfObjects clefs;
        ClassIdComparison ac(CLEF);
        element->FindAllDescendantsByComparison(&clefs, &ac);
        bool hasClef = (clefs.size() != 0);
        if (hasClef) {
            for (Object *obj : clefs) {
                Clef *clef = dynamic_cast<Clef *>(obj);
                this->Drag(clef->GetID(), x, y, false);
            }
        }

        ListOfObjects ncs;
        ClassIdComparison ncComp(NC);
        element->FindAllDescendantsByComparison(&ncs, &ncComp);
        for (auto nc = ncs.begin(); nc != ncs.end(); ++nc) {
            this->Drag((*nc)->GetID(), x, y, false);
        }
    }
    else if (element->Is(NEUME)) {
        ListOfObjects ncs;
        ClassIdComparison ncComp(NC);
        element->FindAllDescendantsByComparison(&ncs, &ncComp);
        for (auto nc = ncs.begin(); nc != ncs.end(); ++nc) {
            this->Drag((*nc)->GetID(), x, y, false);
        }
    }
    else if (element->Is(NC)) {
        // Don't adjust the same facsimile twice. NCs in a ligature share a single zone.
        bool skipLigature = false;
        Nc *nc = dynamic_cast<Nc *>(element);
        if (nc->GetLigated() == BOOLEAN_true) {
            Neume *neume = vrv_cast<Neume *>(nc->GetFirstAncestor(NEUME));
            Nc *nextNc = dynamic_cast<Nc *>(neume->GetChild(1 + neume->GetChildIndex(element)));
            if (nextNc != NULL && nextNc->GetLigated() == BOOLEAN_true && nextNc->GetZone() == nc->GetZone()) {
                skipLigature = true;
            }
        }
        if (!skipLigature) {
            FacsimileInterface *fi = element->GetFacsimileInterface();
            assert(fi);
            Zone *zone = fi->GetZone();
            assert(zone);
            zone->ShiftByXY(x, -y);
        }
        this->AdjustPitchAfterDrag(nc, y);
    }
    else if (element->Is(CLEF)) {
        Clef *clef = dynamic_cast<Clef *>(element);
        if (!clef->HasFacs()) {
            LogError("Clef dragging is only supported for clefs with facsimiles!");
            m_editInfo.import("status", "FAILURE");
            m_editInfo.import("message", "Clef dragging is only supported for clefs with facsimiles.");
            return false;
        }
        FacsimileInterface *fi = (*clef).GetFacsimileInterface();
        assert(fi);
        // If inside this syllable, moving a clef...
        // Case 1: BEFORE preceeding syllable(s) -> clef becomes start of this syllable, this syllable reorders before
        // preceeding syllable(s), preceeding and following syllables abide by this clef. Case 2: INSIDE this syllable
        // -> following neumes and syllables abide by this clef. Case 3: AFTER this syllable -> clef becomes the end of
        // this syllable, following syllables abide by this clef still.
        if (element->GetParent()->Is(SYLLABLE)) {
            Object *parent = clef->GetParent();
            assert(parent);
            Object *lNc;
            if (parent->GetChildCount(NEUME) == 1) {
                lNc = parent->GetFirst(NEUME)->GetLast();
            }
            else {
                lNc = parent->GetLast(NEUME)->GetLast();
            }
            int xRight = lNc->GetFacsimileInterface()->GetZone()->GetLrx();
            int clefInitial = clef->GetFacsimileInterface()->GetZone()->GetLrx();
            int clefTarget = clefInitial + x;

            // Case 1 & 2
            if (clefTarget < xRight) {
                this->ClefMovementHandler(clef, x, y);
            }
            // Case 3
            // Move to end of syllable and then shift zone so mei position stays at end of syllable
            else {
                int distanceToSyllableEnd = xRight - clefInitial + 1;
                int distanceRemaining = clefTarget - xRight - 1;

                this->ClefMovementHandler(clef, distanceToSyllableEnd, y);
                if (fi->GetZone() != NULL) {
                    Zone *zone = fi->GetZone();
                    assert(zone);
                    zone->ShiftByXY(distanceRemaining, 0);
                }
            }
        }
        // Not inside syllable
        else {
            this->ClefMovementHandler(clef, x, y);
        }
        m_doc->ScoreDefSetCurrentDoc(true); // this is needed for staves without clef
    }
    else if (element->Is(STAFF)) {
        Staff *staff = vrv_cast<Staff *>(element);
        if (!staff->HasFacs()) {
            LogError("Staff dragging is only supported for staves with facsimiles!");
            m_editInfo.import("status", "FAILURE");
            m_editInfo.import("message", "Staff dragging is only supported for staves with facsimiles.");
            return false;
        }

        // Move staff and all staff children with facsimiles
        Zone *staffZone = staff->GetZone();
        assert(staffZone);
        staffZone->ShiftByXY(x, -y);
        ListOfObjects children;
        InterfaceComparison ic(INTERFACE_FACSIMILE);
        staff->FindAllDescendantsByComparison(&children, &ic);
        for (auto it = children.begin(); it != children.end(); ++it) {
            FacsimileInterface *fi = (*it)->GetFacsimileInterface();
            assert(fi);
            Zone *zone = fi->GetZone();
            if (zone) zone->ShiftByXY(x, -y);
        }

        this->SortStaves();

        if (m_doc->IsTranscription() && m_doc->HasFacsimile()) m_doc->SyncFromFacsimileDoc();
        m_doc->GetDrawingPage()->LayOutTranscription(true);

        return true; // Can't reorder by layer since staves contain layers
    }
    else if (element->Is(SYL)) {
        Syl *syl = dynamic_cast<Syl *>(element);
        if (!syl->HasFacs()) {
            LogError("Syl (boundingbox) dragging is only supported for syls with facsimiles!");
            m_editInfo.import("status", "FAILURE");
            m_editInfo.import("message", "Syl dragging is only supported for syls with facsimiles.");
            return false;
        }
        FacsimileInterface *fi = (*syl).GetFacsimileInterface();
        assert(fi);
        if (fi->GetZone() != NULL) {
            fi->GetZone()->ShiftByXY(x, -y);
        }
    }
    else if (element->Is(ACCID)) {
        Accid *accid = dynamic_cast<Accid *>(element);
        if (!accid->HasFacs()) {
            LogError("Accid dragging is only supported for accid with facsimiles!");
            m_editInfo.import("status", "FAILURE");
            m_editInfo.import("message", "Accid dragging is only supported for accid with facsimiles.");
            return false;
        }
        FacsimileInterface *fi = (*accid).GetFacsimileInterface();
        assert(fi);
        if (fi->GetZone() != NULL) {
            Zone *zone = fi->GetZone();
            assert(zone);
            zone->ShiftByXY(x, -y);
        }
    }
    else if (element->Is(DIVLINE)) {
        DivLine *divLine = dynamic_cast<DivLine *>(element);
        if (!divLine->HasFacs()) {
            LogError("DivLine dragging is only supported for divLine with facsimiles!");
            m_editInfo.import("status", "FAILURE");
            m_editInfo.import("message", "DivLine dragging is only supported for divLine with facsimiles.");
            return false;
        }
        FacsimileInterface *fi = (*divLine).GetFacsimileInterface();
        assert(fi);
        if (fi->GetZone() != NULL) {
            Zone *zone = fi->GetZone();
            assert(zone);
            zone->ShiftByXY(x, -y);
        }
        this->ChangeStaff(elementId);
    }
    else {
        LogWarning("Unsupported element for dragging.");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Unsupported element for dragging.");
        return false;
    }

    if (topLevel) {
        Layer *layer = vrv_cast<Layer *>(element->GetFirstAncestor(LAYER));
        layer->ReorderByXPos(); // Reflect position order of elements internally (and in the resulting output file)
        if (m_doc->IsTranscription() && m_doc->HasFacsimile()) m_doc->SyncFromFacsimileDoc();
        m_doc->GetDrawingPage()->LayOutTranscription(true);

        m_editInfo.import("status", status);
        m_editInfo.import("message", message);
    }

    return true;
}

bool EditorToolkitNeume::Insert(std::string elementType, std::string staffId, int ulx, int uly, int lrx, int lry,
    std::vector<std::pair<std::string, std::string>> attributes)
{
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get drawing page");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Could not get drawing page.");
        return false;
    }
    if (!m_doc->HasFacsimile()) {
        LogError("Drawing page without facsimile");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Drawing page without facsimile is unsupported.");
        return false;
    }

    Staff *staff;

    std::string status = "OK", message = "";

    // Find closest valid staff
    if (staffId == "auto") {
        ListOfObjects staves = m_doc->FindAllDescendantsByType(STAFF, false);
        std::vector<Object *> stavesVector(staves.begin(), staves.end());

        ClosestBB comp;
        comp.x = ulx;
        comp.y = uly;

        if (staves.size() > 0) {
            std::sort(stavesVector.begin(), stavesVector.end(), comp);
            staff = dynamic_cast<Staff *>(stavesVector.at(0));
        }
        else {
            staff = NULL;
        }
    }
    else {
        staff = dynamic_cast<Staff *>(m_doc->FindDescendantByID(staffId));
    }

    Facsimile *facsimile = m_doc->GetFacsimile();
    Zone *zone = new Zone();

    if (elementType == "staff") {
        Object *page = m_doc->GetDrawingPage();
        System *newSystem = new System();
        Sb *newSb = new Sb();
        Measure *newMeasure = new Measure(NEUMELINE);
        Staff *newStaff;
        Layer *newLayer = new Layer();
        std::string columnValue;

        // Use closest existing staff (if there is one)
        if (staff) {
            columnValue = staff->GetType();
            int n = page->GetChildCount(SYSTEM) + 1;
            newStaff = new Staff(n);
            newStaff->m_drawingStaffDef = staff->m_drawingStaffDef;
            newStaff->m_drawingNotationType = staff->m_drawingNotationType;
            newStaff->m_drawingLines = staff->m_drawingLines;
        }
        else {
            newStaff = new Staff(1);
            newStaff->m_drawingStaffDef = vrv_cast<StaffDef *>(
                m_doc->GetCorrespondingScore(page)->GetScoreDef()->FindDescendantByType(STAFFDEF));
            newStaff->m_drawingNotationType = NOTATIONTYPE_neume;
            newStaff->m_drawingLines = 4;
        }
        newStaff->m_drawingStaffSize = (uly - lry) / (newStaff->m_drawingLines - 1);
        zone->SetUlx(ulx);
        zone->SetUly(uly);
        zone->SetLrx(lrx);
        zone->SetLry(lry);
        Surface *surface = vrv_cast<Surface *>(m_doc->GetFacsimile()->FindDescendantByType(SURFACE));
        assert(surface);
        surface->AddChild(zone);
        newStaff->AttachZone(zone);
        if (columnValue.length()) newStaff->SetType(columnValue);

        newStaff->AddChild(newLayer);
        newMeasure->AddChild(newStaff);
        newSystem->AddChild(newSb);
        newSystem->AddChild(newMeasure);
        newSystem->SetDrawingScoreDef(vrv_cast<ScoreDef *>(m_doc->GetCorrespondingScore(page)->GetScoreDef()));

        page->InsertAfter(page->GetFirst(SCORE), newSystem);

        this->SortStaves();

        if (m_doc->IsTranscription() && m_doc->HasFacsimile()) m_doc->SyncFromFacsimileDoc();

        m_editInfo.import("uuid", newStaff->GetID());
        m_editInfo.import("status", status);
        m_editInfo.import("message", message);
        return true;
    }

    if (staff == NULL) {
        LogError("A staff must exist in the page to add a non-staff element.");
        delete zone;
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "A staff must exist in the page to add a non-staff element.");
        return false;
    }
    Layer *layer = vrv_cast<Layer *>(staff->FindDescendantByType(LAYER));
    assert(layer);

    if (elementType == "nc" || elementType == "grouping") {
        Syllable *syllable = new Syllable();
        Syl *syl = new Syl();
        Neume *neume = new Neume();
        Nc *nc = new Nc();
        Zone *sylZone = NULL;
        std::string contour = "";
        nc->AttachZone(zone);

        Surface *surface = vrv_cast<Surface *>(facsimile->FindDescendantByType(SURFACE));
        surface->AddChild(zone);

        Text *text = new Text();
        std::u32string str = U"";
        text->SetText(str);
        syl->AddChild(text);

        neume->AddChild(nc);
        syllable->AddChild(neume);
        syllable->AddChild(syl);
        layer->AddChild(syllable);

        const int noteHeight
            = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / NOTE_HEIGHT_TO_STAFF_SIZE_RATIO);
        const int noteWidth
            = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / NOTE_WIDTH_TO_STAFF_SIZE_RATIO);
        const int offsetX = (int)(noteWidth / 2);
        const int offsetY = (int)(noteHeight / 2);

        // Set up facsimile
        zone->SetUlx(ulx - offsetX);
        zone->SetUly(uly - offsetY);
        zone->SetLrx(ulx + offsetX);
        zone->SetLry(uly + offsetY);

        // add syl bounding box if Facs
        if (m_doc->HasFacsimile()) {
            FacsimileInterface *fi = vrv_cast<FacsimileInterface *>(syl->GetFacsimileInterface());
            assert(fi);
            sylZone = new Zone();

            int staffLry = staff->GetZone()->GetLry();

            // width height and offset can be adjusted
            int bboxHeight = 175;
            int bboxOffsetX = 50;

            // calculate staff rotation offset
            double theta = staff->GetDrawingRotation();
            int offsetY = 0;
            if (theta) {
                double factor = 1.3;
                offsetY = (int)((ulx - staff->GetZone()->GetUlx()) * tan(theta * M_PI / 180.0) / factor);
            }

            sylZone->SetUlx(ulx);
            sylZone->SetUly(staffLry + offsetY);
            sylZone->SetLrx(ulx + noteWidth + bboxOffsetX);
            sylZone->SetLry(staffLry + offsetY + bboxHeight);
            surface->AddChild(sylZone);
            fi->AttachZone(sylZone);
        }

        layer->ReorderByXPos();

        if (!AdjustPitchFromPosition(syllable)) {
            delete syllable;
            delete neume;
            delete nc;
            LogError("Failed to set pitch.");

            m_editInfo.import("status", "FAILURE");
            m_editInfo.import("message", "Failed to set pitch.");
            return false;
        }

        // Set as inclinatum or virga or liquescent (if necessary), or get contour for grouping
        for (auto it = attributes.begin(); it != attributes.end(); ++it) {
            if (it->first == "tilt") {
                if (it->second == "s") {
                    nc->SetTilt(COMPASSDIRECTION_s);
                }
                else if (it->second == "se") {
                    nc->SetTilt(COMPASSDIRECTION_se);
                }
                else if (it->second == "n") {
                    nc->SetTilt(COMPASSDIRECTION_n);
                }
            }
            else if (it->first == "contour") {
                contour = it->second;
            }
            else if (it->first == "curve") {
                curvatureDirection_CURVE curve = curvatureDirection_CURVE_NONE;
                if (it->second == "a") {
                    curve = curvatureDirection_CURVE_a;
                    nc->SetCurve(curve);
                }
                else if (it->second == "c") {
                    curve = curvatureDirection_CURVE_c;
                    nc->SetCurve(curve);
                }
                Liquescent *liquescent = new Liquescent();
                nc->AddChild(liquescent);
            }
            else if (it->first == "shape") {
                if (it->second == "quilisma") {
                    Quilisma *quilisma = new Quilisma();
                    nc->AddChild(quilisma);
                }
                else if (it->second == "oriscus") {
                    Oriscus *oriscus = new Oriscus();
                    nc->AddChild(oriscus);
                }
            }
        }

        // If inserting grouping, add the remaining nc children to the neume.
        if (contour != "") {
            Nc *prevNc = nc;
            for (auto it = contour.begin(); it != contour.end(); ++it) {
                Nc *newNc = new Nc();
                Zone *newZone = new Zone();
                int newUlx = ulx + noteWidth;
                int newUly;

                newNc->SetPname(prevNc->GetPname());
                newNc->SetOct(prevNc->GetOct());

                if ((*it) == 'u') {
                    newUly = uly - noteHeight;
                    newNc->AdjustPitchByOffset(1);
                }
                else if ((*it) == 'd') {
                    newUly = uly + noteHeight;
                    newNc->AdjustPitchByOffset(-1);
                }
                else if ((*it) == 's') {
                    newUly = uly;
                }
                else {
                    LogError("Unsupported character in contour.");
                    delete newNc;
                    delete newZone;
                    m_editInfo.import("status", "FAILURE");
                    m_editInfo.import("message", "Unsupported character in contour.");
                    return false;
                }

                // Apply offset due to rotate
                newUly += (newUlx - ulx) * tan(-staff->GetDrawingRotation() * M_PI / 180.0);
                newZone->SetUlx(newUlx - offsetX);
                newZone->SetUly(newUly);
                newZone->SetLrx(newUlx + offsetX);
                newZone->SetLry(newUly + noteHeight);

                newNc->AttachZone(newZone);

                if (sylZone) sylZone->SetLrx(newUlx + noteWidth);

                assert(surface);
                surface->AddChild(newZone);

                neume->AddChild(newNc);

                ulx = newUlx;
                uly = newUly;
                prevNc = newNc;
            }
        }
        if (elementType == "nc") {
            m_editInfo.import("uuid", nc->GetID());
        }
        else {
            m_editInfo.import("uuid", neume->GetID());
        }
    }
    else if (elementType == "clef") {
        delete zone;

        Clef *clef = new Clef();
        data_CLEFSHAPE clefShape = CLEFSHAPE_NONE;

        const int staffSize = m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
        int offsetR = 0, offsetL = 0;

        for (auto it = attributes.begin(); it != attributes.end(); ++it) {
            if (it->first == "shape") {
                if (it->second == "C") {
                    clefShape = CLEFSHAPE_C;
                    offsetR = (int)(staffSize / NOTE_WIDTH_TO_STAFF_SIZE_RATIO / 2);
                    offsetL = offsetR;
                    break;
                }
                else if (it->second == "F") {
                    clefShape = CLEFSHAPE_F;
                    offsetL = (int)(staffSize / NOTE_WIDTH_TO_STAFF_SIZE_RATIO / 2);
                    break;
                }
                else if (it->second == "G") {
                    clefShape = CLEFSHAPE_G;
                    offsetR = (int)(staffSize / NOTE_WIDTH_TO_STAFF_SIZE_RATIO);
                    break;
                }
            }
        }
        if (clefShape == CLEFSHAPE_NONE) {
            LogError("A clef shape must be specified.");
            delete clef;

            m_editInfo.import("status", "FAILURE");
            m_editInfo.import("message", "A clef shape must be specified.");
            return false;
        }
        clef->SetShape(clefShape);
        int yDiff = -staff->GetZone()->GetUly() + uly;
        yDiff += ((ulx - staff->GetZone()->GetUlx()))
            * tan(-staff->GetDrawingRotation() * M_PI / 180.0); // Subtract distance due to rotate.
        int clefLine = staff->m_drawingLines - round((double)yDiff / (double)staffSize);
        clef->SetLine(clefLine);

        Zone *zone = new Zone();
        zone->SetUlx(ulx - offsetR);
        zone->SetUly(uly);
        zone->SetLrx(ulx + offsetL);
        zone->SetLry(uly + staffSize / NOTE_HEIGHT_TO_STAFF_SIZE_RATIO);
        clef->AttachZone(zone);
        Surface *surface = dynamic_cast<Surface *>(facsimile->FindDescendantByType(SURFACE));
        assert(surface);
        surface->AddChild(zone);
        layer->AddChild(clef);
        m_editInfo.import("uuid", clef->GetID());
        layer->ReorderByXPos();

        // ensure pitched elements associated with this clef keep their x,y positions
        m_doc->ScoreDefSetCurrentDoc(true); // this is needed for staves without clef
        ClassIdComparison ac(CLEF);
        Clef *previousClef = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindPreviousChild(&ac, clef));
        Clef *nextClef = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindNextChild(&ac, clef));

        if (previousClef == NULL) {
            // if there is no previous clef, get the default one from the staff def
            previousClef = layer->GetCurrentClef();
        }

        // adjust pitched elements whose clef is changing
        ListOfObjects elements;
        InterfaceComparison ic(INTERFACE_PITCH);

        m_doc->GetDrawingPage()->FindAllDescendantsBetween(
            &elements, &ic, clef, (nextClef != NULL) ? nextClef : m_doc->GetDrawingPage()->GetLast());

        for (auto it = elements.begin(); it != elements.end(); ++it) {
            PitchInterface *pi = (*it)->GetPitchInterface();
            assert(pi);
            pi->AdjustPitchForNewClef(previousClef, clef);
        }
    }
    else if (elementType == "custos") {
        Custos *custos = new Custos();
        zone->SetUlx(ulx);
        Surface *surface = vrv_cast<Surface *>(facsimile->GetFirst(SURFACE));
        surface->AddChild(zone);
        custos->AttachZone(zone);
        layer->AddChild(custos);

        const int noteHeight
            = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / NOTE_HEIGHT_TO_STAFF_SIZE_RATIO);
        const int noteWidth
            = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / NOTE_WIDTH_TO_STAFF_SIZE_RATIO);

        zone->SetUlx(ulx - noteWidth * 0.25);
        zone->SetUly(uly);
        zone->SetLrx(ulx + noteWidth * 0.75);
        zone->SetLry(uly + noteHeight);
        layer->ReorderByXPos();
        if (!AdjustPitchFromPosition(custos)) {
            LogError("Failed to set pitch.");

            m_editInfo.import("status", "FAILURE");
            m_editInfo.import("message", "Failed to set pitch.");
            return false;
        }
        m_editInfo.import("uuid", custos->GetID());
    }
    else if (elementType == "accid") {
        Accid *accid = new Accid();
        data_ACCIDENTAL_WRITTEN accidTypeW = ACCIDENTAL_WRITTEN_NONE;

        for (auto it = attributes.begin(); it != attributes.end(); ++it) {
            if (it->first == "accid") {
                if (it->second == "f") {
                    accidTypeW = ACCIDENTAL_WRITTEN_f;
                    break;
                }
                else if (it->second == "n") {
                    accidTypeW = ACCIDENTAL_WRITTEN_n;
                }
            }
        }
        if (accidTypeW == ACCIDENTAL_WRITTEN_NONE) {
            LogError("A accid type must be specified.");
            delete accid;
            delete zone;

            m_editInfo.import("status", "FAILURE");
            m_editInfo.import("message", "A accid type must be specified.");
            return false;
        }

        accid->SetAccid(accidTypeW);
        zone->SetUlx(ulx);
        Surface *surface = dynamic_cast<Surface *>(facsimile->GetFirst(SURFACE));
        surface->AddChild(zone);
        accid->AttachZone(zone);
        layer->AddChild(accid);

        const int noteHeight
            = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / NOTE_HEIGHT_TO_STAFF_SIZE_RATIO);
        const int noteWidth
            = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / NOTE_WIDTH_TO_STAFF_SIZE_RATIO);
        const int offsetX = (int)(noteWidth / 2);

        ulx -= noteWidth / 2;
        uly -= noteHeight / 2;

        zone->SetUlx(ulx + offsetX);
        zone->SetUly(uly);
        zone->SetLrx(ulx + noteWidth + offsetX);
        zone->SetLry(uly + noteHeight);
        layer->ReorderByXPos();

        m_editInfo.import("uuid", accid->GetID());
    }
    else if (elementType == "divLine") {
        DivLine *divLine = new DivLine();
        divLineLog_FORM divLineTypeW = divLineLog_FORM_NONE;

        for (auto it = attributes.begin(); it != attributes.end(); ++it) {
            if (it->first == "form") {
                if (it->second == "maxima") {
                    divLineTypeW = divLineLog_FORM_maxima;
                    break;
                }
                else if (it->second == "maior") {
                    divLineTypeW = divLineLog_FORM_maior;
                    break;
                }
                else if (it->second == "minima") {
                    divLineTypeW = divLineLog_FORM_minima;
                    break;
                }
                else if (it->second == "finalis") {
                    divLineTypeW = divLineLog_FORM_finalis;
                    break;
                }
                else if (it->second == "caesura") {
                    divLineTypeW = divLineLog_FORM_caesura;
                    break;
                }
                else if (it->second == "virgula") {
                    divLineTypeW = divLineLog_FORM_virgula;
                    break;
                }
            }
        }
        if (divLineTypeW == divLineLog_FORM_NONE) {
            LogError("A divLine type must be specified.");
            delete divLine;
            delete zone;

            m_editInfo.import("status", "FAILURE");
            m_editInfo.import("message", "A divLine type must be specified.");
            return false;
        }

        divLine->SetForm(divLineTypeW);
        zone->SetUlx(ulx);
        Surface *surface = dynamic_cast<Surface *>(facsimile->GetFirst(SURFACE));
        surface->AddChild(zone);
        divLine->AttachZone(zone);
        layer->AddChild(divLine);

        const int noteHeight
            = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / NOTE_HEIGHT_TO_STAFF_SIZE_RATIO);
        const int noteWidth
            = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / NOTE_WIDTH_TO_STAFF_SIZE_RATIO);
        const int offsetX = (int)(noteWidth / 2);

        ulx -= noteWidth / 2;
        uly -= noteHeight / 2;

        zone->SetUlx(ulx + offsetX);
        zone->SetUly(uly);
        zone->SetLrx(ulx + noteWidth + offsetX);
        zone->SetLry(uly + noteHeight);
        layer->ReorderByXPos();

        m_editInfo.import("uuid", divLine->GetID());
    }
    else {
        delete zone;
        LogError("Unsupported type '%s' for insertion", elementType.c_str());
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Unsupported type '" + elementType + "' for insertion.");
        return false;
    }
    layer->ReorderByXPos();

    if (m_doc->IsTranscription() && m_doc->HasFacsimile()) m_doc->SyncFromFacsimileDoc();
    m_doc->GetDrawingPage()->LayOutTranscription(true);

    m_editInfo.import("status", status);
    m_editInfo.import("message", message);
    return true;
}

bool EditorToolkitNeume::InsertToSyllable(std::string elementId)
{
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get drawing page");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Could not get drawing page.");
        return false;
    }
    if (!m_doc->HasFacsimile()) {
        LogError("Drawing page without facsimile");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Drawing page without facsimile is unsupported.");
        return false;
    }

    Object *element = m_doc->GetDrawingPage()->FindDescendantByID(elementId);
    assert(element);
    Object *parent = element->GetParent();
    assert(parent);

    if (element == NULL) {
        LogError("No element exists with ID '%s'.", elementId.c_str());
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "No element exists with ID" + elementId + ".");
        return false;
    }
    if (!(element->Is(DIVLINE) || element->Is(ACCID) || element->Is(CLEF))) {
        LogError("Element is of type %s, but only Divlines and Accids can be inserted into syllables.",
            element->GetClassName().c_str());
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message",
            "Element is of type " + element->GetClassName()
                + ", but only DivLines, Accids, and Clefs can be inserted into syllables.");
        return false;
    }
    if (!parent->Is(LAYER)) {
        LogError("The selected %s is not a child of layer.", element->GetClassName().c_str());
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "The selected " + element->GetClassName() + "is not a child of layer.");
        return false;
    }

    // get the position of the selected element
    int ulx;
    int uly;
    if (dynamic_cast<FacsimileInterface *>(element)->HasFacs()) {
        ulx = element->GetFacsimileInterface()->GetZone()->GetUlx();
        uly = element->GetFacsimileInterface()->GetZone()->GetUly();
    }
    else {
        LogError("Selected '%s' without facsimile", element->GetClassName().c_str());
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Selected '" + element->GetClassName() + "' without facsimile is unsupported.");
        return false;
    }

    Staff *staff = dynamic_cast<Staff *>(element->GetFirstAncestor(STAFF));
    assert(staff);

    // find closest neume
    ListOfObjects neumes;
    ClassIdComparison ac(NEUME);
    staff->FindAllDescendantsByComparison(&neumes, &ac);

    if (neumes.empty()) {
        LogError("A syllable must exist in the staff to insert a '%s' into.", element->GetClassName().c_str());
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import(
            "message", "A syllable must exist in the staff to insert a '" + element->GetClassName() + "' into.");
        return false;
    }

    std::vector<Object *> neumesVector(neumes.begin(), neumes.end());
    ClosestNeume compN;
    compN.x = ulx;
    compN.y = uly;
    std::sort(neumesVector.begin(), neumesVector.end(), compN);

    Object *neume = neumesVector.at(0);
    assert(neume);
    // get nearest syllable using nearest neume
    Object *syllable = neume->GetParent();
    assert(syllable);

    // Find previous clef preceding following syllable (for is element is clef)
    ClassIdComparison cc(CLEF);
    ClassIdComparison sc(SYLLABLE);

    Object *nextSyllable = m_doc->GetDrawingPage()->FindNextChild(&sc, syllable);
    Clef *clefPrecedingNextSyllableBefore
        = (nextSyllable) ? dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindPreviousChild(&cc, nextSyllable)) : NULL;

    // Move clef inside syllable
    element->MoveItselfTo(syllable);
    syllable->ReorderByXPos();
    parent->ClearRelinquishedChildren();
    parent->ReorderByXPos();

    // Adjust pitches of elements following inserted clef inside syllable
    if (element->Is(CLEF)) {
        Clef *clef = dynamic_cast<Clef *>(element);
        Layer *layer = dynamic_cast<Layer *>(clef->GetFirstAncestor(LAYER));
        assert(layer);
        InterfaceComparison ic(INTERFACE_PITCH);

        // Get elements after inserted clef until another inserted clef or end of syllable
        Object *nextClef = m_doc->GetDrawingPage()->FindNextChild(&cc, clef);
        Object *endElement;
        if ((nextClef) && (nextClef->GetParent() == syllable)) {
            endElement = nextClef;
        }
        else {
            endElement = (nextSyllable) ? nextSyllable : m_doc->GetDrawingPage()->GetLast();
        }
        ListOfObjects targetedElements;
        m_doc->GetDrawingPage()->FindAllDescendantsBetween(&targetedElements, &ic, clef, endElement);

        // Adjust pitches for elements in syllable affected by inserted clef
        Clef *precedingClef = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindPreviousChild(&cc, clef));
        Clef *previousClef = (precedingClef) ? precedingClef : layer->GetCurrentClef();
        for (Object *iter : targetedElements) {
            iter->GetPitchInterface()->AdjustPitchForNewClef(previousClef, clef);
        }

        // In the case of this clef moving before any clefs outside the syllable in mei
        if (nextSyllable) {
            Clef *clefPrecedingNextSyllableAfter = (nextSyllable)
                ? dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindPreviousChild(&cc, nextSyllable))
                : NULL;
            // adjust pitches following new clef accordingly
            if (clefPrecedingNextSyllableBefore != clefPrecedingNextSyllableAfter) {
                Clef *nextClef
                    = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindNextChild(&cc, clefPrecedingNextSyllableAfter));
                Object *endElement = (nextClef) ? nextClef : m_doc->GetDrawingPage()->GetLast();
                ListOfObjects followingElements;
                m_doc->GetDrawingPage()->FindAllDescendantsBetween(&followingElements, &ic, nextSyllable, endElement);

                for (Object *iter : followingElements) {
                    iter->GetPitchInterface()->AdjustPitchForNewClef(
                        clefPrecedingNextSyllableBefore, clefPrecedingNextSyllableAfter);
                }
            }
        }
    }

    m_editInfo.import("status", "OK");
    m_editInfo.import("message", "");
    return true;
}

bool EditorToolkitNeume::MoveOutsideSyllable(std::string elementId)
{
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get drawing page");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Could not get drawing page.");
        return false;
    }
    if (!m_doc->HasFacsimile()) {
        LogError("Drawing page without facsimile");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Drawing page without facsimile is unsupported.");
        return false;
    }

    Object *element = m_doc->GetDrawingPage()->FindDescendantByID(elementId);
    assert(element);
    Object *parent = element->GetParent();
    assert(parent);

    if (element == NULL) {
        LogError("No element exists with ID '%s'.", elementId.c_str());
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "No element exists with ID" + elementId + ".");
        return false;
    }
    if (!(element->Is(DIVLINE) || element->Is(ACCID) || element->Is(CLEF))) {
        LogError("Element is of type %s, but only Divlines, Accids, and Clefs can be moved out of syllables.",
            element->GetClassName().c_str());
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message",
            "Element is of type " + element->GetClassName()
                + ", but only DivLines and Accids can be inserted into syllables.");
        return false;
    }
    if (!parent->Is(SYLLABLE)) {
        LogError("The selected %s is not a child of syllable.", element->GetClassName().c_str());
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "The selected " + element->GetClassName() + "is not a child of syllable.");
        return false;
    }

    Object *secondParent = parent->GetParent();
    assert(secondParent);

    // Track variables for clef handling
    Object *fNc = parent->GetFirst(NEUME)->GetFirst(NC);
    Object *lNc;
    if (parent->GetChildCount(NEUME) == 1) {
        lNc = parent->GetFirst(NEUME)->GetLast();
    }
    else {
        lNc = parent->GetLast(NEUME)->GetLast();
    }
    int xLeft = fNc->GetFacsimileInterface()->GetZone()->GetUlx();
    int xRight = lNc->GetFacsimileInterface()->GetZone()->GetLrx();
    int xElement = element->GetFacsimileInterface()->GetZone()->GetLrx();

    // If Clef is BEFORE, INSIDE, or AFTER rest of syllable - move to mei location then use ClefMovementHandler
    if (element->Is(CLEF)) {
        Clef *clef = dynamic_cast<Clef *>(element);
        // BEFORE
        if (xElement < xLeft) {
            this->ClefMovementHandler(clef, (xLeft - xElement), 0);
        }
        // AFTER
        else if (xRight < xElement) {
            clef->GetFacsimileInterface()->GetZone()->ShiftByXY(xRight - xElement + 1, 0);
        }
        // INSIDE do nothing

        Object *syllable = parent;
        Layer *layer = dynamic_cast<Layer *>(clef->GetFirstAncestor(LAYER));
        assert(layer);

        // Restore pitches of neumes affected by clef
        ClassIdComparison cc(CLEF);
        InterfaceComparison ic(INTERFACE_PITCH);
        ClassIdComparison sc(SYLLABLE);
        // Get elements after inserted clef until another inserted clef or end of syllable
        Object *nextSyllable = m_doc->GetDrawingPage()->FindNextChild(&sc, syllable);
        Object *nextClef = m_doc->GetDrawingPage()->FindNextChild(&cc, clef);
        Object *endElement;
        if ((nextClef) && (nextClef->GetParent() == syllable)) {
            endElement = nextClef;
        }
        else {
            endElement = (nextSyllable) ? nextSyllable : m_doc->GetDrawingPage()->GetLast();
        }
        ListOfObjects targetedElements;
        m_doc->GetDrawingPage()->FindAllDescendantsBetween(&targetedElements, &ic, clef, endElement);

        Clef *precedingClef = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindPreviousChild(&cc, clef));
        precedingClef = (precedingClef != NULL) ? precedingClef : layer->GetCurrentClef();

        for (Object *iter : targetedElements) {
            iter->GetPitchInterface()->AdjustPitchForNewClef(clef, precedingClef);
        }
    }

    element->MoveItselfTo(secondParent);
    secondParent->ReorderByXPos();
    parent->ClearRelinquishedChildren();
    parent->ReorderByXPos();

    if (element->Is(CLEF)) {
        Clef *clef = dynamic_cast<Clef *>(element);
        // BEFORE
        if (xElement < xLeft) {
            this->ClefMovementHandler(clef, (xElement - xLeft), 0);
        }
        // AFTER
        else if (xRight < xElement) {
            this->ClefMovementHandler(clef, xElement - xRight - 1, 0);
        }
        // INSIDE do nothing
    }

    m_editInfo.import("status", "OK");
    m_editInfo.import("message", "");
    return true;
}

bool EditorToolkitNeume::DisplaceClefOctave(std::string elementId, std::string direction)
{
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page.");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Could not get the drawing page.");
        return false;
    }

    if (direction != "above" && direction != "below") {
        LogError("Direction can only be either \"above\" or \"below\".");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Direction can only be either \"above\" or \"below\".");
        return false;
    }

    Page *page = m_doc->GetDrawingPage();
    Object *obj = page->FindDescendantByID(elementId);
    if (obj == NULL || !obj->Is(CLEF)) {
        LogError("This action can only be done on clefs!");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "This action can only be done on clefs!");
        return false;
    }

    // Calculate new octave displacement by converting @dis and @dis.place to
    // an integer octave value.
    // Increment or decrement that integer value, then from this new octave
    // value, set @dis and @dis.place accordingly.
    Clef *clef = dynamic_cast<Clef *>(obj);
    int prevDirection = clef->GetDisPlace() == STAFFREL_basic_above ? 1 : -1;
    int move = direction == "above" ? 1 : -1;
    int octaveDis = prevDirection * (clef->GetDis() / 7) + move;

    if (octaveDis > 3 || octaveDis < -3) {
        LogError("Clefs can only be displaced 3 octaves.");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Clefs can only be displaced 3 octaves.");
        return false;
    }

    // Set clef's new octave
    if (octaveDis == 0) {
        clef->SetDis(OCTAVE_DIS_NONE);
        clef->SetDisPlace(STAFFREL_basic_NONE);
    }
    else {
        data_OCTAVE_DIS displaced = OCTAVE_DIS_NONE;
        if (octaveDis == 1 || octaveDis == -1)
            displaced = OCTAVE_DIS_8;
        else if (octaveDis == 2 || octaveDis == -2)
            displaced = OCTAVE_DIS_15;
        else if (octaveDis == 3 || octaveDis == -3)
            displaced = OCTAVE_DIS_22;

        clef->SetDis(displaced);
        clef->SetDisPlace(octaveDis > 0 ? STAFFREL_basic_above : STAFFREL_basic_below);
    }

    // Set new octaves for affected neume components and custodes
    ClassIdComparison equalsClef(CLEF);
    Clef *nextClef = dynamic_cast<Clef *>(page->FindNextChild(&equalsClef, clef));

    ClassIdComparison equalsNcs(NC);
    ListOfObjects ncs;
    page->FindAllDescendantsBetween(&ncs, &equalsNcs, clef, nextClef);
    std::for_each(ncs.begin(), ncs.end(), [&](Object *ncObj) {
        Nc *nc = dynamic_cast<Nc *>(ncObj);
        nc->SetOct(nc->GetOct() + move);
    });

    ClassIdComparison equalsCustodes(CUSTOS);
    ListOfObjects custodes;
    page->FindAllDescendantsBetween(&custodes, &equalsCustodes, clef, nextClef);
    std::for_each(custodes.begin(), custodes.end(), [&](Object *custosObj) {
        Custos *custos = dynamic_cast<Custos *>(custosObj);
        custos->SetOct(custos->GetOct() + move);
    });

    m_editInfo.import("status", "OK");
    m_editInfo.import("message", "");
    return true;
}

bool EditorToolkitNeume::MatchHeight(std::string elementId)
{
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get drawing page");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Could not get drawing page.");
        return false;
    }
    if (!m_doc->HasFacsimile()) {
        LogError("Drawing page without facsimile");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Drawing page without facsimile is unsupported.");
        return false;
    }

    Object *element = m_doc->GetDrawingPage()->FindDescendantByID(elementId);
    assert(element);
    Object *staffParent = element->GetFirstAncestor(STAFF);
    if (element == NULL) {
        LogError("No element exists with ID '%s'.", elementId.c_str());
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "No element exists with ID" + elementId + ".");
        return false;
    }
    if (!element->Is(SYL)) {
        LogError("Element is of type %s, but only <syl> element can match height.", element->GetClassName().c_str());
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import(
            "message", "Element is of type " + element->GetClassName() + ", but only <syl> element can match height.");
        return false;
    }

    // get the position of the selected bbox
    int ulx;
    int uly;
    int height;
    if (dynamic_cast<FacsimileInterface *>(element)->HasFacs()) {
        ulx = element->GetFacsimileInterface()->GetZone()->GetUlx();
        uly = element->GetFacsimileInterface()->GetZone()->GetUly();
        height = element->GetFacsimileInterface()->GetZone()->GetLry() - uly;
    }
    else {
        LogError("Selected '%s' without facsimile", element->GetClassName().c_str());
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Selected '" + element->GetClassName() + "' without facsimile is unsupported.");
        return false;
    }

    // find all syls in staff
    ListOfObjects syls;
    ClassIdComparison ac(SYL);
    staffParent->FindAllDescendantsByComparison(&syls, &ac);
    Syl *syl;
    Zone *zone;
    int itUlx;
    // int itLrx;
    int offsetY;
    // int rightMost = -1;
    double theta = (dynamic_cast<Staff *>(staffParent))->GetDrawingRotation();

    for (auto it = syls.begin(); it != syls.end(); ++it) {
        syl = dynamic_cast<Syl *>(*it);
        zone = syl->GetFacsimileInterface()->GetZone();
        assert(zone);

        // // adjust x-axis first
        // itUlx = zone->GetUlx();
        // itLrx = zone->GetLrx();
        // if (itLrx > rightMost) {
        //     // correct overlap
        //     if (itUlx < rightMost) zone->SetUlx(rightMost);
        //     // Update right most point if needed
        //     rightMost = itLrx;
        // }

        offsetY = 0;
        itUlx = zone->GetUlx();
        if (theta) {
            double factor = 1.3;
            offsetY = (int)((itUlx - ulx) * tan(theta * M_PI / 180.0) / factor);
        }

        zone->SetUly(uly + offsetY);
        zone->SetLry(uly + offsetY + height);
    }

    if (m_doc->IsTranscription() && m_doc->HasFacsimile()) m_doc->SyncFromFacsimileDoc();

    m_editInfo.import("status", "OK");
    m_editInfo.import("message", "");
    return true;
}

bool EditorToolkitNeume::Merge(std::vector<std::string> elementIds)
{
    if (!m_doc->GetDrawingPage()) return false;
    Object *page = m_doc->GetDrawingPage();

    ListOfObjects staves;

    // Get the staves by element ID and fail if a staff does not exist.
    for (auto it = elementIds.begin(); it != elementIds.end(); ++it) {
        Staff *obj = dynamic_cast<Staff *>(m_doc->GetDrawingPage()->FindDescendantByID(*it));
        if (obj != NULL && obj->Is(STAFF)) {
            staves.push_back(obj);
        }
        else {
            LogError("Staff with ID '%s' does not exist!", it->c_str());
            m_editInfo.import("status", "FAILURE");
            m_editInfo.import("message", "Staff with ID '" + *it + "' does not exist.");
            return false;
        }
    }
    if (staves.size() < 2) {
        LogError("At least two staves must be provided.");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "At least two staves must be provided.");
        return false;
    }

    // avgHeight /= staves.size();
    int ulx = dynamic_cast<Staff *>(
        *std::min_element(staves.begin(), staves.end(),
            [](Object *a, Object *b) {
                return (dynamic_cast<Staff *>(a)->GetZone()->GetUlx() < dynamic_cast<Staff *>(b)->GetZone()->GetUlx());
            }))
                  ->GetZone()
                  ->GetUlx();

    int uly = dynamic_cast<Staff *>(
        *std::min_element(staves.begin(), staves.end(),
            [](Object *a, Object *b) {
                return (dynamic_cast<Staff *>(a)->GetZone()->GetUly() < dynamic_cast<Staff *>(b)->GetZone()->GetUly());
            }))
                  ->GetZone()
                  ->GetUly();

    int lrx = dynamic_cast<Staff *>(
        *std::max_element(staves.begin(), staves.end(),
            [](Object *a, Object *b) {
                return (dynamic_cast<Staff *>(a)->GetZone()->GetLrx() < dynamic_cast<Staff *>(b)->GetZone()->GetLrx());
            }))
                  ->GetZone()
                  ->GetLrx();

    int lry = dynamic_cast<Staff *>(
        *std::max_element(staves.begin(), staves.end(),
            [](Object *a, Object *b) {
                return (dynamic_cast<Staff *>(a)->GetZone()->GetLry() < dynamic_cast<Staff *>(b)->GetZone()->GetLry());
            }))
                  ->GetZone()
                  ->GetLry();

    // Move children to the first staff (in order)
    auto stavesIt = staves.begin();
    Staff *fillStaff = dynamic_cast<Staff *>(*stavesIt);
    Layer *fillLayer = vrv_cast<Layer *>(fillStaff->GetFirst(LAYER));
    assert(fillLayer);
    stavesIt++;
    for (; stavesIt != staves.end(); ++stavesIt) {
        Staff *sourceStaff = dynamic_cast<Staff *>(*stavesIt);
        Layer *sourceLayer = vrv_cast<Layer *>(sourceStaff->GetFirst(LAYER));
        fillLayer->MoveChildrenFrom(sourceLayer);
        assert(sourceLayer->GetChildCount() == 0);

        // Delete empty staff with its system parent
        // Move SECTION, PB, and SYSTEM_MILESTONE_END if any
        Object *system = sourceStaff->GetFirstAncestor(SYSTEM);
        if (system->FindDescendantByType(SECTION)) {
            Object *section = system->FindDescendantByType(SECTION);
            Object *nextSystem = page->GetNext(system, SYSTEM);
            if (nextSystem) {
                section = system->DetachChild(section->GetIdx());
                nextSystem->InsertChild(section, 0);
            }
        }
        if (system->FindDescendantByType(PB)) {
            Object *pb = system->FindDescendantByType(PB);
            Object *nextSystem = page->GetNext(system, SYSTEM);
            if (nextSystem) {
                pb = system->DetachChild(pb->GetIdx());
                nextSystem->InsertChild(pb, 1);
            }
        }
        if (system->FindDescendantByType(SYSTEM_MILESTONE_END)) {
            Object *milestoneEnd = system->FindDescendantByType(SYSTEM_MILESTONE_END);
            Object *previousSystem = page->GetPrevious(system, SYSTEM);
            if (previousSystem) {
                milestoneEnd = system->DetachChild(milestoneEnd->GetIdx());
                previousSystem->InsertChild(milestoneEnd, previousSystem->GetChildCount());
            }
        }
        page->DeleteChild(system);
    }
    // Set the bounding box for the staff to the new bounds
    Zone *staffZone = fillStaff->GetZone();
    staffZone->SetUlx(ulx);
    staffZone->SetUly(uly);
    staffZone->SetLrx(lrx);
    staffZone->SetLry(lry);
    staffZone->SetRotate(0);

    fillLayer->ReorderByXPos();

    if (m_doc->IsTranscription() && m_doc->HasFacsimile()) m_doc->SyncFromFacsimileDoc();

    m_editInfo.import("uuid", fillStaff->GetID());
    m_editInfo.import("status", "OK");
    m_editInfo.import("message", "");

    return true;
}

bool EditorToolkitNeume::Set(std::string elementId, std::string attrType, std::string attrValue)
{
    if (!m_doc->GetDrawingPage()) return false;
    Object *element = m_doc->GetDrawingPage()->FindDescendantByID(elementId);
    bool success = false;
    if (AttModule::SetAnalytical(element, attrType, attrValue))
        success = true;
    else if (AttModule::SetCmn(element, attrType, attrValue))
        success = true;
    else if (AttModule::SetCmnornaments(element, attrType, attrValue))
        success = true;
    else if (AttModule::SetCritapp(element, attrType, attrValue))
        success = true;
    else if (AttModule::SetExternalsymbols(element, attrType, attrValue))
        success = true;
    else if (AttModule::SetGestural(element, attrType, attrValue))
        success = true;
    else if (AttModule::SetMei(element, attrType, attrValue))
        success = true;
    else if (AttModule::SetMensural(element, attrType, attrValue))
        success = true;
    else if (AttModule::SetMidi(element, attrType, attrValue))
        success = true;
    else if (AttModule::SetNeumes(element, attrType, attrValue))
        success = true;
    else if (AttModule::SetPagebased(element, attrType, attrValue))
        success = true;
    else if (AttModule::SetShared(element, attrType, attrValue))
        success = true;
    else if (AttModule::SetVisual(element, attrType, attrValue))
        success = true;

    m_doc->GetDrawingPage()->LayOutTranscription(true);
    m_editInfo.import("status", success ? "OK" : "FAILURE");
    m_editInfo.import("message", success ? "" : "Could not set attribute '" + attrType + "' to '" + attrValue + "'.");
    return success;
}

// Update the text of a TextElement by its syl
bool EditorToolkitNeume::SetText(std::string elementId, const std::string &text)
{
    std::string status = "OK", message = "";
    const std::u32string wtext = UTF8to32(text);
    if (!m_doc->GetDrawingPage()) {
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Could not find drawing page.");
        return false;
    }
    Object *element = m_doc->GetDrawingPage()->FindDescendantByID(elementId);
    if (element == NULL) {
        LogWarning("No element with ID '%s' exists", elementId.c_str());
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "No element with ID '" + elementId + "' exists.");
        return false;
    }

    bool success = false;
    if (element->Is(SYL)) {
        Syl *syl = vrv_cast<Syl *>(element);
        assert(syl);
        Object *child = syl->GetFirst();
        if (child == NULL) {
            Text *text = new Text();
            syl->AddChild(text);
            text->SetText(wtext);
            success = true;
        }
        else {
            while (child != NULL) {
                if (child->Is(TEXT)) {
                    Text *text = dynamic_cast<Text *>(child);
                    text->SetText(wtext);
                    success = true;
                    break;
                }
                else if (child->Is(REND)) {
                    Rend *rend = dynamic_cast<Rend *>(child);
                    Object *rendChild = rend->GetFirst();
                    if (rendChild->Is(TEXT)) {
                        Text *rendText = dynamic_cast<Text *>(rendChild);
                        rendText->SetText(wtext);
                        success = true;
                    }
                }
                child = syl->Object::GetNext();
            }
        }
    }
    else if (element->Is(SYLLABLE)) {
        Syllable *syllable = vrv_cast<Syllable *>(element);
        assert(syllable);
        Syl *syl = vrv_cast<Syl *>(syllable->GetFirst(SYL));
        if (syl == NULL) {
            syl = new Syl();
            Text *text = new Text();
            std::u32string str = U"";
            text->SetText(str);
            syl->AddChild(text);
            syllable->AddChild(syl);
            if (m_doc->HasFacsimile()) {
                // Create a default bounding box
                Zone *zone = new Zone();
                int ulx, uly, lrx, lry;
                if (syllable->GenerateZoneBounds(&ulx, &uly, &lrx, &lry)) {
                    // int offSetUlx = 150;
                    int offSetUly = 50;
                    int offSetLrx = 50;
                    int offSetLry = 150;
                    zone->SetUlx(ulx);
                    zone->SetUly(uly + offSetUly);
                    zone->SetLrx(lrx + offSetLrx);
                    zone->SetLry(lry + offSetLry);
                    Surface *surface = dynamic_cast<Surface *>(m_doc->GetFacsimile()->FindDescendantByType(SURFACE));
                    surface->AddChild(zone);
                    syl->AttachZone(zone);
                }
                else {
                    LogWarning("Could not create bounding box for syl.");
                    message += "Could not create bounding box for syl. ";
                    status = "WARNING";
                    delete zone;
                }
                assert(syl->HasFacs());
            }
            success = true;
        }
        else {
            success = SetText(syl->GetID(), text);
        }
    }
    else {
        LogError("Element type '%s' is unsupported for SetText", element->GetClassName().c_str());
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Element type '" + element->GetClassName() + "' is unsupported for SetText.");
        return false;
    }

    if (m_doc->IsTranscription() && m_doc->HasFacsimile()) m_doc->SyncFromFacsimileDoc();

    m_editInfo.import("status", success ? status : "FAILURE");
    m_editInfo.import("message", success ? message : "SetText method failed.");
    return success;
}

bool EditorToolkitNeume::SetClef(std::string elementId, std::string shape)
{
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page.");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Could not get the drawing page.");
        return false;
    }
    ListOfObjects objects;
    bool success = false;
    data_CLEFSHAPE clefShape = CLEFSHAPE_NONE;
    int shift = 0;
    Clef *clef = vrv_cast<Clef *>(m_doc->GetDrawingPage()->FindDescendantByID(elementId));
    assert(clef);

    if (shape == "C") {
        clefShape = CLEFSHAPE_C;
        shift = 4;
    }
    else if (shape == "F") {
        clefShape = CLEFSHAPE_F;
        shift = -4;
    }

    if (clef->GetShape() != clefShape) {
        success = AttModule::SetShared(clef, "shape", shape);
        if (!success) {
            LogError("Unable to set clef shape");
            m_editInfo.import("status", "FAILURE");
            m_editInfo.import("message", "Unable to set clef shape.");
            return false;
        }

        Layer *layer = vrv_cast<Layer *>(clef->GetFirstAncestor(LAYER));
        assert(layer);

        Object *nextClef = m_doc->GetDrawingPage()->GetNext(clef, CLEF);
        assert(nextClef);
        InterfaceComparison ic(INTERFACE_PITCH);

        m_doc->GetDrawingPage()->FindAllDescendantsBetween(
            &objects, &ic, clef, (nextClef != NULL) ? nextClef : m_doc->GetDrawingPage()->GetLast());

        // Adjust all elements who are positioned relative to clef by pitch
        for (auto it = objects.begin(); it != objects.end(); ++it) {
            Object *child = dynamic_cast<Object *>(*it);
            if (child == NULL || layer->GetClef(dynamic_cast<LayerElement *>(child)) != clef) continue;
            PitchInterface *pi = child->GetPitchInterface();
            assert(pi);
            pi->AdjustPitchByOffset(shift);
        }
    }
    m_editInfo.import("status", "OK");
    m_editInfo.import("message", "");
    return true;
}

bool EditorToolkitNeume::SetLiquescent(std::string elementId, std::string curve)
{
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page.");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Could not get the drawing page.");
        return false;
    }

    Nc *nc = vrv_cast<Nc *>(m_doc->GetDrawingPage()->FindDescendantByID(elementId));
    assert(nc);
    bool hasLiquscent = nc->GetChildCount(LIQUESCENT);

    if (curve == "a") {
        curvatureDirection_CURVE curve = curvatureDirection_CURVE_a;
        nc->SetCurve(curve);
        if (!hasLiquscent) {
            Liquescent *liquescent = new Liquescent();
            nc->AddChild(liquescent);
        }
    }
    else if (curve == "c") {
        curvatureDirection_CURVE curve = curvatureDirection_CURVE_c;
        nc->SetCurve(curve);
        if (!hasLiquscent) {
            Liquescent *liquescent = new Liquescent();
            nc->AddChild(liquescent);
        }
    }
    else {
        // For unset curve
        curvatureDirection_CURVE curve = curvatureDirection_CURVE_NONE;
        nc->SetCurve(curve);
        if (hasLiquscent) {
            Liquescent *liquescent = vrv_cast<Liquescent *>(nc->FindDescendantByType(LIQUESCENT));
            nc->DeleteChild(liquescent);
        }
    }

    m_doc->GetDrawingPage()->LayOutTranscription(true);

    m_editInfo.import("status", "OK");
    m_editInfo.import("message", "");
    return true;
}

bool EditorToolkitNeume::SetAquitanianElement(std::string elementId, std::string shape)
{
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page.");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Could not get the drawing page.");
        return false;
    }

    Nc *nc = vrv_cast<Nc *>(m_doc->GetDrawingPage()->FindDescendantByID(elementId));
    assert(nc);
    bool hasQuilisma = nc->GetChildCount(QUILISMA);
    bool hasOriscus = nc->GetChildCount(ORISCUS);

    if (shape == "quilisma") {
        if (!hasQuilisma && !hasOriscus) {
            Quilisma *quisma = new Quilisma();
            nc->AddChild(quisma);
        }
    }
    else if (shape == "oriscus") {
        if (!hasQuilisma && !hasOriscus) {
            Oriscus *oriscus = new Oriscus();
            nc->AddChild(oriscus);
        }
    }
    else {
        // For unset
        if (hasQuilisma || hasOriscus) {
            nc->DeleteChild(nc->GetFirst());
        }
    }

    m_doc->GetDrawingPage()->LayOutTranscription(true);

    m_editInfo.import("status", "OK");
    m_editInfo.import("message", "");
    return true;
}

bool EditorToolkitNeume::SortStaves()
{
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get drawing page.");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Could not get drawing page.");
        return false;
    }

    Object *page = m_doc->GetDrawingPage();
    if (page->GetChildCount(SYSTEM) <= 1) return true;

    page->StableSort(StaffSort());

    Object *pb = page->FindDescendantByType(PB);
    Object *milestoneEnd = page->FindDescendantByType(SYSTEM_MILESTONE_END);
    Object *section = page->FindDescendantByType(SECTION);
    assert(pb);
    assert(milestoneEnd);
    assert(section);

    Object *pbParent = pb->GetParent();
    Object *milestoneEndParent = milestoneEnd->GetParent();
    Object *sectionParent = section->GetParent();

    pb = pbParent->DetachChild(pb->GetIdx());
    milestoneEnd = milestoneEndParent->DetachChild(milestoneEnd->GetIdx());
    section = sectionParent->DetachChild(section->GetIdx());

    Object *firstSystem = page->GetFirst(SYSTEM);
    Object *lastSystem = page->GetLast(SYSTEM);
    assert(firstSystem);
    assert(lastSystem);

    firstSystem->InsertChild(section, 0);
    firstSystem->InsertChild(pb, 1);
    lastSystem->InsertChild(milestoneEnd, lastSystem->GetChildCount());

    return true;
}

bool EditorToolkitNeume::Split(std::string elementId, int x)
{
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Could not get the drawing page.");
        return false;
    }
    Staff *staff = dynamic_cast<Staff *>(m_doc->GetDrawingPage()->FindDescendantByID(elementId));
    // Validate parameters
    if (staff == NULL) {
        LogError("Either no element exists with ID '%s' or it is not a staff.", elementId.c_str());
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Either no element exists with ID '" + elementId + "' or it is not a staff.");
        return false;
    }

    if (staff->GetZone()->GetUlx() > x || staff->GetZone()->GetLrx() < x) {
        LogError("The 'x' parameter is not within the bounds of the original staff.");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "The 'x' parameter is not within bounds of the original staff.");
        return false;
    }

    // Resize current staff and insert new one filling remaining area.
    double theta = staff->GetDrawingRotation();
    int newUlx = x;
    int newLrx = staff->GetZone()->GetLrx();
    int newUly = staff->GetZone()->GetUly() - ((x - staff->GetZone()->GetUlx()) * tan(theta * M_PI / 180.0));
    int newLry = staff->GetZone()->GetLry(); // don't need to maintain height since we're setting rotate manually
    std::vector<std::pair<std::string, std::string>> v;

    if (!this->Insert("staff", "auto", newUlx, newUly, newLrx, newLry, v)) {
        LogError("Failed to create a second staff.");
        m_editInfo.reset();
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Failed to create a second staff.");
        return false;
    }
    Staff *splitStaff
        = dynamic_cast<Staff *>(m_doc->GetDrawingPage()->FindDescendantByID(m_editInfo.get<jsonxx::String>("uuid")));
    assert(splitStaff);
    if (splitStaff == NULL) {
        LogError("Split staff is null");
        m_editInfo.reset();
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Split staff is null.");
        return false;
    }

    splitStaff->GetZone()->SetRotate(theta);

    staff->GetZone()->SetLrx(x);
    if (theta) {
        staff->GetZone()->SetLry(staff->GetZone()->GetLry() + (newLrx - x) * tan(theta * M_PI / 180.0));
    }

    Layer *layer = vrv_cast<Layer *>(staff->GetFirst(LAYER));
    Layer *splitLayer = vrv_cast<Layer *>(splitStaff->GetFirst(LAYER));

    // Move any elements that should be on the second staff there.
    for (Object *child = layer->GetFirst(); child != NULL; child = layer->GetNext()) {
        assert(child);
        FacsimileInterface *fi = child->GetFacsimileInterface();
        if (fi == NULL || !fi->HasFacs()) {
            fi = NULL;
            ListOfObjects facsimileInterfaces;
            InterfaceComparison ic(INTERFACE_FACSIMILE);
            child->FindAllDescendantsByComparison(&facsimileInterfaces, &ic);

            for (auto it = facsimileInterfaces.begin(); it != facsimileInterfaces.end(); ++it) {
                FacsimileInterface *temp = (*it)->GetFacsimileInterface();
                assert(temp);
                if (temp->HasFacs() && (fi == NULL || temp->GetZone()->GetUlx() < fi->GetZone()->GetUlx())) {
                    fi = temp;
                }
            }
        }

        if (fi == NULL) {
            continue;
        }
        else if (fi->GetZone()->GetUlx() > x) {
            child->MoveItselfTo(splitLayer);
        }
    }
    layer->ClearRelinquishedChildren();

    if (m_doc->IsTranscription() && m_doc->HasFacsimile()) m_doc->SyncFromFacsimileDoc();

    m_editInfo.import("status", "OK");
    m_editInfo.import("message", "");
    m_editInfo.import("uuid", splitStaff->GetID());
    return true;
}

void EditorToolkitNeume::UnlinkSyllable(Syllable *syllable)
{
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page.");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Could not get the drawing page.");
        return;
    }

    assert(syllable);

    std::string linkedID = (syllable->HasPrecedes() ? syllable->GetPrecedes() : syllable->GetFollows());
    if (linkedID.compare(0, 1, "#") == 0) linkedID.erase(0, 1);
    Syllable *linkedSyllable = dynamic_cast<Syllable *>(m_doc->GetDrawingPage()->FindDescendantByID(linkedID));
    if (linkedSyllable != NULL) {
        if (linkedSyllable->HasPrecedes()) linkedSyllable->SetPrecedes("");
        if (linkedSyllable->HasFollows()) {
            linkedSyllable->SetFollows("");

            // Create an empty syl for the second part
            Syl *syl = new Syl();
            Text *text = new Text();
            std::u32string str = U"";
            text->SetText(str);
            syl->AddChild(text);
            linkedSyllable->AddChild(syl);

            // Create default bounding box if facs
            if (m_doc->HasFacsimile()) {
                Zone *zone = new Zone();
                Staff *staff = linkedSyllable->GetAncestorStaff();
                const int staffSize = m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);

                zone->SetUlx(
                    linkedSyllable->GetFirst(NEUME)->GetFirst(NC)->GetFacsimileInterface()->GetZone()->GetUlx());
                zone->SetUly(staff->GetFacsimileInterface()->GetZone()->GetLry());
                zone->SetLrx(linkedSyllable->GetLast(NEUME)->GetLast(NC)->GetFacsimileInterface()->GetZone()->GetLrx());
                zone->SetLry(zone->GetUly() + staffSize * 2);

                // Make bbox larger if it has less than 2 ncs
                if (linkedSyllable->GetChildCount(NC, 2) <= 2) {
                    zone->SetLrx(zone->GetLrx() + 50);
                }

                assert(m_doc->GetFacsimile());
                m_doc->GetFacsimile()->FindDescendantByType(SURFACE)->AddChild(zone);
                FacsimileInterface *fi = syl->GetFacsimileInterface();
                assert(fi);
                fi->AttachZone(zone);

                if (m_doc->IsTranscription() && m_doc->HasFacsimile()) m_doc->SyncFromFacsimileDoc();
            }
        }
    }
}

bool EditorToolkitNeume::Remove(std::string elementId)
{
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page.");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Could not get the drawing page.");
        return false;
    }

    Object *element = m_doc->GetDrawingPage()->FindDescendantByID(elementId);
    assert(element);
    Object *parent = element->GetParent();
    assert(parent);

    bool result = false;
    bool isNc = element->Is(NC);
    bool isNeumeOrNc = (element->Is(NEUME) || element->Is(NC));

    // Remove Zone for element (if any)
    InterfaceComparison ic(INTERFACE_FACSIMILE);
    ListOfObjects fiChildren;
    element->FindAllDescendantsByComparison(&fiChildren, &ic);
    FacsimileInterface *fi = element->GetFacsimileInterface();
    if (fi != NULL && fi->HasFacs()) {
        fi->AttachZone(NULL);
    }
    for (auto it = fiChildren.begin(); it != fiChildren.end(); ++it) {
        fi = (*it)->GetFacsimileInterface();
        if (fi != NULL && fi->HasFacs()) {
            fi->AttachZone(NULL);
        }
    }

    if (element->Is(CLEF)) {
        // y position of pitched elements (like neumes) is determined by their pitches
        // so when deleting a clef, the position on a page that a pitch value is associated with could change
        // so we need to change the pitch value of any elements whose clef is going to change
        Clef *clef = vrv_cast<Clef *>(m_doc->GetDrawingPage()->FindDescendantByID(elementId));
        assert(clef);
        ClassIdComparison ac(CLEF);
        Clef *previousClef = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindPreviousChild(&ac, clef));
        Clef *nextClef = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindNextChild(&ac, clef));

        if (previousClef == NULL) {
            // if there is no previous clef, get the default one from the staff def
            Layer *layer = vrv_cast<Layer *>(clef->GetFirstAncestor(LAYER));
            previousClef = layer->GetCurrentClef();
        }

        ListOfObjects elements;
        InterfaceComparison ic(INTERFACE_PITCH);

        m_doc->GetDrawingPage()->FindAllDescendantsBetween(
            &elements, &ic, clef, (nextClef != NULL) ? nextClef : m_doc->GetDrawingPage()->GetLast());

        result = parent->DeleteChild(element);

        if (!result) {
            LogError("Failed to delete the desired element (%s)", elementId.c_str());
            m_editInfo.reset();
            m_editInfo.import("status", "FAILURE");
            m_editInfo.import("message", "Failed to delete the desired element (" + elementId + ").");
            return false;
        }

        for (auto it = elements.begin(); it != elements.end(); ++it) {
            PitchInterface *pi = (*it)->GetPitchInterface();
            assert(pi);
            // removing the current clef, and so the new clef for all of these elements is previousClef
            pi->AdjustPitchForNewClef(clef, previousClef);
        }

        m_editInfo.import("uuid", elementId);
        m_editInfo.import("status", "OK");
        m_editInfo.import("message", "");
        return true;
    }
    else if (element->Is(STAFF)) {
        Object *page = m_doc->GetDrawingPage();
        Object *system = element->GetFirstAncestor(SYSTEM);

        if (page->GetChildCount(SYSTEM) > 1) {
            if (system == page->GetFirst(SYSTEM)) {
                // if the target staff is in the first system,
                // move pb and section to the next system
                Object *nextSystem = page->GetNext(system, SYSTEM);
                Object *section = system->FindDescendantByType(SECTION);
                Object *pb = system->FindDescendantByType(PB);
                assert(pb);
                assert(section);

                section = system->DetachChild(section->GetIdx());
                pb = system->DetachChild(pb->GetIdx());

                nextSystem->InsertChild(section, 0);
                nextSystem->InsertChild(pb, 1);
            }
            else if (system == page->GetLast(SYSTEM)) {
                // if the target staff in is the last system,
                // move system-milestone-end to the previous system
                Object *previousSystem = page->GetPrevious(system, SYSTEM);
                Object *milestoneEnd = system->FindDescendantByType(SYSTEM_MILESTONE_END);
                assert(milestoneEnd);

                int milestoneEndIdx = system->GetChildIndex(milestoneEnd);
                milestoneEnd = system->DetachChild(milestoneEndIdx);

                previousSystem->InsertChild(milestoneEnd, previousSystem->GetChildCount());
            }
        }

        // delete system to delete staff
        result = page->DeleteChild(system);

        if (!result) {
            LogError("Failed to delete the desired element (%s)", elementId.c_str());
            m_editInfo.reset();
            m_editInfo.import("status", "FAILURE");
            m_editInfo.import("message", "Failed to delete the desired element (" + elementId + ").");
            return false;
        }

        m_editInfo.import("uuid", elementId);
        m_editInfo.import("status", "OK");
        m_editInfo.import("message", "");
        return true;
    }

    if (element->Is(SYLLABLE)) {
        Syllable *syllable = dynamic_cast<Syllable *>(element);
        assert(syllable);
        if (syllable->HasPrecedes() || syllable->HasFollows()) {
            UnlinkSyllable(syllable);
        }
    }

    if (!result) {
        result = parent->DeleteChild(element);
    }

    if (!result) {
        LogError("Failed to delete the desired element (%s)", elementId.c_str());
        m_editInfo.reset();
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Failed to delete the desired element (" + elementId + ").");
        return false;
    }

    // Check if this leaves any containers empty and delete them
    if (isNc) {
        assert(parent->Is(NEUME));
        element = parent;
        parent = parent->GetParent();
        if (element->FindDescendantByType(NC) == NULL) {
            // Delete the empty neume
            std::string neumeId = element->GetID();
            result &= parent->DeleteChild(element);
            if (!result) {
                LogError("Failed to delete empty neume (%s)", neumeId.c_str());
                m_editInfo.reset();
                m_editInfo.import("status", "FAILURE");
                m_editInfo.import("message", "Failed to delete empty neume (" + neumeId + ").");
                return false;
            }
        }
    }
    if (isNeumeOrNc) {
        assert(parent->Is(SYLLABLE));
        element = parent;
        parent = parent->GetParent();
        if (element->FindDescendantByType(NC) == NULL) {
            // Check if it is part of a linked/split syllable and unlink
            Syllable *li = dynamic_cast<Syllable *>(element);
            assert(li);
            if (li->HasPrecedes() || li->HasFollows()) {
                this->UnlinkSyllable(li);
            }
            // Delete the syllable empty of neumes
            std::string syllableId = element->GetID();
            result &= parent->DeleteChild(element);
            if (!result) {
                LogError("Failed to delete empty syllable (%s)", syllableId.c_str());
                m_editInfo.reset();
                m_editInfo.import("status", "FAILURE");
                m_editInfo.import("message", "Failed to delete empty syllable (" + syllableId + ").");
                return false;
            }
        }
    }

    m_editInfo.import("uuid", elementId);
    m_editInfo.import("status", "OK");
    m_editInfo.import("message", "");
    return true;
}

bool EditorToolkitNeume::Resize(std::string elementId, int ulx, int uly, int lrx, int lry, float rotate)
{
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page.");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Could not get the drawing page.");
        return false;
    }
    if (!m_doc->HasFacsimile()) {
        LogWarning("Resizing is only available in facsimile mode.");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Resizing is only available in facsimile mode.");
        return false;
    }

    Object *obj = m_doc->GetDrawingPage()->FindDescendantByID(elementId);
    if (obj == NULL) {
        LogError("Object with ID '%s' not found.", elementId.c_str());
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Object with ID '" + elementId + "' could not be found.");
        return false;
    }
    if (obj->Is(STAFF)) {
        Staff *staff = vrv_cast<Staff *>(obj);
        assert(staff);
        if (!staff->HasFacs()) {
            LogError("This staff does not have a facsimile.");
            m_editInfo.import("status", "FAILURE");
            m_editInfo.import("message", "This staff does not have a facsimile.");
            return false;
        }
        Zone *zone = staff->GetZone();
        assert(zone);
        zone->SetUlx(ulx);
        zone->SetUly(uly);
        zone->SetLrx(lrx);
        zone->SetLry(lry);
        double orgRotate = staff->GetDrawingRotation();
        if (!isnan(rotate)) {
            zone->SetRotate(rotate);
        }
        zone->Modify();
        this->SortStaves();

        if (staff->HasDrawingRotation()) {
            ListOfObjects accids = staff->FindAllDescendantsByType(ACCID);
            for (auto it = accids.begin(); it != accids.end(); ++it) {
                Accid *accid = dynamic_cast<Accid *>(*it);
                FacsimileInterface *fi = accid->GetFacsimileInterface();
                Zone *accidZone = fi->GetZone();
                double rotationOffset = (accid->GetDrawingX() - staff->GetDrawingX()) * tan(rotate * M_PI / 180.0);
                if (orgRotate) {
                    double orgOffset = (accid->GetDrawingX() - staff->GetDrawingX()) * tan(orgRotate * M_PI / 180.0);
                    rotationOffset -= orgOffset;
                }
                accidZone->SetUly(accidZone->GetUly() + int(rotationOffset));
                accidZone->SetLry(accidZone->GetLry() + int(rotationOffset));
            }
        }
        if (m_doc->IsTranscription() && m_doc->HasFacsimile()) m_doc->SyncFromFacsimileDoc();
        m_doc->GetDrawingPage()->LayOutTranscription(true);
    }
    else if (obj->Is(SYL)) {
        Syl *syl = vrv_cast<Syl *>(obj);
        assert(syl);
        if (!syl->HasFacs()) {
            LogError("This syl (bounding box) does not have a facsimile");
            m_editInfo.import("status", "FAILURE");
            m_editInfo.import("message", "This syl does not have a facsimile.");
            return false;
        }
        Zone *zone = syl->GetZone();
        assert(zone);
        if (ulx < lrx) {
            zone->SetUlx(ulx);
            zone->SetLrx(lrx);
        }
        else {
            zone->SetUlx(lrx);
            zone->SetLrx(ulx);
        }

        if (uly < lry) {
            zone->SetUly(uly);
            zone->SetLry(lry);
        }
        else {
            zone->SetUly(lry);
            zone->SetLry(uly);
        }

        if (!isnan(rotate)) {
            zone->SetRotate(rotate);
        }
        zone->Modify();
        if (m_doc->IsTranscription() && m_doc->HasFacsimile()) m_doc->SyncFromFacsimileDoc();
    }
    else {
        LogError("Element of type '%s' is unsupported.", obj->GetClassName().c_str());
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Element of type '" + obj->GetClassName() + "' is unsupported.");
        return false;
    }

    m_editInfo.import("status", "OK");
    m_editInfo.import("message", "");
    return true;
}

bool EditorToolkitNeume::Group(std::string groupType, std::vector<std::string> elementIds)
{
    Object *parent = NULL, *secondParent = NULL;
    std::map<Object *, int> parents;
    ListOfObjects elements;
    std::vector<Object *> fullParents;
    std::map<Syllable *, Clef *> clefsBefore;

    std::string status = "OK", message = "";

    // Get the current drawing page
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page.");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Could not get the drawing page.");
        return false;
    }
    if (elementIds.size() == 0) {
        LogWarning("No element IDs to group!");
        status = "WARNING";
        message = "No element IDs to group!";
        m_editInfo.import("status", status);
        m_editInfo.import("message", message);
        return true;
    }
    ClassId elementClass;
    if (groupType == "nc") {
        elementClass = NC;
    }
    else if (groupType == "neume") {
        elementClass = NEUME;
    }
    else {
        LogError("Invalid groupType: %s", groupType.c_str());
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Invalid groupType: " + groupType);
        return false;
    }

    // Determine what the parents are
    for (auto it = elementIds.begin(); it != elementIds.end(); ++it) {
        // Verify that the children are of the same type
        Object *el = m_doc->GetDrawingPage()->FindDescendantByID(*it);
        if (el == NULL) {
            LogError("Could not get element with ID %s", it->c_str());
            m_editInfo.import("status", "FAILURE");
            m_editInfo.import("message", "Could not get element with ID " + *it);
            return false;
        }
        if (el->GetClassId() != elementClass) {
            LogError("Element %s was of class %s. Expected class %s", el->GetID().c_str(), el->GetClassName().c_str(),
                groupType.c_str());
            m_editInfo.import("status", "FAILURE");
            m_editInfo.import("message",
                "Element " + el->GetID() + " was of class " + el->GetClassName() + " but expected class " + groupType
                    + ".");
            return false;
        }

        // Get a set of parents and the number of children they have
        Object *par = el->GetParent();
        if (par == NULL) {
            LogError("Parent of %s is null!", el->GetID().c_str());
            m_editInfo.import("status", "FAILURE");
            m_editInfo.import("message", "Parent of " + el->GetID() + " is null.");
            return false;
        }

        if (par->GetClassId() == SYLLABLE) {
            // Check if it is part of a linked/split syllable
            // unlink, group into two new syllables, and link resulting syllables
            Syllable *parSyllable = dynamic_cast<Syllable *>(par);
            assert(parSyllable);
            std::string linkedID;
            if (parSyllable->HasFollows()) {
                linkedID = parSyllable->GetFollows();
                if (linkedID.compare(0, 1, "#") == 0) linkedID.erase(0, 1);

                // unlink
                this->Set(parSyllable->GetID(), "follows", "");
                this->Set(linkedID, "precedes", "");

                // group into two new syllables
                int idx = static_cast<int>(std::distance(elementIds.begin(), it));
                std::string resultId0;
                std::string resultId1;

                std::vector<std::string> elementIds0 = { elementIds.begin(), elementIds.begin() + idx };
                this->Group("neume", elementIds0);
                if (m_editInfo.get<jsonxx::String>("status") == "FAILURE") {
                    resultId0 = linkedID;
                }
                else {
                    resultId0 = m_editInfo.get<jsonxx::String>("uuid");
                }

                std::vector<std::string> elementIds1 = { elementIds.begin() + idx, elementIds.end() };
                this->Group("neume", elementIds1);
                if (m_editInfo.get<jsonxx::String>("status") == "FAILURE") {
                    resultId1 = par->GetID();
                }
                else {
                    resultId1 = m_editInfo.get<jsonxx::String>("uuid");
                    par = m_doc->GetDrawingPage()->FindDescendantByID(resultId1);
                }

                // link resulting syllables
                this->Set(resultId0, "precedes", "#" + resultId1);
                this->Set(resultId1, "follows", "#" + resultId0);
                Object *resultSyl1 = dynamic_cast<Object *>(par->FindDescendantByType(SYL));
                if (resultSyl1 != NULL) {
                    this->Remove(resultSyl1->GetID());
                }

                jsonxx::Array uuidArray;
                uuidArray << resultId0;
                uuidArray << resultId1;

                m_editInfo.import("uuid", uuidArray);
                m_editInfo.import("status", status);
                m_editInfo.import("message", message);
                return true;
            }
        }

        parents[par]++;
        elements.push_back(el);
    }

    if (parents.size() == 0) {
        LogError("Could not get the parent.");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Could not get the parent.");
        return false;
    }
    else if (parents.size() == 1) {
        LogError("The selected elements are already grouped.");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "The selected elements are already grouped.");
        return false;
    }

    ListOfObjects clefs;
    ListOfObjects syllables;
    ClassIdComparison clefComp(CLEF);
    InterfaceComparison pitchComp(INTERFACE_PITCH);
    Clef *newClef = NULL;

    m_doc->GetDrawingPage()->FindAllDescendantsBetween(
        &clefs, &clefComp, elements.front()->GetFirstAncestor(SYLLABLE), elements.back()->GetFirstAncestor(SYLLABLE));

    // if there are clefs between the elements getting grouped
    // some elements will need their pitch adjusted for the new clef
    // clefsBefore maps the syllable parent to its clef before the group
    // so we can reassociate any pitched children from their old clef to the new one
    if (clefs.size() != 0) {
        for (auto it = elements.begin(); it != elements.end(); ++it) {
            if ((*it)->Is(SYLLABLE)) {
                syllables.push_back(dynamic_cast<Object *>(*it));
            }
            else {
                syllables.push_back((*it)->GetFirstAncestor(SYLLABLE));
            }
        }
        for (auto it = syllables.begin(); it != syllables.end(); ++it) {
            Clef *tempClef = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindPreviousChild(&clefComp, (*it)));
            if (tempClef == NULL) {
                Layer *layer = vrv_cast<Layer *>((*it)->GetFirstAncestor(LAYER));
                tempClef = layer->GetCurrentClef();
            }
            clefsBefore.insert(std::pair<Syllable *, Clef *>(dynamic_cast<Syllable *>(*it), tempClef));
        }
        newClef = clefsBefore[dynamic_cast<Syllable *>(syllables.front())];
    }

    // check if share second level parent
    secondParent = (*parents.begin()).first->GetParent();
    if (secondParent == NULL) {
        LogError("No second level parent!");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "No second level parent.");
        return false;
    }
    // find parents where all of their children are being grouped
    for (auto it = parents.begin(); it != parents.end(); ++it) {
        auto parentPair = *it;
        Object *par = parentPair.first;
        int expected;
        if (par->GetParent() != secondParent) {
            LogError("No shared second level parent!");
            m_editInfo.import("status", "FAILURE");
            m_editInfo.import("message", "No shared second level parent.");
            return false;
        }
        if (par->GetClassId() == SYLLABLE) {
            expected = par->GetChildCount(NEUME);
        }
        else {
            expected = par->GetChildCount();
        }
        if (parentPair.second == expected) {
            fullParents.push_back(parentPair.first);
        }
    }

    // if there are no full parents we need to make a new one to attach everything to
    if (fullParents.empty()) {
        if (elementClass == NC) {
            parent = new Neume();
        }
        else if (elementClass == NEUME) {
            parent = new Syllable();
            Object *oldSyl = (*elements.begin())->GetFirstAncestor(SYLLABLE)->GetFirst(SYL);

            for (auto it = elements.begin(); it != elements.end(); ++it) {
                if (!(*it)->Is(SYL)) {
                    (*it)->MoveItselfTo(parent);
                }
            }

            // Ensure a syl is created so text can be added to syllable
            Syl *syl = new Syl();
            Text *text = new Text();
            std::u32string str = U"";
            text->SetText(str);
            syl->AddChild(text);
            parent->AddChild(syl);

            // add a default bounding box if you need to
            if (m_doc->HasFacsimile()) {
                Zone *zone = new Zone();

                zone->SetUlx(parent->GetFirst(NEUME)->GetFirst(NC)->GetFacsimileInterface()->GetZone()->GetUlx());
                zone->SetUly(oldSyl->GetFacsimileInterface()->GetZone()->GetUly());
                zone->SetLrx(parent->GetLast(NEUME)->GetLast(NC)->GetFacsimileInterface()->GetZone()->GetLrx());
                zone->SetLry(oldSyl->GetFacsimileInterface()->GetZone()->GetLry());

                // Make bbox larger if it has less than 2 ncs
                if (parent->GetChildCount(NC, 2) <= 2) {
                    zone->SetLrx(zone->GetLrx() + 50);
                }

                assert(m_doc->GetFacsimile());
                m_doc->GetFacsimile()->FindDescendantByType(SURFACE)->AddChild(zone);
                FacsimileInterface *fi = syl->GetFacsimileInterface();
                assert(fi);
                fi->AttachZone(zone);
            }
        }

        secondParent->AddChild(parent);
    }

    // if there's only one full parent we just add the other elements to it
    // except don't move syl tags since we want them to stay attached to the first parent
    else if (fullParents.size() == 1) {
        auto iter = fullParents.begin();
        parent = *iter;
        for (auto it = elements.begin(); it != elements.end(); ++it) {
            if ((*it)->GetParent() != parent && !(*it)->Is(SYL)) {
                (*it)->MoveItselfTo(parent);
            }
        }
    }

    // if there is more than 1 full parent we need to concat syl's
    // unless we're just grouping NC's in which case no need to worry about syl's of course
    // also in this case we need to make sure that the facsimile of the resulting syl is correct
    else {
        if (elementClass == NC) {
            parent = new Neume();
            for (auto it = elements.begin(); it != elements.end(); ++it) {
                if ((*it)->GetParent() != parent && !(*it)->Is(SYL)) {
                    (*it)->MoveItselfTo(parent);
                }
            }
            secondParent->AddChild(parent);
        }
        else {
            parent = new Syllable();
            Syl *fullSyl = NULL;
            int ulx, uly, lrx, lry;
            ulx = uly = lrx = lry = 0;

            // construct concatenated string of all the syls
            std::u32string fullString = U"";
            for (auto it = fullParents.begin(); it != fullParents.end(); ++it) {
                Syl *syl = dynamic_cast<Syl *>((*it)->FindDescendantByType(SYL));
                if (syl != NULL && m_doc->HasFacsimile()) {
                    Zone *zone = dynamic_cast<Zone *>(syl->GetFacsimileInterface()->GetZone());

                    if (fullSyl == NULL) {
                        fullSyl = syl;
                        ulx = zone->GetUlx();
                        uly = zone->GetUly();
                        lrx = zone->GetLrx();
                        lry = zone->GetLry();
                    }

                    Text *text = dynamic_cast<Text *>(syl->FindDescendantByType(TEXT));
                    if (text != NULL) {
                        std::u32string currentString = text->GetText();
                        fullString = fullString + currentString;
                    }

                    ulx = zone->GetUlx() < ulx ? zone->GetUlx() : ulx;
                    uly = zone->GetUly() < uly ? zone->GetUly() : uly;
                    lrx = zone->GetLrx() > lrx ? zone->GetLrx() : lrx;
                    lry = zone->GetLry() > lry ? zone->GetLry() : lry;
                }
            }
            // Move elements to the new group syllable
            for (auto it = elements.begin(); it != elements.end(); ++it) {
                if ((*it)->GetParent() != parent && !(*it)->Is(SYL)) {
                    (*it)->MoveItselfTo(parent);
                }
            }

            Text *fullText = dynamic_cast<Text *>(fullSyl->FindDescendantByType(TEXT));
            fullText->SetText(fullString);
            parent->AddChild(fullSyl);

            if (m_doc->HasFacsimile()) {
                Zone *zone = dynamic_cast<Zone *>(fullSyl->GetFacsimileInterface()->GetZone());
                zone->SetUlx(ulx);
                zone->SetUly(uly);
                zone->SetLrx(lrx);
                zone->SetLry(lry);
            }

            secondParent->AddChild(parent);
        }
    }

    // change the pitch of any pitched elements whose clef may have changed
    assert(newClef);
    ListOfObjects pitchedChildren;
    if (syllables.size()) {
        for (auto it = syllables.begin(); it != syllables.end(); ++it) {
            Syllable *syllable = dynamic_cast<Syllable *>(*it);
            if (clefsBefore[syllable] != newClef) {
                syllable->FindAllDescendantsByComparison(&pitchedChildren, &pitchComp);
                for (auto child = pitchedChildren.begin(); child != pitchedChildren.end(); ++child) {
                    (*child)->GetPitchInterface()->AdjustPitchForNewClef(clefsBefore[(syllable)], newClef);
                }
            }
        }
    }

    // Delete any empty parents
    for (auto it = parents.begin(); it != parents.end(); ++it) {
        Object *obj = (*it).first;
        obj->ClearRelinquishedChildren();
        if (obj->GetChildCount() == 0) {
            secondParent->DeleteChild(obj);
        }
        else if (obj->GetChildCount()
            == (obj->GetChildCount(SYL) + obj->GetChildCount(DIVLINE) + obj->GetChildCount(ACCID)
                + obj->GetChildCount(CLEF))) {
            Object *leftover;
            while ((leftover = obj->FindDescendantByType(SYL)) != NULL) {
                Zone *zone = dynamic_cast<Zone *>(leftover->GetFacsimileInterface()->GetZone());
                if (zone != NULL) {
                    m_doc->GetFacsimile()->FindDescendantByType(SURFACE)->DeleteChild(zone);
                }
                obj->DeleteChild(leftover);
            }
            while ((leftover = obj->FindDescendantByType(DIVLINE)) != NULL) {
                leftover->MoveItselfTo(parent);
                obj->ClearRelinquishedChildren();
            }
            while ((leftover = obj->FindDescendantByType(ACCID)) != NULL) {
                leftover->MoveItselfTo(parent);
                obj->ClearRelinquishedChildren();
            }
            while ((leftover = obj->FindDescendantByType(CLEF)) != NULL) {
                leftover->MoveItselfTo(parent);
                obj->ClearRelinquishedChildren();
            }
            secondParent->DeleteChild(obj);
        }
    }

    secondParent->ReorderByXPos();

    if (m_doc->IsTranscription() && m_doc->HasFacsimile()) m_doc->SyncFromFacsimileDoc();

    m_editInfo.import("uuid", parent->GetID());
    m_editInfo.import("status", status);
    m_editInfo.import("message", message);
    return true;
}

bool EditorToolkitNeume::Ungroup(std::string groupType, std::vector<std::string> elementIds)
{
    /**
     * Ungroup neume elements from a syllable (groupType=neume) or neume component elements from
     * a neume (groupType=nc). Keep the syl element attached to the original/first syllable in the
     * former, and preserve ligatures in the latter even though it is made of two neume components.
     * Exception: if the ligature is the entire neume being ungrouped then it should be toggled off
     * and the neume components ungrouped as otherwise nothing would happen.
     */
    Object *fparent = NULL; // Parent one level up (first parent)
    Object *sparent = NULL; // Parent two levels up (second parent)
    Object *currentParent = NULL;
    Object *newParent = NULL;

    Nc *firstNc = NULL;
    Nc *secondNc = NULL;
    bool success1 = false, success2 = false;
    int ligCount = 0; // for ungroup ligature into nc
    int ligNum = 0; // for ligature in ungroupNcs
    int firstIsLig = false;
    bool firstIsSyl = false;
    Zone *oldSylZone = NULL;
    Clef *oldClef = NULL;
    ClassIdComparison ac(CLEF);
    ListOfObjects syllables; // List of syllables used. groupType=neume only.

    jsonxx::Array uuidArray;
    bool breakOnEnd = false;

    // Check if you can get drawing page
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page.");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Could not get the drawing page.");
        return false;
    }

    for (auto it = elementIds.begin(); it != elementIds.end(); ++it) {
        Object *el = m_doc->GetDrawingPage()->FindDescendantByID(*it);

        // Check for ligatures and toggle them before ungrouping
        // only if the ligature is the entire selection
        if (groupType == "nc" && elementIds.size() == 2) {
            Nc *nc = dynamic_cast<Nc *>(el);
            assert(nc);
            if (nc->HasLigated() && nc->GetLigated() == BOOLEAN_true) {
                nc->SetLigated(BOOLEAN_false);
                ligCount++;
                if (ligCount == 1) {
                    firstNc = nc;
                    assert(firstNc);
                }
                else if (ligCount == 2) {
                    secondNc = nc;
                    assert(secondNc);
                    Zone *zone = new Zone();

                    Staff *staff = dynamic_cast<Staff *>(firstNc->GetFirstAncestor(STAFF));
                    assert(staff);
                    Facsimile *facsimile = m_doc->GetFacsimile();
                    assert(facsimile);
                    Surface *surface = dynamic_cast<Surface *>(facsimile->FindDescendantByType(SURFACE));
                    assert(surface);

                    const int noteHeight = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize)
                        / NOTE_HEIGHT_TO_STAFF_SIZE_RATIO);
                    const int noteWidth = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize)
                        / NOTE_WIDTH_TO_STAFF_SIZE_RATIO);

                    if (AttModule::SetNeumes(firstNc, "ligated", "false")) success1 = true;

                    int ligUlx = firstNc->GetZone()->GetUlx();
                    int ligUly = firstNc->GetZone()->GetUly();
                    int ligLrx = firstNc->GetZone()->GetLrx();
                    int ligLry = firstNc->GetZone()->GetLry();

                    zone->SetUlx(ligUlx + noteWidth);
                    zone->SetUly(ligUly + noteHeight);
                    zone->SetLrx(ligLrx + noteWidth);
                    zone->SetLry(ligLry + noteHeight);

                    Zone *origZoneUuid = secondNc->GetZone();
                    surface->DeleteChild(origZoneUuid);

                    secondNc->AttachZone(zone);
                    // secondNc->ResetFacsimile();

                    if (AttModule::SetNeumes(secondNc, "ligated", "false")) success2 = true;
                    if (success1 && success2) {
                        ligCount = 0;
                        firstNc = NULL;
                        secondNc = NULL;
                    }
                    else {
                        LogError("Unable to toggle ligature within ungroup ncs!");
                        m_editInfo.import("status", "FAILURE");
                        m_editInfo.import("message", "Unable to toggle ligature within ungroup ncs.");
                        return false;
                    }
                }
            }
        }
        while (el->Is(ACCID) || el->Is(DIVLINE) || el->Is(CLEF)) {
            fparent = el->GetFirstAncestor(SYLLABLE);
            sparent = el->GetFirstAncestor(LAYER);
            if (fparent && sparent) {
                el->MoveItselfTo(sparent);
                sparent->ReorderByXPos();
                fparent->ClearRelinquishedChildren();
                fparent->ReorderByXPos();
                uuidArray << (*it);
                it = elementIds.erase(it);
                if (it == elementIds.end()) {
                    breakOnEnd = true;
                    break;
                }
                el = m_doc->GetDrawingPage()->FindDescendantByID(*it);
            }
        }
        if (breakOnEnd) break;

        if (elementIds.begin() == it || firstIsSyl) {
            // if the element is a syl we want it to stay attached to the first element
            // we'll still need to initialize all the parents, thus the bool
            if (el->Is(SYL)) {
                firstIsSyl = true;
                oldSylZone = dynamic_cast<Zone *>(el->GetFacsimileInterface()->GetZone());
                if (oldSylZone) {
                    oldSylZone->SetLrx(oldSylZone->GetUlx() + 100);
                }
                continue;
            }
            else if (groupType == "nc") {
                // Check if the first nc is ligature
                Nc *nc = dynamic_cast<Nc *>(el);
                assert(nc);
                if (nc->HasLigated() && nc->GetLigated() == BOOLEAN_true) {
                    firstIsLig = true;
                }

                fparent = el->GetFirstAncestor(NEUME);
                assert(fparent);
                uuidArray << fparent->GetID();
                sparent = fparent->GetFirstAncestor(SYLLABLE);
                assert(sparent);
                currentParent = dynamic_cast<Neume *>(fparent);
                assert(currentParent);
                firstIsSyl = false;
            }
            else if (groupType == "neume") {
                fparent = el->GetFirstAncestor(SYLLABLE);
                assert(fparent);
                uuidArray << fparent->GetID();
                sparent = fparent->GetFirstAncestor(LAYER);
                assert(sparent);
                currentParent = vrv_cast<Syllable *>(fparent);
                assert(currentParent);
                firstIsSyl = false;

                // Get clef for clef changes
                oldClef = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindPreviousChild(&ac, currentParent));
                if (oldClef == NULL) {
                    oldClef = dynamic_cast<Layer *>(sparent)->GetCurrentClef();
                }

                // Get orginal syl zone
                if (!oldSylZone) {
                    oldSylZone = dynamic_cast<Zone *>(currentParent->GetFirst(SYL)->GetFacsimileInterface()->GetZone());
                }
            }

            else {
                LogError("Invalid groupType for ungrouping");
                m_editInfo.import("status", "FAILURE");
                m_editInfo.import("message", "Invalid groupType for ungrouping.");
                return false;
            }
        }
        else if (currentParent) {

            // if the element is a syl then we want to keep it attached to the first node
            if (el->Is(SYL)) {
                if (oldSylZone) {
                    oldSylZone->SetLrx(oldSylZone->GetUlx() + 100);
                }
                continue;
            }

            if (groupType == "nc") {
                Nc *nc = dynamic_cast<Nc *>(el);
                assert(nc);

                if (firstIsLig) {
                    // if 1st is ligature, neglect 2nd, go to the next nc
                    firstIsLig = false;
                    continue;
                }

                if (nc->HasLigated() && nc->GetLigated() == BOOLEAN_true) ligNum++;
            }

            if (ligNum != 2) {
                // if not 2nd ligature, create new parent
                newParent = currentParent->Clone();
                newParent->CloneReset();
                assert(newParent);
                newParent->ClearChildren();
            }

            el->MoveItselfTo(newParent);
            fparent->ClearRelinquishedChildren();

            if (newParent->Is(SYLLABLE)) {
                syllables.push_back(newParent);
                Syl *syl = new Syl();
                Text *text = new Text();
                std::u32string str = U"";
                text->SetText(str);
                syl->AddChild(text);
                newParent->AddChild(syl);

                // Create default bounding box if facs
                if (m_doc->HasFacsimile()) {
                    Zone *zone = new Zone();

                    zone->SetUlx(el->GetFirst(NC)->GetFacsimileInterface()->GetZone()->GetUlx());
                    zone->SetUly(oldSylZone->GetUly());
                    zone->SetLrx(el->GetLast(NC)->GetFacsimileInterface()->GetZone()->GetLrx());
                    zone->SetLry(oldSylZone->GetLry());

                    // Make bbox larger if it has less than 2 ncs
                    if (newParent->GetChildCount(NC, 2) <= 2) {
                        zone->SetLrx(zone->GetLrx() + 50);
                    }

                    assert(m_doc->GetFacsimile());
                    m_doc->GetFacsimile()->FindDescendantByType(SURFACE)->AddChild(zone);
                    FacsimileInterface *fi = syl->GetFacsimileInterface();
                    assert(fi);
                    fi->AttachZone(zone);
                }
            }

            if (ligNum != 1) {
                // if not 1st nc in ligature, add child
                uuidArray << newParent->GetID();

                sparent->AddChild(newParent);
                sparent->ReorderByXPos();
            }

            // reset ligNum
            if (ligNum == 2) ligNum = 0;
        }
    }

    // Check for clef change
    if (syllables.size() != 0) {
        ListOfObjects pitchedChildren;
        InterfaceComparison ic(INTERFACE_PITCH);
        std::vector<Object *> syllablesVector(syllables.begin(), syllables.end());
        std::stable_sort(syllablesVector.begin(), syllablesVector.end(), Object::sortByUlx);
        for (auto it = syllablesVector.begin(); it != syllablesVector.end(); ++it) {
            Clef *currentClef = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindPreviousChild(&ac, (*it)));
            if (currentClef == NULL && sparent != NULL) {
                currentClef = dynamic_cast<Layer *>(sparent)->GetCurrentClef();
            }
            if (currentClef != oldClef) {
                (*it)->FindAllDescendantsByComparison(&pitchedChildren, &ic);
                for (auto pChild = pitchedChildren.begin(); pChild != pitchedChildren.end(); ++pChild) {
                    (*pChild)->GetPitchInterface()->AdjustPitchForNewClef(oldClef, currentClef);
                }
            }
        }
    }

    if (m_doc->IsTranscription() && m_doc->HasFacsimile()) m_doc->SyncFromFacsimileDoc();

    m_editInfo.import("status", "OK");
    m_editInfo.import("message", "");
    m_editInfo.import("uuid", uuidArray);
    return true;
}

bool EditorToolkitNeume::SplitNeume(std::string neumeId, std::string ncId)
{
    // Check if you can get drawing page
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page.");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Could not get the drawing page.");
        return false;
    }

    jsonxx::Array uuidArray;

    // target nc
    Object *elNc = m_doc->GetDrawingPage()->FindDescendantByID(ncId);

    // neume: first parent
    Object *fparent = m_doc->GetDrawingPage()->FindDescendantByID(neumeId);
    assert(fparent);
    uuidArray << fparent->GetID();

    // syllable: second parent
    Object *sparent = fparent->GetFirstAncestor(SYLLABLE);
    assert(sparent);

    // get index
    int nLen = fparent->GetChildCount();
    if (nLen == 0) {
        LogError("The selected neume has no children.");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "The selected neume has no children.");
        return false;
    }

    // get the index of selected nc in the neume
    int fIdx = fparent->GetChildIndex(elNc);
    if (fIdx == -1) {
        LogError("The selected neume component is not a child of the selected neume.");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "The selected neume component is not a child of the selected neume.");
        return false;
    }
    // if click on a ligature, ncId point to the second nc in the ligature, thus minus 1
    if (elNc->HasAttribute("ligated", "true")) fIdx -= 1;

    Object *newParent = fparent->Clone();
    newParent->CloneReset();
    assert(newParent);

    // old method
    // int i = fIdx;
    // while (i > 0) {
    //     newParent->DeleteChild(newParent->GetChild(0));
    //     i -= 1;
    // }

    // i = nLen - fIdx;
    // while (i > 0) {
    //     fparent->DeleteChild(fparent->GetLast());
    //     i -= 1;
    // }

    newParent->ClearChildren();
    Object *it = fparent->GetChild(fIdx);
    while (it) {
        it->MoveItselfTo(newParent);
        fparent->ClearRelinquishedChildren();
        it = fparent->GetChild(fIdx);
    }

    // insert newParent to sparent
    sparent->InsertAfter(fparent, newParent);

    m_editInfo.import("status", "OK");
    m_editInfo.import("message", "");
    m_editInfo.import("uuid", uuidArray);
    return true;
}

bool EditorToolkitNeume::ChangeGroup(std::string elementId, std::string contour)
{
    // Check if you can get drawing page
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page.");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Could not get the drawing page.");
        return false;
    }
    Neume *el = dynamic_cast<Neume *>(m_doc->GetDrawingPage()->FindDescendantByID(elementId));
    if (el == NULL) {
        LogError("Unable to find neume with id %s", elementId.c_str());
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Unable to find neume with id " + elementId + ".");
        return false;
    }
    Nc *firstChild = NULL;
    Nc *prevNc = NULL;

    // Get children of neume. Keep the first child and delete the others.
    ListOfObjects children = el->FindAllDescendantsByType(NC);
    for (auto it = children.begin(); it != children.end(); ++it) {
        if (children.begin() == it) {
            firstChild = dynamic_cast<Nc *>(*it);
        }
        else {
            el->DeleteChild(*it);
        }
    }
    if (!firstChild) {
        LogError("Unable to find first child.");
        return false;
    }
    // Get the coordinates of the remaining child.
    int initialUlx = firstChild->GetZone()->GetUlx();
    int initialUly = firstChild->GetZone()->GetUly();
    int initialLrx = firstChild->GetZone()->GetLrx();
    int initialLry = firstChild->GetZone()->GetLry();

    Staff *staff = el->GetAncestorStaff();
    Facsimile *facsimile = m_doc->GetFacsimile();

    const int noteHeight
        = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / NOTE_HEIGHT_TO_STAFF_SIZE_RATIO);
    const int noteWidth
        = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / NOTE_WIDTH_TO_STAFF_SIZE_RATIO);
    prevNc = firstChild;

    // Iterate throught the contour and build the new grouping.
    for (auto it = contour.begin(); it != contour.end(); ++it) {
        Nc *newNc = new Nc();
        Zone *zone = new Zone();
        int newUlx = initialUlx + noteWidth;
        int newLrx = initialLrx + noteWidth;
        int newUly, newLry;

        newNc->SetPname(prevNc->GetPname());
        newNc->SetOct(prevNc->GetOct());

        if ((*it) == 'u') {
            newUly = initialUly - noteHeight;
            newLry = initialLry - noteHeight;
            newNc->AdjustPitchByOffset(1);
        }
        else if ((*it) == 'd') {
            newUly = initialUly + noteHeight;
            newLry = initialLry + noteHeight;
            newNc->AdjustPitchByOffset(-1);
        }
        else if ((*it) == 's') {
            newUly = initialUly;
            newLry = initialLry;
        }
        else {
            LogError("Unsupported character in contour.");
            delete newNc;
            delete zone;
            m_editInfo.import("status", "FAILURE");
            m_editInfo.import("message", "Unsupported character in contour.");
            return false;
        }
        zone->SetUlx(newUlx);
        zone->SetUly(newUly);
        zone->SetLrx(newLrx);
        zone->SetLry(newLry);

        newNc->AttachZone(zone);

        Surface *surface = vrv_cast<Surface *>(facsimile->FindDescendantByType(SURFACE));
        assert(surface);
        surface->AddChild(zone);

        el->AddChild(newNc);

        initialUlx = newUlx;
        initialUly = newUly;
        initialLrx = newLrx;
        initialLry = newLry;
        prevNc = newNc;
    }

    if (m_doc->IsTranscription() && m_doc->HasFacsimile()) m_doc->SyncFromFacsimileDoc();

    m_editInfo.import("uuid", el->GetID());
    m_editInfo.import("status", "OK");
    m_editInfo.import("message", "");
    return true;
}

bool EditorToolkitNeume::ToggleLigature(std::vector<std::string> elementIds)
{
    assert(elementIds.size() == 2);
    bool success1 = false;
    bool success2 = false;
    std::string firstNcId = elementIds[0];
    std::string secondNcId = elementIds[1];
    // Check if you can get drawing page
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page.");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Could not get the drawing page.");
        return false;
    }

    Nc *firstNc = vrv_cast<Nc *>(m_doc->GetDrawingPage()->FindDescendantByID(firstNcId));
    assert(firstNc);
    Nc *secondNc = vrv_cast<Nc *>(m_doc->GetDrawingPage()->FindDescendantByID(secondNcId));
    assert(secondNc);

    // check if two ncs are adjacent
    int firstIdx = firstNc->GetIdx();
    int secondIdx = secondNc->GetIdx();
    if (std::abs(firstIdx - secondIdx) != 1) {
        LogError("The selected ncs are not adjacent.");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "The selected ncs are not adjacent.");
        return false;
    }

    bool isLigature = false;
    if (firstNc->HasAttribute("ligated", "true") && secondNc->HasAttribute("ligated", "true")) {
        isLigature = true;
    }
    else {
        this->Set(firstNcId, "tilt", "");
        this->Set(secondNcId, "tilt", "");
        this->Set(firstNcId, "curve", "");
        this->Set(secondNcId, "curve", "");
    }
    // Remove liquescent if needed
    if (firstNc->GetChildCount() > 0) {
        ListOfObjects liqChildren = firstNc->FindAllDescendantsByType(LIQUESCENT);
        for (auto it = liqChildren.begin(); it != liqChildren.end(); ++it) {
            firstNc->DeleteChild(*it);
        }
    }
    if (secondNc->GetChildCount() > 0) {
        ListOfObjects liqChildren = secondNc->FindAllDescendantsByType(LIQUESCENT);
        for (auto it = liqChildren.begin(); it != liqChildren.end(); ++it) {
            secondNc->DeleteChild(*it);
        }
    }

    Zone *firstNcZone = firstNc->GetZone();
    Zone *secondNcZone = secondNc->GetZone();

    int ligUlx = firstNcZone->GetUlx();
    int ligUly = firstNcZone->GetUly();
    int ligLrx = firstNcZone->GetLrx();
    int ligLry = firstNcZone->GetLry();

    Staff *staff = dynamic_cast<Staff *>(firstNc->GetFirstAncestor(STAFF));
    assert(staff);
    const int noteHeight
        = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / NOTE_HEIGHT_TO_STAFF_SIZE_RATIO);
    const int noteWidth
        = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / NOTE_WIDTH_TO_STAFF_SIZE_RATIO);

    // set ligature to false and update zone of second Nc
    if (isLigature) {
        if (Set(firstNcId, "ligated", "false")) success1 = true;

        secondNcZone->SetUlx(ligUlx + noteWidth);
        secondNcZone->SetUly(ligUly + noteHeight);
        secondNcZone->SetLrx(ligLrx + noteWidth);
        secondNcZone->SetLry(ligLry + noteHeight);

        if (Set(secondNcId, "ligated", "false")) success2 = true;
    }
    // set ligature to true and update zones to be the same
    else {
        if (Set(firstNcId, "ligated", "true")) success1 = true;

        secondNcZone->SetUlx(ligUlx);
        secondNcZone->SetUly(ligUly + noteHeight);
        secondNcZone->SetLrx(ligLrx);
        secondNcZone->SetLry(ligLry + noteHeight);

        if (Set(secondNcId, "ligated", "true")) success2 = true;
    }

    if (!(success1 && success2)) {
        LogWarning("Unable to update ligature attribute");
        m_editInfo.import("message", "Unable to update ligature attribute.");
        m_editInfo.import("status", "WARNING");
        return false;
    }

    if (m_doc->IsTranscription() && m_doc->HasFacsimile()) m_doc->SyncFromFacsimileDoc();
    m_doc->GetDrawingPage()->LayOutTranscription(true);

    m_editInfo.import("status", "OK");
    m_editInfo.import("message", "");
    return success1 && success2;
}

bool EditorToolkitNeume::ChangeStaff(std::string elementId)
{
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Could not get the drawing page.");
        return false;
    }

    if (!m_doc->HasFacsimile()) {
        LogWarning("Staff re-association is only available in facsimile mode.");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Staff re-association is only available in facsimile mode.");
        return false;
    }

    Object *element = m_doc->GetDrawingPage()->FindDescendantByID(elementId);
    assert(element);
    if (element == NULL) {
        LogError("No element exists with ID '%s'.", elementId.c_str());
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "No element exists with ID" + elementId + ".");
        return false;
    }

    if (!(element->Is(SYLLABLE) || element->Is(CUSTOS) || element->Is(CLEF) || element->Is(DIVLINE)
            || element->Is(ACCID))) {
        LogError("Element is of type %s, but only Syllables, Custos, Clefs, Divlines, and Accids can change staves.",
            element->GetClassName().c_str());
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message",
            "Element is of type " + element->GetClassName()
                + ", but only Syllables, Custos, Clefs, DivLines, and Accids can change staves.");
        return false;
    }

    ListOfObjects stavesList = m_doc->FindAllDescendantsByType(STAFF, false);

    std::vector<Object *> staves(stavesList.begin(), stavesList.end());

    ClosestBB comp;

    if (element->GetFacsimileInterface()->HasFacs()) {
        comp.x = element->GetFacsimileInterface()->GetZone()->GetUlx();
        comp.y = element->GetFacsimileInterface()->GetZone()->GetUly();
    }
    else if (element->Is(SYLLABLE)) {
        int ulx, uly, lrx, lry;
        LayerElement *layerElement = dynamic_cast<LayerElement *>(element);
        if (!layerElement->GenerateZoneBounds(&ulx, &uly, &lrx, &lry)) {
            LogError("Couldn't generate bounding box for syllable.");
            m_editInfo.import("status", "FAILURE");
            m_editInfo.import("message", "Couldn't generate bounding box for syllable.");
            return false;
        }
        comp.x = (lrx + ulx) / 2;
        comp.y = (uly + lry) / 2;
    }
    else {
        LogError("This element does not have a facsimile.");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "This element does not have a facsimile.");
        return false;
    }

    Staff *staff = NULL;

    // find the nearest staff line
    if (staves.size() > 0) {
        std::sort(staves.begin(), staves.end(), comp);
        staff = dynamic_cast<Staff *>(staves.front());
    }
    else {
        LogError("Could not find any staves. This should not happen");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Could not find any staves. This should not happen");
        return false;
    }

    Layer *parent = vrv_cast<Layer *>(element->GetFirstAncestor(LAYER));
    Staff *sParent = vrv_cast<Staff *>(parent->GetFirstAncestor(STAFF));
    assert(parent);
    if (parent == NULL || sParent == NULL) {
        LogError("Couldn't find staff parent of element with id '%s'", elementId.c_str());
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Couldn't find staff parent of element with id " + elementId);
        return false;
    }

    Layer *layer = vrv_cast<Layer *>(staff->FindDescendantByType(LAYER));
    assert(LAYER);
    if (layer == NULL) {
        LogError("Couldn't find layer child of staff. This should not happen");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Couldn't find layer child of staff. This should not happen");
        return false;
    }

    if (layer == parent) {
        m_editInfo.import("status", "WARNING");
        m_editInfo.import("message", "Moving to the same staff as before.");
        m_editInfo.import("elementId", elementId);
        m_editInfo.import("newStaffId", staff->GetID());
        return true;
    }

    if (element->Is(ACCID) || element->Is(DIVLINE)) {
        // if accid or divLine is inside the syllable
        // move it out
        if (element->GetParent()->Is(SYLLABLE)) {
            Object *par = element->GetParent();
            assert(par);
            Object *sPar = par->GetParent();
            assert(sPar);

            element->MoveItselfTo(sPar);
            sPar->ReorderByXPos();
            par->ClearRelinquishedChildren();
            par->ReorderByXPos();
        }
    }

    // Adjust pitch/staff line.
    if (element->Is(CLEF)) {
        Clef *clef = dynamic_cast<Clef *>(element);
        assert(clef);

        Clef *previousClefBefore;
        Clef *nextClefBefore;
        ListOfObjects oldPitchChildren;
        InterfaceComparison ic(INTERFACE_PITCH);
        ClassIdComparison cic(CLEF);

        previousClefBefore = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindPreviousChild(&cic, element));

        if (previousClefBefore == NULL) {
            previousClefBefore = layer->GetCurrentClef();
        }

        nextClefBefore = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindNextChild(&cic, element));

        m_doc->GetDrawingPage()->FindAllDescendantsBetween(&oldPitchChildren, &ic, clef,
            (nextClefBefore != NULL) ? nextClefBefore : m_doc->GetDrawingPage()->GetLast());

        PitchInterface *pi;

        for (auto it = oldPitchChildren.begin(); it != oldPitchChildren.end(); ++it) {
            pi = (*it)->GetPitchInterface();
            assert(pi);
            pi->AdjustPitchForNewClef(clef, previousClefBefore);
        }

        element->MoveItselfTo(layer);
        layer->ReorderByXPos();
        parent->ClearRelinquishedChildren();
        parent->ReorderByXPos();

        // Adjust clefline
        if (!AdjustClefLineFromPosition(dynamic_cast<Clef *>(element), staff)) {
            LogError("Could not adjust clef line of %s", element->GetID().c_str());
            m_editInfo.import("status", "FAILURE");
            m_editInfo.import("message", "Failed to set clef line from facsimile.");
            return false;
        }

        // apply pitch interface changes to any elements whose clef may have changed
        ListOfObjects newPitchChildren;
        Clef *previousClefAfter = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindPreviousChild(&cic, element));
        if (previousClefAfter == NULL) {
            previousClefAfter = layer->GetCurrentClef();
        }
        Clef *nextClefAfter = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindNextChild(&cic, element));
        m_doc->GetDrawingPage()->FindAllDescendantsBetween(
            &newPitchChildren, &ic, clef, (nextClefAfter != NULL) ? nextClefAfter : m_doc->GetDrawingPage()->GetLast());

        for (auto it = newPitchChildren.begin(); it != newPitchChildren.end(); ++it) {
            pi = (*it)->GetPitchInterface();
            assert(pi);
            pi->AdjustPitchForNewClef(previousClefAfter, clef);
        }
    }
    else {
        element->MoveItselfTo(layer);
        layer->ReorderByXPos();
        parent->ClearRelinquishedChildren();
        parent->ReorderByXPos();
        if (!(element->Is(ACCID) || element->Is(DIVLINE))) {
            if (!AdjustPitchFromPosition(element)) {
                LogError("Could not adjust pitch of %s", element->GetID().c_str());
                m_editInfo.import("status", "FAILURE");
                m_editInfo.import("message", "Failed to properly set pitch.");
                m_editInfo.import("elementId", element->GetID());
                m_editInfo.import("newStaffId", staff->GetID());
                return false;
            }
        }
    }

    m_editInfo.import("status", "OK");
    m_editInfo.import("message", "");
    m_editInfo.import("elementId", elementId);
    m_editInfo.import("newStaffId", staff->GetID());
    return true;
}

bool EditorToolkitNeume::ChangeStaffTo(std::string elementId, std::string staffId)
{
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Could not get the drawing page.");
        return false;
    }

    if (!m_doc->HasFacsimile()) {
        LogWarning("Staff re-association is only available in facsimile mode.");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Staff re-association is only available in facsimile mode.");
        return false;
    }

    Object *element = m_doc->GetDrawingPage()->FindDescendantByID(elementId);
    assert(element);
    if (element == NULL) {
        LogError("No element exists with ID '%s'.", elementId.c_str());
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "No element exists with ID" + elementId + ".");
        return false;
    }

    if (!(element->Is(CLEF) || element->Is(DIVLINE) || element->Is(ACCID))) {
        LogError("Element is of type %s, but only Clefs, Divlines, and Accids can change to a specified staff.",
            element->GetClassName().c_str());
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message",
            "Element is of type " + element->GetClassName()
                + ", but only Clefs, Divlines, and Accids can change to a specified staff.");
        return false;
    }

    Staff *staff = dynamic_cast<Staff *>(m_doc->GetDrawingPage()->FindDescendantByID(staffId));

    if (!staff) {
        LogError("Could not find any staves. This should not happen");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Could not find any staves. This should not happen");
        return false;
    }

    Layer *parent = dynamic_cast<Layer *>(element->GetFirstAncestor(LAYER));
    Staff *sParent = dynamic_cast<Staff *>(parent->GetFirstAncestor(STAFF));
    assert(parent);
    if (parent == NULL || sParent == NULL) {
        LogError("Couldn't find staff parent of element with id '%s'", elementId.c_str());
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Couldn't find staff parent of element with id " + elementId);
        return false;
    }

    Layer *layer = dynamic_cast<Layer *>(staff->FindDescendantByType(LAYER));
    assert(LAYER);
    if (layer == NULL) {
        LogError("Couldn't find layer child of staff. This should not happen");
        m_editInfo.import("status", "FAILURE");
        m_editInfo.import("message", "Couldn't find layer child of staff. This should not happen");
        return false;
    }

    if (layer == parent) {
        m_editInfo.import("status", "WARNING");
        m_editInfo.import("message", "Moving to the same staff as before.");
        m_editInfo.import("elementId", elementId);
        m_editInfo.import("newStaffId", staff->GetID());
        return true;
    }

    if (element->Is(ACCID) || element->Is(DIVLINE)) {
        // if accid or divLine is inside the syllable
        // move it out
        if (element->GetParent()->Is(SYLLABLE)) {
            Object *par = element->GetParent();
            assert(par);
            Object *sPar = par->GetParent();
            assert(sPar);

            element->MoveItselfTo(sPar);
            sPar->ReorderByXPos();
            par->ClearRelinquishedChildren();
            par->ReorderByXPos();
        }
    }

    // Adjust pitch/staff line.
    if (element->Is(CLEF)) {
        Clef *clef = dynamic_cast<Clef *>(element);
        assert(clef);

        Clef *previousClefBefore;
        Clef *nextClefBefore;
        ListOfObjects oldPitchChildren;
        InterfaceComparison ic(INTERFACE_PITCH);
        ClassIdComparison cic(CLEF);

        previousClefBefore = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindPreviousChild(&cic, element));

        if (previousClefBefore == NULL) {
            previousClefBefore = layer->GetCurrentClef();
        }

        nextClefBefore = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindNextChild(&cic, element));

        m_doc->GetDrawingPage()->FindAllDescendantsBetween(&oldPitchChildren, &ic, clef,
            (nextClefBefore != NULL) ? nextClefBefore : m_doc->GetDrawingPage()->GetLast());

        PitchInterface *pi;

        for (auto it = oldPitchChildren.begin(); it != oldPitchChildren.end(); ++it) {
            pi = (*it)->GetPitchInterface();
            assert(pi);
            pi->AdjustPitchForNewClef(clef, previousClefBefore);
        }

        element->MoveItselfTo(layer);
        layer->ReorderByXPos();
        parent->ClearRelinquishedChildren();
        parent->ReorderByXPos();

        // Adjust clefline
        if (!AdjustClefLineFromPosition(dynamic_cast<Clef *>(element), staff)) {
            LogError("Could not adjust clef line of %s", element->GetID().c_str());
            m_editInfo.import("status", "FAILURE");
            m_editInfo.import("message", "Failed to set clef line from facsimile.");
            return false;
        }

        // apply pitch interface changes to any elements whose clef may have changed
        ListOfObjects newPitchChildren;
        Clef *previousClefAfter = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindPreviousChild(&cic, element));
        if (previousClefAfter == NULL) {
            previousClefAfter = layer->GetCurrentClef();
        }
        Clef *nextClefAfter = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindNextChild(&cic, element));
        m_doc->GetDrawingPage()->FindAllDescendantsBetween(
            &newPitchChildren, &ic, clef, (nextClefAfter != NULL) ? nextClefAfter : m_doc->GetDrawingPage()->GetLast());

        for (auto it = newPitchChildren.begin(); it != newPitchChildren.end(); ++it) {
            pi = (*it)->GetPitchInterface();
            assert(pi);
            pi->AdjustPitchForNewClef(previousClefAfter, clef);
        }
    }
    else {
        element->MoveItselfTo(layer);
        layer->ReorderByXPos();
        parent->ClearRelinquishedChildren();
        parent->ReorderByXPos();
    }

    m_editInfo.import("status", "OK");
    m_editInfo.import("message", "");
    m_editInfo.import("elementId", elementId);
    m_editInfo.import("newStaffId", staff->GetID());
    return true;
}

bool EditorToolkitNeume::ParseAddSylAction(jsonxx::Object param, std::string *elementId, std::string *sylText)
{
    if (!param.has<jsonxx::String>("elementId")) return false;
    (*elementId) = param.get<jsonxx::String>("elementId");
    if (!param.has<jsonxx::String>("sylText")) return false;
    (*sylText) = param.get<jsonxx::String>("sylText");
    return true;
}

bool EditorToolkitNeume::ParseDragAction(jsonxx::Object param, std::string *elementId, int *x, int *y)
{
    if (!param.has<jsonxx::String>("elementId")) return false;
    (*elementId) = param.get<jsonxx::String>("elementId");
    if (!param.has<jsonxx::Number>("x")) return false;
    (*x) = param.get<jsonxx::Number>("x");
    if (!param.has<jsonxx::Number>("y")) return false;
    (*y) = param.get<jsonxx::Number>("y");
    return true;
}

bool EditorToolkitNeume::ParseInsertAction(
    jsonxx::Object param, std::string *elementType, std::string *startId, std::string *endId)
{
    if (!param.has<jsonxx::String>("elementType")) return false;
    (*elementType) = param.get<jsonxx::String>("elementType");
    if (!param.has<jsonxx::String>("startid")) return false;
    (*startId) = param.get<jsonxx::String>("startid");
    if (!param.has<jsonxx::String>("endid")) return false;
    (*endId) = param.get<jsonxx::String>("endid");
    return true;
}

bool EditorToolkitNeume::ParseInsertAction(jsonxx::Object param, std::string *elementType, std::string *staffId,
    int *ulx, int *uly, int *lrx, int *lry, std::vector<std::pair<std::string, std::string>> *attributes)
{
    if (!param.has<jsonxx::String>("elementType")) return false;
    (*elementType) = param.get<jsonxx::String>("elementType");
    if (!param.has<jsonxx::String>("staffId")) return false;
    (*staffId) = param.get<jsonxx::String>("staffId");
    if (!param.has<jsonxx::Number>("ulx")) return false;
    (*ulx) = param.get<jsonxx::Number>("ulx");
    if (!param.has<jsonxx::Number>("uly")) return false;
    (*uly) = param.get<jsonxx::Number>("uly");
    if (param.has<jsonxx::Object>("attributes")) {
        jsonxx::Object o = param.get<jsonxx::Object>("attributes");
        auto m = o.kv_map();
        for (auto it = m.begin(); it != m.end(); ++it) {
            if (o.has<jsonxx::String>(it->first)) {
                attributes->emplace(attributes->end(), it->first, o.get<jsonxx::String>(it->first));
            }
        }
    }

    if (*elementType != "staff") {
        if (!param.has<jsonxx::Number>("lrx") || !param.has<jsonxx::Number>("lry")) {
            *lrx = -1;
            *lry = -1;
        }
    }
    else {
        if (!param.has<jsonxx::Number>("lrx")) return false;
        *lrx = param.get<jsonxx::Number>("lrx");
        if (!param.has<jsonxx::Number>("lry")) return false;
        *lry = param.get<jsonxx::Number>("lry");
    }
    return true;
}

bool EditorToolkitNeume::ParseInsertToSyllableAction(jsonxx::Object param, std::string *elementId)
{
    if (!param.has<jsonxx::String>("elementId")) return false;
    (*elementId) = param.get<jsonxx::String>("elementId");
    return true;
}

bool EditorToolkitNeume::ParseMoveOutsideSyllableAction(jsonxx::Object param, std::string *elementId)
{
    if (!param.has<jsonxx::String>("elementId")) return false;
    (*elementId) = param.get<jsonxx::String>("elementId");
    return true;
}

bool EditorToolkitNeume::ParseDisplaceClefAction(jsonxx::Object param, std::string *elementId, std::string *direction)
{
    if (!param.has<jsonxx::String>("elementId") || !param.has<jsonxx::String>("direction")) return false;
    *elementId = param.get<jsonxx::String>("elementId");
    *direction = param.get<jsonxx::String>("direction");

    return true;
}

bool EditorToolkitNeume::ParseMatchHeightAction(jsonxx::Object param, std::string *elementId)
{
    if (!param.has<jsonxx::String>("elementId")) return false;
    (*elementId) = param.get<jsonxx::String>("elementId");
    return true;
}

bool EditorToolkitNeume::ParseMergeAction(jsonxx::Object param, std::vector<std::string> *elementIds)
{
    if (!param.has<jsonxx::Array>("elementIds")) return false;
    jsonxx::Array array = param.get<jsonxx::Array>("elementIds");
    for (int i = 0; i < (int)array.size(); ++i) {
        elementIds->push_back(array.get<jsonxx::String>(i));
    }
    return true;
}

bool EditorToolkitNeume::ParseSplitAction(jsonxx::Object param, std::string *elementId, int *x)
{
    if (!param.has<jsonxx::String>("elementId")) {
        LogWarning("Could not parse 'elementId'.");
        return false;
    }
    (*elementId) = param.get<jsonxx::String>("elementId");

    if (!param.has<jsonxx::Number>("x")) {
        LogWarning("Could not parse 'x'.");
        return false;
    }
    (*x) = param.get<jsonxx::Number>("x");

    return true;
}

bool EditorToolkitNeume::ParseSetAction(
    jsonxx::Object param, std::string *elementId, std::string *attrType, std::string *attrValue)
{
    if (!param.has<jsonxx::String>("elementId")) {
        LogWarning("Could not parse 'elementId'");
        return false;
    }
    (*elementId) = param.get<jsonxx::String>("elementId");
    if (!param.has<jsonxx::String>("attrType")) {
        LogWarning("Could not parse 'attrType'");
        return false;
    }
    (*attrType) = param.get<jsonxx::String>("attrType");
    if (!param.has<jsonxx::String>("attrValue")) {
        LogWarning("Could not parse 'attrValue'");
        return false;
    }
    (*attrValue) = param.get<jsonxx::String>("attrValue");
    return true;
}

bool EditorToolkitNeume::ParseSetTextAction(jsonxx::Object param, std::string *elementId, std::string *text)
{
    if (!param.has<jsonxx::String>("elementId")) {
        LogWarning("Could not parse 'elementId'");
        return false;
    }
    *elementId = param.get<jsonxx::String>("elementId");
    if (!param.has<jsonxx::String>("text")) {
        LogWarning("Could not parse 'text'");
        return false;
    }
    *text = param.get<jsonxx::String>("text");
    return true;
}

bool EditorToolkitNeume::ParseSetClefAction(jsonxx::Object param, std::string *elementId, std::string *shape)
{
    if (!param.has<jsonxx::String>("elementId")) {
        LogWarning("Could not parse 'elementId'");
        return false;
    }
    *elementId = param.get<jsonxx::String>("elementId");
    if (!param.has<jsonxx::String>("shape")) {
        LogWarning("Could not parse 'shape'");
        return false;
    }
    *shape = param.get<jsonxx::String>("shape");
    return true;
}

bool EditorToolkitNeume::ParseSetLiquescentAction(jsonxx::Object param, std::string *elementId, std::string *curve)
{
    if (!param.has<jsonxx::String>("elementId")) {
        LogWarning("Could not parse 'elementId'");
        return false;
    }
    *elementId = param.get<jsonxx::String>("elementId");
    if (!param.has<jsonxx::String>("curve")) {
        LogWarning("Could not parse 'curve'");
        return false;
    }
    *curve = param.get<jsonxx::String>("curve");
    return true;
}

bool EditorToolkitNeume::ParseSetAquitanianElementAction(
    jsonxx::Object param, std::string *elementId, std::string *shape)
{
    if (!param.has<jsonxx::String>("elementId")) {
        LogWarning("Could not parse 'elementId'");
        return false;
    }
    *elementId = param.get<jsonxx::String>("elementId");
    if (!param.has<jsonxx::String>("shape")) {
        LogWarning("Could not parse 'shape'");
        return false;
    }
    *shape = param.get<jsonxx::String>("shape");
    return true;
}

bool EditorToolkitNeume::ParseRemoveAction(jsonxx::Object param, std::string *elementId)
{
    if (!param.has<jsonxx::String>("elementId")) return false;
    (*elementId) = param.get<jsonxx::String>("elementId");
    return true;
}

bool EditorToolkitNeume::ParseResizeAction(
    jsonxx::Object param, std::string *elementId, int *ulx, int *uly, int *lrx, int *lry)
{
    if (!param.has<jsonxx::String>("elementId")) return false;
    *elementId = param.get<jsonxx::String>("elementId");
    if (!param.has<jsonxx::Number>("ulx")) return false;
    *ulx = param.get<jsonxx::Number>("ulx");
    if (!param.has<jsonxx::Number>("uly")) return false;
    *uly = param.get<jsonxx::Number>("uly");
    if (!param.has<jsonxx::Number>("lrx")) return false;
    *lrx = param.get<jsonxx::Number>("lrx");
    if (!param.has<jsonxx::Number>("lry")) return false;
    *lry = param.get<jsonxx::Number>("lry");
    return true;
}

bool EditorToolkitNeume::ParseResizeRotateAction(
    jsonxx::Object param, std::string *elementId, int *ulx, int *uly, int *lrx, int *lry, float *rotate)
{
    if (!param.has<jsonxx::String>("elementId")) return false;
    *elementId = param.get<jsonxx::String>("elementId");
    if (!param.has<jsonxx::Number>("ulx")) return false;
    *ulx = param.get<jsonxx::Number>("ulx");
    if (!param.has<jsonxx::Number>("uly")) return false;
    *uly = param.get<jsonxx::Number>("uly");
    if (!param.has<jsonxx::Number>("lrx")) return false;
    *lrx = param.get<jsonxx::Number>("lrx");
    if (!param.has<jsonxx::Number>("lry")) return false;
    *lry = param.get<jsonxx::Number>("lry");
    if (!param.has<jsonxx::Number>("rotate")) return false;
    *rotate = param.get<jsonxx::Number>("rotate");
    return true;
}

bool EditorToolkitNeume::ParseGroupAction(
    jsonxx::Object param, std::string *groupType, std::vector<std::string> *elementIds)
{
    if (!param.has<jsonxx::String>("groupType")) return false;
    (*groupType) = param.get<jsonxx::String>("groupType");
    if (!param.has<jsonxx::Array>("elementIds")) return false;
    jsonxx::Array array = param.get<jsonxx::Array>("elementIds");
    for (int i = 0; i < (int)array.size(); ++i) {
        elementIds->push_back(array.get<jsonxx::String>(i));
    }

    return true;
}

bool EditorToolkitNeume::ParseUngroupAction(
    jsonxx::Object param, std::string *groupType, std::vector<std::string> *elementIds)
{
    if (!param.has<jsonxx::String>("groupType")) return false;
    (*groupType) = param.get<jsonxx::String>("groupType");
    if (!param.has<jsonxx::Array>("elementIds")) return false;
    jsonxx::Array array = param.get<jsonxx::Array>("elementIds");
    for (int i = 0; i < (int)array.size(); ++i) {
        elementIds->push_back(array.get<jsonxx::String>(i));
    }

    return true;
}

bool EditorToolkitNeume::ParseSplitNeumeAction(jsonxx::Object param, std::string *elementId, std::string *ncId)
{
    if (!param.has<jsonxx::String>("elementId")) {
        LogWarning("Could not parse 'elementId'.");
        return false;
    }
    (*elementId) = param.get<jsonxx::String>("elementId");

    if (!param.has<jsonxx::String>("ncId")) {
        LogWarning("Could not parse 'ncId'.");
        return false;
    }
    (*ncId) = param.get<jsonxx::String>("ncId");

    return true;
}

bool EditorToolkitNeume::ParseChangeGroupAction(jsonxx::Object param, std::string *elementId, std::string *contour)
{
    if (!param.has<jsonxx::String>("elementId")) return false;
    (*elementId) = param.get<jsonxx::String>("elementId");
    if (!param.has<jsonxx::String>("contour")) return false;
    (*contour) = param.get<jsonxx::String>("contour");
    return true;
}

bool EditorToolkitNeume::ParseToggleLigatureAction(jsonxx::Object param, std::vector<std::string> *elementIds)
{
    if (!param.has<jsonxx::Array>("elementIds")) return false;
    jsonxx::Array array = param.get<jsonxx::Array>("elementIds");
    for (int i = 0; i < (int)array.size(); ++i) {
        elementIds->push_back(array.get<jsonxx::String>(i));
    }
    return true;
}

bool EditorToolkitNeume::ParseChangeStaffAction(jsonxx::Object param, std::string *elementId)
{
    if (!param.has<jsonxx::String>("elementId")) return false;
    (*elementId) = param.get<jsonxx::String>("elementId");

    return true;
}

bool EditorToolkitNeume::ParseChangeStaffToAction(jsonxx::Object param, std::string *elementId, std::string *staffId)
{
    if (!param.has<jsonxx::String>("elementId")) return false;
    (*elementId) = param.get<jsonxx::String>("elementId");
    if (!param.has<jsonxx::String>("staffId")) return false;
    (*staffId) = param.get<jsonxx::String>("staffId");

    return true;
}

bool EditorToolkitNeume::AdjustPitchAfterDrag(Object *obj, int y)
{
    if (!obj->Is(NC) && !obj->Is(CUSTOS)) {
        LogError("AdjustPitchAfterDrag should only be called on custos or ncs."
                 "It has been called on %s, whose id is %s",
            obj->GetClassName().c_str(), obj->GetID().c_str());
        return false;
    }
    PitchInterface *pi = obj->GetPitchInterface();
    assert(pi);
    Staff *staff = dynamic_cast<Staff *>(obj->GetFirstAncestor(STAFF));
    const int staffSize = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    const int noteHeight = (int)(staffSize * 2 / NOTE_HEIGHT_TO_STAFF_SIZE_RATIO);
    const int yOffset = y > 0 ? noteHeight / 2 : -noteHeight / 2;
    const int pitchDifference = (y + yOffset) / staffSize;
    pi->AdjustPitchByOffset(pitchDifference);
    return true;
}

bool EditorToolkitNeume::AdjustPitchFromPosition(Object *obj)
{
    if (!obj->Is(CUSTOS) && !obj->Is(SYLLABLE) && !obj->Is(NEUME)) {
        LogError("AdjustPitchFromPosition should only be called on custos or syllables/neumes. Called on %s, ID: %s",
            obj->GetClassName().c_str(), obj->GetID().c_str());
        return false;
    }

    // Get parent staff and validate
    Staff *staff = dynamic_cast<Staff *>(obj->GetFirstAncestor(STAFF));
    if (!staff) {
        LogError("Object does not have a valid parent staff.");
        return false;
    }
    const int staffSize = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);

    // Helper lambda to retrieve the clef and calculate its vertical position
    auto getClefAndPosY = [this](Object *obj, Staff *staff, int staffSize, Clef *&clef, int &clefPosY) -> bool {
        clefPosY = 0;
        ClassIdComparison ac(CLEF);
        clef = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindPreviousChild(&ac, obj));

        if (!clef) {
            Layer *layer = vrv_cast<Layer *>(staff->FindDescendantByType(LAYER));
            if (!layer) {
                LogError("Unable to find layer for staff.");
                return false;
            }
            clef = layer->GetCurrentClef();
        }
        // Calculate vertical position of clef
        double staffY = staff->GetDrawingY();
        double lineSpacing = m_doc->GetDrawingDoubleUnit(staffSize);
        int lineOffset = staff->m_drawingLines - clef->GetLine();
        clefPosY = round(staffY - lineOffset * lineSpacing);
        return true;
    };

    // Helper lambda for pitch adjustment calculations
    auto adjustPitch = [this](PitchInterface *pi, FacsimileInterface *fi, Clef *clef, int clefPosY, int staffSize,
                           int baseOctave, data_PITCHNAME pname, Staff *staff) {
        if (!pi || !fi || !fi->HasFacs()) {
            LogError("Pitch adjustment failed due to missing interfaces or facsimile data.");
            return false;
        }

        pi->SetPname(pname);

        // Adjust octave based on clef displacement
        int octave = baseOctave;
        if (clef->GetDis() && clef->GetDisPlace()) {
            int direction = (clef->GetDisPlace() == STAFFREL_basic_above) ? 1 : -1;
            octave += direction * (clef->GetDis() / 7);
        }
        pi->SetOct(octave);

        // Calculate pitch difference based on vertical position
        Zone *zone = fi->GetZone();
        double rotationOffset = staff->GetDrawingRotationOffsetFor(m_view->ToLogicalX(zone->GetUlx()));
        double yPos = m_view->ToLogicalY(zone->GetUly());
        int pitchDifference = round((clefPosY - rotationOffset - yPos) / staffSize);
        pi->AdjustPitchByOffset(-pitchDifference);
        return true;
    };

    // Get clef and its position
    Clef *clef = nullptr;
    int clefPosY = 0;
    if (!getClefAndPosY(obj, staff, staffSize, clef, clefPosY)) {
        return false;
    }

    // Determine base pitch properties from clef shape
    data_PITCHNAME pname;
    int baseOctave;
    switch (clef->GetShape()) {
        case CLEFSHAPE_C:
            pname = PITCHNAME_c;
            baseOctave = 4;
            break;
        case CLEFSHAPE_F:
            pname = PITCHNAME_f;
            baseOctave = 3;
            break;
        case CLEFSHAPE_G:
            pname = PITCHNAME_g;
            baseOctave = 4;
            break;
        default:
            LogError("Clef %s does not have a valid shape: %s", clef->GetID().c_str(), clef->GetShape());
            return false;
    }

    // Handle custos directly
    if (obj->Is(CUSTOS)) {
        return adjustPitch(obj->GetPitchInterface(), obj->GetFacsimileInterface(), clef, clefPosY, staffSize,
            baseOctave, pname, staff);
    }

    // Handle syllable/neume by adjusting all pitched children
    ListOfObjects pitchedChildren;
    InterfaceComparison ic(INTERFACE_PITCH);
    obj->FindAllDescendantsByComparison(&pitchedChildren, &ic);

    if (pitchedChildren.empty()) {
        LogWarning("Syllable/neume has no pitched children: %s", obj->GetID().c_str());
        return true;
    }

    // Adjust pitch for each non-liquescent child
    for (auto *child : pitchedChildren) {
        if (!child->Is(LIQUESCENT)) {
            if (!adjustPitch(child->GetPitchInterface(), child->GetFacsimileInterface(), clef, clefPosY, staffSize,
                    baseOctave, pname, staff)) {
                return false;
            }
        }
    }
    return true;
}

bool EditorToolkitNeume::AdjustClefLineFromPosition(Clef *clef, Staff *staff)
{
    assert(clef);

    if (staff == NULL) {
        staff = clef->GetAncestorStaff();
    }

    if (!clef->HasFacs() || !staff->HasFacs()) {
        return false;
    }

    const double staffSize = m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
    const double yDiff = clef->GetZone()->GetUly() - staff->GetZone()->GetUly()
        + (clef->GetZone()->GetUlx() - staff->GetZone()->GetUlx()) * tan(staff->GetDrawingRotation() * M_PI / 180.0);
    const int clefLine = staff->m_drawingLines - round(yDiff / staffSize);
    clef->SetLine(clefLine);
    return true;
}

} // namespace vrv
