#include <nxi/system/context.hpp>

#include <nxi/database.hpp>

#include <QDebug>
#include <include/stz/observer_ptr.hpp>

namespace nxi
{
    context_system::context_system(nxi::session& session)
        : session_{ session }
    {}

    void context_system::load()
    {
        // load available contexts
        // ndb::load_bulk(available_contexts_);
        if (available_contexts_.size() == 0)
        {
            reset();
            // ndb::load_bulk(available_contexts_);
        }
        nxi_trace("{} contexts available", available_contexts_.size());

        // add contexts
        add<nxi::contexts::command>();
        //add<nxi::contexts::page>();
        //focus(contexts.command);
    }

    void context_system::reset()
    {
        nxi::context_data ac;
        ac.name = nxi::contexts::command::ID;
        ac.priority = nxi::contexts::command::PRIORITY;
        // ndb::store(ac);
        ac.name = nxi::contexts::page::ID;
        ac.priority = nxi::contexts::page::PRIORITY;
        // ndb::store(ac);

    }

    void context_system::add(const QString& id)
    {
        for (const auto& ctx : available_contexts_)
        {
            if (ctx.name == id)
            {
                if (ctx.name == nxi::contexts::command::ID) add<nxi::contexts::command>();
                else if (ctx.name == nxi::contexts::page::ID) add<nxi::contexts::page>();
                break;
            }
        }
    }

    void context_system::del(const QString& id)
    {
        for (const auto& ctx : available_contexts_)
        {
            if (ctx.name == id)
            {
                if (ctx.name == nxi::contexts::command::ID) del<nxi::contexts::command>();
                else if (ctx.name == nxi::contexts::page::ID) del<nxi::contexts::page>();
                break;
            }
        }
    }

    std::vector<nxi::context_data> context_system::available_contexts() const
    {
        std::vector<nxi::context_data> contexts;
        for (const auto& context : available_contexts_)
        {
            contexts.push_back(context);
        }
        return contexts;
    }

    std::vector<stz::observer_ptr<nxi::context>> context_system::contexts() const
    {
        std::vector<stz::observer_ptr<nxi::context>> contexts;
        for (const auto& context : contexts_)
        {
            contexts.push_back(stz::make_observer(context.get()));
        }
        return contexts;
    }

    unsigned int context_system::active_priority() const
    {
        return contexts_.front()->priority();
    }

    /*
    void context_system::focus(const nxi::context& context)
    {
        auto context_it = std::find_if(contexts_.begin(), contexts_.end(), [&context](const nxi::context& c) { return c.name == context.name; });
        std::iter_swap(contexts_.begin(), context_it);
        //qDebug() << "focus: " << context.name;
    }*/
} // nxi