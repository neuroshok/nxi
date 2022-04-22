#ifndef INCLUDE_NXI_MODULE_PLATFORM_HPP_NXI
#define INCLUDE_NXI_MODULE_PLATFORM_HPP_NXI

#include <QtGlobal>

#ifdef Q_OS_WIN
    #include <windows.h>
    #undef interface

    #define nxi_os_module_function GetProcAddress
    #define nxi_os_module_load(path) LoadLibraryA(path)
    #define nxi_os_module_unload FreeLibrary
    namespace nxi { typedef HINSTANCE os_module_handle; }
    #define nxi_os_sharelibext "dll"

    #define NXI_IMPORT __declspec(dllimport)
    #define NXI_EXPORT __declspec(dllexport)

#else
#include <dlfcn.h>

    #define nxi_os_module_function dlsym
    #define nxi_os_module_load(path) dlopen(path, RTLD_LAZY)
    #define nxi_os_module_unload dlclose
    namespace nxi { typedef void* os_module_handle; }
    #define nxi_os_sharelibext "so"

    #define NXI_IMPORT
    #define NXI_EXPORT __attribute__ ((visibility ("default")))
#endif

// export function
#define NXI_SHARED NXI_EXPORT

#endif // INCLUDE_NXI_MODULE_PLATFORM_HPP_NXI