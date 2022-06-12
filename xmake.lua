set_project("nxi")
set_version("0.0.1")

-------------------------------------------------------
---                      config                     ---
-------------------------------------------------------
set_warnings("allextra")

add_rules("mode.release", "mode.releasedbg")

-------------------------------------------------------
---                    libraries                   ---
-------------------------------------------------------
add_requires("spdlog", { alias = "lib_spdlog" })

includes("third_party/nds")

-------------------------------------------------------
---                      option                     ---
-------------------------------------------------------
option("nxi_console")

option("nxi_window")
    set_showmenu(true)
    add_defines("NXI_WINDOW_PLATFORM")

-------------------------------------------------------
---                     targets                     ---
-------------------------------------------------------
target("nxi")
    set_default(true)
    set_kind("binary")
    add_rules("qt.widgetapp")
    add_frameworks("QtCore", "QtWidgets", "QtGui", "QtSvg", "QtSql", "QtNetwork", "QtWebEngineCore", "QtWebEngineWidgets", "QtWebChannel")
    
    add_packages("lib_spdlog")
    add_deps("lib_nds")
    
    set_options("window", "notification")
    
    add_includedirs(
        "include",
        "third_party/wintoast/include")
    
    -- sources
    add_files("source/main.cpp")
    add_files("source/nxi/**.cpp", "include/nxi/**.hpp")
    add_files("source/nxw/**.cpp", "include/nxw/**.hpp")
    add_files("source/platform/generic/**.cpp", "include/platform/generic/**.hpp")
    add_files("source/ui/**.cpp", "include/ui/**.hpp")
    add_files("source/w3c/**.cpp", "include/w3c/**.hpp")
    add_files("resource/*.qrc")
    
    -- config sources
    -- todo fix path
    
    -- local NXI_MODULE_STATIC_INCLUDE = "#include <../module/${module_path}/${module}.hpp>\n"
    -- local NXI_MODULE_STATIC_INIT = ", ${module}_{ core }\n"
    -- local NXI_MODULE_STATIC_LOAD = "${module}_.load();\n"
    -- local NXI_MODULE_STATIC_MEMBER = "nxi::modules::${module} ${module}_;\n"
    
    set_configdir("$(projectdir)/")
    add_configfiles("include/nxi/module/static.hpp.in", { filename = "static.generated.hpp", prefixdir = "include/nxi/module",
    variables = {NXI_MODULE_STATIC_INCLUDE = "", NXI_MODULE_STATIC_INIT = "", NXI_MODULE_STATIC_LOAD = "", NXI_MODULE_STATIC_MEMBER = ""} })
    
    -- platform sources
    if (is_plat("windows")) then
        add_files("resource/icon.rc")
        
        add_cxflags("/permissive-")
        if (has_config("nxi_console")) then
            add_ldflags("/subsystem:console")
        end
        
        -- config
        if get_config("window") == "platform" then
            add_files("source/platform/windows/window.cpp", "include/platform/windows/window.hpp")
        end
        
        add_files("source/platform/windows/notification.cpp", "include/platform/windows/notification.hpp")
        add_files("third_party/wintoast/source/wintoast.cpp")
    end
    
    if (is_plat("linux")) then
        add_syslinks("dl", "pthread")
    end

-------------------------------------------------------
---                     install                     ---
-------------------------------------------------------
set_installdir("$(buildir)/nxi")
add_installfiles("module/nxi.module/(theme/**/*.*)", { prefixdir = "bin/module" })
add_installfiles("module/nxi.module/(web/**/*.*)", { prefixdir = "bin/module" })