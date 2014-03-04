/////////////////////////////////////////////////////////////////////////////
// Name:        view_tuplet.cpp
// Author:      Rodolfo Zitellini
// Created:     21/08/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "view.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <cstring>
#include <stdio.h>
#include <typeinfo>

//----------------------------------------------------------------------------

#include "barline.h"
#include "beam.h"
#include "clef.h"
#include "keysig.h"
#include "layerelement.h"
#include "leipzigbbox.h"
#include "mensur.h"
#include "note.h"
#include "rest.h"
#include "symbol.h"
#include "tie.h"
#include "tuplet.h"

namespace vrv {

#define TUPLET_OFFSET 20
#define OBLIQUE_OFFSET 0x52 //move to oblique figures

std::string View::IntToObliqueFigures(unsigned int number) {
    char buf[16];
    unsigned int len;
    
    memset(buf, 0x00, sizeof(buf));
    
    // We do not convert more that FF values
    if (number > 0xFF) number = 0xFF;
    
    sprintf(buf, "%i", number);
    
    len = strlen(buf);
    assert((sizeof(buf) - 1) > len ); // String conversion overflow 
    
    for (unsigned int i = 0; i < strlen(buf); i++) {
        buf[i] += OBLIQUE_OFFSET;
    }
    
    return std::string(buf);
}

/**
 * Analyze a tuplet object and figure out if all the notes are in the same beam
 * or not
 */
bool View::OneBeamInTuplet(Tuplet* tuplet) {
    
    Beam *currentBeam = NULL;
    ArrayOfObjects elems;
    
    // Are we contained in a beam?
    if (dynamic_cast<Beam*>(tuplet->GetFirstParent(&typeid(Beam), 3)) && !tuplet->m_children.empty())
        return true;

    
    // No we contain a beam? Go on and search for it in the children
    for (unsigned int i = 0; i < tuplet->m_children.size(); i++) {        
        currentBeam = dynamic_cast<Beam*>(tuplet->m_children[i]);
        
        // first child is not a beam, or it is a beam but we have more than one child
        if (!currentBeam || tuplet->GetChildCount() > 1) {
            return false;
        }
    }
        
    return true;
}

/**
 * This function gets the tuplet coords for drawing the bracket and number
 * @param tuplet - the tuplet object
 * @param layer - layer obj
 * @param start, end, center - these are the coordinates returned
 * @return the direction of the beam
 *
 * We can divide the tuplets in three types:
 * 1) All notes beamed
 * 2) All notes unbeamed
 * 3) a mixture of the above
 * 
 * The first type are the simplest to calculate, as we just need the 
 * start and end of the beam
 * types 2 and 3 are threaed in the same manner to calculate the points:
 * - if all the stems are in the same direction, the bracket goes from the
 *   first to the last stem and the number is centered. If a stem in the
 *   middle il longher than the first or last, the y positions are offsetted
 *   accordingly to evitate collisions
 * - if stems go in two different directions, the bracket and number are
 *   placed in the side that has more stems in that direction. If the
 *   stems are equal, if goes up. In this case the bracket is orizontal
 *   so we just need the tallnes of the most tall stem. If a notehead
 *   il lower (or upper) than this stem, we compensate that too with an offset
 
 */

bool View::GetTupletCoordinates(Tuplet* tuplet, Layer *layer, MusPoint* start, MusPoint* end, MusPoint *center) {
    MusPoint first, last;
    int x, y;
    bool direction = true; //true = up, false = down
    
    LayerElement *firstNote, *lastNote;
    
    // AllNotesBeamed tries to figure out if all the notes are in the same beam
    if (OneBeamInTuplet(tuplet)) {
        
        firstNote = dynamic_cast<LayerElement*>(tuplet->m_list.front());
        lastNote = dynamic_cast<LayerElement*>(tuplet->m_list.back());

        // yes they are in a beam
        // get the x position centered from the STEM so it looks better
        // NOTE start and end are left to 0, this is the signal that no bracket has to be drawn
        x = firstNote->m_drawingStemStart.x + (lastNote->m_drawingStemStart.x - firstNote->m_drawingStemStart.x) / 2;
        
        // align the center point at the exact center of the first an last stem
        // TUPLET_OFFSET is summed so it does not collide with the stem
        if (firstNote->m_drawingStemDir)
            y = lastNote->m_drawingStemEnd.y + (firstNote->m_drawingStemEnd.y - lastNote->m_drawingStemEnd.y) / 2 + TUPLET_OFFSET;
        else 
            y = lastNote->m_drawingStemEnd.y + (firstNote->m_drawingStemEnd.y - lastNote->m_drawingStemEnd.y) / 2 - TUPLET_OFFSET;
        
        // Copy the generated coordinates
        center->x = x;
        center->y = y;
        direction =  firstNote->m_drawingStemDir; // stem direction is same for all notes
    } else {
            
        //ArrayOfObjects all_notes;
        
        // We can have a mixed group of Beams and notes. Flatten it
        /*
        for (unsigned int i = 0; i < tuplet->m_children.size(); i++) {
            if (dynamic_cast<Note*>(tuplet->m_children[i]))
                all_notes.push_back(tuplet->m_children[i]);
            
            if (dynamic_cast<Beam*>(tuplet->m_children[i])) {
                Beam* beam = dynamic_cast<Beam*>(tuplet->m_children[i]);
                std::copy( beam->m_list.begin(), beam->m_list.end(), std::back_inserter( all_notes ) );
            }
        }*/
        
        firstNote = dynamic_cast<LayerElement*>(tuplet->m_list.front());
        lastNote = dynamic_cast<LayerElement*>(tuplet->m_list.back());
        
        // There are unbeamed notes of two different beams
        // treat all the notes as unbeames
        int ups = 0, downs = 0; // quantity of up- and down-stems
        
        // In this case use the center of the notehead to calculate the exact center
        // as it looks better
        x = firstNote->GetDrawingX() + (lastNote->GetDrawingX() - firstNote->GetDrawingX() + lastNote->m_selfBB_x2) / 2;
        
        // Return the start and end position for the brackes
        // starting from the first edge and last of the BBoxes
        start->x = firstNote->m_selfBB_x1 + firstNote->GetDrawingX();
        end->x = lastNote->m_selfBB_x2 + lastNote->GetDrawingX();
        
        // THe first step is to calculate all the stem directions
        // cycle into the elements and count the up and down dirs
        ListOfObjects::iterator iter = tuplet->m_list.begin();
        while (iter != tuplet->m_list.end()) {
            LayerElement *currentNote = dynamic_cast<LayerElement*>(*iter);
            
            if (currentNote->m_drawingStemDir == true)
                ups++;
            else
                downs++;
            
            ++iter;
        }
        // true means up
        direction = ups > downs ? true : false;
        
        // if ups or downs is 0, it means all the stems go in the same direction
        if (ups == 0 || downs == 0) {
            
            // Calculate the average between the first and last stem
            // set center, start and end too.
            if (direction) { // up
                y = lastNote->m_drawingStemEnd.y + (firstNote->m_drawingStemEnd.y - lastNote->m_drawingStemEnd.y) / 2 + TUPLET_OFFSET;
                start->y = firstNote->m_drawingStemEnd.y + TUPLET_OFFSET;
                end->y = lastNote->m_drawingStemEnd.y + TUPLET_OFFSET;
            } else {
                y = lastNote->m_drawingStemEnd.y + (firstNote->m_drawingStemEnd.y - lastNote->m_drawingStemEnd.y) / 2 - TUPLET_OFFSET;
                start->y = firstNote->m_drawingStemEnd.y - TUPLET_OFFSET;
                end->y = lastNote->m_drawingStemEnd.y - TUPLET_OFFSET;
            }
            
            // Now we cycle again in all the intermediate notes (i.e. we start from the second note
            // and stop at last -1)
            // We will see if the position of the note is more (or less for down stems) of the calculated
            // average. In this case we offset down or up all the points
            iter = tuplet->m_list.begin();
            while (iter != tuplet->m_list.end()) {
                 LayerElement *currentNote = dynamic_cast<LayerElement*>(*iter);
                
                if (direction) {
                    // The note is more than the avg, adjust to y the difference
                    // from this note to the avg
                    if (currentNote->m_drawingStemEnd.y + TUPLET_OFFSET > y) {
                        int offset = y - (currentNote->m_drawingStemEnd.y + TUPLET_OFFSET);
                        y -= offset;
                        end->y -= offset;
                        start->y -= offset;
                    }
                } else {
                    if (currentNote->m_drawingStemEnd.y - TUPLET_OFFSET < y) {
                        int offset = y - (currentNote->m_drawingStemEnd.y - TUPLET_OFFSET);
                        y -= offset;
                        end->y -= offset;
                        start->y -= offset;
                    }
                }
                
                ++iter;
            }
            
            
        } else { // two directional beams
            // this case is similar to the above, but the bracket is only orizontal
            // y is 0 because the final y pos is above the tallest stem
            y = 0;
            
            // Find the tallest stem and set y to it (with the offset distance)
            iter = tuplet->m_list.begin();
            while (iter != tuplet->m_list.end()) {
                LayerElement *currentNote = dynamic_cast<LayerElement*>(*iter);
                
                if (currentNote->m_drawingStemDir == direction) {
                                        
                    if (direction) {
                        if (y == 0 || currentNote->m_drawingStemEnd.y + TUPLET_OFFSET >= y)
                            y = currentNote->m_drawingStemEnd.y + TUPLET_OFFSET;
                    } else {
                        if (y == 0 || currentNote->m_drawingStemEnd.y - TUPLET_OFFSET <= y)
                            y = currentNote->m_drawingStemEnd.y - TUPLET_OFFSET;
                    }
                        
                } else {
                    // do none for now
                    // but if a notehead with a reversed stem is taller that the last
                    // calculated y, we need to offset
                }
                
                ++iter;
            }
            
            // end and start are on the same line (and so il center when set later)
            end->y = start->y = y;
        }
    }
        
    
    center->x = x;
    center->y = y;
    return direction;
}


void View::DrawTuplet( DeviceContext *dc, Tuplet *tuplet, Layer *layer, Staff *staff)
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    
    tuplet->ResetList(tuplet);
    
    int txt_lenght, txt_height;
    
    std::string notes = IntToObliqueFigures((unsigned int)tuplet->GetNum());
    
    dc->GetTextExtent(notes, &txt_lenght, &txt_height);
    
    MusPoint start, end, center;
    bool direction = GetTupletCoordinates(tuplet, layer, &start, &end, &center);
        
    // Calculate position for number 0x82
    // since the number is slanted, move the center left
    // by 4 pixels so it seems more centered to the eye
    int txt_x = center.x - (txt_lenght / 2) - 4;
    //DrawLeipzigFont ( dc, txt_x,  center.y, notes + 0x82, staff, false);
    
    DrawLeipzigString(dc, txt_x, center.y, notes, 0);
    
    //dc->SetPen(AxBLACK);
    dc->SetPen(AxBLACK, 2, AxSOLID);
    
    // Start is 0 when no line is necessary (i.e. beamed notes)
    if (start.x > 0) {
        // Draw the bracket, interrupt where the number is
        
        // get the slope
        double m = (double)(start.y - end.y) / (double)(start.x - end.x);
        
        // x = 10 pixels before the number
        double x = txt_x - 4;
        // xa = just after, the number is abundant so I do not add anything
        double xa = txt_x + txt_lenght + 2;
        
        // calculate the y coords in the slope
        double y1 = (double)start.y + m * (x - (double)start.x);
        double y2 = (double)start.y + m * (xa - (double)start.x);
        
        // first line
        dc->DrawLine(start.x, ToDeviceContextY(start.y), (int)x, ToDeviceContextY((int)y1));
        // second line after gap
        dc->DrawLine((int)xa, ToDeviceContextY((int)y2), end.x, ToDeviceContextY(end.y));
        
        // vertical bracket lines
        if (direction) {
            dc->DrawLine(start.x, ToDeviceContextY(start.y), start.x, ToDeviceContextY(start.y - 10));
            dc->DrawLine(end.x, ToDeviceContextY(end.y), end.x, ToDeviceContextY(end.y - 10));
        } else {
            dc->DrawLine(start.x, ToDeviceContextY(start.y), start.x, ToDeviceContextY(start.y + 10));
            dc->DrawLine(end.x, ToDeviceContextY(end.y), end.x, ToDeviceContextY(end.y + 10));
        }
                
    }
    
    //rz dc->EndGraphic(element, this );
}

} // namespace vrv
