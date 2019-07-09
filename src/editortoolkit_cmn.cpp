/////////////////////////////////////////////////////////////////////////////
// Name:        editortoolkit_cmn.cpp
// Author:      Laurent Pugin, Juliette Regimbal, Zoe McLennan
// Created:     04/06/2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "editortoolkit_cmn.h"

//--------------------------------------------------------------------------------

#include <locale>
#include <codecvt>
#include <set>

//--------------------------------------------------------------------------------

#include "clef.h"
#include "comparison.h"
#include "hairpin.h"
#include "layer.h"
#include "measure.h"
#include "page.h"
#include "rend.h"
#include "slur.h"
#include "staff.h"
#include "vrv.h"

//--------------------------------------------------------------------------------

#include "jsonxx.h"

//--------------------------------------------------------------------------------

#define CHAINED_ID "[chained-id]"

namespace vrv {

bool EditorToolkitCMN::ParseEditorAction(const std::string &json_editorAction, bool isChain)
{
    jsonxx::Object json;

    // Read JSON actions
    if (!json.parse(json_editorAction)) {
        LogError("Can not parse JSON std::string.");
        return false;
    }

    if (!json.has<jsonxx::String>("action") ||
            (!json.has<jsonxx::Object>("param") && !json.has<jsonxx::Array>("param"))) {
        LogWarning("Incorrectly formatted JSON action.");
    }

    std::string action = json.get<jsonxx::String>("action");

    if (action == "chain") {
        if (!json.has<jsonxx::Array>("param")) {
            LogError("Incorrectly formatted JSON action");
            return false;
        }
        return this->Chain(json.get<jsonxx::Array>("param"));
    }
    else if (action == "drag") {
        std::string elementId;
        int x,y;
        if (this->ParseDragAction(json.get<jsonxx::Object>("param"), elementId, x, y)) {
            return this->Drag(elementId, x, y);
        }
        LogWarning("Could not parse the drag action");
    }
    else if (action == "insert") {
        std::string elementType, startId, endId;
        if (this->ParseInsertAction(json.get<jsonxx::Object>("param"), elementType, startId, endId)) {
            return this->Insert(elementType, startId, endId);
        }
        LogWarning("Could not parse the insert action");
    }
    else if (action == "set") {
        std::string elementId, attrType, attrValue;
        if (this->ParseSetAction(json.get<jsonxx::Object>("param"), elementId, attrType, attrValue)) {
            return this->Set(elementId, attrType, attrValue);
        }
        LogWarning("Could not parse the set action");
    }
    else {
        LogWarning("Unknown action type '%s'.", action.c_str());
    }
    return false;
}

bool EditorToolkitCMN::ParseDragAction(jsonxx::Object param, std::string &elementId, int &x, int &y)
{
    if (!param.has<jsonxx::String>("elementId")) return false;
    elementId = param.get<jsonxx::String>("elementId");
    if (!param.has<jsonxx::Number>("x")) return false;
    x = param.get<jsonxx::Number>("x");
    if (!param.has<jsonxx::Number>("y")) return false;
    y = param.get<jsonxx::Number>("y");
    return true;
}

bool EditorToolkitCMN::ParseInsertAction(
    jsonxx::Object param, std::string &elementType, std::string &startid, std::string &endid)
{
    // assign optional member
    endid = "";
    
    if (!param.has<jsonxx::String>("elementType")) return false;
    elementType = param.get<jsonxx::String>("elementType");
    if (!param.has<jsonxx::String>("startid")) return false;
    startid = param.get<jsonxx::String>("startid");
    // optional
    if (param.has<jsonxx::String>("endid")) {
        endid = param.get<jsonxx::String>("endid");
    }
    return true;
}

bool EditorToolkitCMN::ParseSetAction(
    jsonxx::Object param, std::string &elementId, std::string &attrType, std::string &attrValue)
{
    if (!param.has<jsonxx::String>("elementId")) return false;
    elementId = param.get<jsonxx::String>("elementId");
    if (!param.has<jsonxx::String>("attrType")) return false;
    attrType = param.get<jsonxx::String>("attrType");
    if (!param.has<jsonxx::String>("attrValue")) return false;
    attrValue = param.get<jsonxx::String>("attrValue");
    return true;
}
    
bool EditorToolkitCMN::Chain(jsonxx::Array actions)
{
    bool status = true;
    m_chainedId = "";
    for (int i = 0; i < actions.size(); i++) {
        if (status) status = this->ParseEditorAction(actions.get<jsonxx::Object>(i).json());
        m_editInfo = m_chainedId;
    }
    return status;
}

bool EditorToolkitCMN::Drag(std::string elementId, int x, int y)
{
    if (elementId == CHAINED_ID) {
        elementId = this->m_chainedId;
    }
    else {
        this->m_chainedId = elementId;
    }
    
    if (!m_doc->GetDrawingPage()) return false;

    // Try to get the element on the current drawing page
    Object *element = m_doc->GetDrawingPage()->FindChildByUuid(elementId);

    // If it wasn't there, try on the whole doc
    if (!element) {
        element = m_doc->FindChildByUuid(elementId);
    }
    if (!element) {
        return false;
    }
    // For elements whose y-position corresponds to a certain pitch
    if (element->HasInterface(INTERFACE_PITCH)) {
        Layer *layer = dynamic_cast<Layer *>(element->GetFirstParent(LAYER));
        if (!layer) return false;
        int oct;
        data_PITCHNAME pname
            = (data_PITCHNAME)m_view->CalculatePitchCode(layer, m_view->ToLogicalY(y), element->GetDrawingX(), &oct);
        element->GetPitchInterface()->SetPname(pname);
        element->GetPitchInterface()->SetOct(oct);
        
        return true;
    }
    return false;
}

bool EditorToolkitCMN::Insert(std::string elementType, std::string startid, std::string endid)
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
    
    ControlElement *element = NULL;
    if (elementType == "slur") {
        element = new Slur();
    }
    else if (elementType == "hairpin") {
        element = new Hairpin();
    }
    else {
        LogMessage("Inserting control event '%s' is not supported", elementType.c_str());
        return false;
    }
    
    assert(element);
    TimeSpanningInterface *interface = element->GetTimeSpanningInterface();
    assert(interface);
    measure->AddChild(element);
    interface->SetStartid(startid);
    interface->SetEndid(endid);
    m_doc->PrepareDrawing();
    
    this->m_chainedId = element->GetUuid();
    
    return true;
}

bool EditorToolkitCMN::Set(std::string elementId, std::string attrType, std::string attrValue)
{
    if (elementId == CHAINED_ID) {
        elementId = this->m_chainedId;
    }
    else {
        this->m_chainedId = elementId;
    }
    
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
    else if (Att::SetFacsimile(element, attrType, attrValue))
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
    if (success) {
        m_doc->PrepareDrawing();
        //m_doc->GetDrawingPage()->LayOut(true);
        return true;
    }
    return false;
}

}// namespace vrv
