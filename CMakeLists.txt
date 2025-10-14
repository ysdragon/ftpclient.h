cmake_minimum_required(VERSION 3.16)
project(ftpclient VERSION 0.0.1 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find libcurl
find_package(CURL REQUIRED)

# Header-only library interface
add_library(ftpclient INTERFACE)
target_include_directories(ftpclient INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(ftpclient INTERFACE ${CURL_LIBRARIES})

# Option to build examples
option(BUILD_EXAMPLES "Build example programs" ON)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Installation
install(FILES ftpclient.h DESTINATION include)
install(TARGETS ftpclient EXPORT ftpclientTargets)
install(EXPORT ftpclientTargets
    FILE ftpclientTargets.cmake
    NAMESPACE ftpclient::
    DESTINATION lib/cmake/ftpclient
)

# Create package config file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/ftpclientConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/ftpclientConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/ftpclientConfig.cmake"
    INSTALL_DESTINATION lib/cmake/ftpclient
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/ftpclientConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/ftpclientConfigVersion.cmake"
    DESTINATION lib/cmake/ftpclient
)

# Print configuration
message(STATUS "FTP Client Library Configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Build examples: ${BUILD_EXAMPLES}")
message(STATUS "  CURL found: ${CURL_FOUND}")
message(STATUS "  CURL version: ${CURL_VERSION_STRING}")
