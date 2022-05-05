#ifndef INCLUDE_NXI_SESSION_HPP_NXI
#define INCLUDE_NXI_SESSION_HPP_NXI

#include <nxi/data/session.hpp>


#include <QObject>
#include <QString>

namespace nxi
{
    class user_session;
    class web_session;

    class session : public QObject
    {
        Q_OBJECT

    public:
        session(nxi::user_session&, nxi::session_data data);

        void load();
        void unload();

        int id() const;
        const QString& name() const;
        bool is_active() const;

        nxi::user_session& user_session();
        nxi::web_session& web_session();

    private:
        nxi::user_session& user_session_;
        nxi::web_session* web_session_;

        int id_;
        QString name_;
        bool active_;
    };
} // nxi

#endif // INCLUDE_NXI_SESSION_HPP_NXI