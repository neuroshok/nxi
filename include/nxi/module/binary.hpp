#ifndef NXI_MODULE_BINARY_H_NXI
#define NXI_MODULE_BINARY_H_NXI

#include <nxi/module/platform.hpp>

#include <nxi/database.hpp>
#include <nxi/module.hpp>
#include <nxi/type.hpp>
#include <nxi/core.hpp>

using Module_main_ptr = int (*)(nxi::core&);
using Module_load_ptr = int (*)(nxi::core&);

#include <nxi/log.hpp>
#include <exception>

#include <QString>

namespace nxi
{
    class core;

    class dynamic_module : public module
    {
    public:
      dynamic_module(nxi::core& nxi_core, const QString& name)
          : module(name, module_type::dynamic)
          , nxi_core_{ nxi_core }
          , name_{ name }
      {}

      void on_load() override
      {
          try
          {
              QString module_path = nxi::core::module_path(name_ + "." + nxi_os_sharelibext, nxi::module_type::dynamic);
              nxi::os_module_handle handle = nxi_os_module_load(module_path.toStdString().c_str());
              if (!handle) nxi_error("file not found : " + module_path);

              // get main pointer
              auto main_ptr = reinterpret_cast<Module_main_ptr>(nxi_os_module_function(handle, "nxi_module"));
              if (!main_ptr)
              {
                  nxi_error("function nxi_module missing");
              }
              // get load pointer and call
              auto load_ptr = reinterpret_cast<Module_load_ptr>(nxi_os_module_function(handle, "nxi_module_load"));
              if (load_ptr) load_ptr(nxi_core_);
          }
          catch (const std::exception& e){ nxi_warning("can t load module : {}", e.what()); }
        }

        QString name_;
        nxi::core& nxi_core_;
    };
} // nxi

#endif // NXI_MODULE_BINARY_H_NXI