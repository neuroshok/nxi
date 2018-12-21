cmake_minimum_required(VERSION 3.1)

project(nxi)

message("--------------------------------------------------------
                         NXI
--------------------------------------------------------")

#-------------------------------------------------------
#                       Utility
#-------------------------------------------------------

# functions
macro(SUBDIRLIST result curdir)
    file(GLOB children RELATIVE ${curdir} ${curdir}/*)
    set(dirlist "")
    foreach(child ${children})
        if(IS_DIRECTORY ${curdir}/${child})
            LIST(APPEND dirlist ${child})
        endif()
    endforeach()
    set(${result} ${dirlist})
endmacro()

# module_add
macro(module_add name)
    add_library(module_${name} INTERFACE)
    target_include_directories(module_${name} INTERFACE ${NXI_MODULE_ROOT}/${name})
    set(NXI_MODULES ${NXI_MODULES} "module_${name};" PARENT_SCOPE)
endmacro()

# module_build
macro(module_build name)
    set(NXI_MODULE_${name} ON)
endmacro()

# module_source
macro(module_source)
    foreach (SRC ${ARGN})
        list (APPEND NXI_MODULE_SOURCES "${NXI_MODULE_ROOT}/${NXI_MODULE_CURRENT}/${SRC}")
    endforeach()
    target_sources(module_${NXI_MODULE_CURRENT} INTERFACE ${NXI_MODULE_SOURCES})
endmacro()

#-------------------------------------------------------
#                       Global
#-------------------------------------------------------
# user config file
include(user-config.cmake OPTIONAL)

# must be set before files scan
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(NXI_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(NXI_BIN_ROOT ${NXI_ROOT}/bin)
set(NXI_MODULE_ROOT "${NXI_ROOT}/module")
set(NXI_THIRD_PARTY_ROOT ${NXI_ROOT}/third_party)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE NXI_SOURCES "source/*.cpp" "source/*.qrc" "include/*.hpp")

find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui WebEngine WebEngineWidgets
        3DCore 3DRender 3DInput 3DExtras)

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
#                     DEPENDENCIES
#-------------------------------------------------------
set(NDB_ENGINE_SQLITE ON)
add_subdirectory(${NXI_THIRD_PARTY_ROOT}/ndb ${NXI_THIRD_PARTY_ROOT}/ndb/cmake-build)

#-------------------------------------------------------
#                       Target
#-------------------------------------------------------
add_executable(nxi ${NXI_SOURCES})
target_include_directories(nxi PUBLIC include)

target_link_libraries(nxi lib_ndb ${NXI_MODULES}
        Qt5::Core Qt5::Widgets Qt5::Gui Qt5::WebEngine Qt5::WebEngineWidgets
        Qt5::3DCore Qt5::3DRender Qt5::3DInput Qt5::3DExtras)

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


message("--------------------------------------------------------")