
namespace nxi
{
    template<class Page, class... Args>
    void page_system::add(nxi::page_id source_id, Args&&... args)
    {
        auto page = graph_.emplace<nxi::page, Page>( *this, std::forward<Args>(args)...);
        ndb::store(page->get());
        qDebug() << "______" << page->get().id();

        emit event_add(static_cast<nxi::page&>(page->get()), 0);
    }
} // nxi