#include <nxi/core.hpp>

#include <include/nxi/database/config.hpp>
#include <nxi/database.hpp>
#include <nxi/log.hpp>

namespace nxi
{
    core::core()
        : ndb_init_{}
        , command_system_{ *this }
        , window_system_{ *this }
        , page_system_{ *this }
        , module_system_{ *this }
        , config_{ "nxi" }
    {}

    core::~core()
    {
    }

    void core::load()
    {
        // load modules before other systems
        module_system_.load();

        command_system_.load();
        window_system_.load();
        page_system_.load();

        config_.browser.download_path = "d:/";
        config_.browser.home = "http://youtube.com";
    }

    void core::quit() const
    {
        emit event_quit();
    }

    nxi::command_system& core::command_system()
    {
        return command_system_;
    }

    nxi::module_system& core::module_system()
    {
        return module_system_;
    }

    nxi::page_system& core::page_system()
    {
        return page_system_;
    }

    nxi::window_system& core::window_system()
    {
        return window_system_;
    }
} // nxi
