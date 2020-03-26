#include <nxi/command/initializer.hpp>

#include <nxi/core.hpp>

namespace nxi
{
    void command_initializer::init_settings()
    {
        add_node("settings");

        nxi::command_data config;
        config.action = "config";
        config.shortcut = {{ Qt::Key_Control, Qt::Key_Alt }, { Qt::Key_S }};
        config.function = [this](const nxi::values&){ nxi_core_.page_system().open_static("nxi/config", nxi::renderer_type::widget); };
        add(std::move(config));

        // command_edit
        nxi::command_data command_edit;
        command_edit.action = "command_edit";
        command_edit.description = "Edit command";
        command_edit.function = [this](const nxi::values&)
        {
            // nxi_core_.command_system().update_command()
        };

        command_edit.parameters = {
        { "command", [this](nxi::suggestion_vector& suggestion)
            {
                nxi_core_.command_system().for_each([this, &suggestion](nds::node_ptr<const nxi::command> command)
                {
                    suggestion.push_back(std::move(command));
                });
            }
        }};

        add(std::move(command_edit));
    }
} // nxi