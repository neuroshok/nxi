#include <nxi/type.hpp>

namespace nxi
{
    class core;
    class database;
}

class QString;

namespace nxi::data::page
{
    nxi::page_id add_page(nxi::core& core, const QString& name);
    auto get_page(nxi::core& core, int id);
    auto get_page(nxi::core& core, const QString& name);
} // nxi::data::page

namespace nxi::data::page::internal
{
    void make(nxi::database& db);
    void prepare(nxi::database& db);
} // nxi::data::page::internal
