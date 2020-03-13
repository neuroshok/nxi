#include <nxi/suggestion/search.hpp>

namespace nxi
{

    search_suggestion::search_suggestion(QString text, QString name, QString url, QString icon)
        : name_{ std::move(name) }
        , icon_{ std::move(icon) }
        , info_{ "search on " + name }
        , text_{ std::move(text) }
        , url_{ std::move(url) }
    {}

    const QString& search_suggestion::name() const
    {
        return name_;
    }
    const QString& search_suggestion::icon() const
    {
        return icon_;
    }
    const QString& search_suggestion::info() const
    {
        return info_;
    }
    const QString& search_suggestion::text() const
    {
        return text_;
    }
    const QString& search_suggestion::url() const
    {
        return url_;
    }
} // nxi