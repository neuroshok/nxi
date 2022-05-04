#ifndef INCLUDE_NXI_DATABASE_SESSION_HPP_NXI
#define INCLUDE_NXI_DATABASE_SESSION_HPP_NXI

#include <nxi/database/result.hpp>
#include <nxi/type.hpp>

namespace nxi
{
    class core;
    class database;

    struct session_data
    {
        int id;
        QString name = "unknown_session";
        bool active = false;
    };
}

namespace nxi::data::session
{
    unsigned int add_session(nxi::database&, const QString& session_id);
    void del_session(nxi::database&, const QString& session_id);
    nxi::result get_sessions(nxi::database&);
    void load_session(nxi::database&, const QString& session_id);
    void unload_session(nxi::database&, const QString& session_id);
} // nxi::data::session

namespace nxi::data::session::internal
{
    void make(nxi::database& db);
    void prepare(nxi::database& db);

    inline static constexpr struct table
    {
        nxi::field<0, int> id{};
        nxi::field<1, QString> name{};
        nxi::field<2, bool> active{};
    } window{};

    constexpr std::string_view str_table = R"__(
        CREATE TABLE `session`
        (
            `id` integer,
            `name` text(24),
            `active` integer,
            PRIMARY KEY(`id`)
        )
        )__";
} // nxi::data::session::internal

#endif // INCLUDE_NXI_DATABASE_SESSION_HPP_NXI