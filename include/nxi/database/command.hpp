#ifndef INCLUDE_NXI_DATABASE_COMMAND_HPP_NXI
#define INCLUDE_NXI_DATABASE_COMMAND_HPP_NXI

#include <ndb/preprocessor.hpp>

ndb_object(command
    , ndb_field(name, QString)
    , ndb_field(shortcut, QString)
    , ndb_field(shortcut_context, QString)
)

#endif // INCLUDE_NXI_DATABASE_COMMAND_HPP_NXI
