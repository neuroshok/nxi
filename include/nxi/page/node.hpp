#ifndef NXI_PAGE_NODE_H_NXI
#define NXI_PAGE_NODE_H_NXI

#include <nxi/page.hpp>


namespace nxi
{
    class page_system;

    class page_node : public nxi::page
    {
    public:
        page_node(nds::node_ptr<nxi::page>, nxi::page_system& ps, nxi::page_data);
        page_node(nds::node_ptr<nxi::page>, nxi::page_system& ps, QString name);

        void focus();
        void load();

    private:
    };
} // nxi

#endif // NXI_PAGE_NODE_H_NXI