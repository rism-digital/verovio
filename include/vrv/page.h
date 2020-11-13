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
class RunningElement;
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
    virtual bool IsSupportedChild(Object *object);
    ///@}

    /**
     * Return the number of system (children are System object only)
     */
    int GetSystemCount() const { return (int)GetChildren()->size(); }

    /**
     * @name Get and set the pixel per unit factor.
     */
    ///@{
    double GetPPUFactor() const { return m_PPUFactor; }
    void SetPPUFactor(double PPUFactor) { m_PPUFactor = PPUFactor; }
    ///@}

    /**
     * @name Getter header and footer.
     * Looks if the page is the first one or not
     */
    ///@{
    RunningElement *GetHeader() const;
    RunningElement *GetFooter() const;
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

    //----------//
    // Functors //
    //----------//

    /**
     * Apply the Pixel Per Unit factor of the page to its elements.
     */
    virtual int ApplyPPUFactor(FunctorParams *functorParams);

    /**
     * See Object::ResetVerticalAlignment
     */
    virtual int ResetVerticalAlignment(FunctorParams *functorParams);

    /**
     * See Object::AlignVertically
     */
    ///@{
    virtual int AlignVerticallyEnd(FunctorParams *functorParams);
    ///@}

    /**
     * See Object::AlignSystems
     */
    ///@{
    virtual int AlignSystems(FunctorParams *functorParams);
    virtual int AlignSystemsEnd(FunctorParams *functorParams);
    ///@}

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
    /** Page bottom margin (MEI scoredef@page.botmar). Saved if != 0 */
    int m_pageMarginBottom;
    /** Page left margin (MEI scoredef@page.leftmar). Saved if != 0 */
    int m_pageMarginLeft;
    /** Page right margin (MEI scoredef@page.rightmar). Saved if != 0 */
    int m_pageMarginRight;
    /** Page top margin (MEI scoredef@page.topmar). Saved if != 0 */
    int m_pageMarginTop;
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

    /**
     * Temporary member that will be replace by its LibMEI equivalent in the next version of the page-based MEI
     */
    double m_PPUFactor;

    /**
     * The height that can be justified once the systems are aligned.
     * Takes into account header and footer.
     */
    int m_drawingJustifiableHeight;

    /**
     * the sum of justification factors for each type of spacing in between
     * systems and staves (staff, brace group, bracket group)
     */
    double m_justificationSum;

private:
    /**
     * A flag for indicating whether the layout has been done or not.
     * If yes, Layout will not layout the page (again) unless
     * the force parameter is set.
     */
    bool m_layoutDone;
};

} // namespace vrv

#endif
