#ifndef INCLUDE_UI_COMMAND_MENU_HPP_NXI
#define INCLUDE_UI_COMMAND_MENU_HPP_NXI

#include <nxi/command/fwd.hpp>
#include <nxi/suggestion/vector.hpp>
#include <stz/observer_ptr.hpp>
#include <ui/element.hpp>

#include <QColor>
#include <QMovie>
#include <QWidget>

namespace nxi
{
    class command;
    class command_input;
    class page;
} // nxi

namespace ui
{
    class user_session;
    class menu_item;

    class command_menu : public ui::basic_element<QWidget>
    {
        Q_OBJECT

    public:
        struct style_type
        {
            QPixmap background_image;
            QColor background_color{ 200, 200, 200 };
            QColor item_text_color{ 0, 0, 0 };
            QColor item_text_color_hover{ 0, 0, 0 };
            QColor item_background_color{ 0, 0, 150 };
            QColor item_background_color_hover{ 150, 150, 150 };
            QColor item_background_color_selected{ 100, 100, 200 };

            int header_height{ 24 };
            int header_item_width{ 128 };

            int item_height{ 24 };
            int item_text_size{ 18 };

            int control_padding{ 4 };
        } style_data;

    public:
        command_menu(ui::user_session&, QWidget* parent);

        void set_data(stz::observer_ptr<nxi::commands_view>);
        void set_data(stz::observer_ptr<const nxi::suggestion_vector>);

        void exec();

        nxi::command_input& nxi_input();

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
        void draw_item(const nxi::suggestion& text, QRect& item_rect, bool selected);

        nxi::suggestion_vector& suggestions();

        void draw_pixmap(QPainter&, const QPixmap&, int x, int y, QSize size, int margin);

    private:
        ui::user_session& session_;

        int header_item_index_;
        int hover_index_;
        int selection_index_;

        QRect page_command_area_;
        QRect page_mute_area_;

        stz::observer_ptr<const nxi::commands_view> commands_;

        stz::observer_ptr<const nxi::suggestion_vector> suggestions_;

        // controls
        QPixmap icon_copy_;
        QPixmap icon_sound_;
        QPixmap icon_sound_muted_;
        QPixmap icon_sound_premuted_;
    };
} // ui

#endif // INCLUDE_UI_COMMAND_MENU_HPP_NXI