#ifndef NXI_MODULE_SYSTEM_H_NXI
#define NXI_MODULE_SYSTEM_H_NXI

#include <nxi/module.hpp>
#include <nxi/module/static.generated.hpp>

#include <memory>

#include <QObject>
#include <QString>

class QWebChannel;

namespace nxi
{
    class user_session;
    class web_page;

    class module_system : public QObject
    {
        Q_OBJECT
    public:
        module_system(nxi::user_session&);
        module_system(const module_system&) = delete;
        void operator=(const module_system&) = delete;

        const std::vector<std::unique_ptr<nxi::module>>& get() const;
        void load();

        void process(nxi::web_page&);

        const QString& js_api() const;
        QWebChannel* web_channel() const;

    private:
        nxi::user_session& session_;
        std::vector<std::unique_ptr<nxi::module>> modules_;
        nxi::static_module_container static_modules_;
        QString js_api_;
        QWebChannel* web_channel_;
    };
} // nxi

#endif // NXI_MODULE_SYSTEM_H_NXI