#ifndef PLATFORM_GENERIC_WINDOW_HPP_NXI
#define PLATFORM_GENERIC_WINDOW_HPP_NXI

#include <QObject>
#include <QWidget>

namespace platform::generic
{
    class window : public QWidget
    {
        Q_OBJECT
    public:
        window();

        void mousePressEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent* event);
        void mouseDoubleClickEvent(QMouseEvent* event);

        void set_grip(QWidget* widget);
        void set_fullscreen();

    private:
        bool moving_;
        QPoint move_origin_;
    };
} // platform::generic

#endif // PLATFORM_GENERIC_WINDOW_HPP_NXI
