#include <nxi/command/initializer.hpp>

#include <nxi/core.hpp>
#include <nxi/page/web.hpp>
#include <nxi/values.hpp>

namespace nxi
{
    nds::node_ptr<nxi::command> command_initializer::init_page()
    {
        auto node = add_node("page");

        // new
        nxi::command_data page_new;
        page_new.action = "new";
        page_new.description = "Open new page";
        page_new.context_id = nxi::context::id<nxi::contexts::command>();
        page_new.shortcut = {{ Qt::Key_Control }, { Qt::Key_T }, nxi::context::id<nxi::contexts::page>() };
        //page_new.shortcut = {{ Qt::Key_Control }, { Qt::Key_T }, nxi::contexts::page };
        page_new.function = [this](const nxi::values&){ session_.page_system().open<nxi::web_page>(); };
        add(std::move(page_new));

        // open
        nxi::command_data page_open;
        page_open.action = "new";
        page_open.description = "Open a web_page";
        page_open.function = [this](const nxi::values& params)
        {
            auto url = params.get(0);
            session_.page_system().open<nxi::web_page>(url);
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
            session_.page_system().focus(id);
        };
        page_switch.parameters = {
        { "id", [this](nxi::suggestion_vector& suggestion)
            {
                for (auto& page : session_.page_system().pages())
                {
                    suggestion.push_back(page);
                }
            }
        }};

        add(std::move(page_switch));

        // close
        nxi::command_data page_close;
        page_close.action = "close";
        page_close.description = "Close focused page";
        page_close.shortcut = {{ Qt::Key_Control }, { Qt::Key_W, Qt::Key_X }};
        page_close.function = [this](const nxi::values& params)
        {
            session_.page_system().close_focus();
        };
        add(std::move(page_close));

        // fullscreen
        nxi::command_data fullscreen;
        fullscreen.action = "fullscreen";
        fullscreen.description = "fullscreen";
        fullscreen.shortcut = {{ Qt::Key_Control }, { Qt::Key_Enter }};
        fullscreen.function = [this](const nxi::values& params)
        {
            //session_.page_system().focus()->run("document.documentElement.requestFullscreen();");
        };
        add(std::move(fullscreen));

        // page_run_script
        nxi::command_data page_run_script;
        page_run_script.action = "run_script";
        page_run_script.description = "Run javascript";
        page_run_script.function = [this](const nxi::values& params)
        {
            //nxi_expect(params.size() == 1);
            session_.page_system().focus()->run_script(params.get(0));
        };
        page_run_script.parameters = {{ "script" }};
        add(std::move(page_run_script));

        // links
        nxi::command_data links;
        links.action = "links";
        links.description = "Follow a link";
        links.function = [this](const nxi::values& params)
        {
            //nxi_expect(params.size() == 1);
            session_.error(params.get(0));
        };
        links.parameters = {
        { "link", [this](nxi::suggestion_vector& suggestion)
            {
                auto script = R"__(
                (() => {
                    let output = [];
                    var links = document.getElementsByTagName('a');
                    for (let i = 0; i < links.length; ++i) {
                        let link = links[i];
                        let str = link.innerHTML + " - " + link.href;
                        output.push( str );
                    }
                    return output;
                })();
                )__";

                static_cast<nxi::web_page&>(*session_.page_system().focus()).run_script(script, [&suggestion](const QVariant& variant)
                {
                    for (const auto& item : variant.toList())
                    {
                        suggestion.push_back(item.toString());
                    }
                    emit suggestion.event_update(stz::make_observer(&suggestion));

                    //session_.command_system().command_input().suggest(vector);
                });
            }
        }};
        add(std::move(links));

        return node;
    }
} // nxi