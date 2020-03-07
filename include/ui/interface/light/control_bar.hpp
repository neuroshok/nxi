#ifndef INCLUDE_UI_INTERFACE_LIGHT_CONTROL_BAR_HPP_NXI
#define INCLUDE_UI_INTERFACE_LIGHT_CONTROL_BAR_HPP_NXI

#include <ui/interface.hpp>

class QLabel;

#include <QLabel>
#include <QWheelEvent>

namespace ui
{
    class core;
    class command;
} // ui

namespace ui::interfaces::light
{
    struct node_button : public QLabel
    {
        Q_OBJECT
        using QLabel::QLabel;
        void enterEvent(QEvent* event) override { emit event_enter(); }
        void leaveEvent(QEvent* event) override { emit event_leave(); }

        void wheelEvent(QWheelEvent* event) override { if (event->delta() < 0) emit event_down(); else emit event_up(); }

        signals:
        void event_enter();
        void event_leave();
        void event_up();
        void event_down();
    };

    class control_bar : public ui::interface
    {
    public:
        explicit control_bar(ui::core& ui_core, ui::window* window);

        void enterEvent(QEvent* event) override;
        void leaveEvent(QEvent* event) override;

        ui::command* command_input();

    private:
        ui::core& ui_core_;

        ui::command* command_input_;
        node_button* command_root_;
        node_button* page_root_;
    };
} // ui::interfaces::light

#endif // INCLUDE_UI_INTERFACE_LIGHT_CONTROL_BAR_HPP_NXI