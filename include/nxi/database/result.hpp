#ifndef INCLUDE_NXI_DATABASE_RESULT_HPP_NXI
#define INCLUDE_NXI_DATABASE_RESULT_HPP_NXI

#include <nxi/database/field.hpp>
#include <QSqlQuery>

namespace nxi
{
    struct result
    {
        result(QSqlQuery& query) : query_{ query }
        {
            query_.exec();
        }

        bool next() const { return query_.next(); }
        unsigned int size() const { return (query_.size() > -1) ? query_.size() : 0; }

        template<class Field_ref>
        auto operator[](Field_ref&& field) const
        {
            using Field = std::decay_t<Field_ref>;
            if constexpr (std::is_same_v<Field::type, int>) { return query_.value(Field::index).toInt(); }
            if constexpr (std::is_same_v<Field::type, bool>) { return query_.value(Field::index).toBool(); }
            else if constexpr (std::is_same_v<Field::type, QString>) { return query_.value(Field::index).toString(); }
        }

        QSqlQuery& query_;
    };
} // nxi

#endif // INCLUDE_NXI_DATABASE_RESULT_HPP_NXI
