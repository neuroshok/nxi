//
// Created by Lezenn on 15/05/2022.
//

#include <nxi/action/search.hpp>

namespace nxi
{
    search_action::search_action(QString text, QString name, QString url, QString icon) noexcept
        : name_{ std::move(name) }
        , info_{ "search on " + name }
        , icon_{ std::move(icon) }
        , text_{ std::move(text) }
        , url_{ std::move(url) }
    {}

    const QString& search_action::icon() const noexcept { return icon_; }

    const QString& search_action::info() const noexcept { return info_; }

    const QString& search_action::text() const noexcept { return text_; }

    const QString& search_action::name() const noexcept { return name_; }

    const QString& search_action::url() const noexcept { return url_; }
}