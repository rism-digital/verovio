/////////////////////////////////////////////////////////////////////////////
// Name:        tempo.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TEMPO_H__
#define __VRV_TEMPO_H__

#include "atts_midi.h"
#include "controlelement.h"
#include "textdirinterface.h"
#include "timeinterface.h"

namespace vrv {

class TextElement;

//----------------------------------------------------------------------------
// Tempo
//----------------------------------------------------------------------------

/**
 * This class is an interface for <tempo> elements at the measure level
 */
class Tempo : public ControlElement,
              public TextDirInterface,
              public TimeSpanningInterface,
              public AttExtender,
              public AttLang,
              public AttMidiTempo,
              public AttMmTempo {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method resets all attribute classes
     */
    ///@{
    Tempo();
    virtual ~Tempo();
    Object *Clone() const override { return new Tempo(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "tempo"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    TextDirInterface *GetTextDirInterface() override { return vrv_cast<TextDirInterface *>(this); }
    const TextDirInterface *GetTextDirInterface() const override { return vrv_cast<const TextDirInterface *>(this); }
    TimePointInterface *GetTimePointInterface() override { return vrv_cast<TimePointInterface *>(this); }
    const TimePointInterface *GetTimePointInterface() const override
    {
        return vrv_cast<const TimePointInterface *>(this);
    }
    TimeSpanningInterface *GetTimeSpanningInterface() override { return vrv_cast<TimeSpanningInterface *>(this); }
    const TimeSpanningInterface *GetTimeSpanningInterface() const override
    {
        return vrv_cast<const TimeSpanningInterface *>(this);
    }
    ///@}

    /**
     * Add an element (text, rend. etc.) to a tempo.
     * Only supported elements will be actually added to the child list.
     */
    bool IsSupportedChild(ClassId classId) override;

    /**
     * @name Getter and setter for the X drawing position
     */
    ///@{
    int GetDrawingXRelativeToStaff(int staffN) const;
    void SetDrawingXRelative(int staffN, int drawingX) { m_drawingXRels[staffN] = drawingX; }
    void ResetDrawingXRelative() { m_drawingXRels.clear(); }
    ///@}

    /**
     * See FloatingObject::IsExtenderElement
     */
    bool IsExtenderElement() const override { return GetExtender() == BOOLEAN_true; }

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

    //----------//
    //  Static  //
    //----------//

    /**
     * Calculate tempo from attMmTempo (@mm, @mm.unit, @mm.dots)
     */
    static double CalcTempo(const AttMmTempo *attMmTempo);

private:
    //
public:
    //
private:
    std::map<int, int> m_drawingXRels;
};

} // namespace vrv

#endif
