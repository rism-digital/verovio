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
class ScoreDefElement : public Object, public ScoreDefInterface {
public:
    /**
     * @name Constructors, destructors, and other standard methods.
     */
    ///@{
    ScoreDefElement(std::string classid);
    virtual ~ScoreDefElement();
    virtual void Reset();
    virtual ClassId Is() { return SCOREDEF_ELEMENT; };
    ///@}

    virtual ScoreDefInterface *GetScoreDefInterface() { return vrv_cast2(ScoreDefInterface *)(this); }

    /**
     * @name Methods for checking the presence of clef, key signature, etc. information.
     * Look both at the attributes (e.g., @key.sig) and at child elements (not implemented)
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
    Clef *GetClefCopy();
    KeySig *GetKeySigCopy();
    Mensur *GetMensurCopy();
    MeterSig *GetMeterSigCopy();
    ///@}

private:
    /**
     * @name Methods for checking if clef info is available at the attribute level.
     */
    ///@{
    bool HasClefAttrInfo();
    bool HasKeySigAttrInfo();
    bool HasMensurAttrInfo();
    bool HasMeterSigAttrInfo();
    ///@}

    /**
     * @name Methods for checking if clef info is available at the element level.
     * To be implemented.
     */
    ///@{
    bool HasClefElementInfo();
    bool HasKeySigElementInfo();
    bool HasMensurElementInfo();
    bool HasMeterSigElementInfo();
    ///@}
};

//----------------------------------------------------------------------------
// ScoreDef
//----------------------------------------------------------------------------

/**
 * This class represents a MEI scoreDef.
 * It contains StaffGrp objects.
*/
class ScoreDef : public ScoreDefElement, public ObjectListInterface {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    ScoreDef();
    virtual ~ScoreDef();
    virtual void Reset();
    virtual std::string GetClassName() { return "ScoreDef"; };
    virtual ClassId Is() { return SCOREDEF; };
    ///@}

    void AddStaffGrp(StaffGrp *staffGrp);

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
     * Set the redraw flag to all staffDefs.
     * This is necessary at the beginning or when a scoreDef occurs.
     */
    void SetRedrawFlags(bool clef, bool keySig, bool mensur, bool meterSig, bool keySigCancellation);

    /**
     * @name Set and get the scoreDef drawing flags for clef, keysig and mensur.
     */
    ///@{
    bool DrawLabels() const { return m_drawLabels; };
    void SetDrawLabels(bool drawLabels) { m_drawLabels = drawLabels; };
    ///@}

    /**
     * @name Set and get the scoreDef drawing width.
     */
    ///@{
    int GetDrawingWidth() const { return m_drawingWidth; };
    void SetDrawingWidth(int drawingWidth);
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * Fill a page by adding systems with the appropriate length.
     * For ScoreDef, this means only moving them since their width is not taken into
     * account
     */
    virtual int CastOffSystems(ArrayPtrVoid *params);

protected:
    /**
     * Filter the list for a specific class.
     * For example, keep staffGrp for fast access.
     */
    virtual void FilterList(ListOfObjects *childList);

private:
    //
public:
    //
private:
    /** Flags for indicating whether labels need to be drawn or not */
    bool m_drawLabels;
    /** Store the drawing width (clef and key sig) of the scoreDef */
    int m_drawingWidth;
};

//----------------------------------------------------------------------------
// StaffGrp
//----------------------------------------------------------------------------

/**
 * This class represents a MEI staffGrp.
 * It contains StaffDef objects.
 */
class StaffGrp : public Object,
                 public ObjectListInterface,
                 public AttCommon,
                 public AttCommonPart,
                 public AttLabelsAddl,
                 public AttStaffgroupingsym,
                 public AttStaffGrpVis {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    StaffGrp();
    virtual ~StaffGrp();
    virtual Object *Clone() { return new StaffGrp(*this); };
    virtual void Reset();
    virtual std::string GetClassName() { return "StaffGrp"; };
    virtual ClassId Is() { return STAFFGRP; };
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    void AddStaffDef(StaffDef *staffDef);
    void AddStaffGrp(StaffGrp *staffGrp);
    ///@}

    //----------//
    // Functors //
    //----------//

protected:
    /**
     * Filter the list for a specific class.
     * For example, keep staffDef for fast access.
     */
    virtual void FilterList(ListOfObjects *childList);

private:
    //
public:
    //
private:
};

//----------------------------------------------------------------------------
// StaffDef
//----------------------------------------------------------------------------

/**
 * This class represents a MEI staffDef.
 */
class StaffDef : public ScoreDefElement,
                 public StaffDefDrawingInterface,
                 public AttCommon,
                 public AttCommonPart,
                 public AttLabelsAddl,
                 public AttNotationtype,
                 public AttScalable,
                 public AttStaffDefVis {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    StaffDef();
    virtual ~StaffDef();
    virtual Object *Clone() { return new StaffDef(*this); };
    virtual void Reset();
    virtual std::string GetClassName() { return "StaffDef"; };
    virtual ClassId Is() { return STAFFDEF; };
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * Set the current / drawing clef, key signature, etc. to the StaffDef
     * Called form ScoreDef::ReplaceDrawingValues.
     * See implementation and Object::ReplaceDrawingValuesInStaffDef for the parameters.
     */
    virtual int ReplaceDrawingValuesInStaffDef(ArrayPtrVoid *params);

    /**
     * Set drawing flags for the StaffDef for indicating whether clefs, keysigs, etc. need
     * to be redrawn.
     * This typically occurs when a new System or a new  ScoreDef is encountered.
     * See implementation and Object::SetStaffDefRedrawFlags for the parameters.
     */
    virtual int SetStaffDefRedrawFlags(ArrayPtrVoid *params);

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
