#include <nxi/system/interface.hpp>

#include <QApplication>
#include <nxi/core.hpp>
#include <nxi/style.hpp>
#include <QDir>

namespace nxi
{
    interface_system::interface_system(nxi::core& nxi_core)
         : nxi_core_{ nxi_core }
    {}


    void interface_system::load()
    {
        load_style();
    }

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

    std::vector<QString> interface_system::styles() const
    {
        std::vector<QString> styles;
        QDir dir ("./module/theme/");
        QStringList list = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        auto vec = list.toVector();
        return std::vector<QString>(vec.begin(), vec.end());
    }
} // nxi