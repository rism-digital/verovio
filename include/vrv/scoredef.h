/////////////////////////////////////////////////////////////////////////////
// Name:        scoredef.h
// Author:      Laurent Pugin
// Created:     2013/11/08
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SCOREDEF_H__
#define __VRV_SCOREDEF_H__

#include "atts_gestural.h"
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
class MeterSigGrp;
class PgFoot;
class PgHead;
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
    ScoreDefElement();
    ScoreDefElement(ClassId classId);
    virtual ~ScoreDefElement();
    void Reset() override;
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    ScoreDefInterface *GetScoreDefInterface() override { return vrv_cast<ScoreDefInterface *>(this); }
    const ScoreDefInterface *GetScoreDefInterface() const override { return vrv_cast<const ScoreDefInterface *>(this); }
    ///@}

    /**
     * @name Methods for checking the presence of clef, key signature, etc. information and getting them.
     */
    ///@{
    bool HasClefInfo(int depth = 1) const;
    bool HasKeySigInfo(int depth = 1) const;
    bool HasMensurInfo(int depth = 1) const;
    bool HasMeterSigInfo(int depth = 1) const;
    bool HasMeterSigGrpInfo(int depth = 1) const;
    ///@}

    /**
     * @name Get a copy of the clef, keysig, mensur and meterSig.
     * These methods create new objects (heap) that will need to be deleted.
     * They also convert attribute value objects to an object. For example,
     * if a staffDef has a \@key.sig, the copy will be a KeySig object.
     * The conversion from attribute to element is performed in the appropriate
     * constructor of each corresponding class (Clef, KeySig, etc.)
     */
    ///@{
    Clef *GetClef();
    const Clef *GetClef() const;
    Clef *GetClefCopy() const;
    KeySig *GetKeySig();
    const KeySig *GetKeySig() const;
    KeySig *GetKeySigCopy() const;
    Mensur *GetMensur();
    const Mensur *GetMensur() const;
    Mensur *GetMensurCopy() const;
    MeterSig *GetMeterSig();
    const MeterSig *GetMeterSig() const;
    MeterSig *GetMeterSigCopy() const;
    MeterSigGrp *GetMeterSigGrp();
    const MeterSigGrp *GetMeterSigGrp() const;
    MeterSigGrp *GetMeterSigGrpCopy() const;
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
                 public AttOptimization,
                 public AttTimeBase,
                 public AttTuning {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    ScoreDef();
    virtual ~ScoreDef();
    Object *Clone() const override { return new ScoreDef(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "scoreDef"; }
    ///@}

    /**
     * Check if a object is allowed as child.
     */
    bool IsSupportedChild(ClassId classId) override;

    /**
     * Additional check when adding a child.
     */
    bool AddChildAdditionalCheck(Object *child) override;

    /**
     * Return an order for the given ClassId.
     */
    int GetInsertOrderFor(ClassId classId) const override;

    /**
     * Replace the scoreDef with the content of the newScoreDef.
     */
    void ReplaceDrawingValues(const ScoreDef *newScoreDef);

    /**
     * Replace the corresponding staffDef with the content of the newStaffDef.
     * Looks for the staffDef with the same m_n (@n) and replaces the attribute set.
     * Attribute set is provided by the ScoreOrStaffDefInterface.
     */
    void ReplaceDrawingValues(const StaffDef *newStaffDef);

    /**
     * Replace the corresponding staffGrp with the labels of the newStaffGrp.
     * Looks for the staffGrp with the same m_n (@n) and replaces label child
     */
    void ReplaceDrawingLabels(const StaffGrp *newStaffGrp);

    /**
     * Replace the staffDef score attributes with the ones currently set as drawing values.
     * Used when initializing a selection and adding a temporary score for it.
     */
    void ResetFromDrawingValues();

    /**
     * Get the staffDef with number n (NULL if not found).
     */
    ///@{
    StaffDef *GetStaffDef(int n);
    const StaffDef *GetStaffDef(int n) const;
    ///@}

    /**
     * Get the staffGrp with number n (NULL if not found).
     */
    ///@{
    StaffGrp *GetStaffGrp(const std::string &n);
    const StaffGrp *GetStaffGrp(const std::string &n) const;
    ///@}

    /**
     * Return all the \@n values of the staffDef in a scoreDef
     */
    std::vector<int> GetStaffNs() const;

    /**
     * Set the redraw flag to all staffDefs.
     * This is necessary at the beginning or when a scoreDef occurs.
     */
    void SetRedrawFlags(int redrawFlags);

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
     * @name Set and get the drawing label width.
     */
    ///@{
    int GetDrawingLabelsWidth() const { return m_drawingLabelsWidth; }
    void SetDrawingLabelsWidth(int width);
    void ResetDrawingLabelsWidth() { m_drawingLabelsWidth = 0; }
    ///@}

    /**
     * @name Getters for running elements
     */
    ///@{
    PgFoot *GetPgFoot(data_PGFUNC func);
    const PgFoot *GetPgFoot(data_PGFUNC func) const;
    PgHead *GetPgHead(data_PGFUNC func);
    const PgHead *GetPgHead(data_PGFUNC func) const;
    ///@}

    /**
     * Return the maximum staff size in the scoreDef (100 if empty)
     */
    int GetMaxStaffSize() const;

    bool IsSectionRestart() const;

    /**
     * @return True if a system start line will be drawn
     */
    bool HasSystemStartLine() const;

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

protected:
    /**
     * Filter the flat list and keep only StaffDef elements.
     */
    void FilterList(ListOfConstObjects &childList) const override;

private:
    //
public:
    bool m_setAsDrawing;
    bool m_insertScoreDef;

private:
    /** Flags for indicating whether labels need to be drawn or not */
    bool m_drawLabels;
    /** Store the drawing width (clef and key sig) of the scoreDef */
    int m_drawingWidth;
    /** Store the label drawing width of the scoreDef */
    int m_drawingLabelsWidth;
};

} // namespace vrv

#endif
