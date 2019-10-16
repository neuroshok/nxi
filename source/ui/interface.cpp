#include <ui/interface.hpp>

#include <ui/window.hpp>
#include <include/nxi/log.hpp>

namespace ui
{
    interface::interface(const QString& name, ui::window* window)
        : QWidget(window)
    {
        setObjectName("interface_" + name);
        // nxi_bug_qt_styled_background
        setAttribute(Qt::WA_StyledBackground, true);
    }

    interface::interface(ui::window* window)
         : interface("default", window)
    {}

    ui::window* interface::window() const
    {
        return static_cast<ui::window*>(QWidget::window());
    }
} // ui