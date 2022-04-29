#include <nxi/command/initializer.hpp>

#include <nxi/core.hpp>
#include <nxi/page/web.hpp>

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

        //help
        nxi::command_data help;
        help.action = "help";
        help.icon = ":/icon/help";
        help.description = "Display nxi help";
        help.function = [this](const nxi::values&) { session_.page_system().open<nxi::web_page>("https://github.com/neuroshok/nxi/wiki"); };

        add(std::move(help));
     

        // test
        add("test", [this](const nxi::values&){ qDebug() << "TEST PAGE"; });
        // about
        add("about", [this](const nxi::values&){ session_.page_system().open_static("nxi/about"); });

        return node;
    }
} // nxi