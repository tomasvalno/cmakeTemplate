cmake_minimum_required(VERSION 3.8)

# Apply CMake policies whenever suitable
# CMP0071: Apply AUTOMOC on GENERATED targets
foreach(pol CMP0071)
    if(POLICY ${pol})
        cmake_policy(SET ${pol} NEW)
    endif()
endforeach()

# Project name and type
project(SharedLib
    LANGUAGES CXX
    VERSION 1.0.0)

#find QT and QT targets
find_package(Qt5 CONFIG REQUIRED Core DBus)

# Needed for Yocto compilation, it doesn't use Qt5DBus_QDBUSXML2CPP_EXECUTABLE
if(NOT OE_QMAKE_QDBUSXML2CPP)
    set(OE_QMAKE_QDBUSXML2CPP ${Qt5DBus_QDBUSXML2CPP_EXECUTABLE})
endif(NOT OE_QMAKE_QDBUSXML2CPP)

# needed for pthreads (required for std::thread in posix)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

# CXX standard and extension config
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

qt5_wrap_cpp(MOCS
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/generated/EventAdaptor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/generated/EventProxy.h
    )

add_library(${PROJECT_NAME} SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/src/NodeInfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/generated/EventAdaptor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/generated/EventProxy.cpp
    ${MOCS}
    )

target_compile_options(${PROJECT_NAME}
    PUBLIC -ldl -Wall
    $<$<BOOL:${CXX_SUPPORTS_CONCEPTS}>:-fconcepts>
    $<$<CONFIG:Release>:-Werror>
    )

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/interface
    )

target_link_Libraries(${PROJECT_NAME}
    Threads::Threads
    Qt5::Core
    Qt5::DBus
    )

# Versioned so lib
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib
    )


# generate DBus interfaces
add_custom_command(
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/interface/Events.xml
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/CuxsQDbusTypes.h
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/interface/generated/EventAdaptor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/generated/EventAdaptor.cpp
    COMMAND mkdir -p ${CMAKE_CURRENT_SOURCE_DIR}/interface/generated
    COMMAND ${OE_QMAKE_QDBUSXML2CPP} ${CMAKE_CURRENT_SOURCE_DIR}/interface/Events.xml -i ${CMAKE_CURRENT_SOURCE_DIR}/interface/CuxsQDbusTypes.h -a ${CMAKE_CURRENT_SOURCE_DIR}/interface/generated/EventAdaptor
    )

add_custom_command(
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/interface/Events.xml
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/CuxsQDbusTypes.h
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/interface/generated/EventProxy.h
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/generated/EventProxy.cpp
    COMMAND mkdir -p ${CMAKE_CURRENT_SOURCE_DIR}/interface/generated
    COMMAND ${OE_QMAKE_QDBUSXML2CPP} ${CMAKE_CURRENT_SOURCE_DIR}/interface/Events.xml -i ${CMAKE_CURRENT_SOURCE_DIR}/interface/CuxsQDbusTypes.h -p ${CMAKE_CURRENT_SOURCE_DIR}/interface/generated/EventProxy
    )

add_custom_target(dbus_generator DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/generated/EventAdaptor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/generated/EventAdaptor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/generated/EventProxy.h
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/generated/EventProxy.cpp
    )

add_dependencies(${PROJECT_NAME} dbus_generator)

# Show these files on IDE
add_custom_target(dbus_interface SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/Events.xml
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/CuxsQDbusTypes.h
    )
