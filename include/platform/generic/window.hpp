#ifndef INCLUDE_PLATFORM_GENERIC_WINDOW_HPP_NXI
#define INCLUDE_PLATFORM_GENERIC_WINDOW_HPP_NXI

#include <QObject>
#include <QWidget>

namespace platform::generic
{
    class window : public QWidget
    {
        Q_OBJECT
    public:
        window();

        void mousePressEvent(QMouseEvent*) override;
        void mouseMoveEvent(QMouseEvent*) override;
        void mouseDoubleClickEvent(QMouseEvent*) override;

        void set_grip(QWidget* widget);
        void set_fullscreen();

    private:
        bool moving_;
        QPoint move_origin_;
    };
} // platform::generic

#endif // INCLUDE_PLATFORM_GENERIC_WINDOW_HPP_NXI