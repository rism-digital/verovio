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
#include "staffdef.h"
#include "staffgrp.h"
#include "symbol.h"
#include "timeinterface.h"

namespace vrv {

enum DurExtreme { LONGEST = 0, SHORTEST };

//----------------------------------------------------------------------------
// Comparison
//----------------------------------------------------------------------------

class Comparison {

public:
    virtual bool operator()(const Object *object) = 0;
    // For classes that do a reverse comparison, return reversed result
    bool Result(bool comparison) { return (m_reverse) ? !comparison : comparison; }
    // Set reverse comparison.
    // This is possible only for Comparison classes that allow it explicitly
    void ReverseComparison()
    {
        assert(m_supportReverse);
        m_reverse = true;
    }

protected:
    // This is set to true in contructor of classes that allow it
    bool m_supportReverse = false;

private:
    // The flag indicating if a reverse comparison needs to be done
    bool m_reverse = false;
};

//----------------------------------------------------------------------------
// ClassIdComparison
//----------------------------------------------------------------------------

class ClassIdComparison : public Comparison {

public:
    ClassIdComparison(ClassId classId)
    {
        m_classId = classId;
        m_supportReverse = true;
    }

    bool operator()(const Object *object) override { return Result(this->MatchesType(object)); }

    ClassId GetType() { return m_classId; }

    bool MatchesType(const Object *object) { return (object->Is(m_classId)); }

protected:
    ClassId m_classId;
};

//----------------------------------------------------------------------------
// ClassIdsComparison
//----------------------------------------------------------------------------

class ClassIdsComparison : public Comparison {

public:
    ClassIdsComparison(const std::vector<ClassId> &classIds)
    {
        m_classIds = classIds;
        m_supportReverse = true;
    }

    bool operator()(const Object *object) override { return Result(this->MatchesType(object)); }

    bool MatchesType(const Object *object) { return (object->Is(m_classIds)); }

protected:
    std::vector<ClassId> m_classIds;
};

//----------------------------------------------------------------------------
// InterfaceComparison
//----------------------------------------------------------------------------

class InterfaceComparison : public Comparison {

public:
    InterfaceComparison(InterfaceId interfaceId) { m_interfaceId = interfaceId; }

    bool operator()(const Object *object) override
    {
        if (object->HasInterface(m_interfaceId)) {
            return true;
        }
        return false;
    }

protected:
    InterfaceId m_interfaceId;
};

//----------------------------------------------------------------------------
// ChildOfClassIdComparison
//----------------------------------------------------------------------------

class ChildOfClassIdComparison : public Comparison {

public:
    ChildOfClassIdComparison(ClassId classId) { m_classId = classId; }

    bool operator()(const Object *object) override
    {
        return (object->GetParent() && object->GetParent()->GetClassId() == m_classId);
    }

protected:
    ClassId m_classId;
};

//----------------------------------------------------------------------------
// PointingToComparison
//----------------------------------------------------------------------------

class PointingToComparison : public ClassIdComparison {

public:
    PointingToComparison(ClassId classId, const Object *pointingTo) : ClassIdComparison(classId)
    {
        m_pointingTo = pointingTo;
    }

    bool operator()(const Object *object) override
    {
        if (!MatchesType(object)) return false;
        const TimePointInterface *interface = object->GetTimePointInterface();
        if (!interface) return false;
        return (interface->GetStart() == m_pointingTo);
    }

protected:
    const Object *m_pointingTo;
};

//----------------------------------------------------------------------------
// SpanningToComparison
//----------------------------------------------------------------------------

class SpanningToComparison : public ClassIdComparison {

public:
    SpanningToComparison(ClassId classId, const Object *pointingTo) : ClassIdComparison(classId)
    {
        m_pointingTo = pointingTo;
    }

    bool operator()(const Object *object) override
    {
        if (!MatchesType(object)) return false;
        const TimeSpanningInterface *interface = object->GetTimeSpanningInterface();
        if (!interface) return false;
        return (interface->GetEnd() == m_pointingTo);
    }

protected:
    const Object *m_pointingTo;
};

//----------------------------------------------------------------------------
// IsEditorialElementComparison
//----------------------------------------------------------------------------

/**
 * This class evaluates if the object is an editorial element.
 */
class IsEditorialElementComparison : public Comparison {

public:
    IsEditorialElementComparison() : Comparison() { m_supportReverse = true; }

    bool operator()(const Object *object) override
    {
        if (object->IsEditorialElement()) return Result(true);
        return Result(false);
    }
};

//----------------------------------------------------------------------------
// IsEmptyComparison
//----------------------------------------------------------------------------

/**
 * This class evaluates if the object is of a certain ClassId and is empty
 */
class IsEmptyComparison : public ClassIdComparison {

public:
    IsEmptyComparison(ClassId classId) : ClassIdComparison(classId) { m_supportReverse = true; }

    bool operator()(const Object *object) override
    {
        if (!MatchesType(object)) return false;
        if (object->GetChildCount() == 0) {
            return Result(true);
        }
        else {
            return Result(false);
        }
    }
};

//----------------------------------------------------------------------------
// IsAttributeComparison
//----------------------------------------------------------------------------

/**
 * This class evaluates if the object is of a certain ClassId and is an attribute in the original MEI.
 */
class IsAttributeComparison : public ClassIdComparison {

public:
    IsAttributeComparison(ClassId classId) : ClassIdComparison(classId) {}

    bool operator()(const Object *object) override
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
    AttNIntegerComparison(ClassId classId, const int n) : ClassIdComparison(classId) { m_n = n; }

    void SetN(int n) { m_n = n; }

    bool operator()(const Object *object) override
    {
        if (!MatchesType(object)) return false;
        // This should not happen, but just in case
        if (!object->HasAttClass(ATT_NINTEGER)) return false;
        const AttNInteger *element = dynamic_cast<const AttNInteger *>(object);
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
    AttNIntegerAnyComparison(ClassId classId, std::vector<int> ns) : ClassIdComparison(classId) { m_ns = ns; }

    void SetNs(std::vector<int> ns) { m_ns = ns; }
    void AppendN(int n) { m_ns.push_back(n); }

    bool operator()(const Object *object) override
    {
        if (!MatchesType(object)) return false;
        // This should not happen, but just in case
        if (!object->HasAttClass(ATT_NINTEGER)) return false;
        const AttNInteger *element = dynamic_cast<const AttNInteger *>(object);
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
    AttNNumberLikeComparison(ClassId classId, const std::string n) : ClassIdComparison(classId) { m_n = n; }

    void SetN(std::string n) { m_n = n; }

    bool operator()(const Object *object) override
    {
        if (!MatchesType(object)) return false;
        // This should not happen, but just in case
        if (!object->HasAttClass(ATT_NNUMBERLIKE)) return false;
        const AttNNumberLike *element = dynamic_cast<const AttNNumberLike *>(object);
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
        m_extremeDur = (m_extremeType == LONGEST) ? -VRV_UNSET : VRV_UNSET;
    }

    bool operator()(const Object *object) override
    {
        if (!object->HasInterface(INTERFACE_DURATION)) return false;
        const DurationInterface *interface = dynamic_cast<const DurationInterface *>(object);
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
// AttVisibilityComparison
//----------------------------------------------------------------------------
/**
 * This class evaluates if the object is visible
 */
class AttVisibilityComparison : public ClassIdComparison {

public:
    AttVisibilityComparison(ClassId classId, data_BOOLEAN isVisible) : ClassIdComparison(classId)
    {
        m_isVisible = isVisible;
    };

    bool operator()(const Object *object) override
    {
        if (!MatchesType(object)) return false;
        if (!object->HasAttClass(ATT_VISIBILITY)) return false;
        const AttVisibility *visibility = dynamic_cast<const AttVisibility *>(object);
        assert(visibility);
        return (visibility->GetVisible() == m_isVisible);
    }

private:
    data_BOOLEAN m_isVisible;
};

//----------------------------------------------------------------------------
// AttFormeworkComparison
//----------------------------------------------------------------------------

/**
 * This class evaluates if the object is of a certain ClassId and has a @func of value func.
 */
class AttFormeworkComparison : public ClassIdComparison {

public:
    AttFormeworkComparison(ClassId classId, data_PGFUNC func) : ClassIdComparison(classId) { m_func = func; }

    bool operator()(const Object *object) override
    {
        if (!MatchesType(object)) return false;
        // This should not happen, but just in case
        if (!object->HasAttClass(ATT_FORMEWORK)) return false;
        const AttFormework *element = dynamic_cast<const AttFormework *>(object);
        assert(element);
        return (element->GetFunc() == m_func);
    }

private:
    data_PGFUNC m_func;
};

//----------------------------------------------------------------------------
// CrossAlignmentReferenceComparison
//----------------------------------------------------------------------------

/**
 * This class evaluates if alignment reference contains cross-staff elements
 */
class CrossAlignmentReferenceComparison : public ClassIdComparison {
public:
    CrossAlignmentReferenceComparison() : ClassIdComparison(ALIGNMENT_REFERENCE) {}

    bool operator()(const Object *object) override
    {
        if (!this->MatchesType(object)) return false;
        const AlignmentReference *ref = vrv_cast<const AlignmentReference *>(object);
        assert(ref);
        return ref->HasCrossStaffElements();
    }
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

    bool operator()(const Object *object) override
    {
        if (!MatchesType(object)) return false;
        const Alignment *alignment = vrv_cast<const Alignment *>(object);
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

    bool operator()(const Object *object) override
    {
        if (!MatchesType(object)) return false;
        const Measure *measure = vrv_cast<const Measure *>(object);
        assert(measure);
        return (measure->EnclosesTime(m_time) != VRV_UNSET);
    }

private:
    int m_time;
};

//----------------------------------------------------------------------------
// NoteOrRestOnsetOffsetComparison
//----------------------------------------------------------------------------

/**
 * This class evaluates if the object is a note being played at the given time.
 */
class NoteOrRestOnsetOffsetComparison : public ClassIdsComparison {

public:
    NoteOrRestOnsetOffsetComparison(const int time) : ClassIdsComparison({ NOTE, REST }) { m_time = time; }

    void SetTime(int time) { m_time = time; }

    bool operator()(const Object *object) override
    {
        if (!MatchesType(object)) return false;
        const DurationInterface *interface = object->GetDurationInterface();
        assert(interface);
        return ((m_time >= interface->GetRealTimeOnsetMilliseconds())
            && (m_time <= interface->GetRealTimeOffsetMilliseconds()));
    }

private:
    int m_time;
};

//----------------------------------------------------------------------------
// IDComparison
//----------------------------------------------------------------------------

/**
 * This class evaluates if the object is of a certain ClassId has a certain ID
 */
class IDComparison : public ClassIdComparison {

public:
    IDComparison(ClassId classId, const std::string &id) : ClassIdComparison(classId) { m_id = id; }

    void SetID(const std::string &id) { m_id = id; }

    bool operator()(const Object *object) override
    {
        if (!MatchesType(object)) return false;
        return (object->GetID() == m_id);
    }

private:
    std::string m_id;
};

//----------------------------------------------------------------------------
// VisibleStaffDefOrGrpObject
//----------------------------------------------------------------------------
/**
 * This class evaluates if the object is a visible StaffDef or StaffGrp.
 * As well it is able to exlude object passed to skip from the result set.
 */
class VisibleStaffDefOrGrpObject : public ClassIdsComparison {

public:
    VisibleStaffDefOrGrpObject() : ClassIdsComparison({ STAFFDEF, STAFFGRP }) {}

    void Skip(const Object *objectToExclude) { m_objectToExclude = objectToExclude; }

    bool operator()(const Object *object) override
    {
        if (object == m_objectToExclude || !ClassIdsComparison::operator()(object)) return false;

        if (object->Is(STAFFDEF)) {
            const StaffDef *staffDef = vrv_cast<const StaffDef *>(object);
            return staffDef && staffDef->GetDrawingVisibility() != OPTIMIZATION_HIDDEN;
        }

        const StaffGrp *staffGrp = vrv_cast<const StaffGrp *>(object);
        return staffGrp && staffGrp->GetDrawingVisibility() != OPTIMIZATION_HIDDEN;
    }

protected:
    const Object *m_objectToExclude;
};

//----------------------------------------------------------------------------
// Filters class
//----------------------------------------------------------------------------

/**
 * This class is used to store comparison filters and apply them when necessary
 */
class Filters {
public:
    enum class Type { AllOf, AnyOf };

public:
    Filters() = default;
    explicit Filters(const std::initializer_list<Comparison *> &comp)
    {
        std::copy(comp.begin(), comp.end(), std::back_inserter(m_filters));
    }

    void Add(Comparison *comp) { m_filters.push_back(comp); }
    void Clear() { m_filters.clear(); }
    void SetType(Type type) { m_type = type; }

    /**
     * Apply comparison filter based on the specified type
     */
    bool Apply(const Object *object) const
    {
        auto condition = [object](Comparison *iter) {
            // ignore any class comparison which does not match the object class
            ClassIdComparison *cmp = dynamic_cast<ClassIdComparison *>(iter);
            if (!cmp || (cmp->GetType() != object->GetClassId())) {
                return true;
            }
            return (*iter)(object);
        };
        switch (m_type) {
            case Type::AnyOf: {
                return std::any_of(m_filters.cbegin(), m_filters.cend(), condition);
            }
            case Type::AllOf:
            default: {
                return std::all_of(m_filters.cbegin(), m_filters.cend(), condition);
            }
        }
    }

    Filters &operator=(const std::initializer_list<Comparison *> &other)
    {
        m_filters.clear();
        std::copy(other.begin(), other.end(), std::back_inserter(m_filters));
        return *this;
    }

private:
    std::vector<Comparison *> m_filters;
    Type m_type = Type::AllOf;
};

} // namespace vrv

#endif
