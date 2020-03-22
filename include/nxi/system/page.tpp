
namespace nxi
{
    template<class Page, class... Args>
    page_system::page_ptr page_system::add(page_system::page_ptr source, Args&&... args)
    {
        auto page = graph_.emplace<nxi::page, Page>(root_, *this, std::forward<Args>(args)...);
        ndb::store(*page);

        emit event_add(page, source);
        return page;
    }

    template<class Page, class... Args>
    void page_system::open(page_system::page_ptr source, Args&&... args)
    {
        auto added_page = add<Page>(source, std::forward<Args>(args)...);
        added_page->load();
        focus(added_page);
    }
} // nxi