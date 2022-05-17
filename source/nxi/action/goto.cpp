//
// Created by Lezenn on 15/05/2022.
//

#include <nxi/action/goto.hpp>

namespace nxi
{
    goto_action::goto_action(QString uri, QString icon) noexcept
        : text_{ std::move(uri) }
        , icon_{ std::move(icon) }
        , info_{ "Go to URI " + uri }
    {}

    const QString& goto_action::info() const noexcept { return info_; }
    const QString& goto_action::text() const noexcept { return text_; }
    const QString& goto_action::icon() const noexcept { return icon_; }
}