#include <nxi/system/command.hpp>

#include <nxi/core.hpp>
#include <nxi/system/page.hpp>

#include <QUrl>
#include <include/nxi/log.hpp>
#include <nxi/module/web.hpp>

#include <nxi/system/command.hpp>
#include <nxi/system/command.hpp>
#include <nxi/system/module.hpp>
#include <nxi/system/page.hpp>
#include <nxi/system/interface.hpp>

#include <nds/encoder/graph.hpp>
#include <nds/algorithm/graph.hpp>

#include <nxi/page/custom.hpp>
#include <include/ui/view/config.hpp>

#include <nds/algorithm/graph.hpp>

namespace nds::encoders
{
    template<>
    template<>
    std::string dot<void>::node_name(const nxi::command& p)
    {
        return p.name().toStdString();
    }
} // nds::encoders

namespace nxi
{
    command_system::command_system(nxi::core& nxi_core)
        : nxi_core_{ nxi_core }
        , command_input_{ *this }
    {}

    void command_system::load()
    {
        nxi_trace("");
        init_commands();

        //auto cmd_page = add(nxi::command("nxi", "page", [this](const nxi::command_params&){ nxi_core_.page_system().focus(); }), main_cmd);
        //cmd_page->get().add_param("page_id", nxi_core_.page_system().get());

        //nds::encoders::dot<>::encode<nds::console>(graph_);

        // load module commands
        /*
        for (auto& module : nxi_core_.module_system().get())
        {
            if (module->type() == module_type::web)
            {
                auto web_module = static_cast<const nxi::web_module&>(*module);
                nxi::command cmd(module->name(), web_module.browser_action().default_title, std::bind(&nxi::core::quit, &nxi_core_), "module/webextension/test/" + web_module.browser_action().default_icon);
                add(std::move(cmd));
            }
        }*/

/*        nxi::module_load
        if (is_module_command) nxi.command_system()get(cmd).exec()
        else ui.command_system().get(cmd).exec()

        button { "nxi:quit" }*/
    }

    nxi::command* command_system::find(const QString& module_action, const QString& module_name) const
    {
        nxi::command* command = nullptr;

        nds::algorithm::graph::find_if(graph_
        , [&module_action, &module_name](auto&& node){ return node->get().action_name() == module_action && node->get().module_name() == module_name ; }
        , [&command](auto&& found_node){ command = std::addressof(found_node->get()); });

        return command;
    }

    const nxi::command& command_system::get(const QString& action_name, const QString& module_name) const
    {
        nxi::command* command = find(action_name, module_name);

        if (command == nullptr) nxi_error("nxi::command not found : {}", action_name);
        return *command;
    }

    nds::node<nxi::command>* command_system::add(nxi::command command, nds::node<nxi::command>* source)
    {
        return graph_.add(std::move(command), source);
    }

    void command_system::exec(const QString& str_command, command_context context)
    {
        nxi_trace("{}", str_command);


        QUrl url{ str_command };
        auto* command = find(url.path(), url.scheme());
        if (command == nullptr) nxi_core_.error("command not found " + str_command);
        else command->exec();


        //qDebug() << "scheme : " << url.scheme() << "__" << url.path();

        switch (context)
        {
            case command_context::deduced:


                break;
            case command_context::web:
            {
                /*
                qDebug() << "exec web";
                nxi::web_page page;
                page.url = command.toStdString();
                nxi_core_.page_system().load(std::move(page));*/
            }
                break;
            case command_context::explorer:
                break;
        }
    }

    command_system::commands_view command_system::search(const QString& search_string)
    {
        commands_view commands;

        nds::algorithm::graph::for_each(graph_, [&commands, &search_string](auto&& node)
        {
            if (node->get().name().contains(search_string)) commands.emplace_back(std::addressof(node->get()));
        });

        return commands;
    }

    nds::node<nxi::command>* command_system::init_group(const QString& command_node, nds::node<nxi::command>* source)
    {
        init_node_group_ = add(nxi::command("nxi", command_node, [](const nxi::command_params&){}, ":/icon/node"), source);

        auto fn = [this, group = init_node_group_](const nxi::command_params&)
        {
            set_root(group);
        };
        init_node_group_->get().set_function(fn);
        return init_node_group_;
    }

    void command_system::init(const QString& action, function_type fn, const QString& icon)
    {
        init_node_command_ = add(nxi::command("nxi", action, std::move(fn), icon), init_node_group_);
    }

    void command_system::init(nxi::command_data data)
    {
        init_node_command_ = add(nxi::command(std::move(data)), init_node_group_);
    }

    void command_system::init_param(const QString& name, std::function<void(std::vector<QString>&)> fn)
    {
        init_node_command_->get().add_param(name, fn);
    }

    void command_system::init_commands()
    {
        nds::node<nxi::command>* node_group = nullptr;
        nds::node<nxi::command>* node_command = nullptr;

        // MAIN
        auto group_main = init_group("main");
        set_root(init_node_group_);
            // quit
            nxi::command_data quit;
            quit.action = "quit";
            quit.icon = ":/icon/quit";
            quit.description = "Quit application";
            quit.shortcut = {{ Qt::Key_Control }, { Qt::Key_Q }};
            quit.function = [this](const nxi::command_params&){ nxi_core_.quit(); };
            init(std::move(quit));

            // test
            init("test", [this](const nxi::command_params&){ qDebug() << "TEST PAGE"; });
            // about
            init("about", [this](const nxi::command_params&){ nxi_core_.page_system().open_static("nxi/about"); });

        // SETTINGS
        init_group("settings", group_main);
            nxi::command_data config;
            config.action = "config";
            config.shortcut = {{ Qt::Key_Control, Qt::Key_Alt }, { Qt::Key_S }};
            config.function = [this](const nxi::command_params&){ nxi_core_.page_system().open_static("nxi/config", nxi::renderer_type::widget); };
            init(std::move(config));

        // PAGE
        nds::node<nxi::command>* page_node = init_group("page", group_main);
            // new
            nxi::command_data page_new;
            page_new.action = "new";
            page_new.description = "Open new web_page";
            //page_new.shortcut = {{ Qt::Key_Control }, { Qt::Key_T }};
            page_new.shortcut = {{  }, { Qt::Key_Exclam, Qt::Key_N, Qt::Key_E, Qt::Key_W }};
            page_new.function = [this](const nxi::command_params&){ nxi_core_.page_system().open<nxi::web_page>(0); };
            init(std::move(page_new));

            // open
            init("open", [this](const nxi::command_params&)
            {
                nxi_core_.page_system().open<nxi::web_page>(0);
            }, ":/image/nex");
                init_param("command", [](std::vector<QString>& suggestion)
                {
                    suggestion.push_back("www.youtube.com");
                    suggestion.push_back("www.google.com");
                });

            // switch
            nxi::command_data page_switch;
            page_switch.action = "switch";
            page_switch.icon = ":/icon/switch";
            page_switch.description = "Switch between all pages";
            page_switch.shortcut = {{ Qt::Key_Control }, { Qt::Key_W, Qt::Key_S }};
            page_switch.function = [this](const nxi::command_params& params)
            {
                auto id = params.get(0).toUInt();
                nxi_core_.page_system().focus(id);
            };
            init(std::move(page_switch));
                init_param("id", [this](std::vector<QString>& suggestion)
                {
                    qDebug() << "pages" << nxi_core_.page_system().get().size();
                    for (auto& page : nxi_core_.page_system().get())
                    {
                        suggestion.push_back(QString::number(page->id()));
                    }
                });
            // close
            nxi::command_data page_close;
            page_close.action = "close";
            page_close.description = "Close active page";
            page_close.shortcut = {{ Qt::Key_Control }, { Qt::Key_W, Qt::Key_X }};
            page_close.function = [this](const nxi::command_params& params)
            {
                // close
            };
            init(std::move(page_close));

        // INTERFACE
        init_group("interface", group_main);
            nxi::command_data load_style;
            load_style.action = "load_style";
            load_style.description = "Load style...";
            load_style.function = [this](const nxi::command_params& params)
            {
                auto name = params.get(0);
                nxi_core_.interface_system().load_style(name);
            };
            init(std::move(load_style));


            // shortcust test
            /*
            nxi::command_data test;
            test.shortcut = {{ Qt::Key_Control }, { Qt::Key_W }};
            test.function = [this, page_node](const nxi::command_params&)
            {
                nxi_core_.command_system().set_root(page_node);
            };
            init(test);*/

            // init trigger keys
            for_each([this](auto&& node)
            {
                const nxi::shortcut& shortcut = node->get().shortcut();
                if (shortcut.combo_keys.size() > 0) command_input().shortcut_input().add_trigger_key(shortcut.combo_keys[0]);
                else if (shortcut.sequence_keys.size() > 0) command_input().shortcut_input().add_trigger_key(shortcut.sequence_keys[0]);
            });
    }

    nxi::command_input& command_system::command_input()
    {
        return command_input_;
    }

    void command_system::set_root(nds::node<nxi::command>* node)
    {
        root_ = node;
        emit event_root_update(root_);
    }

    nxi::commands_view command_system::root_list()
    {
        nxi::commands_view commands;
        graph_.targets(root_, [&commands](auto&& node)
        {
            commands.push_back(stz::make_observer(&node->get()));
        });
        qDebug() << "list " << commands.size();
        return commands;
    }
} // nxi

/* /aze/zer // explorer path
 * proto://host // net path
 * search_context/ search expression // search in widgets
 */
