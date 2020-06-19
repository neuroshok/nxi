#ifndef INCLUDE_NXI_DATABASE_HPP_NXI
#define INCLUDE_NXI_DATABASE_HPP_NXI

#include <array>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>

namespace nxi
{
    enum class dbs { core, module, size_ };
    enum class prepared_query
    {
        add_page,
        get_page_id, get_page_name,
        size_
    };

    class database
    {
        inline static QString path = "./database/";

    public:
        database();
        database(const database&) = delete;
        database& operator=(const database&) = delete;

        void connect(nxi::dbs);
        QSqlDatabase& get(nxi::dbs);
        void make(nxi::dbs);
        void prepare(nxi::dbs, nxi::prepared_query, const QString& str_query);
        QSqlQuery& prepared_query(nxi::prepared_query);
        void query(nxi::dbs, const QString& str_query);

    private:
        bool make_required_;
        std::array<QSqlDatabase, static_cast<size_t>(dbs::size_)> databases_;
        std::array<QSqlQuery, static_cast<size_t>(nxi::prepared_query::size_)> queries_;
    };
} // nxi

#endif // INCLUDE_NXI_DATABASE_HPP_NXI
