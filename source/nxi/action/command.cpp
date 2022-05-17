//
// Created by Lezenn on 15/05/2022.
//

#include <nxi/action/command.hpp>
#include <nxi/command.hpp>

namespace nxi
{
    command_action::command_action(nds::node_ptr<const nxi::command> data) noexcept
        : data_{ data }
    {}

    const QString& command_action::info() const noexcept { return data_->description(); }
    const QString& command_action::text() const noexcept { return data_->name(); }
    const QString& command_action::icon() const noexcept { return data_->icon(); }
}
