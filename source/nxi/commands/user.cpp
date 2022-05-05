#include <nxi/command/initializer.hpp>

#include <nxi/core.hpp>

namespace nxi
{
    nds::node_ptr<nxi::command> command_initializer::init_user()
    {
        auto node = add_node("user");

        // add_user
        nxi::command_data add_user;
        add_user.action = "add_user";
        add_user.description = "Add a new user";
        add_user.function = [this](const nxi::values& params)
        {
            auto user_id = params.get(0);
            core_.user_system().add(user_id);
        };
        add_user.parameters = {{ "name" }};
        add(std::move(add_user));

        // del_user
        nxi::command_data del_user;
        del_user.action = "del_user";
        del_user.description = "Delete a user";
        del_user.function = [this](const nxi::values& params)
        {
            auto user_id = params.get(0);
            core_.user_system().del(user_id);
        };
        del_user.parameters = {{ "name" }};
        add(std::move(del_user));

        // load_user
        nxi::command_data load_user;
        load_user.action = "load_user";
        load_user.description = "Load a user session";
        load_user.function = [this](const nxi::values& params)
        {
            auto user_id = params.get(0);
            core_.user_system().load(user_id);
        };
        load_user.parameters = {{ "name" }};
        add(std::move(load_user));

        // unload_user
        nxi::command_data unload_user;
        unload_user.action = "unload_user";
        unload_user.description = "Unload the current user session";
        unload_user.function = [this](const nxi::values& params)
        {
            core_.user_system().unload_focus();
        };
        add(std::move(unload_user));

        return node;
    }
} // nxi