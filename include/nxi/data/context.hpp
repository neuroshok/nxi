#ifndef INCLUDE_NXI_DATA_CONTEXT_HPP_NXI
#define INCLUDE_NXI_DATA_CONTEXT_HPP_NXI

#define MAKE_STRUCT
#include "context_struct.hpp"

#define MAKE_TABLE
#include "context_struct.hpp"

#define MAKE_MODEL
#include "context_struct.hpp"

#include <nxi/data/context.hpp>
#include <nxi/database/result.hpp>

namespace nxi
{
    class context;
    class database;
} // nxi

namespace nxi::data::context
{
    void add(nxi::database& db, const nxi::context&);
    void add_available(nxi::database&, const nxi::context_data&);
    void del(nxi::database& db, const nxi::context&);
    nxi::result get(nxi::database&);
    nxi::result get_available(nxi::database&);

    nxi::context_data from_get(const nxi::result& result);
} // nxi::data::context

#endif // INCLUDE_NXI_DATA_CONTEXT_HPP_NXI