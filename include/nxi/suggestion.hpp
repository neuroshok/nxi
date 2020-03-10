#ifndef INCLUDE_NXI_SUGGESTION_HPP_NXI
#define INCLUDE_NXI_SUGGESTION_HPP_NXI

#include <nxi/command/fwd.hpp>
#include <nxi/utility.hpp>

#include <QString>

namespace nxi
{
    class page;

    enum class suggestion_type { text, command, page };

    class suggestion
    {
    public:
        suggestion(suggestion_type type);
        suggestion(QString text, QString icon = "", QString info = "");
        virtual ~suggestion() = default;

        virtual const QString& icon() const;
        virtual const QString& info() const;
        virtual const QString& text() const;
        suggestion_type type() const;

        template<class T>
        static suggestion_type get_type() { nxi_unreachable(); }

    private:
        suggestion_type type_;
        QString icon_;
        QString info_;
        QString text_;
    };

    template<> suggestion_type suggestion::get_type<nxi::command>();
    template<> suggestion_type suggestion::get_type<nxi::page>();
} // nxi

#endif // INCLUDE_NXI_SUGGESTION_HPP_NXI