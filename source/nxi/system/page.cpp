#include <nxi/system/page.hpp>

#include <nxi/config.hpp>
#include <nxi/core.hpp>
#include <nxi/data/page.hpp>
#include <nxi/database.hpp>
#include <nxi/log.hpp>
#include <nxi/page/custom.hpp>
#include <nxi/page/node.hpp>
#include <nxi/page/web.hpp>
#include <nxi/system/session.hpp>

namespace nxi
{
    page_system::page_system(nxi::core& core, nxi::database& user_database)
        : core_{ core }
        , user_database_{ user_database }
        , root_{ nullptr }
    {}

    void page_system::load()
    {
        nxi_trace("");

        // first exec
        auto page_count = nxi::data::page::count(user_database_);

        if (page_count == 0)
        {
            auto main = graph_.emplace<nxi::page, nxi::page_node>(*this, "page");
            auto page_id = nxi::data::page::add(user_database_, *main);
            set_root(main);
            core_.session().config().page.root = main->id();
        }

        // load pages, connect all pages to root
        auto result = nxi::data::page::get(user_database_);
        nds::node_ptr<nxi::page> page_root;
        while (result.next())
        {
            auto page_data = nxi::data::page::from_get(result);
            auto page_type = page_data.type;

            switch (page_type)
            {
            case page_type::node: {
                auto page = graph_.emplace<nxi::page, nxi::page_node>(*this, std::move(page_data));
                emit event_add(page, nullptr);
                emit page->event_load();
                emit event_load(page);
                page_root = page;
                break;
            }

            case page_type::web: {
                auto page = graph_.emplace<nxi::page, nxi::web_page>(*this, std::move(page_data));
                emit event_add(page, nullptr);
                if (page->is_loaded())
                {
                    emit page->event_load();
                    emit event_load(page);
                    // todo : use navigation system
                    // focus(page);
                }
                break;
            }

            case page_type::explorer:
                // page = std::make_unique<nxi::explorer_page>(*this, page_id);
                break;

            case page_type::static_: {
                auto page = graph_.emplace<nxi::page, nxi::custom_page>(*this, std::move(page_data));
                emit event_add(page, nullptr);
                emit page->event_load();
                emit event_load(page);

                break;
            }

            default:
                nxi_assert("unknown page type");
            }
        }

        // load page connections, move pages from nullptr to real source
        auto arcs = nxi::data::page::get_arcs(user_database_);
        while (arcs.next())
        {
            graph_.connect(get(arcs[nxi_model.page_arc.source_id]), get(arcs[nxi_model.page_arc.target_id]));
        }

        // set root
        // set_root(get(core_.session().config().page.root.get()));
        set_root(page_root);

        // notify
        graph_.targets(root_, [this](auto&& page) { emit event_add(page, root_); });

        connect(&core_.session_system(), &nxi::session_system::event_focus, [this](nxi::session& s) { session_id_ = s.id(); });
    }

    page_system::page_ptr page_system::add_static(const QString& path, nxi::renderer_type renderer_type)
    {
        if (renderer_type == nxi::renderer_type::web) return add<nxi::page>(path, nxi::core::page_path(path), nxi::page_type::static_, renderer_type);
        else return add<nxi::page>(path, path, nxi::page_type::static_, renderer_type);
    }

    void page_system::open_static(const QString& path, nxi::renderer_type renderer_type)
    {
        auto added_page = add_static(path, renderer_type);
        load(added_page);
        focus(added_page);
    }

    template<>
    void page_system::open<nxi::web_page>()
    {
        open<nxi::web_page>(root_, core_.session_config().browser.home.get());
    }

    void page_system::close(nds::node_ptr<nxi::page> page)
    {
        if (!page)
        {
            nxi_warning("page_system::close with null page");
            return;
        }
        // last page remaining
        if (graph_.count_nodes() <= 1) return;

        page->close();
        emit event_close(page);

        bool had_focus = (focus_ == page);

        erase(page);

        if (had_focus)
        {
            // get the next page to focus
            nds::node_ptr<nxi::page> new_focus;
            graph_.targets(root_, [&new_focus](auto&& page) { new_focus = page; });
            if (!new_focus) new_focus = root_;
            focus(new_focus);
        }
    }

    void page_system::close_focus() { close(focus_); }

    void page_system::erase(nds::node_ptr<nxi::page> page)
    {
        nxi_assert(page);

        nxi::data::page::del_arc(user_database_, page->id());
        nxi::data::page::del(user_database_, *page);

        graph_.erase(page);
    }

    const page_system::page_ptr& page_system::focus() const { return focus_; }

    void page_system::focus(nxi::page_id id) { focus(get(id)); }

    void page_system::focus(nds::node_ptr<nxi::page> page)
    {
        nxi_assert(page);

        if (focus_ && page == focus_) return;
        focus_ = page;

        core_.session_system().focus(core_.session_system().get(page->session_id()));

        if (!focus_->is_loaded()) load(focus_);
        emit focus_->event_focus();
        emit event_focus(focus_);
    }

    page_system::page_ptr page_system::get(nxi::page_id id) const
    {
        nds::node_ptr<nxi::page> found_node;
        nds::algorithm::graph::find_first_if(
        graph_,
        [&id](auto&& node) { return node->id() == id; },
        [&found_node](auto&& node) { found_node = node; });

        nxi_assert(found_node);
        return found_node;
    }

    void page_system::load(nxi::page_id id) { load(get(id)); }

    void page_system::load(page_system::page_ptr page)
    {
        page->load();
        emit event_load(page);
    }

    page_system::pages_view page_system::pages() const
    {
        pages_view pages;
        graph_.nodes([&pages](auto&& node) { pages.push_back(node); });
        return pages;
    }

    page_system::pages_view page_system::root_targets() const
    {
        page_system::pages_view pages;
        graph_.targets(root_, [&pages](auto&& node) { pages.push_back(node); });
        return pages;
    }

    void page_system::search(const QString& search_string, std::function<void(nds::node_ptr<nxi::page>)> callback) const
    {
        auto core_id = core_.session().id();
        graph_.nodes([&callback, &search_string, core_id](auto&& node) {
            if (core_id == node->id() && node->name().toLower().contains(search_string)) callback(node);
        });
    }

    page_system::pages_view page_system::targets(nds::node_ptr<const nxi::page> source) const
    {
        pages_view pages;
        graph_.targets(source, [&pages](auto&& node) { pages.push_back(node); });
        return pages;
    }

    page_system::pages_view page_system::targets(nxi::page_id source) const
    {
        return targets(static_cast<nds::node_ptr<const nxi::page>>(get(source)));
    }

    void page_system::set_root(nds::node_ptr<nxi::page> page)
    {
        root_ = page;
        emit event_update_root(root_);
    }

    void page_system::move(nxi::page_id page_id, nxi::page_id source_id, nxi::page_id target_id)
    {

        // nxi_log << "PS MOVE : " << page_id << source_id << target_id;

        // page doesn't move
        if (source_id == target_id) return;

        // ndb::query<dbs::core>() << (
        // ndb::set(pc.source_id = target_id, pc.target_id = page_id)
        //<< // ndb::filter(pc.source_id == source_id && pc.target_id == page_id)
        //);

        emit event_move(page_id, source_id, target_id);
    }

    void page_system::update(nxi::page_id id)
    {
        // emit event_update(page_.at(id));
    }

    int page_system::session_id() const { return session_id_; }
} // nxi