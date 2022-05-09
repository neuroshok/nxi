#include <nxi/system/context.hpp>

#include <nxi/core.hpp>
#include <nxi/data/context.hpp>
#include <nxi/database.hpp>
#include <nxi/database/result.hpp>
#include <nxi/user.hpp>

#include <stz/observer_ptr.hpp>
#include <QDebug>

//! context example
//! page 1
//! command 1
//! command_executor 9
//! focus : first vector element (page)
//! active : elements with higher priority (command_executor)
//! activate

namespace nxi
{
    context_system::context_system(nxi::core& core, nxi::database& user_database)
        : core_{ core }
        , user_database_{ user_database }
    {}

    void context_system::load()
    {
        // load available contexts
        auto result = nxi::data::context::get_available(core_.user_database());

        if (result.size() == 0)
        {
            add_available(nxi::contexts::page::ID, 1);
            add_available(nxi::contexts::command::ID, 1);
        }

        while (result.next())
        {
            auto data = nxi::data::context::from_get(result);
            available_contexts_.push_back(std::move(data));
            // add contexts
            auto& last_data = available_contexts_.back();
            if (last_data.active)
            {
                add(last_data.name, last_data.priority);
            }
        }

        nxi_trace("{} contexts available", available_contexts_.size());

        if (contexts_.empty())
        {
            add<nxi::contexts::command>();
            add<nxi::contexts::page>();
        }
    }

    void context_system::add_available(const QString& id, unsigned int priority)
    {
        nxi::context_data data{ id, priority };
        available_contexts_.push_back(std::move(data));
        nxi::data::context::add_available(core_.user_database(), available_contexts_.back());
    }

    void context_system::add(const QString& id, unsigned int priority)
    {
        if (std::find_if(available_contexts_.begin(), available_contexts_.end(), [&id](const auto& c) { return c.name == id; }) !=
            available_contexts_.end())
        {
            if (id == nxi::contexts::command::ID) add<nxi::contexts::command>(priority);
            else if (id == nxi::contexts::page::ID) add<nxi::contexts::page>(priority);
            else add<nxi::contexts::custom>(id, priority);
        }
        else nxi_warning("context {} is not available", id);
    }

    void context_system::del(const QString& id)
    {
        nxi::context* focus_context = nullptr;
        if (contexts_.size() > 0) focus_context = contexts_.front().get();

        // if context exist, erase
        auto it = std::find_if(contexts_.begin(), contexts_.end(), [&id](const auto& c) { return c->id() == id; });
        if (it != contexts_.end())
        {
            emit event_context_del(**it);
            nxi::data::context::del(core_.user_database(), **it);
            contexts_.erase(it);
        }
        else nxi_warning("context {} not found", id);

        std::sort(contexts_.begin(), contexts_.end(), [](const auto& c1, const auto& c2) { return c1->priority() > c2->priority(); });

        if (focus_context != contexts_.front().get()) emit event_focus_context_update(*contexts_.front());
        // if priority change, active contexts have changed
        // if (focus_context && focus_context->priority() != contexts_.front()->priority())
    }

    unsigned int context_system::active_priority() const { return contexts_.front()->priority(); }

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

    void context_system::focus(const QString& id)
    {
        auto context_it = std::find_if(contexts_.begin(), contexts_.end(), [&id](const auto& c) { return c->id() == id; });
        if (context_it == contexts_.end())
        {
            nxi_warning("context {} not found", id);
            return;
        }
        if ((*context_it)->priority() < active_priority())
        {
            nxi_warning("unable to focus context {}, it has lower priority", id);
            return;
        }
        std::iter_swap(contexts_.begin(), context_it);
        emit event_focus_context_update(*contexts_.front());
    }
} // nxi