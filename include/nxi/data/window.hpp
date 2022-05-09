#ifndef INCLUDE_NXI_DATA_WINDOW_HPP_NXI
#define INCLUDE_NXI_DATA_WINDOW_HPP_NXI

#define MAKE_STRUCT
#include "window_struct.hpp"

#define MAKE_TABLE
#include "window_struct.hpp"

#define MAKE_MODEL
#include "window_struct.hpp"

#include <nxi/database/result.hpp>

namespace nxi
{
    class database;
} // nxi

namespace nxi::data::window
{
    unsigned int add_window(nxi::database& core, nxi::window_data);
    nxi::result get_windows(nxi::database& core);
    void move_window(nxi::database& core, unsigned int id, int x, int y);
} // nxi::data::window

#endif // INCLUDE_NXI_DATA_WINDOW_HPP_NXI