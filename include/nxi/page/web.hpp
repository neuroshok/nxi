#ifndef NXI_PAGE_WEB_H_NXI
#define NXI_PAGE_WEB_H_NXI

#include <nxi/page.hpp>

namespace nxi
{
    class page_system;

    class web_page : public nxi::page
    {
        Q_OBJECT
    public:
        web_page(nxi::page_system& ps);
        web_page(nxi::page_system& ps, const QString& name) : nxi::page(ps, name) {}

        void focus();
        void load();


    private:


    signals:
        //void event_load_begin();
        //void event_load_end();
    };
} // nxi

#endif // NXI_PAGE_WEB_H_NXI
