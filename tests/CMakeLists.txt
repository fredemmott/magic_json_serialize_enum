# Copyright 2024 Frederick Emmott
# SPDX-License-Identifier: MIT

set(ENABLE_CPP23_DEFAULT OFF)
if("cxx_std_23" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
    message(STATUS "CMake reports that the compiler supports C++23")
    set(ENABLE_CPP23_DEFAULT ON)
endif()


option(ENABLE_CPP17 "Enable C++17 build" ON)
option(ENABLE_CPP20 "Enable C++20 build" ON)
option(ENABLE_CPP23 "Enable C++23 build" ${ENABLE_CPP23_DEFAULT})

message(STATUS "Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")

set(COMPILER_ID "${CMAKE_CXX_COMPILER_ID}")
if(CMAKE_CXX_SIMULATE_ID)
  # eg clang-cl
  set(COMPILER_ID "${COMPILER_ID}-${CMAKE_CXX_SIMULATE_ID}")
endif()

function(add_test_executable TARGET CPP_VERSION)
    add_executable(
        ${TARGET}
        basics.cpp
        overrides.cpp
    )

    target_link_libraries(
        ${TARGET}
        PRIVATE
        Catch2::Catch2
        Catch2::Catch2WithMain
        nlohmann_json::nlohmann_json
        magic_enum::magic_enum
        "${PROJECT_NAME}::${PROJECT_NAME}"
    )

    set_property(
        TARGET ${TARGET}
        PROPERTY CXX_STANDARD "${CPP_VERSION}"
    )
endfunction()

foreach(CPP_VERSION IN ITEMS 17 20 23)
    set(ENABLE_VAR "ENABLE_CPP${CPP_VERSION}")
    set(ENABLED "$$ENABLE_VAR")
    if(NOT ENABLED)
        message(WARNING "SKIPPING tests for C++${CPP_VERSION}; enable with -D${ENABLE_VAR}=ON")
        continue()
    endif()
    message(STATUS "BUILDING tests for C++${CPP_VERSION}; disable with -D${ENABLE_VAR}=OFF")

    set(TARGET "mjse-tests_${COMPILER_ID}_cpp${CPP_VERSION}")
    add_test_executable("${TARGET}" "${CPP_VERSION}")
    add_test(
        NAME "${TARGET}"
        COMMAND "$<TARGET_FILE:${TARGET}>"
    )
endforeach()