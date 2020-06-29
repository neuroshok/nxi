#include "nxi_static.hpp"

#include <nxi/core.hpp>
#include <nxi/system/window.hpp>
#include <nxi/log.hpp>

namespace nxi::modules
{
    nxi_static::nxi_static(nxi::session& session)
        : module("nxi_static", module_type::compiled)
        , session_{ session }
    {}

    void nxi_static::on_load()
    {
        auto cmd = nxi::command("nxi_static", "action", [this](const nxi::values&)
        {
            session_.error("nxi_static module is loaded");
        });

        session_.command_system().add(std::move(cmd));
    }

} // nxi::modules