#include <nxi/command/initializer.hpp>

#include <nxi/core.hpp>

namespace nxi
{
    nds::node_ptr<nxi::command> command_initializer::init_session()
    {
        auto node = add_node("session");

        // add_session
        nxi::command_data add_session;
        add_session.action = "add_session";
        add_session.description = "Add a new session";
        add_session.function = [this](const nxi::values& params)
        {
            auto session_id = params.get(0);
            core_.session_system().add(session_id);
        };
        add_session.parameters = {{ "name" }};
        add(std::move(add_session));

        // del_session
        nxi::command_data del_session;
        del_session.action = "del_session";
        del_session.description = "Delete a session";
        del_session.function = [this](const nxi::values& params)
        {
            auto session_id = params.get(0);
            core_.session_system().del(session_id);
        };
        del_session.parameters = {{ "name" }};
        add(std::move(del_session));

        // load_session
        nxi::command_data load_session;
        load_session.action = "load_session";
        load_session.description = "Load a session";
        load_session.function = [this](const nxi::values& params)
        {
            auto session_id = params.get(0);
            core_.session_system().load(session_id);
        };
        load_session.parameters = {{ "name" }};
        add(std::move(load_session));

        // unload_session
        nxi::command_data unload_session;
        unload_session.action = "unload_session";
        unload_session.description = "Close this session";
        unload_session.function = [this](const nxi::values& params)
        {
            //core_.session_system().unload(core_.name());
        };
        add(std::move(unload_session));

        // switch_session
        nxi::command_data switch_session;
        switch_session.action = "switch_session";
        switch_session.description = "Switch session";
        switch_session.function = [this](const nxi::values& values)
        {
            nxi_assert(values.size() == 1);
            core_.session_system().switch_focus(values.get(0));
        };
        switch_session.parameters = {
        { "name", [this](nxi::suggestion_vector& suggestion)
            {
                for (const auto& session : core_.session_system().sessions())
                {
                    suggestion.push_back(session->name());
                }
            }
        }};

        add(std::move(switch_session));

        return node;
    }
} // nxi