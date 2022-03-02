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
     * Return the line for a the tuning and a given course and a notation type
     */
    int CalcPitchPos(int course, data_NOTATIONTYPE notationType, int lines) const;

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
