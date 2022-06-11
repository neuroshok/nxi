#include <nxi/command/initializer.hpp>

#include <nxi/core.hpp>
#include <nxi/page/web.hpp>
#include <nxi/system/page.hpp>

namespace nxi
{
    nds::node_ptr<nxi::command> command_initializer::init_main()
    {
        auto node = add_node("main");
        set_node(node);
        core_.command_system().set_root(node);

        // add_window
        nxi::command_data add_window;
        add_window.action = "add_window";
        add_window.icon = ":/icon/add";
        add_window.description = "Add a new window";
        add_window.function = [this](const nxi::values& values) { core_.user().window_system().add(); };

        add(std::move(add_window));

        // help
        nxi::command_data help;
        help.action = "help";
        help.icon = ":/icon/help";
        help.description = "Display nxi help";
        help.function = [this](const nxi::values&) { core_.page_system().open<nxi::web_page>("https://github.com/neuroshok/nxi/wiki"); };

        add(std::move(help));

        // test
        add("test", [this](const nxi::values&) { qDebug() << "TEST PAGE"; });
        // about
        add(
            "about", [this](const nxi::values&) { core_.page_system().open_static("nxi/about"); }, ":/icon/nxi");

        // quit
        nxi::command_data quit;
        quit.action = "quit";
        quit.icon = ":/icon/quit";
        quit.description = "Quit application";
        quit.shortcut = { { Qt::Key_Control }, { Qt::Key_Q } };
        quit.function = [this](const nxi::values&) { core_.quit(); };

        add(std::move(quit));

        return node;
    }
} // nxi