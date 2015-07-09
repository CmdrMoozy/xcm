cmake_minimum_required(VERSION 3.0.2)
project(xcm)

set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Note which type of build we're performing.
if(NOT CMAKE_BUILD_TYPE)
	message(STATUS "No CMAKE_BUILD_TYPE specified; defaulting to Release.")
	set(CMAKE_BUILD_TYPE "Release")
endif()
string(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWER)

# Find our dependencies.

find_package(XCB REQUIRED)

# Setup our compile flags.

include(${PROJECT_SOURCE_DIR}/cmake/SetFlags.cmake)
xcmSetCompileFlags()

if(CMAKE_BUILD_TYPE_LOWER MATCHES debug)
	add_definitions(-DXCM_DEBUG)
endif()

add_definitions(${LIBXCB_DEFINITIONS})

include_directories(3rdparty/include src)

# Define the libraries to link against.

set(xcm_LIBRARIES

	${LIBXCB_LIBRARIES}

)

# Add our targets.

enable_testing()

add_subdirectory(src/xcm)
add_subdirectory(src/xcmc)

if(CMAKE_BUILD_TYPE_LOWER MATCHES debug)
	add_subdirectory(src/test)
endif()
