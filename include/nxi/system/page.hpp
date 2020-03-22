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
        using pages_type = std::unordered_map<nxi::page_id, nds::node_ptr<nxi::page>>;
        using pages_view = std::vector<nds::node_ptr<const nxi::page>>;
        using page_ptr = nds::node_ptr<nxi::page>;

        using page_connections_type = std::vector<ndb::objects::page_connection>;

        page_system(nxi::core&);

        void load();

        void load(nxi::page_id page);
        void load(nxi::web_page& page);

        pages_view list_root();
        pages_view pages() const;
        pages_view list(nxi::page_id);

        const page_connections_type& connections() const;
        page_ptr get(nxi::page_id id) const;

        page_ptr add_static(const QString& path, nxi::renderer_type renderer_type = nxi::renderer_type::web);
        void open_static(const QString& path, nxi::renderer_type renderer_type = nxi::renderer_type::web);

        template<class Page, class... Args>
        page_ptr add(page_ptr source, Args&&... args);

        template<class Page, class... Args>
        void open(page_ptr source, Args&&... args);

        void focus(nxi::page_id id);
        void focus(page_ptr);

        page_ptr focus() { return focus_; }

        void focus(nxi::web_page&);
        void focus(nxi::page_node&);
        void focus(nxi::custom_page&);

        void move(nxi::page_id page_id, nxi::page_id source_id, nxi::page_id target_id);

        void update(nxi::page_id id);

    signals:
        void event_add(page_ptr, page_ptr source) const;
        void event_init_static(nxi::page&);

        void event_focus(page_ptr);
        void event_load(page_ptr);

        void event_move(nxi::page_id page_id, nxi::page_id source_id, nxi::page_id target_id);
        void event_update(page_ptr);

        //void event_root_update(nds::node_ptr<>);


    public:
        nxi::core& nxi_core_;
        nds::graph<nxi::page> graph_;

        page_ptr root_;
        page_ptr focus_;

        //std::vector<page_ptr> actives_;
        std::vector<page_ptr> visible_pages_;
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