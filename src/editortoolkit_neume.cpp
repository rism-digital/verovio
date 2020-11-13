/////////////////////////////////////////////////////////////////////////////
// Name:        editortoolkit_neume.cpp
// Author:      Laurent Pugin, Juliette Regimbal, Zoe McLennan
// Created:     04/06/2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "editortoolkit_neume.h"

//--------------------------------------------------------------------------------

#include <algorithm>
#include <codecvt>
#include <limits.h>
#include <locale>
#include <math.h>
#include <set>

//--------------------------------------------------------------------------------

#include "clef.h"
#include "comparison.h"
#include "custos.h"
#include "layer.h"
#include "nc.h"
#include "neume.h"
#include "page.h"
#include "rend.h"
#include "staff.h"
#include "staffdef.h"
#include "surface.h"
#include "syl.h"
#include "syllable.h"
#include "text.h"
#include "vrv.h"

//--------------------------------------------------------------------------------

#include "jsonxx.h"

namespace vrv {

bool EditorToolkitNeume::ParseEditorAction(const std::string &json_editorAction)
{
    jsonxx::Object json;
    m_infoObject.reset();

    // Read JSON actions
    if (!json.parse(json_editorAction)) {
        LogError("Cannot parse JSON std::string.");
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "Cannot parse JSON from std::string " + json_editorAction);
        return false;
    }

    if (!json.has<jsonxx::String>("action")
        || (!json.has<jsonxx::Object>("param") && !json.has<jsonxx::Array>("param"))) {
        LogWarning("Incorrectly formatted JSON action");
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "JSON action misformatted.");
        return false;
    }

    std::string action = json.get<jsonxx::String>("action");

    if (action != "chain" && json.has<jsonxx::Array>("param")) {
        LogWarning("Only 'chain' uses 'param' as an array.");
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "'param' can only be an array for a chain action.");
        return false;
    }

    if (action == "drag") {
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
        std::vector<std::pair<std::string, std::string> > attributes;
        if (this->ParseInsertAction(
                json.get<jsonxx::Object>("param"), &elementType, &staffId, &ulx, &uly, &lrx, &lry, &attributes)) {
            return this->Insert(elementType, staffId, ulx, uly, lrx, lry, attributes);
        }
        LogWarning("Could not parse the insert action");
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
        if (this->ParseToggleLigatureAction(json.get<jsonxx::Object>("param"), &elementIds, &isLigature)) {
            return this->ToggleLigature(elementIds, isLigature);
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
    else {
        LogWarning("Unknown action type '%s'.", action.c_str());
    }
    m_infoObject.import("status", "FAILURE");
    m_infoObject.import("message", "Action " + action + " could not be parsed or is unknown.");
    return false;
}

bool EditorToolkitNeume::Chain(jsonxx::Array actions)
{
    bool status = true;
    jsonxx::Object results;
    for (int i = 0; i < (int)actions.size(); i++) {
        if (!actions.has<jsonxx::Object>(i)) {
            LogError("Action %d was not an object", i);
            m_infoObject.reset();
            m_infoObject.import("status", "FAILURE");
            m_infoObject.import("message", "Action " + std::to_string(i) + " was not an object.");
            return false;
        }
        status |= this->ParseEditorAction(actions.get<jsonxx::Object>(i).json());
        results.import(std::to_string(i), m_infoObject);
    }
    m_infoObject = results;
    return status;
}

bool EditorToolkitNeume::Drag(std::string elementId, int x, int y)
{
    std::string status = "OK", message = "";
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get drawing page.");
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "Could not get drawing page.");
        return false;
    }

    // Try to get the element on the current drawing page
    Object *element = m_doc->GetDrawingPage()->FindDescendantByUuid(elementId);

    // If it wasn't there, go back up to the whole doc
    if (!element) {
        element = m_doc->FindDescendantByUuid(elementId);
    }
    if (!element) {
        LogWarning("element is null");
        status = "WARNING";
        message += "Element is null. ";
    }
    assert(element);

    if (element->HasInterface(INTERFACE_PITCH) || element->Is(NEUME) || element->Is(SYLLABLE)) {
        Layer *layer = dynamic_cast<Layer *>(element->GetFirstAncestor(LAYER));
        if (!layer) {
            LogError("Element does not have Layer parent. This should not happen.");
            m_infoObject.import("status", "FAILURE");
            m_infoObject.import("message", "Element does not have Layer parent.");
            return false;
        }

        // clef association is done at the syllable level because of MEI structure
        // also note this will initialize syllable as null in the case of custos
        // which is why all the references to syllable are ternary
        Syllable *syllable = ((element->Is(SYLLABLE)) ? (dynamic_cast<Syllable *>(element))
                                                      : dynamic_cast<Syllable *>(element->GetFirstAncestor(SYLLABLE)));

        ClassIdComparison ac(CLEF);
        InterfaceComparison facsIC(INTERFACE_FACSIMILE);
        InterfaceComparison pitchIC(INTERFACE_PITCH);

        FacsimileInterface *fi = element->GetFacsimileInterface();
        if (fi && fi->HasFacs()) {
            bool ignoreFacs = false;
            // Dont adjust the same facsimile twice. NCs in a ligature share a single zone.
            if (element->Is(NC)) {
                Nc *nc = dynamic_cast<Nc *>(element);
                if (nc->GetLigated() == BOOLEAN_true) {
                    Neume *neume = dynamic_cast<Neume *>(nc->GetFirstAncestor(NEUME));
                    Nc *nextNc = dynamic_cast<Nc *>(neume->GetChild(1 + neume->GetChildIndex(element)));
                    if (nextNc != NULL && nextNc->GetLigated() == BOOLEAN_true && nextNc->GetZone() == nc->GetZone()) {
                        ignoreFacs = true;
                    }
                }
            }
            if (!ignoreFacs) {
                FacsimileInterface *fi = element->GetFacsimileInterface();
                assert(fi);
                Zone *zone = fi->GetZone();
                assert(zone);
                zone->ShiftByXY(x, -y);
            }
        }
        else {
            ListOfObjects facsChildren;
            element->FindAllDescendantByComparison(&facsChildren, &facsIC);
            for (auto it = facsChildren.begin(); it != facsChildren.end(); ++it) {
                // dont change the text bbox position
                if ((*it)->Is(SYL) || !(*it)->GetFacsimileInterface()->HasFacs()) {
                    continue;
                }
                (*it)->GetFacsimileInterface()->GetZone()->ShiftByXY(x, -y);
            }
        }

        layer->ReorderByXPos();

        if (element->Is(CUSTOS)) {
            AdjustPitchFromPosition(element);
        }
        else {
            AdjustPitchFromPosition(syllable);
        }
    }
    else if (element->Is(CLEF)) {
        Clef *clef = vrv_cast<Clef *>(element);
        assert(clef);
        Layer *layer = dynamic_cast<Layer *>(clef->GetFirstAncestor(LAYER));
        if (!layer) return false;

        Staff *staff = vrv_cast<Staff *>(layer->GetFirstAncestor(STAFF));
        assert(staff);
        // Note that y param is relative to initial position for clefs
        int initialClefLine = clef->GetLine();
        int clefLine = round(((double)y - x * tan(staff->GetDrawingRotate() * M_PI / 180.0))
                / (double)m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize)
            + initialClefLine);

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        // The rest of this if branch (element->Is(CLEF)) is dedicated to ensuring that pitched elements
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

        int lineDiff = clefLine - initialClefLine;

        ListOfObjects withThisClefBefore;
        ListOfObjects withPrecedingClefBefore;

        ClassIdComparison ac(CLEF);
        InterfaceComparison ic(INTERFACE_PITCH);

        Clef *precedingClefBefore = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindPreviousChild(&ac, clef));
        Clef *nextClefBefore = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindNextChild(&ac, clef));

        m_doc->GetDrawingPage()->FindAllDescendantBetween(&withThisClefBefore, &ic, clef,
            (nextClefBefore != layer->GetCurrentClef()) ? nextClefBefore : m_doc->GetDrawingPage()->GetLast());

        m_doc->GetDrawingPage()->FindAllDescendantBetween(&withPrecedingClefBefore, &ic, precedingClefBefore, clef);

        if (clef->HasFacs()) { // adjust facsimile for clef (if it exists)
            Zone *zone = clef->GetZone();
            assert(zone);
            zone->ShiftByXY(x,
                (clefLine - initialClefLine) * 2 * staff->m_drawingStaffSize
                    - x * tan(staff->GetDrawingRotate() * M_PI / 180.0));
        }

        layer->ReorderByXPos();

        Clef *precedingClefAfter = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindPreviousChild(&ac, clef));
        Clef *nextClefAfter = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindNextChild(&ac, clef));

        // case 1
        if (precedingClefAfter == precedingClefBefore && nextClefAfter == nextClefBefore) {
            ListOfObjects withThisClefAfter;
            ListOfObjects withPrecedingClefAfter;

            m_doc->GetDrawingPage()->FindAllDescendantBetween(&withThisClefAfter, &ic, clef,
                (nextClefAfter != NULL) ? nextClefAfter : m_doc->GetDrawingPage()->GetLast());
            m_doc->GetDrawingPage()->FindAllDescendantBetween(&withPrecedingClefAfter, &ic, precedingClefBefore, clef);

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

            m_doc->GetDrawingPage()->FindAllDescendantBetween(&withOldPrecedingClefAfter, &ic, precedingClefBefore,
                (nextClefBefore != NULL) ? nextClefBefore : m_doc->GetDrawingPage()->GetLast());

            m_doc->GetDrawingPage()->FindAllDescendantBetween(&withNewPrecedingClefBefore, &ic, precedingClefAfter,
                (nextClefAfter != NULL) ? nextClefAfter : m_doc->GetDrawingPage()->GetLast());

            m_doc->GetDrawingPage()->FindAllDescendantBetween(
                &withNewPrecedingClefAfter, &ic, precedingClefAfter, clef);

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
    }
    else if (element->Is(STAFF)) {
        Staff *staff = dynamic_cast<Staff *>(element);
        if (!staff->HasFacs()) {
            LogError("Staff dragging is only supported for staves with facsimiles!");
            m_infoObject.import("status", "FAILURE");
            m_infoObject.import("message", "Staff dragging is only supported for staves with facsimiles.");
            return false;
        }

        // Move staff and all staff children with facsimiles
        ListOfObjects children;
        InterfaceComparison ic(INTERFACE_FACSIMILE);
        staff->FindAllDescendantByComparison(&children, &ic);
        std::set<Zone *> zones;
        zones.insert(staff->GetZone());
        for (auto it = children.begin(); it != children.end(); ++it) {
            FacsimileInterface *fi = (*it)->GetFacsimileInterface();
            assert(fi);
            if (fi->GetZone() != NULL) zones.insert(fi->GetZone());
        }
        for (auto it = zones.begin(); it != zones.end(); ++it) {
            // Transform y to device context
            (*it)->ShiftByXY(x, -y);
        }

        staff->GetParent()->StableSort(StaffSort());

        return true; // Can't reorder by layer since staves contain layers
    }
    else if (element->Is(SYL)) {
        Syl *syl = dynamic_cast<Syl *>(element);
        if (!syl->HasFacs()) {
            LogError("Syl (boundingbox) dragging is only supported for syls with facsimiles!");
            m_infoObject.import("status", "FAILURE");
            m_infoObject.import("message", "Syl dragging is only supported for syls with facsimiles.");
            return false;
        }
        FacsimileInterface *fi = (*syl).GetFacsimileInterface();
        assert(fi);
        if (fi->GetZone() != NULL) {
            fi->GetZone()->ShiftByXY(x, -y);
        }
    }
    else {
        LogWarning("Unsupported element for dragging.");
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "Unsupported element for dragging.");
        return false;
    }
    Layer *layer = dynamic_cast<Layer *>(element->GetFirstAncestor(LAYER));
    layer->ReorderByXPos(); // Reflect position order of elements internally (and in the resulting output file)
    m_infoObject.import("status", status);
    m_infoObject.import("message", message);
    return true;
}

bool EditorToolkitNeume::Insert(std::string elementType, std::string staffId, int ulx, int uly, int lrx, int lry,
    std::vector<std::pair<std::string, std::string> > attributes)
{
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get drawing page");
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "Could not get drawing page.");
        return false;
    }
    if (m_doc->GetType() != Facs) {
        LogError("Drawing page without facsimile");
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "Drawing page without facsimile is unsupported.");
        return false;
    }

    Staff *staff;

    std::string status = "OK", message = "";

    // Find closest valid staff
    if (staffId == "auto") {
        ListOfObjects staves;
        ClassIdComparison ac(STAFF);
        m_doc->FindAllDescendantByComparison(&staves, &ac);
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
        staff = dynamic_cast<Staff *>(m_doc->FindDescendantByUuid(staffId));
    }

    Facsimile *facsimile = m_doc->GetFacsimile();
    Zone *zone = new Zone();

    if (elementType == "staff") {
        Object *parent;
        Staff *newStaff;
        // Use closest existing staff (if there is one)
        if (staff) {
            parent = staff->GetParent();
            assert(parent);
            int n = parent->GetChildCount() + 1;
            newStaff = new Staff(n);
            newStaff->m_drawingStaffDef = staff->m_drawingStaffDef;
            newStaff->m_drawingNotationType = staff->m_drawingNotationType;
            newStaff->m_drawingLines = staff->m_drawingLines;
        }
        else {
            parent = m_doc->GetDrawingPage()->FindDescendantByType(MEASURE);
            assert(parent);
            newStaff = new Staff(1);
            newStaff->m_drawingStaffDef
                = dynamic_cast<StaffDef *>(m_doc->m_mdivScoreDef.FindDescendantByType(STAFFDEF));
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
        newStaff->SetZone(zone);
        Layer *newLayer = new Layer();
        newStaff->AddChild(newLayer);

        // Find index to insert new staff
        ListOfObjects staves;
        ClassIdComparison ac(STAFF);
        parent->FindAllDescendantByComparison(&staves, &ac);
        std::vector<Object *> stavesVector(staves.begin(), staves.end());
        stavesVector.push_back(newStaff);
        StaffSort staffSort;
        std::stable_sort(stavesVector.begin(), stavesVector.end(), staffSort);
        for (int i = 0; i < (int)staves.size(); i++) {
            if (stavesVector.at(i) == newStaff) {
                newStaff->SetParent(parent);
                parent->InsertChild(newStaff, i);
                parent->Modify();

                m_infoObject.import("uuid", newStaff->GetUuid());
                m_infoObject.import("status", status);
                m_infoObject.import("message", message);

                return true;
            }
        }
        LogWarning("Failed to insert newStaff into staff");
        message += "Failed to insert newStaff into staves.";
        parent->AddChild(newStaff);
        parent->Modify();

        m_infoObject.import("uuid", newStaff->GetUuid());
        m_infoObject.import("status", status);
        m_infoObject.import("message", message);
        return true;
    }

    if (staff == NULL) {
        LogError("A staff must exist in the page to add a non-staff element.");
        delete zone;
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "A staff must exist in the page to add a non-staff element.");
        return false;
    }
    Layer *layer = vrv_cast<Layer *>(staff->FindDescendantByType(LAYER));
    assert(layer);

    if (elementType == "nc" || elementType == "grouping") {
        Syllable *syllable = new Syllable();
        Syl *syl = new Syl();
        Neume *neume = new Neume();
        Nc *nc = new Nc();
        std::string contour = "";
        nc->SetZone(zone);

        Surface *surface = dynamic_cast<Surface *>(facsimile->FindDescendantByType(SURFACE));
        surface->AddChild(zone);
        zone->SetUlx(ulx);

        neume->AddChild(nc);
        syllable->AddChild(neume);
        syllable->AddChild(syl);
        layer->AddChild(syllable);

        // add syl bounding box if Facs
        if (m_doc->GetType() == Facs) {
            FacsimileInterface *fi = vrv_cast<FacsimileInterface *>(syl->GetFacsimileInterface());
            assert(fi);
            Text *text = new Text();
            syl->AddChild(text);
            Zone *sylZone = new Zone();

            // these constants are just to improve visibility of the default boundingbox
            int offSetUlx = 150;
            int offSetUly = 50;
            int offSetLrx = 350;
            int offSetLry = 250;

            sylZone->SetUlx(ulx + offSetUlx);
            sylZone->SetUly(uly + offSetUly);
            sylZone->SetLrx(ulx + offSetLrx);
            sylZone->SetLry(uly + offSetLry);
            surface->AddChild(sylZone);
            fi->SetZone(sylZone);
        }

        const int noteHeight = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 2);
        const int noteWidth = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 1.4);
        ulx -= noteWidth / 2;
        // uly -= noteHeight / 2;
        // Set up facsimile
        zone->SetUlx(ulx);
        zone->SetUly(uly);
        zone->SetLrx(ulx + noteWidth);
        zone->SetLry(uly + noteHeight);

        layer->ReorderByXPos();

        if (!AdjustPitchFromPosition(syllable)) {
            delete syllable;
            delete neume;
            delete nc;
            LogError("Failed to set pitch.");

            m_infoObject.import("status", "FAILURE");
            m_infoObject.import("message", "Failed to set pitch.");
            return false;
        }

        // Set as inclinatum or virga (if necessary), or get contour for grouping
        for (auto it = attributes.begin(); it != attributes.end(); ++it) {
            if (it->first == "tilt") {
                if (it->second == "n") {
                    nc->SetTilt(COMPASSDIRECTION_n);
                }
                else if (it->second == "se") {
                    nc->SetTilt(COMPASSDIRECTION_se);
                }
            }
            else if (it->first == "contour") {
                contour = it->second;
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
                    m_infoObject.import("status", "FAILURE");
                    m_infoObject.import("message", "Unsupported character in contour.");
                    return false;
                }

                // Apply offset due to rotate
                newUly += (newUlx - ulx) * tan(-staff->GetDrawingRotate() * M_PI / 180.0);
                newZone->SetUlx(newUlx);
                newZone->SetUly(newUly);
                ;
                newZone->SetLrx(newUlx + noteWidth);
                newZone->SetLry(newUly + noteHeight);

                newNc->SetZone(newZone);

                assert(surface);
                surface->AddChild(newZone);

                neume->AddChild(newNc);

                ulx = newUlx;
                uly = newUly;
                prevNc = newNc;
            }
        }
        if (elementType == "nc") {
            m_infoObject.import("uuid", nc->GetUuid());
        }
        else {
            m_infoObject.import("uuid", neume->GetUuid());
        }
    }
    else if (elementType == "clef") {
        delete zone;

        Clef *clef = new Clef();
        data_CLEFSHAPE clefShape = CLEFSHAPE_NONE;

        for (auto it = attributes.begin(); it != attributes.end(); ++it) {
            if (it->first == "shape") {
                if (it->second == "C") {
                    clefShape = CLEFSHAPE_C;
                    break;
                }
                else if (it->second == "F") {
                    clefShape = CLEFSHAPE_F;
                    break;
                }
            }
        }
        if (clefShape == CLEFSHAPE_NONE) {
            LogError("A clef shape must be specified.");
            delete clef;

            m_infoObject.import("status", "FAILURE");
            m_infoObject.import("message", "A clef shape must be specified.");
            return false;
        }
        clef->SetShape(clefShape);
        const int staffSize = m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
        int yDiff = -staff->GetDrawingY() + uly;
        yDiff += ((ulx - staff->GetZone()->GetUlx()))
            * tan(-staff->GetDrawingRotate() * M_PI / 180.0); // Subtract distance due to rotate.
        int clefLine = staff->m_drawingLines - round((double)yDiff / (double)staffSize);
        clef->SetLine(clefLine);

        Zone *zone = new Zone();
        zone->SetUlx(ulx);
        zone->SetUly(uly);
        zone->SetLrx(ulx + staffSize / 1.4);
        zone->SetLry(uly + staffSize / 2);
        clef->SetZone(zone);
        Surface *surface = vrv_cast<Surface *>(facsimile->FindDescendantByType(SURFACE));
        assert(surface);
        surface->AddChild(zone);
        layer->AddChild(clef);
        m_infoObject.import("uuid", clef->GetUuid());
        layer->ReorderByXPos();

        // ensure pitched elements associated with this clef keep their x,y positions

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

        m_doc->GetDrawingPage()->FindAllDescendantBetween(
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
        Surface *surface = dynamic_cast<Surface *>(facsimile->GetFirst(SURFACE));
        surface->AddChild(zone);
        custos->SetZone(zone);
        layer->AddChild(custos);

        const int noteHeight = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 2);
        const int noteWidth = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 1.4);

        ulx -= noteWidth / 2;
        uly -= noteHeight / 2;

        zone->SetUlx(ulx);
        zone->SetUly(uly);
        zone->SetLrx(ulx + noteWidth);
        zone->SetLry(uly + noteHeight);
        layer->ReorderByXPos();
        if (!AdjustPitchFromPosition(custos)) {
            LogError("Failed to set pitch.");

            m_infoObject.import("status", "FAILURE");
            m_infoObject.import("message", "Failed to set pitch.");
            return false;
        }
        m_infoObject.import("uuid", custos->GetUuid());
    }
    else {
        delete zone;
        LogError("Unsupported type '%s' for insertion", elementType.c_str());
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "Unsupported type '" + elementType + "' for insertion.");
        return false;
    }
    layer->ReorderByXPos();
    m_infoObject.import("status", status);
    m_infoObject.import("message", message);
    return true;
}

bool EditorToolkitNeume::Merge(std::vector<std::string> elementIds)
{
    if (!m_doc->GetDrawingPage()) return false;
    ListOfObjects staves;

    // Get the staves by element ID and fail if a staff does not exist.
    for (auto it = elementIds.begin(); it != elementIds.end(); ++it) {
        Staff *obj = dynamic_cast<Staff *>(m_doc->GetDrawingPage()->FindDescendantByUuid(*it));
        if (obj != NULL && obj->Is(STAFF)) {
            staves.push_back(obj);
        }
        else {
            LogError("Staff with ID '%s' does not exist!", it->c_str());
            m_infoObject.import("status", "FAILURE");
            m_infoObject.import("message", "Staff with ID '" + *it + "' does not exist.");
            return false;
        }
    }
    if (staves.size() < 2) {
        LogError("At least two staves must be provided.");
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "At least two staves must be provided.");
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
        Layer *sourceLayer = dynamic_cast<Layer *>(sourceStaff->GetFirst(LAYER));
        fillLayer->MoveChildrenFrom(sourceLayer);
        assert(sourceLayer->GetChildCount() == 0);
        Object *parent = sourceStaff->GetParent();
        parent->DeleteChild(sourceStaff);
    }
    // Set the bounding box for the staff to the new bounds
    Zone *staffZone = fillStaff->GetZone();
    staffZone->SetUlx(ulx);
    staffZone->SetUly(uly);
    staffZone->SetLrx(lrx);
    staffZone->SetLry(lry);
    staffZone->SetRotate(0);

    fillLayer->ReorderByXPos();

    m_infoObject.import("uuid", fillStaff->GetUuid());
    m_infoObject.import("status", "OK");
    m_infoObject.import("message", "");

    // TODO change zones for staff children

    return true;
}

bool EditorToolkitNeume::Set(std::string elementId, std::string attrType, std::string attrValue)
{
    if (!m_doc->GetDrawingPage()) return false;
    Object *element = m_doc->GetDrawingPage()->FindDescendantByUuid(elementId);
    bool success = false;
    if (Att::SetAnalytical(element, attrType, attrValue))
        success = true;
    else if (Att::SetCmn(element, attrType, attrValue))
        success = true;
    else if (Att::SetCmnornaments(element, attrType, attrValue))
        success = true;
    else if (Att::SetCritapp(element, attrType, attrValue))
        success = true;
    else if (Att::SetExternalsymbols(element, attrType, attrValue))
        success = true;
    else if (Att::SetGestural(element, attrType, attrValue))
        success = true;
    else if (Att::SetMei(element, attrType, attrValue))
        success = true;
    else if (Att::SetMensural(element, attrType, attrValue))
        success = true;
    else if (Att::SetMidi(element, attrType, attrValue))
        success = true;
    else if (Att::SetNeumes(element, attrType, attrValue))
        success = true;
    else if (Att::SetPagebased(element, attrType, attrValue))
        success = true;
    else if (Att::SetShared(element, attrType, attrValue))
        success = true;
    else if (Att::SetVisual(element, attrType, attrValue))
        success = true;
    if (success && m_doc->GetType() != Facs) {
        m_doc->PrepareDrawing();
        m_doc->GetDrawingPage()->LayOut(true);
    }
    m_infoObject.import("status", success ? "OK" : "FAILURE");
    m_infoObject.import("message", success ? "" : "Could not set attribute '" + attrType + "' to '" + attrValue + "'.");
    return success;
}

// Update the text of a TextElement by its syl
bool EditorToolkitNeume::SetText(std::string elementId, std::string text)
{
    std::string status = "OK", message = "";
    std::wstring wtext;
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t> > conv;
    wtext = conv.from_bytes(text);
    if (!m_doc->GetDrawingPage()) {
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "Could not find drawing page.");
        return false;
    }
    Object *element = m_doc->GetDrawingPage()->FindDescendantByUuid(elementId);
    if (element == NULL) {
        LogWarning("No element with ID '%s' exists", elementId.c_str());
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "No element with ID '" + elementId + "' exists.");
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
        Syl *syl = dynamic_cast<Syl *>(syllable->GetFirst(SYL));
        if (syl == NULL) {
            syl = new Syl();
            syllable->AddChild(syl);
            Text *textChild = new Text();
            textChild->SetText(wtext);
            syl->AddChild(textChild);
            if (m_doc->GetType() == Facs) {
                // Create a default bounding box
                Zone *zone = new Zone();
                int ulx, uly, lrx, lry;
                if (syllable->GenerateZoneBounds(&ulx, &uly, &lrx, &lry)) {
                    zone->SetUlx(ulx);
                    zone->SetUly(uly);
                    zone->SetLrx(lrx);
                    zone->SetLry(lry);
                    Surface *surface = dynamic_cast<Surface *>(m_doc->GetFacsimile()->FindDescendantByType(SURFACE));
                    surface->AddChild(zone);
                    syl->SetZone(zone);
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
            success = SetText(syl->GetUuid(), text);
        }
    }
    else {
        LogError("Element type '%s' is unsupported for SetText", element->GetClassName().c_str());
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "Element type '" + element->GetClassName() + "' is unsupported for SetText.");
        return false;
    }
    m_infoObject.import("status", success ? status : "FAILURE");
    m_infoObject.import("message", success ? message : "SetText method failed.");
    return success;
}

bool EditorToolkitNeume::SetClef(std::string elementId, std::string shape)
{
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page.");
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "Could not get the drawing page.");
        return false;
    }
    ListOfObjects objects;
    bool success = false;
    data_CLEFSHAPE clefShape = CLEFSHAPE_NONE;
    int shift = 0;
    Clef *clef = vrv_cast<Clef *>(m_doc->GetDrawingPage()->FindDescendantByUuid(elementId));
    assert(clef);

    if (shape == "C") {
        clefShape = CLEFSHAPE_C;
        shift = -3;
    }
    else if (shape == "F") {
        clefShape = CLEFSHAPE_F;
        shift = 3;
    }

    if (clef->GetShape() != clefShape) {
        success = Att::SetShared(clef, "shape", shape);
        if (!success) {
            LogError("Unable to set clef shape");
            m_infoObject.import("status", "FAILURE");
            m_infoObject.import("message", "Unable to set clef shape.");
            return false;
        }

        Layer *layer = vrv_cast<Layer *>(clef->GetFirstAncestor(LAYER));
        assert(layer);

        Object *nextClef = m_doc->GetDrawingPage()->GetNext(clef, CLEF);
        assert(nextClef);
        InterfaceComparison ic(INTERFACE_PITCH);

        m_doc->GetDrawingPage()->FindAllDescendantBetween(
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
    if (success && m_doc->GetType() != Facs) {
        m_doc->PrepareDrawing();
        m_doc->GetDrawingPage()->LayOut(true);
    }
    m_infoObject.import("status", "OK");
    m_infoObject.import("message", "");
    return true;
}

bool EditorToolkitNeume::Split(std::string elementId, int x)
{
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page");
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "Could not get the drawing page.");
        return false;
    }
    Staff *staff = dynamic_cast<Staff *>(m_doc->GetDrawingPage()->FindDescendantByUuid(elementId));
    // Validate parameters
    if (staff == NULL) {
        LogError("Either no element exists with ID '%s' or it is not a staff.", elementId.c_str());
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "Either no element exists with ID '" + elementId + "' or it is not a staff.");
        return false;
    }

    if (staff->GetZone()->GetUlx() > x || staff->GetZone()->GetLrx() < x) {
        LogError("The 'x' parameter is not within the bounds of the original staff.");
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "The 'x' parameter is not within bounds of the original staff.");
        return false;
    }

    // Resize current staff and insert new one filling remaining area.
    int newUlx = x;
    int newLrx = staff->GetZone()->GetLrx();
    int newUly = staff->GetZone()->GetUly()
        - ((x - staff->GetZone()->GetUlx()) * tan(staff->GetZone()->GetRotate() * M_PI / 180.0));
    int newLry = staff->GetZone()->GetLry(); // don't need to maintain height since we're setting rotate manually
    std::vector<std::pair<std::string, std::string> > v;

    if (!this->Insert("staff", "auto", newUlx, newUly, newLrx, newLry, v)) {
        LogError("Failed to create a second staff.");
        m_infoObject.reset();
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "Failed to create a second staff.");
        return false;
    }
    Staff *splitStaff = dynamic_cast<Staff *>(
        m_doc->GetDrawingPage()->FindDescendantByUuid(m_infoObject.get<jsonxx::String>("uuid")));
    assert(splitStaff);
    if (splitStaff == NULL) {
        LogError("Split staff is null");
        m_infoObject.reset();
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "Split staff is null.");
        return false;
    }

    splitStaff->GetZone()->SetRotate(staff->GetZone()->GetRotate());

    staff->GetZone()->SetLrx(x);
    if (staff->GetZone()->GetRotate() != 0) {
        staff->GetZone()->SetLry(
            staff->GetZone()->GetLry() + (newLrx - x) * tan(staff->GetZone()->GetRotate() * M_PI / 180.0));
    }

    Layer *layer = dynamic_cast<Layer *>(staff->GetFirst(LAYER));
    Layer *splitLayer = dynamic_cast<Layer *>(splitStaff->GetFirst(LAYER));

    // Move any elements that should be on the second staff there.
    for (Object *child = layer->GetFirst(); child != NULL; child = layer->GetNext()) {
        assert(child);
        FacsimileInterface *fi = dynamic_cast<FacsimileInterface *>(child);
        if (fi == NULL || !fi->HasFacs()) {
            fi = NULL;
            ListOfObjects facsimileInterfaces;
            InterfaceComparison ic(INTERFACE_FACSIMILE);
            child->FindAllDescendantByComparison(&facsimileInterfaces, &ic);

            for (auto it = facsimileInterfaces.begin(); it != facsimileInterfaces.end(); ++it) {
                FacsimileInterface *temp = dynamic_cast<FacsimileInterface *>(*it);
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
    m_infoObject.import("status", "OK");
    m_infoObject.import("message", "");
    m_infoObject.import("uuid", splitStaff->GetUuid());
    return true;
}

bool EditorToolkitNeume::Remove(std::string elementId)
{
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page.");
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "Could not get the drawing page.");
        return false;
    }
    Object *obj = m_doc->GetDrawingPage()->FindDescendantByUuid(elementId);
    assert(obj);
    bool result = false;
    bool isNeumeOrNc, isNc, isClef;
    isNeumeOrNc = (obj->Is(NC) || obj->Is(NEUME));
    isNc = obj->Is(NC);
    isClef = obj->Is(CLEF);
    Object *parent = obj->GetParent();
    assert(parent);
    m_infoObject.import("uuid", elementId);
    // Remove Zone for element (if any)
    InterfaceComparison ic(INTERFACE_FACSIMILE);
    ListOfObjects fiChildren;
    obj->FindAllDescendantByComparison(&fiChildren, &ic);
    FacsimileInterface *fi = dynamic_cast<FacsimileInterface *>(obj);
    if (fi != NULL && fi->HasFacs()) {
        fi->SetZone(NULL);
    }
    for (auto it = fiChildren.begin(); it != fiChildren.end(); ++it) {
        fi = dynamic_cast<FacsimileInterface *>(*it);
        if (fi != NULL && fi->HasFacs()) {
            fi->SetZone(NULL);
        }
    }
    if (isClef) {
        // y position of pitched elements (like neumes) is determined by their pitches
        // so when deleting a clef, the position on a page that a pitch value is associated with could change
        // so we need to change the pitch value of any elements whose clef is going to change
        Clef *clef = vrv_cast<Clef *>(m_doc->GetDrawingPage()->FindDescendantByUuid(elementId));
        assert(clef);
        ClassIdComparison ac(CLEF);
        Clef *previousClef = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindPreviousChild(&ac, clef));
        Clef *nextClef = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindNextChild(&ac, clef));

        if (previousClef == NULL) {
            // if there is no previous clef, get the default one from the staff def
            Layer *layer = dynamic_cast<Layer *>(clef->GetFirstAncestor(LAYER));
            previousClef = layer->GetCurrentClef();
        }

        ListOfObjects elements;
        InterfaceComparison ic(INTERFACE_PITCH);

        m_doc->GetDrawingPage()->FindAllDescendantBetween(
            &elements, &ic, clef, (nextClef != NULL) ? nextClef : m_doc->GetDrawingPage()->GetLast());

        result = parent->DeleteChild(obj);

        if (!result) {
            LogError("Failed to delete the desired element (%s)", elementId.c_str());
            m_infoObject.reset();
            m_infoObject.import("status", "FAILURE");
            m_infoObject.import("message", "Failed to delete the desired element (" + elementId + ").");
            return false;
        }

        for (auto it = elements.begin(); it != elements.end(); ++it) {
            PitchInterface *pi = (*it)->GetPitchInterface();
            assert(pi);
            // removing the current clef, and so the new clef for all of these elements is previousClef
            pi->AdjustPitchForNewClef(clef, previousClef);
        }
    }

    if (!result) {
        result = parent->DeleteChild(obj);
    }

    if (!result) {
        LogError("Failed to delete the desired element (%s)", elementId.c_str());
        m_infoObject.reset();
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "Failed to delete the desired element (" + elementId + ").");
        return false;
    }
    // Check if this leaves any containers empty and delete them
    if (isNc && result) {
        assert(parent->Is(NEUME));
        obj = parent;
        parent = parent->GetParent();
        if (obj->FindDescendantByType(NC) == NULL) {
            // Delete the empty neume
            std::string neumeId = obj->GetUuid();
            result &= parent->DeleteChild(obj);
            if (!result) {
                LogError("Failed to delete empty neume (%s)", neumeId.c_str());
                m_infoObject.reset();
                m_infoObject.import("status", "FAILURE");
                m_infoObject.import("message", "Failed to delete empty neume (" + neumeId + ").");
                return false;
            }
        }
    }
    if (isNeumeOrNc && result) {
        assert(parent->Is(SYLLABLE));
        obj = parent;
        parent = parent->GetParent();
        if (obj->FindDescendantByType(NC) == NULL) {
            // Delete the syllable empty of neumes
            std::string syllableId = obj->GetUuid();
            result &= parent->DeleteChild(obj);
            if (!result) {
                LogError("Failed to delete empty syllable (%s)", syllableId.c_str());
                m_infoObject.reset();
                m_infoObject.import("status", "FAILURE");
                m_infoObject.import("message", "Failed to delete empty syllable (" + syllableId + ").");
                return false;
            }
        }
    }

    m_infoObject.import("status", "OK");
    m_infoObject.import("message", "");
    return true;
}

bool EditorToolkitNeume::Resize(std::string elementId, int ulx, int uly, int lrx, int lry, float rotate)
{
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page.");
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "Could not get the drawing page.");
        return false;
    }
    if (m_doc->GetType() != Facs) {
        LogWarning("Resizing is only available in facsimile mode.");
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "Resizing is only available in facsimile mode.");
        return false;
    }

    Object *obj = m_doc->GetDrawingPage()->FindDescendantByUuid(elementId);
    if (obj == NULL) {
        LogError("Object with ID '%s' not found.", elementId.c_str());
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "Object with ID '" + elementId + "' could not be found.");
        return false;
    }
    if (obj->Is(STAFF)) {
        Staff *staff = vrv_cast<Staff *>(obj);
        assert(staff);
        if (!staff->HasFacs()) {
            LogError("This staff does not have a facsimile.");
            m_infoObject.import("status", "FAILURE");
            m_infoObject.import("message", "This staff does not have a facsimile.");
            return false;
        }
        Zone *zone = staff->GetZone();
        assert(zone);
        zone->SetUlx(ulx);
        zone->SetUly(uly);
        zone->SetLrx(lrx);
        zone->SetLry(lry);
        if (!isnan(rotate)) {
            zone->SetRotate(rotate);
        }
        zone->Modify();
        staff->GetParent()->StableSort(StaffSort());
    }
    else if (obj->Is(SYL)) {
        Syl *syl = vrv_cast<Syl *>(obj);
        assert(syl);
        if (!syl->HasFacs()) {
            LogError("This syl (bounding box) does not have a facsimile");
            m_infoObject.import("status", "FAILURE");
            m_infoObject.import("message", "This syl does not have a facsimile.");
            return false;
        }
        Zone *zone = syl->GetZone();
        assert(zone);
        zone->SetUlx(ulx);
        zone->SetUly(uly);
        zone->SetLrx(lrx);
        zone->SetLry(lry);
        if (!isnan(rotate)) {
            zone->SetRotate(rotate);
        }
        zone->Modify();
    }
    else {
        LogError("Element of type '%s' is unsupported.", obj->GetClassName().c_str());
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "Element of type '" + obj->GetClassName() + "' is unsupported.");
        return false;
    }
    m_infoObject.import("status", "OK");
    m_infoObject.import("message", "");
    return true;
}

bool EditorToolkitNeume::Group(std::string groupType, std::vector<std::string> elementIds)
{
    Object *parent = NULL, *doubleParent = NULL;
    std::map<Object *, int> parents;
    std::set<Object *> elements;
    std::vector<Object *> sortedElements;
    std::vector<Object *> fullParents;
    std::map<Syllable *, Clef *> clefsBefore;

    std::string status = "OK", message = "";

    // Get the current drawing page
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page.");
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "Could not get the drawing page.");
        return false;
    }
    if (elementIds.size() == 0) {
        LogWarning("No element IDs to group!");
        status = "WARNING";
        message = "No element IDs to group!";
        m_infoObject.import("status", status);
        m_infoObject.import("message", message);
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
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "Invalid groupType: " + groupType);
        return false;
    }

    // Determine what the parents are
    for (auto it = elementIds.begin(); it != elementIds.end(); ++it) {
        // Verify that the children are of the same type
        Object *el = m_doc->GetDrawingPage()->FindDescendantByUuid(*it);
        if (el == NULL) {
            LogError("Could not get element with ID %s", it->c_str());
            m_infoObject.import("status", "FAILURE");
            m_infoObject.import("message", "Could not get element with ID " + *it);
            return false;
        }
        if (el->GetClassId() != elementClass) {
            LogError("Element %s was of class %s. Expected class %s", el->GetUuid().c_str(), el->GetClassName().c_str(),
                groupType.c_str());
            m_infoObject.import("status", "FAILURE");
            m_infoObject.import("message",
                "Element " + el->GetUuid() + " was of class " + el->GetClassName() + " but expected class " + groupType
                    + ".");
            return false;
        }

        // Get a set of parents and the number of children they have
        Object *par = el->GetParent();
        if (par == NULL) {
            LogError("Parent of %s is null!", el->GetUuid().c_str());
            m_infoObject.import("status", "FAILURE");
            m_infoObject.import("message", "Parent of " + el->GetUuid() + " is null.");
            return false;
        }
        if (doubleParent == NULL) {
            doubleParent = par->GetParent();
            if (doubleParent == NULL) {
                LogError("No second level parent!");
                m_infoObject.import("status", "FAILURE");
                m_infoObject.import("message", "No second level parent.");
                return false;
            }
        }
        else {
            if (par->GetParent() != doubleParent) {
                LogError("No shared second level parent!");
                m_infoObject.import("status", "FAILURE");
                m_infoObject.import("message", "No shared second level parent.");
                return false;
            }
        }
        auto possibleEntry = parents.find(el->GetParent());
        if (possibleEntry == parents.end()) {
            parents.emplace(el->GetParent(), 1);
        }
        else {
            possibleEntry->second += 1;
        }
        elements.insert(el);
    }

    std::copy(elements.begin(), elements.end(), std::back_inserter(sortedElements));
    std::stable_sort(sortedElements.begin(), sortedElements.end(), Object::sortByUlx);

    ListOfObjects clefs;
    std::set<Object *> syllables;
    ListOfObjects sortedSyllables;
    ClassIdComparison clefComp(CLEF);
    InterfaceComparison pitchComp(INTERFACE_PITCH);
    Clef *newClef = NULL;

    m_doc->GetDrawingPage()->FindAllDescendantBetween(&clefs, &clefComp,
        sortedElements.front()->GetFirstAncestor(SYLLABLE), sortedElements.back()->GetFirstAncestor(SYLLABLE));

    // if there are clefs between the elements getting grouped
    // some elements will need their pitch adjusted for the new clef
    // clefsBefore maps the syllable parent to its clef before the group
    // so we can reassociate any pitched children from their old clef to the new one
    if (clefs.size() != 0) {
        for (auto it = sortedElements.begin(); it != sortedElements.end(); ++it) {
            if ((*it)->Is(SYLLABLE)) {
                syllables.insert(dynamic_cast<Object *>(*it));
            }
            else {
                syllables.insert((*it)->GetFirstAncestor(SYLLABLE));
            }
        }
        std::copy(syllables.begin(), syllables.end(), std::back_inserter(sortedSyllables));
        for (auto it = sortedSyllables.begin(); it != sortedSyllables.end(); ++it) {
            Clef *tempClef = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindPreviousChild(&clefComp, (*it)));
            if (tempClef == NULL) {
                Layer *layer = dynamic_cast<Layer *>((*it)->GetFirstAncestor(LAYER));
                tempClef = layer->GetCurrentClef();
            }
            clefsBefore.insert(std::pair<Syllable *, Clef *>(dynamic_cast<Syllable *>(*it), tempClef));
        }
        newClef = clefsBefore[dynamic_cast<Syllable *>(sortedSyllables.front())];
    }

    // find parents where all of their children are being grouped
    for (auto it = parents.begin(); it != parents.end(); ++it) {
        auto parentPair = *it;
        Object *par = parentPair.first;
        int expected;
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

            for (auto it = elements.begin(); it != elements.end(); ++it) {
                if ((*it)->GetParent() != parent && !(*it)->Is(SYL)) {
                    (*it)->MoveItselfTo(parent);
                }
            }

            // Ensure a syl is created so text can be added to syllable
            Syl *syl = new Syl();
            Text *text = new Text();
            syl->AddChild(text);
            parent->AddChild(syl);

            // add a default bounding box if you need to
            if (m_doc->GetType() == Facs) {
                Zone *zone = new Zone();

                // if it's syllable parent has position values just use those
                FacsimileInterface *syllableFi = NULL;
                if (syl->GetFirstAncestor(SYLLABLE)->GetFacsimileInterface()->HasFacs()) {
                    syllableFi = syl->GetFirstAncestor(SYLLABLE)->GetFacsimileInterface();
                    Zone *tempZone = dynamic_cast<Zone *>(syllableFi->GetZone());
                    zone->SetUlx(tempZone->GetUlx());
                    zone->SetUly(tempZone->GetUly());
                    zone->SetLrx(tempZone->GetLrx());
                    zone->SetLry(tempZone->GetLry());
                }
                // otherwise get a boundingbox that comprises all the neumes in the syllable
                else {
                    ListOfObjects children;
                    InterfaceComparison comp(INTERFACE_FACSIMILE);
                    syl->GetFirstAncestor(SYLLABLE)->FindAllDescendantByComparison(&children, &comp);
                    for (auto iter2 = children.begin(); iter2 != children.end(); ++iter2) {
                        FacsimileInterface *temp = dynamic_cast<FacsimileInterface *>(*iter2);
                        assert(temp);
                        Zone *tempZone = vrv_cast<Zone *>(temp->GetZone());
                        assert(tempZone);
                        if (temp->HasFacs()) {
                            if (syllableFi == NULL) {
                                zone->SetUlx(tempZone->GetUlx());
                                zone->SetUly(tempZone->GetUly());
                                zone->SetLrx(tempZone->GetLrx());
                                zone->SetLry(tempZone->GetLry());
                            }
                            else {
                                if (tempZone->GetUlx() < zone->GetUlx()) {
                                    zone->SetUlx(tempZone->GetUlx());
                                }
                                if (tempZone->GetUly() < zone->GetUly()) {
                                    zone->SetUly(tempZone->GetUly());
                                }
                                if (tempZone->GetLrx() > zone->GetLrx()) {
                                    zone->SetLrx(tempZone->GetLrx());
                                }
                                if (tempZone->GetLry() > zone->GetLry()) {
                                    zone->SetLry(tempZone->GetLry());
                                }
                            }
                        }
                    }
                }

                // make the bounding box a little bigger and lower so it's easier to edit
                int offSetUly = 100;
                int offSetLrx = 100;
                int offSetLry = 200;

                zone->SetUly(zone->GetUly() + offSetUly);
                zone->SetLrx(zone->GetLrx() + offSetLrx);
                zone->SetLry(zone->GetLry() + offSetLry);

                assert(m_doc->GetFacsimile());
                m_doc->GetFacsimile()->FindDescendantByType(SURFACE)->AddChild(zone);
                FacsimileInterface *fi = vrv_cast<FacsimileInterface *>((*syl).GetFacsimileInterface());
                assert(fi);
                fi->SetZone(zone);

                // syl->ResetFacsimile();
                // syl->SetFacs(zone->GetUuid());
            }
        }

        parent->ReorderByXPos();
        if (doubleParent == NULL) {
            return false;
        }
        doubleParent->AddChild(parent);

        Layer *layer = vrv_cast<Layer *>(parent->GetFirstAncestor(LAYER));
        assert(layer);
        layer->ReorderByXPos();
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
        parent->ReorderByXPos();
    }

    // if there is more than 1 full parent we need to concat syl's
    // unless we're just grouping NC's in which case no need to worry about syl's of course
    // also in this case we need to make sure that the facsimile of the resulting syl is correct
    else {
        if (elementClass == NC) {
            if (doubleParent) {
                parent = new Neume();
                for (auto it = elements.begin(); it != elements.end(); ++it) {
                    if ((*it)->GetParent() != parent && !(*it)->Is(SYL)) {
                        (*it)->MoveItselfTo(parent);
                    }
                }
                doubleParent->AddChild(parent);
            }
        }
        else {
            std::sort(fullParents.begin(), fullParents.end(), Object::sortByUlx);
            Syllable *fullSyllable = new Syllable();
            Syl *fullSyl = NULL;

            // construct concatenated string of all the syls
            std::wstring fullString = L"";
            for (auto it = fullParents.begin(); it != fullParents.end(); ++it) {
                Syl *syl = dynamic_cast<Syl *>((*it)->FindDescendantByType(SYL));
                if (syl == NULL) continue;
                if (fullSyl == NULL) {
                    fullSyl = syl;
                }
                Text *text = dynamic_cast<Text *>(syl->FindDescendantByType(TEXT));
                if (text != NULL) {
                    std::wstring currentString = text->GetText();
                    fullString = fullString + currentString;
                }
            }
            // find the new boundingbox comprising all of the text
            int ulx = -1, uly = -1, lrx = -1, lry = -1;
            for (auto it = fullParents.begin(); it != fullParents.end(); ++it) {
                Object *par = vrv_cast<Object *>(*it);
                assert(par);
                Syl *descSyl = vrv_cast<Syl *>(par->FindDescendantByType(SYL));
                assert(descSyl);
                // FacsimileInterface *facsInter = dynamic_cast<FacsimileInterface *>
                // ((*it)->FindDescendantByType(SYL)->GetFacsimileInterface());
                FacsimileInterface *facsInter = dynamic_cast<FacsimileInterface *>(descSyl->GetFacsimileInterface());
                if (facsInter != NULL) {
                    if (ulx == -1) {
                        ulx = facsInter->GetDrawingX();
                        uly = facsInter->GetDrawingY();
                        lrx = facsInter->GetWidth() + ulx;
                        lry = facsInter->GetHeight() + uly;
                    }
                    else {
                        lrx = facsInter->GetWidth() + facsInter->GetDrawingX();
                        lry = facsInter->GetHeight() + facsInter->GetDrawingY();
                    }
                }
            }
            assert(fullSyl);
            Text *text = vrv_cast<Text *>(fullSyl->FindDescendantByType(TEXT));
            assert(text);
            text->SetText(fullString);
            assert(fullSyllable);
            fullSyllable->AddChild(fullSyl);
            for (auto it = elements.begin(); it != elements.end(); ++it) {
                if ((*it)->GetParent() != fullSyllable && !(*it)->Is(SYL)) {
                    (*it)->MoveItselfTo(fullSyllable);
                }
            }
            if (doubleParent == NULL) {
                LogError("No second level parent!");
                return false;
            }
            doubleParent->AddChild(fullSyllable);
            Layer *layer = vrv_cast<Layer *>(fullSyllable->GetFirstAncestor(LAYER));
            assert(layer);
            if (ulx >= 0 && uly >= 0 && lrx >= 0 && lry >= 0) {
                FacsimileInterface *facsInter = vrv_cast<FacsimileInterface *>(fullSyl->GetFacsimileInterface());
                assert(facsInter);
                Zone *zone = vrv_cast<Zone *>(facsInter->GetZone());
                assert(zone);
                assert(ulx >= 0);
                zone->SetUlx(ulx);
                assert(uly >= 0);
                zone->SetUly(uly);
                assert(lrx >= 0);
                zone->SetLrx(lrx);
                assert(lry >= 0);
                zone->SetLry(lry);
            }
            layer->ReorderByXPos();
            parent = fullSyllable;
        }
    }

    // change the pitch of any pitched elements whose clef may have changed
    assert(newClef);
    ListOfObjects pitchedChildren;
    if (sortedSyllables.size()) {
        for (auto it = sortedSyllables.begin(); it != sortedSyllables.end(); ++it) {
            Syllable *syllable = dynamic_cast<Syllable *>(*it);
            if (clefsBefore[syllable] != newClef) {
                syllable->FindAllDescendantByComparison(&pitchedChildren, &pitchComp);
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
            if (doubleParent == NULL) {
                LogError("No second level parent!");
                return false;
            }
            doubleParent->DeleteChild(obj);
        }
        else if (obj->GetChildCount() == obj->GetChildCount(SYL)) {
            Object *syl;
            while ((syl = obj->FindDescendantByType(SYL)) != NULL) {
                obj->DeleteChild(syl);
            }
            if (doubleParent == NULL) {
                LogError("No second level parent!");
                return false;
            }
            doubleParent->DeleteChild(obj);
        }
    }

    m_infoObject.import("uuid", parent->GetUuid());
    m_infoObject.import("status", status);
    m_infoObject.import("message", message);
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
    int ligCount = 0; // track ligature components. groupType=nc only.
    Clef *oldClef = NULL; // clef element originally applied to neumes. groupType=neume only.
    ClassIdComparison ac(CLEF);
    ListOfObjects syllables; // List of syllables used. groupType=neume only.

    jsonxx::Array uuidArray;

    // Check if you can get drawing page
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page.");
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "Could not get the drawing page.");
        return false;
    }
    for (auto it = elementIds.begin(); it != elementIds.end(); ++it) {
        Object *el = m_doc->GetDrawingPage()->FindDescendantByUuid(*it);

        if (groupType == "neume") {
            // Keep the syl attached to the original syllable.
            if (el->Is(SYL)) {
                continue;
            }
            if (currentParent == NULL) {
                fparent = el->GetFirstAncestor(SYLLABLE);
                assert(fparent);
                uuidArray << fparent->GetUuid();
                sparent = fparent->GetFirstAncestor(LAYER);
                assert(sparent);
                currentParent = vrv_cast<Syllable *>(fparent);
                assert(currentParent);

                // Get clef for clef changes
                oldClef = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindPreviousChild(&ac, currentParent));
                if (oldClef == NULL) {
                    oldClef = dynamic_cast<Layer *>(sparent)->GetCurrentClef();
                }
            }

            else {
                // Create a new parent
                Object *newParent = currentParent->Clone();
                newParent->CloneReset();
                assert(newParent);
                newParent->ClearChildren();

                // Reassign neume
                el->MoveItselfTo(newParent);
                fparent->ClearRelinquishedChildren();

                syllables.push_back(newParent);
                Syl *syl = new Syl();
                Text *text = new Text();
                syl->AddChild(text);
                newParent->AddChild(syl);

                // Create default bounding box if facs
                if (m_doc->GetType() == Facs) {
                    Zone *zone = new Zone();

                    // Use syllable parent positions if possible
                    FacsimileInterface *syllableFi = NULL;
                    if (syl->GetFirstAncestor(SYLLABLE)->GetFacsimileInterface()->HasFacs()) {
                        syllableFi = syl->GetFirstAncestor(SYLLABLE)->GetFacsimileInterface();
                        Zone *tempZone = dynamic_cast<Zone *>(syllableFi->GetZone());
                        zone->SetUlx(tempZone->GetUlx());
                        zone->SetUly(tempZone->GetUly());
                        zone->SetLrx(tempZone->GetLrx());
                        zone->SetLry(tempZone->GetLry());
                    }
                    // otherwise get a boundingbox that comprises all the neumes in the syllable
                    else {
                        ListOfObjects children;
                        InterfaceComparison comp(INTERFACE_FACSIMILE);
                        syl->GetFirstAncestor(SYLLABLE)->FindAllDescendantByComparison(&children, &comp);
                        for (auto iter2 = children.begin(); iter2 != children.end(); ++iter2) {
                            FacsimileInterface *temp = dynamic_cast<FacsimileInterface *>(*iter2);
                            assert(temp);
                            Zone *tempZone = vrv_cast<Zone *>(temp->GetZone());
                            assert(tempZone);
                            if (temp->HasFacs()) {
                                if (syllableFi == NULL) {
                                    zone->SetUlx(tempZone->GetUlx());
                                    zone->SetUly(tempZone->GetUly());
                                    zone->SetLrx(tempZone->GetLrx());
                                    zone->SetLry(tempZone->GetLry());
                                }
                                else {
                                    if (tempZone->GetUlx() < zone->GetUlx()) {
                                        zone->SetUlx(tempZone->GetUlx());
                                    }
                                    if (tempZone->GetUly() < zone->GetUly()) {
                                        zone->SetUly(tempZone->GetUly());
                                    }
                                    if (tempZone->GetLrx() > zone->GetLrx()) {
                                        zone->SetLrx(tempZone->GetLrx());
                                    }
                                    if (tempZone->GetLry() > zone->GetLry()) {
                                        zone->SetLry(tempZone->GetLry());
                                    }
                                }
                            }
                        }
                    }

                    // make the bounding box a little bigger and lower so it's easier to edit
                    zone->SetUly(zone->GetUly() + 100);
                    zone->SetLrx(zone->GetLrx() + 100);
                    zone->SetLry(zone->GetLry() + 200);

                    assert(m_doc->GetFacsimile());
                    m_doc->GetFacsimile()->FindDescendantByType(SURFACE)->AddChild(zone);
                    FacsimileInterface *fi = dynamic_cast<FacsimileInterface *>((*syl).GetFacsimileInterface());
                    assert(fi);
                    Object *surface = m_doc->GetFacsimile()->FindDescendantByType(SURFACE);
                    if (surface != NULL) {
                        surface->AddChild(zone);
                        fi->SetZone(zone);
                    }
                    else {
                        delete zone;
                    }
                }
                uuidArray << newParent->GetUuid();
                sparent->AddChild(newParent);
                sparent->ReorderByXPos();
            }
        }
        else if (groupType == "nc") {
            // Only ungroup ligatures if the entire neume is a ligature!
            Nc *nc = vrv_cast<Nc *>(el);
            assert(nc);
            if (elementIds.size() == 2 && nc->GetLigated() == BOOLEAN_true) {
                this->ToggleLigature(elementIds, "true");
            }
            // Keep track of existing ligatures
            if (nc->GetLigated() == BOOLEAN_true) {
                ligCount += 1;
            }

            if (elementIds.begin() == it) {
                fparent = el->GetFirstAncestor(NEUME);
                assert(fparent);
                uuidArray << fparent->GetUuid();
                sparent = fparent->GetFirstAncestor(SYLLABLE);
                assert(sparent);
                currentParent = vrv_cast<Neume *>(fparent);
                assert(currentParent);
            }
            else {
                // Maintain ligatures
                if (ligCount == 2) {
                    if (currentParent != fparent) {
                        el->MoveItselfTo(currentParent);
                        fparent->ClearRelinquishedChildren();
                    }
                    ligCount = 0;
                }
                else {
                    Object *newParent = currentParent->Clone();
                    newParent->CloneReset();
                    assert(newParent);
                    newParent->ClearChildren();

                    el->MoveItselfTo(newParent);
                    fparent->ClearRelinquishedChildren();
                    uuidArray << newParent->GetUuid();
                    sparent->AddChild(newParent);
                    sparent->ReorderByXPos();

                    currentParent = newParent;
                }
            }
        }
        else {
            LogError("Invalid groupType: %s", groupType.c_str());
            m_infoObject.import("status", "FAILURE");
            m_infoObject.import("message", "Invalid groupType for ungrouping.");
            return false;
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
                (*it)->FindAllDescendantByComparison(&pitchedChildren, &ic);
                for (auto pChild = pitchedChildren.begin(); pChild != pitchedChildren.end(); ++pChild) {
                    (*pChild)->GetPitchInterface()->AdjustPitchForNewClef(oldClef, currentClef);
                }
            }
        }
    }

    m_infoObject.import("status", "OK");
    m_infoObject.import("message", "");
    m_infoObject.import("uuid", uuidArray);
    return true;
}

bool EditorToolkitNeume::ChangeGroup(std::string elementId, std::string contour)
{
    // Check if you can get drawing page
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page.");
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "Could not get the drawing page.");
        return false;
    }
    Neume *el = dynamic_cast<Neume *>(m_doc->GetDrawingPage()->FindDescendantByUuid(elementId));
    if (el == NULL) {
        LogError("Unable to find neume with id %s", elementId.c_str());
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "Unable to find neume with id " + elementId + ".");
        return false;
    }
    Nc *firstChild = NULL;
    Nc *prevNc = NULL;

    // Get children of neume. Keep the first child and delete the others.
    ClassIdComparison ac(NC);
    ListOfObjects children;
    el->FindAllDescendantByComparison(&children, &ac);
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

    Staff *staff = vrv_cast<Staff *>(el->GetFirstAncestor(STAFF));
    assert(staff);
    Facsimile *facsimile = m_doc->GetFacsimile();

    const int noteHeight = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 2);
    const int noteWidth = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 1.4);
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
            m_infoObject.import("status", "FAILURE");
            m_infoObject.import("message", "Unsupported character in contour.");
            return false;
        }
        zone->SetUlx(newUlx);
        zone->SetUly(newUly);
        ;
        zone->SetLrx(newLrx);
        zone->SetLry(newLry);

        newNc->SetZone(zone);

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
    m_infoObject.import("uuid", el->GetUuid());
    m_infoObject.import("status", "OK");
    m_infoObject.import("message", "");
    return true;
}

bool EditorToolkitNeume::ToggleLigature(std::vector<std::string> elementIds, std::string isLigature)
{
    assert(elementIds.size() == 2);
    bool success1 = false;
    bool success2 = false;
    Facsimile *facsimile = m_doc->GetFacsimile();
    assert(facsimile);
    Surface *surface = vrv_cast<Surface *>(facsimile->FindDescendantByType(SURFACE));
    assert(surface);
    std::string firstNcId = elementIds[0];
    std::string secondNcId = elementIds[1];
    // Check if you can get drawing page
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page.");
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "Could not get the drawing page.");
        return false;
    }

    Nc *firstNc = vrv_cast<Nc *>(m_doc->GetDrawingPage()->FindDescendantByUuid(firstNcId));
    assert(firstNc);
    Nc *secondNc = vrv_cast<Nc *>(m_doc->GetDrawingPage()->FindDescendantByUuid(secondNcId));
    assert(secondNc);
    Zone *zone = new Zone();
    // set ligature to false and update zone of second Nc
    if (isLigature == "true") {
        if (Att::SetNeumes(firstNc, "ligated", "false")) success1 = true;

        int ligUlx = firstNc->GetZone()->GetUlx();
        int ligUly = firstNc->GetZone()->GetUly();
        int ligLrx = firstNc->GetZone()->GetLrx();
        int ligLry = firstNc->GetZone()->GetLry();

        Staff *staff = vrv_cast<Staff *>(firstNc->GetFirstAncestor(STAFF));
        assert(staff);

        const int noteHeight = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 2);
        const int noteWidth = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 1.4);

        zone->SetUlx(ligUlx + noteWidth);
        zone->SetUly(ligUly + noteHeight);
        zone->SetLrx(ligLrx + noteWidth);
        zone->SetLry(ligLry + noteHeight);

        secondNc->SetZone(zone);

        if (Att::SetNeumes(secondNc, "ligated", "false")) success2 = true;
    }
    // set ligature to true and update zones to be the same
    else if (isLigature == "false") {
        if (Att::SetNeumes(firstNc, "ligated", "true")) success1 = true;

        zone->SetUlx(firstNc->GetZone()->GetUlx());
        zone->SetUly(firstNc->GetZone()->GetUly());
        zone->SetLrx(firstNc->GetZone()->GetLrx());
        zone->SetLry(firstNc->GetZone()->GetLry());

        secondNc->SetZone(zone);

        if (Att::SetNeumes(secondNc, "ligated", "true")) success2 = true;
    }
    else {
        LogError("isLigature is invalid!");
        delete zone;
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "isLigature value '" + isLigature + "' is invalid.");
        return false;
    }
    if (success1 && success2 && m_doc->GetType() != Facs) {
        m_doc->PrepareDrawing();
        m_doc->GetDrawingPage()->LayOut(true);
    }
    m_infoObject.import("status", "OK");
    m_infoObject.import("message", "");
    if (!(success1 && success2)) {
        LogWarning("Unable to update ligature attribute");
        m_infoObject.import("message", "Unable to update ligature attribute.");
        m_infoObject.import("status", "WARNING");
    }

    surface->AddChild(zone);
    return success1 && success2;
}

bool EditorToolkitNeume::ChangeStaff(std::string elementId)
{
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page");
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "Could not get the drawing page.");
        return false;
    }

    if (m_doc->GetType() != Facs) {
        LogWarning("Staff re-association is only available in facsimile mode.");
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "Staff re-association is only available in facsimile mode.");
        return false;
    }

    Object *element = m_doc->GetDrawingPage()->FindDescendantByUuid(elementId);
    assert(element);
    if (element == NULL) {
        LogError("No element exists with ID '%s'.", elementId.c_str());
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "No element exists with ID" + elementId + ".");
        return false;
    }

    if (!(element->Is(SYLLABLE) || element->Is(CUSTOS) || element->Is(CLEF))) {
        LogError("Element is of type %s, but only Syllables, Custos, and Clefs can change staves.",
            element->GetClassName().c_str());
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message",
            "Element is of type " + element->GetClassName()
                + ", but only Syllables, Custos, and Clefs can change staves.");
        return false;
    }

    ListOfObjects stavesList;
    ClassIdComparison ac(STAFF);
    m_doc->FindAllDescendantByComparison(&stavesList, &ac);

    std::vector<Object *> staves(stavesList.begin(), stavesList.end());

    ClosestBB comp;

    if (dynamic_cast<FacsimileInterface *>(element)->HasFacs()) {
        comp.x = element->GetFacsimileInterface()->GetZone()->GetUlx();
        comp.y = element->GetFacsimileInterface()->GetZone()->GetUly();
    }

    else if (element->Is(SYLLABLE)) {
        int ulx, uly, lrx, lry;
        LayerElement *layerElement = dynamic_cast<LayerElement *>(element);
        if (!layerElement->GenerateZoneBounds(&ulx, &uly, &lrx, &lry)) {
            LogError("Couldn't generate bounding box for syllable.");
            m_infoObject.import("status", "FAILURE");
            m_infoObject.import("message", "Couldn't generate bounding box for syllable.");
            return false;
        }
        comp.x = (lrx + ulx) / 2;
        comp.y = (uly + lry) / 2;
    }

    else {
        LogError("This element does not have a facsimile.");
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "This element does not have a facsimile.");
        return false;
    }

    Staff *staff = NULL;

    if (staves.size() > 0) {
        std::sort(staves.begin(), staves.end(), comp);
        staff = dynamic_cast<Staff *>(staves.front());
    }
    else {
        LogError("Could not find any staves. This should not happen");
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "Could not find any staves. This should not happen");
        return false;
    }

    Layer *parent = dynamic_cast<Layer *>(element->GetFirstAncestor(LAYER));
    Staff *sParent = vrv_cast<Staff *>(parent->GetFirstAncestor(STAFF));
    assert(parent);
    if (parent == NULL || sParent == NULL) {
        LogError("Couldn't find staff parent of element with id '%s'", elementId.c_str());
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "Couldn't find staff parent of element with id " + elementId);
        return false;
    }

    Layer *layer = vrv_cast<Layer *>(staff->FindDescendantByType(LAYER));
    assert(LAYER);
    if (layer == NULL) {
        LogError("Couldn't find layer child of staff. This should not happen");
        m_infoObject.import("status", "FAILURE");
        m_infoObject.import("message", "Couldn't find layer child of staff. This should not happen");
        return false;
    }

    if (layer == parent) {
        m_infoObject.import("status", "WARNING");
        m_infoObject.import("message", "Moving to the same staff as before.");
        m_infoObject.import("elementId", elementId);
        m_infoObject.import("newStaffId", staff->GetUuid());
        return true;
    }

    // Adjust pitch/staff line.
    if (element->Is(CLEF)) {
        Clef *clef = vrv_cast<Clef *>(element);
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

        m_doc->GetDrawingPage()->FindAllDescendantBetween(&oldPitchChildren, &ic, clef,
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
            LogError("Could not adjust clef line of %s", element->GetUuid().c_str());
            m_infoObject.import("status", "FAILURE");
            m_infoObject.import("message", "Failed to set clef line from facsimile.");
            return false;
        }

        // apply pitch interface changes to any elements whose clef may have changed
        ListOfObjects newPitchChildren;
        Clef *previousClefAfter = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindPreviousChild(&cic, element));
        if (previousClefAfter == NULL) {
            previousClefAfter = layer->GetCurrentClef();
        }
        Clef *nextClefAfter = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindNextChild(&cic, element));
        m_doc->GetDrawingPage()->FindAllDescendantBetween(
            &newPitchChildren, &ic, clef, (nextClefAfter != NULL) ? nextClefAfter : m_doc->GetDrawingPage()->GetLast());

        for (auto it = newPitchChildren.begin(); it != newPitchChildren.end(); ++it) {
            pi = (*it)->GetPitchInterface();
            assert(pi);
            pi->AdjustPitchForNewClef(previousClefAfter, clef);
        }
    }
    // custos or syllable
    else {
        element->MoveItselfTo(layer);
        layer->ReorderByXPos();
        parent->ClearRelinquishedChildren();
        parent->ReorderByXPos();
        if (!AdjustPitchFromPosition(element)) {
            LogError("Could not adjust pitch of %s", element->GetUuid().c_str());
            m_infoObject.import("status", "FAILURE");
            m_infoObject.import("message", "Failed to properly set pitch.");
            m_infoObject.import("elementId", element->GetUuid());
            m_infoObject.import("newStaffId", staff->GetUuid());
            return false;
        }
    }

    m_infoObject.import("status", "OK");
    m_infoObject.import("message", "");
    m_infoObject.import("elementId", elementId);
    m_infoObject.import("newStaffId", staff->GetUuid());
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
    int *ulx, int *uly, int *lrx, int *lry, std::vector<std::pair<std::string, std::string> > *attributes)
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
        for (auto it = m.begin(); it != m.end(); it++) {
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

bool EditorToolkitNeume::ParseMergeAction(jsonxx::Object param, std::vector<std::string> *elementIds)
{
    if (!param.has<jsonxx::Array>("elementIds")) return false;
    jsonxx::Array array = param.get<jsonxx::Array>("elementIds");
    for (int i = 0; i < (int)array.size(); i++) {
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
    for (int i = 0; i < (int)array.size(); i++) {
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
    for (int i = 0; i < (int)array.size(); i++) {
        elementIds->push_back(array.get<jsonxx::String>(i));
    }

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

bool EditorToolkitNeume::ParseToggleLigatureAction(
    jsonxx::Object param, std::vector<std::string> *elementIds, std::string *isLigature)
{
    if (!param.has<jsonxx::Array>("elementIds")) return false;
    jsonxx::Array array = param.get<jsonxx::Array>("elementIds");
    for (int i = 0; i < (int)array.size(); i++) {
        elementIds->push_back(array.get<jsonxx::String>(i));
    }
    if (!param.has<jsonxx::String>("isLigature")) return false;
    (*isLigature) = param.get<jsonxx::String>("isLigature");

    return true;
}

bool EditorToolkitNeume::ParseChangeStaffAction(jsonxx::Object param, std::string *elementId)
{
    if (!param.has<jsonxx::String>("elementId")) return false;
    (*elementId) = param.get<jsonxx::String>("elementId");

    return true;
}

bool EditorToolkitNeume::AdjustPitchFromPosition(Object *obj, Clef *clef)
{
    // remember to reorderbyxpos! (not called in function so that it can be used in loops)
    // clef re-association is done at the syllable level (not neume or nc)
    // so this function should only be called on custos or syllables
    // it should also only be called in cases where finding the old clef is not required
    // since doing it based only on clefs is much more efficient than based on position
    // also if you are calling this function in a loop you should always be passing a clef argument
    // since repeatedly finding the previous clef is very inneficient

    if (obj->Is(CUSTOS)) {
        Custos *custos = dynamic_cast<Custos *>(obj);
        Staff *staff = vrv_cast<Staff *>(custos->GetFirstAncestor(STAFF));
        assert(staff);

        // Check interfaces
        if ((custos->GetPitchInterface() == NULL) || (custos->GetFacsimileInterface() == NULL)) {
            LogError("Element is lacking an interface which is required for pitch adjusting");
            return false;
        }
        PitchInterface *pi = custos->GetPitchInterface();
        FacsimileInterface *fi = custos->GetFacsimileInterface();

        // Check for facsimile
        if (!fi->HasFacs() || !staff->HasFacs()) {
            LogError("Could not adjust pitch: the element or staff lacks facsimile data");
            return false;
        }

        if (clef == NULL) {
            ClassIdComparison ac(CLEF);
            clef = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindPreviousChild(&ac, obj));
            if (clef == NULL) {
                Layer *layer = vrv_cast<Layer *>(staff->FindDescendantByType(LAYER));
                assert(layer);
                clef = layer->GetCurrentClef();
            }
        }

        assert(clef);

        // Reset pitch to be "on clef"
        if (clef->GetShape() == CLEFSHAPE_C) {
            pi->SetPname(PITCHNAME_c);
        }
        else if (clef->GetShape() == CLEFSHAPE_F) {
            pi->SetPname(PITCHNAME_f);
        }
        else if (clef->GetShape() == CLEFSHAPE_G) {
            pi->SetPname(PITCHNAME_g);
        }
        else {
            LogError("Clef %s does not have valid shape. Shape is %s", clef->GetUuid().c_str(), clef->GetShape());
            return false;
        }
        pi->SetOct(3);

        // glyphs in verovio are actually not centered, but are in the top left corner of a giant box
        int centerY = fi->GetZone()->GetUly();
        int centerX = fi->GetZone()->GetUlx();

        const int staffSize = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        const int pitchDifference = round(
            (double)(staff->GetZone()->GetUly() + (2 * staffSize * (staff->m_drawingLines - clef->GetLine())) - centerY
                - ((centerX - staff->GetZone()->GetUlx()) * tan(-staff->GetDrawingRotate() * M_PI / 180.0)))
            / (double)(staffSize));

        pi->AdjustPitchByOffset(pitchDifference);
        return true;
    }

    else if (obj->Is(SYLLABLE)) {
        Syllable *syl = dynamic_cast<Syllable *>(obj);
        Staff *staff = vrv_cast<Staff *>(syl->GetFirstAncestor(STAFF));
        assert(staff);

        ListOfObjects pitchedChildren;
        InterfaceComparison ic(INTERFACE_PITCH);
        syl->FindAllDescendantByComparison(&pitchedChildren, &ic);

        if (pitchedChildren.empty()) {
            LogWarning("Syllable had no pitched children to reorder for syllable %s", obj->GetUuid().c_str());
            return true;
        }

        if (clef == NULL) {
            ClassIdComparison ac(CLEF);
            clef = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindPreviousChild(&ac, obj));
            if (clef == NULL) {
                Layer *layer = vrv_cast<Layer *>(staff->FindDescendantByType(LAYER));
                assert(layer);
                clef = layer->GetCurrentClef();
            }
        }

        assert(clef);

        data_PITCHNAME pname;
        switch (clef->GetShape()) {
            case CLEFSHAPE_C: pname = PITCHNAME_c; break;
            case CLEFSHAPE_F: pname = PITCHNAME_f; break;
            case CLEFSHAPE_G: pname = PITCHNAME_g; break;
            default:
                LogError("Clef %s does not have valid shape. Shape is %s", clef->GetUuid().c_str(), clef->GetShape());
                return false;
        }

        const int staffSize = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);

        for (auto it = pitchedChildren.begin(); it != pitchedChildren.end(); ++it) {
            FacsimileInterface *fi = (*it)->GetFacsimileInterface();
            if (fi == NULL || !fi->HasFacs()) {
                LogError("Could not adjust pitch: child %s does not have facsimile data", (*it)->GetUuid().c_str());
                return false;
            }

            PitchInterface *pi = (*it)->GetPitchInterface();
            assert(pi);
            pi->SetPname(pname);
            pi->SetOct(3);

            /*int pitchDifference = round((double) (staff->GetZone()->GetUly() +
                (2 * staffSize * (staff->m_drawingLines - clef->GetLine())) -
                fi->GetZone()->GetUly() - ((fi->GetZone()->GetUlx() - staff->GetZone()->GetUlx()) *
                tan(-staff->GetDrawingRotate() * M_PI / 180.0))) / (double) (staffSize));*/
            int pitchDifference
                = round((double)(staff->GetDrawingY() + (2 * staffSize * (staff->m_drawingLines - clef->GetLine()))
                            - fi->GetZone()->GetUly()
                            - ((fi->GetZone()->GetUlx() - staff->GetZone()->GetUlx())
                                * tan(-staff->GetDrawingRotate() * M_PI / 180.0)))
                    / (double)(staffSize));

            pi->AdjustPitchByOffset(pitchDifference);
        }

        return true;
    }

    else {
        LogError("AdjustPitchFromPosition should only be called on custos or syllables."
                 "It has been called on %s, whose id is %s",
            obj->GetClassName().c_str(), obj->GetUuid().c_str());
        return false;
    }
}

bool EditorToolkitNeume::AdjustClefLineFromPosition(Clef *clef, Staff *staff)
{
    assert(clef);

    if (staff == NULL) {
        staff = dynamic_cast<Staff *>(clef->GetFirstAncestor(STAFF));
    }
    assert(staff);

    if (!clef->HasFacs() || !staff->HasFacs()) {
        return false;
    }

    const int staffSize = m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
    int yDiff = clef->GetZone()->GetUly() - staff->GetZone()->GetUly()
        + (clef->GetZone()->GetUlx() - staff->GetZone()->GetUlx()) * tan(staff->GetDrawingRotate() * M_PI / 180.0);
    int clefLine = staff->m_drawingLines - round((double)yDiff / (double)staffSize);
    clef->SetLine(clefLine);
    return true;
}

} // namespace vrv
