
namespace nxi
{
    template<class Page, class... Args>
    Page& page_system::add(nxi::page_id source_id, Args&&... args)
    {
        auto graph_node_page = graph_.emplace<nxi::page, Page>(*this, std::forward<Args>(args)...);
        ndb::store(graph_node_page->get());

        auto& added_page = static_cast<nxi::page&>(graph_node_page->get());
        emit event_add(added_page, 0);
        return graph_node_page->get();
    }

    template<class Page, class... Args>
    void page_system::open(nxi::page_id source_id, Args&&... args)
    {
        Page& added_page = add<Page>(source_id, std::forward<Args>(args)...);
        added_page.load();
    }
} // nxi