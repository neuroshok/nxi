#ifndef CORE_H_NXI
#define CORE_H_NXI

#include <nxi/module/api/core.hpp>
#include <nxi/system/command.hpp>
#include <nxi/system/context.hpp>
#include <nxi/system/page.hpp>
#include <nxi/system/module.hpp>
#include <nxi/system/interface.hpp>
#include <nxi/system/session.hpp>
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

        nxi::config& config();
        nxi::database& global_database();

        nxi::session_system& session_system();

        static QString module_path();
        static QString module_path(const QString& name, nxi::module_type);
        static QString page_path(const QString&);

    signals:
        void event_quit() const;

    private:
        nxi::global_database global_database_;
        nxi::config config_;

        nxi::session_system session_system_;
    };
} // nxi

#endif // CORE_H_NXI