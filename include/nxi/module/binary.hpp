#ifndef NXI_MODULE_BINARY_H_NXI
#define NXI_MODULE_BINARY_H_NXI

#include <nxi/module/platform.hpp>

#include <nxi/core.hpp>
#include <nxi/database.hpp>
#include <nxi/module.hpp>
#include <nxi/type.hpp>
#include <nxi/user.hpp>

#include <nxi/log.hpp>
#include <exception>

#include <QString>

using Module_load_ptr = int (*)(nxi::core*);
using Module_unload_ptr = int (*)(nxi::core*);

namespace nxi
{
    class core;

    class dynamic_module : public module
    {
    public:
        dynamic_module(nxi::core& core, const QString& name)
            : module(name, module_type::dynamic)
            , core_{ core }
            , name_{ name }
        {}
        dynamic_module(const dynamic_module&) = delete;
        dynamic_module& operator=(const dynamic_module&) = delete;

        void on_load() override
        {
            QString module_path = nxi::core::module_path(name_ + "." + nxi_os_sharelibext, nxi::module_type::dynamic);
            nxi::os_module_handle handle = nxi_os_module_load(module_path.toStdString().c_str());
            if (!handle) nxi_error("unable to load module : {}", module_path);
            else
            {
                // get main pointer
                auto load_ptr = reinterpret_cast<Module_load_ptr>(nxi_os_module_function(handle, "nxi_load"));
                if (!load_ptr) nxi_error("function nxi_load missing");
                else load_ptr(&core_);

                // auto unload_ptr = reinterpret_cast<Module_unload_ptr>(nxi_os_module_function(handle, "nxi_unload"));
                // if (load_ptr) unload_ptr(session_.api());
            }
        }

        QString name_;
        nxi::core& core_;
    };
} // nxi

#endif // NXI_MODULE_BINARY_H_NXI