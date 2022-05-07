// clang-format off

#include <nxi/data/init.hpp>

#include <nxi/type.hpp>

#include <QString>

init_open(session)
    init_field(0, int, id, "integer")
    init_field(1, QString, name, "text")
    init_field(2, bool, active, "integer")
    init_SQL("PRIMARY KEY(`id`)")
init_close(session)

#include <nxi/data/deinit.hpp>