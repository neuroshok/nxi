#include <nxi/system/interface.hpp>

#include <QApplication>
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
        auto style = new nxi::style(name);
        style->load();
        QApplication::setStyle(style); // QApplication own style
        emit event_update_style(*style);
    }

    nxi::style& interface_system::style()
    {
        return *static_cast<nxi::style*>(QApplication::style());
    }
} // nxi