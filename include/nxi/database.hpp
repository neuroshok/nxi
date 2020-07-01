#ifndef INCLUDE_NXI_DATABASE_HPP_NXI
#define INCLUDE_NXI_DATABASE_HPP_NXI

#include <array>
#include <vector>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>

namespace nxi
{
    enum class prepared_query
    {
        add_page, get_page_id, get_page_name,
        add_session, del_session, get_sessions, load_session, unload_session,
        add_window, get_windows, move_window,
        size_
    };

    class database
    {
    public:
        inline static QString path = "./database/";

        database(QString name, const QString& subpath = "");
        database(const database&) = delete;
        database& operator=(const database&) = delete;
        database(database&&) = default;
        database& operator=(database&&) = default;

        void connect();
        void close();
        QSqlDatabase& get();
        virtual void make() = 0;
        virtual void prepare_queries();
        void prepare(nxi::prepared_query, const QString& str_query);
        QSqlQuery& prepared_query(nxi::prepared_query);
        void query(const QString& str_query);

    private:
        std::array<QSqlQuery, static_cast<size_t>(nxi::prepared_query::size_)> queries_;

        bool make_required_;
        QString name_;
        QString path_;
        QSqlDatabase database_;
    };

    class core_database : public database
    {
    public:
        core_database(QString session_id);

        virtual void make() override;
        virtual void prepare_queries() override;
    };
    class global_database : public database
    {
    public:
        global_database();

        virtual void make() override;
        virtual void prepare_queries() override;
    };
} // nxi

#endif // INCLUDE_NXI_DATABASE_HPP_NXI
