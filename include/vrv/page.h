/////////////////////////////////////////////////////////////////////////////
// Name:        page.h
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_PAGE_H__
#define __VRV_PAGE_H__

#include "object.h"
#include "scoredef.h"

namespace vrv {

class DeviceContext;
class PrepareProcessingListsParams;
class Staff;
class System;

//----------------------------------------------------------------------------
// Page
//----------------------------------------------------------------------------

/**
 * This class represents a page in a laid-out score (Doc).
 * A Page is contained in a Doc.
 * It contains System objects.
*/
class Page : public Object {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Page();
    virtual ~Page();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Page"; }
    virtual ClassId GetClassId() const { return PAGE; }
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    virtual void AddChild(Object *object);
    ///@}

    /**
     * Return the number of system (children are System object only)
     */
    int GetSystemCount() const { return (int)m_children.size(); }

    /**
     * @name Get and set the pixel per unit factor.
     */
    ///@{
    double GetPPUFactor() const { return m_PPUFactor; }
    void SetPPUFactor(double PPUFactor) { m_PPUFactor = PPUFactor; }
    ///@}

    /**
     * Return the index position of the page in its document parent
     */
    int GetPageIdx() const { return Object::GetIdx(); }

    /**
     * Return the position of the staff on the page, from top to bottom
     */
    int GetStaffPosOnPage(Staff *staff) const;

    /**
     * Do the layout of the page, which means aligning its content horizontally
     * and vertically, and justify horizontally and vertically if wanted.
     * This will be done only if m_layoutDone is false or force is true.
     */
    void LayOut(bool force = false);

    /**
     * Do the layout for a transcription page (with layout information).
     * This only calculates positioning or layer element parts using provided layout of parents.
     */
    void LayOutTranscription(bool force = false);

    /**
     * Lay out the content of the page (measures and their content) horizontally
     */
    void LayOutHorizontally();

    /**
     * Justifiy the content of the page (measures and their content) horizontally
     */
    void JustifyHorizontally();

    /**
     * Lay out the content of the page (system/staves) vertically.
     */
    void LayOutVertically();

    /**
     * Justifiy the content of the page (system/staves) vertically
     */
    void JustifyVertically();

    /**
     * Lay out the pitch positions and stems (without redoing the entire layout)
     */
    void LayOutPitchPos();

    /**
     * Return the height of the content by looking at the last system of the page.
     * This is used for adjusting the page height when this is the expected behaviour,
     * typically with the --adjust-page-height option in the commandline tool
     */
    int GetContentHeight() const;

    /**
     * Return the width of the content by looking at the first system of the page.
     * This is used for adjusting the page width when this is the expected behaviour,
     * typically with the --no_layout option in the commandline tool
     *
     */
    int GetContentWidth() const;

    /**
     * Custom method for upgrading page-based page transcription data
     */
    void UpgradePageBasedMEI(Doc *doc);

    //----------//
    // Functors //
    //----------//

    /**
     * Apply the Pixel Per Unit factor of the page to its elements.
     */
    virtual int ApplyPPUFactor(FunctorParams *);

private:
    /**
     * Adjust the horizontal postition of the syl processing verse by verse
     */
    void AdjustSylSpacingByVerse(PrepareProcessingListsParams &listsParams, Doc *doc);

    //
public:
    /** Page width (MEI scoredef@page.width). Saved if != -1 */
    int m_pageWidth;
    /** Page height (MEI scoredef@page.height). Saved if != -1 */
    int m_pageHeight;
    /** Page left margin (MEI scoredef@page.leftmar). Saved if != 0 */
    short m_pageLeftMar;
    /** Page right margin (MEI scoredef@page.rightmar). Saved if != 0 */
    short m_pageRightMar;
    /** Page top margin (MEI scoredef@page.topmar). Saved if != 0 */
    short m_pageTopMar;
    /**
     * Surface (MEI @surface). Saved as facsimile for transciption layout.
     * For now, the target of the <graphic> element within surface is loaded here.
     */
    std::string m_surface;

    /**
     * Hold the top scoreDef of the page.
     * The value must be initialized by going through the whole score for finding
     * all the clef or key changes that might occur within the text.
     * The value is initialized by the Object::SetCurrentScoreDef functor.
     */
    ScoreDef m_drawingScoreDef;

private:
    /**
     * A flag for indicating whether the layout has been done or not.
     * If yes, Layout will not layout the page (again) unless
     * the force parameter is set.
     */
    bool m_layoutDone;

    /**
     *
     */
    double m_PPUFactor;
};

} // namespace vrv

#endif
