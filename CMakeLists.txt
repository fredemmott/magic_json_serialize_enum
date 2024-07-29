# Copyright 2024 Frederick Emmott
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.27)

project("fredemmott_magic_json_serialize_enum" VERSION 0.0.1 LANGUAGES CXX C)

add_library("${PROJECT_NAME}" INTERFACE)
add_library("${PROJECT_NAME}::${PROJECT_NAME}" ALIAS "${PROJECT_NAME}")
target_include_directories(
  "${PROJECT_NAME}"
  INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

if (PROJECT_IS_TOP_LEVEL)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)

  include(CTest)

  add_subdirectory(third-party)
  add_subdirectory(tests)
endif()
