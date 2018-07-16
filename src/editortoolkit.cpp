/////////////////////////////////////////////////////////////////////////////
// Name:        editortoolkit.cpp
// Author:      Laurent Pugin, Juliette Regimbal
// Created:     16/05/2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "editortoolkit.h"

//-------------------------------------------------------------------------------- 

#include <set>

//--------------------------------------------------------------------------------

#include "clef.h"
#include "custos.h"
#include "layer.h"
#include "measure.h"
#include "nc.h"
#include "neume.h"
#include "page.h"
#include "slur.h"
#include "staff.h"
#include "staffdef.h"
#include "syllable.h"
#include "vrv.h"

//--------------------------------------------------------------------------------

#include "jsonxx.h"

namespace vrv {

#ifdef USE_EMSCRIPTEN
bool EditorToolkit::ParseEditorAction(const std::string &json_editorAction)
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
            return this->Drag(elementId, x, y);
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
    else if (action == "remove") {
        std::string elementId;
        if (this->ParseRemoveAction(json.get<jsonxx::Object>("param"), &elementId)) {
            return this->Remove(elementId);
        }
        LogWarning("Could not parse the remove action");
    }
    else if (action == "chain") {
        if (!json.has<jsonxx::Array>("param")) {
            LogError("Incorrectly formatted JSON action");
            return false;
        }
        return this->Chain(json.get<jsonxx::Array>("param"));
    }
    else {
        LogWarning("Unknown action type.");
    }
    return false;
}

bool EditorToolkit::Chain(jsonxx::Array actions)
{
    bool status = true;
    std::string info = "[";
    for (int i = 0; i < actions.size(); i++) {
        if (!actions.has<jsonxx::Object>(i)) {
            LogError("Action %d was not an object", i);
            return false;
        }
        status |= this->ParseEditorAction(actions.get<jsonxx::Object>(i).json());
        if (i != 0)
            info += ", ";
        info += "\"" + m_editInfo + "\"";
    }
    info += "]";
    m_editInfo = info; 
    return status;
}

bool EditorToolkit::Drag(std::string elementId, int x, int y)
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
                if (nc->GetLigature() == BOOLEAN_true) {
                    Neume *neume = dynamic_cast<Neume *>(nc->GetFirstParent(NEUME));
                    Nc *nextNc = dynamic_cast<Nc *>(neume->GetChild(1 + neume->GetChildIndex(element)));
                    if (nextNc != nullptr && nextNc->GetLigature() == BOOLEAN_true && nextNc->GetZone() == nc->GetZone())
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
        AttComparison ac(NC);
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
            for (Object *child = neume->GetFirst(); child != nullptr; child = neume->GetNext()) {
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

            layer->FindAllChildByComparison(&objects, &ic); 

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
        // Move staff facsimile
        Zone *zone = staff->GetZone();
        assert(zone);
        zone->ShiftByXY(x, y);

        // Move all staff children with facsimiles
        ArrayOfObjects children;
        InterfaceComparison ic(INTERFACE_FACSIMILE);
        staff->FindAllChildByComparison(&children, &ic);
        std::set<Zone *> zones;
        for (auto it = children.begin(); it != children.end(); ++it) {
            FacsimileInterface *fi = (*it)->GetFacsimileInterface();
            assert(fi);
            if (fi->GetZone() != nullptr)
                zones.insert(fi->GetZone());
        }
        for (auto it = zones.begin(); it != zones.end(); ++it) {
            (*it)->ShiftByXY(x, y);
        }
        return true; // Can't reorder by layer since staves contain layers
    }
    else {
        LogWarning("Unsupported element for dragging.");
        return false;
    }
    Layer *layer = dynamic_cast<Layer *>(element->GetFirstParent(LAYER));
    layer->ReorderByXPos(); // Reflect position order of elements internally (and in the resulting output file)
    return true;
}

bool EditorToolkit::Insert(std::string elementType, std::string startid, std::string endid)
{
    LogMessage("Insert!");
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
        AttComparison ac(STAFF);
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
        Staff *newStaff = new Staff();
        zone->SetUlx(ulx);
        zone->SetUly(uly);
        zone->SetLrx(lrx);
        zone->SetLry(lry);
        Surface *surface = dynamic_cast<Surface *>(m_doc->GetFacsimile()->FindChildByType(SURFACE));
        assert(surface);
        surface->AddChild(zone);
        newStaff->SetZone(zone);
        newStaff->SetFacs(zone->GetUuid());
        Layer *newlayer = new Layer();
        Object *parent = staff->GetParent();
        assert(parent);
        parent->AddChild(newStaff);
        return true;
    }

    if (elementType == "nc") {
        Syllable *syllable = new Syllable();
        Neume *neume = new Neume();
        Nc *nc = new Nc();

        // Find closest valid clef
        ArrayOfObjects clefs;
        AttComparison ac(CLEF);
        layer->FindAllChildByComparison(&clefs, &ac);
        Clef *clef = nullptr;

        for (auto it = clefs.begin(); it != clefs.end(); it++) {
            Clef *current = dynamic_cast<Clef *>(*it);
            assert(current);
            if (current->GetZone()->GetUlx() < ulx) {
                if (clef == nullptr || clef->GetZone()->GetUlx() < current->GetZone()->GetUlx())
                    clef = current;
            }
        }

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

        // Set as inclinatum or virga (if necessary)
        for (auto it = attributes.begin(); it != attributes.end(); ++it) {
            if (it->first == "diagonalright") {
                if (it->second == "u") {
                    nc->SetDiagonalright(ncVis_DIAGONALRIGHT_u);
                }
            }
            else if (it->first == "name") {
                if (it->second == "inclinatum") {
                    nc->SetName(ncVis_NAME_inclinatum);
                }
            }
        }

        const int staffSize = m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
        const int pitchDifference = round((double) (clef->GetZone()->GetUly() - uly) / (double) staffSize);
        nc->AdjustPitchByOffset(pitchDifference);

        const int noteHeight = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 2);
        const int noteWidth = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 1.4);

        // Set up facsimile
        zone->SetUlx(ulx);
        zone->SetUly(uly);
        zone->SetLrx(ulx + noteWidth);
        zone->SetLry(uly + noteHeight);
        nc->SetZone(zone);
        nc->SetFacs(zone->GetUuid());
        Surface *surface = dynamic_cast<Surface *>(facsimile->FindChildByType(SURFACE));
        assert(surface);
        surface->AddChild(zone);

        neume->AddChild(nc);
        syllable->AddChild(neume);
        layer->AddChild(syllable);
        m_editInfo = nc->GetUuid();
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
    }
    else if (elementType == "custos") {
        Custos *custos = new Custos();
        
        // Find closest valid clef
        ArrayOfObjects clefs;
        AttComparison ac(CLEF);
        layer->FindAllChildByComparison(&clefs, &ac);
        Clef *clef = nullptr;

        for (auto it = clefs.begin(); it != clefs.end(); it++) {
            Clef *current = dynamic_cast<Clef *>(*it);
            assert(current);
            if (current->GetZone()->GetUlx() < ulx) {
                if (clef == nullptr || clef->GetZone()->GetUlx() < current->GetZone()->GetUlx())
                    clef = current;
            }
        }

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

        const int staffSize = m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
        const int pitchDifference = round((double) (clef->GetZone()->GetUly() - uly) / (double) staffSize);
        custos->AdjustPitchByOffset(pitchDifference);
        const int noteHeight = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 2);
        const int noteWidth = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 1.4);
        
        zone->SetUlx(ulx);
        zone->SetUly(uly);
        zone->SetLrx(ulx + noteWidth);
        zone->SetLry(uly + noteHeight);
        custos->SetZone(zone);
        custos->SetFacs(zone->GetUuid());
        Surface *surface = dynamic_cast<Surface *>(facsimile->FindChildByType(SURFACE));
        assert(surface);
        surface->AddChild(zone);
        layer->AddChild(custos);
        m_editInfo = custos->GetUuid();
    }
    else {
        LogError("Unsupported type '%s' for insertion", elementType.c_str());
        return false;
    }
    layer->ReorderByXPos();
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

bool EditorToolkit::Remove(std::string elementId)
{
    if (!m_doc->GetDrawingPage()) {
        LogError("Could not get the drawing page.");
        return false;
    }
    Object *obj = m_doc->GetDrawingPage()->FindChildByUuid(elementId);
    assert(obj);
    Object *parent = obj->GetParent();
    assert(parent);
    m_editInfo = elementId;
    return parent->DeleteChild(obj);
}

std::string EditorToolkit::EditInfo()
{
    return m_editInfo;
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

bool EditorToolkit::ParseRemoveAction(
    jsonxx::Object param, std::string *elementId)
{
    if (!param.has<jsonxx::String>("elementId")) return false;
    (*elementId) = param.get<jsonxx::String>("elementId");
    return true;
}

#endif
// USE_EMSCRIPTEN
} // namespace vrv
