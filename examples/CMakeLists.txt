cmake_minimum_required(VERSION 3.16)
project(ftpclient_examples C)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find libcurl
find_package(CURL REQUIRED)

# Include directory for the header-only library
set(FTPCLIENT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

# List of example executables
set(EXAMPLES
    basic
    directory
    progress
    ssl
)

# Create executables for each example
foreach(EXAMPLE ${EXAMPLES})
    add_executable(${EXAMPLE} ${EXAMPLE}.c)
    
    # Include directories
    target_include_directories(${EXAMPLE} PRIVATE 
        ${FTPCLIENT_INCLUDE_DIR}
        ${CURL_INCLUDE_DIRS}
    )
    
    # Link libraries
    target_link_libraries(${EXAMPLE} PRIVATE 
        ${CURL_LIBRARIES}
    )
    
    # Platform-specific settings
    if(WIN32)
        target_link_libraries(${EXAMPLE} PRIVATE ws2_32)
    endif()
    
    # Set output directory
    set_target_properties(${EXAMPLE} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endforeach()

# Optional: Add a custom target to build all examples
add_custom_target(examples DEPENDS ${EXAMPLES})

# Print configuration information
message(STATUS "FTP Client Examples Configuration:")
message(STATUS "  CURL Include: ${CURL_INCLUDE_DIRS}")
message(STATUS "  CURL Libraries: ${CURL_LIBRARIES}")
message(STATUS "  Build directory: ${CMAKE_BINARY_DIR}")
