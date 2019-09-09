#ifndef NXI_COMMAND_SYSTEM_H_NXI
#define NXI_COMMAND_SYSTEM_H_NXI

#include <nxi/command.hpp>
#include <nxi/window.hpp>

#include <functional>
#include <stz/observer_ptr.hpp>

#include <QObject>
#include <QHash>

#include <nds/graph.hpp>

namespace nxi
{
	class core;

	enum class command_context { deduced, web, explorer };

    class command_system : public QObject
    {
        Q_OBJECT
    public:
        using commands_view = std::vector<stz::observer_ptr<nxi::command>>;

        command_system(nxi::core&);
        command_system(const command_system&) = delete;
        void operator=(const command_system&) = delete;

        void load();

        const std::vector<std::unique_ptr<nxi::command>>& get();
        const nxi::command& get(const QString& module_action, const QString& module_name = "nxi") const;
        nxi::command* find(const QString& module_action, const QString& module_name = "nxi") const;
        nds::node<nxi::command>* add(nxi::command command, nds::node<nxi::command>* source = nullptr);
        void exec(const QString& command, command_context context = command_context::deduced);
        commands_view search(const QString&);

        signals:
        void event_add(const nxi::command&);

    private:
        nxi::core& nxi_core_;

        std::vector<std::unique_ptr<nxi::command>> commands_;
        QHash<QString, size_t> command_indexes_;
        // QHash<shortcut, unsigned int
        nds::graph<nxi::command> graph_;

    };
} // nxi

#endif // NXI_COMMAND_SYSTEM_H_NXI