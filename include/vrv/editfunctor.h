/////////////////////////////////////////////////////////////////////////////
// Name:        editfunctor.h
// Author:      Laurent Pugin
// Created:     2025
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_EDITFUNCTOR_H__
#define __VRV_EDITFUNCTOR_H__

#include "cursor.h"
#include "functor.h"

namespace vrv {

enum StaffInsert : int8_t { INSERT_ABOVE = 0, INSERT_BELOW };

enum StaffMove : int8_t { MOVE_UP = 0, MOVE_DOWN };

//----------------------------------------------------------------------------
// AddLayerFunctor
//----------------------------------------------------------------------------

/**
 * This adds a layer to a staff.
 */
class AddLayerFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AddLayerFunctor(int n);
    virtual ~AddLayerFunctor();

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    FunctorCode VisitDoc(Doc *doc) override;
    FunctorCode VisitStaff(Staff *staff) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    int m_n;
    int m_layerN;
};

//----------------------------------------------------------------------------
// DeleteLayerFunctor
//----------------------------------------------------------------------------

/**
 * This deletes a layer and updates the @n of the following layers.
 */
class DeleteLayerFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    DeleteLayerFunctor(int staffN, int layerN);
    virtual ~DeleteLayerFunctor();

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    FunctorCode VisitDoc(Doc *doc) override;
    FunctorCode VisitStaff(Staff *staff) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    int m_staffN;
    int m_layerN;
    bool m_canDelete;
};

//----------------------------------------------------------------------------
// AddStaffFunctor
//----------------------------------------------------------------------------

/**
 * This add a staff above or below.
 */
class AddStaffFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AddStaffFunctor(int n, StaffInsert staffInsert);
    virtual ~AddStaffFunctor();

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    FunctorCode VisitScore(Score *score) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitScoreDef(ScoreDef *scoreDef) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    StaffDef *m_staffDef;
    Staff *m_staff;
    int m_n;
    StaffInsert m_insert;
};

//----------------------------------------------------------------------------
// DeleteStaffFunctor
//----------------------------------------------------------------------------

/**
 * This deletes a staff and its corresponding staffDef.
 */
class DeleteStaffFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    DeleteStaffFunctor(int n);
    virtual ~DeleteStaffFunctor();

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    FunctorCode VisitScore(Score *score) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitScoreDef(ScoreDef *scoreDef) override;
    ///@}

    const std::set<std::string> &GetObjectsToDelete() const { return m_objectsToDelete; }

protected:
    //
private:
    void DeleteStaffDef(ScoreDef *scoreDef);

public:
    //
private:
    int m_n;
    bool m_scoreDefProcessed;
    std::set<std::string> m_objectsToDelete;
};

//----------------------------------------------------------------------------
// MoveStaffFunctor
//----------------------------------------------------------------------------

/**
 * This moves a staff up or down.
 */
class MoveStaffFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    MoveStaffFunctor(int n, StaffMove move);
    virtual ~MoveStaffFunctor();

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    FunctorCode VisitScore(Score *score) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitScoreDef(ScoreDef *scoreDef) override;
    ///@}

protected:
    //
private:
public:
    //
private:
    int m_n;
    int m_nTarget;
    StaffMove m_move;
};

//----------------------------------------------------------------------------
// UpdateStaffGrpFunctor
//----------------------------------------------------------------------------

/**
 * This updates the staffGrp structure and reorders the corresponding staves.
 */
class UpdateStaffGrpFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    UpdateStaffGrpFunctor(Doc *doc, ScoreDef *mainScoreDef, StaffGrp *staffGrp);
    virtual ~UpdateStaffGrpFunctor();

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitScoreDef(ScoreDef *scoreDef) override;
    ///@}

    bool IsSuccess() const { return m_success; }

protected:
    //
private:
    bool GetStaffRange(const Measure *measure, int &firstStaffIdx) const;
    bool UpdateScoreDef(ScoreDef *scoreDef, StaffGrp *staffGrp);

public:
    //
private:
    ScoreDef *m_mainScoreDef;
    StaffGrp *m_staffGrpTemplate;
    std::map<int, int> m_staffPositions;
    bool m_success;
};

//----------------------------------------------------------------------------
// ReorderStaffNFunctor
//----------------------------------------------------------------------------

/**
 * Reoder staff N
 */
class ReorderStaffNFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    ReorderStaffNFunctor();
    virtual ~ReorderStaffNFunctor();

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    FunctorCode VisitControlElement(ControlElement *controlElement) override;
    FunctorCode VisitLayerElement(LayerElement *layerElement) override;
    FunctorCode VisitScore(Score *score) override;
    FunctorCode VisitStaff(Staff *staff) override;
    FunctorCode VisitStaffDef(StaffDef *staffDef) override;
    ///@}

protected:
    //
private:
    void MapStaffIdent(AttStaffIdent *att);

public:
    //
private:
    std::map<int, int> m_mapping;
};

//----------------------------------------------------------------------------
// CursorFunctor
//----------------------------------------------------------------------------

/**
 * This set or reset the editor cursor.
 */
class CursorFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    CursorFunctor(Layer *layer, LayerElement *position);
    virtual ~CursorFunctor();

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    FunctorCode VisitLayer(Layer *layer) override;
    ///@}

    Cursor *GetCursor() { return m_cursor; }

protected:
    //
private:
    //
public:
    //
private:
    Layer *m_layer;
    LayerElement *m_position;
    Cursor *m_cursor;
    Cursor *m_previous;
};

//----------------------------------------------------------------------------
// SectionContextFunctor
//----------------------------------------------------------------------------

/**
 * This builds a tree of EditorTreeObject that represent the original score-based structure.
 */
class SectionContextFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    SectionContextFunctor(Object *object);
    virtual ~SectionContextFunctor() = default;

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    FunctorCode VisitObject(Object *object) override;
    FunctorCode VisitObjectEnd(Object *object) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    /** The current object in the tree */
    Object *m_current;
};

//----------------------------------------------------------------------------
// ScoreContextFunctor
//----------------------------------------------------------------------------

/**
 * This builds a tree of EditorTreeObject that represent the original score-based structure.
 */
class ScoreContextFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    ScoreContextFunctor(Object *object);
    virtual ~ScoreContextFunctor() = default;

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    FunctorCode VisitObject(Object *object) override;
    FunctorCode VisitObjectEnd(Object *object) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    enum TreeLevel { NOT_IN_SCORE = 0, TO_INCLUDE, INCLUDED };

    /** The current object in the tree */
    Object *m_current;
    /** A flag for building a score context or a section context */
    TreeLevel m_inScoreLevel;
};

} // namespace vrv

#endif // __VRV_SAVEFUNCTOR_H__
