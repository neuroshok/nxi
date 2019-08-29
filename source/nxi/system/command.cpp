#include <nxi/system/command.hpp>

#include <nxi/core.hpp>
#include <nxi/system/page.hpp>

#include <QUrl>
#include <include/nxi/log.hpp>
#include <include/nxi/module/web.hpp>
#include <include/nxi/system/command.hpp>
#include <nxi/error.hpp>
#include <nxi/system/command.hpp>
#include <nxi/system/module.hpp>
#include <nxi/system/page.hpp>

#include <nds/encoder/graph.hpp>
#include <nds/algorithm/graph/find.hpp>

#include <nxi/page/custom.hpp>
#include <include/ui/view/config.hpp>

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
    namespace commands
    {
        void config(nxi::core& core)
        {
            //core.page_system().add<nxi::custom_page>();
        }
    }

    command_system::command_system(nxi::core& nxi_core) :
        nxi_core_{ nxi_core }
    {}

    void command_system::load()
    {
        nxi_trace("call nxi::command_system::load");


        // add nxi commands
        auto main_cmd = graph_.add(nxi::command("nxi", "command_node", std::bind(&nxi::core::quit, &nxi_core_)));

        // test cmd
        add(nxi::command("nxi", "test", []()
        {
             qDebug() << "TEST PAGE";
        }), main_cmd);

        add(nxi::command("nxi", "quit", std::bind(&nxi::core::quit, &nxi_core_), ":/button/quit"), main_cmd);
        add(nxi::command("nxi", "zeta", std::bind(&nxi::core::quit, &nxi_core_), ":/image/nex"), main_cmd);
        add(nxi::command("nxi", "config", [this](){ nxi_core_.page_system().add<nxi::web_page>();  }, ":/image/nex"), main_cmd);

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

    const nxi::command& command_system::get(const QString& action_name, const QString& module_name) const
    {
        nxi::command* command = nullptr;

        nds::algorithm::graph::find(graph_
        , [&command](auto&& found_node){ command = std::addressof(found_node->get()); }
        , [&action_name](auto&& node){ return node->get().action_name() == action_name; });

        if (command == nullptr) nxi_error("nxi::command not found : {}", action_name);
        return *command;
    }

    nds::node<nxi::command>* command_system::add(nxi::command command, nds::node<nxi::command>* source)
    {
        //qDebug()<< "__________" << command.action_name();
        auto command_node = graph_.add(std::move(command), source);
        //qDebug()<< "__________" << command_node->get().name();

        return command_node;
        /*
        size_t index = commands_.size();
        command_indexes_.insert(command.action_name(), index);
        commands_.emplace_back(std::make_unique<nxi::command>(std::move(command)));

        emit event_add(*commands_[index]);*/
    }

    void command_system::exec(const QString& command, command_context context)
    {
        QUrl url{ command };
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

        for (const auto& command : commands_)
        {
            //qDebug() << "search " << search_string << " match : " << command->name();
            if (command->name().contains(search_string)) commands.emplace_back(command.get());
        }

        return commands;
    }
} // nxi

/* /aze/zer // explorer path
 * proto://host // net path
 * search_context/ search expression // search in widgets
 */