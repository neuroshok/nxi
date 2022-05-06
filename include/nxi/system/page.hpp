#ifndef NXI_PAGE_SYSTEM_H_NXI
#define NXI_PAGE_SYSTEM_H_NXI

#include <nds/graph.hpp>
#include <nxi/type.hpp>

#include <functional>
#include <vector>
#include <unordered_map>

#include <QObject>

namespace nxi
{
    class core;
    class custom_page;
    class database;
    class explorer_page;
    class page;
    class page_node;
    class web_page;

    class page_system : public QObject
    {
        Q_OBJECT
    public:
        using pages_type = std::unordered_map<nxi::page_id, nds::node_ptr<nxi::page>>;
        using pages_view = std::vector<nds::node_ptr<nxi::page>>;
        using page_ptr = nds::node_ptr<nxi::page>;

    public:
        page_system(nxi::core&, nxi::database&);
        page_system(const page_system&) = delete;
        page_system& operator=(const page_system&) = delete;


        void load();

        page_ptr add_static(const QString& path, nxi::renderer_type renderer_type = nxi::renderer_type::web);
        void open_static(const QString& path, nxi::renderer_type renderer_type = nxi::renderer_type::web);

        template<class Page, class... Args>
        page_ptr add(page_ptr source, Args&&... args);
        template<class Page, class... Args>
        page_ptr add(Args&&... args);
        template<class Page, class... Args>
        void open(page_ptr source, Args&&... args);
        template<class Page, class... Args>
        void open(Args&&... args);

        void close(nds::node_ptr<nxi::page>);
        void close_focus();

        void erase(nds::node_ptr<nxi::page>);
        void focus(nxi::page_id id);
        void focus(nds::node_ptr<nxi::page>);
        const page_ptr& focus() const;
        page_ptr get(nxi::page_id id) const;
        void load(nxi::page_id page);
        void load(page_ptr);

        void set_root(nds::node_ptr<nxi::page>);

        pages_view pages() const;
        pages_view root_targets() const;
        void search(const QString&, std::function<void(nds::node_ptr<nxi::page>)>) const;
        pages_view targets(nds::node_ptr<const nxi::page> source) const;
        pages_view targets(nxi::page_id source) const;

        void move(nxi::page_id page_id, nxi::page_id source_id, nxi::page_id target_id);
        void update(nxi::page_id id);

        int session_id() const;

    signals:
        void event_add(page_ptr, page_ptr source) const;
        void event_init_static(nxi::page&);

        void event_close(page_ptr);
        void event_focus(page_ptr);
        void event_load(page_ptr);

        void event_move(nxi::page_id page_id, nxi::page_id source_id, nxi::page_id target_id);
        void event_update(page_ptr);
        void event_update_command(const nxi::page& source, const QString& target);

        void event_update_root(page_ptr);

    public:
        nxi::core& core_;
        nxi::database& user_database_;
        nds::graph<nxi::page> graph_;

        page_ptr focus_;
        page_ptr root_;

        int session_id_ = 0;
        // std::vector<page_ptr> actives_;
        std::vector<page_ptr> visible_pages_;
    };

    template<>
    void page_system::open<nxi::web_page>();
} // nxi

#include <nxi/system/page.tpp>

#endif // NXI_PAGE_SYSTEM_H_NXI