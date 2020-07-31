#include <ui/interface/light/control_bar.hpp>

#include <nxi/core.hpp>
#include <nxi/log.hpp>
#include <nxw/hbox_layout.hpp>

#include <ui/command/input.hpp>
#include <ui/core.hpp>
#include <ui/interface/light/button.hpp>

namespace ui::interfaces::light
{
    control_bar::control_bar(ui::session& session, ui::window* window)
        : ui::interface("control_bar", window)
        , session_{ session }
    {
        nxi_trace("");

        auto* layout = new nxw::hbox_layout;
        setLayout(layout);

        // command_root
        command_root_ = new light::button("command_root_", this);
        command_root_->setStyleSheet("font-weight: bold; color: #00BBFF; padding: 0 20 0 20;");
        command_root_->style_data.text_color = QColor{ 0, 187, 255 };
        connect(command_root_, &light::button::event_enter, [this]()
        {
          session_.nxi_session().context_system().focus<nxi::contexts::command>();
          session_.nxi_session().command_system().command_input().suggest_command();
        });
        connect(command_root_, &light::button::event_mousewheel_up, [this]() { session_.nxi_session().command_system().command_input().suggestions().select_previous(); });
        connect(command_root_, &light::button::event_mousewheel_down, [this]() { session_.nxi_session().command_system().command_input().suggestions().select_next(); });
        connect(&session_.nxi_session().command_system(), &nxi::command_system::event_root_update,
        [this](nds::node_ptr<nxi::command> command)
        {
            command_root_->setText(command->action_name());
        });

        // page_root
        page_root_ = new light::button("page_root_", this);
        page_root_->setStyleSheet("font-weight: bold; background-color: #0F1419; color: #FFBB00; padding: 0 20 0 20;");
        page_root_->style_data.text_color = QColor{ 255, 187, 0 };
        connect(page_root_, &light::button::event_enter, [this]()
        {
          session_.nxi_session().context_system().focus<nxi::contexts::page>();
          session_.nxi_session().command_system().command_input().suggest_page();
        });
        connect(page_root_, &light::button::event_mousewheel_up, [this]() {
            session_.nxi_session().command_system().command_input().suggestions().select_previous();
            session_.nxi_session().command_system().command_input().exec();
        });
        connect(page_root_, &light::button::event_mousewheel_down, [this]() {
            session_.nxi_session().command_system().command_input().suggestions().select_next();
            session_.nxi_session().command_system().command_input().exec();
        });
        connect(&session_.nxi_session().page_system(), &nxi::page_system::event_update_root,
        [this](nds::node_ptr<const nxi::page> page)
        {
            page_root_->setText(page->name());
        });

        // navigation
        auto navigation = new light::button("< o >", this);
        navigation->setStyleSheet("font-weight: bold; color: #BB2200; padding: 0 20 0 20;");
        connect(navigation, &light::button::event_enter, [this]()
        {
            session_.nxi_session().command_system().command_input().suggest_navigation();
        });

        command_input_ = new ui::command_input(session_);
        command_input_->setFocus();

        // context
        context_ = new light::button("context_", this);
        context_->setStyleSheet("font-weight: bold; color: #00BB99; padding: 0 20 0 20;");
        connect(context_, &light::button::event_enter, [this]()
        {
            session_.nxi_session().command_system().command_input().suggest_context();
        });

        layout->addWidget(command_root_);
        layout->addWidget(page_root_);
        layout->addWidget(navigation);
        layout->addWidget(command_input_);
        layout->addWidget(context_);

        /*
        connect(&session_.nxi_session().command_system(), &nxi::page_system::event_root_update,
        [this](nds::node_ptr<nxi::command> command)
        {
            command_root_->setText(command->action_name());
        });*/

        connect(&session_.nxi_session().context_system(), &nxi::context_system::event_focus_context_update,
        [this](const nxi::context& context)
        {
            context_->setText("[" + context.name() + "]");
        });

        connect(&session_.nxi_session().context_system(), &nxi::context_system::event_context_add,
        [this](const nxi::context& context)
        {
            context.apply(
            [this](const nxi::contexts::command&) { command_root_->activate(); }
            , [this](const nxi::contexts::page&) { page_root_->activate(); }
            , [this](auto&&) {}
            );
        });

        connect(&session_.nxi_session().context_system(), &nxi::context_system::event_context_del,
        [this](const nxi::context& context)
        {
            context.apply(
            [this](const nxi::contexts::command&) { command_root_->activate(false); }
            , [this](const nxi::contexts::page&) { page_root_->activate(false); }
            , [this](auto&&) {}
            );
        });
    }

    ui::command_input* control_bar::command_input()
    {
        return command_input_;
    }

    void control_bar::enterEvent(QEvent* event)
    {

    }

    void control_bar::leaveEvent(QEvent* event)
    {

    }
} // ui::interfaces