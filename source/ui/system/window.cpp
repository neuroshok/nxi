#include <ui/system/window.hpp>

#include <nxi/core.hpp>
#include <nxi/data/window.hpp>
#include <nxi/log.hpp>
#include <nxi/system/window.hpp>
#include <nxi/user.hpp>
#include <nxi/window.hpp>

#include <ui/core.hpp>
#include <ui/interface/standard/main.hpp>
#include <ui/system/window.hpp>
#include <ui/window.hpp>

namespace ui
{
    window_system::window_system(ui::user& user)
        : user_{ user }
    {
        connect(&user_.nxi_user().window_system(), &nxi::window_system::event_add, this, [this](nxi::window& window) {
            nxi_trace_event("");
            add(window);
        });

        connect(&user_.nxi_user().window_system(), &nxi::window_system::event_close, this, [this](int window_id) { close(window_id); });
    }

    window_system::~window_system() { unload(); }

    void window_system::unload()
    { windows_.clear(); }

    ui::window* window_system::add(nxi::window& window)
    {
        auto ui_window = std::make_unique<ui::window>(user_, window);
        // make default interface
        ui_window->move(window.x(), window.y());
        ui_window->resize(window.width(), window.height());
        ui_window->show();

        auto ui_interface = user_.make_main_interface(ui_window.get());
        ui_window->set_interface(ui_interface);

        windows_.emplace_back(std::move(ui_window));

        return windows_.back().get();
    }

    void window_system::close(int id)
    {
        auto window_it = std::find_if(windows_.begin(), windows_.end(), [id](const auto& window) { return window->id() == id; });
        nxi_assert(window_it != windows_.end());
        windows_.erase(std::remove(windows_.begin(), windows_.end(), *window_it), windows_.end());
        //(*window_it)->deleteLater();
    }

    void window_system::move(ui::window* window, int x, int y) { user_.nxi_user().window_system().move(window->id(), x, y); }

    void window_system::resize(ui::window* window, int w, int h) { user_.nxi_user().window_system().resize(window->id(), w, h); }

    void window_system::minimize(ui::window* window)
    {
        user_.nxi_user().window_system().minimize(window->id());
        window->showMinimized();
    }

    size_t window_system::count() const { return windows_.size(); }

    std::vector<std::unique_ptr<ui::window>>& window_system::windows() { return windows_; }
} // ui