cmake_minimum_required(VERSION 3.12)

cmake_policy(SET CMP0077 NEW)

#add_compile_options(/EHsc)

#-------------------------------------------------------
#                     Config
#-------------------------------------------------------
include(cmake/utility.cmake)

# default
# target system
if (NOT NXI_TARGET_SYSTEM)
    set(NXI_TARGET_DESKTOP 1)
    set(NXI_TARGET_SYSTEM ${CMAKE_HOST_SYSTEM_NAME})
endif()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

option(NXI_WINDOW_GENERIC "Use generic window" ON)

# user config file
include(user-config.cmake OPTIONAL)

#-------------------------------------------------------
#                     Variable
#-------------------------------------------------------
# path
set(NXI_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(NXI_BIN_ROOT ${NXI_ROOT}/bin/nxi)
set(NXI_LIB_ROOT ${NXI_ROOT}/lib)
set(NXI_MODULE_ROOT "${NXI_ROOT}/module")
set(NXI_MODULE_BIN_ROOT ${NXI_BIN_ROOT}/module)
set(NXI_THIRD_PARTY_ROOT ${NXI_ROOT}/third_party)

# qt path
if (NOT NXI_QT_ROOT)
    message(FATAL_ERROR "Variable NXI_QT_ROOT not found")
endif()

set(CMAKE_FIND_ROOT_PATH  ${NXI_QT_ROOT})
set(CMAKE_PREFIX_PATH  ${NXI_QT_ROOT})
set(CMAKE_MODULE_PATH  ${NXI_QT_ROOT})

## PLATFORM
# windows
if (NXI_TARGET_DESKTOP)
    set(NXI_QT_COMPONENTS Core Widgets Gui SvgWidgets Sql WebEngineWidgets WebChannel)
    set(NXI_QT_LIBS Qt6::Core Qt6::Widgets Qt6::Gui Qt6::SvgWidgets Qt6::Sql Qt6::WebEngineWidgets Qt6::WebChannel)
endif ()

# android
include(platform/android/cmake/android.cmake)

#-------------------------------------------------------
#                     Project
#-------------------------------------------------------
project(nxi)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


#-------------------------------------------------------
#                     Dependencies
#-------------------------------------------------------
# nds
add_subdirectory(${NXI_THIRD_PARTY_ROOT}/nds)

# speedlog
add_library(lib_speedlog INTERFACE)
target_include_directories(lib_speedlog INTERFACE ${NXI_THIRD_PARTY_ROOT}/spdlog/include)

# QT
find_package(Qt6 REQUIRED COMPONENTS ${NXI_QT_COMPONENTS})
# must be set before files scan
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


#-------------------------------------------------------
#                       Global
#-------------------------------------------------------

message("--------------------------------------------------------
                         NXI
--------------------------------------------------------")
message("Host system : ${CMAKE_HOST_SYSTEM_NAME}")
message("Target system : ${NXI_TARGET_SYSTEM}")
message("Qt path : ${NXI_QT_ROOT}")
message("")


#-------------------------------------------------------
#                       Module
#-------------------------------------------------------
internal_add_modules(${NXI_MODULE_ROOT})

#-------------------------------------------------------
#                       Target
#-------------------------------------------------------

if (NXI_TARGET_SYSTEM STREQUAL "android")
    add_library(nxi SHARED ${NXI_SOURCES})
    target_include_directories(nxi PUBLIC include)
    target_link_libraries(nxi lib_nds ${NXI_STATIC_MODULES} ${NXI_QT_LIBS})

    nxi_android_apk_target(nxi)
endif()

if (NXI_TARGET_DESKTOP)
    # sources
    if (WIN32)
        set(NXI_ICON "${PROJECT_SOURCE_DIR}/resource/icon.rc" "${PROJECT_SOURCE_DIR}/resource/icon.ico")

        # use generic window
        if (NOT NXI_WINDOW_GENERIC)
            list(APPEND NXI_PLATFORM_SOURCES "${PROJECT_SOURCE_DIR}/platform/windows/window.cpp")
        endif ()

        list(APPEND NXI_PLATFORM_SOURCES
                "${PROJECT_SOURCE_DIR}/platform/windows/notification.cpp"
                "${PROJECT_SOURCE_DIR}/third_party/wintoast/wintoast.cpp")
    endif()

    if (NOT WIN32)
        set(NXI_WINDOW_GENERIC ON)
    endif ()

    # use generic window
    if (NXI_WINDOW_GENERIC)
        list(APPEND NXI_PLATFORM_SOURCES "${PROJECT_SOURCE_DIR}/platform/generic/window.cpp")
        list(APPEND NXI_DEFINITIONS NXI_WINDOW_GENERIC)
    endif ()

    # generic platform sources
    list(APPEND NXI_PLATFORM_SOURCES "${PROJECT_SOURCE_DIR}/platform/generic/notification.cpp")

    file(GLOB_RECURSE NXI_SOURCES
            "${PROJECT_SOURCE_DIR}/source/nxi/*.cpp"
            "${PROJECT_SOURCE_DIR}/source/nxw/*.cpp"
            "${PROJECT_SOURCE_DIR}/source/ui/*.cpp"
            "${PROJECT_SOURCE_DIR}/source/w3c/*.cpp"
            "${PROJECT_SOURCE_DIR}/resource/*.qrc"
            "${PROJECT_SOURCE_DIR}/include/*.hpp")
    set(NXI_SOURCES ${NXI_SOURCES} ${NXI_PLATFORM_SOURCES} source/main.cpp)

    list(APPEND NXI_DEFINITIONS QT_FORCE_ASSERTS)

    add_executable(nxi_exe ${NXI_SOURCES} ${NXI_ICON})
    target_include_directories(nxi_exe PUBLIC include)
    target_compile_definitions(nxi_exe PUBLIC ${NXI_DEFINITIONS})
    target_link_libraries(nxi_exe lib_nds lib_speedlog ${NXI_STATIC_MODULES} ${NXI_QT_LIBS})

    # console
    if (NXI_CONSOLE)
        set_target_properties(nxi_exe PROPERTIES WIN32_EXECUTABLE false)
    else()
        set_target_properties(nxi_exe PROPERTIES WIN32_EXECUTABLE true)
    endif()

    set_target_properties(nxi_exe
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${NXI_BIN_ROOT}"
            RUNTIME_OUTPUT_DIRECTORY_RELEASE "${NXI_BIN_ROOT}"
            OUTPUT_NAME "nxi"
            )


    # tools
    add_executable(tool_theme_converter tools/theme_converter/main.cpp)
    target_include_directories(tool_theme_converter PUBLIC include)
    target_link_libraries(tool_theme_converter Qt6::Core)
    set_target_properties(tool_theme_converter
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${NXI_BIN_ROOT}"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${NXI_BIN_ROOT}"
        OUTPUT_NAME "theme_converter"
        )

    # deploy
    get_target_property(QT_QMAKE_EXECUTABLE Qt::qmake IMPORTED_LOCATION)
    get_filename_component(QT_DEPLOYQT_PATH ${QT_QMAKE_EXECUTABLE} PATH)

    if (NXI_TARGET_SYSTEM STREQUAL "Windows")
        set(QT_DEPLOYQT_EXECUTABLE "${QT_DEPLOYQT_PATH}/windeployqt.exe")
    elseif (NXI_TARGET_SYSTEM STREQUAL "Darwin")
        set(QT_DEPLOYQT_EXECUTABLE "${QT_DEPLOYQT_PATH}/macdeployqt")
    elseif (NXI_TARGET_SYSTEM STREQUAL "Linux")
        set(QT_DEPLOYQT_EXECUTABLE "linuxdeployqt")
    endif ()

    add_custom_target(nxi_deploy
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/nxi${CMAKE_STATIC_LIBRARY_SUFFIX} ${NXI_LIB_ROOT}/nxi${CMAKE_STATIC_LIBRARY_SUFFIX}
            COMMAND ${QT_DEPLOYQT_EXECUTABLE} --qmldir ${CMAKE_SOURCE_DIR} $<TARGET_FILE_DIR:nxi_exe>
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${NXI_ROOT}/module/nxi.module/theme/ $<TARGET_FILE_DIR:nxi_exe>/module/theme/
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${NXI_ROOT}/module/nxi.module/web/ $<TARGET_FILE_DIR:nxi_exe>/module/web/
            )
    add_dependencies(nxi_deploy nxi_exe)

endif()

message("--------------------------------------------------------")