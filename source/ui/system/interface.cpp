#include <ui/system/interface.hpp>

#include <nxi/core.hpp>
#include <nxi/system/interface.hpp>

#include <ui/core.hpp>
#include <ui/utility.hpp>

namespace ui
{
    interface_system::interface_system(ui::core& ui_core)
        : ui_core_{ ui_core }
    {}

    void interface_system::add_icon(const QString& name, QPixmap icon)
    {
        // ui::make_pixmap_from_svg(name, size, foreground_color, background_color);
        icons_[name] = std::move(icon);
    }

    const QPixmap& interface_system::icon(const QString& name) { return icons_[name]; }
} // ui