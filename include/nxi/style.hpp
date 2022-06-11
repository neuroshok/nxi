#ifndef INCLUDE_NXI_STYLE_HPP_NXI
#define INCLUDE_NXI_STYLE_HPP_NXI

#include <nxi/style_data.hpp>

#include <QProxyStyle>

namespace ui
{
    class command_input;
    class command_menu;
    class main_interface;
    class user;

    namespace interfaces
    {
        class main;
        class content;
        class control_bar;
    } // interfaces
} // ui

namespace nxw
{
    class menu;
} // nxw
namespace w3c
{
    struct theme;
} // w3c

class QString;
class QWidget;
class QPalette;

namespace nxi
{
    class style : public QProxyStyle
    {
    public:
        style();
        explicit style(QString name);

        void load();
        const QString& name() const;
        [[nodiscard]] const QString& path() const;
        [[nodiscard]] const nxi::style_data& data() const;

        void update(ui::user&) const;

        void update(ui::command_input*) const;
        void update(ui::main_interface*) const;

        void update(ui::command_menu*) const;

        void polish(QPalette& palette) override;
        void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const override;

    private:
        void from_w3c(w3c::theme&);

    private:
        QString name_;
        QString path_;
        nxi::style_data data_;
    };
} // nxi

#endif // INCLUDE_NXI_STYLE_HPP_NXI