#ifndef INCLUDE_UI_COMMAND_MENU_HPP_NXI
#define INCLUDE_UI_COMMAND_MENU_HPP_NXI

#include <stz/observer_ptr.hpp>
#include <vector>

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
    class core;
    class menu_item;

    class command_menu : public QWidget
    {
        Q_OBJECT
    public:
        struct style_type
        {
            QColor background_color{ 200, 200, 200 };
            QColor item_text_color{ 0, 0, 0 };
            QColor item_text_color_hover{ 0, 0, 0 };
            QColor item_background_color_hover{ 150, 150, 150 };
            QColor item_background_color_selected{ 100, 100, 200 };

            int item_height{ 24 };
            int item_text_size{ 18 };
        } style_data;

    public:
        command_menu(ui::core&, QWidget* parent);

        void set_data(std::vector<stz::observer_ptr<nxi::command>>);

        void exec();

    protected:
        void paintEvent(QPaintEvent*) override;

    private:
        void draw_item(stz::observer_ptr<nxi::command>, QRect&);

    private:
        ui::core& ui_core_;
        std::vector<stz::observer_ptr<nxi::command>> commands_;
        std::vector<stz::observer_ptr<nxi::page>> pages_;

        QMovie movie_;
    };
} // ui

#endif // INCLUDE_UI_COMMAND_MENU_HPP_NXI
