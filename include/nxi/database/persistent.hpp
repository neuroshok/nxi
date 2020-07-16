#ifndef INCLUDE_NXI_DATABASE_PERSISTENT_HPP_NXI
#define INCLUDE_NXI_DATABASE_PERSISTENT_HPP_NXI

#include <nxi/database/model.hpp>

#include <variant>
#include <vector>

#include <QByteArray>
#include <QString>

namespace nxi
{
    template<class T>
    class persistent;

    class persistent_group
    {
        template<class T> friend class persistent;

        using view_type = std::vector<std::variant<
            nxi::persistent<bool>*
            , nxi::persistent<int>*
            , nxi::persistent<double>*
            , nxi::persistent<QString>*
            >>;
        inline static view_type view_;

      public:
        persistent_group(const persistent_group* parent_group, QString name, nxi::database& db)
            : name_{ std::move(name) }
            , path_{ parent_group ? (parent_group->path() + "." + name_) : name_ }
            , database_{ db }
        {}

        persistent_group(const persistent_group* parent_group, QString name)
            : persistent_group{ parent_group, name, parent_group->database_ }
        {}

        persistent_group(QString name, nxi::database& db)
            : persistent_group(nullptr, std::move(name), db)
        {}

        const view_type& list() const { return view_; }
        const QString& name() const { return name_; }
        const QString& path() const { return path_; }

      private:
        QString name_;
        QString path_;
        nxi::database& database_;
    };

    template<class T>
    class persistent
    {
      public:
        using value_type = T;

        persistent(const persistent_group* group, QString name, const T& default_value = T{})
            : database_{ group->database_ }
            , name_{ std::move(name) }
            , path_{ group->path() + "." + name_ }
            , value_{}
            , default_value_{ default_value }
        {
            persistent_group::view_.push_back(this);

            nxi::result result = nxi::data::config::get(database_, path_);

            if (result.next())
            {
                QByteArray data = result[nxi_model.config.value];

                if constexpr (!std::is_fundamental_v<T>) value_ = T{ data };
                else value_ = *reinterpret_cast<T*>(data.data());
            }
            else value_ = default_value_;
        }

        operator T() const { return value_; }

        const T& get() const { return value_; }
        const QString& name() const { return name_; }
        const QString& path() const { return path_; }
        const T& default_value() const { return default_value_; }

        void operator=(T value)
        {
            // ignore update if value didn't change
            if (value_ == value)
                return;

            value_ = std::move(value);

            QByteArray data;

            if constexpr (std::is_same_v<T, QString>)
            {
                data.append(value_.toUtf8());
            }
            else if constexpr (std::is_fundamental_v<T>)
            {
                auto begin = reinterpret_cast<char*>(&value_);
                data.append(begin, sizeof(T));
            }

            nxi::data::config::set(database_, path_, data);
        }

      private:
        nxi::database& database_;
        QString name_;
        QString path_;
        T value_;
        T default_value_;
    };
} // nxi

#endif // INCLUDE_NXI_DATABASE_PERSISTENT_HPP_NXI
