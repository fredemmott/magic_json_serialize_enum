# magic_json_serialize_enum

This project is a small, header-only library that uses [magic_enum](https://github.com/Neargye/magic_enum) to provide concise serialization for enums with [JSON for Modern C++, a.k.a 'nlohmann/json'](https://github.com/nlohmann/json).

It provides functions to encode enums as their name (e.g. `MyEnum::Foo` becomes `"Foo"`), or other JSON values.

For example:

```C++
#include <FredEmmott/magic_json_serialize_enum.hpp>

enum class TestEnum {
  Foo = 0,
  Bar,
  Baz = -1,
};

FREDEMMOTT_MAGIC_JSON_SERIALIZE_ENUM(TestEnum);

void foo() {
    nlohmann::json j = TestEnum::Bar;
    assert(j == "Bar");
}
```

## WARNING - behavior change: values that are not in the enum

If an invalid value is specified:
- `NLOHMANN_JSON_SERIALIZE_ENUM()` will use the first value from the provided list
- this library will return the value with the lowest integer value

For example, after `nlohmann::json j = static_cast<TestEnum>(-123)`:
- if `NLOHMANN_JSON_SERIALIZE_ENUM()` has been used with `Foo` as the first value, the result will be `Foo`, as `Foo` is the first value
- if `FREDEMMOTT_MAGIC_JSON_SERIALIZE_ENUM()` has been used, the result will be `Baz`, as `Baz` is the lowest numbered value

This limitation is unavoidable with C++23 and below; it might be possible to fix it with reflection in C++26 or later.

## Overrides

You might want to use a different JSON value for a specific enum example; for example, perhaps you want to store `null` for an `Invalid` member:

```c++
enum class TestEnum {
    Foo = 0,
    Bar = 1,
    Invalid = -1,
};
FREDEMMOTT_MAGIC_JSON_SERIALIZE_ENUM_WITH_OVERRIDES(TestEnum, {
    { TestEnum::Invalid, null },
})
```

- Values without an override will keep their usual string representation
- The original string value is also accepted when converting from JSON, unless it is used in a different override

## Functions

The `FredEmmott::magic_json_serialize_enum` namespace contains several functions that can be used without the macros:

- `to_json(json&j, const T value)` and `from_json(const json&j, T& value)`: convert a JSON value to and from an enum value; if the input is invalid, the lowest-valued entry in the enum is used instead
- `try_to_json(json& j, const T value)` and `try_from_json(const json& j, T& value)`: convert a JSON value to and from an enum value; if the input is invalid, `false` is returned, and the output is not modified

These functions are useful if you need further customization beyond `FREDEMMOTT_MAGIC_JSON_SERIALIZE_ENUM_WITH_OVERRIDES`: you can define `from_json()` and `to_json()` in the containing ADL scope for your enum - just like for other types - then delegate to these functions. This is how the macros are implemented.

## Supported Platforms

This project aims to only use standard C++, with compatibility for C++17 through C++23; it is currently tested on:

| OS | Compiler | C++17 | C++20 | C++23 | Compiler Version |
|-|-|-|-|-|-|
| Linux | `clang++` | ✅ | ✅ | ✅  | 14.0.0 |
| Linux | `g++` | ✅ | ✅ | ✅  | 11.4.0 |
| Windows | `cl.exe` (MSVC) | ✅ | ✅ | ✅  | 19.40.33812.0 (MSVC 2022) |
| Windows | `clang++.exe` | ✅ | ✅ | ✅  | 18.1.8 |
| Windows | `clang-cl.exe` | ✅ | ✅ | ✅  | 18.1.8 |

'Compiler Version' is the version available on GitHub Actions as of 2024-07-29.

## Including this in your project

- add [magic_enum](https://github.com/Neargye/magic_enum) and [JSON for Modern C++, a.k.a 'nlohmann/json'](https://github.com/nlohmann/json) to using your favorite mechanism
- either add [the header](https://github.com/fredemmott/magic_json_serialize_enum/tree/main/include/FredEmmott) to your project directly via a submodule or just copying it, or if you use cmake, you can use `add_subdirectory(... EXCLUDE_FROM_ALL)` to make the `fredemmott_magic_json_serialize_enum::fredemmott_magic_json_serialize_enum` library target available

## Building this project

This is only needed if you are developing it or want to run tests - to use this library, you just need the header.

To build, install submodules, then build like any other CMake project, e.g.:

```
git submodule update --init --recursive
mkdir build
cd build
cmake ..
cmake --build . --Config Debug
ctest . -C Debug
```

## License

This project is [MIT-licensed](LICENSE).