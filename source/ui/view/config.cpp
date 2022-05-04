#include <ui/view/config.hpp>

#include <QLineEdit>
#include <QLabel>
#include <QFormLayout>

#include <ui/core.hpp>
#include <nxi/core.hpp>
#include <nxi/config.hpp>

namespace ui::views
{
    config::config(ui::user_session& session)
        : session_{ session }
    {
        connect(&session.nxi_core(), &nxi::core::event_load, [ this]
        {
            auto& cfg = session_.nxi_session().config();
            for (auto key : cfg.list())
            {
                std::visit(overloaded{
                [this](auto config_value) { add_row(config_value); }
                }, key);
            }
        });

        form_ = new QFormLayout(this);
    }
} // nxi::views