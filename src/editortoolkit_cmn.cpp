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

bool EditorToolkitCMN::ParseEditorAction(const std::string &json_editorAction)
{
    jsonxx::Object json;

    // Read JSON actions
    if (!json.parse(json_editorAction)) {
        LogError("Can not parse JSON std::string.");
        return false;
    }


    if (!json.has<jsonxx::String>("action")) {
        LogWarning("Incorrectly formatted JSON action.");
    }

    std::string action = json.get<jsonxx::String>("action");

    // Action without parameter
    if (action == "commit") {
        m_doc->PrepareDrawing();
        return true;
    }

    if (!json.has<jsonxx::Object>("param") && !json.has<jsonxx::Array>("param")) {
        LogWarning("Incorrectly formatted JSON param.");
    }

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
    else if (action == "keyDown") {
        std::string elementId;
        int key;
        bool shiftKey, ctrlKey;
        if (this->ParseKeyDownAction(json.get<jsonxx::Object>("param"), elementId, key, shiftKey, ctrlKey)) {
            return this->KeyDown(elementId, key, shiftKey, ctrlKey);
        }
        LogWarning("Could not parse the keyDown action");
    }
    else if (action == "insert") {
        std::string elementType, startid, endid;
        if (this->ParseInsertAction(json.get<jsonxx::Object>("param"), elementType, startid, endid)) {
            return this->Insert(elementType, startid, endid);
        }
        LogWarning("Could not parse the insert action");
    }
    else if (action == "set") {
        std::string elementId, attribute, value;
        if (this->ParseSetAction(json.get<jsonxx::Object>("param"), elementId, attribute, value)) {
            return this->Set(elementId, attribute, value);
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

bool EditorToolkitCMN::ParseKeyDownAction(
    jsonxx::Object param, std::string &elementId, int &key, bool &shiftKey, bool &ctrlKey)
{
    // assign optional member
    shiftKey = false;
    ctrlKey = false;

    if (!param.has<jsonxx::String>("elementId")) return false;
    elementId = param.get<jsonxx::String>("elementId");
    if (!param.has<jsonxx::Number>("key")) return false;
    key = param.get<jsonxx::Number>("key");
    // optional
    if (param.has<jsonxx::Boolean>("shiftKey")) {
        shiftKey = param.get<jsonxx::Boolean>("shiftKey");
    }
    if (param.has<jsonxx::Boolean>("ctrlKey")) {
        ctrlKey = param.get<jsonxx::Boolean>("ctrlKey");
    }
    return true;
}

bool EditorToolkitCMN::ParseSetAction(
    jsonxx::Object param, std::string &elementId, std::string &attribute, std::string &value)
{
    if (!param.has<jsonxx::String>("elementId")) return false;
    elementId = param.get<jsonxx::String>("elementId");
    if (!param.has<jsonxx::String>("attribute")) return false;
    attribute = param.get<jsonxx::String>("attribute");
    if (!param.has<jsonxx::String>("value")) return false;
    value = param.get<jsonxx::String>("value");
    return true;
}

bool EditorToolkitCMN::Chain(jsonxx::Array actions)
{
    bool status = true;
    m_chainedId = "";
    for (int i = 0; i < (int)actions.size(); i++) {
        if (status) status = this->ParseEditorAction(actions.get<jsonxx::Object>(i).json());
        m_editInfo = m_chainedId;
    }
    return status;
}

bool EditorToolkitCMN::Drag(std::string elementId, int x, int y)
{
    Object *element = this->GetElement(elementId);
    if (!element) return false;

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

bool EditorToolkitCMN::KeyDown(std::string elementId, int key, bool shiftKey, bool ctrlKey)
{
    Object *element = this->GetElement(elementId);
    if (!element) return false;

    // For elements whose y-position corresponds to a certain pitch
    if (element->HasInterface(INTERFACE_PITCH)) {
        PitchInterface *interface = element->GetPitchInterface();
        assert(interface);
        int step;
        switch (key) {
            case KEY_UP: step = 1; break;
            case KEY_DOWN: step = -1; break;
            default: step = 0;
        }
        interface->AdjustPitchByOffset(step);
        return true;
    }
    return false;
}

bool EditorToolkitCMN::Insert(std::string elementType, std::string startid, std::string endid)
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

    this->m_chainedId = element->GetUuid();

    return true;
}

bool EditorToolkitCMN::Set(std::string elementId, std::string attribute, std::string value)
{
    Object *element = this->GetElement(elementId);
    if (!element) return false;

    bool success = false;
    if (Att::SetAnalytical(element, attribute, value))
        success = true;
    else if (Att::SetCmn(element, attribute, value))
        success = true;
    else if (Att::SetCmnornaments(element, attribute, value))
        success = true;
    else if (Att::SetCritapp(element, attribute, value))
        success = true;
    else if (Att::SetExternalsymbols(element, attribute, value))
        success = true;
    else if (Att::SetFacsimile(element, attribute, value))
        success = true;
    else if (Att::SetGestural(element, attribute, value))
        success = true;
    else if (Att::SetMei(element, attribute, value))
        success = true;
    else if (Att::SetMensural(element, attribute, value))
        success = true;
    else if (Att::SetMidi(element, attribute, value))
        success = true;
    else if (Att::SetNeumes(element, attribute, value))
        success = true;
    else if (Att::SetPagebased(element, attribute, value))
        success = true;
    else if (Att::SetShared(element, attribute, value))
        success = true;
    else if (Att::SetVisual(element, attribute, value))
        success = true;
    if (success) {
        return true;
    }
    return false;
}

Object *EditorToolkitCMN::GetElement(std::string &elementId)
{
    if (elementId == CHAINED_ID) {
        elementId = this->m_chainedId;
    }
    else {
        this->m_chainedId = elementId;
    }

    if (!m_doc->GetDrawingPage()) return NULL;

    // Try to get the element on the current drawing page
    Object *element = m_doc->GetDrawingPage()->FindChildByUuid(elementId);
    // If it wasn't there, try on the whole doc
    if (!element) {
        element = m_doc->FindChildByUuid(elementId);
    }
    return element;
}

}// namespace vrv
