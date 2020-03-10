#include <nxi/suggestion.hpp>

namespace nxi
{
    class page;

    suggestion::suggestion(suggestion_type type)
         : type_{ type }
         , icon_{ ":/image/nex" }
         , text_{ "suggestion" }
    {}

    suggestion::suggestion(QString text, QString icon, QString info)
         : type_{ suggestion_type::text }
         , icon_{ std::move(icon) }
         , info_{ std::move(info) }
         , text_{ std::move(text) }
    {}

    const QString& suggestion::icon() const { return icon_; };
    const QString& suggestion::info() const { return info_; };
    const QString& suggestion::text() const { return text_; };
    suggestion_type suggestion::type() const { return type_; }

    template<> suggestion_type suggestion::get_type<nxi::command>() { return suggestion_type::command; }
    template<> suggestion_type suggestion::get_type<nxi::page>() { return suggestion_type::page; }
} // nxi