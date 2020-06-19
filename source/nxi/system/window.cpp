#include <nxi/system/window.hpp>

#include <nxi/core.hpp>
#include <nxi/database/queries/window.hpp>
#include <nxi/log.hpp>

#include <QGuiApplication>
#include <QRect>
#include <QScreen>

namespace nxi
{
    window_system::window_system(nxi::core& nxi_core) :
            nxi_core_{ nxi_core }
    {
        //nxi_log << "init window_system";
    }

    void window_system::load()
    {
        nxi_trace("");

        // load stored windows
        /*
        for (auto& window :  ndb::oget<dbs::core>(nxi_model.window))
        {
            windows_.emplace(window.id, std::move(window));
            emit event_add(window);
        }*/

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

    void window_system::add(nxi::window_data win)
    {
        //nxi::queries::add_window(win.x, win.y, win.w, win.h);

        //win.id = // ndb::last_id<dbs::core>();

        windows_.emplace(win.id, std::move(win));

        emit event_add((win));
    }

    void window_system::del(int id)
    {
        //nxi::queries::del_window(id);
    }

    void window_system::move(unsigned int id, int x, int y)
    {
        //nxi::queries::move_window(nxi_core_.session_id(), id, x, y);
        emit event_position_update(x, y);
    }

    void window_system::resize(unsigned int id, int w, int h)
    {
        //// ndb::query<dbs::core>() << // ndb::set(nxi_model.window.w = w, nxi_model.window.h = h);
    }

    std::map<unsigned int, nxi::window_data>& window_system::get()
    {
        return windows_;
    }

    void window_system::minimize(unsigned int id)
    {
	    // windows_[id].state = window_states::minimized;
        emit event_state_update(id, window_states::minimized);
    }
} // nxi