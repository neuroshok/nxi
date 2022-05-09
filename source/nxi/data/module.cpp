#include <nxi/data/module.hpp>

#include <nxi/core.hpp>
#include <nxi/database/model.hpp>
#include <nxi/database/result.hpp>
#include <nxi/module.hpp>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

namespace nxi::data::module
{
    nxi::result get(nxi::database& db)
    {
        auto& query = db.prepared_query(nxi::prepared_query::get_modules);
        return nxi::result{ query };
    }

    void activate(database& db, int module_id, bool state)
    {
        auto& query = db.prepared_query(nxi::prepared_query::set_module_active);
        query.bindValue(0, state);
        query.bindValue(1, module_id);
        if (!query.exec()) nxi_error("query error : {}", query.lastError().text());
    }

    nxi::module_data from_get(const nxi::result& result)
    {
        nxi::module_data data;
        data.id = result[nxi_model.module.id];
        data.name = result[nxi_model.module.name];
        data.type = static_cast<nxi::module_type>(result[nxi_model.module.type]);
        data.active = result[nxi_model.module.active];
        return data;
    }
} // nxi::data::module

namespace nxi::data::module::internal
{
    void prepare(nxi::database& db)
    {
        db.prepare(prepared_query::get_modules, "SELECT * FROM module");
        db.prepare(prepared_query::set_module_active, "UPDATE module SET active = ? WHERE id = ?");
    }
} // nxi::data::page::internal