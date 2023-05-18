/////////////////////////////////////////////////////////////////////////////
// Name:        tuning.h
// Author:      Laurent Pugin
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TUNING_H__
#define __VRV_TUNING_H__

#include "atts_frettab.h"
#include "object.h"

namespace vrv {

//----------------------------------------------------------------------------
// Tuning
//----------------------------------------------------------------------------

/**
 * This class models the MEI <tuning> element.
 */
class Tuning : public Object, public AttCourseLog {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Tuning();
    virtual ~Tuning();
    Object *Clone() const override { return new Tuning(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "Tuning"; }
    ///@}

    /**
     * Add an element to a element.
     */
    bool IsSupportedChild(Object *object) override;

    /**
     * Return the line for a note according to tablature type.
     * Guitar, french and italian tablature: the line is based on the course.
     * German tablature: the line is based on the note's index in the note list.
     *
     * @param[in] course
     * @param[in] notationType
     * @param[in] lines
     * @param[in] listSize
     * @param[in] index - 0 based from the bottom of the chord
     * @return position in staff half lines
     */
    int CalcPitchPos(int course, data_NOTATIONTYPE notationType, int lines, int listSize, int index) const;

    /**
     * Calclate the MIDI pitch number for course/fret
     *
     * @param[in] course
     * @param[in] fret
     * @param[in] notationType, used to default tuning if not otherwise specified
     *
     * @return MIDI pitch
     */
    int CalcPitchNumber(int course, int fret, data_NOTATIONTYPE notationType) const;

    //----------//
    // Functors //
    //----------//

    /**
     * Interface for class functor visitation
     */
    ///@{
    FunctorCode Accept(MutableFunctor &functor) override;
    FunctorCode Accept(ConstFunctor &functor) const override;
    FunctorCode AcceptEnd(MutableFunctor &functor) override;
    FunctorCode AcceptEnd(ConstFunctor &functor) const override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif
