#include <nxi/module/api/core.hpp>

#include <nxi/core.hpp>

#include <QString>
#include <string>

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