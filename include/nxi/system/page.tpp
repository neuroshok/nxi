
namespace nxi
{
    template<class Page, class... Args>
    Page& page_system::add(nxi::page_id source_id, Args&&... args)
    {
        auto graph_node_page = graph_.emplace<nxi::page, Page>(root_, *this, std::forward<Args>(args)...);
        ndb::store(*graph_node_page);

        auto& added_page = static_cast<nxi::page&>(*graph_node_page);
        //pages_.emplace(added_page.id(), &added_page);

        emit event_add(added_page, 0);
        return static_cast<Page&>(*graph_node_page);
    }

    template<class Page, class... Args>
    void page_system::open(nxi::page_id source_id, Args&&... args)
    {
        Page& added_page = add<Page>(source_id, std::forward<Args>(args)...);
        added_page.load();
        added_page.focus();
    }
} // nxi