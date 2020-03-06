#include <nxi/command/initializer.hpp>

#include <nxi/core.hpp>

namespace nxi
{
    void command_initializer::init_page()
    {
        add_node("page");

        // new
        nxi::command_data page_new;
        page_new.action = "new";
        page_new.description = "Open new web_page";
        page_new.shortcut = {{ Qt::Key_Control }, { Qt::Key_T }};
        page_new.function = [this](const nxi::command_params&){ nxi_core_.page_system().open<nxi::web_page>(0); };
        add(std::move(page_new));

        // open
        add("open", [this](const nxi::command_params& params)
        {
            auto url = params.get(0);
            nxi_core_.page_system().open<nxi::web_page>(0, url);
        }, ":/image/nex");

        add_param("command", [](nxi::suggestion_vector& suggestion)
        {
            suggestion.add("www.twitch.com");
            suggestion.add("www.google.com");
        });

        // switch
        nxi::command_data page_switch;
        page_switch.action = "switch";
        page_switch.icon = ":/icon/switch";
        page_switch.description = "Switch between all pages";
        page_switch.shortcut = {{ Qt::Key_Control }, { Qt::Key_W, Qt::Key_S }};
        page_switch.preview = true;
        page_switch.function = [this](const nxi::command_params& params)
        {
            if (params.values_.size() < 1)
            {
                nxi_warning("command_param error : page_switch");
                return;
            }
            auto id = params.get(0).toUInt();
            nxi_core_.page_system().focus(id);
        };

        add(std::move(page_switch));
        add_param("id", [this](nxi::suggestion_vector& suggestion)
        {
            for (auto& page : nxi_core_.page_system().get())
            {
                suggestion.add(nxi::suggestion{ QString::number(page->id()), "", page->command() });
            }
        });

        // close
        nxi::command_data page_close;
        page_close.action = "close";
        page_close.description = "Close active page";
        page_close.shortcut = {{ Qt::Key_Control }, { Qt::Key_W, Qt::Key_X }};
        page_close.function = [this](const nxi::command_params& params)
        {
            // close
        };
        add(std::move(page_close));
    }
} // nxi