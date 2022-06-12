#include <nxi/system/command.hpp>

#include <nxi/command.hpp>
#include <nxi/core.hpp>

#include <nxi/page/web.hpp>
#include <nxi/system/command.hpp>
#include <nxi/system/interface.hpp>
#include <nxi/system/module.hpp>
#include <nxi/system/page.hpp>
#include <nxi/values.hpp>

#include <nds/algorithm/graph.hpp>
#include <nds/encoder/graph.hpp>

namespace nds::encoders
{
    template<>
    template<>
    std::string dot<void>::node_name(const nxi::command& p)
    {
        return p.name().toStdString();
    }
} // nds::encoders

namespace nxi
{
    command_system::command_system(nxi::core& core)
        : core_{ core }
        , command_initializer_{ core }
        , root_{ nullptr }
    {}

    void command_system::load()
    {
        nxi_trace("");
        command_initializer_.load();

        // init trigger keys
        for_each([this](auto&& node) {
            const nxi::shortcut& shortcut = node->shortcut();
            // if (shortcut.combo_keys.size() > 0) command_input().shortcut_input().add_trigger_key(shortcut.combo_keys[0]);
            // else if (shortcut.sequence_keys.size() > 0) command_input().shortcut_input().add_trigger_key(shortcut.sequence_keys[0]);
        });

        // auto cmd_page = add(nxi::command("nxi", "page", [this](const nxi::values&){ session_.page_system().focus(); }), main_cmd);
        // cmd_page->get().add_param("page_id", session_.page_system().get());

        // nds::encoders::dot<>::encode<nds::console>(graph_);

        // load module commands
        /*
        for (auto& module : nxi_core_.module_system().get())
        {
            if (module->type() == module_type::web)
            {
                auto web_module = static_cast<const nxi::web_module&>(*module);
                nxi::command cmd(module->name(), web_module.browser_action().default_title, std::bind(&nxi::core::quit, &nxi_core_),
        "module/webextension/test/" + web_module.browser_action().default_icon); add(std::move(cmd));
            }
        }*/

        /*        nxi::module_load
                if (is_module_command) nxi.command_system()get(cmd).exec()
                else ui.command_system().get(cmd).exec()

                button { "nxi:quit" }*/
    }

    nds::node_ptr<nxi::command> command_system::find(const QString& module_action, const QString& module_name) const
    {
        nds::node_ptr<nxi::command> command;

        nds::algorithm::graph::find_first_if(
        graph_,
        [&module_action, &module_name](auto&& node) { return node->action_name() == module_action && node->module_name() == module_name; },
        [&command](auto&& found_node) { command = found_node; });

        return command;
    }

    const nxi::command& command_system::get(const QString& action_name, const QString& module_name) const
    {
        nds::node_ptr<nxi::command> command = find(action_name, module_name);

        if (!command) nxi_error("nxi::command not found : {}", action_name);
        return *command;
    }

    nds::node_ptr<nxi::command> command_system::add(nxi::command command, nds::node_ptr<nxi::command> source)
    {
        return graph_.add(std::move(command), source);
    }

    void command_system::exec(nds::node_ptr<const nxi::command> command) const
    {
        if (command->parameters_count() > 0) emit event_execution_request(command);
        else exec(command, nxi::values{});
    }

    void command_system::exec(nds::node_ptr<const nxi::command> command, const nxi::values& params) const { command->exec(params); }

    command_system::commands_view command_system::search(const QString& search_string)
    {
        commands_view commands;

        nds::algorithm::graph::for_each(graph_, [&commands, &search_string](auto&& node) {
            if (node->name().contains(search_string)) commands.emplace_back(node);
        });

        return commands;
    }

    void command_system::search(const QString& search_string, callback_type fn)
    {
        nds::algorithm::graph::for_each(graph_, [&search_string, &fn](auto&& node) {
            if (node->name().contains(search_string))
            {
                fn(node);
            }
        });
    }

    void command_system::set_root(nds::node_ptr<nxi::command> node)
    {
        root_ = node;
        emit event_root_update(root_);
    }

    nds::node_ptr<nxi::command> command_system::root() const { return root_; }

    nxi::commands_view command_system::root_list() const
    {
        nxi_assert(root_);
        nxi::commands_view commands;
        graph_.targets(root_, [&commands](const auto& node) { commands.push_back(node); });
        return commands;
    }

    void command_system::root_list(callback_type fn) const
    {
        nxi_assert(root_);
        graph_.targets(root_, [&fn](auto&& node) { fn(node); });
    }

    nxi::commands_view command_system::root_sources() const
    {
        nxi_assert(root_);
        nxi::commands_view commands;
        graph_.sources(root_, [&commands](auto&& node) { commands.push_back(node); });
        return commands;
    }
} // nxi

/* /aze/zer // explorer path
 * proto://host // net path
 * search_context/ search expression // search in widgets
 */