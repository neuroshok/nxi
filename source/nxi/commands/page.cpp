#include <nxi/command/initializer.hpp>

#include <nxi/core.hpp>
#include <nxi/page/web.hpp>
#include <nxi/values.hpp>
#include <nxi/system/page.hpp>

namespace nxi
{
    nds::node_ptr<nxi::command> command_initializer::init_page()
    {
        auto node = add_node("page");

        // new
        nxi::command_data page_new;
        page_new.action = "new";
        page_new.icon = ":/icon/add";
        page_new.description = "Open new page";
        page_new.context_id = nxi::context::id<nxi::contexts::command>();
        page_new.shortcut = {{ Qt::Key_Control }, { Qt::Key_T }, nxi::context::id<nxi::contexts::page>() };
        //page_new.shortcut = {{ Qt::Key_Control }, { Qt::Key_T }, nxi::contexts::page };
        page_new.function = [this](const nxi::values&) {
            core_.page_system().open<nxi::web_page>();
            // auto p = core_.page_system().focus();
            // p->update_session(core_.session().id());
        };
        add(std::move(page_new));

        // open
        nxi::command_data page_open;
        page_open.action = "new";
        page_open.description = "Open a web_page";
        page_open.function = [this](const nxi::values& params)
        {
            auto url = params.get(0);
            core_.page_system().open<nxi::web_page>(url);
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
            core_.page_system().focus(id);
        };
        page_switch.parameters = {
        { "id", [this](nxi::suggestion_vector& suggestion)
            {
                for (auto& page : core_.page_system().pages())
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
            core_.page_system().close_focus();
        };
        add(std::move(page_close));

        // fullscreen
        nxi::command_data fullscreen;
        fullscreen.action = "fullscreen";
        fullscreen.description = "fullscreen";
        fullscreen.shortcut = {{ Qt::Key_Control }, { Qt::Key_Enter }};
        fullscreen.function = [this](const nxi::values& params)
        {
            // core_.page_system().focus().run("document.documentElement.requestFullscreen();");
        };
        add(std::move(fullscreen));

        // page_run_script
        nxi::command_data page_run_script;
        page_run_script.action = "run_script";
        page_run_script.description = "Run javascript";
        page_run_script.function = [this](const nxi::values& params)
        {
            //nxi_expect(params.size() == 1);
            core_.page_system().focus()->run_script(params.get(0));
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
            //core_.error(params.get(0));
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

                static_cast<nxi::web_page&>(*core_.page_system().focus()).run_script(script, [&suggestion](const QVariant& variant)
                {
                    for (const auto& item : variant.toList())
                    {
                        suggestion.push_back(item.toString());
                    }
                    emit suggestion.event_update(stz::make_observer(&suggestion));

                    //core_.command_system().command_input().suggest(vector);
                });
            }
        }};
        add(std::move(links));

        // navigate_next
        nxi::command_data navigate_next;
        navigate_next.action = "next_page";
        navigate_next.description = "Navigate to next page";
        navigate_next.shortcut = {{ Qt::Key_Alt }, { Qt::Key_Right }};
        navigate_next.function = [this](const nxi::values& params)
        {
            core_.navigation_system().next_page_command();
        };
        add(std::move(navigate_next));

        // navigate_previous
        nxi::command_data navigate_previous;
        navigate_previous.action = "previous_page";
        navigate_previous.description = "Navigate to previous page";
        navigate_previous.shortcut = { { Qt::Key_Alt }, { Qt::Key_Left } };
        navigate_previous.function = [this](const nxi::values& params) { core_.navigation_system().previous_page_command(); };
        add(std::move(navigate_previous));

        // reload
        nxi::command_data page_reload;
        page_reload.action = "reload";
        page_reload.description = "Reload this page";
        page_reload.context_id = nxi::context::id<nxi::contexts::page>();
        page_reload.function = [this](const nxi::values& values) { core_.page_system().focus()->reload(); };
        add(std::move(page_reload));

        // set_property
        nxi::command_data page_set_property;
        page_set_property.action = "set_property";
        page_set_property.description = "Set a property";
        page_set_property.context_id = nxi::context::id<nxi::contexts::command>();
        page_set_property.function = [this](const nxi::values& values) {
            nxi_assert(values.size() == 2);
            core_.page_system().focus()->update_property(values.get(0), values.get(1));
        };

        page_set_property.parameters = { { "name",
                                           [this](nxi::suggestion_vector& suggestion) {
                                               suggestion.push_back("color");
                                               suggestion.push_back("name");
                                           } },
                                         { "value" } };

        add(std::move(page_set_property));

        return node;
    }
} // nxi