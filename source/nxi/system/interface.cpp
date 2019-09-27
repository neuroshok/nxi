#include <nxi/system/interface.hpp>

#include <nxi/core.hpp>
#include <nxi/style.hpp>

namespace nxi
{
    interface_system::interface_system(nxi::core& nxi_core)
         : nxi_core_{ nxi_core }
    {}


    void interface_system::load(){}

    void interface_system::load_style(const QString& name)
    {
        style_ = nxi::style{ name };
        style_.load();
        emit event_update_style(style_);
    }

    nxi::style& interface_system::style()
    {
        return style_;
    }
} // nxi