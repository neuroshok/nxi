//
// Created by Lezenn on 15/05/2022.
//

#include <nxi/action/text.hpp>

namespace nxi
{
    text_action::text_action(QString text, QString info, QString icon) noexcept
        : text_{ std::move(text) }
        , info_{ std::move(info) }
        , icon_{ std::move(icon) }
    {}

    const QString& text_action::info() const noexcept { return info_; }

    const QString& text_action::text() const noexcept { return text_; }

    const QString& text_action::icon() const noexcept { return icon_; }
}