#ifndef INCLUDE_NXI_CONTEXT_HPP_NXI
#define INCLUDE_NXI_CONTEXT_HPP_NXI

#include <nds/cx/index_of.hpp>
#include <nds/graph/node.hpp>
#include <nxi/command/executor.hpp>
#include <nxi/utility.hpp>

#include <variant>
#include <QString>

namespace nxi
{
    class command;
    class command_executor;

    using context_id = QString;

    struct basic_context
    {
        basic_context(const QString id, QString name, unsigned int priority)
            : id_{ std::move(id) }
            , name_{ std::move(name) }
            , priority_{ priority }
        {}

        const nxi::context_id& id() const { return id_; }
        const QString& name() const { return name_; }
        unsigned int priority() const { return priority_; }

        nxi::context_id id_;
        QString name_;
        unsigned int priority_;
    };

    namespace contexts
    {
        struct custom : basic_context
        {
            static inline const QString ID = "custom_";
            static inline const int PRIORITY = 1;
            custom(QString name, unsigned int priority = PRIORITY) : basic_context(ID + name, name, priority) {}
        };

        struct page : basic_context
        {
            static inline const QString ID = "page";
            static inline const int PRIORITY = 1;
            page(unsigned int priority = PRIORITY) : basic_context(ID, "Page", priority) {}
        };

        struct command : basic_context
        {
            static inline const QString ID = "command";
            static inline const int PRIORITY = 1;
            command(unsigned int priority = PRIORITY) : basic_context(ID, "Command", priority) {}
        };

        struct command_executor : basic_context
        {
            static inline QString ID = "command_executor";
            command_executor(const nxi::command_executor& c, unsigned int priority = 9)
            : basic_context(ID, "Executing command", priority), data{ c } {}

            const nxi::command_executor& data;
        };
    } // contexts

    class context
    {
    public:
        using context_type = std::variant
            < contexts::custom
            , contexts::page
            , contexts::command
            , contexts::command_executor
            >;

        template<class... Ts>
        context(Ts&&... ts) : context_{ std::forward<Ts>(ts)... } {}

        template<class... Fs>
        decltype(auto) apply(Fs&&... fs) const
        {
            return std::visit(overloaded{ fs... }, context_);
        }

        const QString& id() const
        {
            return apply([](auto&& c) -> const QString& { return c.id(); });
        }

        const QString& name() const
        {
            return apply([](auto&& c) -> const QString& { return c.name(); });
        }

        unsigned int priority() const
        {
            return apply([](auto&& c) { return c.priority(); });
        }

        template<class Context>
        static const QString& id()
        {
            return Context::ID;
        }

        template<class Context>
        static QString id(const QString& id)
        {
            static_assert(std::is_same_v<Context, context::custom>);
            return Context::ID + id;
        }

        template<class Context_type, class Context>
        static bool is(const Context& context)
        {
            return context.apply(
            [](const Context_type& c) { return true; }
            , [](auto&& c) { return false; }
            );
        }

    private:
        context_type context_;
    };
} // nxi

#endif // INCLUDE_NXI_CONTEXT_HPP_NXI
