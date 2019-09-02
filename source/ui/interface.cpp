#include <ui/interface.hpp>

#include <ui/window.hpp>
#include <include/nxi/log.hpp>

namespace ui
{
    interface::interface(const QString& name, ui::window* window)
        : QWidget(window)
        , fullmode_{ false }
    {
        setObjectName("interface_" + name);
        // nxi_bug_qt_styled_background
        setAttribute(Qt::WA_StyledBackground, true);
    }

    interface::interface(const QString& name)
         : interface(name, nullptr)
    {}

    ui::window* interface::window() const
    {
        return static_cast<ui::window*>(QWidget::window());
    }

    void interface::toggle_fullmode()
    {
        fullmode_ = !fullmode_;
    }

    bool interface::fullmode() const
    {
        return fullmode_;
    }

} // ui