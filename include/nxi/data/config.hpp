#ifndef INCLUDE_NXI_DATA_CONFIG_HPP_NXI
#define INCLUDE_NXI_DATA_CONFIG_HPP_NXI

#include <nxi/database.hpp>
#include <nxi/database/result.hpp>
#include <nxi/log.hpp>
#include <nxi/type.hpp>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

namespace nxi
{
    class core;
    class database;
}

namespace nxi::data::config
{
    inline nxi::result get(nxi::database& db, const QString& key)
    {
        auto& query = db.prepared_query(nxi::prepared_query::get_config_value);
        query.bindValue(0, key);

        return nxi::result{ query };
    }

    template<class T>
    inline void set(nxi::database& db, const QString& key, const T& value)
    {
        auto& query = db.prepared_query(nxi::prepared_query::set_config_value);
        query.bindValue(0, key);
        query.bindValue(1, value);
        if (!query.exec()) nxi_error("query error : {}", query.lastError().text());
    }

} // nxi::data::config

namespace nxi::data::config::internal
{
    void make(nxi::database& db);
    void prepare(nxi::database& db);

    inline static constexpr struct table
    {
        nxi::field<0, int> key{};
        nxi::field<1, int> type{};
        nxi::field<2, QByteArray> value{};
    } config{};

    constexpr std::string_view str_table = R"__(
        CREATE TABLE `config`
        (
            `key` varchar(24),
            `type` integer,
            `value` blob,
            PRIMARY KEY(`key`)
        )
        )__";
} // nxi::data::config::internal

#endif // INCLUDE_NXI_DATA_CONFIG_HPP_NXI