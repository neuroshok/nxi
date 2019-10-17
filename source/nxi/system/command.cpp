#include <nxi/system/command.hpp>

#include <nxi/core.hpp>
#include <nxi/system/page.hpp>

#include <QUrl>
#include <include/nxi/log.hpp>
#include <include/nxi/module/web.hpp>
#include <include/nxi/system/command.hpp>
#include <nxi/system/command.hpp>
#include <nxi/system/module.hpp>
#include <nxi/system/page.hpp>
#include <nxi/system/interface.hpp>

#include <nds/encoder/graph.hpp>
#include <nds/algorithm/graph/find.hpp>

#include <nxi/page/custom.hpp>
#include <include/ui/view/config.hpp>

#include <nds/algorithm/graph/find.hpp>

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
        , user_input_{ *this }
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

    const std::vector<std::unique_ptr<nxi::command>>& command_system::get()
    {
        return commands_;
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

    void command_system::init_group(const QString& command_node)
    {
        init_node_group_ = add(nxi::command("nxi", command_node, [this](const nxi::command_params&){}, ":/image/nex"), init_node_group_);
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

        init_group("main");
            // quit
            nxi::command_data quit;
            quit.action = "quit";
            quit.icon = ":/button/quit";
            quit.description = "Quit application";
            quit.function = [this](const nxi::command_params&){ nxi_core_.quit(); };
            init(std::move(quit));

            // test
            init("test", [this](const nxi::command_params&){ qDebug() << "TEST PAGE"; });
            // about
            init("about", [this](const nxi::command_params&){ nxi_core_.page_system().open_static("nxi/about"); });

        // SETTINGS
        init_group("settings");
            init("config", [this](const nxi::command_params&){ nxi_core_.page_system().open_static("nxi/config", nxi::renderer_type::widget); });

        // PAGE
        init_group("page");
            // new
            init("new_page", [this](const nxi::command_params&)
            {
                nxi_core_.page_system().open<nxi::web_page>(0);
            });
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

                nxi_core_.page_system().get();


/*
        // add nxi commands
        auto main_cmd = graph_.add(nxi::command("nxi", "command_node", std::bind(&nxi::core::quit, &nxi_core_)));

        // test cmd
        add(nxi::command("nxi", "test", [](const nxi::command_params&)
        {
             qDebug() << "TEST PAGE";
        }), main_cmd);

        add(nxi::command("nxi", "quit", [this](const nxi::command_params&){ nxi_core_.quit(); }, ":/button/quit"), main_cmd);
        add(nxi::command("nxi", "config", [this](const nxi::command_params&){ nxi_core_.page_system().open_static("nxi/config", nxi::renderer_type::widget);  }, ":/image/nex"), main_cmd);
        add(nxi::command("nxi", "about", [this](const nxi::command_params&){ nxi_core_.page_system().open_static("nxi/about");  }), main_cmd);
        add(nxi::command("nxi", "aboutgl", [this](const nxi::command_params&){ nxi_core_.page_system().open_static("nxi/aboutgl", nxi::renderer_type::widget);  }), main_cmd);

        auto node_nxi = add(nxi::command("nxi", "nxi", [this](const nxi::command_params&){ nxi_core_.page_system().open_static("nxi/about"); }));
        auto node_theme = add(nxi::command("nxi", "theme", [this](const nxi::command_params&){ nxi_core_.page_system().open_static("nxi/about"); }), node_nxi);

        auto page_load = nxi::command("nxi", "open", [this](const nxi::command_params& params)
        {
            //auto page_command = params.get(0);
            nxi_core_.page_system().open<nxi::web_page>(0);
        });
        //page_load.add_param("command", { "http://www.google.fr", "www.youtube.com" });
        add(std::move(page_load), node_nxi);


        auto theme_load = nxi::command("nxi", "load", [this](const nxi::command_params& params)
        {
            auto style_name = params.get(0);
            nxi_core_.interface_system().load_style(style_name);
        });
        theme_load.add_param("name", nxi_core_.interface_system().styles());

        add(std::move(theme_load), node_theme);
        add(nxi::command("nxi", "load_theme_nebula", [this](const nxi::command_params&){ nxi_core_.interface_system().load_style("nebula_space"); }), main_cmd);
*/
    }

    nxi::command_input& command_system::user_input()
    {
        return user_input_;
    }
} // nxi

/* /aze/zer // explorer path
 * proto://host // net path
 * search_context/ search expression // search in widgets
 */
