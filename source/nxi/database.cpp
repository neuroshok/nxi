#include <nxi/database.hpp>

#include <nxi/data/config.hpp>
#include <nxi/data/context.hpp>
#include <nxi/data/cookie.hpp>
#include <nxi/data/module.hpp>
#include <nxi/data/navigation.hpp>
#include <nxi/data/page.hpp>
#include <nxi/data/session.hpp>
#include <nxi/data/user.hpp>
#include <nxi/data/window.hpp>
#include <nxi/log.hpp>

#include <QDir>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>

namespace nxi
{
    database::database(QString name, const QString& subpath)
        : make_required_{ false }
        , name_{ std::move(name) }
        , path_{ path + subpath }
        , database_ { QSqlDatabase::addDatabase("QSQLITE", path_ + name_) }
    {
        if (!QDir(path_).exists())
        {
            QDir().mkpath(path_);
            make_required_ = true;
        }

        database_.setDatabaseName(path_ + name_);
    }

    void database::connect()
    {
        nxi_trace("connect to database {}", database_.databaseName());

        if(!database_.open()) nxi_error("connection error : {}", database_.lastError().text());
        if (make_required_) make();

        prepare_queries();
    }

    void database::close()
    {
        database_.close();
    }

    QSqlQuery database::exec(const QString& str_query)
    {
        QSqlQuery query{ database_ };
        if (!query.exec(str_query)) nxi_error("query error : {} | {}", query.lastError().text(), str_query);
        return query;
    }

    void database::exec(QSqlQuery& query)
    {
        if (!query.exec()) nxi_error("query error : {}", query.lastError().text());
    }

    void database::make() {}

    void database::prepare_queries() {}

    void database::prepare(nxi::prepared_query pquery, const QString& str_query)
    {
        QSqlQuery query{ database_ };
        query.prepare(str_query);
        queries_[static_cast<size_t>(pquery)] = std::move(query);
    }

    QSqlQuery& database::prepared_query(nxi::prepared_query pquery)
    {
        return queries_[static_cast<size_t>(pquery)];
    }

    QSqlDatabase& database::get() { return database_; }

    //

    global_database::global_database()
        : database("global")
    {}

    void global_database::make()
    {
        nxi::data::user::internal::make(*this);
    }

    void global_database::prepare_queries()
    {
        nxi::data::user::internal::prepare(*this);
    }

    //

    core_database::core_database(QString session_id)
        : database("core", session_id + "/" )
    {}

    void core_database::make()
    {
        nxi::data::config::internal::make(*this);
        nxi::data::context::internal::make(*this);
        nxi::data::cookie::internal::make(*this);
        nxi::data::module::internal::make(*this);
        nxi::data::navigation::internal::make(*this);
        nxi::data::page::internal::make(*this);
        nxi::data::session::internal::make(*this);
        nxi::data::window::internal::make(*this);
    }

    void core_database::prepare_queries()
    {
        nxi::data::config::internal::prepare(*this);
        nxi::data::context::internal::prepare(*this);
        nxi::data::cookie::internal::prepare(*this);
        nxi::data::module::internal::prepare(*this);
        nxi::data::navigation::internal::prepare(*this);
        nxi::data::page::internal::prepare(*this);
        nxi::data::session::internal::prepare(*this);
        nxi::data::window::internal::prepare(*this);
    }
} // nxi