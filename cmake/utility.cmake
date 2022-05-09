#-------------------------------------------------------
#                       Utility
#-------------------------------------------------------

# functions
function(list_folders result curdir)
    file(GLOB children RELATIVE ${curdir} ${curdir}/*)

    set(dirlist "")
    foreach(child ${children})
        if(IS_DIRECTORY ${curdir}/${child})
            LIST(APPEND dirlist ${child})
        endif()
    endforeach()
    set(${result} ${dirlist} PARENT_SCOPE)
endfunction()

function(internal_module_pack_name module_path module_pack_name)
    set(MODULE_PATH_SIZE)
    set(SLASH_POS)
    string(LENGTH ${NXI_MODULE_ROOT} MODULE_PATH_SIZE)
    math(EXPR MODULE_PATH_SIZE "${MODULE_PATH_SIZE} + 1")
    string(SUBSTRING ${module_path} ${MODULE_PATH_SIZE} -1 module_pack_name)
    string(FIND ${module_pack_name} "/" SLASH_POS)
    string(SUBSTRING ${module_pack_name} 0 ${SLASH_POS} module_pack_name)
    set(module_pack_name ${module_pack_name} PARENT_SCOPE)
endfunction()

function(internal_module_fullname module module_fullname)
    internal_module_pack_name(${CMAKE_CURRENT_SOURCE_DIR} module_pack_name)
    set(module_fullname ${module_pack_name}_${module} PARENT_SCOPE)
endfunction()

#-------------------------------------------------------
#                       Modules
#-------------------------------------------------------

# internal_generate_module
function(internal_generate_static_module module_pack module module_path)
    set(NXI_STATIC_MODULES ${NXI_STATIC_MODULES} "module_${module_pack}_${module};" PARENT_SCOPE)

    # create variables for generated file
    string(APPEND NXI_MODULE_STATIC_INCLUDE "#include <../module/${module_path}/${module}.hpp>\n")
    string(APPEND NXI_MODULE_STATIC_INIT ", ${module}_{ core }\n")
    string(APPEND NXI_MODULE_STATIC_LOAD "${module}_.load();\n")
    string(APPEND NXI_MODULE_STATIC_MEMBER "nxi::modules::${module} ${module}_;\n")

    set(NXI_MODULE_STATIC_INCLUDE ${NXI_MODULE_STATIC_INCLUDE} PARENT_SCOPE)
    set(NXI_MODULE_STATIC_INIT ${NXI_MODULE_STATIC_INIT} PARENT_SCOPE)
    set(NXI_MODULE_STATIC_LOAD ${NXI_MODULE_STATIC_LOAD} PARENT_SCOPE)
    set(NXI_MODULE_STATIC_MEMBER ${NXI_MODULE_STATIC_MEMBER} PARENT_SCOPE)
endfunction()

# internal_add_modules
function(internal_add_modules path)
    message("Module build")
    if (NOT EXISTS ${path})
        message(FATAL_ERROR "Can't add modules, path not found : ${path}")
    endif()

    list_folders(module_packs ${path})

    foreach(module_pack ${module_packs})

        # message("Module pack : ${module_pack}")
        message("- ${module_pack}")

        list_folders(module_types ${path}/${module_pack})

        foreach(module_type ${module_types})

            # message("Module type : ${module_type}")
            list_folders(module_types ${path}/${module_pack}/${module_type})

            foreach(module ${module_types})

                if (NXI_MODULE_${module_pack}_${module})
                    option(NXI_MODULE_${module_pack}_${module}} "Module ${module}" ${NXI_MODULE_${module_pack}_${module}})
                    # message("Module : ${module}")
                    if (module_type STREQUAL "static" OR module_type STREQUAL "dynamic" AND EXISTS ${NXI_MODULE_ROOT}/${module_pack}/${module_type}/${module}/CMakeLists.txt)
                        message("  - ${module} [${module_type}]")

                        if (module_type STREQUAL "static")
                            internal_generate_static_module(${module_pack} ${module} "${module_pack}/${module_type}/${module}")
                        endif()

                        add_subdirectory(${NXI_MODULE_ROOT}/${module_pack}/${module_type}/${module})
                    endif()
                endif()

            endforeach()
        endforeach()
    endforeach()

    # create container file
    configure_file(
            "${NXI_ROOT}/include/nxi/module/static.hpp.in"
            "${NXI_ROOT}/include/nxi/module/static.generated.hpp"
    )

    set(NXI_STATIC_MODULES ${NXI_STATIC_MODULES} PARENT_SCOPE)
endfunction()

# module_static
function(module_static module)
    internal_module_fullname(${module} module_fullname)
    add_library(module_${module_fullname} INTERFACE)
    target_include_directories(module_${module_fullname} INTERFACE ${NXI_MODULE_ROOT}/static/${module_fullname})
    add_custom_target(static_${module_fullname})
endfunction()

# module_dynamic
function(module_dynamic module type)
    internal_module_fullname(${module} module_fullname)

    if (type STREQUAL "rust")
        add_custom_target(dynamic_${module_fullname})

        add_custom_command(
            TARGET dynamic_${module_fullname} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${NXI_LIB_ROOT}/nxi.lib ./nxi.lib
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/ ./
            COMMAND cargo
            ARGS build --release
        )
        add_custom_command(
            TARGET dynamic_${module_fullname} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/target/release/${module}${CMAKE_SHARED_LIBRARY_SUFFIX} ${NXI_MODULE_BIN_ROOT}/dynamic/${module_fullname}${CMAKE_SHARED_LIBRARY_SUFFIX}
        )
        add_dependencies(dynamic_${module_fullname} nxi_deploy)
    endif()
endfunction()

# module_build
macro(module_build module_pack module)
    # internal_module_fullname(${CMAKE_CURRENT_SOURCE_DIR}/module module_fullname)
    set(NXI_MODULE_${module_pack}_${module} ON)
endmacro()

# module_source
macro(module_source module)
    foreach (SRC ${ARGN})
        list (APPEND NXI_MODULE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${SRC}")
    endforeach()
    internal_module_fullname(${module} module_fullname)
    target_sources(module_${module_fullname} INTERFACE ${NXI_MODULE_SOURCES})
endmacro()