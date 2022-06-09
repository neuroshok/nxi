#include <nxi/command/initializer.hpp>

#include <nxi/command.hpp>
#include <nxi/core.hpp>
#include <nxi/suggestion/text.hpp>
#include <nxi/system/buffer.hpp>
#include <nxi/user.hpp>
#include <nxi/values.hpp>

namespace nxi
{
    command_initializer::command_initializer(nxi::core& core)
        : core_{ core }
        , node_{ nullptr }
    {}

    nds::node_ptr<nxi::command> command_initializer::add(nxi::command_data data)
    {
        auto command = core_.command_system().add(nxi::command(std::move(data)), node_);
        return command;
    }

    nds::node_ptr<nxi::command> command_initializer::add(const QString& action, command_system::function_type fn, const QString& icon)
    {
        auto command = core_.command_system().add(nxi::command("nxi", action, std::move(fn), icon), node_);
        return command;
    }

    nds::node_ptr<nxi::command> command_initializer::add_node(const QString& str_command_node)
    {
        auto command = nxi::command{ "nxi", str_command_node, [](const nxi::values&) {}, ":/icon/node" };

        auto command_node = core_.command_system().add(std::move(command), node_);

        auto fn = [this, command_node](const nxi::values&) {
            // core_.command_system().set_root(command_node);
            // core_.command_system().suggest_command();
        };
        command_node->function_ = std::move(fn);

        set_node(command_node);
        return command_node;
    }

    void command_initializer::set_node(nds::node_ptr<nxi::command> node) { node_ = node; }

    void command_initializer::load()
    {
        nxi_trace("");

        auto main = init_main();

        set_node(main);
        init_page();

        set_node(main);
        add_node("interface");

        nxi::command_data load_style;
            load_style.action = "load_style";
            load_style.description = "Load style...";
            load_style.preview = true;
            load_style.function = [this](const nxi::values& params)
            {
                nxi_assert(params.size() == 1);
                const auto& name = params.get(0);
                core_.interface_system().load_style(name);
            };
            load_style.parameters = {
            { "name", [this](nxi::suggestion_vector& suggestion)
                {
                                           for (const auto& style_name : core_.interface_system().styles())
                                           {
                                               suggestion.push_back(nxi::text_suggestion{ style_name });
                                           }
                                       }
            }};

            add(std::move(load_style));

            set_node(main);
            init_context();
            set_node(main);
            init_settings();
            set_node(main);
            init_session();
            set_node(main);
            init_tool();
            set_node(main);
            init_user();
    }
} // nxi