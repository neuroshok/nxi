#include <ui/view/config.hpp>

#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>

#include <nxi/config.hpp>
#include <nxi/core.hpp>
#include <nxi/user.hpp>
#include <ui/core.hpp>

namespace ui::views
{
    config::config(ui::user& user)
        : user_{ user }
    {
        connect(&user_.nxi_core(), &nxi::core::event_load, [this] {
            auto& cfg = user_.nxi_user().config();
            for (auto key : cfg.list())
            {
                std::visit(overloaded{ [this](auto config_value) { add_row(config_value); } }, key);
            }
        });

        form_ = new QFormLayout(this);
    }
} // nxi::views