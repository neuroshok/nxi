#include <platform/generic/window.hpp>

#include <QMouseEvent>

namespace platform::generic
{
    window::window()
        : moving_{ false }
    {
        setWindowFlag(Qt::Window);
        setWindowFlag(Qt::FramelessWindowHint);
        setWindowFlag(Qt::WindowSystemMenuHint);
    }

    void window::mousePressEvent(QMouseEvent* event)
    {
        if (event->button() == Qt::LeftButton)
        {
            moving_ = true;
            move_origin_ = event->globalPosition().toPoint() - pos();
        }
    }

    void window::mouseMoveEvent(QMouseEvent* event)
    {
        QPoint delta = event->globalPosition().toPoint() - move_origin_;
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