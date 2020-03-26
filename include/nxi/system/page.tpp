#include <nxi/database.hpp>

namespace nxi
{
    template<class Page, class... Args>
    page_system::page_ptr page_system::add(page_system::page_ptr source, Args&&... args)
    {
        auto page = graph_.emplace<nxi::page, Page>(source, *this, std::forward<Args>(args)...);
        ndb::store(*page);

        graph_.connect(source, page);
        ndb::query<dbs::core>() << ndb::add( nxi_model.page_connection.source_id = source->id(), nxi_model.page_connection.target_id = page->id() );

        emit event_add(page, source);
        return page;
    }

    template<class Page, class... Args>
    page_system::page_ptr page_system::add(Args&&... args)
    {
        return add<Page>(root_, std::forward<Args>(args)...);
    }

    template<class Page, class... Args>
    void page_system::open(page_system::page_ptr source, Args&&... args)
    {
        auto added_page = add<Page>(source, std::forward<Args>(args)...);
        added_page->load();
        focus(added_page);
    }

    template<class Page, class... Args>
    void page_system::open(Args&&... args)
    {
        open<Page>(root_, std::forward<Args>(args)...);
    }
} // nxi