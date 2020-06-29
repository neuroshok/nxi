#ifndef INCLUDE_NXI_MODULE_API_CORE_HPP_NXI
#define INCLUDE_NXI_MODULE_API_CORE_HPP_NXI

namespace nxi { class core; }
namespace ui { class web_page; }

#include <nxi/module/platform.hpp>
#include <QObject>
#include <nxi/module/api/command_system.hpp>
#include <nxi/module/api/page_system.hpp>

namespace nxi::api
{
    class core : public QObject
    {
        Q_OBJECT
    public:
        core(nxi::session& session)
            : session_{ session }
            , command_system_{ session }
            , page_system_{ session }
        {}

        NXI_SHARED nxi::api::command_system& command_system();
        NXI_SHARED nxi::api::page_system& page_system();
        NXI_SHARED void error(const std::string& message);

    public slots:
        // js_api
        void log(const QString& message);

    private:
        nxi::session& session_;

        nxi::api::command_system command_system_;
        nxi::api::page_system page_system_;
    };
} // nxi::api

#endif // INCLUDE_NXI_MODULE_API_CORE_HPP_NXI
