/////////////////////////////////////////////////////////////////////////////
// Name:        fraction.h
// Author:      Laurent Pugin
// Created:     2024
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FRACTION_H__
#define __VRV_FRACTION_H__

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Fraction
//----------------------------------------------------------------------------

class Fraction {

public:
    // Constructors - make them explicit to avoid type conversion
    explicit Fraction(int num = 0) : m_numerator(num), m_denominator(1) {}
    explicit Fraction(int num, int denom);
    explicit Fraction(data_DURATION duration);

    // Enable implicit conversion constructor for `int`
    template <typename T, typename = std::enable_if_t<std::is_same_v<T, int>>>
    Fraction(T num) : m_numerator(num), m_denominator(1)
    {
    }

    /** Addition operator */
    Fraction operator+(const Fraction &other) const;
    /** Subtraction operator */
    Fraction operator-(const Fraction &other) const;
    /** Multiplication operator */
    Fraction operator*(const Fraction &other) const;
    /** Division operator */
    Fraction operator/(const Fraction &other) const;
    /** Modulo operator */
    Fraction operator%(const Fraction &other) const;

    /** Equality operator */
    bool operator==(const Fraction &other) const;
    /** Ordering operator */
    std::strong_ordering operator<=>(const Fraction &other) const;

    /** Getters */
    int GetNumerator() const { return m_numerator; }
    int GetDenominator() const { return m_denominator; }

    /** Convert fraction to a double */
    double ToDouble() const;

    /** Convert fraction to a string */
    std::string ToString() const;

    /** Convert to data_DURATION and the remaining Fraction */
    std::pair<data_DURATION, Fraction> ToDur() const;

    //----------------//
    // Static methods //
    //----------------//

    /** Reduce the fraction represented by the two numbers */
    static void Reduce(int &numerator, int &denominator);

private:
    /** Reduce the fraction */
    void Reduce();

public:
    //
private:
    int m_numerator;
    int m_denominator;
};

} // namespace vrv

#endif
