#include <ui/view/config.hpp>

#include <QLineEdit>
#include <QLabel>
#include <QFormLayout>

#include <ui/core.hpp>
#include <nxi/core.hpp>
#include <nxi/config.hpp>

namespace ui::views
{
    config::config(ui::session& session)
        : session_{ session }
    {
        //auto& cfg = session_.nxi_session().config();

        form_ = new QFormLayout(this);

        /*
        for (auto& key : cfg.list())
        {
            std::visit(overloaded{
            [this](auto config_key) { add_row(*config_key); }
            }, key);
        }*/
    }
} // nxi::views