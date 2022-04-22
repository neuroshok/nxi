#include <ui/core.hpp>

#include <nxi/core.hpp>
#include <nxi/database.hpp>
#include <nxi/system/interface.hpp>
#include <nxi/style.hpp>
#include <nxi/log.hpp>

#include <ui/system/page.hpp>
#include <ui/system/window.hpp>
#include <ui/interface/standard/main.hpp>
#include <ui/interface/light/main.hpp>
#include <ui/window.hpp>

#include <QApplication>
#include <QFile>
#include <QSystemTrayIcon>
#include <QWidgetList>



namespace ui
{
    core::core(QApplication& app, nxi::core& nxi_core) :
        app_{ app }
        , nxi_core_{ nxi_core }
        , session_system_{ *this }
    {
        // systray
        systray_ = new QSystemTrayIcon;
        systray_->setIcon(QIcon(":/image/nex"));
        systray_->show();

        connect(&nxi_core_, &nxi::core::event_quit, this, &core::quit);
    }

    void core::quit()
    {
        //session_system_.unload();

        QApplication::quit();
    }

    nxi::core& core::nxi_core()
    {
        return nxi_core_;
    }
} // ui