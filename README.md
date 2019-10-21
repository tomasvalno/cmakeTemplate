# CMake Project Template

Target-oriented CMake project template for ease of use and build system maintainability.

## Quickstart

Clone the repo and modify the targets/modules at need. Then change the remote and push to your project repository.

### Prerequisites

The template needs `CMake`, at least version 3.8, `Qt5` and a threads library (`pthread` preferred).

### Caveats

There are several issues regarding the version of CMake used. There are several issues in CMake 3.8.x (the version used in Yocto) that are not present in CMake 3.10.x (the version used in Ubuntu).

Versions 3.8.x have an issue finding the thread library, regarding the way the compiler flag is tested by TRY_RUN(). You will see the following error:

```
-- Check if compiler accepts -pthread
CMake Error: TRY_RUN() invoked in cross-compiling mode, please set the following cache variables appropriately:
        THREADS_PTHREAD_ARG (advanced)
For details see /home/eduardo/Dev/Projects/CmakeProjectTemplate/build/TryRunResults.cmake
-- Check if compiler accepts -pthread - no
```

To fix it, you must invoke CMake with the `-DTHREADS_PTHREAD_ARG=2` argument:

```
cmake -DCMAKE_TOOLCHAIN_FILE=/opt/hades/2.4.3/sysroots/x86_64-hadessdk-linux/usr/share/cmake/OEToolchainConfig.cmake -DCMAKE_BUILD_TYPE=Release -DTHREADS_PTHREAD_ARG=2 ..
```

Another issue in CMake 3.8.x regarding the Qt MOC files is that CMake doesn't run `MOC` automatically on `GENERATED` files. This time the fix is applied in the CMake enabling a CMake policy whenever suitable:

```
foreach(pol CMP0071)
    if(POLICY ${pol})
        cmake_policy(SET ${pol} NEW)
    endif()
endforeach()
```

and calling `qt5_wrap_ccp`:

```
qt5_wrap_cpp(MOCS
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/generated/EventAdaptor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/generated/EventProxy.h
    )
```

## CMake target-oriented coding

The new APIs in CMake allow us to write cleaner CMake code. This APIs are more target-focused, preferring the declaration of all definitions and properties target by target to global declarations and custom variables.

The CMake targets provide the compile- and link-time requirements to their dependers, so you only need to specify the target name to transitively add those requirements to the depender.

Also, it is possible to specify the visibility of the requirements:

| Visibilty | Description                           |
| --------- | ------------------------------------- |
| PRIVATE   | Needed by me, but not my dependers    |
| PUBLIC    | Needed by me and my dependers         |
| INTERFACE | Needed not by me, but by my dependers |

TODO: `target_link_libraries` usage

### Variables

Variables:

* are fragile.
* leak to other contexts.
* don't express scope of dependencies.
* are not checked for correctness or content.

So try to avoid unnecessary variables!


### Target functions

#### target\_include\_directories

```
target_include_directories(<target> [SYSTEM] [BEFORE]
                    <INTERFACE|PUBLIC|PRIVATE> [items1...]
                    [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])
```

#### target\_sources

```
target_sources(<target>
                    <INTERFACE|PUBLIC|PRIVATE> [items1...]
                    [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])
```

#### target\_link\_libraries

```
target_link_libraries(<target> ... <item>... ...)
OR
target_link_libraries(<target>
                    <PRIVATE|PUBLIC|INTERFACE> <item>...
                    [<PRIVATE|PUBLIC|INTERFACE> <item>...]...)
```

#### target\_compile\_definitions

```
target_compile_definitions(<target>
                    <INTERFACE|PUBLIC|PRIVATE> [items1...]
                    [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])
```

#### target\_compile\_features

```
target_compile_features(<target> <PRIVATE|PUBLIC|INTERFACE> <feature> [...])
```

#### target\_compile\_options

```
target_compile_options(<target> [BEFORE]
                    <INTERFACE|PUBLIC|PRIVATE> [items1...]
                    [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])
```

#### set\_target\_properties

```
set_target_properties(target1 target2 ...
                    PROPERTIES prop1 value1
                    prop2 value2 ...)
```


### CMake Policies

Modify the CMake behaviour &mdash; behaviours changed on newer versions or new behaviours &mdash;.

IMPORTANT: Allow OLD CMake but use NEW behaviour whenever the policies apply.

```
cmake_minimum_required(VERSION 3.0)
foreach (pol CMP0053
        CMP0063
        CMP0065
        )
    if(POLICY ${pol})
        cmake_policy(SET ${pol} NEW)
    endif()
endforeach()
```

## DBUS targets generation

The DBUS adaptors and proxies must be generated at compile time, and properly updated if there is any change to the interface XML file or any header included in them.

The Qt5 DBus target provides a few macros to do it:

```
qt5_add_dbus_interface(outfiles interface basename)

qt5_add_dbus_adaptor(outfiles xmlfile parentheader parentclassname [basename] [classname])
```

But they have the issue of not allowing to include additional headers, so the command `qdbusxml2cpp` must be invoked manually in those cases.

Furthermore, another CMake 3.8 related issue (solved since 3.10) is that CMake doesn't process `GENERATED` files`in AUTOMOC, thus the following policy must be used:

```
# CMP0071: Apply AUTOMOC on GENERATED targets
foreach(pol CMP0071)
    if(POLICY ${pol})
        cmake_policy(SET ${pol} NEW)
    endif()
endforeach()
```

So we define custom commands, like the following one:

```
add_custom_command(
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/interface/Events.xml
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/CuxsQDbusTypes.h
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/interface/generated/EventAdaptor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/generated/EventAdaptor.cpp
    COMMAND mkdir -p ${CMAKE_CURRENT_SOURCE_DIR}/interface/generated
    COMMAND ${OE_QMAKE_QDBUSXML2CPP} ${CMAKE_CURRENT_SOURCE_DIR}/interface/Events.xml 
    -i ${CMAKE_CURRENT_SOURCE_DIR}/interface/CuxsQDbusTypes.h 
    -a ${CMAKE_CURRENT_SOURCE_DIR}/interface/generated/EventAdaptor
    )
```

Then we add a new target which depends on the generated files:

```
add_custom_target(dbus_generator DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/generated/EventAdaptor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/generated/EventAdaptor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/generated/EventProxy.h
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/generated/EventProxy.cpp
    )
```

Finally, we add this new target as a project dependency:

```
add_dependencies(${PROJECT_NAME} dbus_generator)
```

For manual MOC on the generated files, `qt5_wrap_cpp` must be used:

```
qt5_wrap_cpp(MOCS
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/generated/EventAdaptor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/generated/EventProxy.h
    )
```


## Contributing

Please submit a pull request for any modification/improvement over the template.

## Authors

* **Eduardo Aguilar** - *Initial work* - [eduardo.aguilar](https://bitbucket.apps.verisure.com/plugins/servlet/network/profile/eduardo.aguilar@securitasdirect.es)

See also the list of [contributors](https://bitbucket.apps.verisure.com/plugins/servlet/network/contributorsgraph/UID/xcmaketemplate) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* [Embracing Modern CMake](http://www.steveire.com/embracing-modern-cmake.pdf)

