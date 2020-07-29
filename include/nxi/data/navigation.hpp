#ifndef INCLUDE_NXI_DATA_NAVIGATION_HPP_NXI
#define INCLUDE_NXI_DATA_NAVIGATION_HPP_NXI

#include <nxi/database/field.hpp>
#include <nxi/type.hpp>

#include <QString>

namespace nxi
{
    class core;
    class database;
    class page;
    class result;
} // nxi

namespace nxi::data::navigation
{
    void log(nxi::database&, nxi::page_id, const QString& source, const QString& target, int type);
} // nxi::data::navigation

namespace nxi::data::navigation::internal
{
    void make(nxi::database&);
    void prepare(nxi::database&);

    static constexpr struct table_page
    {
        static constexpr nxi::field<0, QString> datetime{};
        static constexpr nxi::field<1, int> page_id{};
        static constexpr nxi::field<1, QString> source{};
        static constexpr nxi::field<2, QString> target{};
        static constexpr nxi::field<3, int> type{};
    } navigation_log{};

    constexpr std::string_view str_table_navigation_log = R"__(
        CREATE TABLE `navigation_log`
        (
            `datetime` datetime,
            `page_id` integer,
            `source` text(2048),
            `target` text(2048),
            `type` integer
        )
    )__";
} // nxi::data::navigation::internal

#endif // INCLUDE_NXI_DATA_NAVIGATION_HPP_NXI
