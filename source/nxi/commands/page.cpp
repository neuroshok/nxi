#include <nxi/command/initializer.hpp>

#include <nxi/core.hpp>
#include <nxi/values.hpp>

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
        page_new.function = [this](const nxi::values&){ nxi_core_.page_system().open<nxi::web_page>(0); };
        add(std::move(page_new));

        // open
        nxi::command_data page_open;
        page_open.action = "new";
        page_open.description = "Open a web_page";
        page_open.function = [this](const nxi::values& params)
        {
            auto url = params.get(0);
            nxi_core_.page_system().open<nxi::web_page>(0, url);
        };
        page_open.parameters = {
        { "command", [](nxi::suggestion_vector& suggestion)
            {
                suggestion.push_back("www.twitch.com");
                suggestion.push_back("www.google.com");
            }
        }};

        add(std::move(page_open));

        // switch
        nxi::command_data page_switch;
        page_switch.action = "switch";
        page_switch.icon = ":/icon/switch";
        page_switch.description = "Switch between all pages";
        page_switch.shortcut = {{ Qt::Key_Control }, { Qt::Key_W, Qt::Key_S }};
        page_switch.preview = true;
        page_switch.function = [this](const nxi::values& params)
        {
            if (params.values_.size() < 1)
            {
                nxi_warning("command_param error : page_switch");
                return;
            }
            auto id = params.get(0).toUInt();
            nxi_core_.page_system().focus(id);
        };
        page_switch.parameters = {
        { "id", [this](nxi::suggestion_vector& suggestion)
            {
                for (auto& page : nxi_core_.page_system().get())
                {
                    suggestion.push_back(nxi::text_suggestion{ QString::number(page->id()), "", page->command() });
                }
            }
        }};

        add(std::move(page_switch));

        // close
        nxi::command_data page_close;
        page_close.action = "close";
        page_close.description = "Close active page";
        page_close.shortcut = {{ Qt::Key_Control }, { Qt::Key_W, Qt::Key_X }};
        page_close.function = [this](const nxi::values& params)
        {
            // close
        };
        add(std::move(page_close));
    }
} // nxi