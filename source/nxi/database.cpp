#include <nxi/database.hpp>

#include <nxi/data/page.hpp>
#include <nxi/data/session.hpp>
#include <nxi/data/window.hpp>
#include <nxi/log.hpp>

#include <filesystem>

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>

#include <qdebug.h>

namespace fs = std::filesystem;

namespace nxi
{
    database::database(QString name, const QString& subpath)
        : make_required_{ false }
        , name_{ std::move(name) }
        , path_{ path + subpath }
    {
        if (!fs::exists(path_.toStdString()))
        {
            fs::create_directory(path_.toStdString());
            make_required_ = true;
        }

        database_ = QSqlDatabase::addDatabase("QSQLITE", path_ + name_);
        database_.setDatabaseName(path_ + name_);
    }

    void database::connect()
    {
        nxi_trace("connect to database {}", database_.databaseName());

        if(!database_.open()) nxi_error("connection error : {}", database_.lastError().text());
        if (make_required_) make();

        prepare_queries();
    }

    void database::make()
    {
        nxi::data::session::internal::make(*this);
    }

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

    void database::query(const QString& str_query)
    {
        QSqlQuery query{ database_ };
        if (!query.exec(str_query)) nxi_error("query error : {}", query.lastError().text());
    }

    //

    global_database::global_database()
        : database("global")
    {}

    void global_database::make()
    {
        nxi::data::session::internal::make(*this);
    }

    void global_database::prepare_queries()
    {
        nxi::data::session::internal::prepare(*this);
    }

    //

    core_database::core_database(QString session_id)
        : database("core", session_id + "/" )
    {}

    void core_database::make()
    {
        nxi::data::page::internal::make(*this);
        nxi::data::window::internal::make(*this);
    }

    void core_database::prepare_queries()
    {
        nxi::data::page::internal::prepare(*this);
        nxi::data::window::internal::prepare(*this);
    }
} // nxi
