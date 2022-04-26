#include "strateon.hpp"

#include <nxi/core.hpp>
#include <nxi/window.hpp>
#include <nxi/system/window.hpp>
#include <nxi/log.hpp>
#include <module/strateon/Core/MainWindow.h>


#include <nxi/page/custom.hpp>
#include <QtCore/qglobal.h>



namespace nxi::modules
{
    strateon::strateon(nxi::session& nxi_core)
        : module("strateon", module_type::compiled)
        , nxi_core_{ nxi_core }
    {}


    void strateon::on_load()
    {
        return;
        //nxi_log << "load static module strateon";


        // custom_page
        /*
        QObject::connect(&session_.page_system(), qOverload<nxi::custom_page&, nxi::page_id>(&nxi::page_system::event_add), [this](nxi::custom_page& page, nxi::page_id source_id)
        {
            auto win = new MainWindow;
            win->show();
            nxi_core_.test(win);
            //page.update_name("document.txt");
        });


        auto action = [this]()
        {
            //custom_page.on_add([](){ auto win = new MainWindow; })
            //custom_page.on_render([](){ auto win = new MainWindow; })
            session_.page_system().add<custom_page>(0);
        };

        auto cmd = nxi::command("strateon", "main", action, ":/strateon/logo");



        session_.command_system().add(std::move(cmd));

         */
    }

} // nxi::modules