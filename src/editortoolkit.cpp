/////////////////////////////////////////////////////////////////////////////
// Name:        editortoolkit.cpp
// Author:      Laurent Pugin, Juliette Regimbal
// Created:     16/05/2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "editortoolkit.h"

//-------------------------------------------------------------------------------- 

#include <cmath>

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

    if (!json.has<jsonxx::String>("action") || !json.has<jsonxx::Object>("param"))
        return false;

    std::string action = json.get<jsonxx::String>("action");

    if (action == "drag") {
        std::string elementId;
        int x,y;
        if (this->ParseDragAction(json.get<jsonxx::Object>("param"), &elementId, &x, &y)) {
            return this->Drag(elementId, x, y);
        }    
    }
    else if (action == "insert") {
        std::string elementType, startId, endId;
        if (this->ParseInsertAction(json.get<jsonxx::Object>("param"), &elementType, &startId, &endId)) {
            return this->Insert(elementType, startId, endId);
        }

    }
    else if (action == "set") {
        std::string elementId, attrType, attrValue;
        if (this->ParseSetAction(json.get<jsonxx::Object>("param"), &elementId, &attrType, &attrValue)) {
            return this->Set(elementId, attrType, attrValue);
        }
    }
    else {

    }
    return false;
}

std::string EditorToolkit::ParseQueryAction(const std::string &json_queryAction)
{
    jsonxx::Object json;

    // Read JSON actions
    if (!json.parse(json_queryAction)) {
        LogError("Can not parse JSON std::string.");
            return "";
    }

    if (!json.has<jsonxx::String>("action") || !json.has<jsonxx::Object>("param"))
        return "";

    std::string action = json.get<jsonxx::String>("action");

    if (action == "neume-info") {
        std::string elementId;
        if (this->ParseNeumeInfoAction(json.get<jsonxx::Object>("param"), &elementId)) {
            return this->GetNeumeInfo(elementId);
        }
    }
    return "";
}

bool EditorToolkit::Drag(std::string elementId, int x, int y)
{
    if (!m_doc->GetDrawingPage()) return false;

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
        if(!layer) return false;
        int oct;
        data_PITCHNAME pname
            = (data_PITCHNAME)m_view->CalculatePitchCode(layer, element->GetDrawingY() + y, element->GetDrawingX(), &oct);
        element->GetPitchInterface()->SetPname(pname);
        element->GetPitchInterface()->SetOct(oct);
        /*if (element->HasAttClass(ATT_COORDINATED)) {
            AttCoordinated *att = dynamic_cast<AttCoordinated *>(element);
            att->SetUlx(x);
        }*/
        return true;
    }
    if (element->Is(NEUME)) {
        Neume *neume = dynamic_cast<Neume *>(element);
        assert(neume);
        Layer *layer = dynamic_cast<Layer *>(neume->GetFirstParent(LAYER));
        if (!layer) return false;
        Staff *staff = dynamic_cast<Staff *>(layer->GetFirstParent(STAFF));
        assert(staff);
        // Calculate difference in pitch based on y difference
        int pitchDifference = round((double)y / (double)staff->m_drawingStaffSize);

        // Get components of neume
        AttComparison ac(NC);
        ArrayOfObjects objects;
        neume->FindAllChildByComparison(&objects, &ac);

        for (auto it = objects.begin(); it != objects.end(); ++it) {
            Nc *nc = dynamic_cast<Nc *>(*it);
            // Update the neume component
            nc->AdjustPitchByOffset(pitchDifference); 
            //// Temporarily removing ULX attributes for coordinate refactor
            // nc->SetUlx(nc->GetUlx() - x);
        }
        return true;
    }
    if (element->Is(CLEF)) {
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
        //// Temporarily removing ULX attributes for coordinate refactor
        // clef->SetUlx(x);

        if (initialClefLine != clefLine) {  // adjust notes so they stay in the same position
            int lineDiff = clefLine - initialClefLine;
            ArrayOfObjects objects;
            InterfaceComparison ic(INTERFACE_PITCH);

            layer->FindAllChildByComparison(&objects, &ic); 

            // Adjust all elements who are positioned relative to clef by pitch
            for (auto it = objects.begin(); it != objects.end(); ++it) {
                Object *child = dynamic_cast<Object *>(*it);
                if (child == nullptr) continue;
                PitchInterface *pi = child->GetPitchInterface();
                assert(pi);
                pi->AdjustPitchByOffset(-2 * lineDiff); // One line -> 2 pitches
            } 
        }

        return true;
    }
    return false;
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
    if (success) {
        m_doc->PrepareDrawing();
        m_doc->GetDrawingPage()->LayOut(true);
        return true;
    }
    return false;
}

std::string EditorToolkit::GetNeumeInfo(std::string elementId)
{
    if (!m_doc->GetDrawingPage()) return "";
    Object *element = m_doc->GetDrawingPage()->FindChildByUuid(elementId);
    Neume *neume = dynamic_cast<Neume *>(element);
    assert(neume);

    std::string neumeInfo = "";
    ArrayOfObjects children;
    AttComparison ac(NC);
    neume->FindAllChildByComparison(&children, &ac);

    auto it = children.begin();
    Nc *previous = dynamic_cast<Nc *>(*it);
    assert(previous);
    it++;

    for (; it != children.end(); it++) {
        Nc *current = dynamic_cast<Nc *>(*it);
        assert(current);
        int pitchDifference = current->PitchDifferenceTo(previous);
        if (pitchDifference > 0)
        {
            neumeInfo += ".u" + std::to_string(pitchDifference);
        }
        else if (pitchDifference < 0)
        {
            neumeInfo += ".d" + std::to_string(pitchDifference * -1);
        }
        else
        {
            neumeInfo += ".s";
        }
        previous = current;
    }
    neumeInfo = Neume::NeumeGroupToString(neume->GetNeumeGroup()) + neumeInfo;
    return neumeInfo;
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

bool EditorToolkit::ParseSetAction(
    jsonxx::Object param, std::string *elementId, std::string *attrType, std::string *attrValue)
{
    if (!param.has<jsonxx::String>("elementId")) return false;
    (*elementId) = param.get<jsonxx::String>("elementId");
    if (!param.has<jsonxx::String>("attrType")) return false;
    (*attrType) = param.get<jsonxx::String>("attrType");
    if (!param.has<jsonxx::String>("attrValue")) return false;
    (*attrValue) = param.get<jsonxx::String>("attrValue");
    return true;
}

bool EditorToolkit::ParseNeumeInfoAction(
    jsonxx::Object param, std::string *elementId)
{
    if (!param.has<jsonxx::String>("elementId")) return false;
    (*elementId) = param.get<jsonxx::String>("elementId");
    return true;
}
#endif
// USE_EMSCRIPTEN
} // namespace vrv
