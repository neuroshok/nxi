#include <nxi/module/api/core.hpp>

#include <nxi/core.hpp>

#include <QString>
#include <string>

void nxi_core_error(nxi::core* core, const char* message)
{
    core->session_system().focus()->error(message);
}

void nxi_page_system_open(nxi::core* core)
{
    core->session_system().focus()->page_system().open<nxi::web_page>();
}


namespace nxi::api
{
    api::command_system& core::command_system() { return command_system_; }
    api::page_system& core::page_system() { return page_system_; }

    void core::log(const QString& message)
    {
        session_.error(message);
    };

    void core::error(const std::string& message)
    {
        session_.error(QString::fromStdString(message));
    };


} // nxi::api