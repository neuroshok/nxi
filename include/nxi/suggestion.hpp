#ifndef INCLUDE_NXI_SUGGESTION_HPP_NXI
#define INCLUDE_NXI_SUGGESTION_HPP_NXI

#include <nds/graph/node.hpp>

#include <nxi/command.hpp>
#include <nxi/page.hpp>
#include <nxi/suggestion/text.hpp>
#include <nxi/suggestion/search.hpp>
#include <nxi/utility.hpp>

#include <variant>

#include <QString>

namespace nxi
{
    class page;

    class suggestion
    {
    public:
        using suggestion_type = std::variant
            < nds::node_ptr<const nxi::command>
            , nds::node_ptr<const nxi::page>
            , nxi::search_suggestion
            , nxi::text_suggestion
            >;

        template<class T>
        suggestion(T&& v) : suggestion_{ std::forward<T>(v) } {}

        const QString& icon() const;
        const QString& info() const;
        const QString& text() const;

        template<class F>
        decltype(auto) apply(F&& f) const
        {
            return std::visit(overloaded
            {
                [&f](auto&& s) -> decltype(auto) { return f(s); }
            }, suggestion_);
        }

        template<class... Fs>
        decltype(auto) visit(Fs&&... fs) const
        {
            return std::visit(overloaded{ fs... }, suggestion_);
        }

    private:
        /*
        template<class T>
        const QString& icon(T&& data) const;*/

        const QString& icon(nds::node_ptr<const nxi::command> data) const { return data->icon(); }
        const QString& info(nds::node_ptr<const nxi::command> data) const { return data->description(); }
        const QString& text(nds::node_ptr<const nxi::command> data) const { return data->name(); }

        const QString& icon(nds::node_ptr<const nxi::page> data) const { return data->name(); }
        const QString& info(nds::node_ptr<const nxi::page> data) const { return data->command(); }
        const QString& text(nds::node_ptr<const nxi::page> data) const { return data->name(); }

        const QString& icon(const nxi::text_suggestion& data) const { return data.icon(); }
        const QString& info(const nxi::text_suggestion& data) const { return data.info(); }
        const QString& text(const nxi::text_suggestion& data) const { return data.text(); }

        const QString& icon(const nxi::search_suggestion& data) const { return data.icon(); }
        const QString& info(const nxi::search_suggestion& data) const { return data.name(); }
        const QString& text(const nxi::search_suggestion& data) const { return data.text(); }

    private:
        suggestion_type suggestion_;
    };
} // nxi

#endif // INCLUDE_NXI_SUGGESTION_HPP_NXI