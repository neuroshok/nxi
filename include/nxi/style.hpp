#ifndef INCLUDE_NXI_STYLE_HPP_NXI
#define INCLUDE_NXI_STYLE_HPP_NXI

class QString;
class QWidget;
class QPalette;

namespace ui
{
    class command_input;
    class command_menu;
    class main_interface;

    namespace interfaces
    {
        class main;
        class page_bar;
        class content;
        class control_bar;
    } // interfaces
} // ui

namespace nxw { class menu; }

namespace w3c { struct theme; }

#include <nxi/style_data.hpp>

#include <QProxyStyle>
#include <ui/interface/main.hpp>

namespace nxi
{

} // nxi

namespace nxi
{
    class style : public QProxyStyle
    {
    public:
        style();
        style(const QString& name);

        void load();
        const QString& name() const;
        const QString& path() const;
        const nxi::style_data& data() const;

        void update(QWidget*) const;
        void update(ui::command_input*) const;
        void update(ui::main_interface*) const;

        void update(ui::command_menu*);

        void polish(QPalette& palette) override;
        void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const override;

    private:
        void from_w3c(w3c::theme&);

    private:
        QString name_;
        QString path_;
        nxi::style_data data_;
    };
} // nxi

#endif // INCLUDE_NXI_STYLE_HPP_NXI
