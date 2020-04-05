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
    target_include_directories(module_${name} INTERFACE ${NXI_MODULE_ROOT}/static/${name})
    set(NXI_MODULES ${NXI_MODULES} "module_${name};" PARENT_SCOPE)
endmacro()

# module_build
macro(module_build name)
    set(NXI_MODULE_${name} ON)
endmacro()

# module_source
macro(module_source)
    foreach (SRC ${ARGN})
        list (APPEND NXI_MODULE_SOURCES "${NXI_MODULE_ROOT}/static/${NXI_MODULE_CURRENT}/${SRC}")
    endforeach()
    target_sources(module_${NXI_MODULE_CURRENT} INTERFACE ${NXI_MODULE_SOURCES})
endmacro()