#include <ui/interface/standard/control_bar.hpp>

#include <ui/core.hpp>
#include <nxi/command.hpp>
#include <nxi/core.hpp>
#include <nxi/system/command.hpp>

#include <QComboBox>
#include <QLabel>
#include <QLineEdit>

#include <nxi/config.hpp>
#include <nxi/log.hpp>
#include <nxi/module/web.hpp>
#include <nxw/hbox_layout.hpp>
#include <nxw/icon_button.hpp>
#include <nxw/menu.hpp>
#include <nxw/menu/item.hpp>
#include <ui/interface/standard/control_bar.hpp>
#include <QWebEngineView>

#include <ui/command/input.hpp>
#include <ui/window.hpp>

namespace nxw
{
    class popup : public QWebEngineView
    {
    public:
        popup(const QString& url)
        {
            load(QUrl::fromLocalFile(url));
        }
    };
}

namespace ui::interfaces::standard
{
    class window_controls : public ui::interface
    {
    public:
        window_controls(ui::user_session& session, ui::window* w) : interface(w)
        {
            /*
            auto layout = new nxw::hbox_layout;
            setLayout(layout);

            auto win_minimize = new nxw::icon_button(this, ":/icon/minimize");
            QObject::connect(win_minimize, &QPushButton::pressed, [this, &session]()
            {
                session.window_system().minimize(window());
            });

            auto win_close = new nxw::icon_button(this, ":/icon/close");
            QObject::connect(win_close, &QPushButton::pressed, &ui_core, &ui::core::quit);

            layout->addWidget(win_minimize);
            layout->addWidget(new nxw::icon_button(this, ":/icon/maximize"));
            layout->addWidget(win_close);*/
        }
    };

    control_bar::control_bar(ui::core& ui_core, ui::window* window)
        : ui::interface("control_bar", window)
        , m_ui_core{ ui_core }
    {
        nxi_trace("");

        QHBoxLayout* layout = new nxw::hbox_layout;
        setLayout(layout);

        //auto btn_menu = new nxw::icon_button(this, ":/icon/menu");


        auto menu = new nxw::menu{ this };
        //menu->add<nxw::menu_item>("new window", [&ui_core](){ window_system().add({}); });
        //menu->add<nxw::menu_item>(ui_core.nxi_core().command_system().get("quit").name(), [&ui_core](){  ui_core.nxi_core().command_system().get("quit").exec(); } );

/*        menu->add(ui_core.nxi_core().command_system().get("config"));
        menu->add(ui_core.nxi_core().command_system().get("about"));
        //menu->add<nxw::menu_separator>();
        menu->add(ui_core.nxi_core().command_system().get("quit"));
        menu->add(ui_core.nxi_core().command_system().get("test"));
        menu->show_at(btn_menu);*/

        //QObject::connect(btn_menu, &nxw::icon_button::clicked, menu, &nxw::menu::exec);
        //menu->add(nxi::command::get("nxi", "quit"), customwidget);


        m_context = new QComboBox(this);
        m_context->addItem("auto");
        m_context->addItem("web");
        m_context->addItem("explorer");
        m_context->hide();

        //command_bar_ = new ui::command(session_);

        // module command
        // init
        /*
        module_controls_ = new nxw::hbox_layout;
        for (const auto& command : m_ui_core.nxi_core().command_system().get())
        {
            command_add(*command);
        }
         */
        // event
        /*
        QObject::connect(&ui_core.nxi_core().command_system(), &nxi::command_system::event_add, [this](const nxi::command& command)
        {
            command_add(command);
        });*/


        //auto window_controls = new ui::interfaces::standard::window_controls(ui_core, window);

        //layout->addWidget(btn_menu);
        layout->addSpacing(64);
/*
        layout->addWidget(new nxw::icon_button(this, ":/icon/previous"));
        layout->addWidget(new nxw::icon_button(this, ":/icon/next"));
        layout->addWidget(new nxw::icon_button(this, ":/icon/message"));
        layout->addWidget(new nxw::icon_button(this, ":/icon/download"));*/

        //ui::window::get(this)->
        static_cast<ui::window*>(this->window())->set_grip(this);

        layout->addWidget(m_context);
        layout->addWidget(command_bar_, 1);
        layout->addLayout(module_controls_);
        layout->addStretch();
        //layout->addWidget(window_controls);
    }

    void control_bar::command_add(const nxi::command& command)
    {
        /*
        auto button = new nxw::icon_button(this, command.icon());
        connect(button, &nxw::icon_button::clicked, [this, &command]()
        {

            nxi::internal_command
            nxi::web_module_command

            if (command.is_internal()) command.exec();
            if (command.is_webaction())
            {

            }

            //
            command.exec();


            QString path = "C:\\Projet\\nk\\nex\\bin\\nxi\\module\\webextension\\beastify\\popup\\choose_beast.html";
            auto popup = new nxw::popup(path);
            popup->show();
        });

        button->setText(command.name());

        //popup->show_at(button);

        module_controls_->addWidget(button);
        */
    }
} // ui::interfaces