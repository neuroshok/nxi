#ifndef INCLUDE_NXI_SESSION_HPP_NXI
#define INCLUDE_NXI_SESSION_HPP_NXI

#include <nxi/config.hpp>
#include <nxi/web_downloader.hpp>

#include <QObject>
#include <QString>

namespace nxi
{
    class core;
    class web_session;

    class session : public QObject
    {
        Q_OBJECT

    public:
        session(nxi::core&, nxi::session_data data);

        void load();
        void unload();

        int id() const;
        const QString& name() const;
        bool is_active() const;

        nxi::config& config();
        const nxi::config& config() const;
        nxi::core& core();
        nxi::web_downloader& web_downloader();
        nxi::web_session& web_session();

    private:
        nxi::core& core_;

        int id_;
        QString name_;
        bool active_;

        nxi::config config_;
        nxi::web_downloader web_downloader_;
        nxi::web_session* web_session_;
    };
} // nxi

#endif // INCLUDE_NXI_SESSION_HPP_NXI