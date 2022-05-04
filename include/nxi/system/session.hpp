#ifndef INCLUDE_NXI_SYSTEM_SESSION_HPP_NXI
#define INCLUDE_NXI_SYSTEM_SESSION_HPP_NXI

#include <nxi/session.hpp>
#include <stz/observer_ptr.hpp>
#include <memory>
#include <unordered_map>
#include <QObject>

namespace nxi
{
    class core;

    class session_system : public QObject
    {
        Q_OBJECT
    public:
        explicit session_system(nxi::core&);
        session_system(const session_system&) = delete;
        void operator=(const session_system&) = delete;

        void add(nxi::session_data);
        void add(const QString& session_id);
        void del(const QString& session_id);
        void focus(nxi::session&);
        nxi::session& get(int id);
        nxi::session& get(const QString& session_id);
        void load();
        void load(nxi::session&);
        void load(const QString& session_id);
        void unload();
        void unload(const QString& session_id);

        void switch_focus(const QString& new_session_id);

        stz::observer_ptr<nxi::session> focus();

    signals:
        void event_load(); // session_system loaded
        void event_add(nxi::session&);
        void event_unload(nxi::session&);
        void event_focus_update(nxi::session&);

    private:
        nxi::core& nxi_core_;
        stz::observer_ptr<nxi::session> focus_;
        std::vector<std::unique_ptr<nxi::session>> sessions_;
    };
} // nxi

#endif // INCLUDE_NXI_SYSTEM_SESSION_HPP_NXI