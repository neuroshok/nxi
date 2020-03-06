#ifndef NXI_COMMAND_SYSTEM_H_NXI
#define NXI_COMMAND_SYSTEM_H_NXI

#include <nxi/command.hpp>
#include <nxi/window.hpp>

#include <functional>
#include <stz/observer_ptr.hpp>

#include <QObject>
#include <QHash>

#include <nds/graph.hpp>
#include <nds/algorithm/graph.hpp>

#include <nxi/system/command.hpp>
#include <nxi/command/input.hpp>

namespace nxi
{
	class core;

	enum class command_context { deduced, web, explorer };

    class command_system : public QObject
    {
        Q_OBJECT
    public:
        using commands_view = std::vector<stz::observer_ptr<nxi::command>>;
        using function_type = std::function<void(const nxi::command_params&)>;

        command_system(nxi::core&);
        command_system(const command_system&) = delete;
        void operator=(const command_system&) = delete;

        void load();

        nxi::commands_view root_list();
        void root_list(std::function<void(const nxi::command&)>);

        const nxi::command& get(const QString& module_action, const QString& module_name = "nxi") const;
        nxi::command* find(const QString& module_action, const QString& module_name = "nxi") const;
        template<class Callback>
        void for_each(Callback&&);
        nds::node<nxi::command>* add(nxi::command command, nds::node<nxi::command>* source = nullptr);
        void exec(stz::observer_ptr<const nxi::command>);
        void exec(stz::observer_ptr<const nxi::command>, const nxi::command_params&);
        void exec(const QString& command, command_context context = command_context::deduced);
        commands_view search(const QString&);
        void search(const QString&, std::function<void(const nxi::command&)>);
        nxi::command_input& command_input();

        void set_root(nds::node<nxi::command>*);

        signals:
        void event_add(const nxi::command&);
        void event_root_update(nds::node<nxi::command>*);
        void event_param_required(stz::observer_ptr<const nxi::command>);

    private:
        void init_commands();
        nds::node<nxi::command>* init_group(const QString& command_node, nds::node<nxi::command>* = nullptr);
        void init(nxi::command_data);
        void init(const QString& action, function_type, const QString& icon = "");
        void init_param(const QString& name, std::function<void(nxi::suggestion_vector&)>);

        nds::node<nxi::command>* init_node_group_ = nullptr;
        nds::node<nxi::command>* init_node_command_ = nullptr;

    private:
        nxi::core& nxi_core_;
        nxi::command_input command_input_;
        nds::node<nxi::command>* root_;

        QHash<QString, size_t> command_indexes_;
        // QHash<shortcut, unsigned int
        nds::graph<nxi::command> graph_;

    };
} // nxi

namespace nxi
{
    template<class Callback>
    void command_system::for_each(Callback&& fn)
    {
        nds::algorithm::graph::for_each(graph_, [&fn](auto&& node)
        {
            fn(node);
        });
    }
} // nxi

#endif // NXI_COMMAND_SYSTEM_H_NXI