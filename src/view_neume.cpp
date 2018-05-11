/////////////////////////////////////////////////////////////////////////////
// Name:        view_syllable.cpp
// Author:      Andrew Tran
// Created:     2017
// Copyright (c) Author and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "view.h"

//----------------------------------------------------------------------------

#include <algorithm>
#include <assert.h>
#include <iostream>
#include <math.h>

//----------------------------------------------------------------------------

#include "syllable.h"
#include "devicecontext.h"
#include "doc.h"
#include "layer.h"
#include "layerelement.h"
#include "nc.h"
#include "note.h"
#include "neume.h"
#include "rpt.h"
#include "smufl.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

void View::DrawSyllable(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(layer);
    assert(staff);
    assert(measure);

    Syllable *syllable = dynamic_cast<Syllable *>(element);
    assert(syllable);
    
    /******************************************************************/
    // Start the Beam graphic and draw the children

    dc->StartGraphic(element, "", element->GetUuid());

    /******************************************************************/
    // Draw the children

    DrawLayerChildren(dc, syllable, layer, staff, measure);

    dc->EndGraphic(element, this);
}

void View::DrawNeume(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(layer);
    assert(staff);
    assert(measure);

    Neume *neume = dynamic_cast<Neume *>(element);
    assert(neume);



    /******************************************************************/
    // initialization
    std::map<std::string, NeumeGroup> *neumeGroups = Neume::GetGroups();
    

    // Obatining list of NC to extract intm data
    // Format of *neumechildren, first object is <neume>, all objects after are <nc>
    const ListOfObjects neumeChildren = neume->GetList(neume)[0];
    if (neumeChildren.empty()) {
        return;
    }

    // Loading the contour of the neume shape from the nc children
    std::vector<Nc *> ncVector;
    std::string contour = "";
    for (int i = 1; i < (int)neumeChildren.size(); i++) {
         Nc *nc = dynamic_cast<Nc *>(neumeChildren[i]);
         ncVector.push_back(nc);
         if (i >= 2) {
             contour.append(nc->GetIntm());
         }
    }

    // Obtaining the NeumeGroup enum name from the map 
    NeumeGroup neumeName = (*neumeGroups)[contour];

    // If the shape cannot be found in the map, NeumeGroup::ERROR will be returned since its
    // value is 0 and std::map returns 0 by default for missing keys.
    if (neumeName == NEUME_ERROR) {
        // TODO: Error Handling for unfound neumes
        std::cout << "Neume Grouping not found" << std::endl;
    }
    else {
        std::cout << neumeName << std::endl;
    }

    /******************************************************************/
    // Start the Neume graphic and draw the children

    dc->StartGraphic(element, "", element->GetUuid());
    // If there is a defined way to draw the neume grouping, we should use that to draw the specialized SVG shape,
    // or else we draw each nc in the neume as a punctum.
    switch (neumeName) {
        case PORRECTUS:
        {   
            // TODO: Check for octave switches
            int pitchDifference = ncVector[1]->GetPname() - ncVector[0]->GetPname();
            int octaveDifference = ncVector[1]->GetOct() - ncVector[0]->GetOct();
            int noteSize = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 2);
            wchar_t lineCode;
            wchar_t ligatureCode;
            pitchDifference += octaveDifference * 7;
            switch (pitchDifference)    
            {
                case -1: 
                    lineCode = SMUFL_E9B4_chantEntryLineAsc2nd;
                    ligatureCode = SMUFL_E9B9_chantLigaturaDesc2nd;
                    break;
                case -2:
                    lineCode = SMUFL_E9B5_chantEntryLineAsc3rd;
                    ligatureCode = SMUFL_E9BA_chantLigaturaDesc3rd;
                    break;
                case -3:
                    lineCode = SMUFL_E9B6_chantEntryLineAsc4th;
                    ligatureCode = SMUFL_E9BB_chantLigaturaDesc4th;
                    break;
                case -4:
                    lineCode = SMUFL_E9B7_chantEntryLineAsc5th;
                    ligatureCode = SMUFL_E9BC_chantLigaturaDesc5th;
                    break;
                default:
                    // TODO: Make default draw all 3 punctums individually
                    lineCode = SMUFL_E9B4_chantEntryLineAsc2nd;
                    ligatureCode = SMUFL_E9B9_chantLigaturaDesc2nd;
                    break;
            }
            DrawNc(dc, dynamic_cast<LayerElement *>(ncVector[0]), layer, staff, measure, lineCode, 0, noteSize * pitchDifference);
            DrawNc(dc, dynamic_cast<LayerElement *>(ncVector[0]), layer, staff, measure, ligatureCode);
            DrawNc(dc, dynamic_cast<LayerElement *>(ncVector[2]), layer, staff, measure);
            break;
        }
        case CLIVIS:
        {
            int pitchDifference = ncVector[1]->GetPname() - ncVector[0]->GetPname();
            int noteSize = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 2);
            pitchDifference += (ncVector[1]->GetOct() - ncVector[0]->GetOct()) * 7;
            wchar_t lineCode;
            switch (pitchDifference)
            {
                case -1:
                    lineCode = SMUFL_E9BD_chantConnectingLineAsc2nd;
                    break;
                case -2:
                    lineCode = SMUFL_E9BE_chantConnectingLineAsc3rd;
                    break;
                case -3:
                    lineCode = SMUFL_E9BF_chantConnectingLineAsc4th;
                    break;
                case -4:
                    lineCode = SMUFL_E9C0_chantConnectingLineAsc5th;
                    break;
                default:
                    // TODO: Draw each punctum separately
                    lineCode = SMUFL_E9BD_chantConnectingLineAsc2nd;
                    break;
            }
            DrawNc(dc, dynamic_cast<LayerElement *>(ncVector[0]), layer, staff, measure, SMUFL_E990_chantPunctum, noteSize / 4, 0);
            DrawNc(dc, dynamic_cast<LayerElement *>(ncVector[1]), layer, staff, measure, lineCode, 0, 0);
            DrawNc(dc, dynamic_cast<LayerElement *>(ncVector[1]), layer, staff, measure);
            break;
        }
        case CLIMACUS:
        {
            wchar_t tailCode = SMUFL_E991_chantPunctumInclinatum;
            DrawNc(dc, dynamic_cast<LayerElement *>(ncVector[0]), layer, staff, measure);
            for (int i = 1; i < (int)ncVector.size(); i++)
            {
                DrawNc(dc, dynamic_cast<LayerElement *>(ncVector[i]), layer, staff, measure, tailCode);
            }
            break;
        }
        default: 
            DrawLayerChildren(dc, neume, layer, staff, measure);
    }

    dc->EndGraphic(element, this);
}

void View::DrawNc(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure, wchar_t fontNo, int xOffset, int yOffset) {
    Nc *nc = dynamic_cast<Nc *>(element);
    assert(nc);

    Clef *clef = layer->GetClef(element);
    int staffSize = m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
    int staffLineNumber = staff->m_drawingLines;
    int clefLine = clef->GetLine();

    int noteY = element->GetDrawingY();
    int noteX = element->GetDrawingX();

    int clefYPosition = noteY - ( staffSize * (staffLineNumber - clefLine) );
    int pitchOffset;
    int octaveOffset = (nc->GetOct() - 3) * ( (staffSize / 2) * 7);

    if(clef->GetShape() == CLEFSHAPE_C){
        pitchOffset = (nc->GetPname() - 1) * (staffSize / 2);
    }
    else if(clef->GetShape() == CLEFSHAPE_F){
        pitchOffset = (nc->GetPname() - 4) * (staffSize / 2);
    }

    int yValue = clefYPosition + pitchOffset + octaveOffset;

    DrawSmuflCode(dc, noteX + xOffset, yValue + yOffset, fontNo, staff->m_drawingStaffSize, false, true);

}

} // namespace vrv
