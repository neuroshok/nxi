#include <nxi/utility/json_loader.hpp>

namespace nxi
{
    json_object::json_object(QJsonValueRef value)
        : value_{ value.toObject() }
    {}

    json_object::json_object(QJsonObject value)
        : value_{ std::move(value) }
    {}

    json_object::json_object(json_object* parent, const QString& key)
        : value_{ parent->value_.toObject()[key].toObject() }
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

    template<>
    std::vector<QString> json_load_value<std::vector<QString>>(const QJsonValue& values)
    {
        std::vector<QString> vector;
        for (const auto& value : values.toArray())
        {
            vector.push_back(value.toString());
        }
        return vector;
    }
} // nxi