cmake_minimum_required(VERSION 3.8)

# Project name and type
project(StaticLib
    LANGUAGES CXX
    VERSION 1.0.0)

#find QT and QT targets
find_package(Qt5 CONFIG REQUIRED Core)

# needed for pthreads (required for std::thread in posix)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

# CXX standard and extension config
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(${PROJECT_NAME} STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Log.cpp
    )

target_compile_options(${PROJECT_NAME}
    PUBLIC -ldl -Wall
    $<$<BOOL:${CXX_SUPPORTS_CONCEPTS}>:-fconcepts>
    $<$<CONFIG:Release>:-Werror>
    )

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

target_link_Libraries(${PROJECT_NAME}
    Threads::Threads
    Qt5::Core
    )

install(TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION lib
    )
