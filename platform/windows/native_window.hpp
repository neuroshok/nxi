#ifndef PLATFORM_WINDOWS_NATIVE_WINDOW_HPP_NXI
#define PLATFORM_WINDOWS_NATIVE_WINDOW_HPP_NXI


#include <QObject>
#include <QWidget>

namespace platform::windows
{
    class native_window : public QWidget
    {
        Q_OBJECT
    public:
        explicit native_window(QWidget* parent = 0);
        void set_grip(QWidget* widget);
:
        void setContentsMargins(const QMargins &margins);
        void setContentsMargins(int left, int top, int right, int bottom);
        QMargins contentsMargins() const;
        QRect contentsRect() const;
        void getContentsMargins(int *left, int *top, int *right, int *bottom) const;

        void set_fullscreen();

    protected:
        bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

    private:
        QWidget* grip_;
        bool mouse_down_;

        QMargins m_margins;
        QMargins m_frames;
        bool m_bJustMaximized;
    };
} // platform

#endif // PLATFORM_WINDOWS_NATIVE_WINDOW_HPP_NXI
