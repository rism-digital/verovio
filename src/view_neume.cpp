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
    // initialization

//    ListOfObjects *beamChildren = beam->GetList(beam);


    /******************************************************************/
    // Start the Beam graphic and draw the children

    dc->StartGraphic(element, "", element->GetUuid());

    /******************************************************************/
    // Draw the children
    const ListOfObjects *syllableChildren = syllable->GetList(syllable);

    DrawLayerChildren(dc, syllable, layer, staff, measure);


    dc->EndGraphic(element, this);
}

void View::DrawNc(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(layer);
    assert(staff);
    assert(measure);

    Nc *nc = dynamic_cast<Nc *>(element);
    assert(nc);

    /******************************************************************/
    // Start the Neume graphic and draw the children

    dc->StartGraphic(element, "", element->GetUuid());

    /******************************************************************/
    // Draw the children
    DrawLayerChildren(dc, nc, layer, staff, measure);

    // Draw the actual notes
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

    wchar_t fontNo = SMUFL_E990_chantPunctum;
    int yValue = clefYPosition + pitchOffset + octaveOffset;

    DrawSmuflCode(dc, noteX, yValue, fontNo, staff->m_drawingStaffSize, false, true);

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
    std::map<std::string, NeumeGroup> neumeGroups = neume->GetGroups();
    
    // Obatining list of NC to extract intm data
    // Format of *neumechildren, first object is <neume>, all objects after are <nc>
    const ListOfObjects *neumeChildren = neume->GetList(neume);
    if (neumeChildren->empty()) {
        return;
    }

    // Loading the contour of the neume shape from the nc children
    std::string contour = "";
    for (int i = 1; i < (int)neumeChildren[0].size(); i++) {
         Nc *nc = dynamic_cast<Nc *>(neumeChildren[0][i]);
         if (i >= 2) {
             contour.append(nc->GetIntm());
         }
    }

    // Obtaining the NeumeGroup enum name from the map 
    NeumeGroup neumeName = neumeGroups[contour];

    // If the shape cannot be found in the map, the neume size will be greater than 2, and the default for
    // std::map for keys that aren't in is 0. This won't conflict with actual Punctums because the size will always
    // be 2 in that case.
    if ( (int)neumeChildren[0].size() > 2 && neumeGroups[contour] == 0) {
        //TODO: Error Handling for unfound neumes
        std::cout << "Neume Grouping not found";
    }

    /******************************************************************/
    // Start the Neume graphic and draw the children

    dc->StartGraphic(element, "", element->GetUuid());

    /******************************************************************/
    // Draw the children
    DrawLayerChildren(dc, neume, layer, staff, measure);

    dc->EndGraphic(element, this);
}

} // namespace vrv
