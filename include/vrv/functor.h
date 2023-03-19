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
    FunctorBase(){};
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
     * Getters/Setters for the properties
     */
    ///@{
    Filters *GetFilters() { return m_filters; }
    void SetFilters(Filters *filters) { m_filters = filters; }
    bool VisibleOnly() const { return m_visibleOnly; }
    void SetVisibleOnly(bool visibleOnly) { m_visibleOnly = visibleOnly; }
    bool GetDirection() const { return m_direction; }
    void SetDirection(bool direction) { m_direction = direction; }
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
// MutableFunctor
//----------------------------------------------------------------------------

/**
 * This abstract class is the base class for all mutable functors.
 */
class MutableFunctor : public FunctorBase, public FunctorInterface {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    MutableFunctor(){};
    virtual ~MutableFunctor() = default;
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
    ConstFunctor(){};
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
class DocFunctor : public MutableFunctor {
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

} // namespace vrv

#endif // __VRV_FUNCTOR_H__
