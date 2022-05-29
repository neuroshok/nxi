-- // clang-format off
set_project("nxi")
set_version("0.0.1")

-------------------------------------------------------
---                      config                     ---
-------------------------------------------------------
set_defaultarchs("x64")

-------------------------------------------------------
---                    libraries                   ---
-------------------------------------------------------
add_requires("spdlog", { alias = "lib_spdlog" })

includes("E:/project/nk/nds")

-------------------------------------------------------
---                      option                     ---
-------------------------------------------------------
option("window")
set_showmenu(true)
set_values("generic", "platform")
on_check(function(option)
    if option:is_on("platform") then
        if is_plat("windows") then
            add_defines("NXI_WINDOW_WINDOWS")
        end
        if is_plat("linux") then
            add_defines("NXI_WINDOW_LINUX")
        end
    end
end)

option("notification")
set_showmenu(true)
set_values("generic", "platform")
on_check(function(option)
    if option:is_on("platform") then
        if is_plat("windows") then
            add_defines("NXI_NOTIFICATION_WINDOWS")
        end
        if is_plat("linux") then
            add_defines("NXI_NOTIFICATION_LINUX")
        end
    end
end)

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

add_includedirs("include", "third_party")

-- sources
add_files("source/main.cpp")
add_files("source/nxi/**.cpp", "include/nxi/**.hpp")
add_files("source/nxw/**.cpp", "include/nxw/**.hpp")
add_files("source/ui/**.cpp", "include/ui/**.hpp")
add_files("source/w3c/**.cpp", "include/w3c/**.hpp")
add_files("resource/*.qrc")
-- platform sources
if get_config("window") == "generic" then
    add_files("source/platform/generic/window.cpp", "include/platform/generic/window.hpp")
end
if get_config("notification") == "generic" then
    add_files("source/platform/generic/notification.cpp", "include/platform/generic/notification.hpp")
end

add_cxflags("/permissive-")
add_ldflags("/subsystem:console")

-------------------------------------------------------
---                     install                     ---
-------------------------------------------------------
set_installdir("$(buildir)/nxi")
add_installfiles("module/nxi.module/(theme/**/*.*)", { prefixdir = "bin/module" })
add_installfiles("module/nxi.module/(web/**/*.*)", { prefixdir = "bin/module" })