#ifndef INCLUDE_NXI_MODULE_API_COMMAND_SYSTEM_HPP_NXI
#define INCLUDE_NXI_MODULE_API_COMMAND_SYSTEM_HPP_NXI

namespace nxi { class core; }

#include <nxi/module/platform.hpp>

#include <nxi/log.hpp>

#include <ui/page/web.hpp>

#include <nxi/system/command.hpp>

namespace nxi::api
{
    class command_system : public QObject
    {
        enum class event {add = 0, focus = 1};

        Q_OBJECT
    public:
        command_system(nxi::core& nxi_core)
            : nxi_core_{ nxi_core }
        {}

        NXI_SHARED void add(const std::string& name, std::function<void(const nxi::values&)> fn);

    private:
        nxi::core& nxi_core_;
    };
} // nxi::api

#endif // INCLUDE_NXI_MODULE_API_COMMAND_SYSTEM_HPP_NXI
