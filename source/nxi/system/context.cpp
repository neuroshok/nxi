#include <nxi/system/context.hpp>

#include <nxi/database.hpp>

#include <QDebug>

namespace nxi
{
    context_system::context_system(nxi::core& nxi_core)
        : nxi_core_{ nxi_core }
    {}

    void context_system::load()
    {
        // load available contexts
        ndb::load_bulk(available_contexts_);
        if (available_contexts_.size() == 0)
        {
            reset();
            ndb::load_bulk(available_contexts_);
        }
        nxi_trace("{} contexts available", available_contexts_.size());

        // add contexts
        add<nxi::contexts::command>();
        add<nxi::contexts::page>();
        //focus(contexts.command);
    }

    void context_system::reset()
    {
        available_context ac;
        ac.name = nxi::contexts::command::ID;
        ac.priority = nxi::contexts::command::PRIORITY;
        ndb::store(ac);
        ac.name = nxi::contexts::page::ID;
        ac.priority = nxi::contexts::page::PRIORITY;
        ndb::store(ac);

    }

    void context_system::add(const QString& id)
    {
        /*
        for (const auto& ctx : available_contexts_)
        {
            if (ctx.name == id) add
        }*/
    }

    std::vector<nxi::available_context> context_system::available_contexts()
    {
        std::vector<nxi::available_context> contexts;
        for (const auto& context : available_contexts_)
        {
            contexts.push_back(context);
        }
        return contexts;
    }

    unsigned int context_system::active_priority() const
    {
        return contexts_.front()->priority();
    }

    /*
    void context_system::add(const nxi::context& ctx)
    {
        //add(ctx, ctx.priority);
    }

    void context_system::add(nxi::context context, int priority)
    {
        context.priority = priority;
        contexts_.push_back(context);
        std::sort(contexts_.begin(), contexts_.end(), [](const nxi::context& c1, const nxi::context& c2){ return c1.priority > c2.priority; });
        emit event_context_update(contexts_.front());
    }

    void context_system::del(const nxi::context& context)
    {
        contexts_.erase(std::find_if(contexts_.begin(), contexts_.end(), [&context](const nxi::context& c) { return c.name == context.name; }));
        std::sort(contexts_.begin(), contexts_.end(), [](const nxi::context& c1, const nxi::context& c2){ return c1.priority > c2.priority; });
        emit event_context_update(contexts_.front());
    }

    void context_system::focus(const nxi::context& context)
    {
        auto context_it = std::find_if(contexts_.begin(), contexts_.end(), [&context](const nxi::context& c) { return c.name == context.name; });
        std::iter_swap(contexts_.begin(), context_it);
        //qDebug() << "focus: " << context.name;
    }

    bool context_system::is_active(const nxi::context& context) const
    {

    }*/
} // nxi