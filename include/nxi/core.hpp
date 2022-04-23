#ifndef CORE_H_NXI
#define CORE_H_NXI

#include <nxi/config.hpp>
#include <nxi/system/session.hpp>

namespace nxi
{
    class core : public QObject
    {
        Q_OBJECT
    public:
        core();
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
        void event_load() const;
        void event_quit() const;

    private:
        nxi::global_database global_database_;
        std::unique_ptr<nxi::config> config_;

        nxi::session_system session_system_;
    };
} // nxi

#endif // CORE_H_NXI