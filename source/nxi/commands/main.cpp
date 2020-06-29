#include <nxi/command/initializer.hpp>

#include <nxi/core.hpp>

namespace nxi
{
    nds::node_ptr<nxi::command> command_initializer::init_main()
    {
        auto node = add_node("main");
        set_node(node);
        set_root(node);

        // quit
        nxi::command_data quit;
        quit.action = "quit";
        quit.icon = ":/icon/quit";
        quit.description = "Quit application";
        quit.shortcut = {{ Qt::Key_Control }, { Qt::Key_Q }};
        quit.function = [this](const nxi::values&){ session_.nxi_core().quit(); };

        add(std::move(quit));

        // test
        add("test", [this](const nxi::values&){ qDebug() << "TEST PAGE"; });
        // about
        add("about", [this](const nxi::values&){ session_.page_system().open_static("nxi/about"); });

        return node;
    }
} // nxi