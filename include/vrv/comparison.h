/////////////////////////////////////////////////////////////////////////////
// Name:        comparison.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_COMPARISON_H__
#define __VRV_COMPARISON_H__

#include "artic.h"
#include "atts_shared.h"
#include "durationinterface.h"
#include "horizontalaligner.h"
#include "measure.h"
#include "note.h"
#include "object.h"

namespace vrv {

enum DurExtreme { LONGEST = 0, SHORTEST };

//----------------------------------------------------------------------------
// Comparison
//----------------------------------------------------------------------------

class Comparison {

public:
    virtual bool operator()(Object *object) = 0;
    virtual bool MatchesType(Object *object) = 0;
};

//----------------------------------------------------------------------------
// ClassIdComparison
//----------------------------------------------------------------------------

class ClassIdComparison : public Comparison {

public:
    ClassIdComparison(ClassId classId) { m_classId = classId; }

    virtual bool operator()(Object *object)
    {
        if (object->Is(m_classId)) {
            return true;
        }
        return false;
    }

    ClassId GetType() { return m_classId; }

    bool MatchesType(Object *object)
    {
        if (object->Is(m_classId)) {
            return true;
        }
        return false;
    }

protected:
    ClassId m_classId;
};

//----------------------------------------------------------------------------
// ClassIdsComparison
//----------------------------------------------------------------------------

class ClassIdsComparison : public Comparison {

public:
    ClassIdsComparison(const std::vector<ClassId> &classIds) { m_classIds = classIds; }

    virtual bool operator()(Object *object)
    {
        if (object->Is(m_classIds)) {
            return true;
        }
        return false;
    }

    bool MatchesType(Object *object) { return true; }

protected:
    std::vector<ClassId> m_classIds;
};

//----------------------------------------------------------------------------
// InterfaceComparison
//----------------------------------------------------------------------------

class InterfaceComparison : public Comparison {

public:
    InterfaceComparison(InterfaceId interfaceId) { m_interfaceId = interfaceId; }

    virtual bool operator()(Object *object)
    {
        if (object->HasInterface(m_interfaceId)) {
            return true;
        }
        return false;
    }

    bool MatchesType(Object *object) { return true; }

protected:
    InterfaceId m_interfaceId;
};

//----------------------------------------------------------------------------
// IsEditorialElementComparison
//----------------------------------------------------------------------------

/**
 * This class evaluates if the object is an editorial element.
 */
class IsEditorialElementComparison : public Comparison {

public:
    IsEditorialElementComparison() : Comparison() {}

    virtual bool operator()(Object *object)
    {
        if (object->IsEditorialElement()) return true;
        return false;
    }

    bool MatchesType(Object *object) { return true; }
};

//----------------------------------------------------------------------------
// IsEmptyComparison
//----------------------------------------------------------------------------

/**
 * This class evaluates if the object is of a certain ClassId and is empty
 */
class IsEmptyComparison : public ClassIdComparison {

public:
    IsEmptyComparison(ClassId ClassId, bool reverse = false) : ClassIdComparison(ClassId) { m_reverse = reverse; }

    virtual bool operator()(Object *object)
    {
        if (!MatchesType(object)) return false;
        if (object->GetChildCount() == 0) {
            if (!m_reverse) return true;
        }
        else {
            if (m_reverse) return true;
        }
        return false;
    }

private:
    bool m_reverse;
};

//----------------------------------------------------------------------------
// IsAttributeComparison
//----------------------------------------------------------------------------

/**
 * This class evaluates if the object is of a certain ClassId and is an attribute in the original MEI.
 */
class IsAttributeComparison : public ClassIdComparison {

public:
    IsAttributeComparison(ClassId ClassId) : ClassIdComparison(ClassId) {}

    virtual bool operator()(Object *object)
    {
        if (!MatchesType(object)) return false;
        if (object->IsAttribute()) return true;
        return false;
    }
};

//----------------------------------------------------------------------------
// AttNIntegerComparison
//----------------------------------------------------------------------------

/**
 * This class evaluates if the object is of a certain ClassId and has a @n of value n.
 */
class AttNIntegerComparison : public ClassIdComparison {

public:
    AttNIntegerComparison(ClassId ClassId, const int n) : ClassIdComparison(ClassId) { m_n = n; }

    void SetN(int n) { m_n = n; }

    virtual bool operator()(Object *object)
    {
        if (!MatchesType(object)) return false;
        // This should not happen, but just in case
        if (!object->HasAttClass(ATT_NINTEGER)) return false;
        AttNInteger *element = dynamic_cast<AttNInteger *>(object);
        assert(element);
        return (element->GetN() == m_n);
    }

private:
    int m_n;
};

//----------------------------------------------------------------------------
// AttNIntegerAnyComparison
//----------------------------------------------------------------------------

/**
 * This class evaluates if the object is of a certain ClassId and has a @n of value n.
 */
class AttNIntegerAnyComparison : public ClassIdComparison {

public:
    AttNIntegerAnyComparison(ClassId ClassId, std::vector<int> ns) : ClassIdComparison(ClassId) { m_ns = ns; }

    void SetNs(std::vector<int> ns) { m_ns = ns; }

    virtual bool operator()(Object *object)
    {
        if (!MatchesType(object)) return false;
        // This should not happen, but just in case
        if (!object->HasAttClass(ATT_NINTEGER)) return false;
        AttNInteger *element = dynamic_cast<AttNInteger *>(object);
        assert(element);
        return (std::find(m_ns.begin(), m_ns.end(), element->GetN()) != m_ns.end());
    }

private:
    std::vector<int> m_ns;
};

//----------------------------------------------------------------------------
// AttNNumberLikeComparison
//----------------------------------------------------------------------------

/**
 * This class evaluates if the object is of a certain ClassId and has a @n of value n.
 */
class AttNNumberLikeComparison : public ClassIdComparison {

public:
    AttNNumberLikeComparison(ClassId ClassId, const std::string n) : ClassIdComparison(ClassId) { m_n = n; }

    void SetN(std::string n) { m_n = n; }

    virtual bool operator()(Object *object)
    {
        if (!MatchesType(object)) return false;
        // This should not happen, but just in case
        if (!object->HasAttClass(ATT_NNUMBERLIKE)) return false;
        AttNNumberLike *element = dynamic_cast<AttNNumberLike *>(object);
        assert(element);
        return (element->GetN() == m_n);
    }

private:
    std::string m_n;
};

//----------------------------------------------------------------------------
// AttDurExtremeComparison
//----------------------------------------------------------------------------

/**
 * This class evaluates if the object the extreme duration so far
 * The object has to have a DurationInterface and to have a @dur.
 * The class can look for LONGEST or SHORTEST duration (Constructor)
 */
class AttDurExtremeComparison : public ClassIdComparison {

public:
    AttDurExtremeComparison(DurExtreme extremeType) : ClassIdComparison(OBJECT)
    {
        m_extremeType = extremeType;
        if (m_extremeType == LONGEST)
            m_extremeDur = -VRV_UNSET;
        else
            m_extremeDur = VRV_UNSET;
    }

    virtual bool operator()(Object *object)
    {
        if (!object->HasInterface(INTERFACE_DURATION)) return false;
        DurationInterface *interface = dynamic_cast<DurationInterface *>(object);
        assert(interface);
        if (interface->HasDur()) {
            if ((m_extremeType == LONGEST) && (interface->GetActualDur() < m_extremeDur)) {
                m_extremeDur = interface->GetActualDur();
                return true;
            }
            else if ((m_extremeType == SHORTEST) && (interface->GetActualDur() > m_extremeDur)) {
                m_extremeDur = interface->GetActualDur();
                return true;
            }
        }
        return false;
    }

private:
    int m_extremeDur;
    DurExtreme m_extremeType;
};

//----------------------------------------------------------------------------
// ArticPartTypeComparison
//----------------------------------------------------------------------------

/**
 * This class evaluates if the object is an Alignment of a certain type
 */
class ArticPartTypeComparison : public ClassIdComparison {

public:
    ArticPartTypeComparison(const ArticPartType type) : ClassIdComparison(ARTIC_PART) { m_type = type; }

    void SetType(ArticPartType type) { m_type = type; }

    virtual bool operator()(Object *object)
    {
        if (!MatchesType(object)) return false;
        ArticPart *articPart = dynamic_cast<ArticPart *>(object);
        assert(articPart);
        return (articPart->GetType() == m_type);
    }

private:
    ArticPartType m_type;
};

//----------------------------------------------------------------------------
// MeasureAlignerTypeComparison
//----------------------------------------------------------------------------

/**
 * This class evaluates if the object is an Alignment of a certain type
 */
class MeasureAlignerTypeComparison : public ClassIdComparison {

public:
    MeasureAlignerTypeComparison(const AlignmentType type) : ClassIdComparison(ALIGNMENT) { m_type = type; }

    void SetType(AlignmentType type) { m_type = type; }

    virtual bool operator()(Object *object)
    {
        if (!MatchesType(object)) return false;
        Alignment *alignment = dynamic_cast<Alignment *>(object);
        assert(alignment);
        return (alignment->GetType() == m_type);
    }

private:
    AlignmentType m_type;
};

//----------------------------------------------------------------------------
// MeasureOnsetOffsetComparison
//----------------------------------------------------------------------------

/**
 * This class evaluates if the object is a measure enclosing the given time
 */
class MeasureOnsetOffsetComparison : public ClassIdComparison {

public:
    MeasureOnsetOffsetComparison(const int time) : ClassIdComparison(MEASURE) { m_time = time; }

    void SetTime(int time) { m_time = time; }

    virtual bool operator()(Object *object)
    {
        if (!MatchesType(object)) return false;
        Measure *measure = dynamic_cast<Measure *>(object);
        assert(measure);
        return (measure->EnclosesTime(m_time) > 0);
    }

private:
    int m_time;
};

//----------------------------------------------------------------------------
// NoteOnsetOffsetComparison
//----------------------------------------------------------------------------

/**
 * This class evaluates if the object is of a certain ClassId and has a @n of value n.
 */
class NoteOnsetOffsetComparison : public ClassIdComparison {

public:
    NoteOnsetOffsetComparison(const int time) : ClassIdComparison(NOTE) { m_time = time; }

    void SetTime(int time) { m_time = time; }

    virtual bool operator()(Object *object)
    {
        if (!MatchesType(object)) return false;
        Note *note = dynamic_cast<Note *>(object);
        assert(note);
        return ((m_time >= note->GetRealTimeOnsetMilliseconds()) && (m_time <= note->GetRealTimeOffsetMilliseconds()));
    }

private:
    int m_time;
};

} // namespace vrv

#endif
