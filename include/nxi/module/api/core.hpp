#ifndef INCLUDE_NXI_MODULE_API_CORE_HPP_NXI
#define INCLUDE_NXI_MODULE_API_CORE_HPP_NXI

#include <nxi/module/api/command_system.hpp>
#include <nxi/module/api/page_system.hpp>
#include <nxi/module/platform.hpp>
#include <QObject>

namespace nxi
{
    class core;
} // nxi
namespace ui
{
    class web_page;
} // ui

namespace nxi::api
{
    class core : public QObject
    {
        Q_OBJECT
    public:
        core(nxi::core& core)
            : core_{ core }
            , command_system_{ core }
            , page_system_{ core }
        {}
        core(const core&) = delete;
        core& operator=(const core&) = delete;

        NXI_SHARED nxi::api::command_system& command_system();
        NXI_SHARED nxi::api::page_system& page_system();
        NXI_SHARED void error(const std::string& message);

    public slots:
        // js_api
        void log(const QString& message);

    private:
        nxi::core& core_;

        nxi::api::command_system command_system_;
        nxi::api::page_system page_system_;
    };
} // nxi::api

extern "C"
{
    NXI_EXPORT void nxi_core_error(nxi::core*, const char* message);
    NXI_EXPORT void nxi_page_system_open(nxi::core*);
};

#endif // INCLUDE_NXI_MODULE_API_CORE_HPP_NXI