cmake_minimum_required(VERSION 3.8)

# Project name and type
project(InterfaceLib
    LANGUAGES CXX
    VERSION 1.0.0)

# CXX standard and extension config
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
    )

# Install library
set(IFCLIB_PUBLIC_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/include/AutoCounter.h
    )

include(CMakePackageConfigHelpers)
set(IFCLIB_CONFIG_FILE "${PROJECT_NAME}Config.cmake")
set(IFCLIB_CONFIG_VERSION_FILE "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake")
write_basic_package_version_file(
    ${IFCLIB_CONFIG_VERSION_FILE}
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY ExactVersion
)

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}
)

install(FILES
    ${IFCLIB_PUBLIC_HEADERS} DESTINATION include
)

install(FILES
    ${IFCLIB_CONFIG_VERSION_FILE} DESTINATION lib/cmake/${PROJECT_NAME}
)

install(EXPORT ${PROJECT_NAME}
    FILE ${IFCLIB_CONFIG_FILE} DESTINATION lib/cmake/${PROJECT_NAME}
)
