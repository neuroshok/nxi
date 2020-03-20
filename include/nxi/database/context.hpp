#ifndef INCLUDE_NXI_DATABASE_CONTEXT_HPP_NXI
#define INCLUDE_NXI_DATABASE_CONTEXT_HPP_NXI

#include <ndb/preprocessor.hpp>

ndb_object(
    context_available
    , ndb_field(name, QString)
    , ndb_field(priority, unsigned int)
)

ndb_table(
    context
    , ndb_field_id
    , ndb_field(context_id, int)
)

#endif // INCLUDE_NXI_DATABASE_CONTEXT_HPP_NXI
