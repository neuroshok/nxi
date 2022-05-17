//
// Created by Lezenn on 15/05/2022.
//

#include <nxi/action/page.hpp>
#include <nxi/page.hpp>

namespace nxi
{
    page_action::page_action(nds::node_ptr<nxi::page> page) noexcept
        : page_{ page }
    {}

    const QString& page_action::info() const noexcept { return page_->command(); }
    const QString& page_action::text() const noexcept { return page_->name(); }
    const QString& page_action::icon() const noexcept { return page_->name(); }

} // nxi