#include <nxi/data/config.hpp>

#include <nxi/database.hpp>

namespace nxi::data::config
{
    nxi::result get(nxi::database& db, const QString& key, int session_id)
    {
        auto& query = db.prepared_query(nxi::prepared_query::get_config_value);
        query.bindValue(0, key);
        query.bindValue(1, session_id);

        return nxi::result{ query };
    }
} // nxi::data::config

namespace nxi::data::config::internal
{
    void prepare(nxi::database& db)
    {
        db.prepare(prepared_query::get_config_value, "SELECT * FROM config WHERE key = ? AND session_id = ?");
        db.prepare(prepared_query::set_config_value, "INSERT OR REPLACE INTO config(key, value, session_id) VALUES(?, ?, ?)");
    }
} // nxi::data::config::internal