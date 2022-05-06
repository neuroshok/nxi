#ifndef INCLUDE_NXI_SYSTEM_CONTEXT_HPP_NXI
#define INCLUDE_NXI_SYSTEM_CONTEXT_HPP_NXI

#include <nxi/context.hpp>
#include <nxi/data/context.hpp>
#include <nxi/database.hpp>
#include <nxi/log.hpp>

#include <stz/observer_ptr.hpp>
#include <memory>
#include <vector>

#include <QObject>

namespace nxi
{
    class core;
    class database;

    class context_system : public QObject
    {
        Q_OBJECT
    public:
        context_system(nxi::core&, nxi::database&);
        context_system(const context_system&) = delete;
        context_system& operator=(const context_system&) = delete;

        void load();

        void add(const QString& id, unsigned int priority);
        void add_available(const QString& id, unsigned int priority);

        template<class Context, class... Args>
        void add(Args&&... args)
        {
            nxi::context* focus_context = nullptr;
            auto context_id = nxi::context::id<Context>(std::forward<Args>(args)...);
            if (contexts_.size() > 0) focus_context = contexts_.front().get();

            if (std::find_if(contexts_.begin(), contexts_.end(), [&context_id](const auto& c) { return c->id() == context_id; }) != contexts_.end())
            {
                nxi_warning("context already exist");
                return;
            }
            contexts_.emplace_back(std::make_unique<nxi::context>(Context{ std::forward<Args>(args)... }));

            nxi::data::context::add(user_database_, *contexts_.back());
            emit event_context_add(*contexts_.back());

            std::sort(contexts_.begin(), contexts_.end(), [](const auto& c1, const auto& c2) { return c1->priority() > c2->priority(); });

            if (focus_context != contexts_.front().get()) emit event_focus_context_update(*contexts_.front());
        }

        template<class... Fs>
        void apply_on_active(Fs&&... fs)
        {
            int active_priority = contexts_.front()->priority();
            for (const auto& context : contexts_)
            {
                if (context->priority() == active_priority) context->apply(std::forward<Fs>(fs)...);
                else break;
            }
        }

        template<class... Fs>
        void apply_on_focus(Fs&&... fs)
        {
            const auto& focus = contexts_.front();
            focus->apply(std::forward<Fs>(fs)...);
        }

        template<class Context>
        void del()
        {
            del(Context::ID);
        }
        void del(const QString& id);

        template<class Context, class... Args>
        void focus(Args&&... args)
        {
            focus(nxi::context::id<Context>(std::forward<Args>(args)...));
        }
        void focus(const QString& id);

        unsigned int active_priority() const;
        std::vector<nxi::context_data> available_contexts() const;
        std::vector<stz::observer_ptr<nxi::context>> contexts() const;

        template<class Context>
        bool is_active() const
        {
            int active_priority = contexts_.front()->priority();
            auto f = [active_priority](const std::unique_ptr<nxi::context>& context) {
                return context->apply([active_priority](const auto& ctx) {
                    return std::is_same_v<std::decay_t<decltype(ctx)>, Context> && ctx.priority() == active_priority;
                });
            };
            if (std::find_if(contexts_.begin(), contexts_.end(), f) != contexts_.end()) return true;

            return false;
        }

        template<class Context, class... Args>
        bool is_focus(Args&&... args) const
        {
            return contexts_.front()->id() == nxi::context::id<Context>(std::forward<Args>(args)...);
        }

    signals:
        void event_context_add(const nxi::context&) const;
        void event_context_del(const nxi::context&) const;
        void event_focus_context_update(const nxi::context&) const;
        // void event_active_context_update(std::vector<stz::observer_ptr<const nxi::context>) const;

    private:
        nxi::core& core_;
        nxi::database& user_database_;

        std::vector<std::unique_ptr<nxi::context>> contexts_;
        std::vector<nxi::context_data> available_contexts_;
    };
} // nxi

#endif // INCLUDE_NXI_SYSTEM_CONTEXT_HPP_NXI