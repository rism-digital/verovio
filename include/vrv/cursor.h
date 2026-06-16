/////////////////////////////////////////////////////////////////////////////
// Name:        cursor.h
// Author:      Laurent Pugin
// Created:     2026
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CURSOR_H__
#define __VRV_CURSOR_H__

#include "elementpart.h"
#include "note.h"

namespace vrv {

//----------------------------------------------------------------------------
// Cursor
//----------------------------------------------------------------------------

/**
 * This class implements a curor for the editor
 */
class Cursor : public Note {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Cursor();
    virtual ~Cursor();
    Object *Clone() const override { return new Cursor(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "cursor"; }
    ///@}

    /**
     * Overriding CloneReset() method to be called after copy / assignment calls.
     */
    void CloneReset() override;

    bool IsCursor() const override { return true; }

    void SetCursorAlignment(Alignment *alignment);
    void ResetCursorAlignment();

    void SetPosition(LayerElement *position) { m_position = position; }
    LayerElement *GetPosition() const { return m_position; }
    bool HasPosition() const { return (m_position); }

    bool HasAccid() const { return m_accid.HasAccid(); }
    data_ACCIDENTAL_WRITTEN GetAccid() const { return m_accid.GetAccid(); }
    void SetAccid(data_ACCIDENTAL_WRITTEN accid);
    bool IsAccidImplicit() const { return m_isAccidImplicit; }
    void SetAccidImplicit(bool isAccidImplicit);

    Accid *GetAccidElement() { return &m_accid; }

    bool IsRestMode() const { return m_restMode; }
    void SetRestMode(bool restMode);

    enum ChordMode : int8_t { NONE = 0, NEW, EDIT_NEW, EDIT_EXISTING };

    ChordMode GetChordMode() const { return m_chordMode; }
    void SetChordMode(ChordMode chordMode);

    enum InputMode : int8_t { PITCH_FIRST = 0, DURATION_FIRST };

    InputMode GetInputMode() const { return m_inputMode; }
    void SetInputMode(InputMode inputMode);

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
    /** The reference postion */
    LayerElement *m_position;
    /** Accidentals and dots */
    Accid m_accid;
    /** A flag indicating if the accidental is implicit */
    bool m_isAccidImplicit;
    /** A flag indicating it is in restMode */
    bool m_restMode;
    /** A field indicating the chordMode status */
    ChordMode m_chordMode;
    /** A field indicating the inputMode */
    InputMode m_inputMode;
};

} // namespace vrv

#endif
