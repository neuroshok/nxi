#ifndef DATABASE_PAGE_H_NXI
#define DATABASE_PAGE_H_NXI

#include <ndb/preprocessor.hpp>

ndb_object(page
    , ndb_field(name, QString)
    , ndb_field(command, QString)
    , ndb_field(type, nxi::page_type)
    , ndb_field(renderer_type, nxi::renderer_type)
    , ndb_field(loaded, bool)
    , ndb_field(muted, bool)
)

ndb_object(page_system
    , ndb_field(focus, nxi::page_id)
    , ndb_field(active, std::vector<nxi::page_id>)
)

ndb_table(
    page_connection
    , ndb_field(source_id, nxi::page_id)
    , ndb_field(target_id, nxi::page_id)
)

#endif // DATABASE_PAGE_H_NXI