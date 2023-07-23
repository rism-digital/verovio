/////////////////////////////////////////////////////////////////////////////
// Name:        functor.h
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FUNCTOR_H__
#define __VRV_FUNCTOR_H__

#include "comparison.h"
#include "functorinterface.h"
#include "vrvdef.h"

namespace vrv {

class Doc;

//----------------------------------------------------------------------------
// FunctorBase
//----------------------------------------------------------------------------

/**
 * This abstract class contains functionality that is common to all functors.
 */
class FunctorBase {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    FunctorBase() {}
    virtual ~FunctorBase() = default;
    ///@}

    /**
     * Getter/Setter for the functor code which controls traversal
     */
    ///@{
    FunctorCode GetCode() const { return m_code; }
    void ResetCode() { m_code = FUNCTOR_CONTINUE; }
    void SetCode(FunctorCode code) { m_code = code; }
    ///@}

    /**
     * Getter/Setter for the visibility
     */
    ///@{
    bool VisibleOnly() const { return m_visibleOnly; }
    void SetVisibleOnly(bool visibleOnly) { m_visibleOnly = visibleOnly; }
    ///@}

    /**
     * Getters/Setters for the filters and direction
     * Here setters return the previous value
     */
    ///@{
    Filters *GetFilters() const { return m_filters; }
    Filters *SetFilters(Filters *filters)
    {
        Filters *previous = m_filters;
        m_filters = filters;
        return previous;
    }
    bool GetDirection() const { return m_direction; }
    bool SetDirection(bool direction)
    {
        bool previous = m_direction;
        m_direction = direction;
        return previous;
    }
    ///@}

    /**
     * Return true if the functor implements the end interface
     */
    virtual bool ImplementsEndInterface() const = 0;

private:
    //
public:
    //
private:
    // The functor code
    FunctorCode m_code = FUNCTOR_CONTINUE;
    // The filters
    Filters *m_filters = NULL;
    // Visible only flag
    bool m_visibleOnly = true;
    // Direction
    bool m_direction = FORWARD;
};

//----------------------------------------------------------------------------
// Functor
//----------------------------------------------------------------------------

/**
 * This abstract class is the base class for all mutable functors.
 */
class Functor : public FunctorBase, public FunctorInterface {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    Functor() {}
    virtual ~Functor() = default;
    ///@}

private:
    //
public:
    //
private:
    //
};

//----------------------------------------------------------------------------
// ConstFunctor
//----------------------------------------------------------------------------

/**
 * This abstract class is the base class for all const functors.
 */
class ConstFunctor : public FunctorBase, public ConstFunctorInterface {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    ConstFunctor() {}
    virtual ~ConstFunctor() = default;
    ///@}

private:
    //
public:
    //
private:
    //
};

//----------------------------------------------------------------------------
// DocFunctor
//----------------------------------------------------------------------------

/**
 * This abstract class is the base class for all mutable functors that need access to the document.
 */
class DocFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    DocFunctor(Doc *doc) { m_doc = doc; }
    virtual ~DocFunctor() = default;
    ///@}

    /*
     * Getter for the document
     */
    Doc *GetDoc() { return m_doc; }

private:
    //
public:
    //
protected:
    //
    Doc *m_doc;

private:
    //
};

//----------------------------------------------------------------------------
// DocConstFunctor
//----------------------------------------------------------------------------

/**
 * This abstract class is the base class for all const functors that need access to the document.
 */
class DocConstFunctor : public ConstFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    DocConstFunctor(const Doc *doc) { m_doc = doc; }
    virtual ~DocConstFunctor() = default;
    ///@}

    /*
     * Getter for the document
     */
    const Doc *GetDoc() const { return m_doc; }

private:
    //
public:
    //
protected:
    //
    const Doc *m_doc;

private:
    //
};

//----------------------------------------------------------------------------
// CollectAndProcess
//----------------------------------------------------------------------------

/**
 * This class is a mixin for all functors that require two step processing:
 * (1) Collecing data. (2) Processing data.
 */
class CollectAndProcess {
protected:
    /**
     * @name Constructors, destructors
     */
    ///@{
    CollectAndProcess() = default;
    ~CollectAndProcess() = default;
    ///@}

public:
    /**
     * Check and switch the current phase.
     */
    ///@{
    bool IsCollectingData() const { return !m_processingData; }
    bool IsProcessingData() const { return m_processingData; }
    void SetDataCollectionCompleted() { m_processingData = true; }
    ///@}

private:
    //
public:
    //
protected:
    //
private:
    // Indicates the current phase: collecting vs processing data
    bool m_processingData = false;
};

} // namespace vrv

#endif // __VRV_FUNCTOR_H__
