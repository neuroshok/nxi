#ifndef INCLUDE_NXI_DATA_MODULE_HPP_NXI
#define INCLUDE_NXI_DATA_MODULE_HPP_NXI

#define MAKE_STRUCT
#include "module_struct.hpp"

#define MAKE_TABLE
#include "module_struct.hpp"

#define MAKE_MODEL
#include "module_struct.hpp"

#include <nxi/database/result.hpp>

namespace nxi
{
    class database;
} // nxi

namespace nxi::data::module
{
    nxi::result get(nxi::database& db);
    void activate(nxi::database&, int module_id, bool state);

    nxi::module_data from_get(const nxi::result& result);
} // nxi::data::module

#endif // INCLUDE_NXI_DATA_MODULE_HPP_NXI