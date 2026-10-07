/////////////////////////////////////////////////////////////////////////////
// Name:        cursor.h
// Author:      Laurent Pugin
// Created:     2026
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CURSOR_H__
#define __VRV_CURSOR_H__

#include <stack>

//----------------------------------------------------------------------------

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

    int GetDrawingX() const override;

    void SetCursorAlignment(Alignment *alignment);
    void ResetCursorAlignment();

    void SetPosition(LayerElement *position) { m_position = position; }
    LayerElement *GetPosition() const { return m_position; }
    bool HasPosition() const { return (m_position); }

    void SetContainer(LayerElement *container) { m_container.push(container); }
    LayerElement *GetContainer() const { return m_container.top(); }
    bool HasContainer() const { return (!m_container.empty()); }

    bool HasAccid() const { return m_accid.HasAccid(); }
    data_ACCIDENTAL_WRITTEN GetAccid() const { return m_accid.GetAccid(); }
    void SetAccid(data_ACCIDENTAL_WRITTEN accid);
    bool IsAccidImplicit() const { return m_isAccidImplicit; }
    void SetAccidImplicit(bool isAccidImplicit);
    void SetAccidValue(const Accid *accid);
    std::pair<data_ACCIDENTAL_WRITTEN, data_ACCIDENTAL_GESTURAL> GetAccidValue();

    Accid *GetAccidElement() { return &m_accid; }

    bool IsRestMode() const { return m_restMode; }
    void SetRestMode(bool restMode);

    enum ChordMode : int8_t { CHORD_NONE = 0, NEW, EDIT_NEW, EDIT_EXISTING };

    ChordMode GetChordMode() const { return m_chordMode; }
    void SetChordMode(ChordMode chordMode);
    bool IsChordMode() const { return (m_chordMode != ChordMode::CHORD_NONE); }
    bool IsChordEditMode() const
    {
        return (m_chordMode == ChordMode::EDIT_NEW) || (m_chordMode == ChordMode::EDIT_EXISTING);
    }

    enum TieMode : int8_t { TIE_NONE = 0, TIE, COPY };

    TieMode GetTieMode() const { return m_tieMode; }
    void SetTieMode(TieMode tieMode);
    bool IsTieMode() const { return (m_tieMode != TieMode::TIE_NONE); }

    enum InputMode : int8_t { PITCH_FIRST = 0, DURATION_FIRST };

    InputMode GetInputMode() const { return m_inputMode; }
    void SetInputMode(InputMode inputMode);

    int GetYRelPitchC() const { return m_yRelPitchC; }
    void SetYRelPitchC(int yRelPitchC) { m_yRelPitchC = yRelPitchC; }

    bool Veto(const std::string &attribute) const;
    void OnSet(const std::string &attribute);

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
    /** yRel for pitch C */
    int m_yRelPitchC;
    /** A field indicating the tieMode status */
    TieMode m_tieMode;
    /** The cursor container (e.g., tuplet, graceGrp) */
    std::stack<LayerElement *> m_container;
};

} // namespace vrv

#endif
