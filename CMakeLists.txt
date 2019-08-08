cmake_minimum_required(VERSION 3.13)

#-------------------------------------------------------
#                     Config
#-------------------------------------------------------
include(cmake/utility.cmake)

# default
# target system
if (NOT NXI_TARGET_SYSTEM)
    set(NXI_TARGET_SYSTEM windows)
endif()
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# user config file
include(user-config.cmake OPTIONAL)

#-------------------------------------------------------
#                     Variable
#-------------------------------------------------------
# path
set(NXI_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(NXI_BIN_ROOT ${NXI_ROOT}/bin)
set(NXI_MODULE_ROOT "${NXI_ROOT}/module")
set(NXI_THIRD_PARTY_ROOT ${NXI_ROOT}/third_party)

# qt path
if (NOT NXI_QT_ROOT)
    message(FATAL_ERROR "Variable NXI_QT_ROOT not found")
endif()
set(CMAKE_FIND_ROOT_PATH  ${NXI_QT_ROOT})
set(CMAKE_PREFIX_PATH  ${NXI_QT_ROOT})


## PLATFORM
# windows
if (NXI_TARGET_SYSTEM STREQUAL "windows")
    set(NXI_QT_COMPONENTS Core Widgets Gui WebEngine WebEngineWidgets 3DCore 3DRender 3DInput 3DExtras)
    set(NXI_QT_LIBS Qt5::Core Qt5::Widgets Qt5::Gui Qt5::WebEngine Qt5::WebEngineWidgets
            Qt5::3DCore Qt5::3DRender Qt5::3DInput Qt5::3DExtras)
endif()

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
# NDB
set(NDB_ENGINE_SQLITE ON)
add_subdirectory(${NXI_THIRD_PARTY_ROOT}/ndb ${NXI_THIRD_PARTY_ROOT}/ndb/cmake-build)

# NDS
add_subdirectory(${NXI_THIRD_PARTY_ROOT}/nds)

# speedlog
add_library(lib_speedlog INTERFACE)
target_include_directories(lib_speedlog INTERFACE ${NXI_THIRD_PARTY_ROOT}/spdlog/include)

# QT
find_package(Qt5 REQUIRED COMPONENTS ${NXI_QT_COMPONENTS})
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


#-------------------------------------------------------
#                       Module
#-------------------------------------------------------
message("Module build")

SUBDIRLIST(SUBDIRS ${NXI_MODULE_ROOT})

# add modules
foreach(module ${SUBDIRS})
    option(NXI_MODULE_${module} "Module ${module}" ${NXI_MODULE_${module}})
    message("- ${module} : ${NXI_MODULE_${module}}")
    if (NXI_MODULE_${module})
        set(NXI_MODULE_CURRENT ${module})
        add_subdirectory(${NXI_MODULE_ROOT}/${module})
        # create variables for generated file
        string(APPEND  NXI_MODULE_STATIC_INCLUDE "#include \"module/${module}/${module}.hpp\"\n")
        string(APPEND  NXI_MODULE_STATIC_INIT ", ${module}_{ nxi_core }\n")
        string(APPEND  NXI_MODULE_STATIC_LOAD "${module}_.load();\n")
        string(APPEND  NXI_MODULE_STATIC_MEMBER "nxi::modules::${module} ${module}_;\n")
    endif()
endforeach()

# create container file
configure_file(
        "${NXI_MODULE_ROOT}/static_module.hpp.in"
        "${NXI_MODULE_ROOT}/static_module.hpp"
)

#-------------------------------------------------------
#                       Target
#-------------------------------------------------------

if (NXI_TARGET_SYSTEM STREQUAL "android")
    add_library(nxi SHARED ${NXI_SOURCES})
    target_include_directories(nxi PUBLIC include)
    target_link_libraries(nxi lib_ndb lib_nds ${NXI_MODULES} ${NXI_QT_LIBS})

    nxi_android_apk_target(nxi)
else()
    # sources
    file(GLOB_RECURSE NXI_SOURCES
            "${PROJECT_SOURCE_DIR}/source/nxi/*.cpp"
            "${PROJECT_SOURCE_DIR}/source/nxw/*.cpp"
            "${PROJECT_SOURCE_DIR}/source/ui/*.cpp"
            "${PROJECT_SOURCE_DIR}/source/ui/*.qrc"
            "${PROJECT_SOURCE_DIR}/include/*.hpp")
    set(NXI_SOURCES ${NXI_SOURCES} source/main.cpp)

    add_executable(nxi ${NXI_SOURCES})
    target_include_directories(nxi PUBLIC include)

    target_link_libraries(nxi lib_ndb lib_nds lib_speedlog ${NXI_MODULES} ${NXI_QT_LIBS})

    set_target_properties(nxi
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${NXI_BIN_ROOT}/nxi"
            OUTPUT_NAME  "nxi"
            )


    # deploy
    get_target_property(QT5_QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(QT5_WINDEPLOYQT_EXECUTABLE ${QT5_QMAKE_EXECUTABLE} PATH)
    set(QT5_WINDEPLOYQT_EXECUTABLE "${QT5_WINDEPLOYQT_EXECUTABLE}/windeployqt.exe")

    add_custom_target(nxi_deploy COMMAND ${QT5_WINDEPLOYQT_EXECUTABLE} --qmldir ${CMAKE_SOURCE_DIR} $<TARGET_FILE_DIR:nxi>)
    add_dependencies(nxi_deploy nxi)
endif()



message("--------------------------------------------------------")