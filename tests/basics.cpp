// Copyright 2024 Frederick Emmott
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>

#include <FredEmmott/magic_json_serialize_enum.hpp>

namespace MJSE = FredEmmott::magic_json_serialize_enum;

namespace {
enum class TestEnum {
  Foo = 0,
  Bar,
  Baz = -1,
};

FREDEMMOTT_MAGIC_JSON_SERIALIZE_ENUM(TestEnum);
}// namespace

TEST_CASE("to_json for valid values") {
  nlohmann::json j;

  j = TestEnum::Foo;
  REQUIRE(j == "Foo");

  j = TestEnum::Bar;
  REQUIRE(j == "Bar");

  j = TestEnum::Baz;
  REQUIRE(j == "Baz");
}

TEST_CASE("to_json for invalid values") {
  nlohmann::json j = static_cast<TestEnum>(-123);

  // Lowest value, not first; first is expected with nlohmann::json
  if (j == "Baz") {
    // Perhaps possible with reflection in C++26?
    SKIP(
      "it is not possible in C++23 or below to distinguish first vs lowest "
      "enum value");
  }

  // Actually the first value
  REQUIRE(j == "Foo");
  FAIL("Remove SKIP from this test");
}

TEST_CASE("from_json for valid values") {
  nlohmann::json j;

  j = "Foo";
  REQUIRE(j.get<TestEnum>() == TestEnum::Foo);

  j = "Bar";
  REQUIRE(j.get<TestEnum>() == TestEnum::Bar);

  j = "Baz";
  REQUIRE(j.get<TestEnum>() == TestEnum::Baz);
}

TEST_CASE("from_json for invalid values") {
  nlohmann::json j = "Not a valid value";
  const auto value = j.get<TestEnum>();

  // Lowest value, not first; first is expected with nlohmann::json
  if (value == TestEnum::Baz) {
    // Perhaps possible with reflection in C++26?
    SKIP(
      "it is not possible in C++23 or below to distinguish first vs lowest "
      "enum value");
  }

  // Actually the first value
  REQUIRE(value == TestEnum::Foo);
  FAIL("Remove SKIP from this test");
}