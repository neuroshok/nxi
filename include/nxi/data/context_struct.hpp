#include <nxi/data/init.hpp>

#include <QString>

init_open(context) init_field(0, QString, name, "text") init_field(1, int, priority, "integer default 1")
init_field(2, int, active, "integer default 0") init_SQL("PRIMARY KEY(`name`)") init_close(context)

#include <nxi/data/deinit.hpp>