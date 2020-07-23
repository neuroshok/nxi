#include <nxi/page/node.hpp>

#include <nxi/system/page.hpp>

namespace nxi
{
    page_node::page_node(nds::node_ptr<nxi::page> page_ptr, nxi::page_system& ps, nxi::page_data data)
        : nxi::page(page_ptr, ps, std::move(data))
    {}

    page_node::page_node(nds::node_ptr<nxi::page> page_ptr, nxi::page_system& ps, QString name)
        : page_node(page_ptr, ps, nxi::page_data{ 0, std::move(name), "", nxi::page_type::node, nxi::renderer_type::widget })
    {}

    void page_node::focus()
    {
        //page_system_.focus(*this);
    }

    void page_node::load()
    {
        emit event_load();
    }
} // nxi