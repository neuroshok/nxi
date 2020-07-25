#ifndef INCLUDE_NXI_DATABASE_CONTEXT_HPP_NXI
#define INCLUDE_NXI_DATABASE_CONTEXT_HPP_NXI

#include <nxi/database/field.hpp>
#include <nxi/type.hpp>

#include <QString>

namespace nxi
{
    class context;
    class core;
    class database;
    class result;

    struct context_data
    {
        QString name = "context";
        unsigned int priority = 1;
        bool enabled = false;

        static context_data from_get(const nxi::result& result);
    };
} // nxi

namespace nxi::data::context
{
    void add(nxi::database& db, const nxi::context&);
    void add_available(nxi::database&, const nxi::context_data&);
    void del(nxi::database& db, const nxi::context&);
    nxi::result get(nxi::database&);
    nxi::result get_available(nxi::database&);

} // nxi::data::context

namespace nxi::data::context::internal
{
    void make(nxi::database&);
    void prepare(nxi::database&);

    inline static constexpr struct table_context
    {
        inline static constexpr nxi::field<0, QString> name{};
        inline static constexpr nxi::field<1, int> priority{};
        inline static constexpr nxi::field<2, bool> enabled{};
    } context{};

    constexpr std::string_view str_table_context = R"__(
        CREATE TABLE `context`
        (
            `name` text(24),
            `priority` integer default 1,
            `enabled` integer default 0,
            PRIMARY KEY(`name`)
        )
    )__";
} // nxi::data::context::internal

#endif // INCLUDE_NXI_DATABASE_CONTEXT_HPP_NXI
