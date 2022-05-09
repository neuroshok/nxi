// clang-format off

#include <nxi/data/init.hpp>

#include <nxi/type.hpp>

#include <QString>

init_open(page) init_field(0, nxi::page_id, id, "integer")
    init_field(1, QString, name, "text")
    init_field(2, QString, command, "text")
    init_field(3, nxi::page_type, type, "integer")
    init_field(4, nxi::renderer_type, renderer_type, "integer")
    init_field(5, bool, loaded, "integer")
    init_field(6, bool, audible, "integer")
    init_field(7, bool, muted, "integer")
    init_field(8, int, session_id, "integer")
    init_SQL("PRIMARY KEY(`id`)")
init_close(page)

init_open(page_arc)
    init_field(0, int, source_id, "integer")
    init_field(1, int, target_id, "integer")
    init_SQL("PRIMARY KEY(`source_id`, `target_id`)")
init_close(page_arc)

#include <nxi/data/deinit.hpp>