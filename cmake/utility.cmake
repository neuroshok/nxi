#-------------------------------------------------------
#                       Utility
#-------------------------------------------------------

# functions
function(SUBDIRLIST result curdir)
    file(GLOB children RELATIVE ${curdir} ${curdir}/*)

    set(dirlist "")
    foreach(child ${children})
        if(IS_DIRECTORY ${curdir}/${child})
            LIST(APPEND dirlist ${child})
        endif()
    endforeach()
    set(${result} ${dirlist} PARENT_SCOPE)
endfunction()

# internal_generate_module
function(internal_generate_module module submodule_path)
    if (NOT submodule_path STREQUAL "")
        set(SPACE "    ")
    endif()

    option(NXI_MODULE_${module} "Module ${module}" ${NXI_MODULE_${module}})
    message("${SPACE}- ${module} : ${NXI_MODULE_${module}}")
    if (${NXI_MODULE_${module}})
        set(NXI_MODULES ${NXI_MODULES} "module_${module};" PARENT_SCOPE)
    endif()

    if (NXI_MODULE_${module})
        set(NXI_MODULE_CURRENT ${module})
        add_subdirectory(${NXI_MODULE_ROOT}/static${submodule_path}/${module})

        # create variables for generated file
        string(APPEND NXI_MODULE_STATIC_INCLUDE "#include \"module/static${submodule_path}/${module}/${module}.hpp\"\n")
        string(APPEND NXI_MODULE_STATIC_INIT ", ${module}_{ session }\n")
        string(APPEND NXI_MODULE_STATIC_LOAD "${module}_.load();\n")
        string(APPEND NXI_MODULE_STATIC_MEMBER "nxi::modules::${module} ${module}_;\n")

        set(NXI_MODULE_STATIC_INCLUDE ${NXI_MODULE_STATIC_INCLUDE} PARENT_SCOPE)
        set(NXI_MODULE_STATIC_INIT ${NXI_MODULE_STATIC_INIT} PARENT_SCOPE)
        set(NXI_MODULE_STATIC_LOAD ${NXI_MODULE_STATIC_LOAD} PARENT_SCOPE)
        set(NXI_MODULE_STATIC_MEMBER ${NXI_MODULE_STATIC_MEMBER} PARENT_SCOPE)
    endif()
endfunction()

# internal_add_modules
function(internal_add_modules path)
    if (NOT EXISTS ${path})
        message(FATAL_ERROR "Can't add modules, path not found : ${path}")
    endif()

    SUBDIRLIST(SUBDIRS ${path})

    foreach(module ${SUBDIRS})
        if (NOT EXISTS ${path}/${module}/CMakeLists.txt)
            SUBDIRLIST(module_subdirs ${path}/${module})
            message("- ${module}")
            foreach(submodule ${module_subdirs})
                internal_generate_module(${submodule} "/${module}")
            endforeach()
        else()
            internal_generate_module(${module} "")
        endif()
    endforeach()

    # create container file
    configure_file(
            "${NXI_ROOT}/include/nxi/module/static.hpp.in"
            "${NXI_ROOT}/include/nxi/module/static.generated.hpp"
    )

    set(NXI_MODULES ${NXI_MODULES} PARENT_SCOPE)
endfunction()

# module_add
function(module_add name)
    add_library(module_${name} INTERFACE)
    target_include_directories(module_${name} INTERFACE ${NXI_MODULE_ROOT}/static/${name})
    add_custom_target(mod_${name})
endfunction()

# module_build
macro(module_build name)
    set(NXI_MODULE_${name} ON)
endmacro()

# module_source
macro(module_source)
    foreach (SRC ${ARGN})
        list (APPEND NXI_MODULE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${SRC}")
    endforeach()
    target_sources(module_${NXI_MODULE_CURRENT} INTERFACE ${NXI_MODULE_SOURCES})
endmacro()