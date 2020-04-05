#include "nxi_static.hpp"

#include <nxi/core.hpp>
#include <nxi/window.hpp>
#include <nxi/system/window.hpp>
#include <nxi/log.hpp>

namespace nxi::modules
{
    nxi_static::nxi_static(nxi::core& nxi_core)
        : module("nxi_static", module_type::compiled)
        , nxi_core_{ nxi_core }
    {}

    void nxi_static::on_load()
    {
        nxi_core_.error("nxi_static module loaded");

        auto cmd = nxi::command("nxi_static", "action", std::bind(&nxi::core::quit, &nxi_core_));

        nxi_core_.command_system().add(std::move(cmd));
    }

} // nxi::modules