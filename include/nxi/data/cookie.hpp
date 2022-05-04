#ifndef INCLUDE_NXI_DATA_COOKIE_HPP_NXI
#define INCLUDE_NXI_DATA_COOKIE_HPP_NXI

#include <nxi/database/field.hpp>
#include <nxi/type.hpp>

#define MAKE_STRUCT
#include "cookie_struct.hpp"

#define MAKE_TABLE
#include "cookie_struct.hpp"

#define MAKE_MODEL
#include "cookie_struct.hpp"

class QNetworkCookie;

namespace nxi::data::cookie
{
    nxi::result get(nxi::database&);
    nxi::result get(nxi::database&, const QString& domain);
    void del(nxi::database&, const QNetworkCookie&);
    void set(nxi::database&, const QNetworkCookie&);

    nxi::cookie_data from_get(const nxi::result&);
    QNetworkCookie make(const nxi::cookie_data&);
} // nxi::data::cookie_data

#endif // INCLUDE_NXI_DATA_COOKIE_HPP_NXI