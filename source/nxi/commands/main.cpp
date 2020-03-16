#include <nxi/command/initializer.hpp>

#include <nxi/core.hpp>

namespace nxi
{
    void command_initializer::init_main()
    {
        auto main = add_node("main");
        set_root(main);

        // quit
        nxi::command_data quit;
        quit.action = "quit";
        quit.icon = ":/icon/quit";
        quit.description = "Quit application";
        quit.shortcut = {{ Qt::Key_Control }, { Qt::Key_Q }};
        quit.function = [this](const nxi::values&){ nxi_core_.quit(); };

        add(std::move(quit));

        // test
        add("test", [this](const nxi::values&){ qDebug() << "TEST PAGE"; });
        // about
        add("about", [this](const nxi::values&){ nxi_core_.page_system().open_static("nxi/about"); });
    }
} // nxi