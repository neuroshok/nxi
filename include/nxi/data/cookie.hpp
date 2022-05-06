#ifndef INCLUDE_NXI_DATA_COOKIE_HPP_NXI
#define INCLUDE_NXI_DATA_COOKIE_HPP_NXI

#define MAKE_STRUCT
#include "cookie_struct.hpp"

#define MAKE_TABLE
#include "cookie_struct.hpp"

#define MAKE_MODEL
#include "cookie_struct.hpp"

#include <nxi/database/result.hpp>

namespace nxi
{
    class database;
    struct cookie_data;
} // nxi

class QNetworkCookie;

namespace nxi::data::cookie
{
    nxi::result get(nxi::database&, int session_id);
    nxi::result get(nxi::database&, const QString& domain, int session_id);
    void del(nxi::database&, const QNetworkCookie&);
    void set(nxi::database&, const QNetworkCookie&, int session_id);

    nxi::cookie_data from_get(const nxi::result&);
    QNetworkCookie make(const nxi::cookie_data&);
} // nxi::data::cookie

#endif // INCLUDE_NXI_DATA_COOKIE_HPP_NXI