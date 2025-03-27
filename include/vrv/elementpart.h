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
 * This class models a group of dots as a layer element part and has no direct MEI equivalent.
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
    std::string GetClassName() const override { return "dots"; }
    Object *Clone() const override { return new Dots(*this); }
    ///@}

    /** Override the method since alignment is required */
    bool HasToBeAligned() const override { return true; }

    std::set<int> GetDotLocsForStaff(const Staff *staff) const;
    std::set<int> &ModifyDotLocsForStaff(const Staff *staff);

    const MapOfDotLocs &GetMapOfDotLocs() const { return m_dotLocsByStaff; }
    void SetMapOfDotLocs(const MapOfDotLocs &dotLocs) { m_dotLocsByStaff = dotLocs; };
    void ResetMapOfDotLocs() { m_dotLocsByStaff.clear(); }

    void IsAdjusted(bool isAdjusted) { m_isAdjusted = isAdjusted; }
    bool IsAdjusted() const { return m_isAdjusted; }

    //----------//
    // Functors //
    //----------//

    /**
     * Interface for class functor visitation
     */
    ///@{
    FunctorCode Accept(Functor &functor) override;
    FunctorCode Accept(ConstFunctor &functor) const override;
    FunctorCode AcceptEnd(Functor &functor) override;
    FunctorCode AcceptEnd(ConstFunctor &functor) const override;
    ///@}

    /**
     * Set/get methods for the flagShift
     */
    ///@{
    int GetFlagShift() const { return m_flagShift; }
    void SetFlagShift(int shiftVal) { m_flagShift = shiftVal; }
    ///@}

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
 * This class models a stem as a layer element part and has no direct MEI equivalent.
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
    std::string GetClassName() const override { return "flag"; }
    Object *Clone() const override { return new Flag(*this); }
    ///@}

    /** Override the method since alignment is required */
    bool HasToBeAligned() const override { return true; }

    char32_t GetFlagGlyph(data_STEMDIRECTION stemDir) const;

    Point GetStemUpSE(const Doc *doc, int staffSize, bool graceSize) const;
    Point GetStemDownNW(const Doc *doc, int staffSize, bool graceSize) const;

    //----------//
    // Functors //
    //----------//

    /**
     * Interface for class functor visitation
     */
    ///@{
    FunctorCode Accept(Functor &functor) override;
    FunctorCode Accept(ConstFunctor &functor) const override;
    FunctorCode AcceptEnd(Functor &functor) override;
    FunctorCode AcceptEnd(ConstFunctor &functor) const override;
    ///@}

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
 * This class models a bracket as a layer element part and has no direct MEI equivalent.
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
    std::string GetClassName() const override { return "tupletBracket"; }
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
     * Interface for class functor visitation
     */
    ///@{
    FunctorCode Accept(Functor &functor) override;
    FunctorCode Accept(ConstFunctor &functor) const override;
    FunctorCode AcceptEnd(Functor &functor) override;
    FunctorCode AcceptEnd(ConstFunctor &functor) const override;
    ///@}

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
 * This class models a tuplet num as a layer element part and has no direct MEI equivalent.
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
    std::string GetClassName() const override { return "tupletNum"; }
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
     * Interface for class functor visitation
     */
    ///@{
    FunctorCode Accept(Functor &functor) override;
    FunctorCode Accept(ConstFunctor &functor) const override;
    FunctorCode AcceptEnd(Functor &functor) override;
    FunctorCode AcceptEnd(ConstFunctor &functor) const override;
    ///@}

private:
    //
public:
    //
private:
    /** A pointer to the bracket with which the TupletNum is aligned (if any) */
    TupletBracket *m_alignedBracket;
};

} // namespace vrv

#endif
