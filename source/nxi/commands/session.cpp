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
        add_session.icon = ":/icon/add_session";
        add_session.description = "Add a new session";
        add_session.function = [this](const nxi::values& params)
        {
            auto session_id = params.get(0);
            nxi_core_.session_system().add(session_id);
        };
        add_session.parameters = {{ "name" }};
        add(std::move(add_session));

        return node;
    }
} // nxi