#ifndef NXI_MODULE_SYSTEM_H_NXI
#define NXI_MODULE_SYSTEM_H_NXI

#include <nxi/module.hpp>

#include <QObject>
#include <QString>
#include <nxi/module/static.generated.hpp>

namespace nxi
{
	class core;
	class web_page;

    class module_system : public QObject
    {
        Q_OBJECT
    public:
		module_system(nxi::core&);
		module_system(const module_system&) = delete;
        void operator=(const module_system&) = delete;

        const std::vector<std::unique_ptr<nxi::module>>& get() const;
        void load();

        void process(nxi::web_page&);

        const QString& js_api() const;

    private:
		nxi::core& nxi_core_;
        std::vector<std::unique_ptr<nxi::module>> modules_;
        nxi::static_module_container static_modules_;
        QString js_api_;
    };
} // nxi

#endif // NXI_MODULE_SYSTEM_H_NXI