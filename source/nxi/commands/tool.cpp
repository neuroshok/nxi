#include <nxi/command/initializer.hpp>

#include <nxi/core.hpp>
#include <nxi/web_session.hpp>

namespace nxi
{
    nds::node_ptr<nxi::command> command_initializer::init_tool()
    {
        auto node = add_node("tool");
        set_node(node);
        set_root(node);

        // import
        nxi::command_data import;
        import.action = "import";
        import.icon = ":/icon/download";
        import.description = "Import data from other browsers";
        import.function = [this](const nxi::values& values)
        {
            nxi_assert(values.size() == 1);
            session_.web_session().import_cookies(values.get(0));
        };
        import.parameters = {{ "path" }};

        add(std::move(import));

        return node;
    }
} // nxi