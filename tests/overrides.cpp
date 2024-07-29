// Copyright 2024 Frederick Emmott
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>

#include <FredEmmott/magic_json_serialize_enum.hpp>

namespace {
enum class TestEnum {
  EncodeAsNull = -1,
  Foo = 0,
  Bar,
  EncodeAsOtherString,
};

FREDEMMOTT_MAGIC_JSON_SERIALIZE_ENUM_WITH_OVERRIDES(
  TestEnum,
  {
    {TestEnum::EncodeAsOtherString, "Hello, world"},
    {TestEnum::EncodeAsNull, nullptr},
  });
}// namespace

TEST_CASE("to_json for standard values") {
  nlohmann::json j;

  j = TestEnum::Foo;
  REQUIRE(j == "Foo");

  j = TestEnum::Bar;
  REQUIRE(j == "Bar");
}

TEST_CASE("to_json for overriden string value") {
  nlohmann::json j;

  j = TestEnum::EncodeAsOtherString;
  REQUIRE(j == "Hello, world");
}

TEST_CASE("to_json for overriden non-string value") {
  nlohmann::json j;

  j = TestEnum::EncodeAsNull;
  REQUIRE(j.is_null());
  REQUIRE(j == nullptr);
}

TEST_CASE("from_json standard values") {
  nlohmann::json j;

  j = "Foo";
  REQUIRE(j == TestEnum::Foo);

  j = "Bar";
  REQUIRE(j == TestEnum::Bar);
}

TEST_CASE("from_json for overriden string value") {
  const nlohmann::json j = "Hello, world";

  REQUIRE(j == TestEnum::EncodeAsOtherString);
  REQUIRE(j.get<TestEnum>() == TestEnum::EncodeAsOtherString);
}

TEST_CASE("from_json for overriden non-string value") {
  const nlohmann::json j = nullptr;

  REQUIRE(j == TestEnum::EncodeAsNull);
  REQUIRE(j.get<TestEnum>() == TestEnum::EncodeAsNull);
}

TEST_CASE("from_json with original value when an override exists") {
    // **NOT** testing `j == `, as that converts the RHS to JSON
    {
        const nlohmann::json j = "EncodeAsOtherString";
        REQUIRE(j.get<TestEnum>() == TestEnum::EncodeAsOtherString);
    }
    {
        const nlohmann::json j = "EncodeAsNull";
        REQUIRE(j.get<TestEnum>() == TestEnum::EncodeAsNull);
    }
}