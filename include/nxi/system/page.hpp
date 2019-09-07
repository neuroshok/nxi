#ifndef NXI_PAGE_SYSTEM_H_NXI
#define NXI_PAGE_SYSTEM_H_NXI

#include <nxi/page.hpp>
#include <nxi/page/id.hpp>

#include <QObject>
#include <unordered_map>
#include <stz/observer_ptr.hpp>
#include <optional>
#include <nxi/log.hpp>

#include <nxi/page/web.hpp>

#include <nxi/database.hpp>

#include <nds/graph.hpp>

namespace nxi
{
    class core;
    class page;
    class page_node;
    class web_page;
    class explorer_page;
    class custom_page;

    class page_system : public QObject
    {
        Q_OBJECT
    public:
        using pages_type = std::unordered_map<nxi::page_id, std::unique_ptr<nxi::page>>;
        using pages_view = std::vector<stz::observer_ptr<nxi::page>>;

        using page_connections_type = std::vector<ndb::objects::page_connection>;

        page_system(nxi::core&);

        void load();

        void load(nxi::page_id page);
        void load(nxi::web_page& page);

        pages_view list(const nxi::page& source);

        const pages_view& get() const;
        const page_connections_type& connections() const;
        nxi::page& get(nxi::page_id id) const;

        nxi::page& add_static(const QString& path, nxi::renderer_type renderer_type = nxi::renderer_type::web);
        void open_static(const QString& path, nxi::renderer_type renderer_type = nxi::renderer_type::web);

        template<class Page, class... Args>
        Page& add(nxi::page_id source_id, Args&&... args);

        template<class Page, class... Args>
        void open(nxi::page_id source_id, Args&&... args);

        void focus(nxi::page_id id);

        std::optional<stz::observer_ptr<nxi::page>> focus() { return focus_; }

        template<class Page>
        void focus(nxi::page_id id)
        {
            nxi_trace_event("{}", id);
            emit event_focus(static_cast<Page&>(get(id)));
            emit event_focus(get(id));
        }

        void focus(nxi::web_page&);
        void focus(nxi::page_node&);
        void focus(nxi::custom_page&);

        void move(nxi::page_id page_id, nxi::page_id source_id, nxi::page_id target_id);

        void update(nxi::page_id id);

    signals:
        void event_add(nxi::page&, nxi::page_id source_id);
        void event_init_static(nxi::page&);

        void event_focus(nxi::page&);
        void event_focus(nxi::page_node&);
        void event_focus(nxi::web_page&);
        void event_focus(nxi::explorer_page&);
        void event_focus(nxi::custom_page&);

        void event_load(nxi::web_page&);

        void event_move(nxi::page_id page_id, nxi::page_id source_id, nxi::page_id target_id);

        void event_update(const nxi::page&);
        void event_update(const nxi::web_page&);


    public:
        nxi::core& nxi_core_;
        nds::graph<nxi::page> graph_;
        //nxi::com::connect(source, target)
        //nxi::com::send(page_system::add)

        nxi::page* current_page_;
        std::optional<stz::observer_ptr<nxi::page>> focus_;
        std::vector<nxi::page*> visible_pages_;

        pages_type pages_;
        pages_view pages_view_;

        page_connections_type page_connections_;
    };
} // nxi

#include <nxi/system/page.tpp>

#endif // NXI_PAGE_SYSTEM_H_NXI
/* add / delete
 * load / unload
 * update
 * change
 * show / hide
 *
 */