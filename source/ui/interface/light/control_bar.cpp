#include <ui/interface/light/control_bar.hpp>

#include <nxi/core.hpp>
#include <nxi/log.hpp>
#include <nxw/hbox_layout.hpp>

#include <ui/command.hpp>
#include <ui/core.hpp>
#include <ui/interface/light/button.hpp>

namespace ui::interfaces::light
{
    control_bar::control_bar(ui::core& ui_core, ui::window* window)
        : ui::interface("control_bar", window)
        , ui_core_{ ui_core }
    {
        nxi_trace("");

        auto* layout = new nxw::hbox_layout;
        setLayout(layout);

        // command_root
        command_root_ = new light::button("command_root_", this);
        connect(command_root_, &light::button::event_enter, [this]()
        {
            ui_core_.nxi_core().command_system().command_input().suggest_command();
            //command_root_->setStyleSheet("font-weight: bold; background-color: #00FFFF; color: #00BBFF; padding: 0 20 0 20;");
        });
        connect(command_root_, &light::button::event_mousewheel_up, [this]() { ui_core_.nxi_core().command_system().command_input().suggestions().select_previous(); });
        connect(command_root_, &light::button::event_mousewheel_down, [this]() { ui_core_.nxi_core().command_system().command_input().suggestions().select_next(); });

        // page_root
        page_root_ = new light::button("page_root_", this);
        connect(page_root_, &light::button::event_enter, [this]()
        {
            ui_core_.nxi_core().command_system().command_input().suggest_page();
        });

        command_input_ = new ui::command(ui_core_);
        command_input_->setFocus();
        layout->addWidget(command_root_);
        layout->addWidget(page_root_);
        layout->addWidget(command_input_);

        // context
        context_ = new light::button("context_", this);
        connect(context_, &light::button::event_enter, [this]()
        {
            ui_core_.nxi_core().command_system().command_input().suggest_context();
        });
        layout->addWidget(context_);

        connect(&ui_core_.nxi_core().command_system(), &nxi::command_system::event_root_update,
        [this](nds::node_ptr<nxi::command> command)
        {
            command_root_->setText(command->action_name());
        });

        /*
        connect(&ui_core_.nxi_core().command_system(), &nxi::page_system::event_root_update,
        [this](nds::node_ptr<nxi::command> command)
        {
            command_root_->setText(command->action_name());
        });*/

        connect(&ui_core_.nxi_core().context_system(), &nxi::context_system::event_focus_context_update,
        [this](const nxi::context& context)
        {
            context_->setText(context.name());
        });

        connect(&ui_core_.nxi_core().context_system(), &nxi::context_system::event_context_add,
        [this](const nxi::context& context)
        {
            context.apply(
            [this](const nxi::contexts::command&) { command_root_->activate(); }
            , [this](const nxi::contexts::page&) { page_root_->activate(); }
            , [this](auto&&) {}
            );
        });

        connect(&ui_core_.nxi_core().context_system(), &nxi::context_system::event_context_del,
        [this](const nxi::context& context)
        {
            context.apply(
            [this](const nxi::contexts::command&) { command_root_->activate(false); }
            , [this](const nxi::contexts::page&) { page_root_->activate(false); }
            , [this](auto&&) {}
            );
        });
    }

    ui::command* control_bar::command_input()
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