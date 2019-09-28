#include <nxi/utility/json_loader.hpp>

namespace nxi
{
    json_object::json_object(QJsonValueRef value)
        : value_{ value }
    {}
    json_object::json_object(json_object* parent, const QString& key)
        : value_{ parent->value_.toObject()[key] }
    {}

    template<>
    int json_load_value<int>(const QJsonValue& value)
    {
        return value.toInt();
    }

    template<>
    QSize json_load_value<QSize>(const QJsonValue& value)
    {
        QJsonArray array{ value.toArray() };
        nxi_assert(array.size() == 2);
        return QSize{ array[0].toInt(), array[1].toInt() };
    }

    template<>
    QImage json_load_value<QImage>(const QJsonValue& value)
    {
        return QImage{ value.toString() };
    }
} // nxi