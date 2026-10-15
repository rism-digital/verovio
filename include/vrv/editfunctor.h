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
