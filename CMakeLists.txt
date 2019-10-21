cmake_minimum_required(VERSION 3.8)

# Project name and type
project(CmakeProjectTemplate
    LANGUAGES CXX
    VERSION 1.0.0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
    "${CMAKE_CURRENT_SOURCE_DIR}/ProjectConfig.h.in"
    "${PROJECT_BINARY_DIR}/ProjectConfig.h"
    )

#find QT and QT targets
find_package(Qt5 CONFIG REQUIRED Core DBus)

# needed for pthreads (required for std::thread in posix)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

# CXX standard and extension config
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# CXX checks
option (WITH_CONCEPTS "Use C++ Concepts" ON)

if (WITH_CONCEPTS)
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG(-fconcepts CXX_SUPPORTS_CONCEPTS)
endif(WITH_CONCEPTS)

# Export compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(interface_lib)
add_subdirectory(static_lib)
add_subdirectory(dbus)

add_executable(${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/myclass.cpp
    )

target_compile_options(${PROJECT_NAME}
    PUBLIC -ldl -Wall
    $<$<BOOL:${CXX_SUPPORTS_CONCEPTS}>:-fconcepts>
    $<$<CONFIG:Release>:-Werror>
    )

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_BINARY_DIR}
    )

target_link_Libraries(${PROJECT_NAME}
    Threads::Threads
    Qt5::Core
    Qt5::DBus
    InterfaceLib
    StaticLib
    SharedLib
    )

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    )


# Enable CMake testing
enable_testing()

#define a macro to simplify adding tests
macro (do_test arg result)
    add_test (${PROJECT_NAME}${arg} ${PROJECT_NAME} ${arg})
    set_tests_properties (${PROJECT_NAME}${arg}
        PROPERTIES PASS_REGULAR_EXPRESSION ${result})
endmacro (do_test)

# do a bunch of result based tests
do_test (25 "Hello")
do_test (-25 "World")
