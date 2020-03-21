#ifndef INCLUDE_NXI_SYSTEM_CONTEXT_HPP_NXI
#define INCLUDE_NXI_SYSTEM_CONTEXT_HPP_NXI

#include <nxi/context.hpp>
#include <nxi/database.hpp>

#include <vector>
#include <stz/observer_ptr.hpp>

#include <QObject>

namespace nxi
{
    class core;

    struct available_context : ndb::object<dbs::core, ndb::objects::context_available>    {    };
    //using available_context = ndb::objects::context_available;

    class context_system : public QObject
    {
        Q_OBJECT
    public:
		context_system(nxi::core& );
		context_system(const context_system&) = delete;
        void operator=(const context_system&) = delete;

        void load();
        void reset();

        std::vector<nxi::available_context> available_contexts() const;
        std::vector<stz::observer_ptr<nxi::context>> contexts() const;

        unsigned int active_priority() const;

        void add(const QString& id);
        void del(const QString& id);

        template<class Context, class... Args>
        void add(Args&&... args)
        {
            nxi::context* focus_context = nullptr;
            if (contexts_.size() > 0) focus_context = contexts_.front().get();

            if (std::find_if(contexts_.begin(), contexts_.end(), [](const auto& c) { return c->id() == Context::ID; }) != contexts_.end())
            {
                nxi_warning("context already exist");
                return;
            }
            contexts_.emplace_back( std::make_unique<nxi::context>( Context{ std::forward<Args>(args)... } ) );

            emit event_context_add(*contexts_.back());

            std::sort(contexts_.begin(), contexts_.end(), [](const auto& c1, const auto& c2){ return c1->priority() > c2->priority(); });

            if (focus_context != contexts_.front().get()) emit event_focus_context_update(*contexts_.front());
        }

        template<class Context>
        void del()
        {
            nxi::context* focus_context = nullptr;
            if (contexts_.size() > 0) focus_context = contexts_.front().get();

            // if context exist, erase
            auto it = std::find_if(contexts_.begin(), contexts_.end(), [](const auto& c) { return nxi::context::is<Context>(*c); });
            if (it != contexts_.end())
            {
                emit event_context_del(**it);
                contexts_.erase(it);
            }

            std::sort(contexts_.begin(), contexts_.end(), [](const auto& c1, const auto& c2){ return c1->priority() > c2->priority(); });

            if (focus_context != contexts_.front().get()) emit event_focus_context_update(*contexts_.front());
            // if priority change, active contexts have changed
            //if (focus_context && focus_context->priority() != contexts_.front()->priority())
        }


        template<class... Fs>
        void apply_on_active(Fs&&... fs)
        {
            int active_priority = contexts_.front()->priority();
            for (const auto& context : contexts_)
            {
                if (context->priority() == active_priority) context->apply( std::forward<Fs>(fs)... );
                else break;
            }
        }

        template<class Context>
        bool is_active() const
        {
            int active_priority = contexts_.front()->priority();
            auto f = [active_priority](const std::unique_ptr<nxi::context>& context)
            {
                return context->apply([active_priority](const auto& ctx){ return std::is_same_v<std::decay_t<decltype(ctx)>, Context> && ctx.priority() == active_priority; });
            };
            if (std::find_if(contexts_.begin(), contexts_.end(), f) != contexts_.end()) return true;

            return false;
        }

    signals:
        void event_context_add(const nxi::context&) const;
        void event_context_del(const nxi::context&) const;
        void event_focus_context_update(const nxi::context&) const;
        //void event_active_context_update(std::vector<stz::observer_ptr<const nxi::context>) const;

    private:
		nxi::core& nxi_core_;

		std::vector<std::unique_ptr<nxi::context>> contexts_;
		std::vector<nxi::available_context> available_contexts_;
    };
} // nxi

#endif // INCLUDE_NXI_SYSTEM_CONTEXT_HPP_NXI
