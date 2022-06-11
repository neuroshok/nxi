#include <ui/interface/light/control_bar.hpp>

#include <nxi/core.hpp>
#include <nxi/log.hpp>
#include <nxi/notification_data.hpp>
#include <nxi/system/notification.hpp>
#include <nxi/system/window.hpp>
#include <nxi/user.hpp>

#include <nxw/hbox_layout.hpp>
#include <nxw/icon_button.hpp>

#include <ui/command/input.hpp>
#include <ui/core.hpp>
#include <ui/interface/light/button.hpp>
#include <ui/window.hpp>

#include <QIcon>

namespace ui::interfaces::light
{
    control_bar::control_bar(ui::user& user, ui::window* window)
        : ui::interface("control_bar", window)
        , user_{ user }
    {
        nxi_trace("");

        auto* layout = new nxw::hbox_layout;
        setLayout(layout);

        if (user_.nxi_user().config().browser.interface.light.console_mode.get())
        {
            command_input_ = new ui::command_input(user_, this);
            command_input_->setFocus();
            layout->addWidget(command_input_);
            return;
        }

        // session
        auto session_button = new light::button("session", this);
        session_button->setStyleSheet("font-weight: bold; color: #7722FF; padding: 0 20 0 20;");
        session_button->style_data.text_color = QColor{ 255, 0, 0 };
        connect(&user_.nxi_core().session_system(), &nxi::session_system::event_focus, this,
                [session_button](nxi::session& s) { session_button->setText(s.name()); });

        connect(session_button, &light::button::event_enter, this,
                [this]() { user_.nxi_user().buffer_system().group(ui_window()->id()).suggest_session(); });

        // command_root
        command_root_ = new light::button("command_root_", this);
        command_root_->setStyleSheet("font-weight: bold; color: #00BBFF; padding: 0 20 0 20;");
        command_root_->style_data.text_color = QColor{ 0, 187, 255 };
        connect(command_root_, &light::button::event_enter, this, [this]() {
            user_.nxi_user().context_system().focus<nxi::contexts::command>();
            buffer_group().suggest_command();
        });
        connect(command_root_, &light::button::event_mousewheel_up, this, [this]() { buffer_group().suggestions().select_previous(); });
        connect(command_root_, &light::button::event_mousewheel_down, this, [this]() { buffer_group().suggestions().select_next(); });
        connect(&buffer_group(), &nxi::buffer_group::event_command_root_update, this,
                [this](nds::node_ptr<nxi::command> command) { command_root_->setText(command->action_name()); });

        // page_root
        page_root_ = new light::button("page_root_", this);
        page_root_->setStyleSheet("font-weight: bold; background-color: #0F1419; color: #FFBB00; padding: 0 20 0 20;");
        page_root_->style_data.text_color = QColor{ 255, 187, 0 };
        connect(page_root_, &light::button::event_enter, this, [this]() {
            user_.nxi_user().context_system().focus<nxi::contexts::page>();
            buffer_group().suggest_page();
        });
        connect(&buffer_group(), &nxi::buffer_group::event_page_root_update, this,
                [this](nds::node_ptr<nxi::page> page) { page_root_->setText(page->name()); });

        // navigation
        auto navigation = new light::button("< o >", this);
        navigation->setStyleSheet("font-weight: bold; color: #BB2200; padding: 0 20 0 20;");
        connect(navigation, &light::button::event_enter, this, [this]() { buffer_group().suggest_navigation(); });

        // context
        context_ = new light::button("context_", this);
        context_->setStyleSheet("font-weight: bold; color: #00BB99; padding: 0 20 0 20;");
        connect(context_, &light::button::event_enter, this, [this]() { buffer_group().suggest_context(); });

        // notification
        auto notification_ = new nxw::icon_button{ user_, this, ":/icon/notification", "" };

        connect(notification_, &QPushButton::clicked, this, [this] { user_.nxi_core().notification_system().send("notif"); });

        // tools
        auto download_button = new nxw::icon_button{ user_, this, ":/icon/download" };
        auto close_button = new nxw::icon_button{ user_, this, ":/icon/close" };
        connect(close_button, &QPushButton::clicked, this, [this] { user_.nxi_core().window_system().close(ui_window()->id()); });

        if (user_.nxi_user().config().browser.interface.light.console_mode.get())
        {
            command_input_ = new ui::command_input(user_, this);
            command_input_->setFocus();
            layout->addWidget(command_input_);
        }
        else
        {
            command_input_ = new ui::command_input(user_, this);
            command_input_->setFocus();

            auto label = new QLabel{ this };
            auto icon = new QIcon{ ":/image/nxi" };
            label->setPixmap(icon->pixmap(16, 16));
            layout->addWidget(label);

            layout->addWidget(session_button);
            layout->addWidget(command_root_);
            layout->addWidget(page_root_);
            layout->addWidget(navigation);
            layout->addWidget(new nxw::icon_button{ user_, this, ":/icon/refresh", "nxi:reload" });
            layout->addWidget(new nxw::icon_button{ user_, this, ":/icon/previous" });
            layout->addWidget(new nxw::icon_button{ user_, this, ":/icon/next" });
            layout->addWidget(new nxw::icon_button{ user_, this, ":/icon/add", "nxi:new" });
            layout->addWidget(command_input_);
            layout->addWidget(new nxw::icon_button{ user_, this, ":/icon/close", "nxi:close" });
            layout->addWidget(context_);
            layout->addWidget(new nxw::icon_button{ user_, this, ":/icon/at" });
            layout->addWidget(new nxw::icon_button{ user_, this, ":/icon/friend" });
            layout->addWidget(notification_);
            layout->addWidget(download_button);
            layout->addWidget(new nxw::icon_button{ user_, this, ":/icon/help" });
            layout->addWidget(close_button);
        }

        /*
        connect(&buffer_group(), &nxi::page_system::event_root_update,
        [this](nds::node_ptr<nxi::command> command)
        {
            command_root_->setText(command->action_name());
        });*/

        connect(&user_.nxi_user().context_system(), &nxi::context_system::event_focus_context_update, this,
                [this](const nxi::context& context) { context_->setText("[" + context.name() + "]"); });

        connect(&user_.nxi_user().context_system(), &nxi::context_system::event_context_add, this, [this](const nxi::context& context) {
            context.apply([this](const nxi::contexts::command&) { command_root_->activate(); },
                          [this](const nxi::contexts::page&) { page_root_->activate(); }, [this](auto&&) {});
        });

        connect(&user_.nxi_user().context_system(), &nxi::context_system::event_context_del, this, [this](const nxi::context& context) {
            context.apply([this](const nxi::contexts::command&) { command_root_->activate(false); },
                          [this](const nxi::contexts::page&) { page_root_->activate(false); }, [this](auto&&) {});
        });
    }

    ui::command_input* control_bar::command_input() { return command_input_; }

    void control_bar::enterEvent(QEnterEvent* event) {}

    void control_bar::leaveEvent(QEvent* event) {}

    nxi::buffer_group& control_bar::buffer_group() { return user_.nxi_user().buffer_system().group(interface::group_id()); }
} // ui::interfaces