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

    dc->StartGraphic(element, "", element->GetUuid());

    /******************************************************************/

    // Intializing necessary variables
    Clef *clef = layer->GetClef(element);
    int staffSize = m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
    int staffLineNumber = staff->m_drawingLines;
    int clefLine = clef->GetLine();

    int noteY = element->GetDrawingY();
    int noteX = element->GetDrawingX();

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

    int yValue = clefYPosition + pitchOffset + octaveOffset;

    // Determine grouping component is a part of and its position
    Neume *neume = dynamic_cast<Neume *>(nc->GetParent());
    assert(neume);

    NeumeGroup group = neume->GetNeumeGroup();
    int position = neume->GetChildIndex(element); 

    wchar_t fontNo = SMUFL_E990_chantPunctum, secondaryFontNo = 0;

    float xOffset = 0, yOffset = 0, xOffset2 = 0, yOffset2 = 0;
    bool secondaryGlyph = false;

    std::vector<int> pitchDifferences = neume->GetPitchDifferences();

    switch (group) {
        case PES:
        case PES_SUBPUNCTIS:
        {
            if (position == 1 && pitchDifferences.at(0) > 0) {
                xOffset = -0.25;
            }
            else if (position >= 2) {
                fontNo = SMUFL_E991_chantPunctumInclinatum;
            }
            break;
        }
        case PORRECTUS:
        case PORRECTUS_FLEXUS:
        case PORRECTUS_SUBPUNCTIS:
        {
            switch (position) {
                case 0:
                case 1:
                    switch (pitchDifferences.at(0)) {
                        case -1:
                            fontNo = (position == 0) ?
                                SMUFL_E9B4_chantEntryLineAsc2nd : SMUFL_E9B9_chantLigaturaDesc2nd;
                            break;
                        case -2:
                            fontNo = (position == 0) ?
                                SMUFL_E9B5_chantEntryLineAsc3rd : SMUFL_E9BA_chantLigaturaDesc3rd;
                            break;
                        case -3:
                            fontNo = (position == 0) ?
                                SMUFL_E9B6_chantEntryLineAsc4th : SMUFL_E9BB_chantLigaturaDesc4th;
                            break;
                        case -4:
                            fontNo = (position == 0) ?
                                SMUFL_E9B7_chantEntryLineAsc5th : SMUFL_E9BC_chantLigaturaDesc5th;
                            break;
                        default: break;
                    }
                    if (position == 0)
                        yOffset = pitchDifferences.at(0);
                    else {
                        xOffset = -1;
                        yOffset = -pitchDifferences.at(0);
                    }
                    break;
                case 2:
                    break;
                default:
                    if (group == PORRECTUS_SUBPUNCTIS)
                        fontNo = SMUFL_E991_chantPunctumInclinatum;
                    break;
            }
            break;
        }
        case CLIVIS:
        {
            // TODO add connecting lie
            if (pitchDifferences.at(0) < -1 && position == 1) {
                secondaryGlyph = true;
                switch (pitchDifferences.at(0)) {
                    case -2:
                        secondaryFontNo = SMUFL_E9BE_chantConnectingLineAsc3rd;
                        break;
                    case -3:
                        secondaryFontNo = SMUFL_E9BF_chantConnectingLineAsc4th;
                        break;
                    case -4:
                        secondaryFontNo = SMUFL_E9C0_chantConnectingLineAsc5th;
                        break;
                    default:
                        break;
                }
            }
            if (position == 0) {
                xOffset = 0.25;
            }
            break;
        }
        case CLIMACUS:
        case CLIMACUS_RESUPINUS:
        {
            if (position != 0) {
                fontNo = SMUFL_E991_chantPunctumInclinatum;
            }
            break;
        }
        //case SCANDICUS:
        //case SCANDICUS_FLEXUS:
        case SCANDICUS_SUBPUNCTIS:
        {
            if (position > 2) {
                fontNo = SMUFL_E991_chantPunctumInclinatum; 
            }
            break;
        }
        case TORCULUS_RESUPINUS:
        {
            if (pitchDifferences.at(1) == 1) {
                if (position == 1) {
                    fontNo = SMUFL_E9B9_chantLigaturaDesc2nd;
                }
                else if (position == 2) {
                    fontNo = SMUFL_E9BD_chantConnectingLineAsc2nd;
                }
            }
            else if (pitchDifferences.at(1) == 2) {
                if (position == 1) {
                    fontNo = SMUFL_E9BA_chantLigaturaDesc3rd;
                }
                else if (position == 2) {
                    fontNo = SMUFL_E9BD_chantConnectingLineAsc2nd;
                }
            }
            break;
        }
        case PRESSUS:
        {
            fontNo = (position == 2) ? SMUFL_E991_chantPunctumInclinatum : SMUFL_E990_chantPunctum;
            break;
        }
        default: break;
    } 
    // If the nc is supposed to be a virga and currently is being rendered as a punctum
    // change it to a virga
    if (nc->HasAttribute("diagonalright", "u") && fontNo == SMUFL_E990_chantPunctum) {
        fontNo = SMUFL_E996_chantPunctumVirga;
    }

    const int noteHeight = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 2);
    const int noteWidth = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 1.4);
    DrawSmuflCode(dc, noteX + xOffset * noteWidth, yValue + yOffset * noteHeight,
            fontNo, staff->m_drawingStaffSize, false, true);
   
    if (secondaryGlyph) {
        DrawSmuflCode(dc, noteX + xOffset2 * noteWidth, yValue + yOffset2 * noteHeight,
            secondaryFontNo, staff->m_drawingStaffSize, false, true);
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
