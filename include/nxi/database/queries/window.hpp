#ifndef INCLUDE_NXI_DATABASE_QUERIES_WINDOW_HPP_NXI
#define INCLUDE_NXI_DATABASE_QUERIES_WINDOW_HPP_NXI

#include <nxi/database.hpp>

namespace nxi::queries
{
    const auto& window = nxi_model.window;

    ndb_prepare(dbs::core, add_window, window.x, window.y, window.w, window.h) { ndb_pquery << (ndb::add(window.x = _0 , window.y = _1 , window.w = _2 , window.h = _3); }
    ndb_prepare(dbs::core, del_window, window.id) { ndb_pquery << (ndb::del << ndb::source(window) << ndb::filter(window.id == _0)); }
    ndb_prepare(dbs::core, move_window, window.id, window.x, window.y) { ndb_pquery << (ndb::set(window.x = _0, window.y = _1) << ndb::filter(window.id == _0)); }
}

#endif // INCLUDE_NXI_DATABASE_QUERIES_WINDOW_HPP_NXI
