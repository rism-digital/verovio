/////////////////////////////////////////////////////////////////////////////
// Name:        docselection.h
// Author:      Laurent Pugin
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_DOC_SELECTION_H__
#define __VRV_DOC_SELECTION_H__

#include <string>

namespace vrv {

class Doc;

//----------------------------------------------------------------------------
// DocSelection
//----------------------------------------------------------------------------

/**
 * This class stores a document selection
 */
class DocSelection {

public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    DocSelection();
    virtual ~DocSelection() {}
    ///@}

    /**
     * Parse the stringified JSON string a stores the values.
     * Priority is given to measureRange selection over start / end ids
     */
    bool Parse(const std::string &selection);

    /**
     * Set the selection to the Doc.
     * Find measure IDs for measureRange selection.
     */
    void Set(Doc *doc);

private:
    //
public:
    std::string m_selectionStart;
    std::string m_selectionEnd;
    int m_selectionRangeStart;
    int m_selectionRangeEnd;
    std::string m_measureRange;
    bool m_isPending;
};

} // namespace vrv

#endif
