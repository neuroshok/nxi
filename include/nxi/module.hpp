#ifndef NXI_MODULE_H_NXI
#define NXI_MODULE_H_NXI

#include <nxi/database.hpp>
#include <nxi/type.hpp>

#include <QString>

namespace nxi
{
    class web_page;

    class module
    {
    public:
        module(QString name, module_type);

        void load();
        void unload();

        virtual void on_load();
        virtual void on_unload();
        virtual void process(nxi::web_page&);

        const QString& name() const;
        module_type type() const;
        bool is_loaded() const;

    private:
        QString name_;
        module_type type_;
        bool loaded_;
    };
} // nxi

#endif // NXI_MODULE_H_NXI