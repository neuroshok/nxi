#include <ui/interface/main.hpp>

#include <ui/window.hpp>

namespace ui
{
    main_interface::main_interface(ui::window* window)
        : ui::interface(window)
        , fullmode_{ false }
    {}

    bool main_interface::toggle_fullmode(int states)
    {
        ++fullmode_;
        fullmode_ %= states; // 0 = normal, window = 1, screen = 2
        return fullmode_;
    }

    int main_interface::fullmode() const { return fullmode_; }
} // nxi