/////////////////////////////////////////////////////////////////////////////
// Name:        editortoolkit.cpp
// Author:      Laurent Pugin
// Created:     27/08/2025
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "editortoolkit.h"

//--------------------------------------------------------------------------------

#include "object.h"
#include "page.h"
#include "vrv.h"

//--------------------------------------------------------------------------------

namespace vrv {

#ifndef NO_EDIT_SUPPORT

bool EditorToolkit::AppendChild(const std::string &elementId, const std::string &elementName)
{
    Object *element = this->GetElement(elementId);
    if (!element) return false;

    Object *childElement = this->PrepareInsertion(element, elementName);
    if (!childElement) return false;

    if (!element->AddChild(childElement)) {
        delete childElement;
        return false;
    }

    return true;
}

bool EditorToolkit::InsertBefore(const std::string &elementId, const std::string &elementName)
{
    Object *element = this->GetElement(elementId);
    if (!element) return false;

    Object *parent = element->GetParent();
    assert(parent);

    Object *childElement = this->PrepareInsertion(parent, elementName);
    if (!childElement) return false;

    parent->InsertBefore(element, childElement);

    return true;
}

bool EditorToolkit::InsertAfter(const std::string &elementId, const std::string &elementName)
{
    Object *element = this->GetElement(elementId);
    if (!element) return false;

    Object *parent = element->GetParent();
    assert(parent);

    Object *childElement = this->PrepareInsertion(parent, elementName);
    if (!childElement) return false;

    parent->InsertAfter(element, childElement);

    return true;
}

Object *EditorToolkit::GetElement(const std::string &elementId)
{
    Object *element = NULL;

    // Try to get the element on the current drawing page
    if (m_doc->GetDrawingPage()) {
        element = m_doc->GetDrawingPage()->FindDescendantByID(elementId);
    }
    // If it wasn't there, try on the whole doc
    if (!element) {
        element = m_doc->FindDescendantByID(elementId);
    }

    if (!element) {
        LogError("Element '%s' could not be found", elementId.c_str());
    }

    return element;
}

Object *EditorToolkit::PrepareInsertion(Object *parent, const std::string &elementName)
{
    assert(parent);

    ClassId classId = ObjectFactory::GetInstance()->GetClassId(elementName);
    if (!parent->IsSupportedChild(classId)) {
        LogError("Element '%s' is not supported as child of '%s'", elementName.c_str(), parent->GetClassName().c_str());
        return NULL;
    }

    Object *childElement = ObjectFactory::GetInstance()->Create(elementName);
    if (!childElement) {
        LogError("Creating a '%s' object failed", elementName.c_str());
    }

    return childElement;
}

#endif /* NO_EDIT_SUPPORT */

} // namespace vrv
