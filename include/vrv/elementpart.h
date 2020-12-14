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
    virtual void Reset();
    virtual std::string GetClassName() const { return "Dots"; }
    virtual ClassId GetClassId() const { return DOTS; }
    ///@}

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

    std::list<int> *GetDotLocsForStaff(Staff *staff);

    const MapOfDotLocs *GetMapOfDotLocs() const { return &m_dotLocsByStaff; }

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::CalcStem
     */
    // virtual int CalcStem(FunctorParams *functorParams);

    /**
     * Overwritten version of Save that avoids anything to be written
     */
    ///@{
    virtual int Save(FunctorParams *) { return FUNCTOR_CONTINUE; }
    virtual int SaveEnd(FunctorParams *) { return FUNCTOR_CONTINUE; }
    ///@}

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

    /**
     * See Object::ResetHorizontalAlignment
     */
    virtual int ResetHorizontalAlignment(FunctorParams *functorParams);

private:
    //
public:
    //
private:
    /**
     * A map of dot locations
     */
    MapOfDotLocs m_dotLocsByStaff;
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
    virtual void Reset();
    virtual std::string GetClassName() const { return "Flag"; }
    virtual ClassId GetClassId() const { return FLAG; }
    ///@}

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

    wchar_t GetSmuflCode(data_STEMDIRECTION stemDir);

    Point GetStemUpSE(Doc *doc, int staffSize, bool graceSize, wchar_t &code);
    Point GetStemDownNW(Doc *doc, int staffSize, bool graceSize, wchar_t &code);

    //----------//
    // Functors //
    //----------//

    /**
     * Overwritten version of Save that avoids anything to be written
     */
    ///@{
    virtual int Save(FunctorParams *) { return FUNCTOR_CONTINUE; }
    virtual int SaveEnd(FunctorParams *) { return FUNCTOR_CONTINUE; }
    ///@}

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

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
    virtual void Reset();
    virtual std::string GetClassName() const { return "TupletBracket"; }
    virtual ClassId GetClassId() const { return TUPLET_BRACKET; }
    ///@}

    /**
     * @name Setter and getter for darwing rel positions
     */
    ///@{
    int GetDrawingXRelLeft() { return m_drawingXRelLeft; }
    void SetDrawingXRelLeft(int drawingXRelLeft) { m_drawingXRelLeft = drawingXRelLeft; }
    int GetDrawingXRelRight() { return m_drawingXRelRight; }
    void SetDrawingXRelRight(int drawingXRelRight) { m_drawingXRelRight = drawingXRelRight; }
    ///@}

    /**
     * @name Setter and getter for darwing positions.
     * Takes into account:
     * - the position of the first and last element.
     * - the position of the beam if aligned with a beam.
     */
    ///@{
    int GetDrawingXLeft();
    int GetDrawingXRight();
    int GetDrawingYLeft();
    int GetDrawingYRight();
    ///@}

    /**
     * @name Setter and getter for the aligned num
     */
    ///@{
    TupletNum *GetAlignedNum() { return m_alignedNum; }
    void SetAlignedNum(TupletNum *alignedNum) { m_alignedNum = alignedNum; }
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * Overwritten version of Save that avoids anything to be written
     */
    ///@{
    virtual int Save(FunctorParams *) { return FUNCTOR_CONTINUE; }
    virtual int SaveEnd(FunctorParams *) { return FUNCTOR_CONTINUE; }
    ///@}

    /**
     * See Object::ResetHorizontalAlignment
     */
    virtual int ResetHorizontalAlignment(FunctorParams *functorParams);

    /**
     * See Object::ResetVerticalAlignment
     */
    virtual int ResetVerticalAlignment(FunctorParams *functorParams);

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
    virtual void Reset();
    virtual std::string GetClassName() const { return "TupletNum"; }
    virtual ClassId GetClassId() const { return TUPLET_NUM; }
    ///@}

    /**
     * @name Setter and getter for darwing positions.
     * Takes into account:
     * - the position of the first and last element.
     * - the position of the bracket if aligned with a bracket.
     * - the position of the beam if aligned with a beam.
     */
    ///@{
    int GetDrawingYMid();
    int GetDrawingXMid(Doc *doc = NULL);
    ///@}

    /**
     * @name Setter and getter for the aligned bracket
     */
    ///@{
    TupletBracket *GetAlignedBracket() { return m_alignedBracket; }
    void SetAlignedBracket(TupletBracket *alignedBracket);
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * Overwritten version of Save that avoids anything to be written
     */
    ///@{
    virtual int Save(FunctorParams *) { return FUNCTOR_CONTINUE; }
    virtual int SaveEnd(FunctorParams *) { return FUNCTOR_CONTINUE; }
    ///@}

    /**
     * See Object::ResetHorizontalAlignment
     */
    virtual int ResetHorizontalAlignment(FunctorParams *functorParams);

    /**
     * See Object::ResetVerticalAlignment
     */
    virtual int ResetVerticalAlignment(FunctorParams *functorParams);

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
    virtual void Reset();
    virtual std::string GetClassName() const { return "Stem"; }
    virtual ClassId GetClassId() const { return STEM; }
    ///@}

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

    /**
     * Add an element (only flag supported) to a stem.
     */
    virtual bool IsSupportedChild(Object *object);

    /**
     * @name Setter and getter for darwing stem direction and length
     */
    ///@{
    data_STEMDIRECTION GetDrawingStemDir() { return m_drawingStemDir; }
    void SetDrawingStemDir(data_STEMDIRECTION drawingStemDir) { m_drawingStemDir = drawingStemDir; }
    int GetDrawingStemLen() { return m_drawingStemLen; }
    void SetDrawingStemLen(int drawingStemLen) { m_drawingStemLen = drawingStemLen; }
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
    virtual void AdjustOverlappingLayers(Doc *doc, const std::vector<LayerElement *> &otherElements, bool &isUnison);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::CalcStem
     */
    virtual int CalcStem(FunctorParams *functorParams);

    /**
     * Overwritten version of Save that avoids anything to be written
     */
    ///@{
    virtual int Save(FunctorParams *) { return FUNCTOR_CONTINUE; }
    virtual int SaveEnd(FunctorParams *) { return FUNCTOR_CONTINUE; }
    ///@}

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

private:
    /**
     * Addjusts flag placement and stem length if they are crossing notehead or ledger lines
     */
    void AdjustFlagPlacement(Doc *doc, Flag *flag, int staffSize, int verticalCenter, int duration);

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
     * A flag indicating if a stem if virtual and should never be rendered.
     * Virtual stems are added to whole notes (and longer) for position calculation and
     * for supporting MEI @stem.mod
     */
    bool m_isVirtual;
};

} // namespace vrv

#endif
