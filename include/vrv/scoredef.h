/////////////////////////////////////////////////////////////////////////////
// Name:        scoredef.h
// Author:      Laurent Pugin
// Created:     2013/11/08
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SCOREDEF_H__
#define __VRV_SCOREDEF_H__

#include "atts_mei.h"
#include "atts_shared.h"
#include "drawinginterface.h"
#include "object.h"
#include "scoredefinterface.h"

namespace vrv {

class Clef;
class KeySig;
class Mensur;
class MeterSig;
class PgFoot;
class PgFoot2;
class PgHead;
class PgHead2;
class StaffGrp;
class StaffDef;

//----------------------------------------------------------------------------
// ScoreDefElement
//----------------------------------------------------------------------------

/**
 * This class is a base class for MEI scoreDef or staffDef elements.
 * It implements the ScoreDefInterface that implements the attribute classes
 * for clef, key signature, mensur and meter signature.
 * It also provides methods for checking if the scoreDef or staffDef has some
 * information about clef, key signature, etc. This information can be either
 * attributes (implemented) of the ScoreDefInterface or elements (not implemented).
 */
class ScoreDefElement : public Object, public ScoreDefInterface, public AttTyped {
public:
    /**
     * @name Constructors, destructors, and other standard methods.
     */
    ///@{
    ScoreDefElement(const std::string &classid);
    virtual ~ScoreDefElement();
    virtual void Reset();
    virtual ClassId GetClassId() const { return SCOREDEF_ELEMENT; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual ScoreDefInterface *GetScoreDefInterface() { return dynamic_cast<ScoreDefInterface *>(this); }
    ///@}

    /**
     * @name Methods for checking the presence of clef, key signature, etc. information and getting them.
     */
    ///@{
    bool HasClefInfo();
    bool HasKeySigInfo();
    bool HasMensurInfo();
    bool HasMeterSigInfo();
    ///@}

    /**
     * @name Get a copy of the clef, keysig, mensur and meterSig.
     * These methods create new objects (heap) that will need to be deleted.
     * They also convert attribute value objects to an object. For example,
     * if a staffDef has a @key.sig, the copy will be a KeySig object.
     * The conversion from attribute to element is performed in the appropriate
     * constructor of each corresponding class (Clef, KeySig, etc.)
     */
    ///@{
    Clef *GetClef();
    Clef *GetClefCopy();
    KeySig *GetKeySig();
    KeySig *GetKeySigCopy();
    Mensur *GetMensur();
    Mensur *GetMensurCopy();
    MeterSig *GetMeterSig();
    MeterSig *GetMeterSigCopy();
    ///@}

    //----------//
    // Functors //
    //----------//

private:
    //
public:
    //
private:
    //
};

//----------------------------------------------------------------------------
// ScoreDef
//----------------------------------------------------------------------------

/**
 * This class represents a MEI scoreDef.
 * It contains StaffGrp objects.
 */
class ScoreDef : public ScoreDefElement,
                 public ObjectListInterface,
                 public AttDistances,
                 public AttEndings,
                 public AttOptimization {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    ScoreDef();
    virtual ~ScoreDef();
    virtual Object *Clone() const { return new ScoreDef(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "ScoreDef"; }
    virtual ClassId GetClassId() const { return SCOREDEF; }
    ///@}

    virtual bool IsSupportedChild(Object *object);

    /**
     * Replace the scoreDef with the content of the newScoreDef.
     */
    void ReplaceDrawingValues(ScoreDef *newScoreDef);

    /**
     * Replace the corresponding staffDef with the content of the newStaffDef.
     * Looks for the staffDef with the same m_n (@n) and replaces the attribute set.
     * Attribute set is provided by the ScoreOrStaffDefInterface.
     */
    void ReplaceDrawingValues(StaffDef *newStaffDef);

    /**
     * Get the staffDef with number n (NULL if not found).
     */
    StaffDef *GetStaffDef(int n);

    /**
     * Return all the @n values of the staffDef in a scoreDef
     */
    std::vector<int> GetStaffNs();

    /**
     * Set the redraw flag to all staffDefs.
     * This is necessary at the beginning or when a scoreDef occurs.
     * Only true flags are applied, unless applyToAll is set to true.
     */
    void SetRedrawFlags(bool clef, bool keySig, bool mensur, bool meterSig, bool applyToAll);

    /**
     * @name Set and get the scoreDef drawing flags for clef, keysig and mensur.
     */
    ///@{
    bool DrawLabels() const { return m_drawLabels; }
    void SetDrawLabels(bool drawLabels) { m_drawLabels = drawLabels; }
    ///@}

    /**
     * @name Set and get the scoreDef drawing width.
     */
    ///@{
    int GetDrawingWidth() const { return m_drawingWidth; }
    void SetDrawingWidth(int drawingWidth);
    ///@}

    /**
     * @name Getters for running elements
     */
    ///@{
    PgFoot *GetPgFoot();
    PgFoot2 *GetPgFoot2();
    PgHead *GetPgHead();
    PgHead2 *GetPgHead2();
    ///@}

    /**
     * Return the maximum staff size in the scoreDef (100 if empty)
     */
    int GetMaxStaffSize();

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ConvertToPageBased
     */
    virtual int ConvertToPageBased(FunctorParams *functorParams);

    /**
     * See Object::CastOffSystems
     */
    virtual int CastOffSystems(FunctorParams *functorParams);

    /**

     * See Object::CastOffEncoding
     */
    virtual int CastOffEncoding(FunctorParams *functorParams);

protected:
    /**
     * Filter the flat list and keep only StaffDef elements.
     */
    virtual void FilterList(ArrayOfObjects *childList);

private:
    //
public:
    bool m_setAsDrawing;
    //
private:
    /** Flags for indicating whether labels need to be drawn or not */
    bool m_drawLabels;
    /** Store the drawing width (clef and key sig) of the scoreDef */
    int m_drawingWidth;
};

} // namespace vrv

#endif
