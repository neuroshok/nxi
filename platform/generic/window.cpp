#include <platform/generic/window.hpp>

#include <QMouseEvent>

namespace platform::generic
{
    window::window()
        : moving_{ false }
    {
        setWindowFlag(Qt::Window, true);
        setWindowFlag(Qt::FramelessWindowHint, true);
        setWindowFlag(Qt::WindowSystemMenuHint, true);
    }

    void window::mousePressEvent(QMouseEvent* event)
    {
        if (event->button() == Qt::LeftButton)
        {
            moving_ = true;
            move_origin_ = event->globalPos() - pos();
        }
    }

    void window::mouseMoveEvent(QMouseEvent* event)
    {
        QPoint delta = event->globalPos() - move_origin_;
        if (moving_) move(delta.x(), delta.y());
    }

    void window::mouseDoubleClickEvent(QMouseEvent* event)
    {
        if (event->button() == Qt::LeftButton)
        {
            if (isMaximized()) showNormal();
            else showMaximized();
        }
    }

    void window::set_grip(QWidget* widget) {}
    void window::set_fullscreen() {}
} // platform::generic
