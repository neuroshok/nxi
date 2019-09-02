#include <ui/interface.hpp>

#include <ui/window.hpp>
#include <include/nxi/log.hpp>

namespace ui
{
    interface::interface(ui::window* window)
        : fullmode_{ false }
        , QWidget(window)
    {}

    interface::interface()
         : interface(nullptr)
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