/////////////////////////////////////////////////////////////////////////////
// Name:        clipboard.h
// Author:      Laurent Pugin
// Created:     2026
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CLIPBOARD_H__
#define __VRV_CLIPBOARD_H__

#include <vector>
#include <cstdint>
#include <string>

//----------------------------------------------------------------------------

namespace vrv {

class Layer;
class Measure;
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
    
    enum ClipboardType : int8_t { CLIPBOARD_NONE = 0, CLIPBOARD_MEASURES, CLIPBOARD_STAVES, CLIPBOARD_LAYERELEMENTS };
    
    ClipboardType GetClipboardType() const { return m_clipboardType; }
    void ClearClipboard();
    
private:
    //
public:
    //
private:
    ClipboardType m_clipboardType;
    System *m_clipboardSystem;
    std::vector<Measure *> *m_clipboardMeasures;
    Layer *m_clipboardLayer;
};

} // namespace vrv

#endif
