// -*-c++-*-
/**
 * TuningsConcepts.h
 * Copyright Paul Walker, 2019-2020
 * Released under the MIT License. See LICENSE.md
 */

#ifndef __INCLUDE_TUNINGS_CONCEPTS_H
#define __INCLUDE_TUNINGS_CONCEPTS_H

#pragma once
#include <concepts>
#include <type_traits>
#include <fstream>

namespace Tunings
{
template <typename P>
concept StreamablePath = requires(const P &p) { std::ifstream{p}; };

template <typename P>
concept PathWithU8 = requires(const P &p) {
    { p.u8string() };
};

template <typename P>
concept PathWithStemU8 = requires(const P &p) {
    { p.filename().stem().u8string() };
};
} // namespace Tunings

#endif
