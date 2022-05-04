#ifndef CORE_H_NXI
#define CORE_H_NXI

#include <nxi/config.hpp>
#include <nxi/system/session.hpp>
#include <nxi/system/user.hpp>

namespace nxi
{
    class command_system;
    class context_system;
    class interface_system;
    class navigation_system;
    class page_system;

    class core : public QObject
    {
        Q_OBJECT
    public:
        core();
        core(const core&) = delete;
        void operator=(const core&) = delete;

        void load();
        void quit() const;

        nxi::database& global_database();

        nxi::session_system& session_system();

        // alias
        nxi::command_system& command_system();
        nxi::context_system& context_system();
        nxi::interface_system& interface_system();
        nxi::navigation_system& navigation_system();
        nxi::page_system& page_system();
        nxi::user_system& user_system();

        static QString module_path();
        static QString module_path(const QString& name, nxi::module_type);
        static QString page_path(const QString&);

    signals:
        void event_load() const;
        void event_quit() const;

    private:
        nxi::global_database global_database_;

        nxi::session_system session_system_;
        nxi::user_system user_system_;
    };
} // nxi

#endif // CORE_H_NXI