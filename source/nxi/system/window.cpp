#include <nxi/system/window.hpp>

#include <nxi/core.hpp>
#include <nxi/data/window.hpp>
#include <nxi/database/model.hpp>
#include <nxi/log.hpp>
#include <nxi/user.hpp>
#include <nxi/window.hpp>

#include <QGuiApplication>
#include <QRect>
#include <QScreen>

namespace nxi
{
    window_system::window_system(nxi::core& core)
        : core_{ core }
    {}

    void window_system::load()
    {
        nxi_trace("");

        // load stored windows
        auto result = nxi::data::window::get_windows(core_.user_database());
        while (result.next())
        {
            nxi::window_data window;
            window.id = result[nxi_model.window.id];
            window.x = result[nxi_model.window.x];
            window.y = result[nxi_model.window.y];
            window.w = result[nxi_model.window.w];
            window.h = result[nxi_model.window.h];

            internal_add(window);
        }

        if (windows_.empty())
        {
            QRect screen_size = QGuiApplication::primaryScreen()->geometry();

            nxi::window_data window;
            window.w = screen_size.width() * 0.8;
            window.h = screen_size.height() * 0.7;
            window.x = (screen_size.width() - window.w) / 2;
            window.y = (screen_size.height() - window.h) / 2;

            add(window);
        }
    }

    void window_system::add(nxi::window_data window)
    {
        window.id = nxi::data::window::add_window(core_.user_database(), window);
        internal_add(window);
    }

    void window_system::del(int id)
    {
        // nxi::queries::del_window(id);
    }

    void window_system::move(unsigned int id, int x, int y)
    {
        nxi::data::window::move_window(core_.user_database(), id, x, y);
        emit event_position_update(x, y);
    }

    void window_system::resize(unsigned int id, int w, int h)
    {
        //// ndb::query<dbs::core>() << // ndb::set(nxi_model.window.w = w, nxi_model.window.h = h);
    }

    std::unordered_map<unsigned int, nxi::window*>& window_system::windows() { return windows_; }

    void window_system::minimize(unsigned int id)
    {
        // windows_[id].state = window_states::minimized;
        emit event_state_update(id, window_states::minimized);
    }

    void window_system::internal_add(nxi::window_data window_data)
    {
        // create a command buffer for the window
        // core_.user().command_system().add_buffer(window.id);

        auto window = new nxi::window{ core_, window_data };
        emit event_add(*window);
        windows_.emplace(window->id(), window);
    }
} // nxi