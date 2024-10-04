# Copyright 2024 Frederick Emmott
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.22.1)

project("magic_json_serialize_enum" VERSION 0.0.1 LANGUAGES CXX C)

add_library("${PROJECT_NAME}" INTERFACE)
target_include_directories(
  "${PROJECT_NAME}"
  INTERFACE
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include>"
)

if(NOT PROJECT_IS_TOP_LEVEL)
  return()
endif()

install(DIRECTORY "include/FredEmmott" DESTINATION include)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)

add_subdirectory(third-party)
add_subdirectory(tests)

install(
  TARGETS "${PROJECT_NAME}"
  EXPORT "${PROJECT_NAME}_Targets"
)

install(
  EXPORT "${PROJECT_NAME}_Targets"
  FILE "${PROJECT_NAME}Targets.cmake"
  NAMESPACE "FredEmmott::"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake"
)
export(
  EXPORT "${PROJECT_NAME}_Targets"
  FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake"
  NAMESPACE "FredEmmott::"
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY ExactVersion)
configure_package_config_file(
  "PackageConfig.cmake.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION
  "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake"
)

install(
  FILES
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake"
)