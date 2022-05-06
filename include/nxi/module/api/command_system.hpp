#ifndef INCLUDE_NXI_MODULE_API_COMMAND_SYSTEM_HPP_NXI
#define INCLUDE_NXI_MODULE_API_COMMAND_SYSTEM_HPP_NXI

#include <nxi/log.hpp>
#include <nxi/module/platform.hpp>
#include <nxi/system/command.hpp>

namespace nxi
{
    class core;
} // nxi

namespace nxi::api
{
    class command_system : public QObject
    {
        Q_OBJECT

        enum class event
        {
            add = 0,
            focus = 1
        };

    public:
        command_system(nxi::core& core)
            : core_{ core }
        {}
        command_system(const command_system&) = delete;
        command_system& operator=(const command_system&) = delete;

        NXI_SHARED void add(const std::string& name, std::function<void(const nxi::values&)> fn);

    private:
        nxi::core& core_;
    };
} // nxi::api

#endif // INCLUDE_NXI_MODULE_API_COMMAND_SYSTEM_HPP_NXI