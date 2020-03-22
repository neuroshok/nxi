#ifndef NXI_PAGE_NODE_H_NXI
#define NXI_PAGE_NODE_H_NXI

#include <nxi/page.hpp>


namespace nxi
{
    class page_system;

    class page_node : public nxi::page
    {
    public:
        page_node(nxi::page_system& ps, const QString& name) : nxi::page(ps, name, "", nxi::page_type::node, nxi::renderer_type::widget) {}
        page_node(nxi::page_system& ps) : page_node(ps, "node") {}

        void focus();
        void load();

    private:
    };
} // nxi

#endif // NXI_PAGE_NODE_H_NXI