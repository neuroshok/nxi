#include <nxi/suggestion/text.hpp>

namespace nxi
{
    text_suggestion::text_suggestion(QString text, QString icon, QString info)
        : icon_{ std::move(icon) }
        , info_{ std::move(info) }
        , text_{ std::move(text) }
    {}

    const QString& text_suggestion::icon() const
    {
        return icon_;
    }
    const QString& text_suggestion::info() const
    {
        return info_;
    }
    const QString& text_suggestion::text() const
    {
        return text_;
    }
} // nxi