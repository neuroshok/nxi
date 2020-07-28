#ifndef INCLUDE_NXI_DATA_PAGE_HPP_NXI
#define INCLUDE_NXI_DATA_PAGE_HPP_NXI

#include <nxi/database/field.hpp>
#include <nxi/type.hpp>

#include <QString>

namespace nxi
{
    class core;
    class database;
    class page;
    class result;

    struct page_data
    {
        nxi::page_id id = 0;
        QString name = "new page";
        QString command;
        nxi::page_type type = nxi::page_type::custom;
        nxi::renderer_type renderer_type = nxi::renderer_type::widget;
        bool loaded = false;
        bool muted = false;

        static page_data from_get(const nxi::result& result);
    };
} // nxi

namespace nxi::data::page
{
    nxi::page_id add(nxi::database&, nxi::page&);
    size_t count(nxi::database&);
    nxi::result get(nxi::database&);
    nxi::result get(nxi::database&, int id);
    nxi::result get(nxi::database&, const QString& name);
    void del(nxi::database&, nxi::page&);
    void set_loaded(nxi::database&, nxi::page_id, bool state);
    void update(nxi::database&, const nxi::page&);

    void add_arc(nxi::database&, int source_id, int target_id);
    void del_arc(nxi::database&, int page_id);
    nxi::result get_arcs(nxi::database&);
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
        inline static constexpr nxi::field<5, bool> loaded{};
        inline static constexpr nxi::field<6, bool> muted{};
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