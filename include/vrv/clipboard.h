/////////////////////////////////////////////////////////////////////////////
// Name:        clipboard.h
// Author:      Laurent Pugin
// Created:     2026
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CLIPBOARD_H__
#define __VRV_CLIPBOARD_H__

#include <cstdint>
#include <string>
#include <vector>

//----------------------------------------------------------------------------

#ifndef NO_EDIT_SUPPORT

namespace vrv {

class Doc;
class Measure;
class Layer;
class Object;
class System;

//----------------------------------------------------------------------------
// Clipboard
//----------------------------------------------------------------------------

/**
 * This class implements a clipboard for the editor
 */
class Clipboard {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Clipboard();
    virtual ~Clipboard();
    ///@}

    void SetDoc(Doc *doc) { m_doc = doc; }

    enum ClipboardType : int8_t { CLIPBOARD_NONE = 0, CLIPBOARD_MEASURES, CLIPBOARD_STAVES, CLIPBOARD_LAYERELEMENTS };

    ClipboardType GetClipboardType() const { return m_clipboardType; }
    void ClearClipboard();

    bool Copy(const Object *selection);
    bool Paste(Object *selection);

    bool CanCopy(const Object *selection) const;
    bool CanPaste(const Object *selection) const;

private:
    //
public:
    //
private:
    ClipboardType m_clipboardType;
    Doc *m_doc;
    System *m_clipboardMeasures;
    System *m_clipboardStaves;
    Layer *m_clipboardLayerElements;
};

} // namespace vrv

#endif /* NO_EDIT_SUPPORT */

#endif
