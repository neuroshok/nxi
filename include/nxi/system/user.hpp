#ifndef INCLUDE_NXI_SYSTEM_USER_HPP_NXI
#define INCLUDE_NXI_SYSTEM_USER_HPP_NXI

#include <nxi/data/user.hpp>
#include <stz/observer_ptr.hpp>

#include <vector>
#include <memory>

#include <QObject>

namespace nxi
{
    class core;
    class user_session;

    class user_system : public QObject
    {
        Q_OBJECT
    public:
        explicit user_system(nxi::core&);
        user_system(const user_system&) = delete;
        void operator=(const user_system&) = delete;

        void add(nxi::user_data);
        void add(const QString& session_id);
        void del(const QString& session_id);
        void focus(nxi::user_session&);
        nxi::user_session& get(int id);
        nxi::user_session& get(const QString& user_id);
        void load();
        void load(nxi::user_session&);
        void load(const QString& user_id);
        void unload();
        void unload(const QString& user_id);

        void switch_focus(const QString& new_user_id);

        stz::observer_ptr<nxi::user_session> focus();

    signals:
        void event_load(); // user_system loaded
        void event_add(nxi::user_session&);
        void event_unload(nxi::user_session&);
        void event_focus_update(nxi::user_session&);

    private:
        nxi::core& nxi_core_;
        stz::observer_ptr<nxi::user_session> focus_;
        std::vector<std::unique_ptr<nxi::user_session>> users_;
    };
} // nxi

#endif // INCLUDE_NXI_SYSTEM_USER_HPP_NXI