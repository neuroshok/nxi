#ifndef INCLUDE_NXI_MODULE_JS_API_CORE_HPP_NXI
#define INCLUDE_NXI_MODULE_JS_API_CORE_HPP_NXI

#include <nxi/core.hpp>

namespace ui { class web_page; }

namespace nxi::js_api
{
    class core : public QObject
    {
        Q_OBJECT
    public:
        core(nxi::core& nxi_core, ui::web_page& ui_page)
            : nxi_core_{ nxi_core }
            , ui_page_{ ui_page }
        {}

    public slots:
        void log(const QString& message)
        {
            nxi_core_.error(message);
        };

    private:
        nxi::core& nxi_core_;
        ui::web_page& ui_page_;
    };
} // nxi::js_api

#endif // INCLUDE_NXI_MODULE_JS_API_CORE_HPP_NXI
