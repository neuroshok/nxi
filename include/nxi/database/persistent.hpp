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
            , nxi::persistent<unsigned int>*
            , nxi::persistent<double>*
            , nxi::persistent<QString>*
            >>;
        inline static view_type view_;

      public:
        persistent_group(const persistent_group* parent_group, QString name, nxi::database& db, int session_id)
            : name_{ std::move(name) }
            , path_{ parent_group ? (parent_group->path() + "." + name_) : name_ }
            , database_{ db }
            , session_id_{ session_id }
        {}

        persistent_group(const persistent_group* parent_group, QString name)
            : persistent_group{ parent_group, name, parent_group->database_, parent_group->session_id() }
        {}

        persistent_group(QString name, nxi::database& db, int session_id)
            : persistent_group(nullptr, std::move(name), db, session_id)
        {}

        const view_type& list() const { return view_; }
        const QString& name() const { return name_; }
        const QString& path() const { return path_; }
        int session_id() const { return session_id_; }

      private:
        QString name_;
        QString path_;
        nxi::database& database_;
        int session_id_;
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
            , session_id_{ group->session_id() }
            , default_value_{ default_value }
        {
            persistent_group::view_.push_back(this);

            nxi::result result = nxi::data::config::get(database_, path_, session_id_);

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

            nxi::data::config::set(database_, path_, data, session_id_);
        }

      private:
        nxi::database& database_;
        QString name_;
        QString path_;
        T value_;
        T default_value_;
        int session_id_;
    };
} // nxi

#endif // INCLUDE_NXI_DATABASE_PERSISTENT_HPP_NXI