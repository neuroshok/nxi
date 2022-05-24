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
    window_system::window_system(ui::user_session& session)
        : session_{ session }
    {
        QObject::connect(&session_.nxi_session().window_system(), &nxi::window_system::event_add, [this](nxi::window& window) {
            nxi_trace_event("");
            add(window);
        });
    }

    window_system::~window_system() { unload(); }

    void window_system::unload()
    {
        for (ui::window* window : windows_)
        {
            window->deleteLater();
        }
    }

    ui::window* window_system::add(nxi::window& window)
    {
        auto ui_window = new ui::window(*this, window);
        // make defaut interface
        ui_window->move(window.x(), window.y());
        ui_window->resize(window.width(), window.height());
        ui_window->show();

        auto ui_interface = session_.make_main_interface(ui_window);
        ui_window->set_interface(ui_interface);

        windows_.push_back(ui_window);
        return ui_window;
    }

    void window_system::close(ui::window* window)
    {
        if (count() == 1) session_.ui_core().quit();
        else
        {
            session_.nxi_session().window_system().del(window->id());
        }
        windows_.erase(std::remove(windows_.begin(), windows_.end(), window), windows_.end());
    }

    void window_system::move(ui::window* window, int x, int y) { session_.nxi_session().window_system().move(window->id(), x, y); }

    void window_system::resize(ui::window* window, int w, int h) { session_.nxi_session().window_system().resize(window->id(), w, h); }

    void window_system::minimize(ui::window* window)
    {
        session_.nxi_session().window_system().minimize(window->id());
        window->showMinimized();
    }

    size_t window_system::count() const { return windows_.size(); }
    std::vector<ui::window*> window_system::windows() { return windows_; }
} // ui