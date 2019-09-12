#include <nxi/system/interface.hpp>

#include <nxi/core.hpp>
#include <nxi/theme.hpp>

namespace nxi
{
    interface_system::interface_system(nxi::core& nxi_core)
         : nxi_core_{ nxi_core }
    {}


    void interface_system::load(){}

    void interface_system::load_theme(const QString& name)
    {
        theme_ = nxi::theme{ name };
        theme_.load();
        emit event_load_theme(theme_);
    }

    nxi::theme& interface_system::theme()
    {
        return theme_;
    }
} // nxi