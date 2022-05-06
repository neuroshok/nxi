#include <nxi/data/init.hpp>

#include <QString>

init_open(user) init_field(0, int, id, "integer") init_field(1, QString, name, "text") init_field(2, bool, active, "integer")
init_field(3, bool, focus, "integer") init_SQL("PRIMARY KEY(`id`)") init_close(user)

#include <nxi/data/deinit.hpp>