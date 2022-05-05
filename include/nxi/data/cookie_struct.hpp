#include <nxi/data/init.hpp>

#include <QString>
#include <QNetworkCookie>

init_open(cookie)
    init_field(0, int, id, "integer")
    init_field(1, QString, domain, "text")
    init_field(2, int64_t, expiration, "integer")
    init_field(3, bool, http_only, "integer")
    init_field(4, QByteArray, name, "text")
    init_field(5, QString, path, "text")
    init_field(6, int, same_site, "integer")
    init_field(7, bool, secured, "integer")
    init_field(8, QByteArray, value, "text")
    init_field(9, int, session_id, "integer")
    init_SQL("CONSTRAINT `unique_cookie` UNIQUE(`name`,`domain`,`path`),")
init_close(cookie)

#include <nxi/data/deinit.hpp>