// clang-format off

#include <nxi/data/init.hpp>

init_open(window)
    init_field(0, int, id, "integer")
    init_field(1, int, x, "integer")
    init_field(2, int, y, "integer")
    init_field(3, int, w, "integer")
    init_field(4, int, h, "integer")
    init_SQL("PRIMARY KEY(`id`)")
init_close(window)

#include <nxi/data/deinit.hpp>