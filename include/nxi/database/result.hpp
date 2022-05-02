#ifndef INCLUDE_NXI_DATABASE_RESULT_HPP_NXI
#define INCLUDE_NXI_DATABASE_RESULT_HPP_NXI

#include <nxi/database/field.hpp>
#include <nxi/log.hpp>
#include <QSqlQuery>
#include <QSqlError>

namespace nxi
{
    class result
    {
    public:
        result(QSqlQuery& query) : query_{ query }
        {
            if (!query_.exec()) nxi_error("query error : {} : {}", query_.lastError().text(), query_.lastQuery());
        }

        bool next() const { return query_.next(); }
        unsigned int size() const
        {
            unsigned size = 0;
            if(query_.last())
            {
                size =  query_.at() + 1;
                query_.first();
                query_.previous();
            }
            return size;
        }

        template<class Field_ref>
        auto operator[](Field_ref&& field) const
        {
            using Field = std::decay_t<Field_ref>;
            if constexpr (std::is_same_v<typename Field::type, int>) { return query_.value(Field::index).toInt(); }
            else if constexpr (std::is_same_v<typename Field::type, int64_t>) { return query_.value(Field::index).toLongLong(); }
            else if constexpr (std::is_same_v<typename Field::type, bool>) { return query_.value(Field::index).toBool(); }
            else if constexpr (std::is_same_v<typename Field::type, QByteArray>) { return query_.value(Field::index).toByteArray(); }
            else if constexpr (std::is_same_v<typename Field::type, QString>) { return query_.value(Field::index).toString(); }
            else nxi_warning("Unknown type");
        }

    private:
        QSqlQuery& query_;
    };
} // nxi

#endif // INCLUDE_NXI_DATABASE_RESULT_HPP_NXI