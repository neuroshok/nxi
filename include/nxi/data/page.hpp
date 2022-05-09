#ifndef INCLUDE_NXI_DATA_PAGE_HPP_NXI
#define INCLUDE_NXI_DATA_PAGE_HPP_NXI

#define MAKE_STRUCT
#include "page_struct.hpp"

#define MAKE_TABLE
#include "page_struct.hpp"

#define MAKE_MODEL
#include "page_struct.hpp"

#include <nxi/database/result.hpp>
#include <nxi/type.hpp>

namespace nxi
{
    class database;
    class page;
} // nxi

namespace nxi::data::page
{
    nxi::page_id add(nxi::database&, nxi::page&);
    size_t count(nxi::database&);
    nxi::result get(nxi::database&);
    nxi::result get(nxi::database&, int id);
    nxi::result get(nxi::database&, const QString& name);
    void del(nxi::database&, nxi::page&);
    void set_loaded(nxi::database&, nxi::page_id, bool state);
    void update(nxi::database&, const nxi::page&);

    void add_arc(nxi::database&, int source_id, int target_id);
    void del_arc(nxi::database&, int page_id);
    nxi::result get_arcs(nxi::database&);

    nxi::page_data from_get(const nxi::result& result);
} // nxi::data::page

#endif // INCLUDE_NXI_DATA_PAGE_HPP_NXI