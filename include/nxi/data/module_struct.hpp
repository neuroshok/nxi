// clang-format off

#include <nxi/data/init.hpp>

#include <nxi/type.hpp>

#include <QString>

init_open(module)
    init_field(0, int, id, "integer")
    init_field(1, QString, name, "text")
    init_field(2, nxi::module_type, type, "integer")
    init_field(3, bool, active, "integer")
    init_SQL("PRIMARY KEY(`id`)")
init_close(module)

#include <nxi/data/deinit.hpp>