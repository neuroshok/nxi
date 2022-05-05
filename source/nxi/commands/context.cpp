#include <nxi/command/initializer.hpp>

#include <nxi/core.hpp>
#include <nxi/suggestion/text.hpp>
#include <nxi/suggestion/vector.hpp>
#include <nxi/system/context.hpp>
#include <nxi/values.hpp>

namespace nxi
{
    nds::node_ptr<nxi::command> command_initializer::init_context()
    {
        auto node = add_node("context");

        // context_add
        nxi::command_data context_add;
        context_add.action = "add_context";
        context_add.description = "Add a context";
        context_add.shortcut = {{ Qt::Key_Control }, { Qt::Key_N , Qt::Key_C }};
        context_add.function = [this](const nxi::values& param)
        {
            nxi_assert(param.size() > 1);
            core_.context_system().add(param.get(0), param.get(1).toInt());
        };
        context_add.parameters =
        {
            {"name", [this](nxi::suggestion_vector& suggestion)
                {
                    for (const auto& ctx : core_.context_system().available_contexts())
                    {
                        QString info = "default priority " + QString::number(ctx.priority);
                        suggestion.push_back(nxi::text_suggestion{ ctx.name, "", info });
                    }
                }
            }
            , {"priority", [this](nxi::suggestion_vector& suggestion)
                {
                    auto active_priority = QString::number(core_.context_system().active_priority());
                    suggestion.push_back(nxi::text_suggestion(std::move(active_priority), "", "last priority"));
                }}
        };

        add(std::move(context_add));

        nxi::command_data context_del;
        context_del.action = "del_context";
        context_del.description = "Delete a context";
        context_del.shortcut = {{ Qt::Key_Control }, { Qt::Key_B , Qt::Key_C }};
        context_del.function = [this](const nxi::values& param)
        {
            nxi_assert(param.size() > 0);
            core_.context_system().del(param.get(0));
        };
        context_del.parameters =
        {
            {"name", [this](nxi::suggestion_vector& suggestion)
                {
                    for (const auto& ctx : core_.context_system().contexts())
                    {
                        suggestion.push_back(nxi::text_suggestion{ ctx->name(), "", "info" });
                    }
                }
            }
        };
        add(std::move(context_del));

        return node;
    }
} // nxi