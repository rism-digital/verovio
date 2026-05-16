/////////////////////////////////////////////////////////////////////////////
// Name:        editortoolkit_neume.h
// Author:      Juliette Regimbal
// Created:     04/06/2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_EDITOR_TOOLKIT_NEUME_H__
#define __VRV_EDITOR_TOOLKIT_NEUME_H__

#include <cmath>
#include <string>
#include <utility>

//--------------------------------------------------------------------------------

#include "doc.h"
#include "editortoolkit.h"
#include "measure.h"
#include "staff.h"
#include "view.h"
#include "vrv.h"
#include "zone.h"

#include "jsonxx.h"

namespace vrv {

//--------------------------------------------------------------------------------
// EditorToolkitNeume
//--------------------------------------------------------------------------------

class EditorToolkitNeume : public EditorToolkit {
public:
    EditorToolkitNeume(Doc *doc, View *view) : EditorToolkit(doc, view) {}
    bool ParseEditorAction(const std::string &json_editorAction) override;
    std::string EditInfo() override;

    /**
     * Experimental editor functions.
     */
    ///@{
    bool AddSyl(std::string elementId, std::string sylText);
    bool Chain(jsonxx::Array actions);
    bool DisplaceClefOctave(std::string elementId, std::string direction);
    bool Drag(std::string elementId, int x, int y, bool topLevel = true);
    bool Insert(std::string elementType, std::string staffId, int ulx, int uly, int lrx, int lry,
        std::vector<std::pair<std::string, std::string>> attributes);
    bool InsertToSyllable(std::string elementId);
    bool MatchHeight(std::string elementId);
    bool Merge(std::vector<std::string> elementIds);
    bool MoveOutsideSyllable(std::string elementId);
    bool Set(std::string elementId, std::string attrType, std::string attrValue);
    bool SetText(std::string elementId, const std::string &text);
    bool SetClef(std::string elementId, std::string shape);
    bool SetLiquescent(std::string elementId, std::string shape);
    bool SetAquitanianElement(std::string elementId, std::string shape);
    bool SortStaves();
    bool Split(std::string elementId, int x);
    bool SplitNeume(std::string elementId, std::string ncId);
    bool Remove(std::string elementId);
    bool Resize(std::string elementId, int ulx, int uly, int lrx, int lry, float resize = NAN);
    bool Group(std::string groupType, std::vector<std::string> elementIds);
    void UnlinkSyllable(Syllable *syllable);
    bool Ungroup(std::string groupType, std::vector<std::string> elementIds);
    bool ChangeGroup(std::string elementId, std::string contour);
    bool ToggleLigature(std::vector<std::string> elementIds);
    bool ChangeStaff(std::string elementId);
    bool ChangeStaffTo(std::string elementId, std::string staffId);
    bool ClefMovementHandler(Clef *clef, int x, int y);
    ///@}
protected:
    /**
     * Parse JSON instructions for experimental editor functions.
     */
    ///@{
    bool ParseAddSylAction(jsonxx::Object param, std::string *elementId, std::string *sylText);
    bool ParseDisplaceClefAction(jsonxx::Object param, std::string *elementId, std::string *direction);
    bool ParseDragAction(jsonxx::Object param, std::string *elementId, int *x, int *y);
    bool ParseInsertAction(jsonxx::Object param, std::string *elementType, std::string *startId, std::string *endId);
    bool ParseInsertAction(jsonxx::Object param, std::string *elementType, std::string *staffId, int *ulx, int *uly,
        int *lrx, int *lry, std::vector<std::pair<std::string, std::string>> *attributes);
    bool ParseInsertToSyllableAction(jsonxx::Object param, std::string *elementId);
    bool ParseMatchHeightAction(jsonxx::Object param, std::string *elementId);
    bool ParseMergeAction(jsonxx::Object param, std::vector<std::string> *elementIds);
    bool ParseMoveOutsideSyllableAction(jsonxx::Object param, std::string *elementId);
    bool ParseSetAction(jsonxx::Object param, std::string *elementId, std::string *attrType, std::string *attrValue);
    bool ParseSetTextAction(jsonxx::Object param, std::string *elementId, std::string *text);
    bool ParseSetClefAction(jsonxx::Object param, std::string *elementId, std::string *shape);
    bool ParseSetLiquescentAction(jsonxx::Object param, std::string *elementId, std::string *shape);
    bool ParseSetAquitanianElementAction(jsonxx::Object param, std::string *elementId, std::string *shape);
    bool ParseSplitAction(jsonxx::Object param, std::string *elementId, int *x);
    bool ParseSplitNeumeAction(jsonxx::Object param, std::string *elementId, std::string *ncId);
    bool ParseRemoveAction(jsonxx::Object param, std::string *elementId);
    bool ParseResizeAction(jsonxx::Object param, std::string *elementId, int *ulx, int *uly, int *lrx, int *lry);
    bool ParseResizeRotateAction(
        jsonxx::Object param, std::string *elementId, int *ulx, int *uly, int *lrx, int *lry, float *rotate);
    bool ParseGroupAction(jsonxx::Object param, std::string *groupType, std::vector<std::string> *elementIds);
    bool ParseUngroupAction(jsonxx::Object param, std::string *groupType, std::vector<std::string> *elementIds);
    bool ParseChangeGroupAction(jsonxx::Object param, std::string *elementId, std::string *contour);
    bool ParseToggleLigatureAction(jsonxx::Object param, std::vector<std::string> *elementIds);
    bool ParseChangeStaffAction(jsonxx::Object param, std::string *elementId);
    bool ParseChangeStaffToAction(jsonxx::Object param, std::string *elementId, std::string *staffId);
    ///@}

    /**
     * Helper functions for editor actions.
     */
    ///@{
    bool AdjustPitchAfterDrag(Object *obj, int y = 0);
    bool AdjustPitchFromPosition(Object *obj);
    bool AdjustClefLineFromPosition(Clef *clef, Staff *staff = NULL);
    ///@}
};

//--------------------------------------------------------------------------------
// Comparator structs
//--------------------------------------------------------------------------------
// To be used with std::sort to find the object with a closest bounding
// box to a point defined by the x and y parameters of ClosestBB

struct ClosestBB {
    int x;
    int y;

    int distanceToBB(int ulx, int uly, int lrx, int lry, double rotate = 0)
    {
        int offset = (x - ulx) * tan(rotate * M_PI / 180.0);
        uly = uly + offset;
        lry = lry + offset;
        int xDiff = std::max((ulx > x ? ulx - x : 0), (x > lrx ? x - lrx : 0));
        int yDiff = std::max((uly > y ? uly - y : 0), (y > lry ? y - lry : 0));

        return sqrt(xDiff * xDiff + yDiff * yDiff);
    }

    bool operator()(Object *a, Object *b)
    {
        if (!a->GetFacsimileInterface() || !b->GetFacsimileInterface()) return true;
        Zone *zoneA = a->GetFacsimileInterface()->GetZone();
        Zone *zoneB = b->GetFacsimileInterface()->GetZone();

        int distA = distanceToBB(zoneA->GetUlx(), zoneA->GetUly(), zoneA->GetLrx(), zoneA->GetLry(),
            zoneA->HasRotate() ? zoneA->GetRotate() : 0);
        int distB = distanceToBB(zoneB->GetUlx(), zoneB->GetUly(), zoneB->GetLrx(), zoneB->GetLry(),
            zoneB->HasRotate() ? zoneB->GetRotate() : 0);
        return (distA < distB);
    }
};

// To be used with std::stable_sort to find the position to insert a new accid / divLine
// TODO: use closesBB instead
struct ClosestNeume {
    int x;
    int y;

    bool operator()(Object *a, Object *b)
    {
        // check if neume has neume components
        if (!a->GetFirst(NC)) {
            LogError("Neume %s doesn't have neume components.", a->GetID().c_str());
            return true;
        }
        if (!b->GetFirst(NC)) {
            LogError("Neume %s doesn't have neume components.", b->GetID().c_str());
            return true;
        }
        if (!a->GetFirst(NC)->GetFacsimileInterface()) {
            LogError("Neume component %s doesn't have facsimile.", a->GetFirst(NC)->GetID().c_str());
            return true;
        }
        if (!b->GetFirst(NC)->GetFacsimileInterface()) {
            LogError("Neume component %s doesn't have facsimile.", b->GetFirst(NC)->GetID().c_str());
            return true;
        }
        Zone *zoneA = a->GetFirst(NC)->GetFacsimileInterface()->GetZone();
        Zone *zoneB = b->GetFirst(NC)->GetFacsimileInterface()->GetZone();

        int distA = std::abs(x - zoneA->GetUlx());
        int distB = std::abs(x - zoneB->GetUlx());

        return (distA < distB);
    }
};

// To be used with std::stable_sort to find the position to insert a new staff

struct StaffSort {
    // Update 2024-04:
    // Used only in neume lines,
    // System->(Measure->Staff)
    // Need to sort Measure to sort staff

    // Sort staves by:
    // 1. Column number derived from staff@type (column1, column2, etc)
    // 2. Within each column: left-to-right and top-to-bottom
    bool operator()(Object *a, Object *b)
    {
        if (!a->Is(SYSTEM) || !b->Is(SYSTEM)) return false;
        if (!a->FindDescendantByType(MEASURE) || !b->FindDescendantByType(MEASURE)) return false;

        Measure *measureA = dynamic_cast<Measure *>(a->FindDescendantByType(MEASURE));
        Measure *measureB = dynamic_cast<Measure *>(b->FindDescendantByType(MEASURE));
        if (!measureA->IsNeumeLine() || !measureB->IsNeumeLine()) return true;

        Staff *staffA = dynamic_cast<Staff *>(a->FindDescendantByType(STAFF));
        Staff *staffB = dynamic_cast<Staff *>(b->FindDescendantByType(STAFF));
        assert(staffA);
        assert(staffB);

        if (staffA->HasType() && staffB->HasType()) {
            // First compare column numbers from staff@type
            std::string typeA = staffA->GetType();
            std::string typeB = staffB->GetType();

            if (staffA->GetType().find("column") == 0 && staffB->GetType().find("column") == 0) {
                int columnA = std::stoi(typeA.substr(6));
                int columnB = std::stoi(typeB.substr(6));
                if (columnA != columnB) {
                    return columnA < columnB;
                }
            }
        }

        // If in same column, use position-based sorting logic
        Zone *zoneA = staffA->GetFacsimileInterface()->GetZone();
        Zone *zoneB = staffB->GetFacsimileInterface()->GetZone();
        assert(zoneA);
        assert(zoneB);

        int aLowest, bLowest, aHighest, bHighest;

        aLowest = zoneA->GetRotate() < 0
            ? zoneA->GetLry()
            : zoneA->GetLry() + (zoneA->GetLrx() - zoneA->GetUlx()) * tan(zoneA->GetRotate() * M_PI / 180.0);

        aHighest = zoneA->GetRotate() < 0
            ? zoneA->GetUly()
            : zoneA->GetUly() - (zoneA->GetLrx() - zoneA->GetUlx()) * tan(zoneA->GetRotate() * M_PI / 180.0);

        bLowest = zoneB->GetRotate() < 0
            ? zoneB->GetLry()
            : zoneB->GetLry() + (zoneB->GetLrx() - zoneB->GetUlx()) * tan(zoneB->GetRotate() * M_PI / 180.0);

        bHighest = zoneB->GetRotate() < 0
            ? zoneB->GetUly()
            : zoneB->GetUly() - (zoneB->GetLrx() - zoneB->GetUlx()) * tan(zoneB->GetRotate() * M_PI / 180.0);

        // Check for y intersection
        // if the x intersection part is smaller than half of length of staffA
        // sort by x coordinate
        if (((aLowest <= bLowest && aLowest >= bHighest) || (aHighest <= bLowest && aHighest >= bHighest)
                || (bLowest <= aLowest && bLowest >= aHighest) || (bHighest <= aLowest && bHighest >= aHighest))
            && (zoneA->GetLrx() - zoneB->GetUlx() <= 0.5 * (zoneA->GetLrx() - zoneA->GetUlx()))) {
            // sort by x center
            return (zoneA->GetUlx() < zoneB->GetUlx());
        }
        else { // no intersection
            return (zoneA->GetUly() < zoneB->GetUly());
        }
    }
};

} // namespace vrv

#endif
