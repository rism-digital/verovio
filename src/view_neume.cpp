/////////////////////////////////////////////////////////////////////////////
// Name:        view_neume.cpp
// Author:      Andrew Tran, Juliette Regimbal
// Created:     2017
// Copyright (c) Author and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "view.h"

//----------------------------------------------------------------------------

#include <algorithm>
#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "devicecontext.h"
#include "doc.h"
#include "layer.h"
#include "layerelement.h"
#include "mrpt.h"
#include "nc.h"
#include "neume.h"
#include "note.h"
#include "smufl.h"
#include "staff.h"
#include "syllable.h"
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

void View::DrawNc(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(layer);
    assert(staff);
    assert(measure);

    Nc *nc = dynamic_cast<Nc *>(element);
    assert(nc);

    struct drawingParams {
        wchar_t fontNo = SMUFL_E990_chantPunctum;
        float xOffset = 0;
        float yOffset = 0;
    };
    std::vector<drawingParams> params;
    params.push_back(drawingParams());
    dc->StartGraphic(element, "", element->GetUuid());

    /******************************************************************/

    // Intializing necessary variables
    Clef *clef = layer->GetClef(element);
    int staffSize = m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
    int staffLineNumber = staff->m_drawingLines;
    int clefLine = clef->GetLine();
   
    Neume *neume = dynamic_cast<Neume*>(nc->GetFirstParent(NEUME));
    assert(neume);
    int position = neume->GetChildIndex(element);

    // Check if nc is part of a ligature or is an inclinatum
    if (nc->HasName() && nc->GetName() == ncVis_NAME_inclinatum) {
        params.at(0).fontNo = SMUFL_E991_chantPunctumInclinatum;
    }
    /*
    else if (nc->HasLigature() && nc->GetLigature() == BOOLEAN_true) {
        // Check if this is the first or second part of a ligature
        Neume *neume = dynamic_cast<Neume *>(nc->GetFirstParent(NEUME));
        assert(neume);
        int currentPosition = neume->GetChildIndex(element);
        Nc *nextNc = dynamic_cast<Nc *>(neume->GetChild(currentPosition + 1));
        // Is the second part
        if (nextNc == nullptr || !nextNc->HasLigature() || nextNc->GetLigature() == BOOLEAN_false) {
            fontNo = SMUFL_E9BD_chantConnectingLineAsc2nd;
        }
        else {


    }*/
    // If the nc is supposed to be a virga and currently is being rendered as a punctum
    // change it to a virga
    if (nc->GetDiagonalright() == ncVis_DIAGONALRIGHT_u && params.at(0).fontNo == SMUFL_E990_chantPunctum) {
        params.at(0).fontNo = SMUFL_E996_chantPunctumVirga;
    }

    const int noteHeight = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 2);
    const int noteWidth = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 1.4);
    int noteY, noteX;
    int yValue;
    if (nc->HasFacs()) {
        noteY = ToLogicalY(staff->GetDrawingY());
        noteX = nc->GetDrawingX();
    }
    else if (neume->HasFacs()) {
        noteY = ToLogicalY(staff->GetDrawingY());
        noteX = neume->GetDrawingX() + position * noteWidth;
    }
    else {
        noteX = element->GetDrawingX();
        noteY = element->GetDrawingY();
    }
    // Calculating proper y offset based on pname, clef, and staff
    int clefYPosition = noteY - (staffSize * (staffLineNumber - clefLine));
    int pitchOffset = 0;
    int octaveOffset = (nc->GetOct() - 3) * ((staffSize / 2) * 7);

    if (clef->GetShape() == CLEFSHAPE_C) {
        pitchOffset = (nc->GetPname() - 1) * (staffSize / 2);
    }
    else if (clef->GetShape() == CLEFSHAPE_F) {
        pitchOffset = (nc->GetPname() - 4) * (staffSize / 2);
    }

    yValue = clefYPosition + pitchOffset + octaveOffset;
   
    for (auto it = params.begin(); it != params.end(); it++) {
        DrawSmuflCode(dc, noteX + it->xOffset * noteWidth, yValue + it->yOffset * noteHeight,
               it->fontNo, staff->m_drawingStaffSize, false, true);
    } 
    
    // Draw the children
    DrawLayerChildren(dc, nc, layer, staff, measure);

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
    // Start the Neume graphic and draw the children

    dc->StartGraphic(element, "", element->GetUuid());
    DrawLayerChildren(dc, neume, layer, staff, measure);
    dc->EndGraphic(element, this);
}

} // namespace vrv
