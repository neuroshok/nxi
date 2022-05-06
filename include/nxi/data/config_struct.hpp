#include <nxi/data/init.hpp>

#include <QString>

init_open(config) init_field(0, QString, key, "text") init_field(1, int, type, "integer") init_field(2, QByteArray, value, "blob")
init_field(3, int, session_id, "integer") init_SQL("PRIMARY KEY(`key`, `session_id`)") init_close(config)

#include <nxi/data/deinit.hpp>