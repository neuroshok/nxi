#include <nxi/user.hpp>

#include <nxi/config.hpp>
#include <nxi/core.hpp>
#include <nxi/web_session.hpp>

namespace nxi
{
    user::user(nxi::core& core, nxi::user_data data)
        : nxi_core_{ core }
        , id_{ data.id }
        , name_{ std::move(data.name) }
        , active_{ std::move(data.active) }
        , user_database_{ name_ }
        , api_{ core }
        , config_{ nullptr }
        , buffer_system_{ core }
        , session_system_{ core }
        , command_system_{ core }
        , context_system_{ core, user_database_ }
        , interface_system_{ core }
        , page_system_{ core, user_database_ }
        , window_system_{ core }
        , module_system_{ core }
        , navigation_system_{ core }
        , notification_system_{ core }
    {}

    void user::load()
    {
        user_database_.connect();
        config_ = std::make_unique<nxi::config>("nxi.user", user_database_, 0);

        window_system_.load();

        session_system_.load();
        command_system_.load();
        page_system_.load();
        context_system_.load();
        // requires window_system
        interface_system_.load();

        module_system_.load();
        navigation_system_.load();

        // requires command_system
        buffer_system_.load();
        // notification_system.load();
    }

    void user::unload() { user_database_.close(); }

    void user::error(const QString& message) const { emit event_error(message); }

    int user::id() const { return id_; }
    const QString& user::name() const { return name_; }
    bool user::is_active() const { return active_; }

    nxi::api::core& user::api() { return api_; }
    nxi::config& user::config()
    {
        nxi_assert(config_ != nullptr);
        return *config_;
    }
    nxi::database& user::database() { return user_database_; }
    nxi::core& user::nxi_core() { return nxi_core_; }

    nxi::buffer_system& user::buffer_system() { return buffer_system_; }
    nxi::command_system& user::command_system() { return command_system_; }
    nxi::context_system& user::context_system() { return context_system_; }
    nxi::interface_system& user::interface_system() { return interface_system_; }
    nxi::module_system& user::module_system() { return module_system_; }
    nxi::navigation_system& user::navigation_system() { return navigation_system_; }
    nxi::notification_system& user::notification_system() { return notification_system_; }
    nxi::page_system& user::page_system() { return page_system_; }
    nxi::session_system& user::session_system() { return session_system_; }
    nxi::window_system& user::window_system() { return window_system_; }
} // nxi