// Copyright 2024 Frederick Emmott
// SPDX-License-Identifier: MIT

#ifndef FREDEMMOTT_MAGIC_JSON_SERIALIZE_ENUM_HPP
#define FREDEMMOTT_MAGIC_JSON_SERIALIZE_ENUM_HPP

#include <algorithm>
#include <array>
#include <type_traits>

#if __has_include(<magic_enum/magic_enum.hpp>)
#include <magic_enum/magic_enum.hpp>
#elif __has_include(<magic_enum.hpp>)
#include <magic_enum.hpp>
#else
#error "Couldn't find magic_enum.hpp"
#endif
#include <nlohmann/json.hpp>

namespace FredEmmott::magic_json_serialize_enum {
template <class T>
using Pair = std::pair<T, nlohmann::json>;

#ifdef __cpp_concepts
// This can give nicer error messages than a static_assert, especially on MSVC
#define FREDEMMOTT_MJSE_REQUIRES(X) requires(X)
#else
#define FREDEMMOTT_MJSE_REQUIRES(X)
#endif

namespace detail {
template <class T, class Mapping>
FREDEMMOTT_MJSE_REQUIRES(std::is_enum_v<T>)
[[nodiscard]]
bool try_from_json(const nlohmann::json& j, T& v, const Mapping& mapping) {
  static_assert(std::is_enum_v<T>);
  auto it
    = std::find_if(mapping.begin(), mapping.end(), [&j](const auto& pair) {
        return j == nlohmann::json(std::get<1>(pair));
      });
  if (it != mapping.end()) {
    v = it->first;
    return true;
  }
  return false;
}

template <class T, class Mapping>
FREDEMMOTT_MJSE_REQUIRES(std::is_enum_v<T>)
[[nodiscard]]
bool try_to_json(nlohmann::json& j, const T v, const Mapping& mapping) {
  static_assert(std::is_enum_v<T>);
  auto it = std::find_if(mapping.begin(), mapping.end(), [v](const auto& pair) {
    return v == std::get<0>(pair);
  });
  if (it != mapping.end()) {
    j = it->second;
    return true;
  }
  return false;
}

// We target C++17, where std::to_array() is not available
template <class T, size_t N>
constexpr auto to_array(const T c_array[N]) {
  std::array<T, N> ret;
  for (size_t i = 0; i < N; ++i) {
    ret[i] = c_array[i];
  }
  return ret;
}
}// namespace detail

template <class T>
FREDEMMOTT_MJSE_REQUIRES(std::is_enum_v<T>)
[[nodiscard]]
bool try_from_json(const nlohmann::json& j, T& v) {
  static_assert(std::is_enum_v<T>);
  constexpr auto mapping = ::magic_enum::enum_entries<T>();
  return detail::try_from_json<T>(j, v, mapping);
}

template <class T>
FREDEMMOTT_MJSE_REQUIRES(std::is_enum_v<T>)
[[nodiscard]]
bool try_to_json(nlohmann::json& j, const T v) {
  static_assert(std::is_enum_v<T>);
  constexpr auto mapping = ::magic_enum::enum_entries<T>();
  return detail::try_to_json<T>(j, v, mapping);
}

template <class T>
FREDEMMOTT_MJSE_REQUIRES(std::is_enum_v<T>)
void from_json(const nlohmann::json& j, T& v) {
  static_assert(std::is_enum_v<T>);
  if (!try_from_json<T>(j, v)) {
    v = ::magic_enum::enum_value<T>(0);
  }
}

template <class T>
FREDEMMOTT_MJSE_REQUIRES(std::is_enum_v<T>)
void to_json(nlohmann::json& j, const T v) {
  static_assert(std::is_enum_v<T>);
  if (!try_to_json<T>(j, v)) {
    constexpr auto firstValue = ::magic_enum::enum_value<T>(0);
    j = ::magic_enum::enum_name<T>(firstValue);
  }
}
}// namespace FredEmmott::magic_json_serialize_enum

#define FREDEMMOTT_MAGIC_JSON_SERIALIZE_ENUM(ENUM_TYPE) \
  template<class BasicJsonType> \
  inline void from_json(const BasicJsonType& j, ENUM_TYPE& v) { \
    ::FredEmmott::magic_json_serialize_enum::from_json(j, v); \
  } \
  template<class BasicJsonType> \
  inline void to_json(BasicJsonType& j, const ENUM_TYPE v) { \
    ::FredEmmott::magic_json_serialize_enum::to_json(j, v); \
  }

#define FREDEMMOTT_MAGIC_JSON_SERIALIZE_ENUM_WITH_OVERRIDES(ENUM_TYPE, ...) \
  template<class BasicJsonType> \
  inline void to_json(BasicJsonType& j, const ENUM_TYPE v) { \
    namespace MJSE = ::FredEmmott::magic_json_serialize_enum; \
    static const MJSE::Pair<ENUM_TYPE> prepend[] = __VA_ARGS__; \
    static const auto prependCPP \
      = MJSE::detail::to_array<MJSE::Pair<ENUM_TYPE>, std::size(prepend)>(prepend); \
    if (MJSE::detail::try_to_json<ENUM_TYPE>(j, v, prependCPP)) { \
      return; \
    } \
    MJSE::to_json<ENUM_TYPE>(j, v); \
  } \
  template<class BasicJsonType> \
  inline void from_json(const BasicJsonType& j, ENUM_TYPE& v) { \
    namespace MJSE = ::FredEmmott::magic_json_serialize_enum; \
    static const MJSE::Pair<ENUM_TYPE> prepend[] = __VA_ARGS__; \
    static const auto prependCPP \
      = MJSE::detail::to_array<MJSE::Pair<ENUM_TYPE>, std::size(prepend)>(prepend); \
    if (MJSE::detail::try_from_json<ENUM_TYPE>(j, v, prependCPP)) { \
      return; \
    } \
    MJSE::from_json<ENUM_TYPE>(j, v); \
  }

#endif
