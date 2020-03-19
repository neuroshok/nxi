#include <nxi/system/context.hpp>

#include <QDebug>

namespace nxi
{
    context_system::context_system(nxi::core& nxi_core)
        : nxi_core_{ nxi_core }
    {}

    void context_system::load()
    {
        // add contexts
        add<nxi::context::command>();
        add<nxi::context::page>();
        //focus(contexts.command);
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