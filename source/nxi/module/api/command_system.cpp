#include <nxi/module/api/command_system.hpp>

#include <nxi/system/command.hpp>
#include <nxi/user_session.hpp>

namespace nxi::api
{
    void command_system::add(const std::string& name, std::function<void(const nxi::values&)> fn)
    {
        auto cmd = nxi::command(QString::fromStdString(name), "main", fn);

        session_.command_system().add(std::move(cmd));
    }
} // nxi::api