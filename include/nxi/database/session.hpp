#ifndef INCLUDE_NXI_DATABASE_SESSION_HPP_NXI
#define INCLUDE_NXI_DATABASE_SESSION_HPP_NXI

#include <ndb/preprocessor.hpp>

ndb_table(
    session
    , ndb_field_id
    , ndb_field(name, QString)
    , ndb_field(active, bool)
)

#endif // INCLUDE_NXI_DATABASE_SESSION_HPP_NXI
