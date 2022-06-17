/////////////////////////////////////////////////////////////////////////////
// Name:        elementpart.h
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ELEMENT_PART_H__
#define __VRV_ELEMENT_PART_H__

#include "atts_cmn.h"
#include "atts_shared.h"
#include "atts_visual.h"
#include "layerelement.h"

namespace vrv {

class TupletNum;

//----------------------------------------------------------------------------
// Dots
//----------------------------------------------------------------------------

/**
 * This class models a group of dots as a layer element part and has not direct MEI equivlatent.
 */
class Dots : public LayerElement, public AttAugmentDots {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Dots();
    virtual ~Dots();
    void Reset() override;
    std::string GetClassName() const override { return "Dots"; }
    Object *Clone() const override { return new Dots(*this); }
    ///@}

    /** Override the method since alignment is required */
    bool HasToBeAligned() const override { return true; }

    std::set<int> GetDotLocsForStaff(const Staff *staff) const;
    std::set<int> &ModifyDotLocsForStaff(const Staff *staff);

    const MapOfDotLocs &GetMapOfDotLocs() const { return m_dotLocsByStaff; }
    void SetMapOfDotLocs(const MapOfDotLocs &dotLocs) { m_dotLocsByStaff = dotLocs; };

    void IsAdjusted(bool isAdjusted) { m_isAdjusted = isAdjusted; }
    bool IsAdjusted() const { return m_isAdjusted; }

    //----------//
    // Functors //
    //----------//

    /**
     * Overwritten version of Save that avoids anything to be written
     */
    ///@{
    int Save(FunctorParams *functorParams) override { return FUNCTOR_CONTINUE; }
    int SaveEnd(FunctorParams *functorParams) override { return FUNCTOR_CONTINUE; }
    ///@}

    /**
     * Set/get methods for the flagShift
     */
    ///@{
    int GetFlagShift() const { return m_flagShift; }
    void SetFlagShift(int shiftVal) { m_flagShift = shiftVal; }
    ///@}

    /**
     * See Object::ResetData
     */
    int ResetData(FunctorParams *functorParams) override;

    /**
     * See Object::ResetHorizontalAlignment
     */
    int ResetHorizontalAlignment(FunctorParams *functorParams) override;

private:
    //
public:
    //
private:
    /**
     * A map of dot locations
     */
    MapOfDotLocs m_dotLocsByStaff;

    bool m_isAdjusted;
    int m_flagShift;
};

//----------------------------------------------------------------------------
// Flag
//----------------------------------------------------------------------------

/**
 * This class models a stem as a layer element part and has not direct MEI equivlatent.
 */
class Flag : public LayerElement {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Flag();
    virtual ~Flag();
    void Reset() override;
    std::string GetClassName() const override { return "Flag"; }
    Object *Clone() const override { return new Flag(*this); }
    ///@}

    /** Override the method since alignment is required */
    bool HasToBeAligned() const override { return true; }

    wchar_t GetFlagGlyph(data_STEMDIRECTION stemDir) const;

    Point GetStemUpSE(const Doc *doc, int staffSize, bool graceSize) const;
    Point GetStemDownNW(const Doc *doc, int staffSize, bool graceSize) const;

    //----------//
    // Functors //
    //----------//

    /**
     * Overwritten version of Save that avoids anything to be written
     */
    ///@{
    int Save(FunctorParams *functorParams) override { return FUNCTOR_CONTINUE; }
    int SaveEnd(FunctorParams *functorParams) override { return FUNCTOR_CONTINUE; }
    ///@}

    /**
     * See Object::ResetData
     */
    int ResetData(FunctorParams *functorParams) override;

private:
    //
public:
    /** The number of flags to be drawn */
    int m_drawingNbFlags;

private:
};

//----------------------------------------------------------------------------
// TupletBracket
//----------------------------------------------------------------------------

/**
 * This class models a bracket as a layer element part and has not direct MEI equivlatent.
 * It is used to represent tuplet brackets.
 */
class TupletBracket : public LayerElement, public AttTupletVis {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    TupletBracket();
    virtual ~TupletBracket();
    void Reset() override;
    std::string GetClassName() const override { return "TupletBracket"; }
    ///@}

    /**
     * @name Setter and getter for drawing rel positions
     */
    ///@{
    int GetDrawingXRelLeft() const { return m_drawingXRelLeft; }
    void SetDrawingXRelLeft(int drawingXRelLeft) { m_drawingXRelLeft = drawingXRelLeft; }
    int GetDrawingXRelRight() const { return m_drawingXRelRight; }
    void SetDrawingXRelRight(int drawingXRelRight) { m_drawingXRelRight = drawingXRelRight; }
    // Vertical positions
    int GetDrawingYRelLeft() const { return m_drawingYRelLeft; }
    void SetDrawingYRelLeft(int drawingYRelLeft) { m_drawingYRelLeft = drawingYRelLeft; }
    int GetDrawingYRelRight() const { return m_drawingYRelRight; }
    void SetDrawingYRelRight(int drawingYRelRight) { m_drawingYRelRight = drawingYRelRight; }
    ///@}

    /**
     * @name Setter and getter for drawing positions.
     * Takes into account:
     * - the position of the first and last element.
     * - the position of the beam if aligned with a beam.
     */
    ///@{
    int GetDrawingXLeft() const;
    int GetDrawingXRight() const;
    int GetDrawingYLeft() const;
    int GetDrawingYRight() const;
    ///@}

    /**
     * @name Setter and getter for the aligned num
     */
    ///@{
    TupletNum *GetAlignedNum() { return m_alignedNum; }
    const TupletNum *GetAlignedNum() const { return m_alignedNum; }
    void SetAlignedNum(TupletNum *alignedNum) { m_alignedNum = alignedNum; }
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * Overwritten version of Save that avoids anything to be written
     */
    ///@{
    int Save(FunctorParams *functorParams) override { return FUNCTOR_CONTINUE; }
    int SaveEnd(FunctorParams *functorParams) override { return FUNCTOR_CONTINUE; }
    ///@}

    /**
     * See Object::ResetHorizontalAlignment
     */
    int ResetHorizontalAlignment(FunctorParams *functorParams) override;

    /**
     * See Object::ResetVerticalAlignment
     */
    int ResetVerticalAlignment(FunctorParams *functorParams) override;

private:
    //
public:
    //
private:
    /**
     * The XRel shift from the left X position.
     * The left X position is the one of the first Chord / Note / Rest in the tuplet
     */
    int m_drawingXRelLeft;
    /**
     * The XRel shift from the right X position.
     * The right X position is the one of the last Chord / Note / Rest in the tuplet
     */
    int m_drawingXRelRight;
    /**
     * The YRel shift for the left X position.
     */
    int m_drawingYRelLeft = 0;
    /**
     * The YRel shift for the right X position.
     */
    int m_drawingYRelRight = 0;
    /** A pointer to the num with which the TupletBracket is aligned (if any) */
    TupletNum *m_alignedNum;
};

//----------------------------------------------------------------------------
// TupletNum
//----------------------------------------------------------------------------

/**
 * This class models a tuplet num as a layer element part and has not direct MEI equivlatent.
 * It is used to represent tuplet number
 */
class TupletNum : public LayerElement, public AttNumberPlacement, public AttTupletVis {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    TupletNum();
    virtual ~TupletNum();
    void Reset() override;
    std::string GetClassName() const override { return "TupletNum"; }
    ///@}

    /**
     * @name Setter and getter for darwing positions.
     * Takes into account:
     * - the position of the first and last element.
     * - the position of the bracket if aligned with a bracket.
     * - the position of the beam if aligned with a beam.
     */
    ///@{
    int GetDrawingYMid() const;
    int GetDrawingXMid(const Doc *doc = NULL) const;
    ///@}

    /**
     * @name Setter and getter for the aligned bracket
     */
    ///@{
    TupletBracket *GetAlignedBracket() { return m_alignedBracket; }
    const TupletBracket *GetAlignedBracket() const { return m_alignedBracket; }
    void SetAlignedBracket(TupletBracket *alignedBracket);
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * Overwritten version of Save that avoids anything to be written
     */
    ///@{
    int Save(FunctorParams *functorParams) override { return FUNCTOR_CONTINUE; }
    int SaveEnd(FunctorParams *functorParams) override { return FUNCTOR_CONTINUE; }
    ///@}

    /**
     * See Object::ResetHorizontalAlignment
     */
    int ResetHorizontalAlignment(FunctorParams *functorParams) override;

    /**
     * See Object::ResetVerticalAlignment
     */
    int ResetVerticalAlignment(FunctorParams *functorParams) override;

private:
    //
public:
    //
private:
    /** A pointer to the bracket with which the TupletNum is aligned (if any) */
    TupletBracket *m_alignedBracket;
};

//----------------------------------------------------------------------------
// Stem
//----------------------------------------------------------------------------

/**
 * This class models a stem as a layer element part and has not direct MEI equivlatent.
 */
class Stem : public LayerElement, public AttGraced, public AttStems, public AttStemsCmn {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Stem();
    virtual ~Stem();
    void Reset() override;
    std::string GetClassName() const override { return "Stem"; }
    Object *Clone() const override { return new Stem(*this); }
    ///@}

    /** Override the method since alignment is required */
    bool HasToBeAligned() const override { return true; }

    /**
     * Add an element (only flag supported) to a stem.
     */
    bool IsSupportedChild(Object *object) override;

    /**
     * @name Setter and getter for darwing stem direction and length
     */
    ///@{
    data_STEMDIRECTION GetDrawingStemDir() const { return m_drawingStemDir; }
    void SetDrawingStemDir(data_STEMDIRECTION drawingStemDir) { m_drawingStemDir = drawingStemDir; }
    int GetDrawingStemLen() const { return m_drawingStemLen; }
    void SetDrawingStemLen(int drawingStemLen) { m_drawingStemLen = drawingStemLen; }
    int GetDrawingStemAdjust() const { return m_drawingStemAdjust; }
    void SetDrawingStemAdjust(int drawingStemAdjust) { m_drawingStemAdjust = drawingStemAdjust; }
    int GetStemModRelY() const { return m_stemModRelY; }
    ///@}

    /**
     * @name Setter and getter of the virtual flag
     */
    ///@{
    bool IsVirtual() const { return m_isVirtual; }
    void IsVirtual(bool isVirtual) { m_isVirtual = isVirtual; }
    ///@}

    /**
     * Helper to adjust overlaping layers for stems
     */
    int CompareToElementPosition(const Doc *doc, const LayerElement *otherElement, int margin) const;

    /**
     * Helper to calculate stem modifier relative Y rel and required adjustment for stem length
     */
    int CalculateStemModAdjustment(const Doc *doc, const Staff *staff, int flagOffset = 0);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::CalcStem
     */
    int CalcStem(FunctorParams *functorParams) override;

    /**
     * Overwritten version of Save that avoids anything to be written
     */
    ///@{
    int Save(FunctorParams *functorParams) override { return FUNCTOR_CONTINUE; }
    int SaveEnd(FunctorParams *functorParams) override { return FUNCTOR_CONTINUE; }
    ///@}

    /**
     * See Object::ResetData
     */
    int ResetData(FunctorParams *functorParams) override;

private:
    /**
     * Addjusts flag placement and stem length if they are crossing notehead or ledger lines
     */
    void AdjustFlagPlacement(const Doc *doc, Flag *flag, int staffSize, int verticalCenter, int duration);

    /**
     * Helper to adjust length of stem based on presence of slashes
     */
    int AdjustSlashes(const Doc *doc, const Staff *staff, int flagOffset) const;

    /**
     * Helper to calculate relative position for the stem modifier
     */
    void CalculateStemModRelY(const Doc *doc, const Staff *staff);

public:
    //
private:
    /**
     * The drawing direction of the stem
     */
    data_STEMDIRECTION m_drawingStemDir;
    /**
     * The drawing length of stem
     */
    int m_drawingStemLen;
    /**
     * Relative Y position for the stem modifier
     */
    int m_stemModRelY;
    /**
     * The adjustment of the drawing stem length (used with french style of beams)
     */
    int m_drawingStemAdjust;
    /**
     * A flag indicating if a stem if virtual and should never be rendered.
     * Virtual stems are added to whole notes (and longer) for position calculation and
     * for supporting MEI @stem.mod
     */
    bool m_isVirtual;
};

} // namespace vrv

#endif
