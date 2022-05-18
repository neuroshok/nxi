#include <ui/core.hpp>

#include <nxi/core.hpp>

#include <ui/window.hpp>

#include <QApplication>
#include <QSystemTrayIcon>
#include <QWidgetList>

namespace ui
{
    core::core(QApplication& app, nxi::core& nxi_core)
        : app_{ app }
        , nxi_core_{ nxi_core }
        , interface_system_{ *this }
        , user_system_{ *this }
    {
        // systray
        systray_ = new QSystemTrayIcon;
        systray_->setIcon(QIcon(":/image/nex"));
        systray_->show();

        connect(&nxi_core_, &nxi::core::event_quit, this, &core::quit);
    }

    void core::quit()
    {
        // session_system_.unload();

        QApplication::quit();
    }

    nxi::core& core::nxi_core() { return nxi_core_; }

    ui::interface_system& core::interface_system() { return interface_system_; }
} // ui