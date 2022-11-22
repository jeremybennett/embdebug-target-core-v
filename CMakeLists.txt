# CV32E40 Cmake configuration
#
# This file is part of the Embecosm Debug Server targets.
#
# Copyright (C) 2020 Embecosm Limited
# SPDX-License-Identifier: MIT
# ----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.4.3)

# Note that this will automatically set PROJECT_VERSION, PROJECT_VERSION_MAJOR,
# PROJECT_VERSION_MINOR and PROJECT_VERSION_PATCH variables.
project(cv32e40_embdebug_target VERSION 0.0.0 LANGUAGES C CXX)

# Option handling - debug server headers
if(CMAKE_PROJECT_NAME STREQUAL "embdebug")
  set (EMBDEBUG_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
else()
  option(EMBDEBUG_INCLUDE_DIR "Include directory for debug server headers")
  if(NOT EMBDEBUG_INCLUDE_DIR)
    message(SEND_ERROR "No include directory specified for debug server headers")
  endif()
endif()

# Configure compiler

# Set C/C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Configure compiler only if not being built as part of EmbDebug
if(NOT CMAKE_PROJECT_NAME STREQUAL "embdebug")
  option(TARGET_ENABLE_WERROR "Enable build failure if warnings triggered." OFF)
  include(cmake/compiler_utils.cmake)
  configure_compiler_defaults()

  enable_clang_format()
endif()

# EmbDebug target build
include_directories(${EMBDEBUG_INCLUDE_DIR})

# If these files aren't specified as GENERATED at this level then cmake tries
# to find them at configure time before they have been generated.
set(CV32E40_EMBDEBUG_TARGET_SRCS target/Cv32e40.cpp)

add_library(embdebug-target-cv32e40 SHARED ${CV32E40_EMBDEBUG_TARGET_SRCS})

set_target_properties(embdebug-target-cv32e40 PROPERTIES
                      VERSION ${cv32e40_embdebug_target_VERSION}
                      SOVERSION ${cv32e40_embdebug_target_VERSION_MAJOR})

install(TARGETS embdebug-target-cv32e40
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib)

message("
  " ${PROJECT_NAME} " version " ${cv32e40_embdebug_target_VERSION} "
  Prefix.......................: " ${CMAKE_INSTALL_PREFIX} "
  C++ Compiler.................: " ${CMAKE_CXX_COMPILER} "
  EmbDebug include directory...: " ${EMBDEBUG_INCLUDE_DIR} "
  ")
