#ifndef CORE_H_NXI
#define CORE_H_NXI

#include <ndb/connector.hpp>
#include <ndb/initializer.hpp>

#include <nxi/system/command.hpp>
#include <nxi/system/context.hpp>
#include <nxi/system/page.hpp>
#include <nxi/system/module.hpp>
#include <nxi/system/interface.hpp>
#include <nxi/system/window.hpp>

#include <nxi/config.hpp>

namespace nxi
{
    class core : public QObject
    {
        Q_OBJECT
    public:
        core();
        ~core();
        core(const core&) = delete;
        void operator=(const core&) = delete;

        void load();
        void quit() const;

        void error(const QString& message) const;

        void test(QWidget* w)
        {
            emit event_test(w);
        }

        nxi::config& config() { return config_; }

        nxi::command_system& command_system();
        nxi::context_system& context_system();
        nxi::interface_system& interface_system();
        nxi::module_system& module_system();
        nxi::page_system& page_system();
        nxi::window_system& window_system();

        static QString page_path(const QString&);

    signals:
        void event_quit() const;
        void event_test(QWidget*);
        void event_error(const QString&) const;

    private:
        ndb::initializer<ndb::sqlite> ndb_init_;
        ndb::connector<dbs::core> ndb_connector_;

        nxi::config config_;

        nxi::command_system command_system_;
        nxi::context_system context_system_;
        nxi::interface_system interface_system_;
        nxi::module_system module_system_;
        nxi::page_system page_system_;
        nxi::window_system window_system_;
    };
} // nxi

#endif // CORE_H_NXI