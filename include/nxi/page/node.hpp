#ifndef NXI_PAGE_NODE_H_NXI
#define NXI_PAGE_NODE_H_NXI

#include <nxi/page.hpp>


namespace nxi
{
    class page_system;

    class page_node : public nxi::page
    {
    public:
        page_node(nxi::page_system& ps) : nxi::page(ps, "new node") {}
        page_node(nxi::page_system& ps, const QString& name) : nxi::page(ps, name) {}

        void focus();

    private:
    };
} // nxi

#endif // NXI_PAGE_NODE_H_NXI