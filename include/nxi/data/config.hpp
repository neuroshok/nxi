#ifndef INCLUDE_NXI_DATA_CONFIG_HPP_NXI
#define INCLUDE_NXI_DATA_CONFIG_HPP_NXI

#define MAKE_STRUCT
#include "config_struct.hpp"

#define MAKE_TABLE
#include "config_struct.hpp"

#define MAKE_MODEL
#include "config_struct.hpp"

#include <nxi/database.hpp>
#include <nxi/database/result.hpp>

namespace nxi::data::config
{
    nxi::result get(nxi::database& db, const QString& key, int session_id);

    template<class T>
    inline void set(nxi::database& db, const QString& key, const T& value, int session_id)
    {
        auto& query = db.prepared_query(nxi::prepared_query::set_config_value);
        query.bindValue(0, key);
        query.bindValue(1, value);
        query.bindValue(2, session_id);
        if (!query.exec()) nxi_error("query error : {}", query.lastError().text());
    }
} // nxi::data::config

#endif // INCLUDE_NXI_DATA_CONFIG_HPP_NXI