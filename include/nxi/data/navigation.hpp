#ifndef INCLUDE_NXI_DATA_NAVIGATION_HPP_NXI
#define INCLUDE_NXI_DATA_NAVIGATION_HPP_NXI

#define MAKE_STRUCT
#include "navigation_struct.hpp"

#define MAKE_TABLE
#include "navigation_struct.hpp"

#define MAKE_MODEL
#include "navigation_struct.hpp"

#include <nxi/type.hpp>

namespace nxi
{
    class database;
} // nxi

namespace nxi::data::navigation
{
    void log(nxi::database&, nxi::page_id, const QString& source, const QString& target, int type);
} // nxi::data::navigation

#endif // INCLUDE_NXI_DATA_NAVIGATION_HPP_NXI