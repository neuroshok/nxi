#ifndef INCLUDE_NXI_SYSTEM_SESSION_HPP_NXI
#define INCLUDE_NXI_SYSTEM_SESSION_HPP_NXI

#include <nxi/data/session.hpp>
#include <nxi/user_session.hpp>
#include <stz/observer_ptr.hpp>
#include <unordered_map>
#include <memory>
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
        void focus(nxi::user_session&);
        nxi::user_session& get(int id);
        nxi::user_session& get(const QString& session_id);
        void load();
        void load(nxi::user_session&);
        void load(const QString& session_id);
        void unload();
        void unload(const QString& session_id);

        void switch_focus(const QString& new_session_id);

        stz::observer_ptr<nxi::user_session> focus();

    signals:
        void event_load(); // session_system loaded
        void event_add(nxi::user_session&);
        void event_unload(nxi::user_session&);
        void event_focus_update(nxi::user_session&);

    private:
        nxi::core& nxi_core_;
        stz::observer_ptr<nxi::user_session> focus_;
        std::vector<std::unique_ptr<nxi::user_session>> sessions_;
        std::vector<std::unique_ptr<nxi::user_session>> user_sessions_;
    };
} // nxi

#endif // INCLUDE_NXI_SYSTEM_SESSION_HPP_NXI