/////////////////////////////////////////////////////////////////////////////
// Name:        editortoolkit.h
// Author:      Laurent Pugin, Juliette Regimbal
// Created:     16/05/2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_EDITOR_TOOLKIT_H__
#define __VRV_EDITOR_TOOLKIT_H__

#include <cmath>
#include <string>
#include <utility>

//--------------------------------------------------------------------------------

#include "doc.h"
#include "view.h"

#include "jsonxx.h"

namespace vrv {

//--------------------------------------------------------------------------------
// EditorToolkit
//-------------------------------------------------------------------------------- 

class EditorToolkit {
public:
    EditorToolkit(Doc *doc, View *view) { m_doc = doc; m_view = view; m_editInfo = ""; }

#ifdef USE_EMSCRIPTEN
    bool ParseEditorAction(const std::string &json_editorAction);

    /**
     * Experimental editor functions.
     */
    ///@{
    bool Drag(std::string elementId, int x, int y);
    bool Insert(std::string elementType, std::string startId, std::string endId);
    bool Insert(std::string elementType, std::string staffId, int ulx, int uly,
        std::vector<std::pair<std::string, std::string>> attributes);
    bool Set(std::string elementId, std::string attrType, std::string attrValue);
    ///@}
    
    /**
     * Get information on the last editor function used
     */
    std::string EditInfo();

#endif

protected:

#ifdef USE_EMSCRIPTEN
    /**
     * Parse JSON instructions for experimental editor functions.
     */
    ///@{
    bool ParseDragAction(jsonxx::Object param, std::string *elementId, int *x, int *y);
    bool ParseInsertAction(jsonxx::Object param, std::string *elementType, std::string *startId, std::string *endId);
    bool ParseInsertAction(jsonxx::Object param, std::string *elementType, std::string *staffId, int *ulx, int *uly,
        std::vector<std::pair<std::string, std::string>> *attributes);
    bool ParseSetAction(jsonxx::Object param, std::string *elementId, std::string *attrType, std::string *attrValue);
    ///@}
#endif

    Doc *m_doc;
    View *m_view;
    std::string m_editInfo;
};

//--------------------------------------------------------------------------------
// ClosestBB Comparator struct
//--------------------------------------------------------------------------------
// To be used with std::sort to find the object with a closest bounding
// box to a point defined by the x and y parameters of ClosestBB

struct ClosestBB {
    int x;
    int y;
    
    int distanceToBB(int ulx, int uly, int lrx, int lry)
    {
        int xDiff = std::max(
                (ulx > x ? ulx - x : 0),
                (x > lrx ? x - lrx : 0)
        );
        int yDiff = std::max(
                (uly > y ? uly - y : 0),
                (y > lry ? y - lry : 0)
        );

        return sqrt(xDiff * xDiff + yDiff * yDiff);
    }
    
    bool operator() (Object *a, Object *b) {
        if (!a->GetFacsimileInterface() || !b->GetFacsimileInterface()) return true;
        Zone *zoneA = a->GetFacsimileInterface()->GetZone();
        Zone *zoneB = b->GetFacsimileInterface()->GetZone();

        int distA = distanceToBB(zoneA->GetUlx(), zoneA->GetUly(), zoneA->GetLrx(), zoneA->GetLry());
        int distB = distanceToBB(zoneB->GetUlx(), zoneB->GetUly(), zoneB->GetLrx(), zoneB->GetLry());
        return (distA < distB);
    }
};

} // namespace vrv

#endif
