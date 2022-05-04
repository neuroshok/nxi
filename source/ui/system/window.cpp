#include <ui/system/window.hpp>

#include <nxi/core.hpp>
#include <nxi/data/window.hpp>
#include <nxi/log.hpp>
#include <nxi/system/window.hpp>

#include <ui/core.hpp>
#include <ui/interface/standard/main.hpp>
#include <ui/system/window.hpp>
#include <ui/window.hpp>

namespace ui
{
    window_system::window_system(ui::user_session& session)
        : session_{ session }
    {
        QObject::connect(&session_.nxi_session().window_system(), &nxi::window_system::event_add, [this](const nxi::window_data& window)
        {
            nxi_trace_event("");
            add(window);
        });
    }

    window_system::~window_system()
    {
        unload();
    }

    void window_system::unload()
    {
        for (ui::window* window : m_windows)
        {
            window->deleteLater();
        }
    }

    ui::window* window_system::add(const nxi::window_data& window)
    {
        auto ui_window = new ui::window(*this, window.id);
        // make defaut interface
        ui_window->move(window.x, window.y);
        ui_window->resize(window.w, window.h);
        ui_window->show();

        auto ui_interface = session_.make_main_interface(ui_window);
        ui_window->set_interface(ui_interface);

        m_windows.push_back(ui_window);
        return ui_window;
    }

    void window_system::close(ui::window* window)
    {
        if (count() == 1) session_.ui_core().quit();
        else
        {
            session_.nxi_session().window_system().del(window->id());
        }
        m_windows.erase(std::remove(m_windows.begin(), m_windows.end(), window), m_windows.end());
    }

    void window_system::move(ui::window* window, int x, int y)
    {
        session_.nxi_session().window_system().move(window->id(), x, y);
    }

    void window_system::resize(ui::window* window, int w, int h)
    {
        session_.nxi_session().window_system().resize(window->id(), w, h);
    }

    void window_system::minimize(ui::window* window)
    {
        session_.nxi_session().window_system().minimize(window->id());
        window->showMinimized();
    }

    size_t window_system::count() const
    {
        return m_windows.size();
    }
} // ui