// clang-format off

#include <nxi/data/init.hpp>

#include <QString>

init_open(navigation)
    init_field(0, QString, datetime, "text")
    init_field(1, int, page_id, "integer")
    init_field(2, QString, source, "text")
    init_lfield(3, QString, target, "text")
init_close(navigation)

#include <nxi/data/deinit.hpp>