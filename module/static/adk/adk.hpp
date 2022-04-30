#ifndef MODULE_STATIC_ADK_ADK_HPP_NXI
#define MODULE_STATIC_ADK_ADK_HPP_NXI

#include <nxi/module.hpp>

#include <QWebEngineUrlRequestInterceptor>
#include <QWebEngineUrlRequestInfo>

namespace nxi { class session; }

struct interceptor;

namespace nxi::modules
{
    class adk : public nxi::module
    {
    public:
        explicit adk(nxi::session&);

        void on_load() override;
        void on_unload() override;

    private:
        nxi::session& session_;
        ::interceptor* interceptor_;
    };
} // nxi::modules

#endif // MODULE_STATIC_ADK_ADK_HPP_NXI