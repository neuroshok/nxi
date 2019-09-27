#ifndef INCLUDE_NXI_STYLE_HPP_NXI
#define INCLUDE_NXI_STYLE_HPP_NXI

class QString;
class QWidget;

namespace ui
{
    class command;
    namespace interfaces
    {
        class main;
        class page_bar;
        class content;
        class control_bar;
    } // interfaces
} // ui

#include <nxi/style_data.hpp>

#include <ui/view/page_tree.hpp>
#include <w3c/theme.hpp>

namespace nxi
{

} // nxi

namespace nxi
{
    class style
    {
    public:
        style();
        style(const QString& name);

        void load();

        void update(ui::views::page_tree*) const;
        void update(QWidget*) const;
        void update(ui::command*) const;
        void update(ui::interfaces::main*) const;
        void update(ui::interfaces::control_bar*) const;

        const nxi::style_data& data() const { return data_; }

    private:
        void from_w3c(w3c::theme&);

    private:
        QString name_;
        nxi::style_data data_;
    };
} // nxi

#endif // INCLUDE_NXI_STYLE_HPP_NXI
