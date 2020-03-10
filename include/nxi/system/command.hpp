#ifndef NXI_COMMAND_SYSTEM_H_NXI
#define NXI_COMMAND_SYSTEM_H_NXI

#include <nxi/system/command.hpp>
#include <nxi/command/input.hpp>
#include <nxi/command/initializer.hpp>

#include <functional>
#include <stz/observer_ptr.hpp>

#include <nds/algorithm/graph.hpp>
#include <nds/graph.hpp>

namespace nxi
{
	class core;

    class command_system : public QObject
    {
        Q_OBJECT
    public:
        using commands_view = std::vector<nds::node_ptr<nxi::command>>;
        using function_type = std::function<void(const nxi::command_params&)>;

        command_system(nxi::core&);
        command_system(const command_system&) = delete;
        void operator=(const command_system&) = delete;

        void load();

        nxi::commands_view root_list();
        void root_list(std::function<void(const nxi::command&)>);

        const nxi::command& get(const QString& module_action, const QString& module_name = "nxi") const;
        nds::node_ptr<nxi::command> find(const QString& module_action, const QString& module_name = "nxi") const;
        template<class Callback>
        void for_each(Callback&&);
        nds::node_ptr<nxi::command> add(nxi::command command, nds::node_ptr<nxi::command> source = {});
        void exec(nds::node_ptr<nxi::command>);
        void exec(nds::node_ptr<nxi::command>, const nxi::command_params&);
        commands_view search(const QString&);
        void search(const QString&, std::function<void(const nxi::command&)>);
        nxi::command_input& command_input();

        void set_root(nds::node_ptr<nxi::command>);

    signals:
        void event_add(const nxi::command&);
        void event_root_update(nds::node_ptr<nxi::command>);
        void event_param_required(const nds::node_ptr<nxi::command>);

    private:
        nxi::core& nxi_core_;
        nxi::command_initializer command_initializer_;
        nxi::command_input command_input_;
        nds::node_ptr<nxi::command> root_;
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