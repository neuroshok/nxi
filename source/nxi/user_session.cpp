#include <nxi/user_session.hpp>

#include <nxi/config.hpp>
#include <nxi/core.hpp>
#include <nxi/web_session.hpp>

namespace nxi
{
    user_session::user_session(nxi::core& core, nxi::user_data data)
        : nxi_core_{ core }
        , id_{ data.id }
        , name_{ std::move(data.name) }
        , active_{ std::move(data.active) }
        , core_database_{ name_ }
        , api_{ *this }
        , config_{ nullptr }
        , command_system_{ *this }
        , context_system_{ *this }
        , interface_system_{ *this }
        , page_system_{ *this, core_database_ }
        , window_system_{ *this }
        , module_system_{ *this }
        , navigation_system_{ *this }
        , web_session_{ new nxi::web_session{ *this, this } }
    {}

    void user_session::load()
    {
        core_database_.connect();
        config_ = std::make_unique<nxi::config>("nxi.user", core_database_);

        window_system_.load(); // load before interface_system

        command_system_.load();
        context_system_.load();
        interface_system_.load();
        page_system_.load();
        module_system_.load();
        navigation_system_.load();

        web_session_->load();
    }

    void user_session::unload()
    {
        core_database_.close();
    }

    void user_session::error(const QString& message) const
    {
        emit event_error(message);
    }

    int user_session::id() const { return id_; }
    const QString& user_session::name() const { return name_; }
    bool user_session::is_active() const { return active_; }

    nxi::api::core& user_session::api() { return api_; }
    nxi::config& user_session::config()
    {
        nxi_assert(config_ != nullptr);
        return *config_;
    }
    nxi::database& user_session::database() { return core_database_; }
    nxi::core& user_session::nxi_core() { return nxi_core_; }
    nxi::web_session& user_session::web_session() { return *web_session_; }

    nxi::command_system& user_session::command_system() { return command_system_; }
    nxi::context_system& user_session::context_system() { return context_system_; }
    nxi::interface_system& user_session::interface_system() { return interface_system_; }
    nxi::module_system& user_session::module_system() { return module_system_; }
    nxi::navigation_system& user_session::navigation_system() { return navigation_system_; }
    nxi::page_system& user_session::page_system() { return page_system_; }
    nxi::window_system& user_session::window_system() { return window_system_; }
} // nxi