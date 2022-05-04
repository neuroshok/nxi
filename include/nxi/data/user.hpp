#ifndef INCLUDE_NXI_DATA_USER_HPP_NXI
#define INCLUDE_NXI_DATA_USER_HPP_NXI

#include <nxi/database/field.hpp>
#include <nxi/type.hpp>

#define MAKE_STRUCT
#include "user_struct.hpp"

#define MAKE_TABLE
#include "user_struct.hpp"

#define MAKE_MODEL
#include "user_struct.hpp"

namespace nxi::data::user
{
    unsigned int add(nxi::database&, const nxi::user_data&);
    void del(nxi::database&, int session_id);
    nxi::result get(nxi::database&);
    void load(nxi::database&, int id);
    void unload(nxi::database&, int id);
} // nxi::data::user

#endif // INCLUDE_NXI_DATA_USER_HPP_NXI