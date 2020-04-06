#ifndef INCLUDE_NXI_MODULE_API_JS_HPP_NXI
#define INCLUDE_NXI_MODULE_API_JS_HPP_NXI

#include <nxi/page.hpp>

#include <QJsonDocument>
#include <QString>

namespace nxi::api
{
    template<class Arg, class... Args>
    void js_call(nxi::page& page, const QString& function, const Arg& arg, const Args&... args)
    {
        QString str_args;
        if constexpr(sizeof...(Args) > 0) str_args = (... + ("," + QJsonDocument{ args }.toJson(QJsonDocument::Compact)));
        QString script = "(" + function + ")(" + QJsonDocument{ arg }.toJson(QJsonDocument::Compact) + str_args + ");";
        page.run_script(std::move(script));
    }
} // nxi::api

#endif // INCLUDE_NXI_MODULE_API_JS_HPP_NXI
