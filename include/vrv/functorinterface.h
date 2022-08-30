/////////////////////////////////////////////////////////////////////////////
// Name:        functorinterface.h
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FUNCTOR_INTERFACE_H__
#define __VRV_FUNCTOR_INTERFACE_H__

#include "vrvdef.h"

namespace vrv {

class Doc;
class Object;

//----------------------------------------------------------------------------
// FunctorInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for functors based on a visitor pattern.
 * It is not an abstract class but should not be instanciated directly.
 */
class FunctorInterface {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    FunctorInterface(){};
    virtual ~FunctorInterface() = default;
    ///@}

    /**
     * @name Visit object and doc
     */
    ///@{
    virtual FunctorCode VisitObject(Object *object) { return FUNCTOR_CONTINUE; }
    virtual FunctorCode VisitObjectEnd(Object *object) { return FUNCTOR_CONTINUE; }
    virtual FunctorCode VisitDoc(Doc *doc);
    virtual FunctorCode VisitDocEnd(Doc *doc);
    ///@}

private:
    //
public:
    //
private:
};

//----------------------------------------------------------------------------
// ConstFunctorInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for const functors based on a visitor pattern.
 * It is not an abstract class but should not be instanciated directly.
 */
class ConstFunctorInterface {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    ConstFunctorInterface(){};
    virtual ~ConstFunctorInterface() = default;
    ///@}

    /**
     * @name Visit object and doc
     */
    ///@{
    virtual FunctorCode VisitObject(const Object *object) { return FUNCTOR_CONTINUE; }
    virtual FunctorCode VisitObjectEnd(const Object *object) { return FUNCTOR_CONTINUE; }
    virtual FunctorCode VisitDoc(const Doc *doc);
    virtual FunctorCode VisitDocEnd(const Doc *doc);
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif // __VRV_FUNCTOR_INTERFACE_H__
