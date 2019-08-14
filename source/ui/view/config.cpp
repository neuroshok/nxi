#include <ui/view/config.hpp>

#include <QLineEdit>
#include <QLabel>
#include <QFormLayout>

#include <ui/core.hpp>
#include <nxi/core.hpp>
#include <nxi/config.hpp>

namespace ui::views
{
    config::config(ui::core& ui_core)
        : ui_core_{ ui_core }
    {
        auto& cfg = ui_core_.nxi_core().config();

        form_ = new QFormLayout(this);

        for (auto& key : cfg.list())
        {
            std::visit(overloaded{
            [this](auto config_key) { add_row(*config_key); }
            }, key);
        }
    }
} // nxi::views