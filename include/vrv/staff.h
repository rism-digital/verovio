/////////////////////////////////////////////////////////////////////////////
// Name:        staff.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_STAFF_H__
#define __VRV_STAFF_H__

#include "atts_shared.h"
#include "object.h"

namespace vrv {

class DeviceContext;
class Layer;
class StaffAlignment;
class StaffDef;
class Syl;
class TimeSpanningInterface;

//----------------------------------------------------------------------------
// Staff
//----------------------------------------------------------------------------

/**
 * This class represents a staff in a laid-out score (Doc).
 * A Staff is contained in a System.
 * It contains Measure objects.
 * For unmeasured music, one single Measure is added for simplifying internal processing
*/
class Staff : public Object, public AttCommon {

public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Staff(int n = -1);
    virtual ~Staff();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Staff"; };
    virtual ClassId Is() const { return STAFF; };
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    virtual void AddChild(Object *object);
    ///@}

    int GetLayerCount() const { return (int)m_children.size(); };

    /**
     * Return the index position of the staff in its measure parent
     */
    int GetStaffIdx() const { return Object::GetIdx(); };

    /**
     * Return the default horizontal spacing of staves.
     */
    int GetVerticalSpacing();

    StaffAlignment *GetAlignment() const { return m_staffAlignment; };

    int GetYRel() const;

    //----------//
    // Functors //
    //----------//

    /**
     * Unset the initial scoreDef of each system and measure
     */
    virtual int UnsetCurrentScoreDef(FunctorParams *functorParams);

    /**
     * @name Reset the vertical alignment
     */
    ///@{
    virtual int ResetVerticalAlignment(FunctorParams *functorParams);
    ///@}

    /**
     * Align the content of a staff vertically.
     */
    virtual int AlignVertically(FunctorParams *functorParams);

    /**
     * See Object::FillStaffCurrentTimeSpanning
     */
    virtual int FillStaffCurrentTimeSpanning(FunctorParams *functorParams);

    /**
     * Reset the drawing values before calling PrepareDrawing after changes.
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

    /**
     * Set the drawing position (m_drawingX and m_drawingY) values for objects
     */
    virtual int SetDrawingXY(FunctorParams *functorParams);

    /**
     * Functor for setting mRpt drawing numbers (if required)
     * See implementation and Object::PrepareRpt
     */
    virtual int PrepareRpt(FunctorParams *functorParams);

public:
    /**
     * Number of lines copied from the staffDef for fast access when drawing
     */
    int m_drawingLines;

    /**
     * Notation type (CMN, mensural, black mensural, etc.), from the staffDef
     */
    int m_drawingNotationType;

    /**
     * Total drawing height from top of the top line to bottom of the bottom line
     */
    int m_drawingHeight;

    /**
     * The drawing staff size (scale), from the staffDef
     */
    int m_drawingStaffSize;

    std::vector<Object *> m_timeSpanningElements;

    /**
 * The Y absolute position of the staff for facsimile (transcription) encodings.
 * This is the top left corner of the staff (the X position is the position of the system).
 */
    int m_yAbs;

    StaffDef *m_drawingStaffDef;

private:
    /**
     * A pointer to a StaffAlignment for aligning the staves
     */
    StaffAlignment *m_staffAlignment;
};

} // namespace vrv

#endif
