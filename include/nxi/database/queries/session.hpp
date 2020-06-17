#ifndef INCLUDE_NXI_DATABASE_QUERIES_SESSION_HPP_NXI
#define INCLUDE_NXI_DATABASE_QUERIES_SESSION_HPP_NXI

#include <nxi/database.hpp>

namespace nxi::queries
{
    const auto& session = nxi_model.session;

    ndb_prepare(dbs::core, get_active_sessions) { ndb_pquery << (ndb::get() << ndb::source(session) <<  ndb::filter(session.active == true)); }
    ndb_prepare(dbs::core, get_session, session.name) { ndb_pquery << (ndb::get() << ndb::source(session) <<  ndb::filter(session.id == _0)); }
    ndb_prepare(dbs::core, add_session, session.name) { ndb_pquery << ndb::add(session.name = _0); }
    ndb_prepare(dbs::core, del_session, session.id) { ndb_pquery << (ndb::del << ndb::source(session) << ndb::filter(session.id == _0)); }
}

#endif // INCLUDE_NXI_DATABASE_QUERIES_SESSION_HPP_NXI
