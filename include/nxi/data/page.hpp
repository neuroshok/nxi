#ifndef INCLUDE_NXI_DATA_PAGE_HPP_NXI
#define INCLUDE_NXI_DATA_PAGE_HPP_NXI

#include <nxi/database/result.hpp>
#include <nxi/type.hpp>

namespace nxi
{
    class core;
    class database;
    class page;
} // nxi

class QString;

namespace nxi::data::page
{
    nxi::page_id add(nxi::database&, nxi::page&);
    void add_arc(nxi::database&, int source_id, int target_id);
    size_t count(nxi::database&);
    nxi::result get_page(nxi::database&, int id);
    nxi::result get_page(nxi::database&, const QString& name);
    nxi::result update(nxi::database&, const nxi::page&);
} // nxi::data::page

namespace nxi::data::page::internal
{
    void make(nxi::database&);
    void prepare(nxi::database&);

    inline static constexpr struct table_page
    {
        inline static constexpr nxi::field<0, int> id{};
        inline static constexpr nxi::field<1, QString> name{};
        inline static constexpr nxi::field<2, QString> command{};
        inline static constexpr nxi::field<3, int> type{};
        inline static constexpr nxi::field<4, int> renderer_type{};
        inline static constexpr nxi::field<5, int> loaded{};
        inline static constexpr nxi::field<6, int> muted{};
    } page{};

    constexpr std::string_view str_table_page = R"__(
        CREATE TABLE `page`
        (
            `id` integer,
            `name` text(8),
            `command` text(8),
            `type` integer,
            `renderer_type` integer,
            `loaded` integer,
            `muted` integer,
            PRIMARY KEY(`id`)
        )
    )__";

    inline static constexpr struct table_page_arc
    {
        inline static constexpr nxi::field<0, int> source_id{};
        inline static constexpr nxi::field<1, int> target_id{};
    } page_arc{};

    constexpr std::string_view str_table_page_arc = R"__(
        CREATE TABLE `page_arc`
        (
            `source_id` integer,
            `target_id` integer,
            PRIMARY KEY(`source_id`, `target_id`)
        )
    )__";
} // nxi::data::page::internal

#endif // INCLUDE_NXI_DATA_PAGE_HPP_NXI