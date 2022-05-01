#ifndef NXI_MODULE_BINARY_H_NXI
#define NXI_MODULE_BINARY_H_NXI

#include <nxi/module/platform.hpp>

#include <nxi/database.hpp>
#include <nxi/module.hpp>
#include <nxi/core.hpp>
#include <nxi/session.hpp>
#include <nxi/type.hpp>

using Module_load_ptr = int(*)(nxi::core*);
using Module_unload_ptr = int(*)(nxi::core*);

#include <nxi/log.hpp>
#include <exception>

#include <QString>

namespace nxi
{
    class session;

    class dynamic_module : public module
    {
    public:
      dynamic_module(nxi::session& session, const QString& name)
          : module(name, module_type::dynamic)
          , session_{ session }
          , name_{ name }
      {}

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
              else load_ptr(&session_.nxi_core());


                  //auto unload_ptr = reinterpret_cast<Module_unload_ptr>(nxi_os_module_function(handle, "nxi_unload"));
                  //if (load_ptr) unload_ptr(session_.api());

          }
      }

        QString name_;
        nxi::session& session_;
    };
} // nxi

#endif // NXI_MODULE_BINARY_H_NXI