#include <nxi/system/page.hpp>

#include <nxi/core.hpp>
#include <nxi/page/node.hpp>
#include <nxi/page/web.hpp>

#include <nxi/database.hpp>

#include <nxi/log.hpp>
#include <include/nxi/page/custom.hpp>

namespace nxi
{
    page_system::page_system(nxi::core& nxi_core) :
            nxi_core_{ nxi_core }
    {}

    void page_system::load()
    {
        // nxi::message_system::send(nxi::messages::page_system_loaded)
        nxi_trace("");

        // load pages, connect all pages to root
        for (auto& page_data : ndb::oget<dbs::core>(nxi_model.page))
        {
            auto page_id = page_data.oid;
            auto page_type = page_data.type;

            // make_page(page_type, args...)
            switch(page_type)
            {
                case page_type::node:

                    break;

                case page_type::web:

                    break;

                case page_type::explorer:
                    //page = std::make_unique<nxi::explorer_page>(*this, page_id);
                    break;

                case page_type::custom:

                    break;

                default:
                    nxi_assert("unknown page type");
            }
        }

        // load page connections, move pages from root to real source
        for (auto& line : ndb::oget<dbs::core>(nxi_model.page_connection))
        {
            //page_connections_.emplace(line.source_id, line.target_id);
            // if source_id == 0, page didn't move
            if (line.source_id != 0) emit event_move(line.target_id, 0, line.source_id);
        }
    }


    void page_system::load(nxi::page_id id)
    {
        /*auto& current_page = static_cast<nxi::web_page&>(page_[m_current_index]);
        current_page.url = page.url;
        emit event_load(current_page);*/
    }

    void page_system::load(nxi::web_page& page)
    {
        // set loaded
        emit event_load(page);
    }

    nxi::page& page_system::add_static(const QString& path, nxi::renderer_type renderer_type)
    {
        if (renderer_type == nxi::renderer_type::web) return add<nxi::page>(0, path, nxi::core::page_path(path), nxi::page_type::static_ , renderer_type);
        else return add<nxi::page>(0, path, path, nxi::page_type::static_ , renderer_type);
    }

    void page_system::open_static(const QString& path, nxi::renderer_type renderer_type)
    {
        nxi::page& added_page = add_static(path, renderer_type);
        added_page.load();
        added_page.focus();
    }

    const page_system::pages_view& page_system::get() const
    {
        return pages_view_;
    }

    const page_system::page_connections_type& page_system::connections() const
    {
        return page_connections_;
    }

    nxi::page& page_system::get(nxi::page_id id) const
    {
        auto page_it = pages_.find(id);
        nxi_assert(page_it != pages_.end());

        return *page_it->second;
    }

    page_system::pages_view page_system::list(const nxi::page& source)
    {
        const auto& pc = nxi_model.page_connection;

        page_system::pages_view result;
        for (auto& page : ndb::query<dbs::core>() << (ndb::get(pc.target_id) << ndb::source(pc) << ndb::filter(pc.source_id = source.id())))
        {
            result.push_back(stz::make_observer(&get(page[pc.target_id])));
        }

        return result;
    }

    void page_system::focus(nxi::web_page& page)
    {
        nxi_trace_event("nxi::page_system::event_focus");
        focus_ = stz::make_observer<nxi::page>(&page);
        emit event_focus(page);
        emit event_focus(
        static_cast<nxi::page&>(page));
    }
    void page_system::focus(nxi::custom_page& page) { emit event_focus(page); emit event_focus(static_cast<nxi::page&>(page)); }
    void page_system::focus(nxi::page_node& node) { emit event_focus(node); }

    void page_system::focus(nxi::page_id id)
    {
        //nxi_log << "FOCUS " << id;
        current_page_ = &get(id);
        current_page_->focus();
        // emit event_change(static_cast<Page*>(current_page_));
        //emit event_focus(*current_page_);
    }

    void page_system::move(nxi::page_id page_id, nxi::page_id source_id, nxi::page_id target_id)
    {
        const auto& pc = nxi_model.page_connection;

        //nxi_log << "PS MOVE : " << page_id << source_id << target_id;

        // page doesn't move
        if (source_id == target_id) return;

        ndb::query<dbs::core>() << (
            ndb::set(pc.source_id = target_id, pc.target_id = page_id)
            << ndb::filter(pc.source_id == source_id && pc.target_id == page_id)
        );


        emit event_move(page_id, source_id, target_id);
    }

    void page_system::update(nxi::page_id id)
    {
        //emit event_update(page_.at(id));
    }
} // nxi