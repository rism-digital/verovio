/////////////////////////////////////////////////////////////////////////////
// Name:        system.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SYSTEM_H__
#define __VRV_SYSTEM_H__

#include "aligner.h"
#include "drawinginterface.h"
#include "object.h"

namespace vrv {

class DeviceContext;
class Measure;
class ScoreDef;

//----------------------------------------------------------------------------
// System
//----------------------------------------------------------------------------

/**
 * This class represents a system in a laid-out score (Doc).
 * A System is contained in a Page.
 * It contains Staff objects.
*/
class System : public Object, public DrawingListInterface {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    System();
    virtual ~System();
    virtual void Reset();
    virtual std::string GetClassName() const { return "System"; };
    virtual ClassId Is() const { return SYSTEM; };
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    void AddMeasure(Measure *measure);
    void AddScoreDef(ScoreDef *scoreDef);
    ///@}

    /**
     * @name Set and get the labels drawing width (normal and abbreviated)
     */
    ///@{
    int GetDrawingLabelsWidth() const { return m_drawingLabelsWidth; };
    void SetDrawingLabelsWidth(int width);
    int GetDrawingAbbrLabelsWidth() const { return m_drawingAbbrLabelsWidth; };
    void SetDrawingAbbrLabelsWidth(int width);
    ///@}

    /**
     * Return the height of the system
     */
    int GetHeight() const;

    /**
     * Return the default horizontal spacing of system.
     */
    int GetVerticalSpacing() const;

    /**
     * Return the index position of the system in its page parent
     */
    int GetSystemIdx() const { return Object::GetIdx(); };

    void SetCurrentFloatingPositioner(int staffN, FloatingElement *element, int x, int y);

    /**
     * @name Setter and getter of the drawing scoreDef
     */
    ///@{
    ScoreDef *GetDrawingScoreDef() const { return m_drawingScoreDef; };
    void SetDrawingScoreDef(ScoreDef *drawingScoreDef);
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * Unset the initial scoreDef of each system and measure
     */
    virtual int UnsetCurrentScoreDef(ArrayPtrVoid *params);

    /**
     * @name Reset the horizontal and vertical alignment
     */
    ///@{
    virtual int ResetHorizontalAlignment(ArrayPtrVoid *params);
    virtual int ResetVerticalAlignment(ArrayPtrVoid *params);
    ///@}

    /**
     * Align the content of a system vertically.
     */
    virtual int AlignVertically(ArrayPtrVoid *params);

    /**
     * Set the position of the StaffAlignment.
     * Redirect the functor to the SytemAligner
     */
    virtual int SetAligmentYPos(ArrayPtrVoid *params);

    /**
     * Correct the Y alignment once the content of a system has been aligned and laid out.
     * Special case that redirects the functor to the SystemAligner.
     */
    virtual int IntegrateBoundingBoxYShift(ArrayPtrVoid *params);

    /**
     * Align the system by adjusting the m_drawingYRel position looking at the SystemAligner.
     */
    virtual int AlignSystems(ArrayPtrVoid *params);

    /**
     * Align the measures by adjusting the m_drawingXRel position looking at the MeasureAligner.
     * In System object resets the shift to 0;
     */
    virtual int AlignMeasures(ArrayPtrVoid *params);

    /**
     * Store the width of the system in the MeasureAligner for justification
     */
    virtual int AlignMeasuresEnd(ArrayPtrVoid *params);

    /**
     * Justify the X positions
     * Special case that redirects the functor to the MeasureAligner.
     */
    virtual int JustifyX(ArrayPtrVoid *params);

    /**
     * See Object::CalcStaffOverlap
     */
    virtual int CalcStaffOverlap(ArrayPtrVoid *params);

    /**
     *
     */
    virtual int AdjustFloatingPostioners(ArrayPtrVoid *params);

    /**
     * See Object::CastOffPages
     */
    virtual int CastOffPages(ArrayPtrVoid *params);

    /**
     * Undo the cast of the system.
     * This is used by Doc::ContinuousLayout
     */
    virtual int UnCastOff(ArrayPtrVoid *params);

    /**
     * Set the drawing position (m_drawingX and m_drawingY) values for objects
     */
    virtual int SetDrawingXY(ArrayPtrVoid *params);

public:
    SystemAligner m_systemAligner;

public:
    /** System left margin (MEI scoredef@system.leftmar). Saved if != 0 */
    int m_systemLeftMar;
    /** System right margin (MEI scoredef@system.rightmar). Saved if != 0 */
    int m_systemRightMar;
    /**
     * The Y absolute position of the staff for facsimile (transcription) encodings.
     * This is the top left corner of the system.
     */
    int m_yAbs;
    /**
     * The Y relative position of the system.
     * It is used internally when calculating the layout and it is not stored in the file.
     */
    int m_drawingYRel;
    /**
     * The x absolute position of the  system for facsimile layouts.
     * This is the top left corner of the system.
     */
    int m_xAbs;
    /**
     * The X relative position of the system.
     * It is used internally when calculating the layout and it is not stored in the file.
     */
    int m_drawingXRel;
    /**
     * The width used by the labels at the left of the system.
     * It is used internally when calculating the layout and it is not stored in the file.
     */
    int m_drawingLabelsWidth;
    /** The width used by the abbreviated labels */
    int m_drawingAbbrLabelsWidth;
    /**
     * @name The total width of the system.
     * It is computed during the layout processed and used for calculating the justification ratio.
     */
    ///@{
    int m_drawingTotalWidth;
    int m_drawingJustifiableWidth;
    ///@}

private:
    ScoreDef *m_drawingScoreDef;
};

} // namespace vrv

#endif
