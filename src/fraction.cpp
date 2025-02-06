/////////////////////////////////////////////////////////////////////////////
// Name:        fraction.cpp
// Author:      Laurent Pugin
// Created:     2024
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "fraction.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <math.h>
#include <numeric>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// Fraction
//----------------------------------------------------------------------------

Fraction::Fraction(int num, int denom)
{
    m_numerator = num;
    if (denom == 0) {
        LogDebug("Denominator cannot be zero.");
        denom = 1;
    }
    m_denominator = denom;
    this->Reduce();
}

Fraction::Fraction(data_DURATION duration)
{
    duration = vrv::DurationMin(duration, DURATION_2048);
    duration = vrv::DurationMax(duration, DURATION_maxima);
    int den = pow(2, (duration + 1));
    m_numerator = 8;
    m_denominator = den;
    this->Reduce();
}

Fraction Fraction::operator+(const Fraction &other) const
{
    int num = m_numerator * other.m_denominator + other.m_numerator * m_denominator;
    int denom = m_denominator * other.m_denominator;
    return Fraction(num, denom);
}

Fraction Fraction::operator-(const Fraction &other) const
{
    int num = m_numerator * other.m_denominator - other.m_numerator * m_denominator;
    int denom = m_denominator * other.m_denominator;
    return Fraction(num, denom);
}

Fraction Fraction::operator*(const Fraction &other) const
{
    int num = m_numerator * other.m_numerator;
    int denom = m_denominator * other.m_denominator;
    return Fraction(num, denom);
}

Fraction Fraction::operator/(const Fraction &other) const
{
    if (other.m_numerator == 0) {
        LogDebug("Cannot divide by zero.");
        return *this;
    }
    int num = m_numerator * other.m_denominator;
    int denom = m_denominator * other.m_numerator;
    return Fraction(num, denom);
}

Fraction Fraction::operator%(const Fraction &other) const
{
    if (other.m_numerator == 0) {
        LogDebug("Cannot divide by zero.");
        return *this;
    }

    // Convert both fractions to common denominator
    int commonDenominator = m_denominator * other.m_denominator;
    int leftNumerator = m_numerator * other.m_denominator;
    int rightNumerator = other.m_numerator * m_denominator;

    // Integer quotient
    int quotient = leftNumerator / rightNumerator;

    // Remainder as a fraction
    Fraction remainder(leftNumerator - quotient * rightNumerator, commonDenominator);
    remainder.Reduce();

    return remainder;
}

bool Fraction::operator==(const Fraction &other) const
{
    return m_numerator * other.m_denominator == other.m_numerator * m_denominator;
}

std::strong_ordering Fraction::operator<=>(const Fraction &other) const
{
    return m_numerator * other.m_denominator <=> other.m_numerator * m_denominator;
}

double Fraction::ToDouble() const
{
    return static_cast<double>(m_numerator) / m_denominator;
}

std::string Fraction::ToString() const
{
    return StringFormat("%d/%d", m_numerator, m_denominator);
}

void Fraction::Reduce()
{
    if (m_denominator < 0) { // Keep the denominator positive
        m_numerator = -m_numerator;
        m_denominator = -m_denominator;
    }
    const int gcdVal = std::gcd(m_numerator, m_denominator);
    if (gcdVal != 1) {
        m_numerator /= gcdVal;
        m_denominator /= gcdVal;
    }
}

std::pair<data_DURATION, Fraction> Fraction::ToDur() const
{
    if (m_numerator == 0) return { DURATION_NONE, 0 };

    int value = ceil(log2((double)m_denominator / (double)m_numerator * 8)) - 1;
    data_DURATION dur = static_cast<data_DURATION>(value);
    dur = vrv::DurationMax(DURATION_maxima, dur);
    dur = vrv::DurationMin(DURATION_2048, dur);

    Fraction remainder = *this - Fraction(dur);
    // Making sure we would not be triggering an inifite loop when looping over the remainder
    if ((remainder >= *this) || (remainder < 0)) remainder = 0;
    return { dur, remainder };
}

void Fraction::Reduce(int &numerator, int &denominator)
{
    Fraction fraction(numerator, denominator);
    numerator = fraction.GetNumerator();
    denominator = fraction.GetDenominator();
}

} // namespace vrv
