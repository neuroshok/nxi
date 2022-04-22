#ifndef INCLUDE_UI_COMMAND_MENU_HPP_NXI
#define INCLUDE_UI_COMMAND_MENU_HPP_NXI

#include <nxi/command/fwd.hpp>
#include <nxi/suggestion/vector.hpp>
#include <stz/observer_ptr.hpp>

#include <QColor>
#include <QMovie>
#include <QWidget>

namespace nxi
{
    class command;
    class page;
} // nxi

namespace ui
{
    class session;
    class menu_item;

    class command_menu : public QWidget
    {
        Q_OBJECT

    public:
        struct style_type
        {
            QImage background_image;
            QColor background_color{ 200, 200, 200 };
            QColor item_text_color{ 0, 0, 0 };
            QColor item_text_color_hover{ 0, 0, 0 };
            QColor item_background_color_hover{ 150, 150, 150 };
            QColor item_background_color_selected{ 100, 100, 200 };

            int item_height{ 24 };
            int item_text_size{ 18 };
        } style_data;

    public:
        command_menu(ui::session&, QWidget* parent);

        void set_data(stz::observer_ptr<nxi::commands_view>);
        void set_data(stz::observer_ptr<const nxi::suggestion_vector>);

        void exec();

    protected:
        void leaveEvent(QEvent*) override;
        void mouseMoveEvent(QMouseEvent*) override;
        void mousePressEvent(QMouseEvent*) override;
        void paintEvent(QPaintEvent*) override;
        void wheelEvent(QWheelEvent*) override;

    private:
        void draw_header();
        void draw_item(nds::node_ptr<const nxi::command>, QRect&, bool selected);
        void draw_item(nds::node_ptr<const nxi::page>, QRect&, bool selected);
        void draw_item(const nxi::suggestion& text,  QRect& item_rect, bool selected);

        nxi::suggestion_vector& suggestions();

    private:
        ui::session& session_;

        int hover_index_;
        int selection_index_;

        stz::observer_ptr<const nxi::commands_view> commands_;
        stz::observer_ptr<const nxi::suggestion_vector> suggestions_;

        QMovie movie_;
    };
} // ui

#endif // INCLUDE_UI_COMMAND_MENU_HPP_NXI