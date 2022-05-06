#ifndef INCLUDE_NXI_SYSTEM_USER_HPP_NXI
#define INCLUDE_NXI_SYSTEM_USER_HPP_NXI

#include <nxi/data/user.hpp>
#include <nxi/user.hpp>

#include <vector>
#include <memory>

#include <QObject>

namespace nxi
{
    class core;
    class user;

    class user_system : public QObject
    {
        Q_OBJECT
    public:
        explicit user_system(nxi::core&);
        user_system(const user_system&) = delete;
        void operator=(const user_system&) = delete;

        void load();
        void unload();

        void add(nxi::user_data);
        void add(const QString& session_id);
        void del(const QString& session_id);
        void focus(nxi::user&);
        nxi::user& get(int id);
        nxi::user& get(const QString& user_id);
        void load(nxi::user&);
        void load(const QString& user_id);
        void unload(const QString& user_id);
        void unload_focus();

        void switch_focus(const QString& new_user_id);

        nxi::user& focus();

    signals:
        void event_load(); // user_system loaded
        void event_add(nxi::user&);
        void event_unload(nxi::user&);
        void event_focus(nxi::user&);

    private:
        nxi::core& nxi_core_;
        stz::observer_ptr<nxi::user> focus_;
        std::vector<std::unique_ptr<nxi::user>> users_;
    };
} // nxi

#endif // INCLUDE_NXI_SYSTEM_USER_HPP_NXI