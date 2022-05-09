#ifndef INCLUDE_NXI_DATA_SESSION_HPP_NXI
#define INCLUDE_NXI_DATA_SESSION_HPP_NXI

#define MAKE_STRUCT
#include "session_struct.hpp"

#define MAKE_TABLE
#include "session_struct.hpp"

#define MAKE_MODEL
#include "session_struct.hpp"

#include <nxi/database/result.hpp>

namespace nxi
{
    class database;
} // nxi

namespace nxi::data::session
{
    unsigned int add(nxi::database&, const nxi::session_data&);
    void del(nxi::database&, int session_id);
    nxi::result get(nxi::database&);
    void load(nxi::database&, int id);
    void unload(nxi::database&, int id);
} // nxi::data::session

#endif // INCLUDE_NXI_DATA_SESSION_HPP_NXI