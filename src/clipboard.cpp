/////////////////////////////////////////////////////////////////////////////
// Name:        clipboard.cpp
// Author:      Laurent Pugin
// Created:     2026
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "clipboard.h"
//----------------------------------------------------------------------------

#include <cassert>
#include <math.h>

//----------------------------------------------------------------------------

#include "doc.h"
#include "layer.h"
#include "measure.h"
#include "object.h"
#include "system.h"
#include "vrv.h"

#ifndef NO_EDIT_SUPPORT

namespace vrv {

//----------------------------------------------------------------------------
// Clipboard
//----------------------------------------------------------------------------

Clipboard::Clipboard()
{
    m_clipboardType = CLIPBOARD_NONE;
    m_clipboardMeasures = NULL;
    m_clipboardStaves = NULL;
    m_clipboardLayerElements = NULL;
    m_doc = NULL;
}

Clipboard::~Clipboard()
{
    this->ClearClipboard();
}

void Clipboard::ClearClipboard()
{
    if (m_clipboardMeasures) {
        delete m_clipboardMeasures;
        m_clipboardMeasures = NULL;
    }
    if (m_clipboardStaves) {
        delete m_clipboardStaves;
        m_clipboardStaves = NULL;
    }
    if (m_clipboardLayerElements) {
        delete m_clipboardLayerElements;
        m_clipboardLayerElements = NULL;
    }
    m_clipboardType = CLIPBOARD_NONE;
}

bool Clipboard::Copy(const Object *selection)
{
    assert(m_doc);
    assert(selection);

    this->ClearClipboard();

    if (selection->Is(STAFF)) {
        Object *copy = selection->Clone();
        copy->CloneReset();

        m_clipboardStaves = new System();
        Measure *measure = new Measure();
        m_clipboardStaves->AddChild(measure);
        measure->AddChildBack(copy);
        m_clipboardType = CLIPBOARD_STAVES;
    }

    return true;
}

bool Clipboard::Paste(Object *selection)
{
    assert(m_doc);
    assert(selection);
    assert(selection->GetParent());

    if (!this->CanPaste(selection)) return false;

    if (selection->Is(STAFF)) {
        Object *clipboard = m_clipboardStaves->FindDescendantByType(STAFF);
        if (!clipboard) return false;
        Object *copy = clipboard->Clone();
        copy->CloneReset();
        selection->CopyAttributesTo(copy);

        selection->GetParent()->ReplaceChild(selection, copy);
        delete selection;
        selection = copy;
    }

    return true;
}

bool Clipboard::CanCopy(const Object *selection) const
{
    assert(m_doc);
    assert(selection);

    return (selection->Is(STAFF));
}

bool Clipboard::CanPaste(const Object *selection) const
{
    assert(m_doc);
    assert(selection);

    return (selection->Is(STAFF) && (m_clipboardType == CLIPBOARD_STAVES));
}

} // namespace vrv

#endif /* NO_EDIT_SUPPORT */
