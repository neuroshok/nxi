#include <nxi/command/initializer.hpp>

#include <nxi/core.hpp>
#include <nxi/values.hpp>

namespace nxi
{
    void command_initializer::init_context()
    {
        add_node("context");

        // context_add
        nxi::command_data context_add;
        context_add.action = "context_add";
        context_add.description = "Add a new context";
        context_add.shortcut = {{ Qt::Key_Control }, { Qt::Key_N , Qt::Key_C }};
        context_add.function = [this](const nxi::values& param)
        {
            nxi_assert(param.size() > 0);
            qDebug() << "add ctx " << param.get(0);
            //nxi_core_.context_system().add(param.get(0));
        };
        context_add.parameters =
        {
            {"name", [this](nxi::suggestion_vector& suggestion)
                {
                    for (const auto& ctx : nxi_core_.context_system().available_contexts())
                    {
                        QString info = "default priority " + QString::number(ctx.priority);
                        suggestion.push_back(nxi::text_suggestion{ ctx.name, "", info });
                    }
                }
            }
            , {"priority", [this](nxi::suggestion_vector& suggestion)
                {
                    auto active_priority = QString::number(nxi_core_.context_system().active_priority());
                    suggestion.push_back(nxi::text_suggestion(std::move(active_priority), "", "last priority"));
                }}
        };

        add(std::move(context_add));
    }
} // nxi