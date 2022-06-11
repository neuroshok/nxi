#ifndef INCLUDE_NXI_SYSTEM_SESSION_HPP_NXI
#define INCLUDE_NXI_SYSTEM_SESSION_HPP_NXI

#include <nxi/data/session.hpp>
#include <nxi/session.hpp>

#include <memory>
#include <stz/observer_ptr.hpp>

#include <QObject>

namespace nxi
{
    class core;
    class user;

    class session_system : public QObject
    {
        Q_OBJECT
    public:
        explicit session_system(nxi::core&);
        session_system(const session_system&) = delete;
        void operator=(const session_system&) = delete;

        void load();
        void unload();

        void add(nxi::session_data);
        void add(const QString& session_id);
        void del(int session_id);
        void focus(nxi::session&);
        nxi::session& get(int id);
        nxi::session& get(const QString& session_id);
        void load(nxi::session&);
        void load(const QString& session_id);
        void unload(const QString& session_id);
        [[nodiscard]] const std::vector<std::unique_ptr<nxi::session>>& sessions() const;

        void switch_focus(const QString& new_session_id);

        nxi::session& focus();

    signals:
        void event_load(); // session_system loaded
        void event_add(nxi::session&);
        void event_unload(nxi::session&);
        void event_focus(nxi::session&);

    private:
        nxi::core& core_;

        stz::observer_ptr<nxi::session> focus_;
        std::vector<std::unique_ptr<nxi::session>> sessions_;
    };
} // nxi

#endif // INCLUDE_NXI_SYSTEM_SESSION_HPP_NXI