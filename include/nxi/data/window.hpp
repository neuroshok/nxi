#ifndef INCLUDE_NXI_DATA_WINDOW_HPP_NXI
#define INCLUDE_NXI_DATA_WINDOW_HPP_NXI

#include <nxi/database/result.hpp>
#include <nxi/type.hpp>

namespace nxi
{
    class core;
    class database;

    struct window_data
    {
        int id = 0;
        int x = 0;
        int y = 0;
        int w = 0;
        int h = 0;
    };
}

namespace nxi::data::window
{
    unsigned int add_window(nxi::core& core, nxi::window_data);
    nxi::result get_windows(nxi::core& core);
} // nxi::data::window

namespace nxi::data::window::internal
{
    void make(nxi::database& db);
    void prepare(nxi::database& db);

    inline static constexpr struct table
    {
        nxi::field<0, int> id{};
        nxi::field<1, int> x{};
        nxi::field<2, int> y{};
        nxi::field<3, int> w{};
        nxi::field<4, int> h{};
    } window;

    constexpr std::string_view str_table = R"__(
        CREATE TABLE `window`
        (
            `id` integer,
            `x` integer,
            `y` integer,
            `w` integer,
            `h` integer,
            PRIMARY KEY(`id`)
        )
        )__";
} // nxi::data::window::internal

#endif // INCLUDE_NXI_DATA_WINDOW_HPP_NXI