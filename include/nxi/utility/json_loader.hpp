#ifndef INCLUDE_NXI_CONFIG_JSON_LOADER_HPP_NXI
#define INCLUDE_NXI_CONFIG_JSON_LOADER_HPP_NXI

#include <nxi/preprocessor.hpp>
#include <nxi/log.hpp>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonValueRef>

#include <QString>
#include <QSize>
#include <QImage>

#define nxi_json_key_overload2(NAME, TYPE) ::nxi::json_value<TYPE> NAME{ this, #NAME };
#define nxi_json_key_overload3(NAME, TYPE, DEFAULT_VALUE) ::nxi::json_value<TYPE> NAME{ this, #NAME, TYPE DEFAULT_VALUE };

#define nxi_json_open(GROUP_NAME) struct : ::nxi::json_object { using json_object::json_object;
#define nxi_json_close(GROUP_NAME) } GROUP_NAME{ this, #GROUP_NAME };
#define nxi_json_key(...) nxm_overload(nxi_json_key_overload, __VA_ARGS__)

namespace nxi
{
    template<class T>
    T json_load_value(const QJsonValue& value)
    {
        return value.toString();
    }

    template<class T> class json_value;

    struct json_object
    {
        template<class T> friend class json_value;
    public:
        json_object() = default;
        json_object(QJsonValueRef value);
        json_object(json_object* parent, const QString& key);
    private:
        QJsonValue value_;
    };

    template<class T>
    class json_value
    {
    public:
        json_value(json_object* parent, const QString& key, const T& default_value = T{})
            : value_{ init_value(parent, key, default_value) }
        {}

        const T& get() const { return value_; }
        void set(const T& v) { value_ = v; }
        bool is_null() const { return is_null_; }

    private:
        T init_value(json_object* parent, const QString& key, const T& default_value)
        {
            is_null_ = false;
            if (!parent->value_.toObject()[key].isNull()) return json_load_value<T>(parent->value_.toObject()[key]);
            is_null_ = true;
            return default_value;
        }

    private:
        bool is_null_;
        T value_;
    };

    template<> int json_load_value<int>(const QJsonValue&);
    template<> QSize json_load_value<QSize>(const QJsonValue&);
    template<> QImage json_load_value<QImage>(const QJsonValue&);
} // nxi

#endif // INCLUDE_NXI_CONFIG_JSON_LOADER_HPP_NXI
