/////////////////////////////////////////////////////////////////////////////
// Name:        editortoolkit.cpp
// Author:      Laurent Pugin, Juliette Regimbal, Zoe McLennan
// Created:     16/05/2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "editortoolkit.h"

//--------------------------------------------------------------------------------

#include <locale>
#include <codecvt>
#include <set>

//--------------------------------------------------------------------------------

#include "clef.h"
#include "comparison.h"
#include "custos.h"
#include "layer.h"
#include "measure.h"
#include "nc.h"
#include "neume.h"
#include "page.h"
#include "rend.h"
#include "slur.h"
#include "staff.h"
#include "staffdef.h"
#include "syl.h"
#include "syllable.h"
#include "text.h"
#include "vrv.h"

//--------------------------------------------------------------------------------

#include "jsonxx.h"

namespace vrv {

#ifdef USE_EMSCRIPTEN
bool EditorToolkit::ParseEditorAction(const std::string &json_editorAction, bool isChain)
{
    jsonxx::Object json;

    // Read JSON actions
    if (!json.parse(json_editorAction)) {
        LogError("Can not parse JSON std::string.");
        return false;
    }

    if (!json.has<jsonxx::String>("action") ||
            (!json.has<jsonxx::Object>("param") && !json.has<jsonxx::Array>("param"))) {
        LogWarning("Incorrectly formatted JSON action");
        return false;
    }

    std::string action = json.get<jsonxx::String>("action");

    if (action != "chain" && json.has<jsonxx::Array>("param")) {
        LogWarning("Only 'chain' uses 'param' as an array.");
        return false;
    }

    if (action == "drag") {
        std::string elementId;
        int x,y;
        if (this->ParseDragAction(json.get<jsonxx::Object>("param"), &elementId, &x, &y)) {
            return this->Drag(elementId, x, y, isChain);
        }
        LogWarning("Could not parse the drag action");
    }
    else if (action == "insert") {
        std::string elementType, startId, endId, staffId;
        int ulx, uly, lrx, lry;
        std::vector<std::pair<std::string, std::string>> attributes;
        if (this->ParseInsertAction(json.get<jsonxx::Object>("param"), &elementType, &startId, &endId)) {
            return this->Insert(elementType, startId, endId);
        }
        else if (this->ParseInsertAction(json.get<jsonxx::Object>("param"), &elementType, &staffId, &ulx, &uly,
                    &lrx, &lry, &attributes)) {
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
        if(this->ParseSetClefAction(json.get<jsonxx::Object>("param"), &elementId, &shape)) {
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
        if (this->ParseGroupAction(json.get<jsonxx::Object>("param"), &groupType, &elementIds)){
            return this->Group(groupType, elementIds);
        }
    }
    else if (action == "ungroup") {
        std::string groupType;
        std::vector<std::string> elementIds;
        if(this->ParseUngroupAction(json.get<jsonxx::Object>("param"), &groupType, &elementIds)){
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
    else if (action == "changeGroup"){
        std::string elementId;
        std::string contour;
        if(this->ParseChangeGroupAction(json.get<jsonxx::Object>("param"), &elementId, &contour)) {
            return this->ChangeGroup(elementId, contour);
        }
        LogWarning("Could not parse change group action");
    }
    else if (action == "toggleLigature"){
        std::vector<std::string> elementIds;
        std::string isLigature;
        if(this->ParseToggleLigatureAction(json.get<jsonxx::Object>("param"), &elementIds, &isLigature)){
            return this->ToggleLigature(elementIds, isLigature);
        }
        LogWarning("Could not parse toggle ligature action");
    }
    else {
        LogWarning("Unknown action type '%s'.", action.c_str());
    }
    return false;
}

bool EditorToolkit::Chain(jsonxx::Array actions)
{
    bool status = true;
    std::string info = "[";
    bool runReorder = false;
    std::string id = "";
    for (int i = 0; i < actions.size(); i++) {
        if (!actions.has<jsonxx::Object>(i)) {
            LogError("Action %d was not an object", i);
            return false;
        }
        if (actions.get<jsonxx::Object>(i).get<jsonxx::String>("action") == "drag") {
            runReorder = true;
            id = actions.get<jsonxx::Object>(i).get<jsonxx::Object>("param").get<jsonxx::String>("elementId");
        }
        status |= this->ParseEditorAction(actions.get<jsonxx::Object>(i).json(), true);
        if (i != 0)
            info += ", ";
        info += "\"" + m_editInfo + "\"";
    }
    info += "]";
    m_editInfo = info;
    if (status && runReorder) {
        Object *obj = m_doc->GetDrawingPage()->FindChildByUuid(id);
        Layer *layer = nullptr;
        assert(obj);
        if (obj->Is(STAFF)) {
            layer = dynamic_cast<Layer *>(obj->GetFirst(LAYER));
        } else {
            layer = dynamic_cast<Layer *>(obj->GetFirstParent(LAYER));
        }
        assert(layer);
        layer->ReorderByXPos();
    }
    return status;
}

bool EditorToolkit::Drag(std::string elementId, int x, int y, bool isChain)
{
    m_editInfo = "";
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get drawing page.");
        return false;
    }

    // Try to get the element on the current drawing page
    Object *element = m_doc->GetDrawingPage()->FindChildByUuid(elementId);

    // If it wasn't there, go back up to the whole doc
    if (!element) {
        element = m_doc->FindChildByUuid(elementId);
    }
    // Use relative x and y for now on
    // For elements whose y-position corresponds to a certain pitch
    if (element->HasInterface(INTERFACE_PITCH)) {
        Layer *layer = dynamic_cast<Layer *>(element->GetFirstParent(LAYER));
        if(!layer) {
            LogError("Element does not have Layer parent. This should not happen.");
            return false;
        }
        Staff *staff = dynamic_cast<Staff *>(layer->GetFirstParent(STAFF));
        assert(staff);
        // Calculate pitch difference based on y difference
        int pitchDifference = round( (double) y / (double) m_doc->GetDrawingUnit(staff->m_drawingStaffSize));
        element->GetPitchInterface()->AdjustPitchByOffset(pitchDifference);

        if (element->HasInterface(INTERFACE_FACSIMILE)) {
            bool ignoreFacs = false;
            // Dont adjust the same facsimile twice. NCs in a ligature share a single zone.
            if (element->Is(NC)) {
                Nc *nc = dynamic_cast<Nc *>(element);
                if (nc->GetLigated() == BOOLEAN_true) {
                    Neume *neume = dynamic_cast<Neume *>(nc->GetFirstParent(NEUME));
                    Nc *nextNc = dynamic_cast<Nc *>(neume->GetChild(1 + neume->GetChildIndex(element)));
                    if (nextNc != nullptr && nextNc->GetLigated() == BOOLEAN_true && nextNc->GetZone() == nc->GetZone())
                        ignoreFacs = true;
                }
            }
            if (!ignoreFacs) {
                FacsimileInterface *fi = element->GetFacsimileInterface();
                assert(fi);
                Zone *zone = fi->GetZone();
                assert(zone);
                zone->ShiftByXY(x, pitchDifference * staff->m_drawingStaffSize);
            }
        }
    }
    // TODO Make more generic
    else if (element->Is(NEUME)) {
        Neume *neume = dynamic_cast<Neume *>(element);
        assert(neume);
        Layer *layer = dynamic_cast<Layer *>(neume->GetFirstParent(LAYER));
        if (!layer) {
            LogError("Element does not have Layer parent. This should not occur.");
            return false;
        }
        Staff *staff = dynamic_cast<Staff *>(layer->GetFirstParent(STAFF));
        assert(staff);
        // Calculate difference in pitch based on y difference
        int pitchDifference = round( (double)y / (double)m_doc->GetDrawingUnit(staff->m_drawingStaffSize));

        // Get components of neume
        ClassIdComparison ac(NC);
        ArrayOfObjects objects;
        neume->FindAllChildByComparison(&objects, &ac);
        for (auto it = objects.begin(); it != objects.end(); ++it) {
            Nc *nc = dynamic_cast<Nc *>(*it);
            // Update the neume component
            nc->AdjustPitchByOffset(pitchDifference);
        }

        if (neume->HasFacs()) {
            Zone *zone = neume->GetZone();
            assert(zone);
            zone->ShiftByXY(x, pitchDifference * staff->m_drawingStaffSize);
        }
        else if (dynamic_cast<Nc*>(neume->FindChildByType(NC))->HasFacs()) {
            std::set<Zone *> childZones;    // Sets do not contain duplicate entries
            for (Object *child = neume->GetFirst(); child != nullptr; child = neume->Object::GetNext()) {
                FacsimileInterface *fi = child->GetFacsimileInterface();
                if (fi != nullptr) {
                    childZones.insert(fi->GetZone());
                }
            }
            for (auto it = childZones.begin(); it != childZones.end(); it++) {
                (*it)->ShiftByXY(x, pitchDifference * staff->m_drawingStaffSize);
            }
        }
    }
    else if(element->Is(SYLLABLE)) {
        Syllable *syllable = dynamic_cast<Syllable *>(element);
        assert(syllable);
        Layer *layer = dynamic_cast<Layer *>(syllable->GetFirstParent(LAYER));
        if (!layer) return false;

        Staff *staff = dynamic_cast<Staff *>(layer->GetFirstParent(STAFF));
        assert(staff);

        int pitchDifference = round( (double)y / (double)m_doc->GetDrawingUnit(staff->m_drawingStaffSize));

        //Get components of syllable
        ClassIdComparison ac(NEUME);
        ArrayOfObjects neumes;
        syllable->FindAllChildByComparison(&neumes, &ac);
        for (auto it = neumes.begin(); it != neumes.end(); ++it) {
            Neume *neume = dynamic_cast<Neume *>(*it);
            assert(neume);
            ClassIdComparison ac(NC);
            ArrayOfObjects ncs;
            neume->FindAllChildByComparison(&ncs, &ac);
            for (auto it = ncs.begin(); it != ncs.end(); ++it) {
                Nc *nc = dynamic_cast<Nc *>(*it);
                // Update the neume component
                nc->AdjustPitchByOffset(pitchDifference);
            }
            if (neume->HasFacs()) {
            Zone *zone = neume->GetZone();
            assert(zone);
            zone->ShiftByXY(x, pitchDifference * staff->m_drawingStaffSize);
            }
            else if (dynamic_cast<Nc*>(neume->FindChildByType(NC))->HasFacs()) {
                std::set<Zone *> childZones;
                for (Object *child = neume->GetFirst(); child != nullptr; child = neume->Object::GetNext()) {
                    FacsimileInterface *fi = child->GetFacsimileInterface();
                    if (fi != nullptr) {
                        childZones.insert(fi->GetZone());
                    }
                }
                for (auto it = childZones.begin(); it != childZones.end(); it++) {
                    (*it)->ShiftByXY(x, pitchDifference * staff->m_drawingStaffSize);
                }
            }
        }
    }
    else if (element->Is(CLEF)) {
        Clef *clef = dynamic_cast<Clef *>(element);
        assert(clef);
        Layer *layer = dynamic_cast<Layer *>(clef->GetFirstParent(LAYER));
        if (!layer) return false;

        Staff *staff = dynamic_cast<Staff *>(layer->GetFirstParent(STAFF));
        assert(staff);
        // Note that y param is relative to initial position for clefs
        int initialClefLine = clef->GetLine();
        int clefLine = round((double) y / (double) m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) + initialClefLine);
        clef->SetLine(clefLine);

        if (initialClefLine != clefLine) {  // adjust notes so they stay in the same position
            int lineDiff = clefLine - initialClefLine;
            ArrayOfObjects objects;
            InterfaceComparison ic(INTERFACE_PITCH);

            Object *nextClef = m_doc->GetDrawingPage()->GetNext(clef, CLEF);
            m_doc->GetDrawingPage()->FindAllChildBetween(&objects, &ic, clef,
                    (nextClef != nullptr) ? nextClef : m_doc->GetDrawingPage()->GetLast());

            // Adjust all elements who are positioned relative to clef by pitch
            for (auto it = objects.begin(); it != objects.end(); ++it) {
                Object *child = dynamic_cast<Object *>(*it);
                if (child == nullptr || layer->GetClef(dynamic_cast<LayerElement *>(child)) != clef) continue;
                PitchInterface *pi = child->GetPitchInterface();
                assert(pi);
                pi->AdjustPitchByOffset(-2 * lineDiff); // One line -> 2 pitches
            }
        }

        if (clef->HasFacs()) { // adjust facsimile for clef (if it exists)
            Zone *zone = clef->GetZone();
            assert(zone);
            zone->ShiftByXY(x, (clefLine - initialClefLine) * 2 * staff->m_drawingStaffSize);
        }
    }
    else if (element->Is(STAFF)) {
        Staff *staff = dynamic_cast<Staff *>(element);
        if (!staff->HasFacs()) {
            LogError("Staff dragging is only supported for staves with facsimiles!");
            return false;
        }

        // Move staff and all staff children with facsimiles
        ArrayOfObjects children;
        InterfaceComparison ic(INTERFACE_FACSIMILE);
        staff->FindAllChildByComparison(&children, &ic);
        std::set<Zone *> zones;
        zones.insert(staff->GetZone());
        for (auto it = children.begin(); it != children.end(); ++it) {
            FacsimileInterface *fi = (*it)->GetFacsimileInterface();
            assert(fi);
            if (fi->GetZone() != nullptr)
                zones.insert(fi->GetZone());
        }
        for (auto it = zones.begin(); it != zones.end(); ++it) {
            // Transform y to device context
            (*it)->ShiftByXY(x, -y);
        }

        //TODO Reorder by left-to-right, top-to-bottom

        return true; // Can't reorder by layer since staves contain layers
    }
    else {
        LogWarning("Unsupported element for dragging.");
        return false;
    }
    if (!isChain) {
        Layer *layer = dynamic_cast<Layer *>(element->GetFirstParent(LAYER));
        layer->ReorderByXPos(); // Reflect position order of elements internally (and in the resulting output file)
    }
    return true;
}

bool EditorToolkit::Insert(std::string elementType, std::string startid, std::string endid)
{
    if (!m_doc->GetDrawingPage()) return false;
    Object *start = m_doc->GetDrawingPage()->FindChildByUuid(startid);
    Object *end = m_doc->GetDrawingPage()->FindChildByUuid(endid);
    // Check if both start and end elements exist
    if (!start || !end) {
        LogMessage("Elements start and end ids '%s' and '%s' could not be found", startid.c_str(), endid.c_str());
        return false;
    }
    // Check if it is a LayerElement
    if (!dynamic_cast<LayerElement *>(start)) {
        LogMessage("Element '%s' is not supported as start element", start->GetClassName().c_str());
        return false;
    }
    if (!dynamic_cast<LayerElement *>(end)) {
        LogMessage("Element '%s' is not supported as end element", start->GetClassName().c_str());
        return false;
    }

    Measure *measure = dynamic_cast<Measure *>(start->GetFirstParent(MEASURE));
    assert(measure);
    if (elementType == "slur") {
        Slur *slur = new Slur();
        slur->SetStartid(startid);
        slur->SetEndid(endid);
        measure->AddChild(slur);
        m_doc->PrepareDrawing();
        return true;
    }
    return false;
}

bool EditorToolkit::Insert(std::string elementType, std::string staffId, int ulx, int uly,
        int lrx, int lry, std::vector<std::pair<std::string, std::string>> attributes)
{
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get drawing page");
        return false;
    }
    if (m_doc->GetType() != Facs) {
        LogError("Drawing page without facsimile");
        return false;
    }

    Staff *staff;


    // Find closest valid staff
    if (staffId == "auto") {
        ArrayOfObjects staves;
        ClassIdComparison ac(STAFF);
        m_doc->FindAllChildByComparison(&staves, &ac);

        ClosestBB comp;
        comp.x = ulx;
        comp.y = uly;

        std::sort(staves.begin(), staves.end(), comp);
        staff = dynamic_cast<Staff *>(staves.at(0));
    }
    else {
        staff = dynamic_cast<Staff *>(m_doc->FindChildByUuid(staffId));
    }

    assert(staff);
    Layer *layer = dynamic_cast<Layer *>(staff->FindChildByType(LAYER));
    assert(layer);
    Facsimile *facsimile = m_doc->GetFacsimile();
    Zone *zone = new Zone();


    if (elementType == "staff") {
        Object *parent = staff->GetParent();
        assert(parent);
        int n = parent->GetChildCount() + 1;
        Staff *newStaff = new Staff(n);
        newStaff->m_drawingStaffDef = staff->m_drawingStaffDef;
        newStaff->m_drawingNotationType = staff->m_drawingNotationType;
        newStaff->m_drawingLines = staff->m_drawingLines;
        newStaff->m_drawingStaffSize = (uly - lry) / (newStaff->m_drawingLines - 1);
        zone->SetUlx(ulx);
        zone->SetUly(uly);
        zone->SetLrx(lrx);
        zone->SetLry(lry);
        Surface *surface = dynamic_cast<Surface *>(m_doc->GetFacsimile()->FindChildByType(SURFACE));
        assert(surface);
        surface->AddChild(zone);
        newStaff->SetZone(zone);
        newStaff->SetFacs(zone->GetUuid());
        Layer *newLayer = new Layer();
        newStaff->AddChild(newLayer);

        // Find index to insert new staff
        ArrayOfObjects staves;
        ClassIdComparison ac(STAFF);
        parent->FindAllChildByComparison(&staves, &ac);
        staves.push_back(newStaff);
        StaffSort staffSort;
        std::stable_sort(staves.begin(), staves.end(), staffSort);
        for (int i = 0; i < staves.size(); i++) {
            if (staves.at(i) == newStaff) {
                newStaff->SetParent(parent);
                parent->InsertChild(newStaff, i);
                parent->Modify();
                m_editInfo = newStaff->GetUuid();
                return true;
            }
        }
        LogMessage("Failed to insert newStaff into staff");
        parent->AddChild(newStaff);
        m_editInfo = newStaff->GetUuid();
        return true;
    }

    if (elementType == "nc" || elementType == "grouping") {
        Syllable *syllable = new Syllable();
        Syl *syl = new Syl();
        Neume *neume = new Neume();
        Nc *nc = new Nc();
        std::string contour = "";
        nc->SetZone(zone);
        nc->SetFacs(zone->GetUuid());
        Surface *surface = dynamic_cast<Surface *>(facsimile->FindChildByType(SURFACE));
        surface->AddChild(zone);
        zone->SetUlx(ulx);

        neume->AddChild(nc);
        syllable->AddChild(neume);
        syllable->AddChild(syl);
        layer->AddChild(syllable);

        // Find closest valid clef
        Clef *clef = nullptr;
        clef = layer->GetClef(nc);
        if (clef == nullptr) {
            LogError("There is no valid clef available.");
            delete syllable;
            delete neume;
            delete nc;
            return false;
        }

        nc->SetOct(3);
        if (clef->GetShape() == CLEFSHAPE_C) {
            nc->SetPname(PITCHNAME_c);
        }
        else if (clef->GetShape() == CLEFSHAPE_F) {
            nc->SetPname(PITCHNAME_f);
        }

        // Set as inclinatum or virga (if necessary), or get contour for grouping
        for (auto it = attributes.begin(); it != attributes.end(); ++it) {
            if (it->first == "tilt") {
                if (it->second == "n") {
                    data_COMPASSDIRECTION direction;
                    direction.SetBasic(COMPASSDIRECTION_basic_n);
                    nc->SetTilt(direction);
                }
                else if (it->second == "se") {
                    data_COMPASSDIRECTION direction;
                    direction.SetExtended(COMPASSDIRECTION_extended_se);
                    nc->SetTilt(direction);
                }
            }
            else if (it->first == "contour") {
                contour = it->second;
            }
        }

        const int staffSize = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        const int noteHeight = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 2);
        const int noteWidth = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 1.4);
        const int pitchDifference = round((double) (staff->GetZone()->GetUly() + (2 * staffSize * (staff->m_drawingLines - clef->GetLine())) - (uly)) / (double) (staffSize));

        nc->AdjustPitchByOffset(pitchDifference);
        ulx -= noteWidth / 2;
        uly -= noteHeight / 2;
        // Set up facsimile
        zone->SetUlx(ulx);
        zone->SetUly(uly);
        zone->SetLrx(ulx + noteWidth);
        zone->SetLry(uly + noteHeight);

        //If inserting grouping, add the remaining nc children to the neume.
        if(contour != ""){
            Nc *prevNc = nc;
            for(auto it = contour.begin(); it != contour.end(); ++it) {
                Nc *newNc = new Nc();
                Zone *newZone = new Zone();
                int newUlx = ulx + noteWidth;
                int newUly;

                newNc->SetPname(prevNc->GetPname());
                newNc->SetOct(prevNc->GetOct());

                if((*it) == 'u'){
                    newUly = uly - noteHeight;
                    newNc->AdjustPitchByOffset(1);
                }
                else if((*it) == 'd'){
                    newUly = uly + noteHeight;
                    newNc->AdjustPitchByOffset(-1);
                }
                else if((*it) == 's'){
                    newUly = uly;
                }
                else{
                    LogMessage("Unsupported character in contour.");
                    return false;
                }
                newZone->SetUlx(newUlx);
                newZone->SetUly(newUly);;
                newZone->SetLrx(newUlx + noteWidth);
                newZone->SetLry(newUly + noteHeight);

                newNc->SetZone(newZone);
                newNc->SetFacs(newZone->GetUuid());

                assert(surface);
                surface->AddChild(newZone);

                neume->AddChild(newNc);

                ulx = newUlx;
                uly = newUly;
                prevNc = newNc;
            }
        }
        if(elementType == "nc"){
            m_editInfo = nc->GetUuid();
        }
        else{
            m_editInfo = neume->GetUuid();
        }
    }
    else if (elementType == "clef") {
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
            return false;
        }
        clef->SetShape(clefShape);
        const int staffSize = m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
        int yDiff = -staff->GetZone()->GetUly() + uly;
        int clefLine = staff->m_drawingLines - round((double) yDiff / (double) staffSize);
        clef->SetLine(clefLine);

        Zone *zone = new Zone();
        zone->SetUlx(ulx);
        zone->SetUly(uly);
        zone->SetLrx(ulx + staffSize / 1.4);
        zone->SetLry(uly + staffSize / 2);
        clef->SetZone(zone);
        clef->SetFacs(zone->GetUuid());
        Surface *surface = dynamic_cast<Surface *>(facsimile->FindChildByType(SURFACE));
        assert(surface);
        surface->AddChild(zone);
        layer->AddChild(clef);
        m_editInfo = clef->GetUuid();
        layer->ReorderByXPos();

        // Ensure children of this clef keep their position if it is NOT the first clef in the file.
        ArrayOfObjects clefs;
        ClassIdComparison ac(CLEF);
        m_doc->GetDrawingPage()->FindAllChildByComparison(&clefs, &ac);
        if (clefs.size() == 0) {
            LogError("Something went wrong. Clef does not appear to be inserted.");
        } else if (clefs.size() > 1) {
            Clef *previousClef = dynamic_cast<Clef *>(clefs.at(0));
            Clef *temp = nullptr;
            for (auto it = clefs.begin(); it != clefs.end(); ++it) {
                temp = dynamic_cast<Clef *>(*it);
                if (temp == nullptr) {
                    LogWarning("Null clef!");
                    continue;
                }
                if (temp != clef) {
                    previousClef = temp;
                } else {
                    temp = dynamic_cast<Clef *>(*(it+1));
                    break;
                }
            }
            if (previousClef != clef) { // if the new clef is the first clef we do nothing
                // Get difference to shift pitches by
                int pitchDifference = (previousClef->GetLine() - clefLine) * 2;
                // Account for clef shape
                if (previousClef->GetShape() != clefShape) {
                    if (previousClef->GetShape() == CLEFSHAPE_F) {
                        // Assume this clef is C
                        pitchDifference -= 3;
                    }
                    else if (previousClef->GetShape() ==CLEFSHAPE_C) {
                        // Assume this clef is F
                        pitchDifference += 3;
                    }
                }

                // Adjust elements with a relative position to clef by pitch
                ArrayOfObjects elements;
                InterfaceComparison ic(INTERFACE_PITCH);
                m_doc->GetDrawingPage()->FindAllChildBetween(&elements, &ic, clef, (temp != clefs.back()) ? temp : m_doc->GetDrawingPage()->GetLast());
                for (auto it = elements.begin(); it != elements.end(); ++it) {
                    PitchInterface *pi = (*it)->GetPitchInterface();
                    assert(pi);
                    pi->AdjustPitchByOffset(pitchDifference);
                }
            }
        }
    }
    else if (elementType == "custos") {
        Custos *custos = new Custos();
        zone->SetUlx(ulx);
        Surface *surface = dynamic_cast<Surface *>(facsimile->GetFirst(SURFACE));
        surface->AddChild(zone);
        custos->SetZone(zone);
        custos->SetFacs(zone->GetUuid());
        layer->AddChild(custos);
        // Find closest valid clef
        Clef *clef = nullptr;
        clef = layer->GetClef(custos);
        if (clef == nullptr) {
            LogError("There is no valid clef available.");
            delete custos;
            return false;
        }

        custos->SetOct(3);
        if (clef->GetShape() == CLEFSHAPE_C)
            custos->SetPname(PITCHNAME_c);
        else if (clef->GetShape() == CLEFSHAPE_F)
            custos->SetPname(PITCHNAME_f);

        const int staffSize = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        const int noteHeight = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 2);
        const int noteWidth = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 1.4);
        const int pitchDifference = round((double) (staff->GetZone()->GetUly() + (2 * staffSize * (staff->m_drawingLines - clef->GetLine())) - (uly)) / (double) (staffSize));

        custos->AdjustPitchByOffset(pitchDifference);
        ulx -= noteWidth / 2;
        uly -= noteHeight / 2;

        zone->SetUlx(ulx);
        zone->SetUly(uly);
        zone->SetLrx(ulx + noteWidth);
        zone->SetLry(uly + noteHeight);
        m_editInfo = custos->GetUuid();
    }
    else {
        LogError("Unsupported type '%s' for insertion", elementType.c_str());
        return false;
    }
    layer->ReorderByXPos();
    return true;
}

bool EditorToolkit::Merge(std::vector<std::string> elementIds)
{
    m_editInfo = "";
    if (!m_doc->GetDrawingPage()) return false;
    ArrayOfObjects staves;
    int ulx = INT_MAX, uly = 0, lrx = 0, lry = 0;

    // Get the staves by element ID and fail if a staff does not exist.
    for (auto it = elementIds.begin(); it != elementIds.end(); ++it) {
        Object *obj = m_doc->GetDrawingPage()->FindChildByUuid(*it);
        if (obj != nullptr && obj->Is(STAFF)) {
            staves.push_back(obj);
            Zone *zone = obj->GetFacsimileInterface()->GetZone();
            ulx = ulx < zone->GetUlx() ? ulx : zone->GetUlx();
            lrx = lrx > zone->GetLrx() ? lrx : zone->GetLrx();
            uly += zone->GetUly();
            lry += zone->GetLry();
        }
        else {
            LogWarning("Staff with ID '%s' does not exist!", it->c_str());
            return false;
        }
    }
    if (staves.size() < 2) {
        LogWarning("At least two staves must be provided.");
        return false;
    }

    uly /= staves.size();
    lry /= staves.size();
    StaffSort staffSort;
    std::sort(staves.begin(), staves.end(), staffSort);

    // Move children to the first staff (in order)
    auto stavesIt = staves.begin();
    Staff *fillStaff = dynamic_cast<Staff *>(*stavesIt);
    Layer *fillLayer = dynamic_cast<Layer *>(fillStaff->GetFirst(LAYER));
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

    fillLayer->ReorderByXPos();

    m_editInfo = fillStaff->GetUuid();

    // TODO change zones for staff children

    return true;
}

bool EditorToolkit::Set(std::string elementId, std::string attrType, std::string attrValue)
{
    if (!m_doc->GetDrawingPage()) return false;
    Object *element = m_doc->GetDrawingPage()->FindChildByUuid(elementId);
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
    return success;
}

// Update the text of a TextElement by its syl
bool EditorToolkit::SetText(std::string elementId, std::string text)
{
    m_editInfo = "";
    std::wstring wtext;
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    wtext = conv.from_bytes(text);

    if (!m_doc->GetDrawingPage()) return false;
    Object *element = m_doc->GetDrawingPage()->FindChildByUuid(elementId);
    if (element == nullptr) {
        LogWarning("No element with ID '%s' exists", elementId.c_str());
        return false;
    }

    bool success = false;
    if (element->Is(SYL)) {
        Syl *syl = dynamic_cast<Syl *>(element);
        assert(syl);
        Object *child = syl->GetFirst();
        if(child == nullptr) {
            Text *text = new Text();
            syl->AddChild(text);
            text->SetText(wtext);
            success = true;
        }
        else {
            while(child != nullptr) {
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
        Syllable *syllable = dynamic_cast<Syllable *>(element);
        assert(syllable);
        Object *syl = syllable->GetFirst(SYL);
        if (syl == nullptr) {
            syl = new Syl();
            syllable->AddChild(syl);
            Text *textChild = new Text();
            textChild->SetText(wtext);
            syl->AddChild(textChild);
            success = true;
        }
        else {
            success = SetText(syl->GetUuid(), text);
        }
    }
    else {
        LogWarning("Element type '%s' is unsupported for SetText", element->GetClassName().c_str());
        return false;
    }
    return success;
}

bool EditorToolkit::SetClef(std::string elementId, std::string shape)
{
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page.");
        return false;
    }
    ArrayOfObjects objects;
    bool success = false;
    data_CLEFSHAPE clefShape = CLEFSHAPE_NONE;
    int shift = 0;
    Clef *clef = dynamic_cast<Clef *>(m_doc->GetDrawingPage()->FindChildByUuid(elementId));
    assert(clef);

    if (shape == "C") {
        clefShape = CLEFSHAPE_C;
        shift = -3;
    }
    else if (shape== "F") {
        clefShape = CLEFSHAPE_F;
        shift = 3;
    }

    if(clef->GetShape() != clefShape){
        success = Att::SetShared(clef, "shape", shape);
        if(!success){
            LogWarning("Unable to set clef shape");
            return false;
        }

        Layer *layer = dynamic_cast<Layer *>(clef->GetFirstParent(LAYER));
        assert(layer);

        Object *nextClef = m_doc->GetDrawingPage()->GetNext(clef, CLEF);
        assert(nextClef);
        InterfaceComparison ic(INTERFACE_PITCH);

        m_doc->GetDrawingPage()->FindAllChildBetween(&objects, &ic, clef,
            (nextClef != nullptr) ? nextClef : m_doc->GetDrawingPage()->GetLast());

        // Adjust all elements who are positioned relative to clef by pitch
        for (auto it = objects.begin(); it != objects.end(); ++it) {
            Object *child = dynamic_cast<Object *>(*it);
            if (child == nullptr || layer->GetClef(dynamic_cast<LayerElement *>(child)) != clef) continue;
            PitchInterface *pi = child->GetPitchInterface();
            assert(pi);
            pi->AdjustPitchByOffset(shift);
        }
    }
    if (success && m_doc->GetType() != Facs) {
        m_doc->PrepareDrawing();
        m_doc->GetDrawingPage()->LayOut(true);
    }
    return true;
}

bool EditorToolkit::Split(std::string elementId, int x)
{
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page");
        return false;
    }
    Staff *staff = dynamic_cast<Staff *>(m_doc->GetDrawingPage()->FindChildByUuid(elementId));
    // Validate parameters
    if (staff == nullptr) {
        LogError("Either no element exists with ID '%s' or it is not a staff.", elementId.c_str());
        return false;
    }

    if (staff->GetZone()->GetUlx() > x || staff->GetZone()->GetLrx() < x) {
        LogError("The 'x' parameter is not within the bounds of the original staff.");
        return false;
    }

    // Resize current staff and insert new one filling remaining area.
    int newUlx = x;
    int newLrx = staff->GetZone()->GetLrx();
    std::vector<std::pair<std::string, std::string>> v;

    if (!this->Insert("staff", "auto", newUlx, staff->GetZone()->GetUly(), newLrx, staff->GetZone()->GetLry(), v)) {
        LogError("Failed to create a second staff.");
        return false;
    }
    Staff *splitStaff = dynamic_cast<Staff *>(m_doc->GetDrawingPage()->FindChildByUuid(m_editInfo));
    assert(splitStaff);
    if (splitStaff == nullptr) {
        LogMessage("Split staff is null");
    }

    staff->GetZone()->SetLrx(x);
    Layer *layer = dynamic_cast<Layer *>(staff->GetFirst(LAYER));
    Layer *splitLayer = dynamic_cast<Layer *>(splitStaff->GetFirst(LAYER));

    // Move any elements that should be on the second staff there.
    for (Object *child = layer->GetFirst(); child != nullptr; child = layer->GetNext()) {
        assert(child);
        FacsimileInterface *fi = dynamic_cast<FacsimileInterface *>(child);
        if (fi == nullptr || !fi->HasFacs()) {
            fi = nullptr;
            ArrayOfObjects facsimileInterfaces;
            InterfaceComparison ic(INTERFACE_FACSIMILE);
            child->FindAllChildByComparison(&facsimileInterfaces, &ic);

            for (auto it = facsimileInterfaces.begin(); it != facsimileInterfaces.end(); ++it) {
                FacsimileInterface *temp = dynamic_cast<FacsimileInterface *>(*it);
                assert(temp);
                if (temp->HasFacs() && (fi == nullptr || temp->GetZone()->GetUlx() < fi->GetZone()->GetUlx())) {
                    fi = temp;
                }
            }
        }

        if (fi == nullptr) {
            continue;
        }
        else if (fi->GetZone()->GetUlx() > x) {
            child->MoveItselfTo(splitLayer);
        }
    }
    layer->ClearRelinquishedChildren();
    m_editInfo = splitStaff->GetUuid();
    return true;
}

bool EditorToolkit::Remove(std::string elementId)
{
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page.");
        return false;
    }
    Object *obj = m_doc->GetDrawingPage()->FindChildByUuid(elementId);
    assert(obj);
    bool result, isNeume;
    isNeume = (obj->Is(NC) || obj->Is(NEUME) || obj->Is(SYLLABLE));
    Object *parent = obj->GetParent();
    assert(parent);
    m_editInfo = elementId;
    // Remove Zone for element (if any)
    InterfaceComparison ic(INTERFACE_FACSIMILE);
    ArrayOfObjects fiChildren;
    obj->FindAllChildByComparison(&fiChildren, &ic);
    FacsimileInterface *fi = dynamic_cast<FacsimileInterface *>(obj);
    if (fi != nullptr && fi->HasFacs()) {
        fi->SetZone(nullptr);
    }
    for (auto it = fiChildren.begin(); it != fiChildren.end(); ++it) {
        fi = dynamic_cast<FacsimileInterface *>(*it);
        if (fi != nullptr && fi->HasFacs()) {
            fi->SetZone(nullptr);
        }
    }
    result = parent->DeleteChild(obj);
    if (isNeume && result) {
        if (!parent->Is(SYLLABLE)) {
            parent = parent->GetFirstParent(SYLLABLE);
            if (parent == nullptr) { LogMessage("Failed to get syllable parent!"); return false; }
        }
        assert(parent->Is(SYLLABLE));
        if (parent->FindChildByType(NC) == nullptr) {
            obj = parent;
            parent = parent->GetParent();
            if (parent == nullptr) { LogMessage("Null parent!"); return false; }
            // Remove Zone for element (if any)
            fi = dynamic_cast<FacsimileInterface *>(obj);
            if (fi != nullptr && fi->HasFacs()) {
                fi->SetZone(nullptr);
            }
            result &= parent->DeleteChild(obj);
        }
    }
    return result;
}

bool EditorToolkit::Resize(std::string elementId, int ulx, int uly, int lrx, int lry)
{
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page.");
        return false;
    }
    if (m_doc->GetType() != Facs) {
        LogWarning("Resizing is only available in facsimile mode.");
        return false;
    }

    Object *obj = m_doc->GetDrawingPage()->FindChildByUuid(elementId);
    if (obj == nullptr) {
        LogError("Object with ID '%s' not found.", elementId.c_str());
        return false;
    }
    if (obj->Is(STAFF)) {
        Staff *staff = dynamic_cast<Staff *>(obj);
        assert(staff);
        if (!staff->HasFacs()) {
            LogError("This staff does not have a facsimile.");
            return false;
        }
        Zone *zone = staff->GetZone();
        zone->SetUlx(ulx);
        zone->SetUly(uly);
        zone->SetLrx(lrx);
        zone->SetLry(lry);
        zone->Modify();
    }
    else {
        LogMessage("Element of type '%s' is unsupported.", obj->GetClassName().c_str());
        return false;
    }
    return true;
}

std::string EditorToolkit::EditInfo()
{
    return m_editInfo;
}

bool EditorToolkit::Group(std::string groupType, std::vector<std::string> elementIds)
{
    m_editInfo = "";
    Object *parent = nullptr, *doubleParent = nullptr;
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    std::map<Object *, int> parents;
    std::set<Object *> elements;
    std::set<Object *> fullParents;

    //Get the current drawing page
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page.");
        return false;
    }
    if (elementIds.size() == 0) {
        LogWarning("No element IDs to group!");
        return true;
    }
    ClassId elementClass;
    if (groupType == "nc") {
        elementClass = NC;
    } else if (groupType == "neume") {
        elementClass = NEUME;
    } else {
        LogError("Invalid groupType: %s", groupType.c_str());
        return false;
    }

    // Determine what the parents are
    for (auto it = elementIds.begin(); it != elementIds.end(); ++it) {
        // Verify that the children are of the same type
        Object *el = m_doc->GetDrawingPage()->FindChildByUuid(*it);
        if (el == nullptr) {
            LogError("Could not get element with ID %s", it->c_str());
            return false;
        }
        if (el->GetClassId() != elementClass) {
            LogError("Element %s was of class %s. Expected class %s",
                el->GetUuid().c_str(), el->GetClassName().c_str(), groupType.c_str());
            return false;
        }

        // Get a set of parents and the number of children they have
        Object *par = el->GetParent();
        if (par == nullptr) {
            LogError("Parent of %s is null!", el->GetUuid().c_str());
            return false;
        }
        if (doubleParent == nullptr) {
            doubleParent = par->GetParent();
            if (doubleParent == nullptr) {
                LogError("No second level parent!");
                return false;
            }
        }
        else {
            if (par->GetParent() != doubleParent) {
                LogError("No shared second level parent!");
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

    // find parents where all of their children are being grouped
    for (auto it = parents.begin(); it != parents.end(); ++it) {
        auto parentPair = *it;
        Object *par = parentPair.first;
        int expected;
        if (par->GetClassId() == SYLLABLE) {
            expected = par->GetChildCount(NEUME);
        } else {
            expected = par->GetChildCount();
        }
        if (parentPair.second == expected) {
            fullParents.emplace(parentPair.first);
        }
    }
    //if there are no full parents we need to make a new one to attach everything to
    if (fullParents.empty()) {
        if (elementClass == NC) {
            parent = new Neume();
        }
        else if (elementClass == NEUME) {
            parent = new Syllable();

            //make sure to add an empty syl
            Syl *syl = new Syl();
            parent->AddChild(syl);
        }
        for (auto it = elements.begin(); it != elements.end(); ++it) {
            if ((*it)->GetParent() != parent && !(*it)->Is(SYL)) {
                (*it)->MoveItselfTo(parent);
            }
        }
        doubleParent->AddChild(parent);
        Layer *layer = dynamic_cast<Layer *> (parent->GetFirstParent(LAYER));
        assert(layer);
        layer->ReorderByXPos();
    }

    //if there's only one full parent we just add the other elements to it
    //except don't move syl tags since we want them to stay attached to the first parent
    else if(fullParents.size() == 1) {
        auto iter = fullParents.begin();
        parent = *iter;
        for (auto it = elements.begin(); it != elements.end(); ++it) {
            if ((*it)->GetParent() != parent && !(*it)->Is(SYL)) {
                (*it)->MoveItselfTo(parent);
            }
        }
    }

    //if there are more than 1 full parent we need to concat syl's
    else {
        Syllable *fullSyllable = new Syllable();
        Syl *fullSyl = new Syl();
        std::wstring fullString = L"";
        for(auto it = fullParents.begin(); it != fullParents.end(); ++it) {
            Object *tempText = (*it)->FindChildByType(SYL)->FindChildByType(TEXT);
            Text *text = dynamic_cast<Text *> (tempText);
            if(text != nullptr) {
                std::wstring currentString = text->GetText();
                fullString = fullString + currentString;
            }
        }
        Text *text = new Text();
        text->SetText(fullString);
        fullSyl->AddChild(text);
        fullSyllable->AddChild(fullSyl);
        for (auto it = elements.begin(); it != elements.end(); ++it) {
            if ((*it)->GetParent() != fullSyllable && !(*it)->Is(SYL)) {
                (*it)->MoveItselfTo(fullSyllable);
            }
        }
        doubleParent->AddChild(fullSyllable);
        Layer *layer = dynamic_cast<Layer *> (fullSyllable->GetFirstParent(LAYER));
        assert(layer);
        layer->ReorderByXPos();

    }
    // Delete any empty parents
    for (auto it = parents.begin(); it != parents.end(); ++it) {
        Object *obj = (*it).first;
        obj->ClearRelinquishedChildren();
        if (obj->GetChildCount() == 0) {
            doubleParent->DeleteChild(obj);
        } else if (obj->GetChildCount() == obj->GetChildCount(SYL)) {
            Object *syl;
            while ((syl = obj->FindChildByType(SYL)) != nullptr) {
                obj->DeleteChild(syl);
            }
            doubleParent->DeleteChild(obj);
        }
    }

    m_editInfo = parent->GetUuid();
    return true;
}

bool EditorToolkit::Ungroup(std::string groupType, std::vector<std::string> elementIds)
{
    m_editInfo = "";
    Object *fparent, *sparent, *currentParent;
    Nc *firstNc, *secondNc;
    bool success1, success2;
    int ligCount = 0;
    bool firstIsSyl = false;

    //Check if you can get drawing page
    if(!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page.");
        return false;
    }
    for (auto it = elementIds.begin(); it != elementIds.end(); ++it) {
        Object *el = m_doc->GetDrawingPage()->FindChildByUuid(*it);
        //Check for ligatures and toggle them before ungrouping
        //only if the ligature is the entire selection
        if(groupType == "nc" && elementIds.size() == 2){
            Nc *nc = dynamic_cast<Nc *> (el);
            if(nc->HasLigated() && nc->GetLigated() == BOOLEAN_true){
                nc->SetLigated(BOOLEAN_false);
                ligCount++;
                if(ligCount == 1){
                    firstNc = nc;
                    assert(firstNc);
                }
                else if(ligCount == 2){
                    secondNc = nc;
                    assert(secondNc);
                    Zone *zone = new Zone();

                    Staff *staff = dynamic_cast<Staff *> (firstNc->GetFirstParent(STAFF));
                    assert(staff);
                    Facsimile *facsimile = m_doc->GetFacsimile();
                    assert(facsimile);
                    Surface *surface = dynamic_cast<Surface *>(facsimile->FindChildByType(SURFACE));
                    assert(surface);

                    const int noteHeight = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 2);
                    const int noteWidth = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 1.4);

                    if (Att::SetVisual(firstNc, "ligature", "false")) success1 = true;

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

                    secondNc->SetZone(zone);
                    secondNc->ResetFacsimile();
                    secondNc->SetFacs(zone->GetUuid());

                    if (Att::SetVisual(secondNc, "ligature", "false")) success2 = true;
                    if(success1 && success2){
                        ligCount = 0;
                        firstNc = nullptr;
                        secondNc = nullptr;
                    }
                    else{
                        LogWarning("Unable to toggle ligature within ungroup ncs!");
                        return false;
                    }
                }
            }
        }
        if (elementIds.begin() == it || firstIsSyl){
            //if the element is a syl we want it to stay attached to the first element
            //we'll still need to initialize all the parents, thus the bool
            if(el->Is(SYL)) {
                firstIsSyl = true;
                continue;
            }
            else if(groupType == "nc"){
                fparent = el->GetFirstParent(NEUME);
                assert(fparent);
                sparent = fparent->GetFirstParent(SYLLABLE);
                assert(sparent);
                currentParent = dynamic_cast<Neume *>(fparent);
                assert(currentParent);
                firstIsSyl = false;
            }
            else if(groupType == "neume"){
                fparent = el->GetFirstParent(SYLLABLE);
                assert(fparent);
                sparent = fparent->GetFirstParent(LAYER);
                assert(sparent);
                currentParent = dynamic_cast<Syllable *>(fparent);
                assert(currentParent);
                firstIsSyl = false;
                
            }
            else{
                LogError("Invalid groupType for ungrouping");
                return false;
            }
        }
        else {
            if (groupType == "nc") {
                Nc *nc = dynamic_cast<Nc*>(el);
                assert(nc);
                if (nc->HasLigated()) continue;
            }

            //if the element is a syl then we want to keep it attached to the first node

            if(el->Is(SYL)) {
                continue;
            }
            Object *newParent = currentParent->Clone();
            assert(newParent);
            newParent->ClearChildren();

            if(newParent->Is(SYLLABLE)) {
                Syl *syl = new Syl();
                newParent->AddChild(syl);
            }
            
            el->MoveItselfTo(newParent);
            fparent->ClearRelinquishedChildren();
            sparent->AddChild(newParent);
            sparent->ReorderByXPos();
        }
    }
    return true;
}

bool EditorToolkit::ChangeGroup(std::string elementId, std::string contour)
{
    m_editInfo = "";
    //Check if you can get drawing page
    if(!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page.");
        return false;
    }
    Neume *el = dynamic_cast<Neume *> (m_doc->GetDrawingPage()->FindChildByUuid(elementId));
    if(el == nullptr){
        LogError("Unable to find neume with id %s", elementId.c_str());
        return false;
    }
    Nc *firstChild, *prevNc;

    //Get children of neume. Keep the first child and delete the others.
    ClassIdComparison ac(NC);
    ArrayOfObjects children;
    el->FindAllChildByComparison(&children, &ac);
    for (auto it = children.begin(); it != children.end(); ++it) {
        if(children.begin() == it){
            firstChild = dynamic_cast<Nc *> (*it);
        }
        else{
            el->DeleteChild(*it);
        }
    }
    //Get the coordinates of the remaining child.
    int initialUlx = firstChild->GetZone()->GetUlx();
    int initialUly = firstChild->GetZone()->GetUly();
    int initialLrx = firstChild->GetZone()->GetLrx();
    int initialLry = firstChild->GetZone()->GetLry();

    Staff *staff = dynamic_cast<Staff *> (el->GetFirstParent(STAFF));
    assert(staff);
    Facsimile *facsimile = m_doc->GetFacsimile();

    const int noteHeight = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 2);
    const int noteWidth = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 1.4);
    prevNc = firstChild;

    //Iterate throught the contour and build the new grouping.
    for(auto it = contour.begin(); it != contour.end(); ++it) {
        Nc *newNc = new Nc();
        Zone *zone = new Zone();
        int newUlx = initialUlx + noteWidth;
        int newLrx = initialLrx + noteWidth;
        int newUly, newLry;

        newNc->SetPname(prevNc->GetPname());
        newNc->SetOct(prevNc->GetOct());

        if((*it) == 'u'){
            newUly = initialUly - noteHeight;
            newLry = initialLry - noteHeight;
            newNc->AdjustPitchByOffset(1);
        }
        else if((*it) == 'd'){
            newUly = initialUly + noteHeight;
            newLry = initialLry + noteHeight;
            newNc->AdjustPitchByOffset(-1);
        }
        else if((*it) == 's'){
            newUly = initialUly;
            newLry = initialLry;
        }
        else{
            LogMessage("Unsupported character in contour.");
            return false;
        }
        zone->SetUlx(newUlx);
        zone->SetUly(newUly);;
        zone->SetLrx(newLrx);
        zone->SetLry(newLry);

        newNc->SetZone(zone);
        newNc->SetFacs(zone->GetUuid());

        Surface *surface = dynamic_cast<Surface *>(facsimile->FindChildByType(SURFACE));
        assert(surface);
        surface->AddChild(zone);

        el->AddChild(newNc);

        initialUlx = newUlx;
        initialUly = newUly;
        initialLrx = newLrx;
        initialLry = newLry;
        prevNc = newNc;
    }
    m_editInfo = el->GetUuid();
    return true;
}

bool EditorToolkit::ToggleLigature(std::vector<std::string> elementIds, std::string isLigature)
{
    m_editInfo = "";
    bool success1 = false;
    bool success2 = false;
    Facsimile *facsimile = m_doc->GetFacsimile();
    assert(facsimile);
    Surface *surface = dynamic_cast<Surface *>(facsimile->FindChildByType(SURFACE));
    assert(surface);
    std::string firstNcId = elementIds[0];
    std::string secondNcId = elementIds[1];
    //Check if you can get drawing page
    if(!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page.");
        return false;
    }

    Nc *firstNc = dynamic_cast<Nc *> (m_doc->GetDrawingPage()->FindChildByUuid(firstNcId));
    assert(firstNc);
    Nc *secondNc = dynamic_cast<Nc *> (m_doc->GetDrawingPage()->FindChildByUuid(secondNcId));
    assert(secondNc);
    Zone *zone = new Zone();
    //set ligature to false and update zone of second Nc
    if(isLigature == "true"){
        if (Att::SetVisual(firstNc, "ligature", "false")) success1 = true;

        int ligUlx = firstNc->GetZone()->GetUlx();
        int ligUly = firstNc->GetZone()->GetUly();
        int ligLrx = firstNc->GetZone()->GetLrx();
        int ligLry = firstNc->GetZone()->GetLry();

        Staff *staff = dynamic_cast<Staff *> (firstNc->GetFirstParent(STAFF));
        assert(staff);

        const int noteHeight = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 2);
        const int noteWidth = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 1.4);

        zone->SetUlx(ligUlx + noteWidth);
        zone->SetUly(ligUly + noteHeight);
        zone->SetLrx(ligLrx + noteWidth);
        zone->SetLry(ligLry + noteHeight);

        Zone *origZoneUuid = secondNc->GetZone();
        surface->DeleteChild(origZoneUuid);

        secondNc->SetZone(zone);
        secondNc->ResetFacsimile();
        secondNc->SetFacs(zone->GetUuid());

        if (Att::SetVisual(secondNc, "ligature", "false")) success2 = true;
    }
    //set ligature to true and update zones to be the same
    else if (isLigature == "false"){
        if (Att::SetVisual(firstNc, "ligature", "true")) success1 = true;

        zone->SetUlx(firstNc->GetZone()->GetUlx());
        zone->SetUly(firstNc->GetZone()->GetUly());
        zone->SetLrx(firstNc->GetZone()->GetLrx());
        zone->SetLry(firstNc->GetZone()->GetLry());

        Zone *origZoneUuid = secondNc->GetZone();
        surface->DeleteChild(origZoneUuid);

        secondNc->SetZone(zone);
        secondNc->ResetFacsimile();
        secondNc->SetFacs(zone->GetUuid());

        if (Att::SetVisual(secondNc, "ligature", "true")) success2 = true;
    }
    else {
        LogWarning("isLigature is invalid!");
        return false;
    }
    if (success1 && success2 && m_doc->GetType() != Facs) {
        m_doc->PrepareDrawing();
        m_doc->GetDrawingPage()->LayOut(true);
    }
    if(!(success1 && success2)){
        LogWarning("Unable to update ligature attribute");
    }

    surface->AddChild(zone);
    return success1 && success2;
}

bool EditorToolkit::ParseDragAction(jsonxx::Object param, std::string *elementId, int *x, int *y)
{
    if (!param.has<jsonxx::String>("elementId")) return false;
    (*elementId) = param.get<jsonxx::String>("elementId");
    if (!param.has<jsonxx::Number>("x")) return false;
    (*x) = param.get<jsonxx::Number>("x");
    if (!param.has<jsonxx::Number>("y")) return false;
    (*y) = param.get<jsonxx::Number>("y");
    return true;
}

bool EditorToolkit::ParseInsertAction(
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

bool EditorToolkit::ParseInsertAction(
    jsonxx::Object param, std::string *elementType, std::string *staffId, int *ulx, int *uly,
    int *lrx, int *lry, std::vector<std::pair<std::string, std::string>> *attributes)
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

bool EditorToolkit::ParseMergeAction(
    jsonxx::Object param, std::vector<std::string> *elementIds)
{
    if (!param.has<jsonxx::Array>("elementIds")) return false;
    jsonxx::Array array = param.get<jsonxx::Array>("elementIds");
    for (int i = 0; i < array.size(); i++) {
        elementIds->push_back(array.get<jsonxx::String>(i));
    }
    return true;
}

bool EditorToolkit::ParseSplitAction(
    jsonxx::Object param, std::string *elementId, int *x)
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

bool EditorToolkit::ParseSetAction(
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

bool EditorToolkit::ParseSetTextAction(
    jsonxx::Object param, std::string *elementId, std::string *text)
{
    if (!param.has<jsonxx::String>("elementId")) {
        LogWarning("Could not parse 'elementId'");
        return false;
    }
    *elementId = param.get<jsonxx::String>("elementId");
    if(!param.has<jsonxx::String>("text")) {
        LogWarning("Could not parse 'text'");
        return false;
    }
    *text = param.get<jsonxx::String>("text");
    return true;
}

bool EditorToolkit::ParseSetClefAction(
    jsonxx::Object param, std::string *elementId, std::string *shape)
{
    if(!param.has<jsonxx::String>("elementId")) {
        LogWarning("Could not parse 'elementId'");
        return false;
    }
    *elementId = param.get<jsonxx::String>("elementId");
    if(!param.has<jsonxx::String>("shape")) {
        LogWarning("Could not parse 'shape'");
        return false;
    }
    *shape = param.get<jsonxx::String>("shape");
    return true;
}

bool EditorToolkit::ParseRemoveAction(
    jsonxx::Object param, std::string *elementId)
{
    if (!param.has<jsonxx::String>("elementId")) return false;
    (*elementId) = param.get<jsonxx::String>("elementId");
    return true;
}

bool EditorToolkit::ParseResizeAction(
    jsonxx::Object param, std::string *elementId, int *ulx, int *uly, int *lrx, int *lry)
{
    if(!param.has<jsonxx::String>("elementId")) return false;
    *elementId = param.get<jsonxx::String>("elementId");
    if(!param.has<jsonxx::Number>("ulx")) return false;
    *ulx = param.get<jsonxx::Number>("ulx");
    if(!param.has<jsonxx::Number>("uly")) return false;
    *uly = param.get<jsonxx::Number>("uly");
    if(!param.has<jsonxx::Number>("lrx")) return false;
    *lrx = param.get<jsonxx::Number>("lrx");
    if(!param.has<jsonxx::Number>("lry")) return false;
    *lry = param.get<jsonxx::Number>("lry");
    return true;
}

bool EditorToolkit::ParseGroupAction(
    jsonxx::Object param, std::string *groupType, std::vector<std::string> *elementIds)
{
    if(!param.has<jsonxx::String>("groupType")) return false;
    (*groupType) = param.get<jsonxx::String>("groupType");
    if(!param.has<jsonxx::Array>("elementIds")) return false;
    jsonxx::Array array = param.get<jsonxx::Array>("elementIds");
    for (int i = 0; i < array.size(); i++) {
        elementIds->push_back(array.get<jsonxx::String>(i));
    }

    return true;
}

bool EditorToolkit::ParseUngroupAction(
    jsonxx::Object param, std::string *groupType, std::vector<std::string> *elementIds)
{
    if(!param.has<jsonxx::String>("groupType")) return false;
    (*groupType) = param.get<jsonxx::String>("groupType");
    if(!param.has<jsonxx::Array>("elementIds")) return false;
    jsonxx::Array array = param.get<jsonxx::Array>("elementIds");
    for (int i = 0; i < array.size(); i++) {
        elementIds->push_back(array.get<jsonxx::String>(i));
    }

    return true;
}

bool EditorToolkit::ParseChangeGroupAction(
    jsonxx::Object param, std::string *elementId, std::string *contour)
{
    if(!param.has<jsonxx::String>("elementId")) return false;
    (*elementId) = param.get<jsonxx::String>("elementId");
    if(!param.has<jsonxx::String>("contour")) return false;
    (*contour) = param.get<jsonxx::String>("contour");
    return true;
}

bool EditorToolkit::ParseToggleLigatureAction(
    jsonxx::Object param, std::vector<std::string> *elementIds, std::string *isLigature)
{
    if(!param.has<jsonxx::Array>("elementIds")) return false;
    jsonxx::Array array = param.get<jsonxx::Array>("elementIds");
    for (int i = 0; i < array.size(); i++) {
        elementIds->push_back(array.get<jsonxx::String>(i));
    }
    if(!param.has<jsonxx::String>("isLigature")) return false;
    (*isLigature) = param.get<jsonxx::String>("isLigature");

    return true;
}

#endif
// USE_EMSCRIPTEN
}// namespace vrv
