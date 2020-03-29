#include <ui/interface/light/button.hpp>

#include <QEvent>
#include <QWheelEvent>

namespace ui::interfaces::light
{
    button::button(const QString& name, QWidget* parent)
        : QLabel(parent)
    {
        setText(name);
        setStyleSheet("font-weight: bold; padding: 0 20 0 20; color:" + style_data.text_color.name() + "; background-color:" + style_data.background_color.name());
    }

    void button::enterEvent(QEvent* event)
    {
        emit event_enter();
    }

    void button::leaveEvent(QEvent* event)
    {
        emit event_leave();
    }

    void button::wheelEvent(QWheelEvent* event)
    {
        if (event->delta() < 0) emit event_mousewheel_down();
        else emit event_mousewheel_up();
    }

    void button::activate(bool n)
    {
        active_ = n;

        auto background_color = style_data.background_color;
        if (active_) background_color = style_data.background_color.lighter(20);

        setStyleSheet("font-weight: bold; padding: 0 20 0 20; color:" + style_data.text_color.name() + "; border-bottom: 2px solid " + background_color.name());
    }
} // ui::interfaces::light