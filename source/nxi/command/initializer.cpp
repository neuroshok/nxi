#include <nxi/command/initializer.hpp>

#include <nxi/core.hpp>
#include <nxi/command.hpp>
#include <nxi/suggestion/text.hpp>
#include <nxi/values.hpp>

namespace nxi
{
    command_initializer::command_initializer(nxi::core& nxi_core)
        : nxi_core_{ nxi_core }
        , command_{ nullptr }
        , node_{ nullptr }
    {}

    nds::node_ptr<nxi::command> command_initializer::add(nxi::command_data data)
    {
        command_ = nxi_core_.command_system().add(nxi::command(std::move(data)), node_);
        return command_;
    }

    nds::node_ptr<nxi::command> command_initializer::add(const QString& action, command_system::function_type fn, const QString& icon)
    {
        command_ = nxi_core_.command_system().add(nxi::command("nxi", action, std::move(fn), icon), node_);
        return command_;
    }

    nds::node_ptr<nxi::command> command_initializer::add_node(const QString& command_node)
    {
        nds::node_ptr<nxi::command> tmp_node;
        auto fn = [this, &tmp_node](const nxi::values&)
        {
            //nxi_core_.command_system().set_root(node);
        };
        node_ = nxi_core_.command_system().add(nxi::command("nxi", command_node, fn, ":/icon/node"), node_);
        tmp_node.reset(node_.get());
        return node_;
    }

    void command_initializer::set_root(nds::node_ptr<nxi::command> node)
    {
        node_ = std::move(node);
        nxi_core_.command_system().set_root(node_);
    }

    void command_initializer::load()
    {
        nxi_trace("");

        init_main();
        init_page();

        add_node("interface");

        nxi::command_data load_style;
        load_style.action = "load_style";
        load_style.description = "Load style...";
        load_style.preview = true;
        load_style.function = [this](const nxi::values& params)
        {
            nxi_assert(params.size() == 1);
            auto name = params.get(0);
            nxi_core_.interface_system().load_style(name);
        };
        load_style.parameters = {
        { "name", [this](nxi::suggestion_vector& suggestion)
            {
                for (auto style_name : nxi_core_.interface_system().styles())
                {
                    suggestion.push_back(nxi::text_suggestion{ style_name });
                }
            }
        }};

        add(std::move(load_style));


        init_context();
        init_settings();
    }
} // nxi