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
        web_page(nds::node_ptr<nxi::page>, nxi::page_system& ps);
        web_page(nds::node_ptr<nxi::page> ptr, nxi::page_system& ps, const QString& url);

    private:


    signals:
        //void event_load_begin();
        //void event_load_end();
    };
} // nxi

#endif // NXI_PAGE_WEB_H_NXI
