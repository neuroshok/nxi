#ifndef INCLUDE_NXI_DATA_MODULE_HPP_NXI
#define INCLUDE_NXI_DATA_MODULE_HPP_NXI

#include <nxi/database/field.hpp>
#include <nxi/type.hpp>

#include <QString>

namespace nxi
{
    class core;
    class database;
    class result;

    struct module_data
    {
        int id = 0;
        QString name = "__module";
        nxi::module_type type = nxi::module_type::dynamic;
        bool loaded = false;

        static module_data from_get(const nxi::result& result);
    };
} // nxi

namespace nxi::data::module
{
    nxi::result get(nxi::database& db);
    void set_loaded(nxi::database&, int module_id, bool state);
} // nxi::data::module

namespace nxi::data::module::internal
{
    void make(nxi::database&);
    void prepare(nxi::database&);

    inline static constexpr struct table_module
    {
        inline static constexpr nxi::field<0, int> id{};
        inline static constexpr nxi::field<1, QString> name{};
        inline static constexpr nxi::field<2, int> type{};
        inline static constexpr nxi::field<3, bool> loaded{};
    } module{};

    constexpr std::string_view str_table_module = R"__(
        CREATE TABLE `module`
        (
            `id` integer,
            `name` text(8),
            `type` integer,
            `loaded` integer,
            PRIMARY KEY(`id`)
        )
    )__";

} // nxi::data::module::internal

#endif // INCLUDE_NXI_DATA_MODULE_HPP_NXI