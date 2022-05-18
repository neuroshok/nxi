#include <nxi/command/initializer.hpp>

#include <nxi/core.hpp>
#include <nxi/system/page.hpp>

namespace nxi
{
    nds::node_ptr<nxi::command> command_initializer::init_settings()
    {
        auto node = add_node("settings");

        nxi::command_data config;
        config.action = "config";
        config.icon = ":/icon/config";
        config.shortcut = { { Qt::Key_Control, Qt::Key_Alt }, { Qt::Key_S } };
        config.function = [this](const nxi::values&){ core_.page_system().open_static("nxi/config", nxi::renderer_type::widget); };
        add(std::move(config));

        // command_add
        nxi::command_data command_add;
        command_add.action = "command_add";
        command_add.description = "Add JS command";
        command_add.function = [this](const nxi::values&)
        {
            //auto command = core_.command_system().get()
            //core_.command_system().update_command()
        };

        // set_shortcut
        nxi::command_data set_shortcut;
        set_shortcut.action = "set_shortcut";
        set_shortcut.description = "Set a shortcut";
        set_shortcut.function = [this](const nxi::values& params)
        {
            nxi_assert(params.size() == 2);
            //auto command = core_.command_system().get(params.get(0));
            //core_.command_system().update_command()
        };

        set_shortcut.parameters = {
        { "command", [this](nxi::suggestion_vector& suggestion)
            {
                core_.command_system().for_each([this, &suggestion](nds::node_ptr<const nxi::command> command)
                {
                    suggestion.push_back(std::move(command));
                });
            }
        },
        { "shortcut", [this](nxi::suggestion_vector& suggestion)
            {
                suggestion.push_back(nxi::text_suggestion("CTRL + E", "", "default value"));
                suggestion.push_back(nxi::text_suggestion("CTRL + E", "", "current value"));
            }
        }};

        add(std::move(set_shortcut));

        return node;
    }
} // nxi